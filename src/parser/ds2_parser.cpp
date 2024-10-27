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
  YYSYMBOL_DAS_DEFAULT = 68,               /* "default"  */
  YYSYMBOL_DAS_UNINITIALIZED = 69,         /* "uninitialized"  */
  YYSYMBOL_DAS_TBOOL = 70,                 /* "bool"  */
  YYSYMBOL_DAS_TVOID = 71,                 /* "void"  */
  YYSYMBOL_DAS_TSTRING = 72,               /* "string"  */
  YYSYMBOL_DAS_TAUTO = 73,                 /* "auto"  */
  YYSYMBOL_DAS_TINT = 74,                  /* "int"  */
  YYSYMBOL_DAS_TINT2 = 75,                 /* "int2"  */
  YYSYMBOL_DAS_TINT3 = 76,                 /* "int3"  */
  YYSYMBOL_DAS_TINT4 = 77,                 /* "int4"  */
  YYSYMBOL_DAS_TUINT = 78,                 /* "uint"  */
  YYSYMBOL_DAS_TBITFIELD = 79,             /* "bitfield"  */
  YYSYMBOL_DAS_TUINT2 = 80,                /* "uint2"  */
  YYSYMBOL_DAS_TUINT3 = 81,                /* "uint3"  */
  YYSYMBOL_DAS_TUINT4 = 82,                /* "uint4"  */
  YYSYMBOL_DAS_TFLOAT = 83,                /* "float"  */
  YYSYMBOL_DAS_TFLOAT2 = 84,               /* "float2"  */
  YYSYMBOL_DAS_TFLOAT3 = 85,               /* "float3"  */
  YYSYMBOL_DAS_TFLOAT4 = 86,               /* "float4"  */
  YYSYMBOL_DAS_TRANGE = 87,                /* "range"  */
  YYSYMBOL_DAS_TURANGE = 88,               /* "urange"  */
  YYSYMBOL_DAS_TRANGE64 = 89,              /* "range64"  */
  YYSYMBOL_DAS_TURANGE64 = 90,             /* "urange64"  */
  YYSYMBOL_DAS_TBLOCK = 91,                /* "block"  */
  YYSYMBOL_DAS_TINT64 = 92,                /* "int64"  */
  YYSYMBOL_DAS_TUINT64 = 93,               /* "uint64"  */
  YYSYMBOL_DAS_TDOUBLE = 94,               /* "double"  */
  YYSYMBOL_DAS_TFUNCTION = 95,             /* "function"  */
  YYSYMBOL_DAS_TLAMBDA = 96,               /* "lambda"  */
  YYSYMBOL_DAS_TINT8 = 97,                 /* "int8"  */
  YYSYMBOL_DAS_TUINT8 = 98,                /* "uint8"  */
  YYSYMBOL_DAS_TINT16 = 99,                /* "int16"  */
  YYSYMBOL_DAS_TUINT16 = 100,              /* "uint16"  */
  YYSYMBOL_DAS_TTUPLE = 101,               /* "tuple"  */
  YYSYMBOL_DAS_TVARIANT = 102,             /* "variant"  */
  YYSYMBOL_DAS_GENERATOR = 103,            /* "generator"  */
  YYSYMBOL_DAS_YIELD = 104,                /* "yield"  */
  YYSYMBOL_DAS_SEALED = 105,               /* "sealed"  */
  YYSYMBOL_ADDEQU = 106,                   /* "+="  */
  YYSYMBOL_SUBEQU = 107,                   /* "-="  */
  YYSYMBOL_DIVEQU = 108,                   /* "/="  */
  YYSYMBOL_MULEQU = 109,                   /* "*="  */
  YYSYMBOL_MODEQU = 110,                   /* "%="  */
  YYSYMBOL_ANDEQU = 111,                   /* "&="  */
  YYSYMBOL_OREQU = 112,                    /* "|="  */
  YYSYMBOL_XOREQU = 113,                   /* "^="  */
  YYSYMBOL_SHL = 114,                      /* "<<"  */
  YYSYMBOL_SHR = 115,                      /* ">>"  */
  YYSYMBOL_ADDADD = 116,                   /* "++"  */
  YYSYMBOL_SUBSUB = 117,                   /* "--"  */
  YYSYMBOL_LEEQU = 118,                    /* "<="  */
  YYSYMBOL_SHLEQU = 119,                   /* "<<="  */
  YYSYMBOL_SHREQU = 120,                   /* ">>="  */
  YYSYMBOL_GREQU = 121,                    /* ">="  */
  YYSYMBOL_EQUEQU = 122,                   /* "=="  */
  YYSYMBOL_NOTEQU = 123,                   /* "!="  */
  YYSYMBOL_RARROW = 124,                   /* "->"  */
  YYSYMBOL_LARROW = 125,                   /* "<-"  */
  YYSYMBOL_QQ = 126,                       /* "??"  */
  YYSYMBOL_QDOT = 127,                     /* "?."  */
  YYSYMBOL_QBRA = 128,                     /* "?["  */
  YYSYMBOL_LPIPE = 129,                    /* "<|"  */
  YYSYMBOL_LBPIPE = 130,                   /* " <|"  */
  YYSYMBOL_LLPIPE = 131,                   /* "$ <|"  */
  YYSYMBOL_LAPIPE = 132,                   /* "@ <|"  */
  YYSYMBOL_LFPIPE = 133,                   /* "@@ <|"  */
  YYSYMBOL_RPIPE = 134,                    /* "|>"  */
  YYSYMBOL_CLONEEQU = 135,                 /* ":="  */
  YYSYMBOL_ROTL = 136,                     /* "<<<"  */
  YYSYMBOL_ROTR = 137,                     /* ">>>"  */
  YYSYMBOL_ROTLEQU = 138,                  /* "<<<="  */
  YYSYMBOL_ROTREQU = 139,                  /* ">>>="  */
  YYSYMBOL_MAPTO = 140,                    /* "=>"  */
  YYSYMBOL_COLCOL = 141,                   /* "::"  */
  YYSYMBOL_ANDAND = 142,                   /* "&&"  */
  YYSYMBOL_OROR = 143,                     /* "||"  */
  YYSYMBOL_XORXOR = 144,                   /* "^^"  */
  YYSYMBOL_ANDANDEQU = 145,                /* "&&="  */
  YYSYMBOL_OROREQU = 146,                  /* "||="  */
  YYSYMBOL_XORXOREQU = 147,                /* "^^="  */
  YYSYMBOL_DOTDOT = 148,                   /* ".."  */
  YYSYMBOL_MTAG_E = 149,                   /* "$$"  */
  YYSYMBOL_MTAG_I = 150,                   /* "$i"  */
  YYSYMBOL_MTAG_V = 151,                   /* "$v"  */
  YYSYMBOL_MTAG_B = 152,                   /* "$b"  */
  YYSYMBOL_MTAG_A = 153,                   /* "$a"  */
  YYSYMBOL_MTAG_T = 154,                   /* "$t"  */
  YYSYMBOL_MTAG_C = 155,                   /* "$c"  */
  YYSYMBOL_MTAG_F = 156,                   /* "$f"  */
  YYSYMBOL_MTAG_DOTDOTDOT = 157,           /* "..."  */
  YYSYMBOL_INTEGER = 158,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 159,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 160,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 161,    /* "unsigned long integer constant"  */
  YYSYMBOL_UNSIGNED_INT8 = 162,            /* "unsigned int8 constant"  */
  YYSYMBOL_FLOAT = 163,                    /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 164,                   /* "double constant"  */
  YYSYMBOL_NAME = 165,                     /* "name"  */
  YYSYMBOL_KEYWORD = 166,                  /* "keyword"  */
  YYSYMBOL_TYPE_FUNCTION = 167,            /* "type function"  */
  YYSYMBOL_BEGIN_STRING = 168,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 169,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 170,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 171,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 172,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 173,          /* "}"  */
  YYSYMBOL_END_OF_READ = 174,              /* "end of failed eader macro"  */
  YYSYMBOL_175_begin_of_code_block_ = 175, /* "begin of code block"  */
  YYSYMBOL_176_end_of_code_block_ = 176,   /* "end of code block"  */
  YYSYMBOL_177_end_of_expression_ = 177,   /* "end of expression"  */
  YYSYMBOL_SEMICOLON_CUR_CUR = 178,        /* ";}}"  */
  YYSYMBOL_SEMICOLON_CUR_SQR = 179,        /* ";}]"  */
  YYSYMBOL_SEMICOLON_SQR_SQR = 180,        /* ";]]"  */
  YYSYMBOL_COMMA_SQR_SQR = 181,            /* ",]]"  */
  YYSYMBOL_COMMA_CUR_SQR = 182,            /* ",}]"  */
  YYSYMBOL_183_ = 183,                     /* ','  */
  YYSYMBOL_184_ = 184,                     /* '='  */
  YYSYMBOL_185_ = 185,                     /* '?'  */
  YYSYMBOL_186_ = 186,                     /* ':'  */
  YYSYMBOL_187_ = 187,                     /* '|'  */
  YYSYMBOL_188_ = 188,                     /* '^'  */
  YYSYMBOL_189_ = 189,                     /* '&'  */
  YYSYMBOL_190_ = 190,                     /* '<'  */
  YYSYMBOL_191_ = 191,                     /* '>'  */
  YYSYMBOL_192_ = 192,                     /* '-'  */
  YYSYMBOL_193_ = 193,                     /* '+'  */
  YYSYMBOL_194_ = 194,                     /* '*'  */
  YYSYMBOL_195_ = 195,                     /* '/'  */
  YYSYMBOL_196_ = 196,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 197,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 198,               /* UNARY_PLUS  */
  YYSYMBOL_199_ = 199,                     /* '~'  */
  YYSYMBOL_200_ = 200,                     /* '!'  */
  YYSYMBOL_PRE_INC = 201,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 202,                  /* PRE_DEC  */
  YYSYMBOL_POST_INC = 203,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 204,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 205,                    /* DEREF  */
  YYSYMBOL_206_ = 206,                     /* '.'  */
  YYSYMBOL_207_ = 207,                     /* '['  */
  YYSYMBOL_208_ = 208,                     /* ']'  */
  YYSYMBOL_209_ = 209,                     /* '('  */
  YYSYMBOL_210_ = 210,                     /* ')'  */
  YYSYMBOL_211_ = 211,                     /* '$'  */
  YYSYMBOL_212_ = 212,                     /* '@'  */
  YYSYMBOL_213_ = 213,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 214,                 /* $accept  */
  YYSYMBOL_program = 215,                  /* program  */
  YYSYMBOL_top_level_reader_macro = 216,   /* top_level_reader_macro  */
  YYSYMBOL_optional_public_or_private_module = 217, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 218,              /* module_name  */
  YYSYMBOL_module_declaration = 219,       /* module_declaration  */
  YYSYMBOL_character_sequence = 220,       /* character_sequence  */
  YYSYMBOL_string_constant = 221,          /* string_constant  */
  YYSYMBOL_string_builder_body = 222,      /* string_builder_body  */
  YYSYMBOL_string_builder = 223,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 224, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 225,              /* expr_reader  */
  YYSYMBOL_226_1 = 226,                    /* $@1  */
  YYSYMBOL_options_declaration = 227,      /* options_declaration  */
  YYSYMBOL_require_declaration = 228,      /* require_declaration  */
  YYSYMBOL_keyword_or_name = 229,          /* keyword_or_name  */
  YYSYMBOL_require_module_name = 230,      /* require_module_name  */
  YYSYMBOL_require_module = 231,           /* require_module  */
  YYSYMBOL_is_public_module = 232,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 233,       /* expect_declaration  */
  YYSYMBOL_expect_list = 234,              /* expect_list  */
  YYSYMBOL_expect_error = 235,             /* expect_error  */
  YYSYMBOL_expression_label = 236,         /* expression_label  */
  YYSYMBOL_expression_goto = 237,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 238,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 239,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 240,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 241, /* expression_else_one_liner  */
  YYSYMBOL_expression_if_one_liner = 242,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 243,  /* expression_if_then_else  */
  YYSYMBOL_expression_for_loop = 244,      /* expression_for_loop  */
  YYSYMBOL_expression_unsafe = 245,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 246,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 247,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 248,    /* expression_with_alias  */
  YYSYMBOL_annotation_argument_value = 249, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 250, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 251, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 252,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 253, /* annotation_argument_list  */
  YYSYMBOL_metadata_argument_list = 254,   /* metadata_argument_list  */
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
  YYSYMBOL_267_2 = 267,                    /* $@2  */
  YYSYMBOL_expression_block = 268,         /* expression_block  */
  YYSYMBOL_expr_call_pipe = 269,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 270,           /* expression_any  */
  YYSYMBOL_expressions = 271,              /* expressions  */
  YYSYMBOL_expr_keyword = 272,             /* expr_keyword  */
  YYSYMBOL_optional_expr_list = 273,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_list_in_braces = 274, /* optional_expr_list_in_braces  */
  YYSYMBOL_type_declaration_no_options_list = 275, /* type_declaration_no_options_list  */
  YYSYMBOL_expression_keyword = 276,       /* expression_keyword  */
  YYSYMBOL_277_3 = 277,                    /* $@3  */
  YYSYMBOL_278_4 = 278,                    /* $@4  */
  YYSYMBOL_279_5 = 279,                    /* $@5  */
  YYSYMBOL_280_6 = 280,                    /* $@6  */
  YYSYMBOL_expr_pipe = 281,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 282,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 283,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 284,     /* new_type_declaration  */
  YYSYMBOL_285_7 = 285,                    /* $@7  */
  YYSYMBOL_286_8 = 286,                    /* $@8  */
  YYSYMBOL_expr_new = 287,                 /* expr_new  */
  YYSYMBOL_expression_break = 288,         /* expression_break  */
  YYSYMBOL_expression_continue = 289,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 290, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 291,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 292, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 293,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 294,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 295,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 296,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 297,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 298,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 299, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 300,           /* expression_let  */
  YYSYMBOL_expr_cast = 301,                /* expr_cast  */
  YYSYMBOL_302_9 = 302,                    /* $@9  */
  YYSYMBOL_303_10 = 303,                   /* $@10  */
  YYSYMBOL_304_11 = 304,                   /* $@11  */
  YYSYMBOL_305_12 = 305,                   /* $@12  */
  YYSYMBOL_306_13 = 306,                   /* $@13  */
  YYSYMBOL_307_14 = 307,                   /* $@14  */
  YYSYMBOL_expr_type_decl = 308,           /* expr_type_decl  */
  YYSYMBOL_309_15 = 309,                   /* $@15  */
  YYSYMBOL_310_16 = 310,                   /* $@16  */
  YYSYMBOL_expr_type_info = 311,           /* expr_type_info  */
  YYSYMBOL_expr_list = 312,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 313,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 314,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 315,            /* capture_entry  */
  YYSYMBOL_capture_list = 316,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 317,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 318,               /* expr_block  */
  YYSYMBOL_expr_full_block = 319,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 320, /* expr_full_block_assumed_piped  */
  YYSYMBOL_expr_numeric_const = 321,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 322,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe_right = 323,   /* expr_assign_pipe_right  */
  YYSYMBOL_expr_assign_pipe = 324,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 325,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 326,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 327,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 328,           /* func_addr_expr  */
  YYSYMBOL_329_17 = 329,                   /* $@17  */
  YYSYMBOL_330_18 = 330,                   /* $@18  */
  YYSYMBOL_331_19 = 331,                   /* $@19  */
  YYSYMBOL_332_20 = 332,                   /* $@20  */
  YYSYMBOL_expr_field = 333,               /* expr_field  */
  YYSYMBOL_334_21 = 334,                   /* $@21  */
  YYSYMBOL_335_22 = 335,                   /* $@22  */
  YYSYMBOL_expr_call = 336,                /* expr_call  */
  YYSYMBOL_expr = 337,                     /* expr  */
  YYSYMBOL_338_23 = 338,                   /* $@23  */
  YYSYMBOL_339_24 = 339,                   /* $@24  */
  YYSYMBOL_340_25 = 340,                   /* $@25  */
  YYSYMBOL_341_26 = 341,                   /* $@26  */
  YYSYMBOL_342_27 = 342,                   /* $@27  */
  YYSYMBOL_343_28 = 343,                   /* $@28  */
  YYSYMBOL_expr_mtag = 344,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 345, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 346,        /* optional_override  */
  YYSYMBOL_optional_constant = 347,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 348, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 349, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 350, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 351, /* struct_variable_declaration_list  */
  YYSYMBOL_352_29 = 352,                   /* $@29  */
  YYSYMBOL_353_30 = 353,                   /* $@30  */
  YYSYMBOL_354_31 = 354,                   /* $@31  */
  YYSYMBOL_function_argument_declaration = 355, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 356,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 357,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 358,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 359,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 360,             /* variant_type  */
  YYSYMBOL_variant_type_list = 361,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 362,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 363,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 364,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 365,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 366,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 367, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 368, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 369, /* global_variable_declaration_list  */
  YYSYMBOL_370_32 = 370,                   /* $@32  */
  YYSYMBOL_optional_shared = 371,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 372, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 373,               /* global_let  */
  YYSYMBOL_374_33 = 374,                   /* $@33  */
  YYSYMBOL_enum_expression = 375,          /* enum_expression  */
  YYSYMBOL_enum_list = 376,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 377, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 378,             /* single_alias  */
  YYSYMBOL_379_34 = 379,                   /* $@34  */
  YYSYMBOL_alias_list = 380,               /* alias_list  */
  YYSYMBOL_alias_declaration = 381,        /* alias_declaration  */
  YYSYMBOL_optional_public_or_private_enum = 382, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 383,                /* enum_name  */
  YYSYMBOL_optional_enum_basic_type_declaration = 384, /* optional_enum_basic_type_declaration  */
  YYSYMBOL_enum_declaration = 385,         /* enum_declaration  */
  YYSYMBOL_386_35 = 386,                   /* $@35  */
  YYSYMBOL_387_36 = 387,                   /* $@36  */
  YYSYMBOL_optional_structure_parent = 388, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 389,          /* optional_sealed  */
  YYSYMBOL_structure_name = 390,           /* structure_name  */
  YYSYMBOL_class_or_struct = 391,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 392, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 393, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 394,    /* structure_declaration  */
  YYSYMBOL_395_37 = 395,                   /* $@37  */
  YYSYMBOL_396_38 = 396,                   /* $@38  */
  YYSYMBOL_variable_name_with_pos_list = 397, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 398,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 399, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 400, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 401,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 402,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 403,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 404, /* bitfield_type_declaration  */
  YYSYMBOL_405_39 = 405,                   /* $@39  */
  YYSYMBOL_406_40 = 406,                   /* $@40  */
  YYSYMBOL_table_type_pair = 407,          /* table_type_pair  */
  YYSYMBOL_dim_list = 408,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 409, /* type_declaration_no_options  */
  YYSYMBOL_410_41 = 410,                   /* $@41  */
  YYSYMBOL_411_42 = 411,                   /* $@42  */
  YYSYMBOL_412_43 = 412,                   /* $@43  */
  YYSYMBOL_413_44 = 413,                   /* $@44  */
  YYSYMBOL_414_45 = 414,                   /* $@45  */
  YYSYMBOL_415_46 = 415,                   /* $@46  */
  YYSYMBOL_416_47 = 416,                   /* $@47  */
  YYSYMBOL_417_48 = 417,                   /* $@48  */
  YYSYMBOL_418_49 = 418,                   /* $@49  */
  YYSYMBOL_419_50 = 419,                   /* $@50  */
  YYSYMBOL_420_51 = 420,                   /* $@51  */
  YYSYMBOL_421_52 = 421,                   /* $@52  */
  YYSYMBOL_422_53 = 422,                   /* $@53  */
  YYSYMBOL_423_54 = 423,                   /* $@54  */
  YYSYMBOL_424_55 = 424,                   /* $@55  */
  YYSYMBOL_425_56 = 425,                   /* $@56  */
  YYSYMBOL_426_57 = 426,                   /* $@57  */
  YYSYMBOL_427_58 = 427,                   /* $@58  */
  YYSYMBOL_428_59 = 428,                   /* $@59  */
  YYSYMBOL_429_60 = 429,                   /* $@60  */
  YYSYMBOL_430_61 = 430,                   /* $@61  */
  YYSYMBOL_431_62 = 431,                   /* $@62  */
  YYSYMBOL_432_63 = 432,                   /* $@63  */
  YYSYMBOL_433_64 = 433,                   /* $@64  */
  YYSYMBOL_434_65 = 434,                   /* $@65  */
  YYSYMBOL_435_66 = 435,                   /* $@66  */
  YYSYMBOL_436_67 = 436,                   /* $@67  */
  YYSYMBOL_type_declaration = 437,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 438,  /* tuple_alias_declaration  */
  YYSYMBOL_439_68 = 439,                   /* $@68  */
  YYSYMBOL_440_69 = 440,                   /* $@69  */
  YYSYMBOL_441_70 = 441,                   /* $@70  */
  YYSYMBOL_variant_alias_declaration = 442, /* variant_alias_declaration  */
  YYSYMBOL_443_71 = 443,                   /* $@71  */
  YYSYMBOL_444_72 = 444,                   /* $@72  */
  YYSYMBOL_445_73 = 445,                   /* $@73  */
  YYSYMBOL_bitfield_alias_declaration = 446, /* bitfield_alias_declaration  */
  YYSYMBOL_447_74 = 447,                   /* $@74  */
  YYSYMBOL_448_75 = 448,                   /* $@75  */
  YYSYMBOL_449_76 = 449,                   /* $@76  */
  YYSYMBOL_make_decl = 450,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 451,       /* make_struct_fields  */
  YYSYMBOL_make_struct_single = 452,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim = 453,          /* make_struct_dim  */
  YYSYMBOL_use_initializer = 454,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 455,         /* make_struct_decl  */
  YYSYMBOL_456_77 = 456,                   /* $@77  */
  YYSYMBOL_457_78 = 457,                   /* $@78  */
  YYSYMBOL_458_79 = 458,                   /* $@79  */
  YYSYMBOL_459_80 = 459,                   /* $@80  */
  YYSYMBOL_460_81 = 460,                   /* $@81  */
  YYSYMBOL_461_82 = 461,                   /* $@82  */
  YYSYMBOL_462_83 = 462,                   /* $@83  */
  YYSYMBOL_463_84 = 463,                   /* $@84  */
  YYSYMBOL_make_map_tuple = 464,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 465,          /* make_tuple_call  */
  YYSYMBOL_466_85 = 466,                   /* $@85  */
  YYSYMBOL_467_86 = 467,                   /* $@86  */
  YYSYMBOL_make_dim_decl = 468,            /* make_dim_decl  */
  YYSYMBOL_469_87 = 469,                   /* $@87  */
  YYSYMBOL_470_88 = 470,                   /* $@88  */
  YYSYMBOL_471_89 = 471,                   /* $@89  */
  YYSYMBOL_472_90 = 472,                   /* $@90  */
  YYSYMBOL_expr_map_tuple_list = 473,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 474,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 475, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 476,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 477       /* array_comprehension  */
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
#define YYLAST   11912

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  214
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  264
/* YYNRULES -- Number of rules.  */
#define YYNRULES  807
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1467

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   441


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
       2,     2,     2,   200,     2,   213,   211,   196,   189,     2,
     209,   210,   194,   193,   183,   192,   206,   195,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   186,   177,
     190,   184,   191,   185,   212,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   207,     2,   208,   188,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   175,   187,   176,   199,     2,     2,     2,
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
     178,   179,   180,   181,   182,   197,   198,   201,   202,   203,
     204,   205
};

#if DAS2_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   541,   541,   542,   547,   548,   549,   550,   551,   552,
     553,   554,   555,   556,   557,   558,   559,   563,   569,   570,
     571,   575,   576,   580,   598,   599,   600,   601,   605,   609,
     614,   623,   631,   647,   652,   660,   660,   699,   717,   721,
     722,   723,   727,   730,   734,   740,   749,   752,   758,   759,
     763,   767,   768,   772,   775,   781,   787,   790,   796,   797,
     801,   802,   803,   812,   813,   817,   818,   824,   825,   826,
     827,   828,   832,   838,   844,   850,   858,   868,   877,   884,
     885,   886,   887,   888,   889,   893,   898,   906,   907,   908,
     912,   913,   914,   915,   916,   917,   918,   919,   925,   928,
     934,   937,   942,   943,   944,   948,   961,   979,   982,   990,
    1001,  1012,  1023,  1026,  1033,  1037,  1044,  1045,  1049,  1050,
    1051,  1055,  1058,  1065,  1069,  1070,  1071,  1072,  1073,  1074,
    1075,  1076,  1077,  1078,  1079,  1080,  1081,  1082,  1083,  1084,
    1085,  1086,  1087,  1088,  1089,  1090,  1091,  1092,  1093,  1094,
    1095,  1096,  1097,  1098,  1099,  1100,  1101,  1102,  1103,  1104,
    1105,  1106,  1107,  1108,  1109,  1110,  1111,  1112,  1113,  1114,
    1115,  1116,  1117,  1118,  1119,  1120,  1121,  1122,  1123,  1124,
    1125,  1126,  1127,  1128,  1129,  1130,  1131,  1132,  1133,  1134,
    1135,  1136,  1137,  1138,  1139,  1140,  1141,  1142,  1143,  1144,
    1145,  1146,  1147,  1148,  1149,  1150,  1151,  1152,  1157,  1175,
    1176,  1177,  1181,  1187,  1187,  1204,  1208,  1219,  1228,  1240,
    1241,  1242,  1243,  1244,  1245,  1246,  1247,  1248,  1249,  1250,
    1251,  1252,  1253,  1254,  1255,  1256,  1257,  1258,  1259,  1260,
    1264,  1269,  1275,  1281,  1292,  1293,  1297,  1298,  1302,  1306,
    1313,  1313,  1313,  1319,  1319,  1319,  1328,  1362,  1365,  1368,
    1371,  1377,  1378,  1389,  1393,  1396,  1404,  1404,  1404,  1410,
    1413,  1417,  1421,  1428,  1434,  1438,  1442,  1445,  1448,  1456,
    1459,  1462,  1470,  1473,  1481,  1484,  1487,  1495,  1501,  1502,
    1503,  1507,  1508,  1512,  1513,  1517,  1522,  1530,  1536,  1548,
    1551,  1557,  1557,  1557,  1560,  1560,  1560,  1565,  1565,  1565,
    1573,  1573,  1573,  1579,  1589,  1600,  1615,  1618,  1624,  1625,
    1632,  1643,  1644,  1645,  1649,  1650,  1651,  1652,  1656,  1661,
    1669,  1670,  1674,  1679,  1686,  1693,  1701,  1702,  1703,  1704,
    1705,  1706,  1707,  1711,  1712,  1713,  1714,  1715,  1716,  1717,
    1718,  1719,  1720,  1721,  1722,  1723,  1724,  1725,  1726,  1727,
    1728,  1729,  1733,  1734,  1735,  1736,  1741,  1742,  1743,  1744,
    1745,  1746,  1747,  1748,  1749,  1750,  1751,  1752,  1753,  1754,
    1755,  1756,  1757,  1762,  1769,  1781,  1786,  1796,  1800,  1807,
    1810,  1810,  1810,  1815,  1815,  1815,  1828,  1832,  1836,  1841,
    1848,  1848,  1848,  1855,  1859,  1868,  1872,  1875,  1881,  1882,
    1883,  1884,  1885,  1886,  1887,  1888,  1889,  1890,  1891,  1892,
    1893,  1894,  1895,  1896,  1897,  1898,  1899,  1900,  1901,  1902,
    1903,  1904,  1905,  1906,  1907,  1908,  1909,  1910,  1911,  1912,
    1913,  1914,  1915,  1916,  1922,  1923,  1924,  1925,  1926,  1939,
    1940,  1941,  1942,  1943,  1944,  1945,  1946,  1947,  1948,  1949,
    1950,  1953,  1956,  1957,  1960,  1960,  1960,  1963,  1968,  1972,
    1976,  1976,  1976,  1981,  1984,  1988,  1988,  1988,  1993,  1996,
    1997,  1998,  1999,  2000,  2001,  2002,  2003,  2004,  2006,  2010,
    2014,  2015,  2016,  2017,  2018,  2019,  2020,  2024,  2028,  2032,
    2036,  2040,  2044,  2048,  2052,  2056,  2063,  2064,  2068,  2069,
    2070,  2074,  2075,  2079,  2080,  2081,  2085,  2086,  2090,  2101,
    2104,  2104,  2123,  2122,  2136,  2135,  2151,  2160,  2170,  2171,
    2175,  2178,  2187,  2188,  2192,  2195,  2198,  2214,  2223,  2224,
    2228,  2231,  2234,  2248,  2249,  2253,  2259,  2265,  2268,  2272,
    2278,  2287,  2288,  2289,  2293,  2294,  2298,  2305,  2310,  2319,
    2325,  2336,  2339,  2344,  2349,  2357,  2368,  2371,  2371,  2391,
    2392,  2396,  2397,  2398,  2402,  2405,  2405,  2423,  2427,  2434,
    2437,  2450,  2467,  2468,  2469,  2474,  2474,  2500,  2501,  2505,
    2506,  2510,  2511,  2512,  2516,  2526,  2529,  2535,  2540,  2535,
    2555,  2556,  2560,  2561,  2565,  2571,  2572,  2576,  2577,  2578,
    2582,  2585,  2591,  2596,  2591,  2610,  2617,  2622,  2631,  2637,
    2648,  2649,  2650,  2651,  2652,  2653,  2654,  2655,  2656,  2657,
    2658,  2659,  2660,  2661,  2662,  2663,  2664,  2665,  2666,  2667,
    2668,  2669,  2670,  2671,  2672,  2673,  2674,  2678,  2679,  2680,
    2681,  2682,  2683,  2684,  2685,  2689,  2700,  2704,  2711,  2723,
    2730,  2739,  2744,  2754,  2767,  2767,  2767,  2780,  2784,  2791,
    2795,  2802,  2803,  2804,  2805,  2806,  2821,  2827,  2827,  2827,
    2831,  2836,  2843,  2843,  2850,  2854,  2858,  2863,  2868,  2873,
    2878,  2882,  2886,  2891,  2895,  2899,  2904,  2904,  2904,  2910,
    2917,  2917,  2917,  2922,  2922,  2922,  2928,  2928,  2928,  2933,
    2937,  2937,  2937,  2942,  2942,  2942,  2951,  2955,  2955,  2955,
    2960,  2960,  2960,  2969,  2973,  2973,  2973,  2978,  2978,  2978,
    2987,  2987,  2987,  2993,  2993,  2993,  3002,  3005,  3016,  3032,
    3037,  3042,  3032,  3067,  3072,  3078,  3067,  3103,  3108,  3113,
    3103,  3143,  3144,  3145,  3146,  3147,  3151,  3158,  3165,  3171,
    3177,  3184,  3191,  3197,  3207,  3215,  3220,  3227,  3228,  3232,
    3232,  3232,  3240,  3240,  3240,  3247,  3247,  3247,  3254,  3254,
    3254,  3265,  3271,  3277,  3283,  3283,  3283,  3293,  3301,  3309,
    3309,  3309,  3317,  3324,  3324,  3324,  3334,  3337,  3343,  3351,
    3359,  3367,  3380,  3381,  3385,  3386,  3391,  3394
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
  "\"inscope\"", "\"static\"", "\"fixed_array\"", "\"default\"",
  "\"uninitialized\"", "\"bool\"", "\"void\"", "\"string\"", "\"auto\"",
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
  "\"...\"", "\"integer constant\"", "\"long integer constant\"",
  "\"unsigned integer constant\"", "\"unsigned long integer constant\"",
  "\"unsigned int8 constant\"", "\"floating point constant\"",
  "\"double constant\"", "\"name\"", "\"keyword\"", "\"type function\"",
  "\"start of the string\"", "STRING_CHARACTER", "STRING_CHARACTER_ESC",
  "\"end of the string\"", "\"{\"", "\"}\"",
  "\"end of failed eader macro\"", "\"begin of code block\"",
  "\"end of code block\"", "\"end of expression\"", "\";}}\"", "\";}]\"",
  "\";]]\"", "\",]]\"", "\",}]\"", "','", "'='", "'?'", "':'", "'|'",
  "'^'", "'&'", "'<'", "'>'", "'-'", "'+'", "'*'", "'/'", "'%'",
  "UNARY_MINUS", "UNARY_PLUS", "'~'", "'!'", "PRE_INC", "PRE_DEC",
  "POST_INC", "POST_DEC", "DEREF", "'.'", "'['", "']'", "'('", "')'",
  "'$'", "'@'", "'#'", "$accept", "program", "top_level_reader_macro",
  "optional_public_or_private_module", "module_name", "module_declaration",
  "character_sequence", "string_constant", "string_builder_body",
  "string_builder", "reader_character_sequence", "expr_reader", "$@1",
  "options_declaration", "require_declaration", "keyword_or_name",
  "require_module_name", "require_module", "is_public_module",
  "expect_declaration", "expect_list", "expect_error", "expression_label",
  "expression_goto", "elif_or_static_elif", "expression_else",
  "if_or_static_if", "expression_else_one_liner",
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
  "expression_any", "expressions", "expr_keyword", "optional_expr_list",
  "optional_expr_list_in_braces", "type_declaration_no_options_list",
  "expression_keyword", "$@3", "$@4", "$@5", "$@6", "expr_pipe",
  "name_in_namespace", "expression_delete", "new_type_declaration", "$@7",
  "$@8", "expr_new", "expression_break", "expression_continue",
  "expression_return_no_pipe", "expression_return",
  "expression_yield_no_pipe", "expression_yield", "expression_try_catch",
  "kwd_let_var_or_nothing", "kwd_let", "optional_in_scope",
  "tuple_expansion", "tuple_expansion_variable_declaration",
  "expression_let", "expr_cast", "$@9", "$@10", "$@11", "$@12", "$@13",
  "$@14", "expr_type_decl", "$@15", "$@16", "expr_type_info", "expr_list",
  "block_or_simple_block", "block_or_lambda", "capture_entry",
  "capture_list", "optional_capture_list", "expr_block", "expr_full_block",
  "expr_full_block_assumed_piped", "expr_numeric_const", "expr_assign",
  "expr_assign_pipe_right", "expr_assign_pipe", "expr_named_call",
  "expr_method_call", "func_addr_name", "func_addr_expr", "$@17", "$@18",
  "$@19", "$@20", "expr_field", "$@21", "$@22", "expr_call", "expr",
  "$@23", "$@24", "$@25", "$@26", "$@27", "$@28", "expr_mtag",
  "optional_field_annotation", "optional_override", "optional_constant",
  "optional_public_or_private_member_variable",
  "optional_static_member_variable", "structure_variable_declaration",
  "struct_variable_declaration_list", "$@29", "$@30", "$@31",
  "function_argument_declaration", "function_argument_list", "tuple_type",
  "tuple_type_list", "tuple_alias_type_list", "variant_type",
  "variant_type_list", "variant_alias_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list", "let_variable_declaration",
  "global_variable_declaration_list", "$@32", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@33",
  "enum_expression", "enum_list", "optional_public_or_private_alias",
  "single_alias", "$@34", "alias_list", "alias_declaration",
  "optional_public_or_private_enum", "enum_name",
  "optional_enum_basic_type_declaration", "enum_declaration", "$@35",
  "$@36", "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "optional_struct_variable_declaration_list", "structure_declaration",
  "$@37", "$@38", "variable_name_with_pos_list", "basic_type_declaration",
  "enum_basic_type_declaration", "structure_type_declaration",
  "auto_type_declaration", "bitfield_bits", "bitfield_alias_bits",
  "bitfield_type_declaration", "$@39", "$@40", "table_type_pair",
  "dim_list", "type_declaration_no_options", "$@41", "$@42", "$@43",
  "$@44", "$@45", "$@46", "$@47", "$@48", "$@49", "$@50", "$@51", "$@52",
  "$@53", "$@54", "$@55", "$@56", "$@57", "$@58", "$@59", "$@60", "$@61",
  "$@62", "$@63", "$@64", "$@65", "$@66", "$@67", "type_declaration",
  "tuple_alias_declaration", "$@68", "$@69", "$@70",
  "variant_alias_declaration", "$@71", "$@72", "$@73",
  "bitfield_alias_declaration", "$@74", "$@75", "$@76", "make_decl",
  "make_struct_fields", "make_struct_single", "make_struct_dim",
  "use_initializer", "make_struct_decl", "$@77", "$@78", "$@79", "$@80",
  "$@81", "$@82", "$@83", "$@84", "make_map_tuple", "make_tuple_call",
  "$@85", "$@86", "make_dim_decl", "$@87", "$@88", "$@89", "$@90",
  "expr_map_tuple_list", "make_table_decl", "array_comprehension_where",
  "optional_comma", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1347)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-728)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1347,   120, -1347, -1347,    54,   -49,   142,   -17, -1347,   -89,
      19,    19,    19, -1347,    29,   209, -1347, -1347,    -5, -1347,
   -1347, -1347,   344, -1347,   201, -1347, -1347, -1347, -1347, -1347,
   -1347, -1347, -1347, -1347, -1347,   116, -1347,   110,   119,   134,
   -1347, -1347, -1347, -1347,   142, -1347,     2, -1347, -1347, -1347,
      19,   211,   169, -1347, -1347,   201,   222,   278,   353,   365,
     410, -1347, -1347, -1347,   209,   209,   209,   157, -1347,   502,
     -93, -1347, -1347, -1347, -1347,   154,   464,   472, -1347,   491,
      23,    54,   398,   -49,   364,   445, -1347,   492,   492, -1347,
     407,   -22, -1347, -1347,   532, -1347, -1347, -1347, -1347,   456,
     417, -1347, -1347,   -87,    54,   209,   209,   209,   209, -1347,
   -1347, -1347, -1347, -1347, -1347, -1347,   477, -1347, -1347, -1347,
   -1347, -1347,   476, -1347, -1347, -1347, -1347, -1347,   334,   210,
   -1347, -1347, -1347, -1347,   537, -1347, -1347, -1347, -1347,   424,
     469, -1347, -1347, -1347,   490,   495,   501, -1347, -1347,   510,
   -1347,   -28, -1347,   395,   539,   502, 11747, -1347,   500,   591,
   -1347,   499, -1347, -1347,   514, -1347, -1347, -1347, -1347, -1347,
   -1347, -1347,    68, -1347, -1347,  1480, -1347, -1347, -1347, -1347,
   -1347, 10457, -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347,
   -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347,
   -1347, -1347, -1347, -1347, -1347, -1347, -1347,   673,   678, -1347,
     504,   540,   467,   542, -1347,   553, -1347,   543,    54,   508,
     175, -1347, -1347, -1347,   210, -1347,   531,   535,   536,   513,
     541,   547, -1347, -1347, -1347,   518, -1347, -1347, -1347, -1347,
   -1347,   548, -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347,
   -1347, -1347, -1347,   552, -1347, -1347, -1347,   554,   555, -1347,
   -1347, -1347, -1347,   558,   562,   524,    29, -1347, -1347, -1347,
   -1347, -1347,   517,   556,   588, -1347, -1347,   589,   592, -1347,
   -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347,
   -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347,   594,
     515, -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347,
   -1347, -1347, -1347,   735, -1347, -1347, -1347, -1347, -1347, -1347,
   -1347, -1347, -1347, -1347, -1347, -1347,   595,   557, -1347, -1347,
     -91,   575, -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347,
   -1347, -1347, -1347, -1347,   576,   593, -1347,   499, -1347,    54,
     561,   724,   389, -1347, -1347, -1347, -1347, -1347,  6868, -1347,
   -1347,   607, -1347,   256,   280,   281, -1347, -1347,  6868,   121,
   -1347, -1347, -1347,    14, -1347, -1347, -1347,    28,  3680, -1347,
     566,  7355, -1347,   596,  7451,    62, -1347, -1347, -1347, -1347,
     609,   640, -1347,   569, -1347,    83, -1347,    35,  1480, -1347,
    1678,   615,    29, -1347, -1347, -1347,   175, -1347,  6868,   616,
     618,  1480, -1347,   -11,  1480,  1480,  1480,   597,   598, -1347,
   -1347,    39,   577,   599,   181, -1347,   196,   581,   601,   602,
     584,   608,   206,   610, -1347,   223,   625,   627,  6868,  6868,
     590,   613,   617,   623,   624,   626, -1347, -1347, -1347, -1347,
   -1347, -1347, -1347, -1347,   637,   644, -1347,  3881,  6868,  6868,
    6868,  6868,  6868,  4082,  6868, -1347,   572, -1347, -1347, -1347,
     628, -1347, -1347, -1347, -1347,   578, -1347, -1347, -1347, -1347,
   -1347, -1347, -1347,   898, -1347,   630, -1347, -1347, -1347, -1347,
   -1347, -1347,  1480,  1480,   634,   641,  1480,   504,  1480,   504,
    1480,   504,  7559,   671,  7819, -1347,  6868, -1347, -1347, -1347,
   -1347,   645, -1347, -1347,  9916,  6868, -1347,   517,   680, -1347,
     -39, -1347,   672,   556,   676,   669, -1347,   683,   685, -1347,
   -1347,  6868, -1347, -1347,   327,   -50, -1347,   556, -1347,  6868,
   -1347, -1347,   327,  2285, -1347,   540,  4281,  6868,   697, -1347,
     688,   708,  4480,   540,  2484,   -33,   -33,   -33,  4679,   831,
   -1347,   691,   692,  6868,   866, -1347, -1347, -1347, -1347, -1347,
   -1347, -1347, -1347, -1347,   478, -1347,   699,   709,   710,   711,
   -1347,   717, -1347, -1347,   826, -1347,   -59, -1347,  7176,   713,
   -1347,   712, -1347,     8, -1347,  7918, -1347,   859,  1224, -1347,
   -1347, -1347,  2882,   225,   314,   721,   141, -1347, -1347, -1347,
     694,   696, -1347,    29, -1347, -1347,  6868,  1480,  6868,  6868,
   -1347, -1347,  6868, -1347, -1347,  6868, -1347, -1347,  6868, -1347,
    1480,   612,   612,  6868,  6868,  6868,  6868,  6868,  6868, -1347,
   -1347,   465,   327, 10492, -1347,   731,   612,   612,   -30,   612,
     612,   327,   732, 11058,   732,   204,  3280,   706, 10331, 11624,
    6868,  6868, -1347, -1347,  6868,  6868,  6868,  6868,   751,  6868,
     -52,  6868,  6868,  6868,  6868,  6868,  6868,  6868,  6868,  6868,
    4878,  6868,  6868,  6868,  6868,  6868,  6868,  6868,  6868,  6868,
    6868,   -40,  6868, -1347,  5077,   413,   418, -1347, -1347,    -6,
     420,   575,   421,   575,   426,   575, -1347,   284, -1347,   303,
   -1347,  1480,   707,   739, -1347, -1347, 10015, -1347,   752,  1480,
   -1347, -1347,  1480, -1347, -1347,  8017,   718,   888, -1347,   101,
   -1347, 10591,    10,  2882, -1347,   739,   801,  7560,   920,  6868,
   11058, 10591,   750, -1347,   749,   780, 11058, -1347,  2882, -1347,
    7560,   727, -1347,   578, -1347, -1347, -1347, 10591,   765, -1347,
   -1347, 10591,  6868,   578, -1347, -1347, -1347, -1347, -1347, -1347,
   -1347,   -64,   -33, -1347,  3081,  3081,  3081,  3081,  3081,  3081,
    3081,  3081,  3081,  3081,  3081,  6868,  3081,  3081,  3081,  3081,
    3081,  3081, -1347,  6868,   615, -1347, -1347,   935,   499, -1347,
     788, -1347,  2882, -1347,  7671, -1347, -1347, -1347,  1480,  1480,
    1480,  1480,   886,  6868,  5276,  1480,  1480,   732,  1117,   731,
    8052,  1480,  1480,  8151,  1480,  1480,   732,  1480,  1480,   732,
    1480,    76,  8250,  8285,  8384,  8483,  8518,  8617,  1480,  1480,
   -1347,  6868,   461,    12,  6868,  6868,   781,    17,  6868,   748,
     754,   757,   758,   282, -1347, -1347,   759,   145,  3481, -1347,
     109,   775,   755,   753,   504,   769,   760, -1347, -1347,   771,
     761, -1347, -1347,   986,   986,   714,   714, 11556, 11556,   762,
     186,   764, -1347, 10114,   -63,   -63,   630,   986,   986, 11334,
   11291,  7217, 11157, 11716, 10690, 11421, 11445, 11469,   714,   714,
     730,   730,   186,   186,   186,   285,  6868,   766,   768,   288,
    6868,   973, 10149, -1347,   151, -1347, -1347,   813, -1347, -1347,
     789, -1347,   790, -1347,   791,  7559, -1347,   671, -1347,   304,
     556, -1347,  6868, -1347, -1347,   556,   556, -1347,  6868,   814,
   -1347,   835, -1347,  1480, -1347,  2882, -1347,  6868, -1347,   739,
    6868,  6868,  6868,  6868,  6868,  6868,  6868,  6868,  6868,  6868,
    6868,  6868,  6868,  6868,  6868,  6868,  6868,   540, 11058, -1347,
    6868, -1347, -1347, -1347,  7560, -1347,   706, -1347, -1347,   546,
     756,   706,   836, -1347, -1347, -1347,   -33,   -33,   -33, -1347,
   -1347,  1086, -1347,  1086, -1347,  1086, -1347,  1086, -1347,  1086,
   -1347,  1086, -1347,  1086, -1347,  1086, -1347,  1086, -1347,  1086,
   -1347,  1086, 11058, -1347,  1086, -1347,  1086, -1347,  1086, -1347,
    1086, -1347,  1086, -1347,  1086, 11058, -1347,   827,   533,   936,
     828, -1347, -1347,  7781, -1347, -1347, -1347, -1347,   556,  1226,
    1409,   428, -1347,    18,   794,   843,  8716,   433,  1754,   799,
    1480,   802,   800, -1347,  1788,  1832, -1347,  1843,  1862,   807,
    2208,  2407,   808,  2606,   706, -1347, -1347, -1347, -1347, -1347,
     819,   308,   316, 10725,  6868, 11058, -1347, -1347,  6868, 11058,
   -1347, -1347,  6868,  6868,  1480,   504,  6868,  6868,  6868,    30,
    7067, -1347,   318, -1347,   -55,   575, -1347,  6868, -1347,  6868,
    5475,  6868, -1347,   839,   821, -1347, -1347,  6868,   822, -1347,
   10248,  6868,  5674,   824, -1347, 10347, -1347, -1347, -1347, -1347,
   -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347,
   -1347, -1347, -1347,  1480,   829,  8751, -1347,   995,     5, -1347,
    7560,   341, 11058, 11058, 11058, 11058, 11058, 11058, 11058, 11058,
   11058, 11058, 11058, 11058, 11058, 11058, 11058, 11058, 11058, -1347,
   11058,   504,  2086,   540, -1347, -1347,  6868, -1347,  2683,   862,
     504, -1347,   189, -1347, -1347, -1347, -1347, -1347, -1347,   258,
   -1347,    22, -1347, -1347, -1347, -1347, -1347,     3, -1347,   833,
   -1347,   323, -1347, -1347, -1347, -1347,  2805,  6868, -1347, -1347,
   -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347,   838,  5873,
   -1347, -1347, -1347,   414,   419,  8850,  8949,  3004,   575,  8984,
   11058, 11058,   834,  3481,   840,    25,   880,   883,   885,   887,
   -1347,    73,   -58,  1480,  9083,  1480,  9182, -1347,   194,  9217,
   -1347,  6868, 11192,  6868, -1347,  9316, -1347,   198,  6868, -1347,
   -1347, -1347, -1347, -1347,   556,  6868, -1347,   890,  6868, -1347,
     575, -1347, -1347, 10591,  6072,  6271, -1347, -1347, -1347, -1347,
   -1347, 11058, -1347,   575,   891,   228,  1019,    22, -1347, -1347,
     533,   844,   848, -1347, -1347,   893,  6868, -1347,   851,   852,
     731,  6868,  6868,  6868,   853,   886,   854,   855,  6470, -1347,
     200,  6868,   856,  6868,  6868, -1347, -1347, -1347,   875,   163,
   -1347,    74,  6868,  6868,  6868, -1347, -1347, -1347, -1347,   -55,
     860,  6669, -1347, -1347,  3203, -1347,   435, -1347, -1347, -1347,
    1480,  9415,  9450, -1347, -1347,  9549,   857, -1347, 11058,   -58,
     546,  6868,  6868, 11058,   540, -1347,  1480,   -11, -1347, -1347,
    1019,   327,   886,   886,   882,  9648,  6868,  6868,   861,   714,
     714,   714,  6868, -1347,   886,   886, -1347,  9683, -1347,   714,
    6868, -1347,   899, 10824,   241, -1347,  6868,  6868,   865,  9782,
   11058, 11058, -1347, -1347,  6868, 11157, -1347, -1347,   436, -1347,
   -1347, -1347, -1347, -1347, -1347, 11058, -1347,   503,  6868, 11747,
   -1347, -1347,   346,   346,  6868, -1347,   732,   731, -1347,   732,
     346,   346, -1347,   867,  1026,   904,   873, -1347,   241, 11058,
   11058, -1347,   171, 11192, -1347, -1347, -1347,  6868, 10923,   905,
   11747,   775,    53,    59,  9881,   874,   889,   901,    67,    89,
   -1347,  6868, -1347, -1347, -1347,  6868,  6868, -1347, 10959, -1347,
   -1347,   540,   346, -1347, -1347, -1347, -1347, -1347, -1347, -1347,
   -1347, 11058, 11058, 11058, -1347, -1347,   775
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   116,     1,   291,     0,     0,     0,   582,   292,     0,
     582,   582,   582,    16,     0,     0,    15,     3,     0,    10,
       9,     8,     0,     7,   569,     6,    11,     5,     4,    13,
      12,    14,    88,    89,    87,    96,    98,    37,    53,    50,
      51,    39,    40,    41,     0,    42,    48,    38,   584,   583,
     582,     0,     0,    22,    21,   569,     0,     0,     0,     0,
     261,    35,   103,   104,     0,     0,     0,   105,   107,   114,
       0,   102,    17,   606,   605,   209,   591,   607,   570,   571,
       0,     0,     0,     0,    43,     0,    49,     0,     0,    46,
       0,   582,   585,   590,    18,   747,   739,   743,   263,     0,
       0,   113,   108,     0,     0,     0,     0,     0,     0,   117,
     211,   210,   213,   208,   593,   592,     0,   609,   608,   612,
     573,   572,   575,    94,    95,    92,    93,    91,     0,     0,
      90,    99,    54,    52,    48,    45,    44,   587,   589,     0,
       0,    19,    20,    23,     0,     0,     0,   262,    33,    36,
     112,     0,   109,   110,   111,   115,     0,   594,   595,   602,
     566,   506,    24,    25,     0,    83,    84,    81,    82,    80,
      79,    85,     0,    47,   588,     0,   748,   740,   744,    34,
     106,     0,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,     0,     0,   123,
     118,     0,     0,     0,   603,     0,   613,   567,     0,   507,
       0,    26,    27,    28,     0,    97,     0,     0,     0,     0,
       0,     0,   620,   640,   621,   656,   622,   626,   627,   628,
     629,   646,   633,   634,   635,   636,   637,   638,   639,   641,
     642,   643,   644,   709,   625,   632,   645,   716,   723,   623,
     630,   624,   631,     0,     0,     0,     0,   655,   671,   674,
     672,   673,   736,   586,   661,   534,   540,   177,   178,   175,
     126,   127,   129,   128,   130,   131,   132,   133,   159,   160,
     157,   158,   150,   161,   162,   151,   148,   149,   176,   170,
       0,   174,   163,   164,   165,   166,   137,   138,   139,   134,
     135,   136,   147,     0,   153,   154,   152,   145,   146,   141,
     140,   142,   143,   144,   125,   124,   169,     0,   155,   156,
     506,   121,   240,   214,   647,   650,   653,   654,   648,   651,
     649,   652,   596,   597,   600,   610,   574,   506,   100,     0,
       0,   556,   554,   576,    86,   677,   700,   703,     0,   706,
     696,     0,   664,   710,   717,   724,   730,   733,     0,     0,
     686,   691,   685,     0,   699,   695,   688,     0,     0,   690,
     675,     0,   662,   804,   741,   745,   179,   180,   173,   168,
     181,   171,   167,     0,   119,   290,   528,     0,     0,   212,
       0,   579,     0,   604,   519,   614,     0,   101,     0,     0,
       0,     0,   555,     0,     0,     0,     0,     0,     0,   414,
     415,     0,     0,     0,     0,   408,     0,     0,     0,     0,
       0,     0,     0,     0,   646,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   495,   336,   338,   337,
     339,   340,   341,   342,     0,     0,    29,     0,     0,     0,
       0,     0,     0,     0,     0,   321,   322,   412,   411,   489,
     409,   482,   481,   480,   479,   116,   485,   410,   484,   483,
     455,   416,   456,     0,   417,     0,   413,   751,   755,   752,
     753,   754,     0,     0,     0,     0,     0,   118,     0,   118,
       0,   118,     0,     0,     0,   682,   244,   693,   694,   687,
     689,     0,   692,   676,     0,     0,   738,   737,   805,   749,
     261,   535,     0,   530,     0,     0,   541,     0,     0,   182,
     172,     0,   288,   289,     0,   506,   120,   122,   242,     0,
      63,    64,     0,   276,   274,     0,     0,     0,     0,   275,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   215,
     219,     0,     0,     0,     0,   235,   230,   227,   226,   228,
     229,   260,   241,   221,   489,   220,     0,    70,    71,    68,
     233,    69,   234,   236,   294,   225,     0,   222,   343,   577,
     580,   804,   601,   520,   568,     0,   558,   559,     0,   552,
     553,   551,     0,     0,     0,     0,   667,   769,   772,   266,
       0,   269,   273,     0,   310,   789,     0,     0,     0,     0,
     301,   304,     0,   307,   793,     0,   778,   784,     0,   775,
       0,   444,   445,     0,     0,     0,     0,     0,     0,   250,
     253,     0,     0,   782,   796,   804,   421,   420,   457,   419,
     418,     0,   804,   316,   804,   323,     0,   330,     0,     0,
       0,     0,   446,   447,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   400,     0,   680,     0,     0,     0,   657,   659,     0,
       0,   121,     0,   121,     0,   121,   532,     0,   538,     0,
     658,     0,     0,   245,   684,   669,     0,   663,     0,     0,
     536,   742,     0,   542,   746,     0,     0,   615,   526,   545,
     529,     0,     0,     0,   280,   277,     0,   316,     0,     0,
     264,     0,     0,   239,     0,     0,    57,    75,     0,   285,
     282,   322,   332,   116,   259,   257,   258,     0,     0,   237,
     238,     0,     0,   116,   218,   224,   231,   232,   279,   284,
     293,     0,     0,   223,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   217,     0,   805,   598,   611,     0,   506,   557,
       0,   561,     0,   565,   343,   678,   701,   704,     0,     0,
       0,     0,   767,   244,     0,     0,     0,   804,     0,   804,
       0,     0,     0,     0,     0,     0,   804,     0,     0,   804,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      32,     0,    30,     0,     0,   805,     0,     0,   805,     0,
       0,     0,     0,   390,   387,   389,     0,   261,     0,   403,
       0,   764,     0,     0,   118,     0,     0,   468,   467,     0,
       0,   469,   473,   422,   423,   435,   436,   433,   434,     0,
     462,     0,   453,     0,   486,   487,   488,   424,   425,   440,
     441,   442,   443,     0,     0,   438,   439,   437,   431,   432,
     427,   426,   428,   429,   430,     0,     0,     0,   396,     0,
       0,     0,     0,   406,     0,   707,   697,     0,   665,   711,
       0,   718,     0,   725,     0,     0,   731,     0,   734,     0,
     248,   681,     0,   670,   750,   531,   537,   527,     0,     0,
     544,     0,   543,     0,   546,     0,    76,     0,   281,   278,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   265,    77,
       0,    55,    56,   286,   283,   323,   330,   243,   240,    60,
      65,   330,     0,   300,   299,   256,     0,     0,     0,   365,
     374,   353,   375,   354,   377,   356,   376,   355,   378,   357,
     368,   347,   369,   348,   370,   349,   379,   358,   380,   359,
     367,   345,   346,   381,   360,   382,   361,   371,   350,   372,
     351,   373,   352,   366,   344,   578,   581,     0,   513,   516,
       0,   560,   563,   343,   564,   679,   702,   705,   668,     0,
       0,     0,   768,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   458,     0,     0,   459,     0,     0,     0,
       0,     0,     0,     0,   330,   490,   491,   492,   493,   494,
       0,     0,     0,     0,     0,   781,   797,   798,     0,   317,
     787,   448,     0,     0,     0,   118,     0,     0,     0,     0,
       0,   405,     0,   404,     0,   121,   464,     0,   470,     0,
       0,     0,   451,     0,     0,   474,   478,     0,     0,   454,
       0,     0,     0,     0,   397,     0,   401,   449,   407,   708,
     698,   660,   666,   712,   714,   719,   721,   726,   728,   533,
     732,   539,   735,     0,     0,     0,   617,   618,   547,   550,
     549,     0,   353,   354,   356,   355,   357,   347,   348,   349,
     358,   359,   345,   360,   361,   350,   351,   352,   344,   287,
      78,   118,     0,     0,    58,    59,     0,    72,     0,     0,
     118,   295,     0,   364,   362,   363,   599,   514,   515,   516,
     517,   508,   521,   562,   770,   773,   267,     0,   271,     0,
     270,     0,   313,   311,   790,   788,     0,     0,   799,   302,
     305,   308,   794,   792,   779,   785,   783,   776,     0,     0,
     251,   254,    31,     0,     0,     0,     0,     0,   121,     0,
     757,   756,     0,     0,     0,     0,     0,     0,     0,     0,
     328,     0,     0,     0,     0,     0,     0,   385,     0,     0,
     475,     0,   463,     0,   452,     0,   398,     0,     0,   450,
     402,   715,   722,   729,   249,   244,   616,     0,     0,    74,
     121,   216,    61,     0,   276,     0,    66,    70,    71,    68,
      69,    67,    73,   121,     0,   554,   511,   508,   509,   510,
     513,     0,     0,   268,   272,     0,     0,   312,     0,     0,
     804,     0,     0,     0,     0,   767,     0,     0,     0,   496,
       0,     0,   246,     0,     0,   388,   505,   391,     0,     0,
     383,     0,     0,     0,     0,   326,   327,   325,   324,     0,
       0,     0,   318,   334,     0,   504,     0,   502,   386,   499,
       0,     0,     0,   498,   399,     0,     0,   619,   548,     0,
      60,     0,     0,   282,     0,   296,     0,     0,   512,   522,
     511,     0,   767,   767,     0,     0,     0,     0,     0,   303,
     306,   309,     0,   780,   767,   767,   460,     0,   497,   252,
     244,   255,   802,   802,     0,   394,     0,     0,     0,     0,
     759,   758,   329,   331,     0,   319,   465,   471,     0,   503,
     501,   500,   683,   333,    62,   283,   335,     0,     0,     0,
     524,   518,     0,     0,     0,   314,   804,   804,   800,   804,
       0,     0,   461,     0,     0,     0,     0,   392,     0,   761,
     760,   384,     0,   320,   466,   472,   476,     0,     0,     0,
       0,   765,     0,     0,     0,     0,     0,     0,     0,     0,
     247,     0,   807,   806,   395,     0,     0,   477,     0,   298,
     523,     0,     0,   771,   774,   315,   791,   801,   795,   786,
     777,   803,   763,   762,   297,   525,   766
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1347, -1347, -1347, -1347, -1347, -1347,   455,  1017, -1347, -1347,
   -1347,  1097, -1347, -1347, -1347,   606,  1056, -1347,   982, -1347,
   -1347,  1034, -1347, -1347, -1347,  -222, -1347, -1347,   -47, -1347,
   -1347, -1347, -1347, -1347, -1347,   900, -1347, -1347,   -54,  1018,
   -1347, -1347, -1347,   257, -1347,  -462,  -479,  -675, -1347, -1347,
   -1347, -1346, -1347, -1347,  -209,  -391, -1347,   147, -1347,  -796,
   -1347,  -138,  -353, -1347, -1347, -1347, -1347,  -469,   -14, -1347,
   -1347, -1347, -1347, -1347,   -45,   -42,   -41, -1347,   -37, -1347,
   -1347, -1347,  1127, -1347, -1347, -1347, -1347, -1347, -1347, -1347,
   -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347,  -460,  -210,
    -478,  -187, -1347,  -927,  -524, -1347,   559, -1347,  -393,   208,
   -1347, -1347, -1347, -1279, -1347, -1347, -1347, -1347, -1347, -1347,
   -1347, -1347, -1347,   701, -1347, -1347, -1347, -1347, -1347, -1347,
   -1347,  -142,  -143,  -214,  -139,   -36, -1347, -1347, -1347, -1347,
   -1347,   603, -1347,  -477, -1347, -1347,  -482, -1347, -1347,  -719,
    -207,  -592,  -133, -1347,  -382, -1347, -1347,  1082, -1347, -1347,
   -1347,   351, -1347,   662,    55, -1347, -1347, -1347, -1347, -1347,
   -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347,
   -1347, -1347, -1347,  -494,  -161, -1347,   725, -1347, -1347, -1347,
   -1347, -1347, -1347, -1347, -1347,  -370, -1347, -1347, -1347, -1347,
   -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347,
   -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347,
   -1347, -1347, -1347,  -140, -1347, -1347, -1347, -1347, -1347, -1347,
   -1347, -1347, -1347, -1347, -1347, -1347,   726,  -640,   105,  -830,
   -1253, -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347,
    -833, -1347, -1347, -1347, -1347, -1347, -1347, -1347, -1347,  -610,
   -1347,  -223,  -586, -1347
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    16,   143,    55,    17,   164,   170,   641,   467,
     149,   468,   100,    19,    20,    45,    46,    47,    89,    21,
      39,    40,   561,   562,  1166,  1167,   563,  1169,   564,   565,
     566,   567,   568,   569,   570,   171,   172,    35,    36,    37,
     219,    67,    68,    69,    70,    22,   331,   399,   210,    23,
     112,   211,   113,   156,   752,   989,   572,   400,   573,   712,
    1371,   929,   469,   838,  1301,   839,  1302,   575,   470,   576,
     610,   811,  1283,   471,   577,   578,   579,   580,   581,   582,
     583,   534,   584,   771,  1172,   983,   585,   472,   821,  1291,
     822,  1292,   824,  1293,   473,   815,  1287,   474,   713,  1323,
     475,  1230,  1231,   864,   754,   476,   792,   477,   736,   990,
     587,   478,   479,   855,   480,  1084,  1374,  1085,  1418,   481,
     911,  1250,   482,   653,  1233,  1424,  1235,  1425,  1330,  1447,
     484,   395,  1280,  1349,  1179,  1181,  1030,   593,   798,  1399,
    1430,   396,   397,   522,   707,   384,   527,   709,   385,  1088,
     728,   602,   413,   352,   353,   217,   347,    79,   122,    25,
     161,   590,   591,    51,    52,   140,    91,    26,   116,   158,
     213,    27,   401,  1027,   403,   215,   216,    77,   119,   405,
      28,   159,   345,   729,   485,   342,   269,   270,   699,   383,
     271,   495,  1122,   605,   380,   272,   414,  1035,   711,   493,
    1120,   415,  1036,   416,  1037,   492,  1119,   496,  1123,   497,
    1251,   498,  1125,   499,  1252,   500,  1127,   501,  1253,   502,
    1130,   503,  1132,   523,    29,   145,   275,   524,    30,   146,
     276,   528,    31,   144,   274,   718,   486,  1431,   862,  1432,
    1043,   487,   809,  1281,   810,  1282,   830,  1297,   827,  1295,
     644,   488,   828,  1296,   489,   816,  1288,   825,  1294,   645,
     490,  1415,   519,   491
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      61,    71,   333,   652,   654,   795,   802,   586,   819,   571,
     945,   517,  1076,   657,   268,  -116,   861,  1044,   701,   220,
     703,   708,   705,    85,   594,   706,   920,   131,   922,   947,
     924,  1074,   755,   756,    48,   273,  1078,   123,   124,    48,
      49,   598,  1363,   417,   418,    49,   507,   574,   732,  1161,
      71,    71,    71,  1429,  1170,   105,   106,   107,    86,   846,
     509,   668,   393,   424,   670,   671,   849,  1278,   850,   426,
    1226,   772,    32,    33,   734,    48,    53,   753,   753,   753,
    1227,    49,  1321,   735,  1451,   749,   350,   905,   906,   532,
     108,    71,    71,    71,    71,  1417,   763,   670,   671,  1402,
    1403,   351,    99,   393,   881,    90,   432,   433,   370,    38,
     763,  1410,  1411,   882,   599,   109,   907,   332,   773,   394,
       2,   218,    54,   150,   600,   908,     3,  1279,   940,  1228,
     940,   797,   533,   803,  1229,   371,   372,  1208,  1087,  1444,
     435,   436,   332,   691,   692,   982,   139,   719,   843,     4,
     940,     5,   571,     6,   138,    81,   817,   847,    50,     7,
    1313,   267,   218,   571,   348,   826,   909,   910,   829,     8,
      59,   917,    72,   601,   856,     9,   691,   692,   465,   751,
      59,   125,   180,  1187,   796,   918,   126,   942,   127,   942,
     574,   128,   381,   941,    60,   941,   860,    87,   373,    10,
     941,   574,   374,   508,    60,   406,   658,   659,    88,   942,
     110,   571,   535,  1092,   457,    15,   111,   510,  1089,    34,
     268,    11,    12,   268,   165,   166,   940,   525,  1188,   609,
    1452,  1049,   129,  1052,   914,   511,  1452,   268,  1222,   526,
    1059,   512,    62,  1062,  1452,   536,   463,   818,   985,   574,
     268,   224,   369,   268,   268,   268,  1319,  1092,   537,   763,
     831,   375,    78,  1453,   948,   376,  1452,  1064,   377,  1454,
     940,    63,   763,   949,   603,   604,   606,  1459,   225,   973,
    1087,  1320,  1378,   378,   941,   942,    99,   943,   940,   379,
     944,   976,  1090,    81,   753,   407,   940,    13,  1376,  1460,
      80,   981,   662,   663,  1276,    82,  1445,    41,    42,    43,
     668,   505,   669,   670,   671,   672,    14,    83,   808,  1091,
     673,   101,   102,   103,  1180,   350,   763,    15,   381,   942,
     506,   268,   268,  1032,   932,   268,   738,   268,    44,   268,
     351,   268,   571,    64,   747,    59,    93,   942,    73,    74,
      59,    75,   695,   696,   851,   942,   700,   571,   702,   852,
     704,  1118,   152,   153,   154,   155,   104,   267,   167,    60,
     267,   615,  1274,   168,    60,   169,    92,   932,   128,    76,
     574,   932,    59,   932,   267,  1095,   617,    95,   592,   984,
     616,   851,   691,   692,   853,   574,   624,   267,   652,  1275,
     267,   267,   267,   861,  1328,   618,    60,   267,  1334,    65,
    1368,   571,   381,   627,  1346,   625,   805,   412,    66,  1258,
    1232,   574,   574,   574,   574,   574,   574,   574,   574,   574,
     574,   574,   628,   574,   574,   574,   574,   574,   574,  1039,
    1040,  1108,  -713,    96,  1113,  1131,  1048,  -713,  1129,   574,
    1109,  1054,  1055,  1114,  1057,  1058,   268,  1060,  1061,  1336,
    1063,   925,  1173,  1174,  1175,  -713,  -720,  -727,  -393,   268,
    1372,  -720,  -727,  -393,  1224,   926,  1139,   726,   267,   267,
     927,  1133,   267,  1225,   267,  1133,   267,  1141,   267,  -720,
    -727,  -393,   727,  1133,   928,  1134,   763,   868,   872,  1210,
    1285,   381,   856,   162,   163,   806,  1314,  1211,   753,   753,
     753,  1187,   886,   763,  1286,   763,   332,   763,    97,   763,
     114,   763,   946,   763,   932,   763,   115,   763,   117,   763,
      98,   763,   969,   763,   118,   370,   763,   105,   763,   107,
     763,   334,   763,  1308,   763,   335,   763,   120,   977,   370,
     268,    99,   979,   121,   571,   763,   132,  1163,   268,   336,
     337,   268,   371,   372,   338,   339,   340,   341,  1164,  1165,
      88,   930,   410,  1433,  1413,   411,   371,   372,   412,   935,
    1438,  1439,   936,  1311,   137,  1339,   148,  1290,   141,  1177,
    1377,  1303,   574,    86,   142,  1178,  1304,   932,  1344,   814,
     381,   174,   932,   267,   915,   381,  1218,   381,   381,   916,
     134,   919,   921,   381,  1213,   381,   267,   923,  1214,  1186,
     381,   147,   381,   381,  1193,   373,  1387,  1426,   599,   374,
     221,   222,   658,   659,   162,   163,   840,   841,   600,   373,
    1238,   854,   157,   374,   105,   106,   107,   268,   268,   268,
     268,   160,  1247,   175,   268,   268,  1029,    41,    42,    43,
     268,   268,   763,   268,   268,   176,   268,   268,  1038,   268,
     177,  1041,    56,    57,    58,  1047,   178,   268,   268,   179,
    1196,   105,  1260,   221,   222,   223,   212,   601,   375,   465,
     751,  1273,   376,   135,   136,   377,   214,   267,   930,   930,
    1071,  1072,   375,  1446,  1358,   267,   376,   328,   267,   377,
     378,   218,   329,   330,  1217,   332,   379,   343,   344,   346,
     349,   355,   358,   389,   378,   356,   357,   361,   662,   663,
     379,   359,  1106,   368,   658,   659,   668,   360,   362,   670,
     671,   672,   363,   381,   364,   365,   673,  1407,   366,  1300,
     658,   659,   367,   382,   386,  1398,   390,   387,  1159,   388,
     391,   398,   402,   652,   268,   392,   409,  1168,   404,   586,
     408,   571,   494,   515,   529,   530,   658,   659,   531,   518,
     589,   596,   268,   597,   655,    15,   613,   607,   608,   614,
     619,   620,   621,   622,   267,   267,   267,   267,   623,   633,
     626,   267,   267,  1138,   735,  1427,   698,   267,   267,   574,
     267,   267,  1466,   267,   267,   629,   267,   630,   691,   692,
    1435,  1436,   634,  1437,   267,   267,   635,   639,   660,   661,
     662,   663,   636,   637,   640,   638,   525,   656,   668,   694,
     669,   670,   671,   672,   697,   717,   662,   663,   673,   720,
     674,   675,   721,   714,   668,   722,   669,   670,   671,   672,
     723,   724,   742,  1324,   673,   743,   744,   758,   759,   760,
     660,   661,   662,   663,   664,   762,   765,   665,   666,   667,
     668,   949,   669,   670,   671,   672,   766,   767,   768,   268,
     673,   770,   674,   675,   769,   794,  1406,   793,   676,   677,
     678,   800,  1409,   812,   679,   813,   686,   687,   688,   689,
     690,   267,   807,   863,   845,   848,   879,   931,   658,   659,
     691,   692,   932,   268,   688,   689,   690,   938,   934,   267,
     939,   772,  1259,   967,   970,   971,   691,   692,   972,   975,
     978,   680,  1028,   681,   682,   683,   684,   685,   686,   687,
     688,   689,   690,  1031,  1262,  1042,  1080,  1077,  1092,  1096,
    1094,  1098,   691,   692,  1081,  1093,  1082,  1083,  1086,  1097,
    1099,  1100,   268,  1101,  1116,  1111,  1397,  1112,  1121,  1136,
    1124,  1126,  1128,   992,   994,   996,   998,  1000,  1002,  1004,
    1006,  1008,  1010,  1254,  1013,  1015,  1017,  1019,  1021,  1023,
    1137,  1171,  1180,  1176,  1190,  1182,   658,   659,  1191,  1195,
    1198,  1197,   660,   661,   662,   663,   664,  1203,  1206,   665,
     666,   667,   668,  1322,   669,   670,   671,   672,  1209,  1240,
    1241,  1243,   673,  1248,   674,   675,   267,  1257,  1255,  1272,
     676,   677,   678,  1284,  1310,  1315,   679,  1298,  1316,  1312,
    1317,  1348,  1318,  1352,  1340,  1337,  1345,  1353,  1354,   483,
    1356,  1357,  1362,  1364,  1365,  1370,  1375,  1392,  1383,   504,
     267,  1408,   268,  1404,   268,  1421,  1414,  1440,  1441,   514,
    1442,  1443,  1450,   680,  1456,   681,   682,   683,   684,   685,
     686,   687,   688,   689,   690,  1326,   842,   130,    18,  1457,
      84,   588,   662,   663,   691,   692,   658,   659,   693,   595,
     668,  1458,   669,   670,   671,   672,   173,   133,  1394,   267,
     673,  1266,   151,  1267,   354,  1162,  1268,  1269,    24,  1393,
    1322,  1270,  1382,   764,  1350,  1396,  1400,    94,   730,   631,
     632,  1351,  1347,  1277,  1401,  1026,   611,   612,  1189,   370,
    1416,     0,     0,     0,     0,     0,     0,     0,   643,   646,
     647,   648,   649,   650,     0,     0,     0,     0,     0,   268,
       0,     0,     0,     0,     0,     0,   371,   372,   686,   687,
     688,   689,   690,     0,     0,   268,     0,     0,     0,     0,
    1388,     0,   691,   692,     0,     0,     0,     0,     0,     0,
     660,   661,   662,   663,   664,     0,     0,   665,   666,   667,
     668,     0,   669,   670,   671,   672,   716,     0,     0,   267,
     673,   267,   674,   675,     0,     0,     0,     0,   676,   677,
     678,     0,   725,     0,   679,     0,     0,     0,     0,   373,
     731,     0,  1465,   374,   737,     0,     0,   740,   741,     0,
       0,     0,     0,   746,     0,   750,   370,     0,   370,   757,
       0,     0,     0,     0,   761,     0,     0,     0,     0,     0,
       0,   680,     0,   681,   682,   683,   684,   685,   686,   687,
     688,   689,   690,   371,   372,   371,   372,     0,     0,     0,
       0,     0,   691,   692,  1050,     0,     0,   465,   751,     0,
       0,     0,   375,   804,     0,     0,   376,     0,  1051,   377,
       0,     0,     0,     0,     0,     0,   267,     0,     0,   643,
     820,     0,     0,   823,   378,     0,     0,     0,     0,     0,
     379,     0,   267,     0,   832,   833,   834,   835,   836,   837,
       0,     0,     0,     0,     0,     0,   373,     0,   373,   599,
     374,     0,   374,     0,     0,     0,     0,     0,     0,   600,
     854,   873,   874,     0,     0,   875,   876,   877,   878,     0,
     880,     0,   883,   884,   885,   887,   888,   889,   890,   891,
     892,   894,   895,   896,   897,   898,   899,   900,   901,   902,
     903,   904,     0,   912,     0,     0,     0,     0,     0,     0,
       0,   801,     0,     0,   854,     0,     0,     0,   601,   375,
       0,   375,     0,   376,     0,   376,   377,  1184,   377,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,   378,   737,     0,     0,   379,     0,   379,
     968,   370,     0,     0,     0,     0,     0,     0,     0,   974,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   980,     0,     0,     0,     0,   371,   372,
       0,     0,     0,     0,     0,   991,   993,   995,   997,   999,
    1001,  1003,  1005,  1007,  1009,  1011,  1012,  1014,  1016,  1018,
    1020,  1022,  1024,     0,  1025,     0,     0,     0,   226,     0,
       0,     0,     0,  1033,   227,     0,     0,     0,     0,     0,
     228,     0,     0,     0,     0,  1046,     0,     0,     0,     0,
     229,     0,     0,     0,     0,     0,     0,     0,   230,     0,
       0,   373,     0,     0,     0,   374,     0,     0,     0,     0,
       0,     0,  1073,   231,     0,  1075,   643,     0,     0,  1079,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,     0,     0,     0,   376,     0,
    1185,   377,     0,     0,     0,     0,     0,  1110,     0,     0,
       0,  1115,     0,     0,     0,     0,   378,     0,     0,     0,
       0,    59,   379,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1079,   265,     0,     0,     0,     0,  1135,
       0,     0,     0,     0,     0,    60,  1140,     0,     0,     0,
       0,  1142,  1143,  1144,  1145,  1146,  1147,  1148,  1149,  1150,
    1151,  1152,  1153,  1154,  1155,  1156,  1157,  1158,     0,     0,
       0,  1160,     0,     0,     0,     0,     0,     0,     0,   538,
       0,     0,   417,   418,     3,     0,   539,   540,   541,     0,
     542,   266,   419,   420,   421,   422,   423,     0,     0,     0,
       0,     0,   424,   543,   425,   544,   545,     0,   426,     0,
       0,     0,     0,     0,     0,   546,   427,     0,     0,   547,
       0,   548,   428,     0,     0,   429,     0,     8,   430,   549,
       0,   550,   431,     0,     0,   551,   552,     0,     0,     0,
       0,     0,   553,     0,     0,   432,   433,     0,   232,   233,
     234,     0,   236,   237,   238,   239,   240,   434,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,     0,
     254,   255,   256,     0,     0,   259,   260,   261,   262,   435,
     436,   437,   554,  1215,  1216,     0,   370,  1219,  1220,  1221,
       0,  1079,     0,     0,   438,   439,     0,     0,  1234,     0,
    1236,     0,  1239,     0,     0,     0,     0,     0,  1242,   555,
     556,   557,  1245,   371,   372,     0,     0,     0,     0,    59,
     370,     0,     0,     0,     0,     0,     0,   440,   441,   442,
     443,   444,     0,   445,     0,   446,   447,   448,   449,   450,
     451,   452,   453,    60,   558,   455,   456,   371,   372,     0,
       0,     0,     0,   457,   559,   560,     0,     0,     0,     0,
       0,     0,     0,   588,   370,     0,     0,  1263,     0,  1271,
     458,   459,   460,     0,    14,   370,   373,   461,   462,     0,
     374,     0,     0,     0,     0,   463,     0,   464,     0,   465,
     466,   371,   372,     0,   370,     0,     0,     0,   643,     0,
       0,     0,   371,   372,     0,     0,     0,     0,     0,     0,
     373,     0,     0,     0,   374,     0,     0,     0,     0,     0,
       0,   371,   372,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   375,
       0,     0,  1331,   376,  1332,  1194,   377,     0,     0,  1335,
       0,     0,     0,     0,   373,     0,     0,     0,   374,  1338,
       0,   378,     0,     0,     0,   373,  1343,   379,     0,   374,
       0,     0,     0,   375,     0,     0,     0,   376,     0,  1199,
     377,     0,     0,     0,   373,     0,     0,  1355,   374,     0,
       0,     0,  1359,  1360,  1361,   378,     0,     0,     0,  1367,
       0,   379,  1369,     0,   643,  1373,     0,     0,     0,     0,
       0,     0,     0,  1379,  1380,  1381,     0,   375,     0,     0,
       0,   376,  1385,  1200,   377,     0,     0,     0,   375,     0,
       0,     0,   376,     0,  1201,   377,     0,     0,     0,   378,
       0,     0,     0,  1395,     0,   379,     0,   375,     0,     0,
     378,   376,     0,  1202,   377,     0,   379,     0,   643,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   378,
       0,     0,     0,     0,     0,   379,     0,  1419,  1420,     0,
       0,     0,     0,     0,     0,  1423,     0,   538,     0,     0,
     417,   418,     3,     0,   539,   540,   541,     0,   542,  1428,
     419,   420,   421,   422,   423,  1434,     0,     0,     0,     0,
     424,   543,   425,   544,   545,     0,   426,     0,     0,     0,
       0,     0,     0,   546,   427,     0,     0,   547,  1448,   548,
     428,     0,     0,   429,     0,     8,   430,   549,     0,   550,
     431,     0,  1461,   551,   552,     0,  1462,  1463,     0,     0,
     553,     0,     0,   432,   433,     0,   232,   233,   234,     0,
     236,   237,   238,   239,   240,   434,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,     0,   254,   255,
     256,     0,     0,   259,   260,   261,   262,   435,   436,   437,
     554,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   438,   439,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   555,   556,   557,
       0,     0,     0,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,   440,   441,   442,   443,   444,
     370,   445,     0,   446,   447,   448,   449,   450,   451,   452,
     453,    60,   558,   455,   456,     0,     0,     0,     0,     0,
       0,   457,  1261,   560,     0,     0,     0,   371,   372,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   458,   459,
     460,     0,    14,     0,     0,   461,   462,     0,     0,   417,
     418,     0,     0,   463,     0,   464,     0,   465,   466,   419,
     420,   421,   422,   423,     0,     0,     0,     0,     0,   424,
       0,   425,     0,     0,     0,   426,     0,     0,     0,     0,
       0,     0,     0,   427,     0,     0,     0,     0,     0,   428,
     373,     0,   429,     0,   374,   430,     0,     0,     0,   431,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   432,   433,     0,   232,   233,   234,     0,   236,
     237,   238,   239,   240,   434,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,     0,   254,   255,   256,
       0,     0,   259,   260,   261,   262,   435,   436,   437,     0,
       0,     0,     0,   375,     0,     0,     0,   376,     0,  1204,
     377,   438,   439,     0,     0,     0,     0,     0,     0,     0,
     733,     0,     0,     0,     0,   378,   555,   556,   557,     0,
       0,   379,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,   440,   441,   442,   443,   444,   370,
     445,     0,   446,   447,   448,   449,   450,   451,   452,   453,
      60,   454,   455,   456,     0,     0,     0,     0,     0,     0,
     457,     0,     0,     0,     0,     0,   371,   372,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   458,   459,   460,
       0,    14,     0,     0,   461,   462,     0,     0,   417,   418,
       0,     0,   463,     0,   464,     0,   465,   466,   419,   420,
     421,   422,   423,     0,     0,     0,     0,     0,   424,     0,
     425,     0,     0,     0,   426,     0,     0,     0,     0,     0,
       0,     0,   427,     0,     0,     0,     0,     0,   428,   373,
       0,   429,     0,   374,   430,     0,     0,     0,   431,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   432,   433,     0,   232,   233,   234,     0,   236,   237,
     238,   239,   240,   434,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,     0,   254,   255,   256,     0,
       0,   259,   260,   261,   262,   435,   436,   437,     0,     0,
       0,     0,   375,     0,     0,     0,   376,     0,  1205,   377,
     438,   439,     0,     0,     0,     0,     0,     0,     0,   748,
       0,     0,     0,     0,   378,   555,   556,   557,     0,     0,
     379,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     0,     0,   440,   441,   442,   443,   444,   370,   445,
       0,   446,   447,   448,   449,   450,   451,   452,   453,    60,
     454,   455,   456,     0,     0,     0,     0,     0,     0,   457,
       0,     0,     0,     0,     0,   371,   372,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   458,   459,   460,     0,
      14,     0,     0,   461,   462,     0,     0,   417,   418,     0,
       0,   463,     0,   464,     0,   465,   466,   419,   420,   421,
     422,   423,     0,     0,     0,     0,     0,   424,  1264,   425,
     544,     0,     0,   426,     0,     0,     0,     0,     0,     0,
       0,   427,     0,     0,     0,     0,     0,   428,   373,     0,
     429,     0,   374,   430,   549,     0,     0,   431,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     432,   433,     0,   232,   233,   234,     0,   236,   237,   238,
     239,   240,   434,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,     0,   254,   255,   256,     0,     0,
     259,   260,   261,   262,   435,   436,   437,  1265,     0,     0,
       0,   375,     0,     0,     0,   376,     0,  1207,   377,   438,
     439,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   378,     0,     0,     0,     0,     0,   379,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,   440,   441,   442,   443,   444,   370,   445,     0,
     446,   447,   448,   449,   450,   451,   452,   453,    60,   454,
     455,   456,     0,     0,     0,     0,     0,     0,   457,     0,
       0,     0,     0,     0,   371,   372,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   458,   459,   460,     0,    14,
       0,     0,   461,   462,     0,     0,   417,   418,     0,     0,
     463,     0,   464,     0,   465,   466,   419,   420,   421,   422,
     423,     0,     0,     0,     0,     0,   424,     0,   425,     0,
       0,     0,   426,     0,     0,     0,     0,     0,     0,     0,
     427,     0,     0,     0,     0,     0,   428,   373,     0,   429,
       0,   374,   430,     0,     0,     0,   431,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   432,
     433,     0,   232,   233,   234,     0,   236,   237,   238,   239,
     240,   434,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,     0,   254,   255,   256,     0,     0,   259,
     260,   261,   262,   435,   436,   437,     0,     0,     0,     0,
     375,     0,     0,     0,   376,     0,  1289,   377,   438,   439,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,   555,   556,   557,     0,     0,   379,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,   440,   441,   442,   443,   444,   370,   445,     0,   446,
     447,   448,   449,   450,   451,   452,   453,    60,   454,   455,
     456,     0,     0,     0,     0,     0,     0,   457,     0,     0,
       0,     0,     0,   371,   372,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   458,   459,   460,     0,    14,     0,
       0,   461,   462,     0,     0,   417,   418,     0,     0,   463,
       0,   464,     0,   465,   466,   419,   420,   421,   422,   423,
       0,     0,     0,     0,     0,   424,     0,   425,     0,     0,
       0,   426,     0,     0,     0,     0,     0,     0,     0,   427,
       0,     0,     0,     0,     0,   428,   373,     0,   429,     0,
     374,   430,     0,     0,     0,   431,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   432,   433,
       0,   232,   233,   234,     0,   236,   237,   238,   239,   240,
     434,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,     0,   254,   255,   256,     0,     0,   259,   260,
     261,   262,   435,   436,   437,     0,     0,     0,     0,   375,
       0,     0,     0,   376,     0,  1307,   377,   438,   439,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,   986,   987,   988,     0,     0,   379,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
     440,   441,   442,   443,   444,   370,   445,     0,   446,   447,
     448,   449,   450,   451,   452,   453,    60,   454,   455,   456,
       0,     0,     0,     0,     0,     0,   457,     0,     0,     0,
       0,     0,   371,   372,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   458,   459,   460,     0,    14,     0,     0,
     461,   462,     0,     0,   417,   418,     0,     0,   463,     0,
     464,     0,   465,   466,   419,   420,   421,   422,   423,     0,
       0,     0,     0,     0,   424,     0,   425,     0,     0,     0,
     426,     0,     0,     0,     0,     0,     0,     0,   427,     0,
       0,     0,     0,     0,   428,   373,     0,   429,     0,   374,
     430,     0,     0,     0,   431,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,   433,     0,
     232,   233,   234,     0,   236,   237,   238,   239,   240,   434,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,     0,   254,   255,   256,     0,     0,   259,   260,   261,
     262,   435,   436,   437,     0,     0,     0,     0,   375,     0,
       0,     0,   376,     0,  1386,   377,   438,   439,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     378,     0,     0,     0,     0,     0,   379,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,   440,
     441,   442,   443,   444,     0,   445,   856,   446,   447,   448,
     449,   450,   451,   452,   453,   857,   454,   455,   456,     0,
       0,     0,     0,     0,     0,   457,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   458,   459,   460,     0,    14,     0,     0,   461,
     462,     0,     0,     0,     0,   417,   418,   858,     0,   464,
     859,   465,   466,   651,     0,   419,   420,   421,   422,   423,
       0,     0,     0,     0,     0,   424,     0,   425,     0,     0,
       0,   426,     0,     0,     0,     0,     0,     0,     0,   427,
       0,     0,     0,     0,     0,   428,     0,     0,   429,     0,
       0,   430,     0,     0,     0,   431,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   432,   433,
       0,   232,   233,   234,     0,   236,   237,   238,   239,   240,
     434,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,     0,   254,   255,   256,     0,     0,   259,   260,
     261,   262,   435,   436,   437,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   438,   439,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
     440,   441,   442,   443,   444,     0,   445,   856,   446,   447,
     448,   449,   450,   451,   452,   453,   857,   454,   455,   456,
       0,     0,     0,     0,     0,     0,   457,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   458,   459,   460,     0,    14,     0,     0,
     461,   462,     0,     0,   417,   418,     0,     0,   463,     0,
     464,     0,   465,   466,   419,   420,   421,   422,   423,     0,
       0,     0,     0,     0,   424,     0,   425,     0,     0,     0,
     426,     0,     0,     0,     0,     0,     0,     0,   427,     0,
       0,     0,     0,     0,   428,     0,     0,   429,     0,     0,
     430,     0,     0,     0,   431,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,   433,     0,
     232,   233,   234,     0,   236,   237,   238,   239,   240,   434,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,     0,   254,   255,   256,     0,     0,   259,   260,   261,
     262,   435,   436,   437,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   438,   439,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,   440,
     441,   442,   443,   444,     0,   445,     0,   446,   447,   448,
     449,   450,   451,   452,   453,    60,   454,   455,   456,     0,
       0,     0,     0,     0,     0,   457,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   458,   459,   460,     0,    14,     0,     0,   461,
     462,     0,     0,     0,     0,   417,   418,   463,   513,   464,
       0,   465,   466,   642,     0,   419,   420,   421,   422,   423,
       0,     0,     0,     0,     0,   424,     0,   425,     0,     0,
       0,   426,     0,     0,     0,     0,     0,     0,     0,   427,
       0,     0,     0,     0,     0,   428,     0,     0,   429,     0,
       0,   430,     0,     0,     0,   431,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   432,   433,
       0,   232,   233,   234,     0,   236,   237,   238,   239,   240,
     434,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,     0,   254,   255,   256,     0,     0,   259,   260,
     261,   262,   435,   436,   437,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   438,   439,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
     440,   441,   442,   443,   444,     0,   445,     0,   446,   447,
     448,   449,   450,   451,   452,   453,    60,   454,   455,   456,
       0,     0,     0,     0,     0,     0,   457,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   458,   459,   460,     0,    14,     0,     0,
     461,   462,     0,     0,     0,     0,   417,   418,   463,     0,
     464,     0,   465,   466,   651,     0,   419,   420,   421,   422,
     423,     0,     0,     0,     0,     0,   424,     0,   425,     0,
       0,     0,   426,     0,     0,     0,     0,     0,     0,     0,
     427,     0,     0,     0,     0,     0,   428,     0,     0,   429,
       0,     0,   430,     0,     0,     0,   431,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   432,
     433,     0,   232,   233,   234,     0,   236,   237,   238,   239,
     240,   434,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,     0,   254,   255,   256,     0,     0,   259,
     260,   261,   262,   435,   436,   437,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   438,   439,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,   440,   441,   442,   443,   444,     0,   445,     0,   446,
     447,   448,   449,   450,   451,   452,   453,    60,   454,   455,
     456,     0,     0,     0,     0,     0,     0,   457,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   458,   459,   460,     0,    14,     0,
       0,   461,   462,     0,     0,   417,   418,     0,     0,   463,
       0,   464,     0,   465,   466,   419,   420,   421,   422,   423,
       0,     0,     0,     0,     0,   424,     0,   425,     0,     0,
       0,   426,     0,     0,     0,     0,     0,     0,     0,   427,
       0,     0,     0,     0,     0,   428,     0,     0,   429,     0,
       0,   430,     0,     0,     0,   431,     0,     0,     0,     0,
       0,   739,     0,     0,     0,     0,     0,     0,   432,   433,
       0,   232,   233,   234,     0,   236,   237,   238,   239,   240,
     434,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,     0,   254,   255,   256,     0,     0,   259,   260,
     261,   262,   435,   436,   437,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   438,   439,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
     440,   441,   442,   443,   444,     0,   445,     0,   446,   447,
     448,   449,   450,   451,   452,   453,    60,   454,   455,   456,
       0,     0,     0,     0,     0,     0,   457,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   458,   459,   460,     0,    14,     0,     0,
     461,   462,     0,     0,   417,   418,     0,     0,   463,     0,
     464,     0,   465,   466,   419,   420,   421,   422,   423,     0,
       0,     0,     0,     0,   424,     0,   425,     0,     0,     0,
     426,     0,     0,     0,     0,     0,     0,     0,   427,     0,
       0,     0,     0,     0,   428,     0,     0,   429,     0,     0,
     430,     0,     0,     0,   431,     0,     0,   745,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,   433,     0,
     232,   233,   234,     0,   236,   237,   238,   239,   240,   434,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,     0,   254,   255,   256,     0,     0,   259,   260,   261,
     262,   435,   436,   437,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   438,   439,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,   440,
     441,   442,   443,   444,     0,   445,     0,   446,   447,   448,
     449,   450,   451,   452,   453,    60,   454,   455,   456,     0,
       0,     0,     0,     0,     0,   457,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   458,   459,   460,     0,    14,     0,     0,   461,
     462,     0,     0,   417,   418,     0,     0,   463,     0,   464,
       0,   465,   466,   419,   420,   421,   422,   423,     0,     0,
       0,     0,     0,   424,     0,   425,     0,     0,     0,   426,
       0,     0,     0,     0,     0,     0,     0,   427,     0,     0,
       0,     0,     0,   428,     0,     0,   429,     0,     0,   430,
       0,     0,     0,   431,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   432,   433,     0,   232,
     233,   234,     0,   236,   237,   238,   239,   240,   434,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
       0,   254,   255,   256,     0,     0,   259,   260,   261,   262,
     435,   436,   437,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   438,   439,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,   440,   441,
     442,   443,   444,     0,   445,     0,   446,   447,   448,   449,
     450,   451,   452,   453,    60,   454,   455,   456,     0,     0,
       0,     0,     0,     0,   457,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   639,
       0,   458,   459,   460,     0,    14,     0,     0,   461,   462,
       0,     0,   417,   418,     0,     0,   463,     0,   464,     0,
     465,   466,   419,   420,   421,   422,   423,     0,     0,   893,
       0,     0,   424,     0,   425,     0,     0,     0,   426,     0,
       0,     0,     0,     0,     0,     0,   427,     0,     0,     0,
       0,     0,   428,     0,     0,   429,     0,     0,   430,     0,
       0,     0,   431,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   432,   433,     0,   232,   233,
     234,     0,   236,   237,   238,   239,   240,   434,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,     0,
     254,   255,   256,     0,     0,   259,   260,   261,   262,   435,
     436,   437,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   438,   439,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     0,     0,   440,   441,   442,
     443,   444,     0,   445,     0,   446,   447,   448,   449,   450,
     451,   452,   453,    60,   454,   455,   456,     0,     0,     0,
       0,     0,     0,   457,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     458,   459,   460,     0,    14,     0,     0,   461,   462,     0,
       0,   417,   418,     0,     0,   463,     0,   464,     0,   465,
     466,   419,   420,   421,   422,   423,     0,     0,     0,     0,
       0,   424,     0,   425,     0,     0,     0,   426,     0,     0,
       0,     0,     0,     0,     0,   427,     0,     0,     0,     0,
       0,   428,     0,     0,   429,     0,     0,   430,     0,     0,
       0,   431,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   432,   433,     0,   232,   233,   234,
       0,   236,   237,   238,   239,   240,   434,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,     0,   254,
     255,   256,     0,     0,   259,   260,   261,   262,   435,   436,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   438,   439,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,   440,   441,   442,   443,
     444,     0,   445,     0,   446,   447,   448,   449,   450,   451,
     452,   453,    60,   454,   455,   456,     0,     0,     0,     0,
       0,     0,   457,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   458,
     459,   460,     0,    14,     0,     0,   461,   462,     0,     0,
     417,   418,     0,     0,   463,     0,   464,   913,   465,   466,
     419,   420,   421,   422,   423,     0,     0,     0,     0,     0,
     424,     0,   425,     0,     0,     0,   426,     0,     0,     0,
       0,     0,     0,     0,   427,     0,     0,     0,     0,     0,
     428,     0,     0,   429,     0,     0,   430,     0,     0,     0,
     431,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   432,   433,     0,   232,   233,   234,     0,
     236,   237,   238,   239,   240,   434,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,     0,   254,   255,
     256,     0,     0,   259,   260,   261,   262,   435,   436,   437,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   438,   439,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,   440,   441,   442,   443,   444,
       0,   445,     0,   446,   447,   448,   449,   450,   451,   452,
     453,    60,   454,   455,   456,     0,     0,     0,     0,     0,
       0,   457,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1045,     0,   458,   459,
     460,     0,    14,     0,     0,   461,   462,     0,     0,   417,
     418,     0,     0,   463,     0,   464,     0,   465,   466,   419,
     420,   421,   422,   423,     0,     0,     0,     0,     0,   424,
       0,   425,     0,     0,     0,   426,     0,     0,     0,     0,
       0,     0,     0,   427,     0,     0,     0,     0,     0,   428,
       0,     0,   429,     0,     0,   430,     0,     0,     0,   431,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   432,   433,     0,   232,   233,   234,     0,   236,
     237,   238,   239,   240,   434,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,     0,   254,   255,   256,
       0,     0,   259,   260,   261,   262,   435,   436,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   439,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,   440,   441,   442,   443,   444,     0,
     445,     0,   446,   447,   448,   449,   450,   451,   452,   453,
      60,   454,   455,   456,     0,     0,     0,     0,     0,     0,
     457,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   458,   459,   460,
       0,    14,     0,     0,   461,   462,     0,     0,   417,   418,
       0,     0,   463,     0,   464,  1237,   465,   466,   419,   420,
     421,   422,   423,     0,     0,     0,     0,     0,   424,     0,
     425,     0,     0,     0,   426,     0,     0,     0,     0,     0,
       0,     0,   427,     0,     0,     0,     0,     0,   428,     0,
       0,   429,     0,     0,   430,     0,     0,     0,   431,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   432,   433,     0,   232,   233,   234,     0,   236,   237,
     238,   239,   240,   434,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,     0,   254,   255,   256,     0,
       0,   259,   260,   261,   262,   435,   436,   437,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     438,   439,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     0,     0,   440,   441,   442,   443,   444,     0,   445,
       0,   446,   447,   448,   449,   450,   451,   452,   453,    60,
     454,   455,   456,     0,     0,     0,     0,     0,     0,   457,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   458,   459,   460,     0,
      14,     0,     0,   461,   462,     0,     0,   417,   418,     0,
       0,   463,     0,   464,  1246,   465,   466,   419,   420,   421,
     422,   423,     0,     0,     0,     0,     0,   424,     0,   425,
       0,     0,     0,   426,     0,     0,     0,     0,     0,     0,
       0,   427,     0,     0,     0,     0,     0,   428,     0,     0,
     429,     0,     0,   430,     0,     0,     0,   431,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     432,   433,     0,   232,   233,   234,     0,   236,   237,   238,
     239,   240,   434,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,     0,   254,   255,   256,     0,     0,
     259,   260,   261,   262,   435,   436,   437,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   438,
     439,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,   440,   441,   442,   443,   444,     0,   445,     0,
     446,   447,   448,   449,   450,   451,   452,   453,    60,   454,
     455,   456,     0,     0,     0,     0,     0,     0,   457,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   458,   459,   460,     0,    14,
       0,     0,   461,   462,     0,     0,   417,   418,     0,     0,
     463,     0,   464,  1299,   465,   466,   419,   420,   421,   422,
     423,     0,     0,     0,     0,     0,   424,     0,   425,     0,
       0,     0,   426,     0,     0,     0,     0,     0,     0,     0,
     427,     0,     0,     0,     0,     0,   428,     0,     0,   429,
       0,     0,   430,     0,     0,     0,   431,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   432,
     433,     0,   232,   233,   234,     0,   236,   237,   238,   239,
     240,   434,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,     0,   254,   255,   256,     0,     0,   259,
     260,   261,   262,   435,   436,   437,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   438,   439,
       0,     0,     0,     0,     0,     0,     0,  1341,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,   440,   441,   442,   443,   444,     0,   445,     0,   446,
     447,   448,   449,   450,   451,   452,   453,    60,   454,   455,
     456,     0,     0,     0,     0,     0,     0,   457,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   458,   459,   460,     0,    14,     0,
       0,   461,   462,     0,     0,   417,   418,     0,     0,   463,
       0,   464,     0,   465,   466,   419,   420,   421,   422,   423,
       0,     0,     0,     0,     0,   424,     0,   425,     0,     0,
       0,   426,     0,     0,     0,     0,     0,     0,     0,   427,
       0,     0,     0,     0,     0,   428,     0,     0,   429,     0,
       0,   430,     0,     0,     0,   431,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   432,   433,
       0,   232,   233,   234,     0,   236,   237,   238,   239,   240,
     434,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,     0,   254,   255,   256,     0,     0,   259,   260,
     261,   262,   435,   436,   437,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   438,   439,     0,
       0,     0,     0,     0,     0,     0,  1342,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
     440,   441,   442,   443,   444,     0,   445,     0,   446,   447,
     448,   449,   450,   451,   452,   453,    60,   454,   455,   456,
       0,     0,     0,     0,     0,     0,   457,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   458,   459,   460,     0,    14,     0,     0,
     461,   462,     0,     0,   417,   418,     0,     0,   463,     0,
     464,     0,   465,   466,   419,   420,   421,   422,   423,     0,
       0,     0,     0,     0,   424,     0,   425,     0,     0,     0,
     426,     0,     0,     0,     0,     0,     0,     0,   427,     0,
       0,     0,     0,     0,   428,     0,     0,   429,     0,     0,
     430,     0,     0,     0,   431,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,   433,     0,
     232,   233,   234,     0,   236,   237,   238,   239,   240,   434,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,     0,   254,   255,   256,     0,     0,   259,   260,   261,
     262,   435,   436,   437,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   438,   439,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,   440,
     441,   442,   443,   444,     0,   445,     0,   446,   447,   448,
     449,   450,   451,   452,   453,    60,   454,   455,   456,     0,
       0,     0,     0,     0,     0,   457,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   458,   459,   460,     0,    14,     0,     0,   461,
     462,     0,     0,   417,   418,     0,     0,   463,     0,   464,
    1366,   465,   466,   419,   420,   421,   422,   423,     0,     0,
       0,     0,     0,   424,     0,   425,     0,     0,     0,   426,
       0,     0,     0,     0,     0,     0,     0,   427,     0,     0,
       0,     0,     0,   428,     0,     0,   429,     0,     0,   430,
       0,     0,     0,   431,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   432,   433,     0,   232,
     233,   234,     0,   236,   237,   238,   239,   240,   434,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
       0,   254,   255,   256,     0,     0,   259,   260,   261,   262,
     435,   436,   437,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   438,   439,     0,     0,     0,
       0,     0,     0,     0,  1384,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,   440,   441,
     442,   443,   444,     0,   445,     0,   446,   447,   448,   449,
     450,   451,   452,   453,    60,   454,   455,   456,     0,     0,
       0,     0,     0,     0,   457,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   458,   459,   460,     0,    14,     0,     0,   461,   462,
       0,     0,   417,   418,     0,     0,   463,     0,   464,     0,
     465,   466,   419,   420,   421,   422,   423,     0,     0,     0,
       0,     0,   424,     0,   425,     0,     0,     0,   426,     0,
       0,     0,     0,     0,     0,     0,   427,     0,     0,     0,
       0,     0,   428,     0,     0,   429,     0,     0,   430,     0,
       0,     0,   431,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   432,   433,     0,   232,   233,
     234,     0,   236,   237,   238,   239,   240,   434,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,     0,
     254,   255,   256,     0,     0,   259,   260,   261,   262,   435,
     436,   437,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   438,   439,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     0,     0,   440,   441,   442,
     443,   444,     0,   445,     0,   446,   447,   448,   449,   450,
     451,   452,   453,    60,   454,   455,   456,     0,     0,     0,
       0,     0,     0,   457,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     458,   459,   460,     0,    14,     0,     0,   461,   462,     0,
       0,   417,   418,     0,     0,   463,     0,   464,     0,   465,
     466,   419,   420,   421,   422,   423,     0,     0,     0,     0,
       0,   424,     0,   425,     0,     0,     0,   426,     0,     0,
       0,     0,     0,     0,     0,   427,     0,     0,     0,     0,
       0,   428,     0,     0,   429,     0,     0,   430,     0,     0,
       0,   431,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   432,   433,     0,   232,   233,   234,
       0,   236,   237,   238,   239,   240,   434,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,     0,   254,
     255,   256,     0,     0,   259,   260,   261,   262,   435,   436,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   438,   439,   -67,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   658,   659,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,   440,   441,   442,   443,
     444,     0,   445,     0,   446,   447,   448,   449,   450,   451,
     452,   453,    60,   454,   455,   456,     0,   658,   659,     0,
       0,     0,   457,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   458,
     459,   460,     0,    14,     0,     0,   461,   462,     0,     0,
       0,     0,     0,     0,  1223,     0,   464,     0,   465,   466,
       0,     0,   774,   775,   776,   777,   778,   779,   780,   781,
     660,   661,   662,   663,   664,   782,   783,   665,   666,   667,
     668,   784,   669,   670,   671,   672,     0,     0,     0,     0,
     673,   785,   674,   675,   786,   787,     0,     0,   676,   677,
     678,   788,   789,   790,   679,     0,     0,     0,     0,     0,
       0,   660,   661,   662,   663,   664,     0,     0,   665,   666,
     667,   668,     0,   669,   670,   671,   672,     0,     0,     0,
       0,   673,     0,   674,   675,     0,     0,     0,     0,   676,
     791,   680,     0,   681,   682,   683,   684,   685,   686,   687,
     688,   689,   690,   226,     0,     0,     0,     0,     0,   227,
       0,     0,   691,   692,     0,   228,     0,   465,   751,     0,
       0,     0,     0,     0,     0,   229,     0,     0,     0,     0,
       0,     0,     0,   230,   681,   682,   683,   684,   685,   686,
     687,   688,   689,   690,     0,     0,     0,     0,   231,     0,
       0,     0,     0,   691,   692,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   226,
       0,     0,     0,     0,     0,   227,     0,     0,     0,     0,
       0,   228,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   229,     0,     0,     0,     0,    59,     0,     0,   230,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   265,
       0,     0,     0,     0,   231,     0,     0,     0,     0,     0,
      60,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   266,     0,   516,     0,
       0,     0,     0,     0,     0,     0,     0,   226,     0,     0,
     658,   659,     0,   227,     0,     0,     0,     0,     0,   228,
       0,     0,    59,     0,     0,     0,     0,     0,     0,   229,
       0,     0,     0,     0,     0,   265,     0,   230,     0,     0,
       0,     0,     0,     0,     0,     0,   520,     0,     0,     0,
       0,     0,   231,     0,     0,     0,     0,     0,   521,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   266,     0,     0,     0,   950,   951,   952,   953,
     954,   955,   956,   957,   660,   661,   662,   663,   664,   958,
     959,   665,   666,   667,   668,   960,   669,   670,   671,   672,
    -343,   658,   659,     0,   673,   785,   674,   675,   961,   962,
      59,     0,   676,   677,   678,   963,   964,   965,   679,     0,
       0,     0,     0,   265,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   520,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   966,   680,     0,   681,   682,   683,
     684,   685,   686,   687,   688,   689,   690,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   691,   692,     0,     0,
     266,   465,   751,     0,     0,     0,     0,   950,   951,   952,
     953,   954,   955,   956,   957,   660,   661,   662,   663,   664,
     958,   959,   665,   666,   667,   668,   960,   669,   670,   671,
     672,   658,   659,     0,     0,   673,   785,   674,   675,   961,
     962,     0,     0,   676,   677,   678,   963,   964,   965,   679,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   658,
     659,     0,     0,     0,     0,     0,     0,     0,  1034,     0,
       0,     0,     0,     0,     0,   966,   680,     0,   681,   682,
     683,   684,   685,   686,   687,   688,   689,   690,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   691,   692,     0,
       0,     0,   465,   751,     0,     0,     0,   950,   951,   952,
     953,   954,   955,   956,   957,   660,   661,   662,   663,   664,
     958,   959,   665,   666,   667,   668,   960,   669,   670,   671,
     672,     0,     0,     0,     0,   673,   785,   674,   675,   961,
     962,     0,     0,   676,   677,   678,   963,   964,   965,   679,
       0,     0,     0,   660,   661,   662,   663,   664,   658,   659,
     665,   666,   667,   668,     0,   669,   670,   671,   672,     0,
       0,     0,     0,   673,     0,   674,   675,     0,  1183,     0,
       0,   676,   677,   678,     0,   966,   680,   679,   681,   682,
     683,   684,   685,   686,   687,   688,   689,   690,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   691,   692,     0,
       0,     0,   465,   751,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   680,     0,   681,   682,   683,   684,
     685,   686,   687,   688,   689,   690,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   691,   692,     0,     0,   710,
       0,     0,   660,   661,   662,   663,   664,   658,   659,   665,
     666,   667,   668,     0,   669,   670,   671,   672,     0,     0,
       0,     0,   673,     0,   674,   675,     0,     0,     0,     0,
     676,   677,   678,     0,     0,     0,   679,     0,     0,     0,
       0,     0,   658,   659,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   680,     0,   681,   682,   683,   684,   685,
     686,   687,   688,   689,   690,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   691,   692,     0,     0,   799,     0,
       0,   660,   661,   662,   663,   664,     0,     0,   665,   666,
     667,   668,     0,   669,   670,   671,   672,     0,     0,     0,
       0,   673,     0,   674,   675,     0,     0,     0,     0,   676,
     677,   678,     0,     0,     0,   679,   660,   661,   662,   663,
     664,   658,   659,   665,   666,   667,   668,     0,   669,   670,
     671,   672,     0,     0,     0,     0,   673,     0,   674,   675,
       0,     0,     0,     0,   676,   677,   678,     0,     0,     0,
     679,     0,   680,     0,   681,   682,   683,   684,   685,   686,
     687,   688,   689,   690,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   691,   692,     0,     0,   937,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   680,     0,   681,
     682,   683,   684,   685,   686,   687,   688,   689,   690,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   691,   692,
       0,     0,  1053,     0,     0,   660,   661,   662,   663,   664,
     658,   659,   665,   666,   667,   668,     0,   669,   670,   671,
     672,     0,     0,     0,     0,   673,     0,   674,   675,     0,
       0,     0,     0,   676,   677,   678,     0,     0,     0,   679,
       0,     0,     0,     0,     0,   658,   659,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   680,     0,   681,   682,
     683,   684,   685,   686,   687,   688,   689,   690,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   691,   692,     0,
       0,  1056,     0,     0,   660,   661,   662,   663,   664,     0,
       0,   665,   666,   667,   668,     0,   669,   670,   671,   672,
       0,     0,     0,     0,   673,     0,   674,   675,     0,     0,
       0,     0,   676,   677,   678,     0,     0,     0,   679,   660,
     661,   662,   663,   664,   658,   659,   665,   666,   667,   668,
       0,   669,   670,   671,   672,     0,     0,     0,     0,   673,
       0,   674,   675,     0,     0,     0,     0,   676,   677,   678,
       0,     0,     0,   679,     0,   680,     0,   681,   682,   683,
     684,   685,   686,   687,   688,   689,   690,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   691,   692,     0,     0,
    1065,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     680,     0,   681,   682,   683,   684,   685,   686,   687,   688,
     689,   690,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   691,   692,     0,     0,  1066,     0,     0,   660,   661,
     662,   663,   664,   658,   659,   665,   666,   667,   668,     0,
     669,   670,   671,   672,     0,     0,     0,     0,   673,     0,
     674,   675,     0,     0,     0,     0,   676,   677,   678,     0,
       0,     0,   679,     0,     0,     0,     0,     0,   658,   659,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   680,
       0,   681,   682,   683,   684,   685,   686,   687,   688,   689,
     690,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     691,   692,     0,     0,  1067,     0,     0,   660,   661,   662,
     663,   664,     0,     0,   665,   666,   667,   668,     0,   669,
     670,   671,   672,     0,     0,     0,     0,   673,     0,   674,
     675,     0,     0,     0,     0,   676,   677,   678,     0,     0,
       0,   679,   660,   661,   662,   663,   664,   658,   659,   665,
     666,   667,   668,     0,   669,   670,   671,   672,     0,     0,
       0,     0,   673,     0,   674,   675,     0,     0,     0,     0,
     676,   677,   678,     0,     0,     0,   679,     0,   680,     0,
     681,   682,   683,   684,   685,   686,   687,   688,   689,   690,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   691,
     692,     0,     0,  1068,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   680,     0,   681,   682,   683,   684,   685,
     686,   687,   688,   689,   690,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   691,   692,     0,     0,  1069,     0,
       0,   660,   661,   662,   663,   664,   658,   659,   665,   666,
     667,   668,     0,   669,   670,   671,   672,     0,     0,     0,
       0,   673,     0,   674,   675,     0,     0,     0,     0,   676,
     677,   678,     0,     0,     0,   679,     0,     0,     0,     0,
       0,   658,   659,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   680,     0,   681,   682,   683,   684,   685,   686,
     687,   688,   689,   690,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   691,   692,     0,     0,  1070,     0,     0,
     660,   661,   662,   663,   664,     0,     0,   665,   666,   667,
     668,     0,   669,   670,   671,   672,     0,     0,     0,     0,
     673,     0,   674,   675,     0,     0,     0,     0,   676,   677,
     678,     0,     0,     0,   679,   660,   661,   662,   663,   664,
     658,   659,   665,   666,   667,   668,     0,   669,   670,   671,
     672,     0,     0,     0,     0,   673,     0,   674,   675,     0,
       0,     0,     0,   676,   677,   678,     0,     0,     0,   679,
       0,   680,     0,   681,   682,   683,   684,   685,   686,   687,
     688,   689,   690,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   691,   692,     0,     0,  1192,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   680,     0,   681,   682,
     683,   684,   685,   686,   687,   688,   689,   690,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   691,   692,     0,
       0,  1256,     0,     0,   660,   661,   662,   663,   664,   658,
     659,   665,   666,   667,   668,     0,   669,   670,   671,   672,
       0,     0,     0,     0,   673,     0,   674,   675,     0,     0,
       0,     0,   676,   677,   678,     0,     0,     0,   679,     0,
       0,     0,     0,     0,   658,   659,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   680,     0,   681,   682,   683,
     684,   685,   686,   687,   688,   689,   690,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   691,   692,     0,     0,
    1305,     0,     0,   660,   661,   662,   663,   664,     0,     0,
     665,   666,   667,   668,     0,   669,   670,   671,   672,     0,
       0,     0,     0,   673,     0,   674,   675,     0,     0,     0,
       0,   676,   677,   678,     0,     0,     0,   679,   660,   661,
     662,   663,   664,   658,   659,   665,   666,   667,   668,     0,
     669,   670,   671,   672,     0,     0,     0,     0,   673,     0,
     674,   675,     0,     0,     0,     0,   676,   677,   678,     0,
       0,     0,   679,     0,   680,     0,   681,   682,   683,   684,
     685,   686,   687,   688,   689,   690,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   691,   692,     0,     0,  1306,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   680,
       0,   681,   682,   683,   684,   685,   686,   687,   688,   689,
     690,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     691,   692,     0,     0,  1309,     0,     0,   660,   661,   662,
     663,   664,   658,   659,   665,   666,   667,   668,     0,   669,
     670,   671,   672,     0,     0,     0,     0,   673,     0,   674,
     675,     0,     0,     0,     0,   676,   677,   678,     0,     0,
       0,   679,     0,     0,     0,     0,     0,   658,   659,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   680,     0,
     681,   682,   683,   684,   685,   686,   687,   688,   689,   690,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   691,
     692,     0,     0,  1325,     0,     0,   660,   661,   662,   663,
     664,     0,     0,   665,   666,   667,   668,     0,   669,   670,
     671,   672,     0,     0,     0,     0,   673,     0,   674,   675,
       0,     0,     0,     0,   676,   677,   678,     0,     0,     0,
     679,   660,   661,   662,   663,   664,   658,   659,   665,   666,
     667,   668,     0,   669,   670,   671,   672,     0,     0,     0,
       0,   673,     0,   674,   675,     0,     0,     0,     0,   676,
     677,   678,     0,     0,     0,   679,     0,   680,     0,   681,
     682,   683,   684,   685,   686,   687,   688,   689,   690,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   691,   692,
       0,     0,  1327,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   680,     0,   681,   682,   683,   684,   685,   686,
     687,   688,   689,   690,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   691,   692,     0,     0,  1329,     0,     0,
     660,   661,   662,   663,   664,   658,   659,   665,   666,   667,
     668,     0,   669,   670,   671,   672,     0,     0,     0,     0,
     673,     0,   674,   675,     0,     0,     0,     0,   676,   677,
     678,     0,     0,     0,   679,     0,     0,     0,     0,     0,
     658,   659,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   680,     0,   681,   682,   683,   684,   685,   686,   687,
     688,   689,   690,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   691,   692,     0,     0,  1333,     0,     0,   660,
     661,   662,   663,   664,     0,     0,   665,   666,   667,   668,
       0,   669,   670,   671,   672,     0,     0,     0,     0,   673,
       0,   674,   675,     0,     0,     0,     0,   676,   677,   678,
       0,     0,     0,   679,   660,   661,   662,   663,   664,   658,
     659,   665,   666,   667,   668,     0,   669,   670,   671,   672,
       0,     0,     0,     0,   673,     0,   674,   675,     0,     0,
       0,     0,   676,   677,   678,     0,     0,     0,   679,     0,
     680,     0,   681,   682,   683,   684,   685,   686,   687,   688,
     689,   690,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   691,   692,     0,     0,  1389,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   680,     0,   681,   682,   683,
     684,   685,   686,   687,   688,   689,   690,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   691,   692,     0,     0,
    1390,     0,     0,   660,   661,   662,   663,   664,   658,   659,
     665,   666,   667,   668,     0,   669,   670,   671,   672,     0,
       0,     0,     0,   673,     0,   674,   675,     0,     0,     0,
       0,   676,   677,   678,     0,     0,     0,   679,     0,     0,
       0,     0,     0,   658,   659,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   680,     0,   681,   682,   683,   684,
     685,   686,   687,   688,   689,   690,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   691,   692,     0,     0,  1391,
       0,     0,   660,   661,   662,   663,   664,     0,     0,   665,
     666,   667,   668,     0,   669,   670,   671,   672,     0,     0,
       0,     0,   673,     0,   674,   675,     0,     0,     0,     0,
     676,   677,   678,     0,     0,     0,   679,   660,   661,   662,
     663,   664,   658,   659,   665,   666,   667,   668,     0,   669,
     670,   671,   672,     0,     0,     0,     0,   673,     0,   674,
     675,     0,     0,     0,     0,   676,   677,   678,     0,     0,
       0,   679,     0,   680,     0,   681,   682,   683,   684,   685,
     686,   687,   688,   689,   690,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   691,   692,     0,     0,  1405,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   680,     0,
     681,   682,   683,   684,   685,   686,   687,   688,   689,   690,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   691,
     692,     0,     0,  1412,     0,     0,   660,   661,   662,   663,
     664,   658,   659,   665,   666,   667,   668,     0,   669,   670,
     671,   672,     0,     0,     0,     0,   673,     0,   674,   675,
       0,     0,     0,     0,   676,   677,   678,     0,     0,     0,
     679,     0,     0,     0,     0,     0,   658,   659,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   680,     0,   681,
     682,   683,   684,   685,   686,   687,   688,   689,   690,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   691,   692,
       0,     0,  1422,     0,     0,   660,   661,   662,   663,   664,
       0,     0,   665,   666,   667,   668,     0,   669,   670,   671,
     672,     0,     0,     0,     0,   673,     0,   674,   675,     0,
       0,     0,     0,   676,   677,   678,     0,     0,     0,   679,
     660,   661,   662,   663,   664,   658,   659,   665,   666,   667,
     668,     0,   669,   670,   671,   672,     0,     0,     0,     0,
     673,     0,   674,   675,     0,     0,     0,     0,   676,   677,
     678,     0,     0,     0,   679,     0,   680,     0,   681,   682,
     683,   684,   685,   686,   687,   688,   689,   690,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   691,   692,     0,
       0,  1455,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   680,     0,   681,   682,   683,   684,   685,   686,   687,
     688,   689,   690,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   691,   692,   715,     0,     0,     0,     0,   660,
     661,   662,   663,   664,   658,   659,   665,   666,   667,   668,
       0,   669,   670,   671,   672,     0,     0,     0,     0,   673,
       0,   674,   675,     0,     0,     0,     0,   676,   677,   678,
       0,     0,     0,   679,     0,     0,     0,     0,     0,   658,
     659,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     680,     0,   681,   682,   683,   684,   685,   686,   687,   688,
     689,   690,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   691,   692,   933,     0,     0,     0,     0,   660,   661,
     662,   663,   664,     0,     0,   665,   666,   667,   668,     0,
     669,   670,   671,   672,     0,     0,     0,     0,   673,     0,
     674,   675,     0,     0,     0,     0,   676,   677,   678,     0,
       0,     0,   679,   660,   661,   662,   663,   664,   658,   659,
     665,   666,   667,   668,     0,   669,   670,   671,   672,     0,
       0,     0,     0,   673,     0,   674,   675,     0,     0,     0,
       0,   676,   677,   678,     0,     0,     0,   679,     0,   680,
       0,   681,   682,   683,   684,   685,   686,   687,   688,   689,
     690,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     691,   692,  1102,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   680,     0,   681,   682,   683,   684,
     685,   686,   687,   688,   689,   690,     0,     0,     0,   865,
       0,     0,     0,     0,     0,   691,   692,  1117,     0,     0,
       0,     0,   660,   661,   662,   663,   664,   658,   659,   665,
     666,   667,   668,     0,   669,   670,   671,   672,     0,     0,
       0,     0,   673,     0,   674,   675,     0,     0,     0,     0,
     676,   677,   678,     0,     0,     0,   679,     0,     0,     0,
       0,   232,   233,   234,     0,   236,   237,   238,   239,   240,
     434,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,     0,   254,   255,   256,     0,     0,   259,   260,
     261,   262,     0,   680,     0,   681,   682,   683,   684,   685,
     686,   687,   688,   689,   690,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   691,   692,  1244,     0,     0,     0,
       0,   660,   661,   662,   663,   664,     0,     0,   665,   666,
     667,   668,     0,   669,   670,   671,   672,   277,   278,     0,
       0,   673,     0,   674,   675,     0,     0,   866,     0,   676,
     677,   678,     0,     0,   279,   679,   867,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   658,   659,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   680,     0,   681,   682,   683,   684,   685,   686,
     687,   688,   689,   690,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   691,   692,  1249,     0,     0,     0,     0,
       0,     0,     0,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,     0,     0,   298,   299,   300,     0,     0,     0,     0,
       0,     0,   301,   302,   303,   304,   305,     0,     0,   306,
     307,   308,   309,   310,   311,   312,   660,   661,   662,   663,
     664,   658,   659,   665,   666,   667,   668,     0,   669,   670,
     671,   672,     0,     0,     0,     0,   673,     0,   674,   675,
       0,     0,   844,     0,   676,   677,   678,     0,     0,     0,
     679,     0,   313,     0,   314,   315,   316,   317,   318,   319,
     320,   321,   322,   323,     0,     0,   324,   325,     0,     0,
       0,     0,     0,   326,   327,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   680,     0,   681,
     682,   683,   684,   685,   686,   687,   688,   689,   690,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   691,   692,
       0,     0,     0,     0,     0,   660,   661,   662,   663,   664,
     658,   659,   665,   666,   667,   668,     0,   669,   670,   671,
     672,     0,     0,     0,     0,   673,     0,   674,   675,     0,
       0,     0,     0,   676,   677,   678,     0,     0,     0,   679,
       0,     0,     0,     0,     0,   658,   659,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   332,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   680,     0,   681,   682,
     683,   684,   685,   686,   687,   688,   689,   690,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   691,   692,     0,
       0,     0,     0,     0,   660,   661,   662,   663,   664,     0,
       0,   665,   666,   667,   668,     0,   669,   670,   671,   672,
       0,     0,     0,     0,   673,     0,   674,   675,     0,     0,
       0,     0,   676,   677,   678,     0,     0,     0,   679,   660,
     661,   662,   663,   664,   658,   659,   665,   666,   667,   668,
       0,   669,   670,   671,   672,     0,     0,     0,     0,   673,
       0,   674,   675,     0,     0,     0,     0,   676,   677,   678,
       0,     0,     0,   679,     0,   680,  1107,   681,   682,   683,
     684,   685,   686,   687,   688,   689,   690,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   691,   692,  1212,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     680,     0,   681,   682,   683,   684,   685,   686,   687,   688,
     689,   690,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   691,   692,     0,     0,     0,     0,     0,   660,   661,
     662,   663,   664,   658,   659,   665,   666,   667,   668,     0,
     669,   670,   671,   672,     0,     0,     0,     0,   673,     0,
     674,   675,     0,     0,     0,     0,   676,   677,   678,     0,
       0,     0,   679,     0,     0,     0,     0,     0,     0,   658,
     659,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1414,     0,     0,     0,     0,     0,     0,     0,   680,
       0,   681,   682,   683,   684,   685,   686,   687,   688,   689,
     690,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     691,   692,     0,     0,     0,     0,     0,   660,   661,   662,
     663,   664,     0,     0,   665,   666,   667,   668,     0,   669,
     670,   671,   672,     0,     0,     0,     0,   673,     0,   674,
     675,     0,     0,     0,     0,   676,   677,   678,     0,     0,
       0,   679,     0,   660,   661,   662,   663,   664,   658,   659,
     665,   666,   667,   668,     0,   669,   670,   671,   672,     0,
       0,     0,     0,   673,     0,   674,   675,     0,     0,     0,
    1449,   676,   677,   678,     0,     0,     0,   679,   680,     0,
     681,   682,   683,   684,   685,   686,   687,   688,   689,   690,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   691,
     692,     0,     0,     0,     0,     0,  1464,     0,     0,     0,
       0,     0,     0,     0,   680,     0,   681,   682,   683,   684,
     685,   686,   687,   688,   689,   690,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   691,   692,     0,     0,     0,
       0,     0,   660,   661,   662,   663,   664,   658,   659,   665,
     666,   667,   668,     0,   669,   670,   671,   672,     0,     0,
       0,     0,   673,     0,   674,   675,     0,     0,     0,     0,
     676,   677,   678,     0,     0,     0,   679,     0,     0,     0,
       0,     0,   658,   659,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   680,     0,   681,   682,   683,   684,   685,
     686,   687,   688,   689,   690,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   691,   692,     0,     0,     0,     0,
       0,   660,   661,   662,   663,   664,     0,     0,   665,   666,
     667,   668,     0,   669,   670,   671,   672,     0,     0,     0,
       0,   673,     0,   674,   675,     0,     0,     0,     0,   676,
     677,   678,     0,     0,     0,  -728,   660,   661,   662,   663,
     664,   658,   659,   665,   666,   667,   668,     0,   669,   670,
     671,   672,     0,     0,     0,     0,   673,     0,   674,   675,
       0,     0,     0,     0,   676,   677,   678,     0,     0,     0,
       0,     0,   680,     0,   681,   682,   683,   684,   685,   686,
     687,   688,   689,   690,   658,   659,     0,     0,     0,     0,
       0,     0,     0,   691,   692,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   680,     0,   681,
     682,   683,   684,   685,   686,   687,   688,   689,   690,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   691,   692,
       0,     0,     0,     0,     0,   660,   661,   662,   663,   664,
       0,     0,   665,   666,   667,   668,     0,   669,   670,   671,
     672,     0,     0,     0,     0,   673,     0,   674,   675,     0,
       0,     0,     0,   676,     0,   678,     0,     0,     0,     0,
       0,   658,   659,     0,     0,     0,     0,     0,   660,   661,
     662,   663,   664,     0,     0,   665,   666,   667,   668,     0,
     669,   670,   671,   672,     0,   658,   659,     0,   673,     0,
     674,   675,     0,     0,     0,     0,     0,     0,   681,   682,
     683,   684,   685,   686,   687,   688,   689,   690,     0,   658,
     659,     0,     0,     0,     0,     0,     0,   691,   692,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   681,   682,   683,   684,   685,   686,   687,   688,   689,
     690,     0,     0,     0,     0,   660,   661,   662,   663,   664,
     691,   692,   665,   666,   667,   668,     0,   669,   670,   671,
     672,     0,     0,     0,     0,   673,     0,   674,   675,   660,
     661,   662,   663,   664,     0,     0,   665,   666,   667,   668,
       0,   669,   670,   671,   672,     0,   658,   659,     0,   673,
       0,   674,   675,   660,   661,   662,   663,   664,     0,     0,
     665,   666,   667,   668,     0,   669,   670,   671,   672,     0,
       0,     0,     0,   673,     0,   674,   675,     0,     0,   682,
     683,   684,   685,   686,   687,   688,   689,   690,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   691,   692,     0,
       0,     0,     0,     0,   683,   684,   685,   686,   687,   688,
     689,   690,   869,     0,     0,     0,     0,     0,     0,     0,
       0,   691,   692,     0,     0,     0,     0,     0,     0,   684,
     685,   686,   687,   688,   689,   690,     0,     0,     0,     0,
     660,   661,   662,   663,   664,   691,   692,   665,     0,     0,
     668,     0,   669,   670,   671,   672,     0,     0,     0,     0,
     673,     0,   674,   675,   232,   233,   234,     0,   236,   237,
     238,   239,   240,   434,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,     0,   254,   255,   256,     0,
       0,   259,   260,   261,   262,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1103,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   684,   685,   686,   687,
     688,   689,   690,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   691,   692,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     870,   181,     0,     0,     0,     0,   232,   233,   234,   871,
     236,   237,   238,   239,   240,   434,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,     0,   254,   255,
     256,     0,     0,   259,   260,   261,   262,   182,     0,   183,
       0,   184,   185,   186,   187,   188,     0,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,     0,   200,
     201,   202,     0,     0,   203,   204,   205,   206,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   207,   208,     0,     0,     0,     0,     0,
       0,     0,  1104,     0,     0,     0,     0,     0,     0,     0,
       0,  1105,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   209
};

static const yytype_int16 yycheck[] =
{
      14,    15,   211,   463,   464,   591,   598,   400,   618,   400,
     729,   381,   845,   475,   175,     7,   656,   813,   497,   161,
     499,   503,   501,    21,   406,   502,   701,    81,   703,    19,
     705,    19,   556,   557,    56,   175,    19,    14,    15,    56,
      62,   411,  1295,     4,     5,    62,    32,   400,   542,   976,
      64,    65,    66,  1399,   981,   142,   143,   144,    56,   645,
      32,   124,   153,    24,   127,   128,   652,    45,   654,    30,
     125,   130,    18,    19,   543,    56,   165,   555,   556,   557,
     135,    62,   140,   543,  1430,   554,   150,   127,   128,     6,
     183,   105,   106,   107,   108,  1374,   574,   127,   128,  1352,
    1353,   165,   141,   153,   156,    50,    67,    68,    32,   158,
     588,  1364,  1365,   165,   125,   208,   156,   175,   177,   210,
       0,   212,   211,   210,   135,   165,     6,   105,   125,   184,
     125,   593,    49,   602,   189,    59,    60,  1064,   135,  1418,
     101,   102,   175,   206,   207,   209,    91,   186,   642,    29,
     125,    31,   543,    33,   176,   183,   616,   651,   175,    39,
     135,   175,   212,   554,   218,   625,   206,   207,   628,    49,
     141,   177,   177,   184,   156,    55,   206,   207,   211,   212,
     141,   158,   210,   165,   176,   191,   163,   184,   165,   184,
     543,   168,   187,   183,   165,   183,   656,   195,   122,    79,
     183,   554,   126,   189,   165,   347,    20,    21,   206,   184,
      56,   602,   177,   183,   175,   207,    62,   189,   858,   165,
     381,   101,   102,   384,    14,    15,   125,   165,   210,   190,
     177,   817,   209,   819,   694,   207,   177,   398,   208,   177,
     826,   213,    33,   829,   177,   210,   207,   617,   772,   602,
     411,   183,   266,   414,   415,   416,   183,   183,   398,   737,
     630,   185,    61,   210,   733,   189,   177,   191,   192,   210,
     125,    62,   750,   733,   414,   415,   416,   210,   210,   748,
     135,   208,   208,   207,   183,   184,   141,   186,   125,   213,
     189,   753,   183,   183,   772,   349,   125,   177,   135,   210,
     184,   763,   116,   117,    46,   186,   135,   165,   166,   167,
     124,   190,   126,   127,   128,   129,   196,   183,   177,   210,
     134,    64,    65,    66,    66,   150,   804,   207,   187,   184,
     209,   492,   493,   802,   183,   496,   545,   498,   196,   500,
     165,   502,   733,   134,   553,   141,   177,   184,     4,     5,
     141,     7,   492,   493,   150,   184,   496,   748,   498,   155,
     500,   210,   105,   106,   107,   108,   209,   381,   158,   165,
     384,   190,   183,   163,   165,   165,   165,   183,   168,    35,
     733,   183,   141,   183,   398,   864,   190,   165,   402,   771,
     209,   150,   206,   207,   190,   748,   190,   411,   858,   210,
     414,   415,   416,  1043,   210,   209,   165,   421,   210,   200,
     210,   802,   187,   190,   186,   209,   191,   189,   209,  1138,
    1095,   774,   775,   776,   777,   778,   779,   780,   781,   782,
     783,   784,   209,   786,   787,   788,   789,   790,   791,   809,
     810,   156,   186,   165,   156,   927,   816,   191,   925,   802,
     165,   821,   822,   165,   824,   825,   617,   827,   828,  1255,
     830,   177,   986,   987,   988,   209,   186,   186,   186,   630,
    1303,   191,   191,   191,   156,   191,   945,   150,   492,   493,
     177,   177,   496,   165,   498,   177,   500,   947,   502,   209,
     209,   209,   165,   177,   191,   191,   974,   658,   659,   191,
     177,   187,   156,   169,   170,   191,  1225,   191,   986,   987,
     988,   165,   673,   991,   191,   993,   175,   995,   165,   997,
      56,   999,   731,  1001,   183,  1003,    62,  1005,    56,  1007,
     165,  1009,   741,  1011,    62,    32,  1014,   142,  1016,   144,
    1018,    74,  1020,  1218,  1022,    78,  1024,    56,   757,    32,
     711,   141,   761,    62,   945,  1033,   158,    11,   719,    92,
      93,   722,    59,    60,    97,    98,    99,   100,    22,    23,
     206,   711,   183,  1403,  1370,   186,    59,    60,   189,   719,
    1410,  1411,   722,  1223,   177,  1260,   169,  1197,    56,    56,
    1309,   177,   945,    56,    62,    62,   177,   183,  1273,   613,
     187,   177,   183,   617,   191,   187,  1085,   187,   187,   191,
     165,   191,   191,   187,  1074,   187,   630,   191,  1078,   191,
     187,   165,   187,   187,   191,   122,   191,   191,   125,   126,
     169,   170,    20,    21,   169,   170,   171,   172,   135,   122,
    1100,   655,   165,   126,   142,   143,   144,   808,   809,   810,
     811,   175,  1112,   184,   815,   816,   798,   165,   166,   167,
     821,   822,  1140,   824,   825,   175,   827,   828,   808,   830,
     175,   811,    10,    11,    12,   815,   175,   838,   839,   169,
    1050,   142,  1161,   169,   170,   171,   186,   184,   185,   211,
     212,  1170,   189,    87,    88,   192,   105,   711,   838,   839,
     838,   839,   185,  1422,  1290,   719,   189,    34,   722,   192,
     207,   212,    34,   209,  1084,   175,   213,   175,   165,   176,
     212,   190,   209,   208,   207,   190,   190,   209,   116,   117,
     213,   190,   893,   209,    20,    21,   124,   190,   190,   127,
     128,   129,   190,   187,   190,   190,   134,  1357,   190,  1209,
      20,    21,   190,   165,   165,  1347,    21,   165,   967,   165,
     165,   186,   186,  1223,   925,   208,    42,    11,   175,  1162,
     209,  1162,   165,   207,   165,   135,    20,    21,   209,   183,
     165,   165,   943,   165,   212,   207,   209,   190,   190,   190,
     209,   190,   190,   209,   808,   809,   810,   811,   190,   209,
     190,   815,   816,   943,  1264,  1397,   165,   821,   822,  1162,
     824,   825,  1452,   827,   828,   190,   830,   190,   206,   207,
    1406,  1407,   209,  1409,   838,   839,   209,   190,   114,   115,
     116,   117,   209,   209,   190,   209,   165,   209,   124,   209,
     126,   127,   128,   129,   210,   165,   116,   117,   134,   177,
     136,   137,   176,   208,   124,   186,   126,   127,   128,   129,
     177,   176,   165,  1233,   134,   177,   158,    36,   177,   177,
     114,   115,   116,   117,   118,     9,   177,   121,   122,   123,
     124,  1341,   126,   127,   128,   129,   177,   177,   177,  1050,
     134,    65,   136,   137,   177,   183,  1356,   184,   142,   143,
     144,    42,  1362,   209,   148,   209,   192,   193,   194,   195,
     196,   925,   191,   207,   183,   183,   165,   210,    20,    21,
     206,   207,   183,  1084,   194,   195,   196,   209,   176,   943,
      42,   130,  1141,    13,   184,   186,   206,   207,   158,   212,
     175,   185,     7,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   165,  1163,    69,   208,   176,   183,   190,
     207,   190,   206,   207,   210,   210,   209,   209,   209,   209,
     209,   209,  1133,   209,     1,   209,  1346,   209,   165,   165,
     191,   191,   191,   775,   776,   777,   778,   779,   780,   781,
     782,   783,   784,  1133,   786,   787,   788,   789,   790,   791,
     165,   165,    66,   176,   210,   177,    20,    21,   165,   210,
     210,   209,   114,   115,   116,   117,   118,   210,   210,   121,
     122,   123,   124,  1232,   126,   127,   128,   129,   209,   190,
     209,   209,   134,   209,   136,   137,  1050,    42,   209,   177,
     142,   143,   144,   210,   210,   165,   148,   209,   165,   209,
     165,    32,   165,   209,  1263,   165,   165,   209,   165,   358,
     209,   209,   209,   209,   209,   209,   191,   210,   208,   368,
    1084,   210,  1233,   191,  1235,   210,   177,   210,    52,   378,
     176,   208,   177,   185,   210,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,  1235,   641,    80,     1,   210,
      44,   400,   116,   117,   206,   207,    20,    21,   210,   408,
     124,   210,   126,   127,   128,   129,   134,    83,  1340,  1133,
     134,  1168,   104,  1168,   224,   978,  1168,  1168,     1,  1339,
    1339,  1168,  1319,   574,  1277,  1344,  1350,    55,   535,   438,
     439,  1280,  1275,  1179,  1351,   794,   421,   421,  1043,    32,
    1373,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   457,   458,
     459,   460,   461,   462,    -1,    -1,    -1,    -1,    -1,  1330,
      -1,    -1,    -1,    -1,    -1,    -1,    59,    60,   192,   193,
     194,   195,   196,    -1,    -1,  1346,    -1,    -1,    -1,    -1,
    1330,    -1,   206,   207,    -1,    -1,    -1,    -1,    -1,    -1,
     114,   115,   116,   117,   118,    -1,    -1,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,   515,    -1,    -1,  1233,
     134,  1235,   136,   137,    -1,    -1,    -1,    -1,   142,   143,
     144,    -1,   531,    -1,   148,    -1,    -1,    -1,    -1,   122,
     539,    -1,  1451,   126,   543,    -1,    -1,   546,   547,    -1,
      -1,    -1,    -1,   552,    -1,   554,    32,    -1,    32,   558,
      -1,    -1,    -1,    -1,   563,    -1,    -1,    -1,    -1,    -1,
      -1,   185,    -1,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,    59,    60,    59,    60,    -1,    -1,    -1,
      -1,    -1,   206,   207,   177,    -1,    -1,   211,   212,    -1,
      -1,    -1,   185,   602,    -1,    -1,   189,    -1,   191,   192,
      -1,    -1,    -1,    -1,    -1,    -1,  1330,    -1,    -1,   618,
     619,    -1,    -1,   622,   207,    -1,    -1,    -1,    -1,    -1,
     213,    -1,  1346,    -1,   633,   634,   635,   636,   637,   638,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,   122,   125,
     126,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,   135,
    1374,   660,   661,    -1,    -1,   664,   665,   666,   667,    -1,
     669,    -1,   671,   672,   673,   674,   675,   676,   677,   678,
     679,   680,   681,   682,   683,   684,   685,   686,   687,   688,
     689,   690,    -1,   692,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,  1418,    -1,    -1,    -1,   184,   185,
      -1,   185,    -1,   189,    -1,   189,   192,   191,   192,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   207,    -1,   207,   733,    -1,    -1,   213,    -1,   213,
     739,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   748,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   762,    -1,    -1,    -1,    -1,    59,    60,
      -1,    -1,    -1,    -1,    -1,   774,   775,   776,   777,   778,
     779,   780,   781,   782,   783,   784,   785,   786,   787,   788,
     789,   790,   791,    -1,   793,    -1,    -1,    -1,    18,    -1,
      -1,    -1,    -1,   802,    24,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,   814,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,
      -1,   122,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,   841,    63,    -1,   844,   845,    -1,    -1,   848,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,    -1,    -1,    -1,   189,    -1,
     191,   192,    -1,    -1,    -1,    -1,    -1,   906,    -1,    -1,
      -1,   910,    -1,    -1,    -1,    -1,   207,    -1,    -1,    -1,
      -1,   141,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   932,   154,    -1,    -1,    -1,    -1,   938,
      -1,    -1,    -1,    -1,    -1,   165,   945,    -1,    -1,    -1,
      -1,   950,   951,   952,   953,   954,   955,   956,   957,   958,
     959,   960,   961,   962,   963,   964,   965,   966,    -1,    -1,
      -1,   970,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,
      -1,    -1,     4,     5,     6,    -1,     8,     9,    10,    -1,
      12,   211,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    24,    25,    26,    27,    28,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    38,    -1,    -1,    41,
      -1,    43,    44,    -1,    -1,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    -1,    57,    58,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,
      72,    -1,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    94,    -1,    -1,    97,    98,    99,   100,   101,
     102,   103,   104,  1082,  1083,    -1,    32,  1086,  1087,  1088,
      -1,  1090,    -1,    -1,   116,   117,    -1,    -1,  1097,    -1,
    1099,    -1,  1101,    -1,    -1,    -1,    -1,    -1,  1107,   131,
     132,   133,  1111,    59,    60,    -1,    -1,    -1,    -1,   141,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,
     152,   153,    -1,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,    59,    60,    -1,
      -1,    -1,    -1,   175,   176,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1162,    32,    -1,    -1,  1166,    -1,  1168,
     192,   193,   194,    -1,   196,    32,   122,   199,   200,    -1,
     126,    -1,    -1,    -1,    -1,   207,    -1,   209,    -1,   211,
     212,    59,    60,    -1,    32,    -1,    -1,    -1,  1197,    -1,
      -1,    -1,    59,    60,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    59,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,
      -1,    -1,  1241,   189,  1243,   191,   192,    -1,    -1,  1248,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,   126,  1258,
      -1,   207,    -1,    -1,    -1,   122,  1265,   213,    -1,   126,
      -1,    -1,    -1,   185,    -1,    -1,    -1,   189,    -1,   191,
     192,    -1,    -1,    -1,   122,    -1,    -1,  1286,   126,    -1,
      -1,    -1,  1291,  1292,  1293,   207,    -1,    -1,    -1,  1298,
      -1,   213,  1301,    -1,  1303,  1304,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1312,  1313,  1314,    -1,   185,    -1,    -1,
      -1,   189,  1321,   191,   192,    -1,    -1,    -1,   185,    -1,
      -1,    -1,   189,    -1,   191,   192,    -1,    -1,    -1,   207,
      -1,    -1,    -1,  1342,    -1,   213,    -1,   185,    -1,    -1,
     207,   189,    -1,   191,   192,    -1,   213,    -1,  1357,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   207,
      -1,    -1,    -1,    -1,    -1,   213,    -1,  1376,  1377,    -1,
      -1,    -1,    -1,    -1,    -1,  1384,    -1,     1,    -1,    -1,
       4,     5,     6,    -1,     8,     9,    10,    -1,    12,  1398,
      14,    15,    16,    17,    18,  1404,    -1,    -1,    -1,    -1,
      24,    25,    26,    27,    28,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    38,    -1,    -1,    41,  1427,    43,
      44,    -1,    -1,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,  1441,    57,    58,    -1,  1445,  1446,    -1,    -1,
      64,    -1,    -1,    67,    68,    -1,    70,    71,    72,    -1,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    -1,    97,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,
      32,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,
      -1,   175,   176,   177,    -1,    -1,    -1,    59,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   192,   193,
     194,    -1,   196,    -1,    -1,   199,   200,    -1,    -1,     4,
       5,    -1,    -1,   207,    -1,   209,    -1,   211,   212,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,
     122,    -1,    47,    -1,   126,    50,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    -1,    70,    71,    72,    -1,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    94,
      -1,    -1,    97,    98,    99,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,   185,    -1,    -1,    -1,   189,    -1,   191,
     192,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,   207,   131,   132,   133,    -1,
      -1,   213,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,    32,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,    -1,    -1,    -1,    -1,    59,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   192,   193,   194,
      -1,   196,    -1,    -1,   199,   200,    -1,    -1,     4,     5,
      -1,    -1,   207,    -1,   209,    -1,   211,   212,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,   122,
      -1,    47,    -1,   126,    50,    -1,    -1,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    -1,    70,    71,    72,    -1,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    -1,    92,    93,    94,    -1,
      -1,    97,    98,    99,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,   185,    -1,    -1,    -1,   189,    -1,   191,   192,
     116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,   207,   131,   132,   133,    -1,    -1,
     213,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,   150,   151,   152,   153,    32,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,   175,
      -1,    -1,    -1,    -1,    -1,    59,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   192,   193,   194,    -1,
     196,    -1,    -1,   199,   200,    -1,    -1,     4,     5,    -1,
      -1,   207,    -1,   209,    -1,   211,   212,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    25,    26,
      27,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    44,   122,    -1,
      47,    -1,   126,    50,    51,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    -1,    70,    71,    72,    -1,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    -1,    92,    93,    94,    -1,    -1,
      97,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,   185,    -1,    -1,    -1,   189,    -1,   191,   192,   116,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   207,    -1,    -1,    -1,    -1,    -1,   213,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,   150,   151,   152,   153,    32,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,
      -1,    -1,    -1,    -1,    59,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   192,   193,   194,    -1,   196,
      -1,    -1,   199,   200,    -1,    -1,     4,     5,    -1,    -1,
     207,    -1,   209,    -1,   211,   212,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,   122,    -1,    47,
      -1,   126,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    -1,    70,    71,    72,    -1,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    -1,    -1,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
     185,    -1,    -1,    -1,   189,    -1,   191,   192,   116,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   207,   131,   132,   133,    -1,    -1,   213,    -1,
      -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,   150,   151,   152,   153,    32,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,
      -1,    -1,    -1,    59,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   192,   193,   194,    -1,   196,    -1,
      -1,   199,   200,    -1,    -1,     4,     5,    -1,    -1,   207,
      -1,   209,    -1,   211,   212,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    44,   122,    -1,    47,    -1,
     126,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      -1,    70,    71,    72,    -1,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    -1,    -1,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,   185,
      -1,    -1,    -1,   189,    -1,   191,   192,   116,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   207,   131,   132,   133,    -1,    -1,   213,    -1,    -1,
      -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,   150,   151,   152,   153,    32,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,
      -1,    -1,    59,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   192,   193,   194,    -1,   196,    -1,    -1,
     199,   200,    -1,    -1,     4,     5,    -1,    -1,   207,    -1,
     209,    -1,   211,   212,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    -1,    44,   122,    -1,    47,    -1,   126,
      50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    -1,
      70,    71,    72,    -1,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    94,    -1,    -1,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,   185,    -1,
      -1,    -1,   189,    -1,   191,   192,   116,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     207,    -1,    -1,    -1,    -1,    -1,   213,    -1,    -1,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
     150,   151,   152,   153,    -1,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   192,   193,   194,    -1,   196,    -1,    -1,   199,
     200,    -1,    -1,    -1,    -1,     4,     5,   207,    -1,   209,
     210,   211,   212,    12,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      -1,    70,    71,    72,    -1,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    -1,    -1,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,   150,   151,   152,   153,    -1,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   192,   193,   194,    -1,   196,    -1,    -1,
     199,   200,    -1,    -1,     4,     5,    -1,    -1,   207,    -1,
     209,    -1,   211,   212,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    -1,
      70,    71,    72,    -1,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    94,    -1,    -1,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
     150,   151,   152,   153,    -1,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   192,   193,   194,    -1,   196,    -1,    -1,   199,
     200,    -1,    -1,    -1,    -1,     4,     5,   207,   208,   209,
      -1,   211,   212,    12,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      -1,    70,    71,    72,    -1,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    -1,    -1,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,   150,   151,   152,   153,    -1,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   192,   193,   194,    -1,   196,    -1,    -1,
     199,   200,    -1,    -1,    -1,    -1,     4,     5,   207,    -1,
     209,    -1,   211,   212,    12,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    -1,    70,    71,    72,    -1,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    -1,    -1,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,   150,   151,   152,   153,    -1,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   192,   193,   194,    -1,   196,    -1,
      -1,   199,   200,    -1,    -1,     4,     5,    -1,    -1,   207,
      -1,   209,    -1,   211,   212,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      -1,    70,    71,    72,    -1,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    -1,    -1,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,   150,   151,   152,   153,    -1,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   192,   193,   194,    -1,   196,    -1,    -1,
     199,   200,    -1,    -1,     4,     5,    -1,    -1,   207,    -1,
     209,    -1,   211,   212,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    -1,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    -1,
      70,    71,    72,    -1,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    94,    -1,    -1,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
     150,   151,   152,   153,    -1,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   192,   193,   194,    -1,   196,    -1,    -1,   199,
     200,    -1,    -1,     4,     5,    -1,    -1,   207,    -1,   209,
      -1,   211,   212,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    -1,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      -1,    92,    93,    94,    -1,    -1,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,
     151,   152,   153,    -1,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,
      -1,   192,   193,   194,    -1,   196,    -1,    -1,   199,   200,
      -1,    -1,     4,     5,    -1,    -1,   207,    -1,   209,    -1,
     211,   212,    14,    15,    16,    17,    18,    -1,    -1,    21,
      -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    -1,    70,    71,
      72,    -1,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    94,    -1,    -1,    97,    98,    99,   100,   101,
     102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,
     152,   153,    -1,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     192,   193,   194,    -1,   196,    -1,    -1,   199,   200,    -1,
      -1,     4,     5,    -1,    -1,   207,    -1,   209,    -1,   211,
     212,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    -1,    70,    71,    72,
      -1,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    92,
      93,    94,    -1,    -1,    97,    98,    99,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,   152,
     153,    -1,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   192,
     193,   194,    -1,   196,    -1,    -1,   199,   200,    -1,    -1,
       4,     5,    -1,    -1,   207,    -1,   209,   210,   211,   212,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    -1,    70,    71,    72,    -1,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    -1,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,
      -1,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,
      -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   190,    -1,   192,   193,
     194,    -1,   196,    -1,    -1,   199,   200,    -1,    -1,     4,
       5,    -1,    -1,   207,    -1,   209,    -1,   211,   212,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    -1,    70,    71,    72,    -1,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    94,
      -1,    -1,    97,    98,    99,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,    -1,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   192,   193,   194,
      -1,   196,    -1,    -1,   199,   200,    -1,    -1,     4,     5,
      -1,    -1,   207,    -1,   209,   210,   211,   212,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    -1,    70,    71,    72,    -1,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    -1,    92,    93,    94,    -1,
      -1,    97,    98,    99,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,   150,   151,   152,   153,    -1,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,   175,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   192,   193,   194,    -1,
     196,    -1,    -1,   199,   200,    -1,    -1,     4,     5,    -1,
      -1,   207,    -1,   209,   210,   211,   212,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    -1,    70,    71,    72,    -1,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    -1,    92,    93,    94,    -1,    -1,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,   150,   151,   152,   153,    -1,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   192,   193,   194,    -1,   196,
      -1,    -1,   199,   200,    -1,    -1,     4,     5,    -1,    -1,
     207,    -1,   209,   210,   211,   212,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    -1,    70,    71,    72,    -1,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    -1,    -1,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,   150,   151,   152,   153,    -1,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   192,   193,   194,    -1,   196,    -1,
      -1,   199,   200,    -1,    -1,     4,     5,    -1,    -1,   207,
      -1,   209,    -1,   211,   212,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      -1,    70,    71,    72,    -1,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    -1,    -1,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,   150,   151,   152,   153,    -1,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   192,   193,   194,    -1,   196,    -1,    -1,
     199,   200,    -1,    -1,     4,     5,    -1,    -1,   207,    -1,
     209,    -1,   211,   212,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    -1,
      70,    71,    72,    -1,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    94,    -1,    -1,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
     150,   151,   152,   153,    -1,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   192,   193,   194,    -1,   196,    -1,    -1,   199,
     200,    -1,    -1,     4,     5,    -1,    -1,   207,    -1,   209,
     210,   211,   212,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    -1,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      -1,    92,    93,    94,    -1,    -1,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,
     151,   152,   153,    -1,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   192,   193,   194,    -1,   196,    -1,    -1,   199,   200,
      -1,    -1,     4,     5,    -1,    -1,   207,    -1,   209,    -1,
     211,   212,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    -1,    70,    71,
      72,    -1,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    94,    -1,    -1,    97,    98,    99,   100,   101,
     102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,
     152,   153,    -1,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     192,   193,   194,    -1,   196,    -1,    -1,   199,   200,    -1,
      -1,     4,     5,    -1,    -1,   207,    -1,   209,    -1,   211,
     212,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    -1,    70,    71,    72,
      -1,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    92,
      93,    94,    -1,    -1,    97,    98,    99,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,   117,     9,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,   152,
     153,    -1,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,    -1,    20,    21,    -1,
      -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   192,
     193,   194,    -1,   196,    -1,    -1,   199,   200,    -1,    -1,
      -1,    -1,    -1,    -1,   207,    -1,   209,    -1,   211,   212,
      -1,    -1,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,   135,   136,   137,   138,   139,    -1,    -1,   142,   143,
     144,   145,   146,   147,   148,    -1,    -1,    -1,    -1,    -1,
      -1,   114,   115,   116,   117,   118,    -1,    -1,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,
     184,   185,    -1,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,   206,   207,    -1,    30,    -1,   211,   212,    -1,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,   206,   207,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,   141,    -1,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
     165,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   211,    -1,   213,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,
      20,    21,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,   154,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,   177,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   211,    -1,    -1,    -1,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,    20,    21,    -1,   134,   135,   136,   137,   138,   139,
     141,    -1,   142,   143,   144,   145,   146,   147,   148,    -1,
      -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   184,   185,    -1,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   206,   207,    -1,    -1,
     211,   211,   212,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,    20,    21,    -1,    -1,   134,   135,   136,   137,   138,
     139,    -1,    -1,   142,   143,   144,   145,   146,   147,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,   184,   185,    -1,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,   207,    -1,
      -1,    -1,   211,   212,    -1,    -1,    -1,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,
     139,    -1,    -1,   142,   143,   144,   145,   146,   147,   148,
      -1,    -1,    -1,   114,   115,   116,   117,   118,    20,    21,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    -1,
      -1,    -1,    -1,   134,    -1,   136,   137,    -1,   177,    -1,
      -1,   142,   143,   144,    -1,   184,   185,   148,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,   207,    -1,
      -1,    -1,   211,   212,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,    -1,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   206,   207,    -1,    -1,   210,
      -1,    -1,   114,   115,   116,   117,   118,    20,    21,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
     142,   143,   144,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   185,    -1,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   206,   207,    -1,    -1,   210,    -1,
      -1,   114,   115,   116,   117,   118,    -1,    -1,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,
     143,   144,    -1,    -1,    -1,   148,   114,   115,   116,   117,
     118,    20,    21,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,
      -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,
     148,    -1,   185,    -1,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   206,   207,    -1,    -1,   210,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,   207,
      -1,    -1,   210,    -1,    -1,   114,   115,   116,   117,   118,
      20,    21,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,   207,    -1,
      -1,   210,    -1,    -1,   114,   115,   116,   117,   118,    -1,
      -1,   121,   122,   123,   124,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,
      -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,   114,
     115,   116,   117,   118,    20,    21,   121,   122,   123,   124,
      -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,
      -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,
      -1,    -1,    -1,   148,    -1,   185,    -1,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   206,   207,    -1,    -1,
     210,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,    -1,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   206,   207,    -1,    -1,   210,    -1,    -1,   114,   115,
     116,   117,   118,    20,    21,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,
      -1,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     206,   207,    -1,    -1,   210,    -1,    -1,   114,   115,   116,
     117,   118,    -1,    -1,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,
      -1,   148,   114,   115,   116,   117,   118,    20,    21,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
     142,   143,   144,    -1,    -1,    -1,   148,    -1,   185,    -1,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,
     207,    -1,    -1,   210,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   185,    -1,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   206,   207,    -1,    -1,   210,    -1,
      -1,   114,   115,   116,   117,   118,    20,    21,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,
     143,   144,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,    -1,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   206,   207,    -1,    -1,   210,    -1,    -1,
     114,   115,   116,   117,   118,    -1,    -1,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,
     144,    -1,    -1,    -1,   148,   114,   115,   116,   117,   118,
      20,    21,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,
      -1,   185,    -1,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   206,   207,    -1,    -1,   210,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,   207,    -1,
      -1,   210,    -1,    -1,   114,   115,   116,   117,   118,    20,
      21,   121,   122,   123,   124,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,
      -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   185,    -1,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   206,   207,    -1,    -1,
     210,    -1,    -1,   114,   115,   116,   117,   118,    -1,    -1,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    -1,
      -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
      -1,   142,   143,   144,    -1,    -1,    -1,   148,   114,   115,
     116,   117,   118,    20,    21,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,    -1,   185,    -1,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   206,   207,    -1,    -1,   210,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,
      -1,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     206,   207,    -1,    -1,   210,    -1,    -1,   114,   115,   116,
     117,   118,    20,    21,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,
     207,    -1,    -1,   210,    -1,    -1,   114,   115,   116,   117,
     118,    -1,    -1,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,
      -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,
     148,   114,   115,   116,   117,   118,    20,    21,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,
     143,   144,    -1,    -1,    -1,   148,    -1,   185,    -1,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,   207,
      -1,    -1,   210,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,    -1,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   206,   207,    -1,    -1,   210,    -1,    -1,
     114,   115,   116,   117,   118,    20,    21,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,
     144,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   185,    -1,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   206,   207,    -1,    -1,   210,    -1,    -1,   114,
     115,   116,   117,   118,    -1,    -1,   121,   122,   123,   124,
      -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,
      -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,
      -1,    -1,    -1,   148,   114,   115,   116,   117,   118,    20,
      21,   121,   122,   123,   124,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,
      -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,    -1,
     185,    -1,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   206,   207,    -1,    -1,   210,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   185,    -1,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   206,   207,    -1,    -1,
     210,    -1,    -1,   114,   115,   116,   117,   118,    20,    21,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    -1,
      -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
      -1,   142,   143,   144,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,    -1,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   206,   207,    -1,    -1,   210,
      -1,    -1,   114,   115,   116,   117,   118,    -1,    -1,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
     142,   143,   144,    -1,    -1,    -1,   148,   114,   115,   116,
     117,   118,    20,    21,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,
      -1,   148,    -1,   185,    -1,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   206,   207,    -1,    -1,   210,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,
     207,    -1,    -1,   210,    -1,    -1,   114,   115,   116,   117,
     118,    20,    21,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,
      -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,   207,
      -1,    -1,   210,    -1,    -1,   114,   115,   116,   117,   118,
      -1,    -1,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,
     114,   115,   116,   117,   118,    20,    21,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,
     144,    -1,    -1,    -1,   148,    -1,   185,    -1,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,   207,    -1,
      -1,   210,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   185,    -1,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,    -1,    -1,    -1,    -1,   114,
     115,   116,   117,   118,    20,    21,   121,   122,   123,   124,
      -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,
      -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,    -1,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   206,   207,   208,    -1,    -1,    -1,    -1,   114,   115,
     116,   117,   118,    -1,    -1,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,   114,   115,   116,   117,   118,    20,    21,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    -1,
      -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
      -1,   142,   143,   144,    -1,    -1,    -1,   148,    -1,   185,
      -1,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,    -1,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    -1,    -1,   206,   207,   208,    -1,    -1,
      -1,    -1,   114,   115,   116,   117,   118,    20,    21,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
     142,   143,   144,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    70,    71,    72,    -1,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    -1,    -1,    97,    98,
      99,   100,    -1,   185,    -1,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,    -1,    -1,    -1,
      -1,   114,   115,   116,   117,   118,    -1,    -1,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    20,    21,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,   156,    -1,   142,
     143,   144,    -1,    -1,    37,   148,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,    -1,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,    -1,    -1,   126,   127,   128,    -1,    -1,    -1,    -1,
      -1,    -1,   135,   136,   137,   138,   139,    -1,    -1,   142,
     143,   144,   145,   146,   147,   148,   114,   115,   116,   117,
     118,    20,    21,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,
      -1,    -1,   140,    -1,   142,   143,   144,    -1,    -1,    -1,
     148,    -1,   185,    -1,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,    -1,    -1,   199,   200,    -1,    -1,
      -1,    -1,    -1,   206,   207,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,   207,
      -1,    -1,    -1,    -1,    -1,   114,   115,   116,   117,   118,
      20,    21,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,   207,    -1,
      -1,    -1,    -1,    -1,   114,   115,   116,   117,   118,    -1,
      -1,   121,   122,   123,   124,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,
      -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,   114,
     115,   116,   117,   118,    20,    21,   121,   122,   123,   124,
      -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,
      -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,
      -1,    -1,    -1,   148,    -1,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   206,   207,   173,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,    -1,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   206,   207,    -1,    -1,    -1,    -1,    -1,   114,   115,
     116,   117,   118,    20,    21,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,
      -1,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     206,   207,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,
     117,   118,    -1,    -1,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,
      -1,   148,    -1,   114,   115,   116,   117,   118,    20,    21,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    -1,
      -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
     177,   142,   143,   144,    -1,    -1,    -1,   148,   185,    -1,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,
     207,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,    -1,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   206,   207,    -1,    -1,    -1,
      -1,    -1,   114,   115,   116,   117,   118,    20,    21,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
     142,   143,   144,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   185,    -1,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   206,   207,    -1,    -1,    -1,    -1,
      -1,   114,   115,   116,   117,   118,    -1,    -1,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,
     143,   144,    -1,    -1,    -1,   148,   114,   115,   116,   117,
     118,    20,    21,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,
      -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,
      -1,    -1,   185,    -1,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   206,   207,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,   207,
      -1,    -1,    -1,    -1,    -1,   114,   115,   116,   117,   118,
      -1,    -1,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    -1,   142,    -1,   144,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,   114,   115,
     116,   117,   118,    -1,    -1,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    -1,    20,    21,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,    -1,    -1,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,   206,   207,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,    -1,    -1,    -1,    -1,   114,   115,   116,   117,   118,
     206,   207,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,   114,
     115,   116,   117,   118,    -1,    -1,   121,   122,   123,   124,
      -1,   126,   127,   128,   129,    -1,    20,    21,    -1,   134,
      -1,   136,   137,   114,   115,   116,   117,   118,    -1,    -1,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    -1,
      -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,   188,
     189,   190,   191,   192,   193,   194,   195,   196,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,   207,    -1,
      -1,    -1,    -1,    -1,   189,   190,   191,   192,   193,   194,
     195,   196,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   206,   207,    -1,    -1,    -1,    -1,    -1,    -1,   190,
     191,   192,   193,   194,   195,   196,    -1,    -1,    -1,    -1,
     114,   115,   116,   117,   118,   206,   207,   121,    -1,    -1,
     124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,   136,   137,    70,    71,    72,    -1,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    -1,    92,    93,    94,    -1,
      -1,    97,    98,    99,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   190,   191,   192,   193,
     194,   195,   196,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   206,   207,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,    34,    -1,    -1,    -1,    -1,    70,    71,    72,   165,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    -1,    97,    98,    99,   100,    70,    -1,    72,
      -1,    74,    75,    76,    77,    78,    -1,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    92,
      93,    94,    -1,    -1,    97,    98,    99,   100,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   165
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   215,     0,     6,    29,    31,    33,    39,    49,    55,
      79,   101,   102,   177,   196,   207,   216,   219,   225,   227,
     228,   233,   259,   263,   296,   373,   381,   385,   394,   438,
     442,   446,    18,    19,   165,   251,   252,   253,   158,   234,
     235,   165,   166,   167,   196,   229,   230,   231,    56,    62,
     175,   377,   378,   165,   211,   218,   377,   377,   377,   141,
     165,   282,    33,    62,   134,   200,   209,   255,   256,   257,
     258,   282,   177,     4,     5,     7,    35,   391,    61,   371,
     184,   183,   186,   183,   230,    21,    56,   195,   206,   232,
     378,   380,   165,   177,   371,   165,   165,   165,   165,   141,
     226,   257,   257,   257,   209,   142,   143,   144,   183,   208,
      56,    62,   264,   266,    56,    62,   382,    56,    62,   392,
      56,    62,   372,    14,    15,   158,   163,   165,   168,   209,
     221,   252,   158,   235,   165,   229,   229,   177,   176,   378,
     379,    56,    62,   217,   447,   439,   443,   165,   169,   224,
     210,   253,   257,   257,   257,   257,   267,   165,   383,   395,
     175,   374,   169,   170,   220,    14,    15,   158,   163,   165,
     221,   249,   250,   232,   177,   184,   175,   175,   175,   169,
     210,    34,    70,    72,    74,    75,    76,    77,    78,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      92,    93,    94,    97,    98,    99,   100,   116,   117,   165,
     262,   265,   186,   384,   105,   389,   390,   369,   212,   254,
     345,   169,   170,   171,   183,   210,    18,    24,    30,    40,
      48,    63,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   154,   211,   282,   398,   400,
     401,   404,   409,   437,   448,   440,   444,    20,    21,    37,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   126,   127,
     128,   135,   136,   137,   138,   139,   142,   143,   144,   145,
     146,   147,   148,   185,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   199,   200,   206,   207,    34,    34,
     209,   260,   175,   268,    74,    78,    92,    93,    97,    98,
      99,   100,   399,   175,   165,   396,   176,   370,   252,   212,
     150,   165,   367,   368,   249,   190,   190,   190,   209,   190,
     190,   209,   190,   190,   190,   190,   190,   190,   209,   282,
      32,    59,    60,   122,   126,   185,   189,   192,   207,   213,
     408,   187,   165,   403,   359,   362,   165,   165,   165,   208,
      21,   165,   208,   153,   210,   345,   355,   356,   186,   261,
     271,   386,   186,   388,   175,   393,   345,   252,   209,    42,
     183,   186,   189,   366,   410,   415,   417,     4,     5,    14,
      15,    16,    17,    18,    24,    26,    30,    38,    44,    47,
      50,    54,    67,    68,    79,   101,   102,   103,   116,   117,
     149,   150,   151,   152,   153,   155,   157,   158,   159,   160,
     161,   162,   163,   164,   166,   167,   168,   175,   192,   193,
     194,   199,   200,   207,   209,   211,   212,   223,   225,   276,
     282,   287,   301,   308,   311,   314,   319,   321,   325,   326,
     328,   333,   336,   337,   344,   398,   450,   455,   465,   468,
     474,   477,   419,   413,   165,   405,   421,   423,   425,   427,
     429,   431,   433,   435,   337,   190,   209,    32,   189,    32,
     189,   207,   213,   208,   337,   207,   213,   409,   183,   476,
     165,   177,   357,   437,   441,   165,   177,   360,   445,   165,
     135,   209,     6,    49,   295,   177,   210,   437,     1,     8,
       9,    10,    12,    25,    27,    28,    37,    41,    43,    51,
      53,    57,    58,    64,   104,   131,   132,   133,   166,   176,
     177,   236,   237,   240,   242,   243,   244,   245,   246,   247,
     248,   269,   270,   272,   276,   281,   283,   288,   289,   290,
     291,   292,   293,   294,   296,   300,   322,   324,   337,   165,
     375,   376,   282,   351,   368,   337,   165,   165,   409,   125,
     135,   184,   365,   437,   437,   407,   437,   190,   190,   190,
     284,   400,   450,   209,   190,   190,   209,   190,   209,   209,
     190,   190,   209,   190,   190,   209,   190,   190,   209,   190,
     190,   337,   337,   209,   209,   209,   209,   209,   209,   190,
     190,   222,    12,   337,   464,   473,   337,   337,   337,   337,
     337,    12,   312,   337,   312,   212,   209,   259,    20,    21,
     114,   115,   116,   117,   118,   121,   122,   123,   124,   126,
     127,   128,   129,   134,   136,   137,   142,   143,   144,   148,
     185,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   206,   207,   210,   209,   437,   437,   210,   165,   402,
     437,   260,   437,   260,   437,   260,   357,   358,   360,   361,
     210,   412,   273,   312,   208,   208,   337,   165,   449,   186,
     177,   176,   186,   177,   176,   337,   150,   165,   364,   397,
     355,   337,   397,   125,   281,   312,   322,   337,   268,    60,
     337,   337,   165,   177,   158,    57,   337,   268,   125,   281,
     337,   212,   268,   314,   318,   318,   318,   337,    36,   177,
     177,   337,     9,   314,   320,   177,   177,   177,   177,   177,
      65,   297,   130,   177,   106,   107,   108,   109,   110,   111,
     112,   113,   119,   120,   125,   135,   138,   139,   145,   146,
     147,   184,   320,   184,   183,   476,   176,   259,   352,   210,
      42,   177,   365,   281,   337,   191,   191,   191,   177,   456,
     458,   285,   209,   209,   282,   309,   469,   312,   409,   473,
     337,   302,   304,   337,   306,   471,   312,   462,   466,   312,
     460,   409,   337,   337,   337,   337,   337,   337,   277,   279,
     171,   172,   220,   397,   140,   183,   476,   397,   183,   476,
     476,   150,   155,   190,   282,   327,   156,   165,   207,   210,
     312,   451,   452,   207,   317,    18,   156,   165,   398,    18,
     156,   165,   398,   337,   337,   337,   337,   337,   337,   165,
     337,   156,   165,   337,   337,   337,   398,   337,   337,   337,
     337,   337,   337,    21,   337,   337,   337,   337,   337,   337,
     337,   337,   337,   337,   337,   127,   128,   156,   165,   206,
     207,   334,   337,   210,   312,   191,   191,   177,   191,   191,
     261,   191,   261,   191,   261,   177,   191,   177,   191,   275,
     437,   210,   183,   208,   176,   437,   437,   210,   209,    42,
     125,   183,   184,   186,   189,   363,   268,    19,   281,   312,
     106,   107,   108,   109,   110,   111,   112,   113,   119,   120,
     125,   138,   139,   145,   146,   147,   184,    13,   337,   268,
     184,   186,   158,   281,   337,   212,   259,   268,   175,   268,
     337,   259,   209,   299,   368,   318,   131,   132,   133,   269,
     323,   337,   323,   337,   323,   337,   323,   337,   323,   337,
     323,   337,   323,   337,   323,   337,   323,   337,   323,   337,
     323,   337,   337,   323,   337,   323,   337,   323,   337,   323,
     337,   323,   337,   323,   337,   337,   375,   387,     7,   345,
     350,   165,   281,   337,   177,   411,   416,   418,   437,   409,
     409,   437,    69,   454,   273,   190,   337,   437,   409,   476,
     177,   191,   476,   210,   409,   409,   210,   409,   409,   476,
     409,   409,   476,   409,   191,   210,   210,   210,   210,   210,
     210,   275,   275,   337,    19,   337,   464,   176,    19,   337,
     208,   210,   209,   209,   329,   331,   209,   135,   363,   451,
     183,   210,   183,   210,   207,   260,   190,   209,   190,   209,
     209,   209,   208,    18,   156,   165,   398,   186,   156,   165,
     337,   209,   209,   156,   165,   337,     1,   208,   210,   420,
     414,   165,   406,   422,   191,   426,   191,   430,   191,   357,
     434,   360,   436,   177,   191,   337,   165,   165,   437,   281,
     337,   312,   337,   337,   337,   337,   337,   337,   337,   337,
     337,   337,   337,   337,   337,   337,   337,   337,   337,   268,
     337,   317,   271,    11,    22,    23,   238,   239,    11,   241,
     317,   165,   298,   318,   318,   318,   176,    56,    62,   348,
      66,   349,   177,   177,   191,   191,   191,   165,   210,   452,
     210,   165,   210,   191,   191,   210,   409,   209,   210,   191,
     191,   191,   191,   210,   191,   191,   210,   191,   317,   209,
     191,   191,   173,   312,   312,   337,   337,   409,   260,   337,
     337,   337,   208,   207,   156,   165,   125,   135,   184,   189,
     315,   316,   261,   338,   337,   340,   337,   210,   312,   337,
     190,   209,   337,   209,   208,   337,   210,   312,   209,   208,
     335,   424,   428,   432,   437,   209,   210,    42,   363,   268,
     260,   176,   268,   337,    25,   104,   242,   288,   289,   290,
     292,   337,   177,   260,   183,   210,    46,   349,    45,   105,
     346,   457,   459,   286,   210,   177,   191,   310,   470,   191,
     473,   303,   305,   307,   472,   463,   467,   461,   209,   210,
     312,   278,   280,   177,   177,   210,   210,   191,   261,   210,
     210,   451,   209,   135,   363,   165,   165,   165,   165,   183,
     208,   140,   268,   313,   409,   210,   437,   210,   210,   210,
     342,   337,   337,   210,   210,   337,   273,   165,   337,   261,
     268,   125,   125,   337,   261,   165,   186,   366,    32,   347,
     346,   348,   209,   209,   165,   337,   209,   209,   476,   337,
     337,   337,   209,   454,   209,   209,   210,   337,   210,   337,
     209,   274,   464,   337,   330,   191,   135,   363,   208,   337,
     337,   337,   315,   208,   125,   337,   191,   191,   437,   210,
     210,   210,   210,   313,   239,   337,   268,   409,   365,   353,
     347,   364,   454,   454,   191,   210,   312,   473,   210,   312,
     454,   454,   210,   273,   177,   475,   475,   327,   332,   337,
     337,   210,   210,   337,   339,   341,   191,   365,   337,   265,
     354,   451,   453,   453,   337,   476,   476,   476,   453,   453,
     210,    52,   176,   208,   327,   135,   363,   343,   337,   177,
     177,   265,   177,   210,   210,   210,   210,   210,   210,   210,
     210,   337,   337,   337,   177,   268,   451
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   214,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   216,   217,   217,
     217,   218,   218,   219,   220,   220,   220,   220,   221,   222,
     222,   222,   223,   224,   224,   226,   225,   227,   228,   229,
     229,   229,   230,   230,   230,   230,   231,   231,   232,   232,
     233,   234,   234,   235,   235,   236,   237,   237,   238,   238,
     239,   239,   239,   240,   240,   241,   241,   242,   242,   242,
     242,   242,   243,   243,   244,   245,   246,   247,   248,   249,
     249,   249,   249,   249,   249,   250,   250,   251,   251,   251,
     252,   252,   252,   252,   252,   252,   252,   252,   253,   253,
     254,   254,   255,   255,   255,   256,   256,   257,   257,   257,
     257,   257,   257,   257,   258,   258,   259,   259,   260,   260,
     260,   261,   261,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   263,   264,
     264,   264,   265,   267,   266,   268,   268,   269,   269,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     271,   271,   271,   272,   273,   273,   274,   274,   275,   275,
     277,   278,   276,   279,   280,   276,   281,   281,   281,   281,
     281,   282,   282,   282,   283,   283,   285,   286,   284,   287,
     287,   287,   287,   287,   288,   289,   290,   290,   290,   291,
     291,   291,   292,   292,   293,   293,   293,   294,   295,   295,
     295,   296,   296,   297,   297,   298,   298,   299,   299,   300,
     300,   302,   303,   301,   304,   305,   301,   306,   307,   301,
     309,   310,   308,   311,   311,   311,   312,   312,   313,   313,
     313,   314,   314,   314,   315,   315,   315,   315,   316,   316,
     317,   317,   318,   318,   319,   320,   321,   321,   321,   321,
     321,   321,   321,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   323,   323,   323,   323,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   325,   325,   326,   326,   327,   327,   328,
     329,   330,   328,   331,   332,   328,   333,   333,   333,   333,
     334,   335,   333,   336,   336,   336,   336,   336,   337,   337,
     337,   337,   337,   337,   337,   337,   337,   337,   337,   337,
     337,   337,   337,   337,   337,   337,   337,   337,   337,   337,
     337,   337,   337,   337,   337,   337,   337,   337,   337,   337,
     337,   337,   337,   337,   337,   337,   337,   337,   337,   337,
     337,   337,   337,   337,   337,   337,   337,   337,   337,   337,
     337,   337,   337,   337,   338,   339,   337,   337,   337,   337,
     340,   341,   337,   337,   337,   342,   343,   337,   337,   337,
     337,   337,   337,   337,   337,   337,   337,   337,   337,   337,
     344,   344,   344,   344,   344,   344,   344,   344,   344,   344,
     344,   344,   344,   344,   344,   344,   345,   345,   346,   346,
     346,   347,   347,   348,   348,   348,   349,   349,   350,   351,
     352,   351,   353,   351,   354,   351,   355,   355,   356,   356,
     357,   357,   358,   358,   359,   359,   359,   360,   361,   361,
     362,   362,   362,   363,   363,   364,   364,   364,   364,   364,
     364,   365,   365,   365,   366,   366,   367,   367,   367,   367,
     367,   368,   368,   368,   368,   368,   369,   370,   369,   371,
     371,   372,   372,   372,   373,   374,   373,   375,   375,   376,
     376,   376,   377,   377,   377,   379,   378,   380,   380,   381,
     381,   382,   382,   382,   383,   384,   384,   386,   387,   385,
     388,   388,   389,   389,   390,   391,   391,   392,   392,   392,
     393,   393,   395,   396,   394,   397,   397,   397,   397,   397,
     398,   398,   398,   398,   398,   398,   398,   398,   398,   398,
     398,   398,   398,   398,   398,   398,   398,   398,   398,   398,
     398,   398,   398,   398,   398,   398,   398,   399,   399,   399,
     399,   399,   399,   399,   399,   400,   401,   401,   401,   402,
     402,   403,   403,   403,   405,   406,   404,   407,   407,   408,
     408,   409,   409,   409,   409,   409,   409,   410,   411,   409,
     409,   409,   412,   409,   409,   409,   409,   409,   409,   409,
     409,   409,   409,   409,   409,   409,   413,   414,   409,   409,
     415,   416,   409,   417,   418,   409,   419,   420,   409,   409,
     421,   422,   409,   423,   424,   409,   409,   425,   426,   409,
     427,   428,   409,   409,   429,   430,   409,   431,   432,   409,
     433,   434,   409,   435,   436,   409,   437,   437,   437,   439,
     440,   441,   438,   443,   444,   445,   442,   447,   448,   449,
     446,   450,   450,   450,   450,   450,   451,   451,   451,   451,
     451,   451,   451,   451,   452,   453,   453,   454,   454,   456,
     457,   455,   458,   459,   455,   460,   461,   455,   462,   463,
     455,   464,   464,   465,   466,   467,   465,   468,   468,   469,
     470,   468,   468,   471,   472,   468,   473,   473,   474,   474,
     474,   474,   475,   475,   476,   476,   477,   477
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
       0,     2,     4,     1,     1,     0,     2,     1,     1,     1,
       1,     1,     4,     5,     5,     2,     3,     3,     4,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     1,     1,
       3,     3,     3,     3,     3,     3,     1,     5,     1,     3,
       2,     3,     1,     1,     1,     1,     4,     1,     2,     3,
       3,     3,     3,     2,     1,     3,     0,     3,     0,     2,
       3,     0,     2,     1,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     3,     3,     2,
       2,     3,     4,     3,     2,     2,     2,     2,     2,     3,
       3,     3,     4,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     0,
       1,     1,     3,     0,     4,     3,     7,     2,     2,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     2,     2,     1,     1,     1,     1,     2,     2,     2,
       0,     2,     2,     3,     0,     1,     0,     3,     1,     3,
       0,     0,     7,     0,     0,     7,     3,     2,     2,     2,
       1,     1,     3,     2,     2,     3,     0,     0,     5,     2,
       5,     5,     6,     2,     1,     1,     1,     2,     3,     2,
       2,     3,     2,     3,     2,     2,     3,     4,     1,     1,
       0,     1,     1,     1,     0,     1,     3,     8,     7,     3,
       3,     0,     0,     7,     0,     0,     7,     0,     0,     7,
       0,     0,     6,     5,     8,    10,     1,     3,     1,     2,
       3,     1,     1,     2,     2,     2,     2,     2,     1,     3,
       0,     5,     1,     6,     6,     6,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     2,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     6,     8,     5,     6,     1,     4,     3,
       0,     0,     8,     0,     0,     9,     3,     4,     5,     6,
       0,     0,     5,     3,     4,     4,     3,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     4,     4,
       5,     4,     5,     3,     4,     1,     1,     2,     4,     4,
       7,     8,     3,     5,     0,     0,     8,     3,     3,     3,
       0,     0,     8,     3,     4,     0,     0,     9,     4,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     1,
       4,     4,     4,     4,     4,     1,     6,     7,     6,     6,
       7,     7,     6,     7,     6,     6,     0,     1,     0,     1,
       1,     0,     1,     0,     1,     1,     0,     1,     5,     0,
       0,     4,     0,     9,     0,    10,     3,     4,     1,     3,
       1,     3,     1,     3,     0,     2,     3,     3,     1,     3,
       0,     2,     3,     1,     1,     1,     2,     3,     5,     3,
       3,     1,     1,     1,     0,     1,     1,     4,     3,     3,
       5,     4,     6,     5,     5,     4,     0,     0,     4,     0,
       1,     0,     1,     1,     6,     0,     6,     1,     3,     0,
       1,     3,     0,     1,     1,     0,     5,     2,     3,     4,
       3,     0,     1,     1,     1,     0,     2,     0,     0,    11,
       0,     2,     0,     1,     3,     1,     1,     0,     1,     1,
       0,     3,     0,     0,     7,     1,     4,     3,     3,     5,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     4,     1,
       3,     0,     1,     3,     0,     0,     6,     1,     3,     3,
       4,     1,     1,     1,     1,     2,     3,     0,     0,     6,
       4,     5,     0,     9,     4,     2,     2,     3,     2,     3,
       2,     2,     3,     3,     3,     2,     0,     0,     6,     2,
       0,     0,     6,     0,     0,     6,     0,     0,     6,     1,
       0,     0,     6,     0,     0,     7,     1,     0,     0,     6,
       0,     0,     7,     1,     0,     0,     6,     0,     0,     7,
       0,     0,     6,     0,     0,     6,     1,     3,     3,     0,
       0,     0,     9,     0,     0,     0,     9,     0,     0,     0,
      10,     1,     1,     1,     1,     1,     3,     3,     5,     5,
       6,     6,     8,     8,     1,     1,     3,     0,     1,     0,
       0,    10,     0,     0,    10,     0,     0,    10,     0,     0,
       7,     3,     1,     5,     0,     0,    10,     4,     5,     0,
       0,    10,     5,     0,     0,    10,     1,     3,     4,     5,
       8,    10,     0,     3,     0,     1,     9,     9
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

    case YYSYMBOL_expr_keyword: /* expr_keyword  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_optional_expr_list: /* optional_expr_list  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_optional_expr_list_in_braces: /* optional_expr_list_in_braces  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_type_declaration_no_options_list: /* type_declaration_no_options_list  */
            { deleteTypeDeclarationList(((*yyvaluep).pTypeDeclList)); }
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

    case YYSYMBOL_expr_full_block_assumed_piped: /* expr_full_block_assumed_piped  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_numeric_const: /* expr_numeric_const  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_assign: /* expr_assign  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_assign_pipe_right: /* expr_assign_pipe_right  */
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

    case YYSYMBOL_let_variable_declaration: /* let_variable_declaration  */
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

  case 66: /* expression_else_one_liner: "else" expression_if_one_liner  */
                                                      {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 67: /* expression_if_one_liner: expr  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 68: /* expression_if_one_liner: expression_return_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 69: /* expression_if_one_liner: expression_yield_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 70: /* expression_if_one_liner: expression_break  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 71: /* expression_if_one_liner: expression_continue  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 72: /* expression_if_then_else: if_or_static_if expr expression_block expression_else  */
                                                                                      {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 73: /* expression_if_then_else: expression_if_one_liner "if" expr expression_else_one_liner "end of expression"  */
                                                                                                        {
        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr(ast_wrapInBlock((yyvsp[-4].pExpression))),(yyvsp[-1].pExpression) ? ExpressionPtr(ast_wrapInBlock((yyvsp[-1].pExpression))) : nullptr);
    }
    break;

  case 74: /* expression_for_loop: "for" variable_name_with_pos_list "in" expr_list expression_block  */
                                                                                                       {
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-3].pNameWithPosList),(yyvsp[-1].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-4])),tokAt(scanner,(yylsp[0])));
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

  case 78: /* expression_with_alias: "assume" "name" '=' expr  */
                                                      {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-3])), *(yyvsp[-2].s), (yyvsp[0].pExpression) );
        delete (yyvsp[-2].s);
    }
    break;

  case 79: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 80: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 81: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 82: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 83: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 84: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 85: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 86: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 87: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 88: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 89: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 90: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 91: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 92: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 93: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 94: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 95: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 96: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 97: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 98: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 99: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 100: /* metadata_argument_list: '@' annotation_argument  */
                                      {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 101: /* metadata_argument_list: metadata_argument_list '@' annotation_argument  */
                                                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 102: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 103: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 104: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 105: /* annotation_declaration_basic: annotation_declaration_name  */
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

  case 106: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 107: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 108: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 109: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 110: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 111: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 112: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 113: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 114: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 115: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 116: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 117: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 118: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 119: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 120: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 121: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 122: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 123: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 124: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 125: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 126: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 127: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 128: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 129: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 130: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 131: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 132: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 133: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 134: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 135: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 136: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 137: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 138: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 139: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 140: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 141: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 142: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 143: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 144: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 145: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 146: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 147: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 148: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 149: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 150: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 151: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 152: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 153: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 154: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 155: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 156: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 157: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 158: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 159: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 160: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 161: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 162: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 163: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 164: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 165: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 166: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 167: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 168: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 169: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 170: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 171: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 172: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 173: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 174: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 175: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 176: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 177: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 178: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 179: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 180: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 181: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 182: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 183: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 184: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 185: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 186: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 187: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 188: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 189: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 190: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 191: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 192: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 193: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 194: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 195: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 196: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 197: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 198: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 199: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 200: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 201: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 202: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 203: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 204: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 205: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 206: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 207: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 208: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 209: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 210: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 211: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 212: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 213: /* $@2: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 214: /* function_declaration: optional_public_or_private_function $@2 function_declaration_header expression_block  */
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

  case 215: /* expression_block: "begin of code block" expressions "end of code block"  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 216: /* expression_block: "begin of code block" expressions "end of code block" "finally" "begin of code block" expressions "end of code block"  */
                                                                                          {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 217: /* expr_call_pipe: expr expr_full_block_assumed_piped  */
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

  case 218: /* expr_call_pipe: expression_keyword expr_full_block_assumed_piped  */
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

  case 219: /* expression_any: "end of expression"  */
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

  case 223: /* expression_any: expr_assign "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 224: /* expression_any: expression_delete "end of expression"  */
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

  case 231: /* expression_any: expression_break "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 232: /* expression_any: expression_continue "end of expression"  */
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

  case 237: /* expression_any: expression_label "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 238: /* expression_any: expression_goto "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 239: /* expression_any: "pass" "end of expression"  */
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

  case 246: /* optional_expr_list_in_braces: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 247: /* optional_expr_list_in_braces: '(' optional_expr_list ')'  */
                                              { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 248: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 249: /* type_declaration_no_options_list: type_declaration_no_options_list "end of expression" type_declaration  */
                                                                           {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 250: /* $@3: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 251: /* $@4: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 252: /* expression_keyword: "keyword" '<' $@3 type_declaration_no_options_list '>' $@4 expr  */
                                                                                                                                                     {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 253: /* $@5: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 254: /* $@6: %empty  */
                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 255: /* expression_keyword: "type function" '<' $@5 type_declaration_no_options_list '>' $@6 optional_expr_list_in_braces  */
                                                                                                                                                                                   {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 256: /* expr_pipe: expr_assign " <|" expr_block  */
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
                das2_yyerror(scanner,"can't pipe into [[ make structure ]]. it already has where closure",
                    tokAt(scanner,(yylsp[-1])),CompilationError::cant_pipe);
                delete (yyvsp[0].pExpression);
            } else {
                pMS->block = (yyvsp[0].pExpression);
            }
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        } else {
            das2_yyerror(scanner,"can only pipe into function call or [[ make structure ]]",
                tokAt(scanner,(yylsp[-1])),CompilationError::cant_pipe);
            delete (yyvsp[0].pExpression);
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        }
    }
    break;

  case 257: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 258: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 259: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 260: /* expr_pipe: expr_call_pipe  */
                             {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 261: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 262: /* name_in_namespace: "name" "::" "name"  */
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

  case 263: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 264: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 265: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 266: /* $@7: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 267: /* $@8: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 268: /* new_type_declaration: '<' $@7 type_declaration '>' $@8  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 269: /* expr_new: "new" structure_type_declaration  */
                                                            {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),true);
    }
    break;

  case 270: /* expr_new: "new" structure_type_declaration '(' optional_expr_list ')'  */
                                                                                                   {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 271: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 272: /* expr_new: "new" new_type_declaration '(' use_initializer make_struct_single ')'  */
                                                                                                            {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 273: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 274: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 275: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 276: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 277: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 278: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 279: /* expression_return: expression_return_no_pipe "end of expression"  */
                                              {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 280: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 281: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 282: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 283: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 284: /* expression_yield: expression_yield_no_pipe "end of expression"  */
                                             {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 285: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 286: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 287: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 288: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 289: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 290: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 291: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 292: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 293: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 294: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 295: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 296: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 297: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 298: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr "end of expression"  */
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

  case 299: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 300: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 301: /* $@9: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 302: /* $@10: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 303: /* expr_cast: "cast" '<' $@9 type_declaration_no_options '>' $@10 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 304: /* $@11: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 305: /* $@12: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 306: /* expr_cast: "upcast" '<' $@11 type_declaration_no_options '>' $@12 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 307: /* $@13: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 308: /* $@14: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 309: /* expr_cast: "reinterpret" '<' $@13 type_declaration_no_options '>' $@14 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 310: /* $@15: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 311: /* $@16: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 312: /* expr_type_decl: "type" '<' $@15 type_declaration '>' $@16  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 313: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 314: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 315: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" "end of expression" "name" '>' expr ')'  */
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

  case 316: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 317: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 318: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 319: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 320: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 321: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 322: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 323: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 324: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 325: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 326: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 327: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 328: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 329: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 330: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 331: /* optional_capture_list: '[' '[' capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 332: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 333: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 334: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 335: /* expr_full_block_assumed_piped: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 336: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 337: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 338: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 339: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 340: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 341: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 342: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 343: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 344: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 345: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 346: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 347: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 348: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 349: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 350: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 351: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 352: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 353: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 354: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 355: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 356: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 357: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 358: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 359: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 360: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 361: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 362: /* expr_assign_pipe_right: "@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 363: /* expr_assign_pipe_right: "@@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 364: /* expr_assign_pipe_right: "$ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 365: /* expr_assign_pipe_right: expr_call_pipe  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 366: /* expr_assign_pipe: expr '=' expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 367: /* expr_assign_pipe: expr "<-" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 368: /* expr_assign_pipe: expr "&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 369: /* expr_assign_pipe: expr "|=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 370: /* expr_assign_pipe: expr "^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 371: /* expr_assign_pipe: expr "&&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 372: /* expr_assign_pipe: expr "||=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 373: /* expr_assign_pipe: expr "^^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 374: /* expr_assign_pipe: expr "+=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 375: /* expr_assign_pipe: expr "-=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 376: /* expr_assign_pipe: expr "*=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 377: /* expr_assign_pipe: expr "/=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 378: /* expr_assign_pipe: expr "%=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 379: /* expr_assign_pipe: expr "<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 380: /* expr_assign_pipe: expr ">>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 381: /* expr_assign_pipe: expr "<<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 382: /* expr_assign_pipe: expr ">>>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 383: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 384: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 385: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 386: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 387: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 388: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 389: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 390: /* $@17: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 391: /* $@18: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 392: /* func_addr_expr: '@' '@' '<' $@17 type_declaration_no_options '>' $@18 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 393: /* $@19: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 394: /* $@20: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 395: /* func_addr_expr: '@' '@' '<' $@19 optional_function_argument_list optional_function_type '>' $@20 func_addr_name  */
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

  case 396: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 397: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 398: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 399: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 400: /* $@21: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 401: /* $@22: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 402: /* expr_field: expr '.' $@21 error $@22  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 403: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 404: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
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

  case 405: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 406: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 407: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 408: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 409: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 410: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 411: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 412: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 413: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 414: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 415: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 416: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 417: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 418: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 419: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 420: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 421: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 422: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 423: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 424: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 425: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 426: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 427: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 428: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 429: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 430: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 431: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 432: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 433: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 434: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 435: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 436: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 437: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 438: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 439: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 440: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 441: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 442: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 443: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 444: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 445: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 446: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 447: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 448: /* expr: '(' expr_list optional_comma ')'  */
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

  case 449: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 450: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 451: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 452: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 453: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 454: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 455: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 456: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 457: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 458: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 459: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 460: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 461: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 462: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 463: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 464: /* $@23: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 465: /* $@24: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 466: /* expr: expr "is" "type" '<' $@23 type_declaration_no_options '>' $@24  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 467: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 468: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 469: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 470: /* $@25: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 471: /* $@26: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 472: /* expr: expr "as" "type" '<' $@25 type_declaration '>' $@26  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 473: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 474: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 475: /* $@27: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 476: /* $@28: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 477: /* expr: expr '?' "as" "type" '<' $@27 type_declaration '>' $@28  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 478: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 479: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 480: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 481: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 482: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 483: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 484: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 485: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 486: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 487: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 488: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 489: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 490: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 491: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 492: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 493: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 494: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 495: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 496: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 497: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 498: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 499: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 500: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 501: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 502: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 503: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 504: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 505: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 506: /* optional_field_annotation: %empty  */
                                      { (yyval.aaList) = nullptr; }
    break;

  case 507: /* optional_field_annotation: metadata_argument_list  */
                                      { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 508: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 509: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 510: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 511: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 512: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 513: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 514: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 515: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 516: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 517: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 518: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 519: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 520: /* $@29: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 521: /* struct_variable_declaration_list: struct_variable_declaration_list $@29 structure_variable_declaration "end of expression"  */
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

  case 522: /* $@30: %empty  */
                                                                                                                     {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 523: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@30 function_declaration_header "end of expression"  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 524: /* $@31: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 525: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@31 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 526: /* function_argument_declaration: optional_field_annotation kwd_let_var_or_nothing variable_declaration  */
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

  case 527: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 528: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 529: /* function_argument_list: function_argument_list "end of expression" function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 530: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 531: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 532: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 533: /* tuple_type_list: tuple_type_list "end of expression" tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 534: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 535: /* tuple_alias_type_list: tuple_alias_type_list "end of expression"  */
                                      {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 536: /* tuple_alias_type_list: tuple_alias_type_list tuple_type "end of expression"  */
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

  case 537: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 538: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 539: /* variant_type_list: variant_type_list "end of expression" variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 540: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 541: /* variant_alias_type_list: variant_alias_type_list "end of expression"  */
                                        {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 542: /* variant_alias_type_list: variant_alias_type_list variant_type "end of expression"  */
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

  case 543: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 544: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 545: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 546: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 547: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 548: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 549: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 550: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 551: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 552: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 553: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 554: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 555: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 556: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 557: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 558: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 559: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 560: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 561: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options "end of expression"  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 562: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 563: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 564: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr "end of expression"  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 565: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 566: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 567: /* $@32: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 568: /* global_variable_declaration_list: global_variable_declaration_list $@32 optional_field_annotation let_variable_declaration  */
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

  case 569: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 570: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 571: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 572: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 573: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 574: /* global_let: kwd_let optional_shared optional_public_or_private_variable "begin of code block" global_variable_declaration_list "end of code block"  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 575: /* $@33: %empty  */
                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 576: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@33 optional_field_annotation let_variable_declaration  */
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

  case 577: /* enum_expression: "name"  */
                   {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 578: /* enum_expression: "name" '=' expr  */
                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        (yyval.pEnumPair) = new EnumPair((yyvsp[-2].s),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 579: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 580: /* enum_list: enum_expression  */
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

  case 581: /* enum_list: enum_list ',' enum_expression  */
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

  case 582: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 583: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 584: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 585: /* $@34: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 586: /* single_alias: optional_public_or_private_alias "name" $@34 '=' type_declaration  */
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

  case 591: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 592: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 593: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 594: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 595: /* optional_enum_basic_type_declaration: %empty  */
        {
        (yyval.type) = Type::tInt;
    }
    break;

  case 596: /* optional_enum_basic_type_declaration: ':' enum_basic_type_declaration  */
                                              {
        (yyval.type) = (yyvsp[0].type);
    }
    break;

  case 597: /* $@35: %empty  */
                                                                                                                                                          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 598: /* $@36: %empty  */
                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 599: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name optional_enum_basic_type_declaration "begin of code block" $@35 enum_list optional_comma $@36 "end of code block"  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-3].pEnum),(yyvsp[-6].type));
    }
    break;

  case 600: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 601: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 602: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 603: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 604: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 605: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 606: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 607: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 608: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 609: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 610: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 611: /* optional_struct_variable_declaration_list: "begin of code block" struct_variable_declaration_list "end of code block"  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 612: /* $@37: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 613: /* $@38: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 614: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@37 structure_name $@38 optional_struct_variable_declaration_list  */
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

  case 615: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 616: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 617: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 618: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 619: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 620: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 621: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 622: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 623: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 624: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 625: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 626: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 627: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 628: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 629: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 630: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 631: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 632: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 633: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 634: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 635: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 636: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 637: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 638: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 639: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 640: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 641: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 642: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 643: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 644: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 645: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 646: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 647: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 648: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 649: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 650: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 651: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 652: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 653: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 654: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 655: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 656: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 657: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 658: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 659: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 660: /* bitfield_bits: bitfield_bits "end of expression" "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 661: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 662: /* bitfield_alias_bits: "name"  */
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

  case 663: /* bitfield_alias_bits: bitfield_alias_bits ',' "name"  */
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

  case 664: /* $@39: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 665: /* $@40: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 666: /* bitfield_type_declaration: "bitfield" '<' $@39 bitfield_bits '>' $@40  */
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

  case 667: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 668: /* table_type_pair: type_declaration "end of expression" type_declaration  */
                                                                          {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 669: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 670: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 671: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 672: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 673: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 674: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 675: /* type_declaration_no_options: type_declaration_no_options dim_list  */
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

  case 676: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 677: /* $@41: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 678: /* $@42: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 679: /* type_declaration_no_options: "type" '<' $@41 type_declaration '>' $@42  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 680: /* type_declaration_no_options: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 681: /* type_declaration_no_options: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 682: /* $@43: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 683: /* type_declaration_no_options: '$' name_in_namespace '<' $@43 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 684: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 685: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 686: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 687: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 688: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 689: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 690: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 691: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 692: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 693: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 694: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 695: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 696: /* $@44: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 697: /* $@45: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 698: /* type_declaration_no_options: "smart_ptr" '<' $@44 type_declaration '>' $@45  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 699: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 700: /* $@46: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 701: /* $@47: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 702: /* type_declaration_no_options: "array" '<' $@46 type_declaration '>' $@47  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 703: /* $@48: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 704: /* $@49: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 705: /* type_declaration_no_options: "table" '<' $@48 table_type_pair '>' $@49  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 706: /* $@50: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 707: /* $@51: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 708: /* type_declaration_no_options: "iterator" '<' $@50 type_declaration '>' $@51  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 709: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 710: /* $@52: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 711: /* $@53: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 712: /* type_declaration_no_options: "block" '<' $@52 type_declaration '>' $@53  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 713: /* $@54: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 714: /* $@55: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 715: /* type_declaration_no_options: "block" '<' $@54 optional_function_argument_list optional_function_type '>' $@55  */
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

  case 716: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 717: /* $@56: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 718: /* $@57: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 719: /* type_declaration_no_options: "function" '<' $@56 type_declaration '>' $@57  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 720: /* $@58: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 721: /* $@59: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 722: /* type_declaration_no_options: "function" '<' $@58 optional_function_argument_list optional_function_type '>' $@59  */
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

  case 723: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 724: /* $@60: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 725: /* $@61: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 726: /* type_declaration_no_options: "lambda" '<' $@60 type_declaration '>' $@61  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 727: /* $@62: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 728: /* $@63: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 729: /* type_declaration_no_options: "lambda" '<' $@62 optional_function_argument_list optional_function_type '>' $@63  */
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

  case 730: /* $@64: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 731: /* $@65: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 732: /* type_declaration_no_options: "tuple" '<' $@64 tuple_type_list '>' $@65  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 733: /* $@66: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 734: /* $@67: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 735: /* type_declaration_no_options: "variant" '<' $@66 variant_type_list '>' $@67  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 736: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 737: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 738: /* type_declaration: type_declaration '|' '#'  */
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

  case 739: /* $@68: %empty  */
                                                                      {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 740: /* $@69: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 741: /* $@70: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 742: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias "name" $@68 "begin of code block" $@69 tuple_alias_type_list $@70 "end of code block"  */
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

  case 743: /* $@71: %empty  */
                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 744: /* $@72: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 745: /* $@73: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 746: /* variant_alias_declaration: "variant" optional_public_or_private_alias "name" $@71 "begin of code block" $@72 variant_alias_type_list $@73 "end of code block"  */
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

  case 747: /* $@74: %empty  */
                                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 748: /* $@75: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 749: /* $@76: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-5]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 750: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias "name" $@74 "begin of code block" $@75 bitfield_alias_bits optional_comma $@76 "end of code block"  */
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

  case 751: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 752: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 753: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 754: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 755: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 756: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 757: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 758: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 759: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 760: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 761: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 762: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 763: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 764: /* make_struct_single: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 765: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 766: /* make_struct_dim: make_struct_dim "end of expression" make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 767: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 768: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 769: /* $@77: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 770: /* $@78: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 771: /* make_struct_decl: "struct" '<' $@77 type_declaration_no_options '>' $@78 '(' use_initializer make_struct_dim ')'  */
                                                                                                                                                                                        {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 772: /* $@79: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 773: /* $@80: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 774: /* make_struct_decl: "class" '<' $@79 type_declaration_no_options '>' $@80 '(' use_initializer make_struct_dim ')'  */
                                                                                                                                                                                       {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 775: /* $@81: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 776: /* $@82: %empty  */
                                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 777: /* make_struct_decl: "variant" '<' $@81 type_declaration_no_options '>' $@82 '(' use_initializer make_struct_dim ')'  */
                                                                                                                                                                                          {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 778: /* $@83: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 779: /* $@84: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 780: /* make_struct_decl: "default" '<' $@83 type_declaration_no_options '>' $@84 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 781: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 782: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 783: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 784: /* $@85: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 785: /* $@86: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 786: /* make_tuple_call: "tuple" '<' $@85 type_declaration_no_options '>' $@86 '(' use_initializer make_struct_dim ')'  */
                                                                                                                                                                                        {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 787: /* make_dim_decl: '[' expr_list optional_comma ']'  */
                                                          {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 788: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
                                                                   {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 789: /* $@87: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 790: /* $@88: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 791: /* make_dim_decl: "array" '<' $@87 type_declaration_no_options '>' $@88 '(' expr_list optional_comma ')'  */
                                                                                                                                                                              {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-9])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 792: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 793: /* $@89: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 794: /* $@90: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 795: /* make_dim_decl: "fixed_array" '<' $@89 type_declaration_no_options '>' $@90 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        if ( !mka->makeType->dim.size() ) mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 796: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 797: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 798: /* make_table_decl: "begin of code block" expr_map_tuple_list optional_comma "end of code block"  */
                                                                    {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 799: /* make_table_decl: "table" '(' expr_map_tuple_list optional_comma ')'  */
                                                                             {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 800: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' expr_map_tuple_list optional_comma ')'  */
                                                                                                                       {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-7])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-7])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 801: /* make_table_decl: "table" '<' type_declaration_no_options "end of expression" type_declaration_no_options '>' '(' expr_map_tuple_list optional_comma ')'  */
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

  case 802: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 803: /* array_comprehension_where: "end of expression" "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 804: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 805: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 806: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 807: /* array_comprehension: "begin of code block" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                              {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
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


