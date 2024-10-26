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
  YYSYMBOL_DAS_TBOOL = 69,                 /* "bool"  */
  YYSYMBOL_DAS_TVOID = 70,                 /* "void"  */
  YYSYMBOL_DAS_TSTRING = 71,               /* "string"  */
  YYSYMBOL_DAS_TAUTO = 72,                 /* "auto"  */
  YYSYMBOL_DAS_TINT = 73,                  /* "int"  */
  YYSYMBOL_DAS_TINT2 = 74,                 /* "int2"  */
  YYSYMBOL_DAS_TINT3 = 75,                 /* "int3"  */
  YYSYMBOL_DAS_TINT4 = 76,                 /* "int4"  */
  YYSYMBOL_DAS_TUINT = 77,                 /* "uint"  */
  YYSYMBOL_DAS_TBITFIELD = 78,             /* "bitfield"  */
  YYSYMBOL_DAS_TUINT2 = 79,                /* "uint2"  */
  YYSYMBOL_DAS_TUINT3 = 80,                /* "uint3"  */
  YYSYMBOL_DAS_TUINT4 = 81,                /* "uint4"  */
  YYSYMBOL_DAS_TFLOAT = 82,                /* "float"  */
  YYSYMBOL_DAS_TFLOAT2 = 83,               /* "float2"  */
  YYSYMBOL_DAS_TFLOAT3 = 84,               /* "float3"  */
  YYSYMBOL_DAS_TFLOAT4 = 85,               /* "float4"  */
  YYSYMBOL_DAS_TRANGE = 86,                /* "range"  */
  YYSYMBOL_DAS_TURANGE = 87,               /* "urange"  */
  YYSYMBOL_DAS_TRANGE64 = 88,              /* "range64"  */
  YYSYMBOL_DAS_TURANGE64 = 89,             /* "urange64"  */
  YYSYMBOL_DAS_TBLOCK = 90,                /* "block"  */
  YYSYMBOL_DAS_TINT64 = 91,                /* "int64"  */
  YYSYMBOL_DAS_TUINT64 = 92,               /* "uint64"  */
  YYSYMBOL_DAS_TDOUBLE = 93,               /* "double"  */
  YYSYMBOL_DAS_TFUNCTION = 94,             /* "function"  */
  YYSYMBOL_DAS_TLAMBDA = 95,               /* "lambda"  */
  YYSYMBOL_DAS_TINT8 = 96,                 /* "int8"  */
  YYSYMBOL_DAS_TUINT8 = 97,                /* "uint8"  */
  YYSYMBOL_DAS_TINT16 = 98,                /* "int16"  */
  YYSYMBOL_DAS_TUINT16 = 99,               /* "uint16"  */
  YYSYMBOL_DAS_TTUPLE = 100,               /* "tuple"  */
  YYSYMBOL_DAS_TVARIANT = 101,             /* "variant"  */
  YYSYMBOL_DAS_GENERATOR = 102,            /* "generator"  */
  YYSYMBOL_DAS_YIELD = 103,                /* "yield"  */
  YYSYMBOL_DAS_SEALED = 104,               /* "sealed"  */
  YYSYMBOL_ADDEQU = 105,                   /* "+="  */
  YYSYMBOL_SUBEQU = 106,                   /* "-="  */
  YYSYMBOL_DIVEQU = 107,                   /* "/="  */
  YYSYMBOL_MULEQU = 108,                   /* "*="  */
  YYSYMBOL_MODEQU = 109,                   /* "%="  */
  YYSYMBOL_ANDEQU = 110,                   /* "&="  */
  YYSYMBOL_OREQU = 111,                    /* "|="  */
  YYSYMBOL_XOREQU = 112,                   /* "^="  */
  YYSYMBOL_SHL = 113,                      /* "<<"  */
  YYSYMBOL_SHR = 114,                      /* ">>"  */
  YYSYMBOL_ADDADD = 115,                   /* "++"  */
  YYSYMBOL_SUBSUB = 116,                   /* "--"  */
  YYSYMBOL_LEEQU = 117,                    /* "<="  */
  YYSYMBOL_SHLEQU = 118,                   /* "<<="  */
  YYSYMBOL_SHREQU = 119,                   /* ">>="  */
  YYSYMBOL_GREQU = 120,                    /* ">="  */
  YYSYMBOL_EQUEQU = 121,                   /* "=="  */
  YYSYMBOL_NOTEQU = 122,                   /* "!="  */
  YYSYMBOL_RARROW = 123,                   /* "->"  */
  YYSYMBOL_LARROW = 124,                   /* "<-"  */
  YYSYMBOL_QQ = 125,                       /* "??"  */
  YYSYMBOL_QDOT = 126,                     /* "?."  */
  YYSYMBOL_QBRA = 127,                     /* "?["  */
  YYSYMBOL_LPIPE = 128,                    /* "<|"  */
  YYSYMBOL_LBPIPE = 129,                   /* " <|"  */
  YYSYMBOL_LLPIPE = 130,                   /* "$ <|"  */
  YYSYMBOL_LAPIPE = 131,                   /* "@ <|"  */
  YYSYMBOL_LFPIPE = 132,                   /* "@@ <|"  */
  YYSYMBOL_RPIPE = 133,                    /* "|>"  */
  YYSYMBOL_CLONEEQU = 134,                 /* ":="  */
  YYSYMBOL_ROTL = 135,                     /* "<<<"  */
  YYSYMBOL_ROTR = 136,                     /* ">>>"  */
  YYSYMBOL_ROTLEQU = 137,                  /* "<<<="  */
  YYSYMBOL_ROTREQU = 138,                  /* ">>>="  */
  YYSYMBOL_MAPTO = 139,                    /* "=>"  */
  YYSYMBOL_COLCOL = 140,                   /* "::"  */
  YYSYMBOL_ANDAND = 141,                   /* "&&"  */
  YYSYMBOL_OROR = 142,                     /* "||"  */
  YYSYMBOL_XORXOR = 143,                   /* "^^"  */
  YYSYMBOL_ANDANDEQU = 144,                /* "&&="  */
  YYSYMBOL_OROREQU = 145,                  /* "||="  */
  YYSYMBOL_XORXOREQU = 146,                /* "^^="  */
  YYSYMBOL_DOTDOT = 147,                   /* ".."  */
  YYSYMBOL_MTAG_E = 148,                   /* "$$"  */
  YYSYMBOL_MTAG_I = 149,                   /* "$i"  */
  YYSYMBOL_MTAG_V = 150,                   /* "$v"  */
  YYSYMBOL_MTAG_B = 151,                   /* "$b"  */
  YYSYMBOL_MTAG_A = 152,                   /* "$a"  */
  YYSYMBOL_MTAG_T = 153,                   /* "$t"  */
  YYSYMBOL_MTAG_C = 154,                   /* "$c"  */
  YYSYMBOL_MTAG_F = 155,                   /* "$f"  */
  YYSYMBOL_MTAG_DOTDOTDOT = 156,           /* "..."  */
  YYSYMBOL_INTEGER = 157,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 158,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 159,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 160,    /* "unsigned long integer constant"  */
  YYSYMBOL_UNSIGNED_INT8 = 161,            /* "unsigned int8 constant"  */
  YYSYMBOL_FLOAT = 162,                    /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 163,                   /* "double constant"  */
  YYSYMBOL_NAME = 164,                     /* "name"  */
  YYSYMBOL_KEYWORD = 165,                  /* "keyword"  */
  YYSYMBOL_TYPE_FUNCTION = 166,            /* "type function"  */
  YYSYMBOL_BEGIN_STRING = 167,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 168,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 169,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 170,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 171,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 172,          /* "}"  */
  YYSYMBOL_END_OF_READ = 173,              /* "end of failed eader macro"  */
  YYSYMBOL_174_begin_of_code_block_ = 174, /* "begin of code block"  */
  YYSYMBOL_175_end_of_code_block_ = 175,   /* "end of code block"  */
  YYSYMBOL_176_end_of_expression_ = 176,   /* "end of expression"  */
  YYSYMBOL_SEMICOLON_CUR_CUR = 177,        /* ";}}"  */
  YYSYMBOL_SEMICOLON_CUR_SQR = 178,        /* ";}]"  */
  YYSYMBOL_SEMICOLON_SQR_SQR = 179,        /* ";]]"  */
  YYSYMBOL_COMMA_SQR_SQR = 180,            /* ",]]"  */
  YYSYMBOL_COMMA_CUR_SQR = 181,            /* ",}]"  */
  YYSYMBOL_182_ = 182,                     /* ','  */
  YYSYMBOL_183_ = 183,                     /* '='  */
  YYSYMBOL_184_ = 184,                     /* '?'  */
  YYSYMBOL_185_ = 185,                     /* ':'  */
  YYSYMBOL_186_ = 186,                     /* '|'  */
  YYSYMBOL_187_ = 187,                     /* '^'  */
  YYSYMBOL_188_ = 188,                     /* '&'  */
  YYSYMBOL_189_ = 189,                     /* '<'  */
  YYSYMBOL_190_ = 190,                     /* '>'  */
  YYSYMBOL_191_ = 191,                     /* '-'  */
  YYSYMBOL_192_ = 192,                     /* '+'  */
  YYSYMBOL_193_ = 193,                     /* '*'  */
  YYSYMBOL_194_ = 194,                     /* '/'  */
  YYSYMBOL_195_ = 195,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 196,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 197,               /* UNARY_PLUS  */
  YYSYMBOL_198_ = 198,                     /* '~'  */
  YYSYMBOL_199_ = 199,                     /* '!'  */
  YYSYMBOL_PRE_INC = 200,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 201,                  /* PRE_DEC  */
  YYSYMBOL_POST_INC = 202,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 203,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 204,                    /* DEREF  */
  YYSYMBOL_205_ = 205,                     /* '.'  */
  YYSYMBOL_206_ = 206,                     /* '['  */
  YYSYMBOL_207_ = 207,                     /* ']'  */
  YYSYMBOL_208_ = 208,                     /* '('  */
  YYSYMBOL_209_ = 209,                     /* ')'  */
  YYSYMBOL_210_ = 210,                     /* '$'  */
  YYSYMBOL_211_ = 211,                     /* '@'  */
  YYSYMBOL_212_ = 212,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 213,                 /* $accept  */
  YYSYMBOL_program = 214,                  /* program  */
  YYSYMBOL_top_level_reader_macro = 215,   /* top_level_reader_macro  */
  YYSYMBOL_optional_public_or_private_module = 216, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 217,              /* module_name  */
  YYSYMBOL_module_declaration = 218,       /* module_declaration  */
  YYSYMBOL_character_sequence = 219,       /* character_sequence  */
  YYSYMBOL_string_constant = 220,          /* string_constant  */
  YYSYMBOL_string_builder_body = 221,      /* string_builder_body  */
  YYSYMBOL_string_builder = 222,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 223, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 224,              /* expr_reader  */
  YYSYMBOL_225_1 = 225,                    /* $@1  */
  YYSYMBOL_options_declaration = 226,      /* options_declaration  */
  YYSYMBOL_require_declaration = 227,      /* require_declaration  */
  YYSYMBOL_keyword_or_name = 228,          /* keyword_or_name  */
  YYSYMBOL_require_module_name = 229,      /* require_module_name  */
  YYSYMBOL_require_module = 230,           /* require_module  */
  YYSYMBOL_is_public_module = 231,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 232,       /* expect_declaration  */
  YYSYMBOL_expect_list = 233,              /* expect_list  */
  YYSYMBOL_expect_error = 234,             /* expect_error  */
  YYSYMBOL_expression_label = 235,         /* expression_label  */
  YYSYMBOL_expression_goto = 236,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 237,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 238,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 239,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 240, /* expression_else_one_liner  */
  YYSYMBOL_241_2 = 241,                    /* $@2  */
  YYSYMBOL_expression_if_one_liner = 242,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 243,  /* expression_if_then_else  */
  YYSYMBOL_244_3 = 244,                    /* $@3  */
  YYSYMBOL_expression_for_loop = 245,      /* expression_for_loop  */
  YYSYMBOL_246_4 = 246,                    /* $@4  */
  YYSYMBOL_expression_unsafe = 247,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 248,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 249,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 250,    /* expression_with_alias  */
  YYSYMBOL_251_5 = 251,                    /* $@5  */
  YYSYMBOL_annotation_argument_value = 252, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 253, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 254, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 255,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 256, /* annotation_argument_list  */
  YYSYMBOL_annotation_declaration_name = 257, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 258, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 259,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 260,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 261, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 262, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 263,   /* optional_function_type  */
  YYSYMBOL_function_name = 264,            /* function_name  */
  YYSYMBOL_global_function_declaration = 265, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 266, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 267, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 268,     /* function_declaration  */
  YYSYMBOL_269_6 = 269,                    /* $@6  */
  YYSYMBOL_expression_block = 270,         /* expression_block  */
  YYSYMBOL_expr_call_pipe = 271,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 272,           /* expression_any  */
  YYSYMBOL_expressions = 273,              /* expressions  */
  YYSYMBOL_expr_keyword = 274,             /* expr_keyword  */
  YYSYMBOL_optional_expr_list = 275,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_list_in_braces = 276, /* optional_expr_list_in_braces  */
  YYSYMBOL_type_declaration_no_options_list = 277, /* type_declaration_no_options_list  */
  YYSYMBOL_expression_keyword = 278,       /* expression_keyword  */
  YYSYMBOL_279_7 = 279,                    /* $@7  */
  YYSYMBOL_280_8 = 280,                    /* $@8  */
  YYSYMBOL_281_9 = 281,                    /* $@9  */
  YYSYMBOL_282_10 = 282,                   /* $@10  */
  YYSYMBOL_expr_pipe = 283,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 284,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 285,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 286,     /* new_type_declaration  */
  YYSYMBOL_287_11 = 287,                   /* $@11  */
  YYSYMBOL_288_12 = 288,                   /* $@12  */
  YYSYMBOL_expr_new = 289,                 /* expr_new  */
  YYSYMBOL_expression_break = 290,         /* expression_break  */
  YYSYMBOL_expression_continue = 291,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 292, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 293,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 294, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 295,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 296,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 297,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 298,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 299,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 300,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 301, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 302,           /* expression_let  */
  YYSYMBOL_expr_cast = 303,                /* expr_cast  */
  YYSYMBOL_304_13 = 304,                   /* $@13  */
  YYSYMBOL_305_14 = 305,                   /* $@14  */
  YYSYMBOL_306_15 = 306,                   /* $@15  */
  YYSYMBOL_307_16 = 307,                   /* $@16  */
  YYSYMBOL_308_17 = 308,                   /* $@17  */
  YYSYMBOL_309_18 = 309,                   /* $@18  */
  YYSYMBOL_expr_type_decl = 310,           /* expr_type_decl  */
  YYSYMBOL_311_19 = 311,                   /* $@19  */
  YYSYMBOL_312_20 = 312,                   /* $@20  */
  YYSYMBOL_expr_type_info = 313,           /* expr_type_info  */
  YYSYMBOL_expr_list = 314,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 315,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 316,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 317,            /* capture_entry  */
  YYSYMBOL_capture_list = 318,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 319,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 320,               /* expr_block  */
  YYSYMBOL_expr_full_block = 321,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 322, /* expr_full_block_assumed_piped  */
  YYSYMBOL_323_21 = 323,                   /* $@21  */
  YYSYMBOL_expr_numeric_const = 324,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 325,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe_right = 326,   /* expr_assign_pipe_right  */
  YYSYMBOL_expr_assign_pipe = 327,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 328,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 329,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 330,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 331,           /* func_addr_expr  */
  YYSYMBOL_332_22 = 332,                   /* $@22  */
  YYSYMBOL_333_23 = 333,                   /* $@23  */
  YYSYMBOL_334_24 = 334,                   /* $@24  */
  YYSYMBOL_335_25 = 335,                   /* $@25  */
  YYSYMBOL_expr_field = 336,               /* expr_field  */
  YYSYMBOL_337_26 = 337,                   /* $@26  */
  YYSYMBOL_338_27 = 338,                   /* $@27  */
  YYSYMBOL_expr_call = 339,                /* expr_call  */
  YYSYMBOL_expr = 340,                     /* expr  */
  YYSYMBOL_341_28 = 341,                   /* $@28  */
  YYSYMBOL_342_29 = 342,                   /* $@29  */
  YYSYMBOL_343_30 = 343,                   /* $@30  */
  YYSYMBOL_344_31 = 344,                   /* $@31  */
  YYSYMBOL_345_32 = 345,                   /* $@32  */
  YYSYMBOL_346_33 = 346,                   /* $@33  */
  YYSYMBOL_expr_mtag = 347,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 348, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 349,        /* optional_override  */
  YYSYMBOL_optional_constant = 350,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 351, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 352, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 353, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 354, /* struct_variable_declaration_list  */
  YYSYMBOL_355_34 = 355,                   /* $@34  */
  YYSYMBOL_356_35 = 356,                   /* $@35  */
  YYSYMBOL_357_36 = 357,                   /* $@36  */
  YYSYMBOL_function_argument_declaration = 358, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 359,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 360,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 361,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 362,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 363,             /* variant_type  */
  YYSYMBOL_variant_type_list = 364,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 365,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 366,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 367,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 368,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 369,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 370, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 371, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 372, /* global_variable_declaration_list  */
  YYSYMBOL_373_37 = 373,                   /* $@37  */
  YYSYMBOL_optional_shared = 374,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 375, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 376,               /* global_let  */
  YYSYMBOL_377_38 = 377,                   /* $@38  */
  YYSYMBOL_enum_list = 378,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 379, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 380,             /* single_alias  */
  YYSYMBOL_381_39 = 381,                   /* $@39  */
  YYSYMBOL_alias_list = 382,               /* alias_list  */
  YYSYMBOL_alias_declaration = 383,        /* alias_declaration  */
  YYSYMBOL_384_40 = 384,                   /* $@40  */
  YYSYMBOL_optional_public_or_private_enum = 385, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 386,                /* enum_name  */
  YYSYMBOL_enum_declaration = 387,         /* enum_declaration  */
  YYSYMBOL_388_41 = 388,                   /* $@41  */
  YYSYMBOL_389_42 = 389,                   /* $@42  */
  YYSYMBOL_390_43 = 390,                   /* $@43  */
  YYSYMBOL_391_44 = 391,                   /* $@44  */
  YYSYMBOL_optional_structure_parent = 392, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 393,          /* optional_sealed  */
  YYSYMBOL_structure_name = 394,           /* structure_name  */
  YYSYMBOL_class_or_struct = 395,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 396, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 397, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 398,    /* structure_declaration  */
  YYSYMBOL_399_45 = 399,                   /* $@45  */
  YYSYMBOL_400_46 = 400,                   /* $@46  */
  YYSYMBOL_variable_name_with_pos_list = 401, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 402,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 403, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 404, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 405,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 406,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 407,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 408, /* bitfield_type_declaration  */
  YYSYMBOL_409_47 = 409,                   /* $@47  */
  YYSYMBOL_410_48 = 410,                   /* $@48  */
  YYSYMBOL_table_type_pair = 411,          /* table_type_pair  */
  YYSYMBOL_dim_list = 412,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 413, /* type_declaration_no_options  */
  YYSYMBOL_414_49 = 414,                   /* $@49  */
  YYSYMBOL_415_50 = 415,                   /* $@50  */
  YYSYMBOL_416_51 = 416,                   /* $@51  */
  YYSYMBOL_417_52 = 417,                   /* $@52  */
  YYSYMBOL_418_53 = 418,                   /* $@53  */
  YYSYMBOL_419_54 = 419,                   /* $@54  */
  YYSYMBOL_420_55 = 420,                   /* $@55  */
  YYSYMBOL_421_56 = 421,                   /* $@56  */
  YYSYMBOL_422_57 = 422,                   /* $@57  */
  YYSYMBOL_423_58 = 423,                   /* $@58  */
  YYSYMBOL_424_59 = 424,                   /* $@59  */
  YYSYMBOL_425_60 = 425,                   /* $@60  */
  YYSYMBOL_426_61 = 426,                   /* $@61  */
  YYSYMBOL_427_62 = 427,                   /* $@62  */
  YYSYMBOL_428_63 = 428,                   /* $@63  */
  YYSYMBOL_429_64 = 429,                   /* $@64  */
  YYSYMBOL_430_65 = 430,                   /* $@65  */
  YYSYMBOL_431_66 = 431,                   /* $@66  */
  YYSYMBOL_432_67 = 432,                   /* $@67  */
  YYSYMBOL_433_68 = 433,                   /* $@68  */
  YYSYMBOL_434_69 = 434,                   /* $@69  */
  YYSYMBOL_435_70 = 435,                   /* $@70  */
  YYSYMBOL_436_71 = 436,                   /* $@71  */
  YYSYMBOL_437_72 = 437,                   /* $@72  */
  YYSYMBOL_438_73 = 438,                   /* $@73  */
  YYSYMBOL_439_74 = 439,                   /* $@74  */
  YYSYMBOL_440_75 = 440,                   /* $@75  */
  YYSYMBOL_type_declaration = 441,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 442,  /* tuple_alias_declaration  */
  YYSYMBOL_443_76 = 443,                   /* $@76  */
  YYSYMBOL_444_77 = 444,                   /* $@77  */
  YYSYMBOL_445_78 = 445,                   /* $@78  */
  YYSYMBOL_446_79 = 446,                   /* $@79  */
  YYSYMBOL_variant_alias_declaration = 447, /* variant_alias_declaration  */
  YYSYMBOL_448_80 = 448,                   /* $@80  */
  YYSYMBOL_449_81 = 449,                   /* $@81  */
  YYSYMBOL_450_82 = 450,                   /* $@82  */
  YYSYMBOL_451_83 = 451,                   /* $@83  */
  YYSYMBOL_bitfield_alias_declaration = 452, /* bitfield_alias_declaration  */
  YYSYMBOL_453_84 = 453,                   /* $@84  */
  YYSYMBOL_454_85 = 454,                   /* $@85  */
  YYSYMBOL_455_86 = 455,                   /* $@86  */
  YYSYMBOL_456_87 = 456,                   /* $@87  */
  YYSYMBOL_make_decl = 457,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 458,       /* make_struct_fields  */
  YYSYMBOL_make_struct_single = 459,       /* make_struct_single  */
  YYSYMBOL_make_struct_decl = 460,         /* make_struct_decl  */
  YYSYMBOL_461_88 = 461,                   /* $@88  */
  YYSYMBOL_462_89 = 462,                   /* $@89  */
  YYSYMBOL_463_90 = 463,                   /* $@90  */
  YYSYMBOL_464_91 = 464,                   /* $@91  */
  YYSYMBOL_465_92 = 465,                   /* $@92  */
  YYSYMBOL_466_93 = 466,                   /* $@93  */
  YYSYMBOL_467_94 = 467,                   /* $@94  */
  YYSYMBOL_468_95 = 468,                   /* $@95  */
  YYSYMBOL_make_map_tuple = 469,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 470,          /* make_tuple_call  */
  YYSYMBOL_471_96 = 471,                   /* $@96  */
  YYSYMBOL_472_97 = 472,                   /* $@97  */
  YYSYMBOL_make_dim_decl = 473,            /* make_dim_decl  */
  YYSYMBOL_474_98 = 474,                   /* $@98  */
  YYSYMBOL_475_99 = 475,                   /* $@99  */
  YYSYMBOL_476_100 = 476,                  /* $@100  */
  YYSYMBOL_477_101 = 477,                  /* $@101  */
  YYSYMBOL_expr_map_tuple_list = 478,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 479,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 480, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 481,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 482       /* array_comprehension  */
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
#define YYLAST   12382

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  213
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  270
/* YYNRULES -- Number of rules.  */
#define YYNRULES  812
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1474

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   440


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
       2,     2,     2,   199,     2,   212,   210,   195,   188,     2,
     208,   209,   193,   192,   182,   191,   205,   194,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   185,   176,
     189,   183,   190,   184,   211,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   206,     2,   207,   187,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   174,   186,   175,   198,     2,     2,     2,
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
     165,   166,   167,   168,   169,   170,   171,   172,   173,   177,
     178,   179,   180,   181,   196,   197,   200,   201,   202,   203,
     204
};

#if DAS2_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   522,   522,   523,   528,   529,   530,   531,   532,   533,
     534,   535,   536,   537,   538,   539,   540,   544,   550,   551,
     552,   556,   557,   561,   579,   580,   581,   582,   586,   590,
     595,   604,   612,   628,   633,   641,   641,   680,   710,   714,
     715,   716,   720,   723,   727,   733,   742,   745,   751,   752,
     756,   760,   761,   765,   768,   774,   780,   783,   789,   790,
     794,   795,   796,   805,   806,   810,   811,   811,   817,   818,
     819,   820,   821,   825,   831,   831,   837,   837,   843,   851,
     861,   870,   870,   877,   878,   879,   880,   881,   882,   886,
     891,   899,   900,   901,   905,   906,   907,   908,   909,   910,
     911,   912,   918,   921,   927,   928,   929,   933,   946,   964,
     967,   975,   986,   997,  1008,  1011,  1018,  1022,  1029,  1030,
    1034,  1035,  1036,  1040,  1043,  1050,  1054,  1055,  1056,  1057,
    1058,  1059,  1060,  1061,  1062,  1063,  1064,  1065,  1066,  1067,
    1068,  1069,  1070,  1071,  1072,  1073,  1074,  1075,  1076,  1077,
    1078,  1079,  1080,  1081,  1082,  1083,  1084,  1085,  1086,  1087,
    1088,  1089,  1090,  1091,  1092,  1093,  1094,  1095,  1096,  1097,
    1098,  1099,  1100,  1101,  1102,  1103,  1104,  1105,  1106,  1107,
    1108,  1109,  1110,  1111,  1112,  1113,  1114,  1115,  1116,  1117,
    1118,  1119,  1120,  1121,  1122,  1123,  1124,  1125,  1126,  1127,
    1128,  1129,  1130,  1131,  1132,  1133,  1134,  1135,  1136,  1137,
    1142,  1160,  1161,  1162,  1166,  1172,  1172,  1189,  1193,  1204,
    1213,  1225,  1226,  1227,  1228,  1229,  1230,  1231,  1232,  1233,
    1234,  1235,  1236,  1237,  1238,  1239,  1240,  1241,  1242,  1243,
    1244,  1245,  1249,  1254,  1260,  1266,  1277,  1278,  1282,  1283,
    1287,  1291,  1298,  1298,  1298,  1304,  1304,  1304,  1313,  1347,
    1350,  1353,  1356,  1362,  1363,  1374,  1378,  1381,  1389,  1389,
    1389,  1392,  1398,  1401,  1404,  1408,  1415,  1421,  1425,  1429,
    1432,  1435,  1443,  1446,  1449,  1457,  1460,  1468,  1471,  1474,
    1482,  1488,  1489,  1490,  1494,  1495,  1499,  1500,  1504,  1509,
    1517,  1523,  1535,  1538,  1544,  1544,  1544,  1547,  1547,  1547,
    1552,  1552,  1552,  1560,  1560,  1560,  1566,  1576,  1587,  1602,
    1605,  1611,  1612,  1619,  1630,  1631,  1632,  1636,  1637,  1638,
    1639,  1643,  1648,  1656,  1657,  1661,  1666,  1673,  1680,  1680,
    1689,  1690,  1691,  1692,  1693,  1694,  1695,  1699,  1700,  1701,
    1702,  1703,  1704,  1705,  1706,  1707,  1708,  1709,  1710,  1711,
    1712,  1713,  1714,  1715,  1716,  1717,  1721,  1722,  1723,  1724,
    1729,  1730,  1731,  1732,  1733,  1734,  1735,  1736,  1737,  1738,
    1739,  1740,  1741,  1742,  1743,  1744,  1745,  1750,  1757,  1769,
    1774,  1784,  1788,  1795,  1798,  1798,  1798,  1803,  1803,  1803,
    1816,  1820,  1824,  1829,  1836,  1836,  1836,  1843,  1847,  1856,
    1860,  1863,  1869,  1870,  1871,  1872,  1873,  1874,  1875,  1876,
    1877,  1878,  1879,  1880,  1881,  1882,  1883,  1884,  1885,  1886,
    1887,  1888,  1889,  1890,  1891,  1892,  1893,  1894,  1895,  1896,
    1897,  1898,  1899,  1900,  1901,  1902,  1903,  1904,  1910,  1911,
    1912,  1913,  1914,  1927,  1928,  1929,  1930,  1931,  1932,  1933,
    1934,  1935,  1936,  1937,  1938,  1941,  1944,  1945,  1948,  1948,
    1948,  1951,  1956,  1960,  1964,  1964,  1964,  1969,  1972,  1976,
    1976,  1976,  1981,  1984,  1985,  1986,  1987,  1988,  1989,  1990,
    1991,  1992,  1994,  1998,  1999,  2004,  2008,  2009,  2010,  2011,
    2012,  2013,  2014,  2018,  2022,  2026,  2030,  2034,  2038,  2042,
    2046,  2050,  2057,  2058,  2062,  2063,  2064,  2068,  2069,  2073,
    2074,  2075,  2079,  2080,  2084,  2095,  2098,  2098,  2117,  2116,
    2131,  2130,  2143,  2152,  2161,  2171,  2172,  2176,  2179,  2188,
    2189,  2193,  2196,  2199,  2215,  2224,  2225,  2229,  2232,  2235,
    2249,  2250,  2254,  2260,  2266,  2269,  2273,  2279,  2288,  2289,
    2290,  2294,  2295,  2299,  2306,  2311,  2320,  2326,  2337,  2340,
    2345,  2350,  2358,  2369,  2372,  2372,  2392,  2393,  2397,  2398,
    2399,  2403,  2406,  2406,  2425,  2428,  2431,  2446,  2465,  2466,
    2467,  2472,  2472,  2498,  2499,  2503,  2504,  2504,  2508,  2509,
    2510,  2514,  2524,  2529,  2524,  2541,  2546,  2541,  2561,  2562,
    2566,  2567,  2571,  2577,  2578,  2582,  2583,  2584,  2588,  2591,
    2597,  2602,  2597,  2616,  2623,  2628,  2637,  2643,  2654,  2655,
    2656,  2657,  2658,  2659,  2660,  2661,  2662,  2663,  2664,  2665,
    2666,  2667,  2668,  2669,  2670,  2671,  2672,  2673,  2674,  2675,
    2676,  2677,  2678,  2679,  2680,  2684,  2685,  2686,  2687,  2688,
    2689,  2693,  2704,  2708,  2715,  2727,  2734,  2743,  2748,  2751,
    2764,  2764,  2764,  2777,  2781,  2788,  2792,  2799,  2800,  2801,
    2802,  2803,  2818,  2824,  2824,  2824,  2828,  2833,  2840,  2840,
    2847,  2851,  2855,  2860,  2865,  2870,  2875,  2879,  2883,  2888,
    2892,  2896,  2901,  2901,  2901,  2907,  2914,  2914,  2914,  2919,
    2919,  2919,  2925,  2925,  2925,  2930,  2934,  2934,  2934,  2939,
    2939,  2939,  2948,  2952,  2952,  2952,  2957,  2957,  2957,  2966,
    2970,  2970,  2970,  2975,  2975,  2975,  2984,  2984,  2984,  2990,
    2990,  2990,  2999,  3002,  3013,  3029,  3029,  3034,  3039,  3029,
    3064,  3064,  3069,  3075,  3064,  3100,  3100,  3105,  3110,  3100,
    3140,  3141,  3142,  3143,  3144,  3148,  3155,  3162,  3168,  3174,
    3181,  3188,  3194,  3204,  3212,  3212,  3212,  3220,  3220,  3220,
    3227,  3227,  3227,  3234,  3234,  3234,  3245,  3251,  3257,  3263,
    3263,  3263,  3273,  3281,  3289,  3289,  3289,  3297,  3304,  3304,
    3304,  3314,  3317,  3323,  3331,  3339,  3347,  3360,  3361,  3365,
    3366,  3371,  3374
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
  "\"bool\"", "\"void\"", "\"string\"", "\"auto\"", "\"int\"", "\"int2\"",
  "\"int3\"", "\"int4\"", "\"uint\"", "\"bitfield\"", "\"uint2\"",
  "\"uint3\"", "\"uint4\"", "\"float\"", "\"float2\"", "\"float3\"",
  "\"float4\"", "\"range\"", "\"urange\"", "\"range64\"", "\"urange64\"",
  "\"block\"", "\"int64\"", "\"uint64\"", "\"double\"", "\"function\"",
  "\"lambda\"", "\"int8\"", "\"uint8\"", "\"int16\"", "\"uint16\"",
  "\"tuple\"", "\"variant\"", "\"generator\"", "\"yield\"", "\"sealed\"",
  "\"+=\"", "\"-=\"", "\"/=\"", "\"*=\"", "\"%=\"", "\"&=\"", "\"|=\"",
  "\"^=\"", "\"<<\"", "\">>\"", "\"++\"", "\"--\"", "\"<=\"", "\"<<=\"",
  "\">>=\"", "\">=\"", "\"==\"", "\"!=\"", "\"->\"", "\"<-\"", "\"??\"",
  "\"?.\"", "\"?[\"", "\"<|\"", "\" <|\"", "\"$ <|\"", "\"@ <|\"",
  "\"@@ <|\"", "\"|>\"", "\":=\"", "\"<<<\"", "\">>>\"", "\"<<<=\"",
  "\">>>=\"", "\"=>\"", "\"::\"", "\"&&\"", "\"||\"", "\"^^\"", "\"&&=\"",
  "\"||=\"", "\"^^=\"", "\"..\"", "\"$$\"", "\"$i\"", "\"$v\"", "\"$b\"",
  "\"$a\"", "\"$t\"", "\"$c\"", "\"$f\"", "\"...\"",
  "\"integer constant\"", "\"long integer constant\"",
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
  "function_declaration", "$@6", "expression_block", "expr_call_pipe",
  "expression_any", "expressions", "expr_keyword", "optional_expr_list",
  "optional_expr_list_in_braces", "type_declaration_no_options_list",
  "expression_keyword", "$@7", "$@8", "$@9", "$@10", "expr_pipe",
  "name_in_namespace", "expression_delete", "new_type_declaration", "$@11",
  "$@12", "expr_new", "expression_break", "expression_continue",
  "expression_return_no_pipe", "expression_return",
  "expression_yield_no_pipe", "expression_yield", "expression_try_catch",
  "kwd_let_var_or_nothing", "kwd_let", "optional_in_scope",
  "tuple_expansion", "tuple_expansion_variable_declaration",
  "expression_let", "expr_cast", "$@13", "$@14", "$@15", "$@16", "$@17",
  "$@18", "expr_type_decl", "$@19", "$@20", "expr_type_info", "expr_list",
  "block_or_simple_block", "block_or_lambda", "capture_entry",
  "capture_list", "optional_capture_list", "expr_block", "expr_full_block",
  "expr_full_block_assumed_piped", "$@21", "expr_numeric_const",
  "expr_assign", "expr_assign_pipe_right", "expr_assign_pipe",
  "expr_named_call", "expr_method_call", "func_addr_name",
  "func_addr_expr", "$@22", "$@23", "$@24", "$@25", "expr_field", "$@26",
  "$@27", "expr_call", "expr", "$@28", "$@29", "$@30", "$@31", "$@32",
  "$@33", "expr_mtag", "optional_field_annotation", "optional_override",
  "optional_constant", "optional_public_or_private_member_variable",
  "optional_static_member_variable", "structure_variable_declaration",
  "struct_variable_declaration_list", "$@34", "$@35", "$@36",
  "function_argument_declaration", "function_argument_list", "tuple_type",
  "tuple_type_list", "tuple_alias_type_list", "variant_type",
  "variant_type_list", "variant_alias_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list", "let_variable_declaration",
  "global_variable_declaration_list", "$@37", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@38", "enum_list",
  "optional_public_or_private_alias", "single_alias", "$@39", "alias_list",
  "alias_declaration", "$@40", "optional_public_or_private_enum",
  "enum_name", "enum_declaration", "$@41", "$@42", "$@43", "$@44",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "optional_struct_variable_declaration_list", "structure_declaration",
  "$@45", "$@46", "variable_name_with_pos_list", "basic_type_declaration",
  "enum_basic_type_declaration", "structure_type_declaration",
  "auto_type_declaration", "bitfield_bits", "bitfield_alias_bits",
  "bitfield_type_declaration", "$@47", "$@48", "table_type_pair",
  "dim_list", "type_declaration_no_options", "$@49", "$@50", "$@51",
  "$@52", "$@53", "$@54", "$@55", "$@56", "$@57", "$@58", "$@59", "$@60",
  "$@61", "$@62", "$@63", "$@64", "$@65", "$@66", "$@67", "$@68", "$@69",
  "$@70", "$@71", "$@72", "$@73", "$@74", "$@75", "type_declaration",
  "tuple_alias_declaration", "$@76", "$@77", "$@78", "$@79",
  "variant_alias_declaration", "$@80", "$@81", "$@82", "$@83",
  "bitfield_alias_declaration", "$@84", "$@85", "$@86", "$@87",
  "make_decl", "make_struct_fields", "make_struct_single",
  "make_struct_decl", "$@88", "$@89", "$@90", "$@91", "$@92", "$@93",
  "$@94", "$@95", "make_map_tuple", "make_tuple_call", "$@96", "$@97",
  "make_dim_decl", "$@98", "$@99", "$@100", "$@101", "expr_map_tuple_list",
  "make_table_decl", "array_comprehension_where", "optional_comma",
  "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1302)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-734)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1302,    30, -1302, -1302,    34,   -86,   115,   -26, -1302,  -110,
     348,   348,   348, -1302,   -12,    40, -1302, -1302,    10, -1302,
   -1302, -1302,   328, -1302,   151, -1302, -1302, -1302, -1302, -1302,
   -1302, -1302, -1302, -1302, -1302,    51, -1302,    76,   240,   155,
   -1302, -1302, -1302, -1302,   115, -1302,    24, -1302,   348,   348,
   -1302, -1302,   151, -1302, -1302, -1302, -1302, -1302,   190,   298,
   -1302, -1302, -1302,    40,    40,    40,   279, -1302,   449,   -38,
   -1302, -1302, -1302, -1302,   359,   430,   463, -1302,   464,    43,
      34,   382,   -86,   306,   358, -1302,   429,   429, -1302,   393,
     399,   -30,   404,   471,   419,   425,   437, -1302,   441,   447,
   -1302, -1302,   -77,    34,    40,    40,    40,    40, -1302, -1302,
   -1302, -1302, -1302, -1302, -1302,   452, -1302, -1302, -1302, -1302,
   -1302,   444, -1302, -1302, -1302, -1302, -1302,   220,   228, -1302,
   -1302, -1302, -1302,   565, -1302, -1302, -1302, -1302, -1302,   446,
   -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302,   456,
   -1302,   -18, -1302,   326,   484,   449, 12218, -1302,   176,   524,
   -1302,   426, -1302, -1302,   428, -1302, -1302, -1302, -1302, -1302,
   -1302, -1302,    12, -1302,   453, -1302,   457,   461,   467, -1302,
   -1302, 10920, -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302,
   -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302,
   -1302, -1302, -1302, -1302, -1302, -1302, -1302,   603,   604, -1302,
     434,   469, -1302,   246, -1302,   480, -1302,   473,   440,   202,
   -1302, -1302, -1302,   228, -1302,  7713, -1302, -1302, -1302,   485,
     486, -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302,
   -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302,
   -1302,   487,   450, -1302, -1302, -1302, -1302, -1302, -1302, -1302,
   -1302, -1302, -1302, -1302, -1302,   632, -1302, -1302, -1302, -1302,
   -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302,   490,   454,
   -1302, -1302,    26,   474, -1302, -1302, -1302, -1302, -1302, -1302,
   -1302, -1302, -1302,   482,   477,   491, -1302,   426,    34,   460,
     618,   333, -1302, -1302,   489,   492,   493,   476,   494,   497,
   -1302, -1302, -1302,   483, -1302, -1302, -1302, -1302, -1302,   499,
   -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302,
   -1302,   503, -1302, -1302, -1302,   504,   507, -1302, -1302, -1302,
   -1302,   508,   509,   495,   -12, -1302, -1302, -1302, -1302, -1302,
    7526,   514, -1302, -1302, -1302, -1302, -1302, -1302, -1302,   505,
     537, -1302,   500, -1302,   114, -1302,   -10,  7713, -1302,  2089,
     223, -1302,   -12, -1302, -1302, -1302,   202,   -20,  7253,   545,
     546,  7713, -1302,   -15, -1302, -1302, -1302,  7253, -1302, -1302,
     547, -1302,   175,   215,   218, -1302, -1302,  7253,  -127, -1302,
   -1302, -1302,    -3, -1302, -1302, -1302,    35,  4081, -1302,   510,
     889,   255,  7605,   297, -1302, -1302,  7253, -1302, -1302,   237,
     -92, -1302,   514, -1302,   525,   528,  7253, -1302, -1302, -1302,
   -1302, -1302,    82,   511,   529,   -49,  2495, -1302, -1302,   469,
      81,  4279,   512,  7253,   551,   532,   533,   516, -1302,   549,
     538,   569,  4477,   -27,   116,   539, -1302,   149,   541,   543,
    2693,  7253,  7253,    63,    63,    63,   535,   542,   544,   550,
     552,   553, -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302,
    4675,   548, -1302,  4875,   700, -1302,  7253,  7253,  7253,  7253,
    7253,  5075,  7253, -1302,   536, -1302, -1302,   570,   573,  7253,
     742, -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302,
     243, -1302,   -90,   577, -1302,   583,   586,   587, -1302,   589,
   -1302, -1302,   701, -1302, -1302, -1302, -1302,   561, -1302, -1302,
     -73, -1302, -1302, -1302, -1302, -1302, -1302,  1535, -1302,   562,
   -1302, -1302, -1302, -1302, -1302, -1302,   334, -1302,   593, -1302,
   -1302,    18, -1302,   568,   571,   580, -1302,  8172, -1302,   734,
     620, -1302, -1302, -1302,  3089,  7713,  7713,  7713,  8270,  7713,
    7713,   572,   613,  7713,   434,  7713,   434,  7713,   434,  7809,
     614,  8368, -1302,  7253, -1302, -1302, -1302, -1302,   575, -1302,
   -1302, 10447,  7253, -1302,  7526,   607, -1302,   605,   -58, -1302,
     611,   514,   616,   608, -1302,   612,   617,  8403,   581,   753,
   -1302,   -78, -1302, -1302, -1302, 10955,   237, -1302,   590, -1302,
   -1302,   -12, -1302, -1302,  7253,  3089, -1302,   623,   670,  7915,
     787,  7713,  7253,  7253, 11614,  7253, 10955,   619, -1302, -1302,
    7253, -1302, -1302,   622,   655, 11614,  7253, -1302, -1302,  7253,
   -1302, -1302,  7253, -1302,  7713,  3089, -1302,  7915,   188,   188,
     609, -1302,   561, -1302, -1302, -1302,  7253,  7253,  7253,  7253,
    7253,  7253, -1302, 10955, -1302,   417,   237, 11053, -1302,   637,
     640,   188,   188,   -84,   188,   188,   237,   641, 11614,   641,
     158, -1302, -1302, 10955, -1302, -1302, -1302, -1302,  3485, -1302,
   -1302, -1302, -1302, -1302, -1302,   -65,   621,    63, -1302, 12064,
   12095,  3287,  3287,  3287,  3287,  3287,  3287,  3287,  3287,  7253,
    7253, -1302, -1302,  7253,  3287,  3287,  7253,  7253,  7253,   651,
    3287,  7253,   200,  7253,  7253,  7253,  7253,  7253,  7253,  3287,
    3287,  7253,  7253,  7253,  3287,  3287,  3287,  7253,  3287,  5273,
    7253,  7253,  7253,  7253,  7253,  7253,  7253,  7253,  7253,  7253,
     -31,  7253, -1302,  5471, -1302,  7253, -1302,   223, -1302,    40,
     817,   426, -1302, -1302,   664, -1302,  3089, -1302,  8025,   282,
     354,   643,   -35, -1302,   364,   376, -1302, -1302,   201,   378,
     474,   381,   474,   383,   474, -1302,   221, -1302,   265, -1302,
    7713,   625,   623, -1302, -1302, 10482, -1302, -1302,  7713, -1302,
   -1302,  7713, -1302, -1302, -1302,  7253,   666, -1302,   671, -1302,
    7713, -1302,  3089,  7713,  7713, -1302,    14,  7713,  3683,  5669,
    7713,  7713,   641, -1302,   623,  7253,  7253,  7253,  7253,  7253,
    7253,  7253,  7253,  7253,  7253,  7253,  7253,  7253,  7253,  7253,
    7253,  7253,  7253,   469,   896,   637, 11614,  8501, -1302, -1302,
    7713,  7713,  8599,  7713, -1302, -1302,  8634,  7713,   641,  7713,
    7713,   641,  7713,  1127, -1302,  7915, -1302,   621,  8732,  8830,
    8865,  8963,  9061,  9096,  7713, -1302,  7713, -1302,  7253,   380,
      15,  7253,  7253,   662, -1302,    19,  7253,   634,   630,   635,
     636,   242, -1302, -1302,   448,  7253,   561,   642,   -23,  3883,
   -1302,    44,   658,   639,   678, -1302, -1302,   645,   434, -1302,
     656,   644, -1302, -1302,   665,   649, -1302, -1302,    63,    63,
      63, -1302, -1302,  7804, -1302,  7804, -1302,  7804, -1302,  7804,
   -1302,  7804, -1302,  7804, -1302,  7804, -1302,  7804,   134,   134,
    1722, -1302,  7804, -1302,  7804,  1722,  1576,  1576,   652, -1302,
    7804,   169,   654, -1302, 10580,   -29,   -29,   562, 11614,   134,
     134, -1302,  7804, -1302,  7804, 11878,  1041, 11791, -1302,  7804,
   -1302,  7804, -1302,  7804, 11649, -1302,  7804, 12187, 11151,  1860,
   11902, 11926,  1722,  1722,   579,   579,   169,   169,   169,   319,
    7253,   657,   659,   325,  7253,   858, 10678, -1302,    73, 11186,
     688,   124,   475,   798,   690, -1302, -1302,  8134, -1302, -1302,
   -1302, -1302,  7713, -1302, -1302,   704, -1302, -1302,   681, -1302,
     683, -1302,   684,  7809, -1302,   614, -1302,   266,   514, -1302,
   -1302,   514,   514,  9194, -1302,   833,   -48, -1302,  7915,  1373,
    1383,  7253,   384, -1302,    84,   668,   714,  9292,   386,  1392,
     673, 11614, 11614, 11614, 11614, 11614, 11614, 11614, 11614, 11614,
   11614, 11614, 11614, 11614, 11614, 11614, 11614, 11614, 11614, -1302,
    7713,   676,   677, -1302,  7253,  1814,  1827, -1302,  2024, -1302,
    3804,   680,  3824,  4672,   682,  4816,   621,   434, -1302, -1302,
   -1302, -1302, -1302,   679,   270,   272, 11284,  7253, 11614, -1302,
   -1302,  2297,  7253, -1302, -1302,  7253,  7253,  7713,   434,   469,
   -1302, -1302,  7253, -1302,  1677,   621,  7253,  7253,  7253,   152,
    7451, -1302,   339, -1302, -1302,    90,   -46,   474, -1302,  7253,
   -1302,  7253, -1302, -1302, -1302,  5867,  7253, -1302,   703,   685,
   -1302, -1302,  7253,   686, -1302, 10713,  7253,  6065,   689, -1302,
   10811, -1302, -1302, -1302, -1302, -1302,   720, -1302, -1302,   254,
   -1302,    32, -1302, -1302, -1302, -1302, -1302,   514, -1302, -1302,
   -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302,
   -1302, -1302,  7713,   691, -1302,   715,  7253, -1302, -1302,   269,
   -1302, -1302, -1302,   273, -1302, -1302, -1302, -1302,  4872,  7253,
   -1302, 11614, -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302,
   -1302,   692,   474,  6263, -1302, -1302, -1302,   360, -1302,   365,
    9327,  9425,  5016,   474, -1302, 10955, -1302,   722,   434,  9523,
   11614, 11614,   693,  3883,   696,     5,   726,    80,   737,   741,
     744,   746, -1302,   164,   -91,  7713,  9558,  7713,  9656, -1302,
     108,  9754, -1302,  7253, 11747,  7253, -1302,  9789, -1302,   109,
    7253, -1302, -1302, -1302,   874,    32, -1302, -1302,   475, -1302,
   -1302, -1302,   514,  7253, -1302, 11614,   706,   707, -1302, -1302,
     748,  7253, -1302,   708,   709,   637,  7253,  7253,  7253,   710,
   -1302,   713,   716,  6461,   -91, -1302,   127,  7253,   717,  7253,
    7253, -1302, -1302, -1302,   732,   448,  2891, -1302,   474,    33,
   -1302,   174,  7253,  7253,  7253, -1302,  7713,   -15, -1302, -1302,
   -1302, -1302,   -46,   719,  6659, -1302, -1302,  7448, -1302,   391,
   -1302, -1302, -1302,  7713,  9887,  9985, -1302, -1302, 10020, -1302,
   -1302,   874,   237,   718,   352,   352,   733, 10118,  7253,  7253,
     721,  1722,  1722,  1722,  7253,   352,   352, -1302, 10216, -1302,
   -1302,  1722,  7253, -1302,   755, 11382,   137, -1302, -1302,  6857,
    7055, -1302, -1302, -1302, -1302, -1302, 11614,   469,  7253,  7253,
     723, 10251, 11614, 11614,   697,  7253, -1302, -1302,  7253, 11649,
   -1302, -1302,   392, -1302, -1302, -1302, 12218, -1302, -1302, -1302,
     104,   724,   725,  7253, -1302,   641,   637, -1302,   641,   729,
     730, -1302,   731,   883,   766,   736, -1302,   137,  7253,  7253,
   11614, -1302, 11614, 11614, -1302,   130,  7253, 11418, 11747, -1302,
   -1302, -1302,   768, 12218, -1302, -1302, 10349,   739,   740,   745,
   -1302, -1302, -1302,  7253, -1302, -1302, -1302, 11614,  7253,  7253,
   11516, -1302, -1302, -1302,   469, -1302, -1302, -1302, -1302, 11614,
   11614, 11614, -1302, -1302
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   118,     1,   294,     0,     0,     0,   596,   295,     0,
     588,   588,   588,    16,     0,     0,    15,     3,     0,    10,
       9,     8,     0,     7,   576,     6,    11,     5,     4,    13,
      12,    14,    92,    93,    91,   100,   102,    37,    53,    50,
      51,    39,    40,    41,     0,    42,    48,    38,   588,   588,
      22,    21,   576,   590,   589,   755,   745,   750,     0,   263,
      35,   105,   106,     0,     0,     0,   107,   109,   116,     0,
     104,    17,   614,   613,   211,   598,   615,   577,   578,     0,
       0,     0,     0,    43,     0,    49,     0,     0,    46,     0,
       0,   588,     0,    18,     0,     0,     0,   265,     0,     0,
     115,   110,     0,     0,     0,     0,     0,     0,   119,   213,
     212,   215,   210,   600,   599,     0,   617,   616,   620,   580,
     579,   582,    98,    99,    96,    97,    95,     0,     0,    94,
     103,    54,    52,    48,    45,    44,   591,   593,   595,     0,
     597,    19,    20,    23,   756,   746,   751,   264,    33,    36,
     114,     0,   111,   112,   113,   117,     0,   601,     0,   610,
     573,   512,    24,    25,     0,    87,    88,    85,    86,    84,
      83,    89,     0,    47,     0,   594,     0,     0,     0,    34,
     108,     0,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,     0,     0,   125,
     120,     0,   602,     0,   611,     0,   621,   574,     0,     0,
      26,    27,    28,     0,   101,     0,   757,   747,   752,   179,
     180,   177,   128,   129,   131,   130,   132,   133,   134,   135,
     161,   162,   159,   160,   152,   163,   164,   153,   150,   151,
     178,   172,     0,   176,   165,   166,   167,   168,   139,   140,
     141,   136,   137,   138,   149,     0,   155,   156,   154,   147,
     148,   143,   142,   144,   145,   146,   127,   126,   171,     0,
     157,   158,   512,   123,   242,   216,   584,   655,   658,   656,
     659,   657,   660,     0,   608,   618,   581,   512,     0,     0,
     563,   561,   583,    90,     0,     0,     0,     0,     0,     0,
     628,   648,   629,   662,   630,   634,   635,   636,   637,   654,
     641,   642,   643,   644,   645,   646,   647,   649,   650,   651,
     652,   715,   633,   640,   653,   722,   729,   631,   638,   632,
     639,     0,     0,     0,     0,   661,   677,   680,   678,   679,
     742,   592,   667,   541,   547,   181,   182,   175,   170,   183,
     173,   169,     0,   121,   293,   535,     0,     0,   214,     0,
     603,   605,     0,   612,   525,   622,     0,     0,     0,     0,
       0,     0,   562,     0,   683,   706,   709,     0,   712,   702,
       0,   670,   716,   723,   730,   736,   739,     0,     0,   692,
     697,   691,     0,   705,   701,   694,     0,     0,   696,   681,
       0,   758,   748,   753,   184,   174,     0,   291,   292,     0,
     512,   122,   124,   244,     0,     0,     0,    63,    64,    76,
     418,   419,     0,     0,     0,     0,   279,   412,   277,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   278,     0,
       0,     0,     0,     0,     0,     0,   654,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   501,   340,   342,   341,   343,   344,   345,   346,
       0,     0,    29,     0,   217,   221,     0,     0,     0,     0,
       0,     0,     0,   324,   325,   416,   415,     0,     0,     0,
       0,   237,   232,   229,   228,   230,   231,   262,   243,   223,
     495,   222,   413,     0,   486,    71,    72,    69,   235,    70,
     236,   238,   297,   227,   485,   484,   483,   118,   489,   414,
       0,   224,   488,   487,   459,   420,   460,   347,   421,     0,
     417,   760,   764,   761,   762,   763,     0,   585,     0,   584,
     609,   526,   575,     0,     0,     0,   495,     0,   565,   566,
       0,   559,   560,   558,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   120,     0,   120,     0,   120,     0,
       0,     0,   688,   246,   699,   700,   693,   695,     0,   698,
     682,     0,     0,   744,   743,     0,   668,     0,   263,   542,
       0,   537,     0,     0,   548,     0,     0,     0,     0,   623,
     533,   552,   536,   774,   777,     0,     0,   268,   272,   271,
     276,     0,   313,   794,     0,     0,   283,   280,     0,   319,
       0,     0,     0,     0,   266,     0,     0,     0,   304,   307,
       0,   241,   310,     0,     0,    57,     0,    78,   798,     0,
     783,   789,     0,   780,     0,     0,   288,   285,   448,   449,
     325,   335,   118,   261,   259,   260,     0,     0,     0,     0,
       0,     0,   252,     0,   255,     0,     0,   787,   801,   809,
       0,   425,   424,   461,   423,   422,     0,   809,   319,   809,
     326,   239,   240,     0,    74,   338,   220,   493,     0,   226,
     233,   234,   282,   287,   296,     0,   333,     0,   225,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   450,   451,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     404,     0,   219,     0,   586,     0,   604,   606,   619,     0,
       0,   512,   513,   564,     0,   568,     0,   572,   347,     0,
       0,     0,   673,   686,     0,     0,   663,   665,     0,     0,
     123,     0,   123,     0,   123,   539,     0,   545,     0,   664,
       0,     0,   247,   690,   675,     0,   669,   759,     0,   543,
     749,     0,   549,   754,   534,     0,     0,   551,     0,   550,
       0,   553,     0,     0,     0,    79,     0,     0,     0,     0,
       0,     0,   809,   284,   281,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   809,   267,     0,    80,    81,
       0,     0,     0,     0,    55,    56,     0,     0,   809,     0,
       0,   809,     0,     0,   289,   286,   326,   333,     0,     0,
       0,     0,     0,     0,     0,   245,     0,    32,     0,    30,
       0,     0,   810,     0,   242,     0,   810,     0,     0,     0,
       0,   394,   391,   393,    60,     0,   118,     0,   263,     0,
     407,     0,   773,     0,     0,   303,   302,     0,   120,   258,
       0,     0,   472,   471,     0,     0,   473,   477,     0,     0,
       0,   369,   378,   357,   379,   358,   381,   360,   380,   359,
     382,   361,   372,   351,   373,   352,   374,   353,   426,   427,
     439,   383,   362,   384,   363,   440,   437,   438,     0,   371,
     349,   466,     0,   457,     0,   490,   491,   492,   350,   428,
     429,   385,   364,   386,   365,   444,   445,   446,   375,   354,
     376,   355,   377,   356,   447,   370,   348,     0,     0,   442,
     443,   441,   435,   436,   431,   430,   432,   433,   434,     0,
       0,     0,   400,     0,     0,     0,     0,   410,     0,     0,
       0,     0,   519,   522,     0,   567,   570,   347,   571,   684,
     707,   710,     0,   713,   703,     0,   671,   717,     0,   724,
       0,   731,     0,     0,   737,     0,   740,     0,   250,   687,
     676,   538,   544,     0,   625,   626,   554,   557,   556,     0,
       0,     0,     0,   273,     0,     0,     0,     0,     0,     0,
       0,   320,   357,   358,   360,   359,   361,   351,   352,   353,
     362,   363,   349,   364,   365,   354,   355,   356,   348,   290,
       0,     0,     0,   462,     0,     0,     0,   463,     0,   494,
       0,     0,     0,     0,     0,     0,   333,   120,   496,   497,
     498,   499,   500,     0,     0,     0,     0,     0,   786,   802,
     803,     0,     0,   792,   452,     0,     0,     0,   120,     0,
      58,    59,     0,    73,    65,   333,     0,     0,     0,     0,
       0,   409,     0,   408,   298,     0,     0,   123,   468,     0,
     474,     0,   368,   366,   367,     0,     0,   455,     0,     0,
     478,   482,     0,     0,   458,     0,     0,     0,     0,   401,
       0,   405,   453,   411,   587,   607,   119,   520,   521,   522,
     523,   514,   527,   569,   685,   708,   711,   674,   714,   704,
     666,   672,   718,   720,   725,   727,   732,   734,   540,   738,
     546,   741,     0,     0,   624,     0,     0,   775,   778,     0,
     269,   274,   275,     0,   316,   314,   795,   793,     0,     0,
     804,    82,   305,   308,   311,   799,   797,   784,   790,   788,
     781,     0,   123,     0,   253,   256,    31,     0,   218,     0,
       0,     0,     0,   123,    61,     0,    66,     0,   120,     0,
     766,   765,     0,     0,     0,     0,     0,   561,     0,     0,
       0,     0,   331,     0,     0,     0,     0,     0,     0,   389,
       0,     0,   479,     0,   467,     0,   456,     0,   402,     0,
       0,   454,   406,   532,   517,   514,   515,   516,   519,   721,
     728,   735,   251,   246,   627,   555,     0,     0,    77,   270,
       0,     0,   315,     0,     0,   809,     0,     0,     0,     0,
     785,     0,     0,     0,     0,   502,     0,     0,   248,     0,
       0,   392,   511,   395,     0,    60,     0,    75,   123,     0,
     387,     0,     0,     0,     0,   299,     0,     0,   329,   330,
     328,   327,     0,     0,     0,   321,   337,     0,   510,     0,
     508,   390,   505,     0,     0,     0,   504,   403,     0,   518,
     528,   517,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   306,   309,   312,     0,     0,     0,   464,     0,   336,
     503,   254,   246,   257,   807,   807,     0,   398,    62,   279,
       0,    67,    71,    72,    69,    70,    68,     0,     0,     0,
       0,     0,   768,   767,     0,     0,   332,   334,     0,   322,
     469,   475,     0,   509,   507,   506,     0,   530,   524,   689,
       0,     0,     0,     0,   317,   809,   809,   805,   809,     0,
       0,   465,     0,     0,     0,     0,   396,     0,     0,     0,
     285,   339,   770,   769,   388,     0,     0,     0,   323,   470,
     476,   480,     0,     0,   776,   779,     0,     0,     0,     0,
     791,   782,   249,     0,   812,   811,   399,   286,     0,     0,
       0,   301,   481,   529,     0,   318,   796,   806,   800,   808,
     772,   771,   300,   531
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1302, -1302, -1302, -1302, -1302, -1302,   271,   866, -1302, -1302,
   -1302,   950, -1302, -1302, -1302,   526,   909, -1302,   859, -1302,
   -1302,   911, -1302, -1302, -1302,  -321, -1302, -1302, -1302,  -320,
   -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302,   774,
   -1302, -1302,   918,   -79, -1302, -1302,   372,   230,  -504,  -539,
    -775, -1302, -1302, -1302, -1272, -1302, -1302,  -208,  -347, -1302,
     106, -1302, -1256, -1302,  -282,  -216, -1302, -1302, -1302, -1302,
    -392,   -14, -1302, -1302, -1302, -1302, -1302,  -315,  -313,  -312,
   -1302,  -311, -1302, -1302, -1302,  1006, -1302, -1302, -1302, -1302,
   -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302,
   -1302,  -422,  -295,   560,  -322, -1302,  -856,  -424, -1302,   502,
   -1302, -1302,  -363,  2284, -1302, -1302, -1302, -1301, -1302, -1302,
   -1302, -1302, -1302, -1302, -1302, -1302, -1302,   633, -1302, -1302,
   -1302, -1302, -1302, -1302, -1302,  -151,  -262,  -337,  -263,  -147,
   -1302, -1302, -1302, -1302, -1302,   596, -1302,  -567, -1302, -1302,
    -571, -1302, -1302,  -606,  -334,  -556,  -221, -1302,  -356, -1302,
   -1302,   976, -1302, -1302, -1302,   488,   598,    74, -1302, -1302,
   -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302,
   -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302,  -561,  -197,
   -1302,   599, -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302,
    -379, -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302,
   -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302,
   -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302,  -153, -1302,
   -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302, -1302,
   -1302, -1302, -1302, -1302,   600,  -891,  -820, -1302, -1302, -1302,
   -1302, -1302, -1302, -1302, -1302, -1302,  -876, -1302, -1302, -1302,
   -1302, -1302, -1302, -1302, -1302,  -625, -1302,  -342,  -676, -1302
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    16,   143,    52,    17,   164,   170,   675,   495,
     149,   496,    99,    19,    20,    45,    46,    47,    88,    21,
      39,    40,   497,   498,  1122,  1123,   499,  1237,  1316,   500,
     501,   905,   502,   616,   503,   504,   505,   506,  1084,   171,
     172,    35,    36,    37,    66,    67,    68,    69,    22,   283,
     368,   210,    23,   111,   211,   112,   156,   661,   931,   508,
     369,   509,   801,  1373,  1037,   556,   884,  1307,   886,  1308,
     511,   512,   513,   618,   827,  1289,   514,   515,   516,   517,
     518,   519,   520,   521,   419,   522,   705,  1135,   915,   523,
     524,   860,  1296,   861,  1297,   863,  1298,   525,   830,  1292,
     526,   687,  1336,   527,  1252,  1253,   918,   663,   528,   762,
     906,   529,   628,   932,   531,   532,   533,   903,   534,  1117,
    1376,  1118,  1427,   535,  1005,  1272,   536,   688,  1255,  1439,
    1257,  1440,  1343,  1462,   538,   364,  1278,  1350,  1169,  1171,
    1014,   551,   771,  1406,  1443,   365,   366,   600,   796,   412,
     605,   798,   413,  1128,   610,   564,   383,   301,   302,   217,
     297,    78,   121,    25,   161,   370,    89,    90,   174,    91,
      26,    49,   115,   158,    27,   286,   548,   549,  1010,   373,
     215,   216,    76,   118,   375,    28,   159,   295,   611,   539,
     293,   347,   348,   788,   411,   349,   572,  1181,   781,   409,
     350,   565,  1174,   800,   570,  1179,   566,  1175,   567,  1176,
     569,  1178,   573,  1182,   574,  1279,   575,  1184,   576,  1280,
     577,  1186,   578,  1281,   579,  1189,   580,  1191,   601,    29,
      95,   177,   353,   602,    30,    96,   178,   354,   606,    31,
      94,   176,   352,   597,   540,   912,   913,   541,   823,  1286,
     824,  1287,   872,  1302,   869,  1300,   678,   542,   870,  1301,
     543,   831,  1293,   867,  1299,   679,   544,  1424,   893,   545
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      60,    70,   560,   285,   776,   822,   530,   855,  1055,   797,
     219,   897,   795,   898,   627,  1028,  1109,  1030,  1129,  1032,
     552,  1097,   507,   706,   151,  -118,    53,  1353,   346,   584,
       2,   594,    54,  1051,  1107,   790,     3,   792,  1112,   794,
     664,   665,   732,   733,   626,    84,   817,   770,  1334,    70,
      70,    70,    32,    33,    50,   826,   707,   122,   123,     4,
     362,     5,   582,     6,   104,   105,   106,   586,   656,     7,
     689,    38,   351,    61,   697,  1426,   817,  1276,  1248,     8,
      85,   583,    98,   284,   299,     9,   424,   425,  1249,   507,
      70,    70,    70,    70,   729,   999,  1000,   732,   733,   300,
      51,   817,    62,   708,   818,   819,   435,   820,    10,   561,
     821,  1127,   440,   507,   218,   890,  1422,    98,   698,   562,
     417,   760,   761,    92,  1001,   895,  1456,   808,    58,   817,
      11,    12,   150,  1002,  1442,   819,  1277,  1250,   410,  1323,
     623,  1022,  1251,   914,   107,   138,   376,   284,    48,   454,
     455,   410,    59,   510,   709,   710,  1060,   817,   877,   624,
     819,   802,    80,   418,    80,   139,   420,  1388,   563,   108,
     346,  1464,   777,    63,  1003,  1004,   760,   761,   362,  1082,
      58,   646,   457,   458,   346,   585,    71,   553,   819,   709,
     710,   180,  1091,   768,   223,  1094,   818,   818,    34,   421,
     124,   818,   832,   834,    59,   125,    13,   126,   709,   710,
     127,   345,    77,   346,   422,   346,   819,   507,    86,   377,
     510,   224,    58,   587,   769,    14,  1130,   868,   817,    87,
     871,   630,   218,   833,    79,   363,    15,   284,  1127,    64,
    1221,   588,   165,   166,   510,   647,    59,   589,    65,   721,
     722,   128,   854,  1131,   817,   835,   483,   729,    80,   731,
     732,   733,   734,   874,  1458,  1326,   835,   735,   382,  1238,
     631,   617,  1246,   493,   660,   873,   911,    58,   507,    41,
      42,    43,  1163,   919,   721,   722,   899,   819,   491,   632,
     835,   835,   729,  1201,   731,   732,   733,   734,    58,  1247,
    1274,    59,   735,   721,   722,   648,   107,   899,   507,   835,
      44,   729,   900,   819,   732,   733,   734,  1341,  1347,   287,
    1170,   735,    59,   288,   649,   755,   756,   757,   758,   759,
     398,  1166,    72,    73,  1132,    74,  1370,    82,   651,   760,
     761,  1008,   289,   290,   291,   292,  1332,   901,   510,   916,
     212,   299,  1321,   345,    97,   962,  1132,   652,   550,  1242,
    -719,   213,  1254,    75,   963,  -719,   300,   345,   346,   346,
     346,  1333,   346,   346,   760,   761,   346,  1025,   346,  1137,
     346,  1390,   346,  -719,  1016,   167,   608,   546,   162,   163,
     168,  1026,   169,   760,   761,   127,   345,  1033,   345,   547,
    -726,   609,  1125,  -733,    53,  -726,  1054,   825,  -733,   510,
      54,  1034,   779,   780,   782,   109,   784,   785,   345,   595,
     789,   110,   791,  -726,   793,    81,  -733,  -397,   858,   507,
    1047,   596,  -397,  1374,   346,   100,   101,   102,    98,   510,
    1196,  1035,  1192,   284,  1049,  1050,  1192,  1304,  1192,  1290,
    -397,   835,  1059,   493,   660,  1036,  1193,   346,  1314,  1119,
    1224,   603,  1225,  1291,  1190,   885,  1188,   104,   410,   106,
    1120,  1121,  1019,   604,  1153,   507,   152,   153,   154,   155,
    1158,  1085,  1086,  1154,  1088,   904,   113,   103,  1090,  1159,
    1092,  1093,   114,  1095,  1244,   510,   510,   510,   510,   510,
     510,   510,   510,  1245,  1142,  1143,  1144,   907,   510,   510,
     764,    87,   923,   927,   510,   380,  1410,   765,   381,   116,
     119,   382,   133,   510,   510,   117,   120,   141,   510,   510,
     510,  1167,   510,   142,  1411,  1412,  1309,  1168,   967,   131,
     410,  1310,   835,  1387,  1020,  1419,  1420,   835,   220,   221,
     410,   345,   345,   345,  1023,   345,   345,   136,  1222,   345,
     510,   345,   410,   345,   410,   345,  1024,   410,  1027,   410,
     410,  1029,   410,  1031,  1200,   137,  1205,   410,   410,  1233,
     140,  1401,  1441,   144,  1295,   162,   163,   887,   888,   145,
     104,   105,   106,    41,    42,    43,   220,   221,   222,   709,
     710,   146,  1104,   346,  1105,   147,   510,   829,    55,    56,
      57,   346,   134,   135,   346,   148,   157,   345,   160,  1360,
    1013,    85,   175,   346,   179,   104,   346,   346,   214,  1199,
     346,   226,   218,   346,   346,   227,   225,   280,   281,  1324,
     345,   228,   282,   284,   294,  1079,   298,  1038,   296,   355,
     356,   357,   399,   359,   360,  1041,   371,   358,  1042,   367,
     379,   361,   372,   346,   346,   374,   346,  1046,   378,   414,
     346,   415,   346,   346,  1052,   346,   902,  1058,   384,   400,
     401,   385,   386,   388,   387,  1227,   389,   346,   391,   346,
    1229,   390,   392,   393,   721,   722,   394,   395,   396,  1318,
     410,  1208,   729,   397,   731,   732,   733,   734,   416,   558,
     559,   571,   735,  1389,   613,   637,   592,   614,   622,   621,
     635,   638,   639,  1260,   640,   641,   643,   642,   650,   399,
     653,  1038,   654,  1038,  1416,  1269,   680,   674,  1232,  1447,
    1448,   402,  1449,   666,   561,   403,   691,   690,   530,   692,
     667,   694,   668,   699,   562,    70,   400,   401,   669,   700,
     670,   671,   701,   702,   507,   703,   704,    15,   766,   672,
     763,  1395,   757,   758,   759,   772,   774,   787,   603,   646,
     807,   786,   803,   806,   760,   761,   345,   809,   812,   815,
    1151,   810,   813,   811,   345,   816,   775,   345,   828,   707,
     853,  1306,   859,   563,   404,   835,   345,   864,   405,   345,
     345,   406,   865,   345,   894,   958,   345,   345,   402,   892,
     876,   561,   403,   896,  1012,   346,   407,   917,  1015,  1459,
    1044,   562,   408,  1021,  1039,  1045,   346,  1110,  1436,  1114,
    1132,  1113,  1134,  1115,  1116,  1138,   345,   345,  1133,   345,
    1126,  1136,  1139,   345,  1140,   345,   345,  1141,   345,  1161,
    1145,   802,  1146,  1165,  1170,  1156,  1172,  1157,  1180,  1177,
     345,  1183,   345,  1185,  1187,  1195,  1337,  1202,  1203,  1284,
     563,   404,  1207,   346,  1209,   405,  1210,  1223,   406,  1216,
    1325,  1219,  1262,  1263,  1265,   510,  1273,  1270,  1317,  1283,
    1303,  1328,  1320,   407,  1322,  1329,  1349,   304,  1330,   408,
    1331,  1234,  1356,   305,  1354,  1355,  1358,  1359,  1364,   306,
     346,  1365,  1377,  1413,  1366,  1372,  1397,  1409,   399,   307,
    1417,  1423,  1434,  1444,  1445,  1453,  1415,   308,  1450,  1451,
    1452,  1454,  1418,  1455,  1463,   129,   889,  1394,  1466,  1467,
     802,    18,   309,    83,  1468,   400,   401,   627,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,  1288,   173,   132,  1378,   346,  1381,   303,   130,  1011,
    1111,  1382,   537,  1383,  1384,  1385,   834,    24,   345,  1369,
    1396,   557,   696,  1351,  1407,  1352,   612,   402,  1408,   345,
     568,   403,  1275,   662,   662,   662,  1327,  1315,    93,    58,
     581,   619,   620,  1425,     0,     0,     0,   767,     0,  1282,
     591,     0,   343,     0,     0,     0,  1335,     0,     0,   607,
       0,     0,     0,    59,     0,     0,     0,     0,   346,   615,
     346,   709,   710,     0,     0,     0,   345,     0,     0,   629,
     695,     0,  1080,     0,   634,     0,   636,     0,     0,     0,
     404,     0,     0,     0,   405,   645,  1081,   406,     0,     0,
       0,     0,     0,   657,   658,   659,  1335,   695,     0,   344,
       0,   593,   407,   345,  1339,     0,     0,     0,   408,     0,
       0,     0,     0,   673,     0,     0,   677,     0,     0,   681,
     682,   683,   684,   685,     0,     0,     0,     0,     0,   346,
       0,     0,   693,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   346,     0,     0,     0,
       0,     0,     0,     0,   719,   720,   721,   722,   723,   399,
       0,   726,   727,   728,   729,     0,   731,   732,   733,   734,
       0,     0,     0,     0,   735,     0,   737,   738,   345,  1431,
       0,     0,   741,     0,   743,     0,   400,   401,     0,   695,
    1402,     0,     0,     0,     0,     0,     0,   778,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   695,     0,     0,
       0,     0,     0,     0,     0,   805,     0,   750,   751,   752,
     753,   754,   755,   756,   757,   758,   759,     0,     0,     0,
       0,   345,     0,   345,     0,     0,   760,   761,   402,     0,
       0,     0,   403,     0,     0,     0,  1473,     0,   629,     0,
       0,     0,     0,     0,     0,   677,   856,   662,   857,     0,
       0,     0,     0,   862,     0,     0,     0,     0,     0,   866,
       0,     0,     0,     0,     0,     0,     0,     0,   875,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   878,
     879,   880,   881,   882,   883,     0,     0,     0,     0,     0,
       0,   404,   345,     0,     0,   405,     0,  1096,   406,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   345,
       0,     0,     0,   407,     0,     0,     0,     0,   695,   408,
       0,     0,     0,     0,   933,   935,   937,   939,   941,   943,
     945,   947,   948,   949,     0,     0,   950,   952,   954,   955,
     956,   957,   902,   960,   961,     0,   964,   965,   966,   968,
     969,   970,   972,   974,   975,   976,   977,   979,   981,   983,
     984,   986,   988,   989,   990,   991,   992,   993,   994,   995,
     996,   997,   998,     0,  1006,     0,     0,     0,  1009,     0,
       0,     0,     0,     0,     0,   399,     0,     0,     0,  1017,
       0,     0,     0,   902,     0,   399,     0,     0,     0,     0,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,     0,   400,   401,     0,   695,     0,     0,     0,     0,
       0,     0,   400,   401,     0,     0,     0,     0,  1043,     0,
       0,   400,   401,     0,     0,  1048,     0,     0,     0,     0,
       0,     0,  1057,     0,     0,     0,     0,     0,  1061,  1062,
    1063,  1064,  1065,  1066,  1067,  1068,  1069,  1070,  1071,  1072,
    1073,  1074,  1075,  1076,  1077,  1078,     0,     0,   662,   662,
     662,     0,     0,   695,   402,   695,     0,   695,   403,   695,
       0,   695,     0,   695,   402,   695,     0,   695,   403,     0,
       0,     0,   695,   402,   695,     0,     0,   403,     0,     0,
     695,  1106,     0,     0,  1108,   677,     0,     0,     0,  1061,
       0,     0,   695,     0,   695,     0,     0,     0,  1124,   695,
       0,   695,     0,   695,   -68,     0,   695,     0,     0,     0,
       0,     0,     0,     0,     0,   709,   710,   404,     0,     0,
       0,   405,     0,  1197,   406,     0,     0,   404,     0,     0,
       0,   405,     0,  1198,   406,     0,   404,   695,     0,   407,
     405,     0,  1206,   406,     0,   408,     0,     0,     0,   407,
       0,     0,     0,     0,     0,   408,   709,   710,   407,     0,
       0,     0,     0,     0,   408,     0,     0,     0,   695,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1155,     0,     0,     0,  1160,     0,     0,
     711,   712,   713,   714,   715,   716,   717,   718,   719,   720,
     721,   722,   723,   724,   725,   726,   727,   728,   729,   730,
     731,   732,   733,   734,     0,     0,     0,     0,   735,   736,
     737,   738,   739,   740,     0,     0,   741,   742,   743,   744,
     745,   746,   747,     0,     0,     0,     0,     0,  1236,   719,
     720,   721,   722,   723,     0,     0,   726,   709,   710,   729,
       0,   731,   732,   733,   734,     0,     0,     0,     0,   735,
       0,   737,   738,     0,     0,     0,     0,  1211,   748,   749,
       0,   750,   751,   752,   753,   754,   755,   756,   757,   758,
     759,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     760,   761,   709,   710,   537,   493,   660,     0,  1230,  1231,
       0,     0,     0,     0,     0,  1235,     0,     0,     0,  1239,
    1240,  1241,     0,  1061,     0,   753,   754,   755,   756,   757,
     758,   759,  1256,     0,  1258,     0,     0,     0,     0,  1261,
       0,   760,   761,     0,     0,  1264,     0,     0,     0,  1267,
     719,   720,   721,   722,   723,     0,     0,   726,   727,   728,
     729,     0,   731,   732,   733,   734,     0,     0,     0,     0,
     735,     0,   737,   738,     0,     0,     0,     0,   741,   742,
     743,     0,     0,     0,   747,     0,     0,     0,     0,  1285,
       0,     0,     0,     0,     0,   719,   720,   721,   722,     0,
       0,     0,   677,     0,     0,   729,   399,   731,   732,   733,
     734,     0,     0,     0,     0,   735,     0,   737,   738,   399,
       0,   749,     0,   750,   751,   752,   753,   754,   755,   756,
     757,   758,   759,   400,   401,     0,     0,     0,     0,     0,
     709,   710,   760,   761,     0,     0,   400,   401,     0,     0,
       0,     0,     0,     0,     0,     0,  1344,     0,  1345,     0,
       0,     0,     0,  1348,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   755,   756,   757,   758,   759,     0,     0,
       0,     0,     0,     0,  1357,     0,     0,   760,   761,  1361,
    1362,  1363,     0,     0,     0,   402,  1368,     0,     0,   403,
    1371,     0,   677,  1375,     0,     0,     0,     0,   402,  1386,
       0,     0,   403,     0,     0,  1391,  1392,  1393,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1399,     0,     0,
       0,     0,     0,   719,   720,   721,   722,   723,     0,     0,
     726,   727,   728,   729,     0,   731,   732,   733,   734,     0,
       0,     0,   677,   735,     0,   737,   738,     0,   404,     0,
       0,     0,   405,     0,  1212,   406,     0,     0,     0,     0,
       0,   404,     0,  1430,     0,   405,     0,  1213,   406,     0,
     407,  1432,  1433,     0,     0,     0,   408,     0,  1437,     0,
       0,  1438,     0,   407,     0,     0,     0,     0,     0,   408,
       0,     0,     0,     0,     0,     0,  1446,   751,   752,   753,
     754,   755,   756,   757,   758,   759,   399,     0,     0,     0,
       0,     0,  1457,     0,     0,   760,   761,     0,     0,  1460,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   400,   401,     0,  1469,     0,     0,     0,
     423,  1470,  1471,   424,   425,     3,     0,   426,   427,   428,
       0,   429,     0,   430,   431,   432,   433,   434,     0,     0,
       0,     0,     0,   435,   436,   437,   438,   439,     0,   440,
       0,     0,     0,     0,     0,     0,   441,   442,     0,     0,
     443,     0,   444,   445,     0,     0,   446,     0,     8,   447,
     448,     0,   449,   450,     0,   402,   451,   452,     0,   403,
       0,     0,     0,   453,     0,     0,   454,   455,   310,   311,
     312,     0,   314,   315,   316,   317,   318,   456,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,     0,
     332,   333,   334,     0,     0,   337,   338,   339,   340,   457,
     458,   459,   460,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   461,   462,     0,     0,   404,     0,
       0,     0,   405,     0,  1214,   406,     0,     0,     0,   463,
     464,   465,     0,     0,     0,     0,     0,     0,     0,    58,
     407,     0,     0,     0,     0,     0,   408,   466,   467,   468,
     469,   470,     0,   471,     0,   472,   473,   474,   475,   476,
     477,   478,   479,    59,   480,   481,   482,     0,     0,     0,
       0,     0,     0,   483,   484,   485,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     486,   487,   488,     0,    14,     0,     0,   489,   490,     0,
       0,     0,     0,     0,     0,   491,     0,   492,   423,   493,
     494,   424,   425,     3,     0,   426,   427,   428,     0,   429,
       0,   430,   431,   432,   433,   434,     0,     0,     0,     0,
       0,   435,   436,   437,   438,   439,     0,   440,     0,     0,
       0,     0,     0,     0,   441,   442,     0,     0,   443,     0,
     444,   445,     0,     0,   446,     0,     8,   447,   448,     0,
     449,   450,     0,     0,   451,   452,     0,     0,     0,     0,
       0,   453,     0,     0,   454,   455,   310,   311,   312,     0,
     314,   315,   316,   317,   318,   456,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,     0,   332,   333,
     334,     0,     0,   337,   338,   339,   340,   457,   458,   459,
     460,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   461,   462,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,   464,   465,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   466,   467,   468,   469,   470,
       0,   471,     0,   472,   473,   474,   475,   476,   477,   478,
     479,    59,   480,   481,   482,     0,     0,     0,     0,     0,
       0,   483,  1228,   485,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   486,   487,
     488,     0,    14,     0,     0,   489,   490,     0,     0,   424,
     425,     0,     0,   491,     0,   492,     0,   493,   494,   430,
     431,   432,   433,   434,     0,     0,     0,     0,     0,   435,
       0,   437,     0,     0,     0,   440,     0,     0,     0,     0,
       0,     0,     0,   442,     0,     0,     0,     0,     0,   445,
       0,     0,   446,     0,     0,   447,     0,     0,     0,   450,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   554,
       0,     0,   454,   455,   310,   311,   312,     0,   314,   315,
     316,   317,   318,   456,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,     0,   332,   333,   334,     0,
       0,   337,   338,   339,   340,   457,   458,   459,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     461,   462,     0,     0,     0,     0,     0,     0,     0,   625,
       0,     0,     0,     0,     0,   463,   464,   465,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   466,   467,   468,   469,   470,     0,   471,
       0,   472,   473,   474,   475,   476,   477,   478,   479,    59,
     555,   481,   482,     0,     0,     0,     0,     0,     0,   483,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   486,   487,   488,     0,
      14,     0,     0,   489,   490,     0,     0,   424,   425,     0,
       0,   491,     0,   492,     0,   493,   494,   430,   431,   432,
     433,   434,     0,     0,     0,     0,     0,   435,     0,   437,
       0,     0,     0,   440,     0,     0,     0,     0,     0,     0,
       0,   442,     0,     0,     0,     0,     0,   445,     0,     0,
     446,     0,     0,   447,     0,     0,     0,   450,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   554,     0,     0,
     454,   455,   310,   311,   312,     0,   314,   315,   316,   317,
     318,   456,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,     0,   332,   333,   334,     0,     0,   337,
     338,   339,   340,   457,   458,   459,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   461,   462,
       0,     0,     0,     0,     0,     0,     0,   655,     0,     0,
       0,     0,     0,   463,   464,   465,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   466,   467,   468,   469,   470,     0,   471,     0,   472,
     473,   474,   475,   476,   477,   478,   479,    59,   555,   481,
     482,     0,     0,     0,     0,     0,     0,   483,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   486,   487,   488,     0,    14,     0,
       0,   489,   490,     0,     0,   424,   425,     0,     0,   491,
       0,   492,     0,   493,   494,   430,   431,   432,   433,   434,
       0,     0,     0,     0,     0,   435,  1379,   437,   438,     0,
       0,   440,     0,     0,     0,     0,     0,     0,     0,   442,
       0,     0,     0,     0,     0,   445,     0,     0,   446,     0,
       0,   447,   448,     0,     0,   450,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   554,     0,     0,   454,   455,
     310,   311,   312,     0,   314,   315,   316,   317,   318,   456,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,     0,   332,   333,   334,     0,     0,   337,   338,   339,
     340,   457,   458,   459,  1380,     0,   934,   936,   938,   940,
     942,   944,   946,     0,     0,     0,   461,   462,   951,   953,
       0,     0,     0,     0,   959,     0,     0,     0,     0,     0,
       0,     0,     0,   971,   973,     0,     0,     0,   978,   980,
     982,    58,   985,     0,     0,     0,     0,     0,     0,   466,
     467,   468,   469,   470,     0,   471,     0,   472,   473,   474,
     475,   476,   477,   478,   479,    59,   555,   481,   482,     0,
       0,     0,     0,     0,     0,   483,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   486,   487,   488,     0,    14,     0,     0,   489,
     490,     0,     0,   424,   425,     0,     0,   491,     0,   492,
       0,   493,   494,   430,   431,   432,   433,   434,     0,     0,
       0,     0,     0,   435,     0,   437,     0,     0,     0,   440,
       0,     0,     0,     0,     0,     0,     0,   442,     0,     0,
       0,     0,     0,   445,     0,     0,   446,     0,     0,   447,
       0,     0,     0,   450,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   554,     0,     0,   454,   455,   310,   311,
     312,     0,   314,   315,   316,   317,   318,   456,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,     0,
     332,   333,   334,     0,     0,   337,   338,   339,   340,   457,
     458,   459,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   461,   462,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   463,
     464,   465,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   466,   467,   468,
     469,   470,     0,   471,     0,   472,   473,   474,   475,   476,
     477,   478,   479,    59,   555,   481,   482,     0,     0,     0,
       0,     0,     0,   483,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     486,   487,   488,     0,    14,     0,     0,   489,   490,     0,
       0,   424,   425,     0,     0,   491,     0,   492,     0,   493,
     494,   430,   431,   432,   433,   434,     0,     0,     0,     0,
       0,   435,     0,   437,     0,     0,     0,   440,     0,     0,
       0,     0,     0,     0,     0,   442,     0,     0,     0,     0,
       0,   445,     0,     0,   446,     0,     0,   447,     0,     0,
       0,   450,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   554,     0,     0,   454,   455,   310,   311,   312,     0,
     314,   315,   316,   317,   318,   456,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,     0,   332,   333,
     334,     0,     0,   337,   338,   339,   340,   457,   458,   459,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   461,   462,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   928,   929,   930,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   466,   467,   468,   469,   470,
       0,   471,     0,   472,   473,   474,   475,   476,   477,   478,
     479,    59,   555,   481,   482,     0,     0,     0,     0,     0,
       0,   483,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   486,   487,
     488,     0,    14,     0,     0,   489,   490,     0,     0,   424,
     425,     0,     0,   491,     0,   492,     0,   493,   494,   430,
     431,   432,   433,   434,     0,     0,     0,     0,     0,   435,
       0,   437,     0,     0,     0,   440,     0,     0,     0,     0,
       0,     0,     0,   442,     0,     0,     0,     0,     0,   445,
       0,     0,   446,     0,     0,   447,     0,     0,     0,   450,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   554,
       0,     0,   454,   455,   310,   311,   312,     0,   314,   315,
     316,   317,   318,   456,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,     0,   332,   333,   334,     0,
       0,   337,   338,   339,   340,   457,   458,   459,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     461,   462,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   466,   467,   468,   469,   470,     0,   471,
     907,   472,   473,   474,   475,   476,   477,   478,   479,   908,
     555,   481,   482,     0,     0,     0,     0,     0,     0,   483,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   486,   487,   488,     0,
      14,     0,     0,   489,   490,     0,     0,   424,   425,     0,
       0,   909,     0,   492,   910,   493,   494,   430,   431,   432,
     433,   434,     0,     0,     0,     0,     0,   435,     0,   437,
       0,     0,     0,   440,     0,     0,     0,     0,     0,     0,
       0,   442,     0,     0,     0,     0,     0,   445,     0,     0,
     446,     0,     0,   447,     0,     0,     0,   450,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   554,     0,     0,
     454,   455,   310,   311,   312,     0,   314,   315,   316,   317,
     318,   456,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,     0,   332,   333,   334,     0,     0,   337,
     338,   339,   340,   457,   458,   459,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   461,   462,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   466,   467,   468,   469,   470,   399,   471,   907,   472,
     473,   474,   475,   476,   477,   478,   479,   908,   555,   481,
     482,     0,     0,     0,     0,     0,   399,   483,     0,     0,
       0,     0,     0,   400,   401,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   486,   487,   488,     0,    14,     0,
       0,   489,   490,   400,   401,     0,     0,   424,   425,   491,
       0,   492,  1053,   493,   494,   686,     0,   430,   431,   432,
     433,   434,     0,     0,     0,     0,     0,   435,     0,   437,
       0,     0,     0,   440,     0,     0,     0,     0,     0,     0,
       0,   442,     0,     0,     0,   402,     0,   445,     0,   403,
     446,     0,     0,   447,     0,     0,     0,   450,     0,     0,
       0,     0,     0,     0,     0,   402,     0,   554,     0,   403,
     454,   455,   310,   311,   312,     0,   314,   315,   316,   317,
     318,   456,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,     0,   332,   333,   334,     0,     0,   337,
     338,   339,   340,   457,   458,   459,     0,     0,   404,     0,
       0,     0,   405,     0,  1215,   406,     0,     0,   461,   462,
       0,     0,     0,     0,     0,     0,     0,     0,   404,     0,
     407,     0,   405,     0,  1217,   406,   408,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
     407,   466,   467,   468,   469,   470,   408,   471,   907,   472,
     473,   474,   475,   476,   477,   478,   479,   908,   555,   481,
     482,     0,     0,     0,     0,     0,     0,   483,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   486,   487,   488,     0,    14,     0,
       0,   489,   490,     0,     0,   424,   425,     0,     0,   491,
       0,   492,     0,   493,   494,   430,   431,   432,   433,   434,
       0,     0,     0,     0,     0,   435,     0,   437,     0,     0,
       0,   440,     0,     0,     0,     0,     0,     0,     0,   442,
       0,     0,     0,     0,     0,   445,     0,     0,   446,     0,
       0,   447,     0,     0,     0,   450,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   554,     0,     0,   454,   455,
     310,   311,   312,     0,   314,   315,   316,   317,   318,   456,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,     0,   332,   333,   334,     0,     0,   337,   338,   339,
     340,   457,   458,   459,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   461,   462,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   466,
     467,   468,   469,   470,     0,   471,     0,   472,   473,   474,
     475,   476,   477,   478,   479,    59,   555,   481,   482,     0,
       0,     0,     0,     0,     0,   483,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   486,   487,   488,     0,    14,     0,     0,   489,
     490,     0,     0,   424,   425,     0,     0,   491,   590,   492,
       0,   493,   494,   430,   431,   432,   433,   434,     0,     0,
       0,     0,     0,   435,     0,   437,     0,     0,     0,   440,
       0,     0,     0,     0,     0,     0,     0,   442,     0,     0,
       0,     0,     0,   445,     0,     0,   446,     0,     0,   447,
       0,     0,     0,   450,     0,     0,     0,     0,     0,   633,
       0,     0,     0,   554,     0,     0,   454,   455,   310,   311,
     312,     0,   314,   315,   316,   317,   318,   456,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,     0,
     332,   333,   334,     0,     0,   337,   338,   339,   340,   457,
     458,   459,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   461,   462,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   466,   467,   468,
     469,   470,     0,   471,     0,   472,   473,   474,   475,   476,
     477,   478,   479,    59,   555,   481,   482,     0,     0,     0,
       0,     0,     0,   483,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     486,   487,   488,     0,    14,     0,     0,   489,   490,     0,
       0,   424,   425,     0,     0,   491,     0,   492,     0,   493,
     494,   430,   431,   432,   433,   434,     0,     0,     0,     0,
       0,   435,     0,   437,     0,     0,     0,   440,     0,     0,
       0,     0,     0,     0,     0,   442,     0,     0,     0,     0,
       0,   445,     0,     0,   446,     0,     0,   447,     0,     0,
       0,   450,     0,     0,   644,     0,     0,     0,     0,     0,
       0,   554,     0,     0,   454,   455,   310,   311,   312,     0,
     314,   315,   316,   317,   318,   456,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,     0,   332,   333,
     334,     0,     0,   337,   338,   339,   340,   457,   458,   459,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   461,   462,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   466,   467,   468,   469,   470,
       0,   471,     0,   472,   473,   474,   475,   476,   477,   478,
     479,    59,   555,   481,   482,     0,     0,     0,     0,     0,
       0,   483,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   486,   487,
     488,     0,    14,     0,     0,   489,   490,     0,     0,   424,
     425,     0,     0,   491,     0,   492,     0,   493,   494,   430,
     431,   432,   433,   434,     0,     0,     0,     0,     0,   435,
       0,   437,     0,     0,   399,   440,     0,     0,     0,     0,
       0,     0,     0,   442,     0,     0,     0,     0,     0,   445,
       0,     0,   446,     0,     0,   447,     0,     0,     0,   450,
       0,   400,   401,     0,     0,     0,     0,     0,     0,   554,
       0,     0,   454,   455,   310,   311,   312,     0,   314,   315,
     316,   317,   318,   456,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,     0,   332,   333,   334,     0,
       0,   337,   338,   339,   340,   457,   458,   459,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     461,   462,     0,   402,     0,     0,     0,   403,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   466,   467,   468,   469,   470,     0,   471,
       0,   472,   473,   474,   475,   476,   477,   478,   479,    59,
     555,   481,   482,     0,     0,     0,     0,     0,   399,   483,
       0,     0,     0,     0,     0,     0,   404,     0,     0,     0,
     405,     0,  1218,   406,   672,     0,   486,   487,   488,     0,
      14,     0,     0,   489,   490,   400,   401,     0,   407,   424,
     425,   491,     0,   492,   408,   493,   494,   676,     0,   430,
     431,   432,   433,   434,     0,     0,     0,     0,     0,   435,
       0,   437,     0,     0,   399,   440,     0,     0,     0,     0,
       0,     0,     0,   442,     0,     0,     0,     0,     0,   445,
       0,     0,   446,     0,     0,   447,     0,     0,     0,   450,
       0,   400,   401,     0,     0,     0,     0,   402,     0,   554,
       0,   403,   454,   455,   310,   311,   312,     0,   314,   315,
     316,   317,   318,   456,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,     0,   332,   333,   334,     0,
       0,   337,   338,   339,   340,   457,   458,   459,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     461,   462,     0,   402,     0,     0,     0,   403,     0,     0,
     404,     0,     0,     0,   405,     0,  1220,   406,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,   407,   466,   467,   468,   469,   470,   408,   471,
       0,   472,   473,   474,   475,   476,   477,   478,   479,    59,
     555,   481,   482,     0,     0,     0,     0,     0,   399,   483,
       0,     0,     0,     0,     0,     0,   404,     0,     0,     0,
     405,     0,  1294,   406,     0,     0,   486,   487,   488,     0,
      14,     0,     0,   489,   490,   400,   401,     0,   407,   424,
     425,   491,     0,   492,   408,   493,   494,   686,     0,   430,
     431,   432,   433,   434,     0,     0,     0,     0,     0,   435,
       0,   437,     0,     0,     0,   440,     0,     0,     0,     0,
       0,     0,     0,   442,     0,     0,     0,     0,     0,   445,
       0,     0,   446,     0,     0,   447,     0,     0,     0,   450,
       0,     0,     0,     0,     0,     0,     0,   402,     0,   554,
       0,   403,   454,   455,   310,   311,   312,     0,   314,   315,
     316,   317,   318,   456,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,     0,   332,   333,   334,     0,
       0,   337,   338,   339,   340,   457,   458,   459,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     461,   462,     0,     0,     0,     0,     0,     0,     0,     0,
     404,     0,     0,     0,   405,     0,  1313,   406,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,   407,   466,   467,   468,   469,   470,   408,   471,
       0,   472,   473,   474,   475,   476,   477,   478,   479,    59,
     555,   481,   482,     0,     0,     0,     0,     0,     0,   483,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   486,   487,   488,     0,
      14,     0,     0,   489,   490,     0,     0,   424,   425,     0,
       0,   491,     0,   492,     0,   493,   494,   430,   431,   432,
     433,   434,     0,     0,   987,     0,     0,   435,     0,   437,
       0,     0,     0,   440,     0,     0,     0,     0,     0,     0,
       0,   442,     0,     0,     0,     0,     0,   445,     0,     0,
     446,     0,     0,   447,     0,     0,     0,   450,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   554,     0,     0,
     454,   455,   310,   311,   312,     0,   314,   315,   316,   317,
     318,   456,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,     0,   332,   333,   334,     0,     0,   337,
     338,   339,   340,   457,   458,   459,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   461,   462,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   466,   467,   468,   469,   470,     0,   471,     0,   472,
     473,   474,   475,   476,   477,   478,   479,    59,   555,   481,
     482,     0,     0,     0,     0,     0,     0,   483,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   486,   487,   488,     0,    14,     0,
       0,   489,   490,     0,     0,   424,   425,     0,     0,   491,
       0,   492,     0,   493,   494,   430,   431,   432,   433,   434,
       0,     0,     0,     0,     0,   435,     0,   437,     0,     0,
       0,   440,     0,     0,     0,     0,     0,     0,     0,   442,
       0,     0,     0,     0,     0,   445,     0,     0,   446,     0,
       0,   447,     0,     0,     0,   450,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   554,     0,     0,   454,   455,
     310,   311,   312,     0,   314,   315,   316,   317,   318,   456,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,     0,   332,   333,   334,     0,     0,   337,   338,   339,
     340,   457,   458,   459,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   461,   462,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   466,
     467,   468,   469,   470,     0,   471,     0,   472,   473,   474,
     475,   476,   477,   478,   479,    59,   555,   481,   482,     0,
       0,     0,     0,     0,     0,   483,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   486,   487,   488,     0,    14,     0,     0,   489,
     490,     0,     0,   424,   425,     0,     0,   491,     0,   492,
    1007,   493,   494,   430,   431,   432,   433,   434,     0,     0,
       0,     0,     0,   435,     0,   437,     0,     0,     0,   440,
       0,     0,     0,     0,     0,     0,     0,   442,     0,     0,
       0,     0,     0,   445,     0,     0,   446,     0,     0,   447,
       0,     0,     0,   450,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   554,     0,     0,   454,   455,   310,   311,
     312,     0,   314,   315,   316,   317,   318,   456,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,     0,
     332,   333,   334,     0,     0,   337,   338,   339,   340,   457,
     458,   459,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   461,   462,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   466,   467,   468,
     469,   470,     0,   471,     0,   472,   473,   474,   475,   476,
     477,   478,   479,    59,   555,   481,   482,     0,     0,     0,
       0,     0,     0,   483,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1056,     0,
     486,   487,   488,     0,    14,     0,     0,   489,   490,     0,
       0,   424,   425,     0,     0,   491,     0,   492,     0,   493,
     494,   430,   431,   432,   433,   434,     0,     0,     0,     0,
       0,   435,     0,   437,     0,     0,     0,   440,     0,     0,
       0,     0,     0,     0,     0,   442,     0,     0,     0,     0,
       0,   445,     0,     0,   446,     0,     0,   447,     0,     0,
       0,   450,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   554,     0,     0,   454,   455,   310,   311,   312,     0,
     314,   315,   316,   317,   318,   456,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,     0,   332,   333,
     334,     0,     0,   337,   338,   339,   340,   457,   458,   459,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   461,   462,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   466,   467,   468,   469,   470,
       0,   471,     0,   472,   473,   474,   475,   476,   477,   478,
     479,    59,   555,   481,   482,     0,     0,     0,     0,     0,
       0,   483,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   486,   487,
     488,     0,    14,     0,     0,   489,   490,     0,     0,   424,
     425,     0,     0,   491,     0,   492,  1259,   493,   494,   430,
     431,   432,   433,   434,     0,     0,     0,     0,     0,   435,
       0,   437,     0,     0,     0,   440,     0,     0,     0,     0,
       0,     0,     0,   442,     0,     0,     0,     0,     0,   445,
       0,     0,   446,     0,     0,   447,     0,     0,     0,   450,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   554,
       0,     0,   454,   455,   310,   311,   312,     0,   314,   315,
     316,   317,   318,   456,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,     0,   332,   333,   334,     0,
       0,   337,   338,   339,   340,   457,   458,   459,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     461,   462,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   466,   467,   468,   469,   470,     0,   471,
       0,   472,   473,   474,   475,   476,   477,   478,   479,    59,
     555,   481,   482,     0,     0,     0,     0,     0,     0,   483,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   486,   487,   488,     0,
      14,     0,     0,   489,   490,     0,     0,   424,   425,     0,
       0,   491,     0,   492,  1268,   493,   494,   430,   431,   432,
     433,   434,     0,     0,     0,     0,     0,   435,     0,   437,
       0,     0,     0,   440,     0,     0,     0,     0,     0,     0,
       0,   442,     0,     0,     0,     0,     0,   445,     0,     0,
     446,     0,     0,   447,     0,     0,     0,   450,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   554,     0,     0,
     454,   455,   310,   311,   312,     0,   314,   315,   316,   317,
     318,   456,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,     0,   332,   333,   334,     0,     0,   337,
     338,   339,   340,   457,   458,   459,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   461,   462,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   466,   467,   468,   469,   470,     0,   471,     0,   472,
     473,   474,   475,   476,   477,   478,   479,    59,   555,   481,
     482,     0,     0,     0,     0,     0,     0,   483,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   486,   487,   488,     0,    14,     0,
       0,   489,   490,     0,     0,   424,   425,     0,     0,   491,
       0,   492,  1305,   493,   494,   430,   431,   432,   433,   434,
       0,     0,     0,     0,     0,   435,     0,   437,     0,     0,
       0,   440,     0,     0,     0,     0,     0,     0,     0,   442,
       0,     0,     0,     0,     0,   445,     0,     0,   446,     0,
       0,   447,     0,     0,     0,   450,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   554,     0,     0,   454,   455,
     310,   311,   312,     0,   314,   315,   316,   317,   318,   456,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,     0,   332,   333,   334,     0,     0,   337,   338,   339,
     340,   457,   458,   459,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   461,   462,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   466,
     467,   468,   469,   470,     0,   471,     0,   472,   473,   474,
     475,   476,   477,   478,   479,    59,   555,   481,   482,     0,
       0,     0,     0,     0,     0,   483,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   486,   487,   488,     0,    14,     0,     0,   489,
     490,     0,     0,   424,   425,     0,     0,   491,     0,   492,
    1367,   493,   494,   430,   431,   432,   433,   434,     0,     0,
       0,     0,     0,   435,     0,   437,     0,     0,     0,   440,
       0,     0,     0,     0,     0,     0,     0,   442,     0,     0,
       0,     0,     0,   445,     0,     0,   446,     0,     0,   447,
       0,     0,     0,   450,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   554,     0,     0,   454,   455,   310,   311,
     312,     0,   314,   315,   316,   317,   318,   456,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,     0,
     332,   333,   334,     0,     0,   337,   338,   339,   340,   457,
     458,   459,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   461,   462,     0,     0,     0,     0,
       0,     0,     0,  1398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   466,   467,   468,
     469,   470,     0,   471,     0,   472,   473,   474,   475,   476,
     477,   478,   479,    59,   555,   481,   482,     0,     0,     0,
       0,     0,     0,   483,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     486,   487,   488,     0,    14,     0,     0,   489,   490,     0,
       0,   424,   425,     0,     0,   491,     0,   492,     0,   493,
     494,   430,   431,   432,   433,   434,     0,     0,     0,     0,
       0,   435,     0,   437,     0,     0,     0,   440,     0,     0,
       0,     0,     0,     0,     0,   442,     0,     0,     0,     0,
       0,   445,     0,     0,   446,     0,     0,   447,     0,     0,
       0,   450,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   554,     0,     0,   454,   455,   310,   311,   312,     0,
     314,   315,   316,   317,   318,   456,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,     0,   332,   333,
     334,     0,     0,   337,   338,   339,   340,   457,   458,   459,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   461,   462,     0,     0,     0,     0,     0,     0,
       0,  1428,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   466,   467,   468,   469,   470,
       0,   471,     0,   472,   473,   474,   475,   476,   477,   478,
     479,    59,   555,   481,   482,     0,     0,     0,     0,     0,
       0,   483,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   486,   487,
     488,     0,    14,     0,     0,   489,   490,     0,     0,   424,
     425,     0,     0,   491,     0,   492,     0,   493,   494,   430,
     431,   432,   433,   434,     0,     0,     0,     0,     0,   435,
       0,   437,     0,     0,     0,   440,     0,     0,     0,     0,
       0,     0,     0,   442,     0,     0,     0,     0,     0,   445,
       0,     0,   446,     0,     0,   447,     0,     0,     0,   450,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   554,
       0,     0,   454,   455,   310,   311,   312,     0,   314,   315,
     316,   317,   318,   456,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,     0,   332,   333,   334,     0,
       0,   337,   338,   339,   340,   457,   458,   459,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     461,   462,     0,     0,     0,     0,     0,     0,     0,  1429,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   466,   467,   468,   469,   470,     0,   471,
       0,   472,   473,   474,   475,   476,   477,   478,   479,    59,
     555,   481,   482,     0,     0,     0,     0,     0,     0,   483,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   486,   487,   488,     0,
      14,     0,     0,   489,   490,     0,     0,   424,   425,     0,
       0,   491,     0,   492,     0,   493,   494,   430,   431,   432,
     433,   434,     0,     0,     0,     0,     0,   435,     0,   437,
       0,     0,     0,   440,     0,     0,     0,     0,     0,     0,
       0,   442,     0,     0,     0,     0,     0,   445,     0,     0,
     446,     0,     0,   447,     0,     0,     0,   450,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   554,     0,     0,
     454,   455,   310,   311,   312,     0,   314,   315,   316,   317,
     318,   456,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,     0,   332,   333,   334,     0,     0,   337,
     338,   339,   340,   457,   458,   459,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   461,   462,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   466,   467,   468,   469,   470,     0,   471,     0,   472,
     473,   474,   475,   476,   477,   478,   479,    59,   555,   481,
     482,     0,     0,     0,     0,     0,     0,   483,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   486,   487,   488,     0,    14,     0,
       0,   489,   490,     0,     0,   424,   425,     0,     0,   491,
       0,   492,     0,   493,   494,   430,   431,   432,   433,   434,
       0,     0,     0,     0,     0,   435,     0,   437,     0,     0,
     399,   440,     0,     0,     0,     0,     0,     0,     0,   442,
       0,     0,     0,     0,     0,   445,     0,     0,   446,     0,
       0,   447,     0,     0,     0,   450,     0,   400,   401,     0,
       0,     0,     0,     0,     0,   554,     0,     0,   454,   455,
     310,   311,   312,     0,   314,   315,   316,   317,   318,   456,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,     0,   332,   333,   334,     0,     0,   337,   338,   339,
     340,   457,   458,   459,     0,     0,     0,     0,   399,     0,
       0,     0,     0,     0,     0,     0,   461,   462,     0,   402,
       0,     0,     0,   403,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   400,   401,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   466,
     467,   468,   469,   470,     0,   471,     0,   472,   473,   474,
     475,   476,   477,   478,   479,    59,   555,   481,   482,     0,
       0,     0,     0,   304,     0,   483,     0,     0,     0,   305,
       0,     0,   404,     0,     0,   306,   405,     0,  1400,   406,
       0,     0,   486,   487,   488,   307,    14,   402,     0,   489,
     490,   403,     0,   308,   407,     0,     0,  1243,     0,   492,
     408,   493,   494,     0,     0,     0,     0,     0,   309,     0,
       0,     0,     0,     0,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,     0,     0,     0,
     404,     0,     0,     0,   405,     0,     0,   406,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   304,   407,     0,     0,     0,     0,   305,   408,     0,
       0,     0,     0,   306,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   307,     0,     0,     0,     0,   343,     0,
       0,   308,     0,     0,     0,     0,     0,     0,     0,   598,
       0,     0,     0,     0,     0,     0,   309,     0,     0,     0,
       0,   599,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   344,     0,     0,     0,     0,
       0,     0,     0,     0,   709,   710,     0,   304,     0,     0,
       0,     0,     0,   305,     0,     0,     0,     0,     0,   306,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   307,
       0,     0,     0,    58,     0,     0,     0,   308,     0,     0,
       0,     0,     0,     0,     0,     0,   343,     0,     0,     0,
       0,     0,   309,     0,     0,     0,     0,    59,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,     0,     0,     0,     0,     0,     0,   719,   720,   721,
     722,   723,     0,   344,   726,   727,   728,   729,     0,   731,
     732,   733,   734,     0,     0,   709,   710,   735,     0,   737,
     738,     0,     0,     0,     0,   741,   742,   743,     0,    58,
       0,   747,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   343,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   598,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   749,     0,
     750,   751,   752,   753,   754,   755,   756,   757,   758,   759,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   760,
     761,     0,     0,     0,   493,   660,     0,     0,     0,   344,
     836,   837,   838,   839,   840,   841,   842,   843,   719,   720,
     721,   722,   723,   844,   845,   726,   727,   728,   729,   846,
     731,   732,   733,   734,  -347,   709,   710,     0,   735,   736,
     737,   738,   847,   848,     0,     0,   741,   742,   743,   849,
     850,   851,   747,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   852,   749,
       0,   750,   751,   752,   753,   754,   755,   756,   757,   758,
     759,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     760,   761,     0,     0,     0,   493,   660,     0,     0,     0,
     836,   837,   838,   839,   840,   841,   842,   843,   719,   720,
     721,   722,   723,   844,   845,   726,   727,   728,   729,   846,
     731,   732,   733,   734,   709,   710,     0,     0,   735,   736,
     737,   738,   847,   848,     0,     0,   741,   742,   743,   849,
     850,   851,   747,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   709,   710,     0,     0,     0,     0,     0,     0,
       0,  1018,     0,     0,     0,     0,     0,     0,   852,   749,
       0,   750,   751,   752,   753,   754,   755,   756,   757,   758,
     759,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     760,   761,     0,     0,     0,   493,   660,     0,     0,   836,
     837,   838,   839,   840,   841,   842,   843,   719,   720,   721,
     722,   723,   844,   845,   726,   727,   728,   729,   846,   731,
     732,   733,   734,     0,     0,     0,     0,   735,   736,   737,
     738,   847,   848,     0,     0,   741,   742,   743,   849,   850,
     851,   747,     0,     0,     0,   719,   720,   721,   722,   723,
     709,   710,   726,   727,   728,   729,     0,   731,   732,   733,
     734,     0,     0,     0,     0,   735,     0,   737,   738,     0,
    1173,     0,     0,   741,   742,   743,     0,   852,   749,   747,
     750,   751,   752,   753,   754,   755,   756,   757,   758,   759,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   760,
     761,     0,     0,     0,   493,   660,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   749,     0,   750,   751,
     752,   753,   754,   755,   756,   757,   758,   759,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   760,   761,     0,
       0,   773,     0,   719,   720,   721,   722,   723,   709,   710,
     726,   727,   728,   729,     0,   731,   732,   733,   734,     0,
       0,     0,     0,   735,     0,   737,   738,     0,     0,     0,
       0,   741,   742,   743,     0,     0,     0,   747,     0,     0,
       0,     0,     0,   709,   710,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   749,     0,   750,   751,   752,   753,
     754,   755,   756,   757,   758,   759,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   760,   761,     0,     0,   783,
       0,   719,   720,   721,   722,   723,     0,     0,   726,   727,
     728,   729,     0,   731,   732,   733,   734,     0,     0,     0,
       0,   735,     0,   737,   738,     0,     0,     0,     0,   741,
     742,   743,     0,     0,     0,   747,   719,   720,   721,   722,
     723,   709,   710,   726,   727,   728,   729,     0,   731,   732,
     733,   734,     0,     0,     0,     0,   735,     0,   737,   738,
       0,     0,     0,     0,   741,   742,   743,     0,     0,     0,
     747,     0,   749,     0,   750,   751,   752,   753,   754,   755,
     756,   757,   758,   759,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   760,   761,     0,     0,   799,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   749,     0,   750,
     751,   752,   753,   754,   755,   756,   757,   758,   759,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   760,   761,
       0,     0,   814,     0,   719,   720,   721,   722,   723,   709,
     710,   726,   727,   728,   729,     0,   731,   732,   733,   734,
       0,     0,     0,     0,   735,     0,   737,   738,     0,     0,
       0,     0,   741,   742,   743,     0,     0,     0,   747,     0,
       0,     0,     0,     0,   709,   710,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   749,     0,   750,   751,   752,
     753,   754,   755,   756,   757,   758,   759,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   760,   761,     0,     0,
    1083,     0,   719,   720,   721,   722,   723,     0,     0,   726,
     727,   728,   729,     0,   731,   732,   733,   734,     0,     0,
       0,     0,   735,     0,   737,   738,     0,     0,     0,     0,
     741,   742,   743,     0,     0,     0,   747,   719,   720,   721,
     722,   723,   709,   710,   726,   727,   728,   729,     0,   731,
     732,   733,   734,     0,     0,     0,     0,   735,     0,   737,
     738,     0,     0,     0,     0,   741,   742,   743,     0,     0,
       0,   747,     0,   749,     0,   750,   751,   752,   753,   754,
     755,   756,   757,   758,   759,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   760,   761,     0,     0,  1087,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   749,     0,
     750,   751,   752,   753,   754,   755,   756,   757,   758,   759,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   760,
     761,     0,     0,  1089,     0,   719,   720,   721,   722,   723,
     709,   710,   726,   727,   728,   729,     0,   731,   732,   733,
     734,     0,     0,     0,     0,   735,     0,   737,   738,     0,
       0,     0,     0,   741,   742,   743,     0,     0,     0,   747,
       0,     0,     0,     0,     0,   709,   710,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   749,     0,   750,   751,
     752,   753,   754,   755,   756,   757,   758,   759,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   760,   761,     0,
       0,  1098,     0,   719,   720,   721,   722,   723,     0,     0,
     726,   727,   728,   729,     0,   731,   732,   733,   734,     0,
       0,     0,     0,   735,     0,   737,   738,     0,     0,     0,
       0,   741,   742,   743,     0,     0,     0,   747,   719,   720,
     721,   722,   723,   709,   710,   726,   727,   728,   729,     0,
     731,   732,   733,   734,     0,     0,     0,     0,   735,     0,
     737,   738,     0,     0,     0,     0,   741,   742,   743,     0,
       0,     0,   747,     0,   749,     0,   750,   751,   752,   753,
     754,   755,   756,   757,   758,   759,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   760,   761,     0,     0,  1099,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   749,
       0,   750,   751,   752,   753,   754,   755,   756,   757,   758,
     759,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     760,   761,     0,     0,  1100,     0,   719,   720,   721,   722,
     723,   709,   710,   726,   727,   728,   729,     0,   731,   732,
     733,   734,     0,     0,     0,     0,   735,     0,   737,   738,
       0,     0,     0,     0,   741,   742,   743,     0,     0,     0,
     747,     0,     0,     0,     0,     0,   709,   710,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   749,     0,   750,
     751,   752,   753,   754,   755,   756,   757,   758,   759,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   760,   761,
       0,     0,  1101,     0,   719,   720,   721,   722,   723,     0,
       0,   726,   727,   728,   729,     0,   731,   732,   733,   734,
       0,     0,     0,     0,   735,     0,   737,   738,     0,     0,
       0,     0,   741,   742,   743,     0,     0,     0,   747,   719,
     720,   721,   722,   723,   709,   710,   726,   727,   728,   729,
       0,   731,   732,   733,   734,     0,     0,     0,     0,   735,
       0,   737,   738,     0,     0,     0,     0,   741,   742,   743,
       0,     0,     0,   747,     0,   749,     0,   750,   751,   752,
     753,   754,   755,   756,   757,   758,   759,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   760,   761,     0,     0,
    1102,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     749,     0,   750,   751,   752,   753,   754,   755,   756,   757,
     758,   759,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   760,   761,     0,     0,  1103,     0,   719,   720,   721,
     722,   723,   709,   710,   726,   727,   728,   729,     0,   731,
     732,   733,   734,     0,     0,     0,     0,   735,     0,   737,
     738,     0,     0,     0,     0,   741,   742,   743,     0,     0,
       0,   747,     0,     0,     0,     0,     0,   709,   710,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   749,     0,
     750,   751,   752,   753,   754,   755,   756,   757,   758,   759,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   760,
     761,     0,     0,  1194,     0,   719,   720,   721,   722,   723,
       0,     0,   726,   727,   728,   729,     0,   731,   732,   733,
     734,     0,     0,     0,     0,   735,     0,   737,   738,     0,
       0,     0,     0,   741,   742,   743,     0,     0,     0,   747,
     719,   720,   721,   722,   723,   709,   710,   726,   727,   728,
     729,     0,   731,   732,   733,   734,     0,     0,     0,     0,
     735,     0,   737,   738,     0,     0,     0,     0,   741,   742,
     743,     0,     0,     0,   747,     0,   749,     0,   750,   751,
     752,   753,   754,   755,   756,   757,   758,   759,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   760,   761,     0,
       0,  1204,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   749,     0,   750,   751,   752,   753,   754,   755,   756,
     757,   758,   759,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   760,   761,     0,     0,  1311,     0,   719,   720,
     721,   722,   723,   709,   710,   726,   727,   728,   729,     0,
     731,   732,   733,   734,     0,     0,     0,     0,   735,     0,
     737,   738,     0,     0,     0,     0,   741,   742,   743,     0,
       0,     0,   747,     0,     0,     0,     0,     0,   709,   710,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   749,
       0,   750,   751,   752,   753,   754,   755,   756,   757,   758,
     759,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     760,   761,     0,     0,  1312,     0,   719,   720,   721,   722,
     723,     0,     0,   726,   727,   728,   729,     0,   731,   732,
     733,   734,     0,     0,     0,     0,   735,     0,   737,   738,
       0,     0,     0,     0,   741,   742,   743,     0,     0,     0,
     747,   719,   720,   721,   722,   723,   709,   710,   726,   727,
     728,   729,     0,   731,   732,   733,   734,     0,     0,     0,
       0,   735,     0,   737,   738,     0,     0,     0,     0,   741,
     742,   743,     0,     0,     0,   747,     0,   749,     0,   750,
     751,   752,   753,   754,   755,   756,   757,   758,   759,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   760,   761,
       0,     0,  1319,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   749,     0,   750,   751,   752,   753,   754,   755,
     756,   757,   758,   759,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   760,   761,     0,     0,  1338,     0,   719,
     720,   721,   722,   723,   709,   710,   726,   727,   728,   729,
       0,   731,   732,   733,   734,     0,     0,     0,     0,   735,
       0,   737,   738,     0,     0,     0,     0,   741,   742,   743,
       0,     0,     0,   747,     0,     0,     0,     0,     0,   709,
     710,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     749,     0,   750,   751,   752,   753,   754,   755,   756,   757,
     758,   759,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   760,   761,     0,     0,  1340,     0,   719,   720,   721,
     722,   723,     0,     0,   726,   727,   728,   729,     0,   731,
     732,   733,   734,     0,     0,     0,     0,   735,     0,   737,
     738,     0,     0,     0,     0,   741,   742,   743,     0,     0,
       0,   747,   719,   720,   721,   722,   723,   709,   710,   726,
     727,   728,   729,     0,   731,   732,   733,   734,     0,     0,
       0,     0,   735,     0,   737,   738,     0,     0,     0,     0,
     741,   742,   743,     0,     0,     0,   747,     0,   749,     0,
     750,   751,   752,   753,   754,   755,   756,   757,   758,   759,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   760,
     761,     0,     0,  1342,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   749,     0,   750,   751,   752,   753,   754,
     755,   756,   757,   758,   759,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   760,   761,     0,     0,  1346,     0,
     719,   720,   721,   722,   723,   709,   710,   726,   727,   728,
     729,     0,   731,   732,   733,   734,     0,     0,     0,     0,
     735,     0,   737,   738,     0,     0,     0,     0,   741,   742,
     743,     0,     0,     0,   747,     0,     0,     0,     0,     0,
     709,   710,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   749,     0,   750,   751,   752,   753,   754,   755,   756,
     757,   758,   759,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   760,   761,     0,     0,  1403,     0,   719,   720,
     721,   722,   723,     0,     0,   726,   727,   728,   729,     0,
     731,   732,   733,   734,     0,     0,     0,     0,   735,     0,
     737,   738,     0,     0,     0,     0,   741,   742,   743,     0,
       0,     0,   747,   719,   720,   721,   722,   723,   709,   710,
     726,   727,   728,   729,     0,   731,   732,   733,   734,     0,
       0,     0,     0,   735,     0,   737,   738,     0,     0,     0,
       0,   741,   742,   743,     0,     0,     0,   747,     0,   749,
       0,   750,   751,   752,   753,   754,   755,   756,   757,   758,
     759,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     760,   761,     0,     0,  1404,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   749,     0,   750,   751,   752,   753,
     754,   755,   756,   757,   758,   759,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   760,   761,     0,     0,  1405,
       0,   719,   720,   721,   722,   723,   709,   710,   726,   727,
     728,   729,     0,   731,   732,   733,   734,     0,     0,     0,
       0,   735,     0,   737,   738,     0,     0,     0,     0,   741,
     742,   743,     0,     0,     0,   747,     0,     0,     0,     0,
       0,   709,   710,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   749,     0,   750,   751,   752,   753,   754,   755,
     756,   757,   758,   759,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   760,   761,     0,     0,  1414,     0,   719,
     720,   721,   722,   723,     0,     0,   726,   727,   728,   729,
       0,   731,   732,   733,   734,     0,     0,     0,     0,   735,
       0,   737,   738,     0,     0,     0,     0,   741,   742,   743,
       0,     0,     0,   747,   719,   720,   721,   722,   723,   709,
     710,   726,   727,   728,   729,     0,   731,   732,   733,   734,
       0,     0,     0,     0,   735,     0,   737,   738,     0,     0,
       0,     0,   741,   742,   743,     0,     0,     0,   747,     0,
     749,     0,   750,   751,   752,   753,   754,   755,   756,   757,
     758,   759,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   760,   761,     0,     0,  1421,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   749,     0,   750,   751,   752,
     753,   754,   755,   756,   757,   758,   759,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   760,   761,     0,     0,
    1435,     0,   719,   720,   721,   722,   723,   709,   710,   726,
     727,   728,   729,     0,   731,   732,   733,   734,     0,     0,
       0,     0,   735,     0,   737,   738,     0,     0,     0,     0,
     741,   742,   743,     0,     0,     0,   747,     0,     0,     0,
       0,     0,   709,   710,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   749,     0,   750,   751,   752,   753,   754,
     755,   756,   757,   758,   759,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   760,   761,     0,     0,  1465,     0,
     719,   720,   721,   722,   723,     0,     0,   726,   727,   728,
     729,     0,   731,   732,   733,   734,     0,     0,     0,     0,
     735,     0,   737,   738,     0,     0,     0,     0,   741,   742,
     743,     0,     0,     0,   747,   719,   720,   721,   722,   723,
     709,   710,   726,   727,   728,   729,     0,   731,   732,   733,
     734,     0,     0,     0,     0,   735,     0,   737,   738,     0,
       0,     0,     0,   741,   742,   743,     0,     0,     0,   747,
       0,   749,     0,   750,   751,   752,   753,   754,   755,   756,
     757,   758,   759,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   760,   761,   804,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   749,     0,   750,   751,
     752,   753,   754,   755,   756,   757,   758,   759,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   760,   761,  1040,
       0,     0,     0,   719,   720,   721,   722,   723,   709,   710,
     726,   727,   728,   729,     0,   731,   732,   733,   734,     0,
       0,     0,     0,   735,     0,   737,   738,     0,     0,     0,
       0,   741,   742,   743,     0,     0,     0,   747,     0,     0,
       0,     0,     0,   709,   710,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   749,     0,   750,   751,   752,   753,
     754,   755,   756,   757,   758,   759,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   760,   761,  1147,     0,     0,
       0,   719,   720,   721,   722,   723,     0,     0,   726,   727,
     728,   729,     0,   731,   732,   733,   734,     0,     0,     0,
       0,   735,     0,   737,   738,     0,     0,     0,     0,   741,
     742,   743,     0,     0,     0,   747,   719,   720,   721,   722,
     723,   709,   710,   726,   727,   728,   729,     0,   731,   732,
     733,   734,     0,     0,     0,     0,   735,     0,   737,   738,
       0,     0,     0,     0,   741,   742,   743,     0,     0,     0,
     747,     0,   749,     0,   750,   751,   752,   753,   754,   755,
     756,   757,   758,   759,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   760,   761,  1162,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   749,     0,   750,
     751,   752,   753,   754,   755,   756,   757,   758,   759,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   760,   761,
    1266,     0,     0,     0,   719,   720,   721,   722,   723,     0,
       0,   726,   727,   728,   729,     0,   731,   732,   733,   734,
     229,   230,     0,     0,   735,     0,   737,   738,     0,     0,
       0,     0,   741,   742,   743,     0,     0,   231,   747,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   709,   710,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   749,     0,   750,   751,   752,
     753,   754,   755,   756,   757,   758,   759,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   760,   761,  1271,     0,
       0,     0,     0,     0,     0,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,     0,     0,   250,   251,   252,     0,     0,
       0,     0,     0,     0,   253,   254,   255,   256,   257,     0,
       0,   258,   259,   260,   261,   262,   263,   264,   719,   720,
     721,   722,   723,   709,   710,   726,   727,   728,   729,     0,
     731,   732,   733,   734,     0,     0,     0,     0,   735,     0,
     737,   738,     0,     0,     0,     0,   741,   742,   743,     0,
       0,     0,   747,     0,   265,     0,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,     0,     0,   276,   277,
       0,     0,     0,     0,     0,   278,   279,     0,     0,   284,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   749,
       0,   750,   751,   752,   753,   754,   755,   756,   757,   758,
     759,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     760,   761,     0,     0,     0,     0,   719,   720,   721,   722,
     723,   709,   710,   726,   727,   728,   729,     0,   731,   732,
     733,   734,     0,     0,     0,     0,   735,     0,   737,   738,
       0,     0,   891,     0,   741,   742,   743,     0,     0,     0,
     747,     0,     0,     0,     0,     0,   709,   710,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   749,     0,   750,
     751,   752,   753,   754,   755,   756,   757,   758,   759,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   760,   761,
       0,     0,     0,     0,   719,   720,   721,   722,   723,     0,
       0,   726,   727,   728,   729,     0,   731,   732,   733,   734,
       0,     0,     0,     0,   735,     0,   737,   738,     0,     0,
       0,     0,   741,   742,   743,     0,     0,     0,   747,   719,
     720,   721,   722,   723,   709,   710,   726,   727,   728,   729,
       0,   731,   732,   733,   734,     0,     0,     0,     0,   735,
       0,   737,   738,     0,     0,     0,     0,   741,   742,   743,
       0,     0,     0,   747,     0,   749,  1152,   750,   751,   752,
     753,   754,   755,   756,   757,   758,   759,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   760,   761,     0,     0,
       0,     0,  1164,     0,     0,     0,     0,     0,     0,     0,
     749,     0,   750,   751,   752,   753,   754,   755,   756,   757,
     758,   759,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   760,   761,     0,     0,     0,     0,   719,   720,   721,
     722,   723,   709,   710,   726,   727,   728,   729,     0,   731,
     732,   733,   734,     0,     0,     0,     0,   735,     0,   737,
     738,     0,     0,     0,     0,   741,   742,   743,     0,     0,
       0,   747,     0,     0,     0,     0,     0,     0,   709,   710,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1226,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   749,     0,
     750,   751,   752,   753,   754,   755,   756,   757,   758,   759,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   760,
     761,     0,     0,     0,     0,   719,   720,   721,   722,   723,
       0,     0,   726,   727,   728,   729,     0,   731,   732,   733,
     734,     0,     0,     0,     0,   735,     0,   737,   738,     0,
       0,     0,     0,   741,   742,   743,     0,     0,     0,   747,
       0,   719,   720,   721,   722,   723,   709,   710,   726,   727,
     728,   729,     0,   731,   732,   733,   734,     0,     0,     0,
       0,   735,     0,   737,   738,     0,     0,     0,  1423,   741,
     742,   743,     0,     0,     0,   747,   749,     0,   750,   751,
     752,   753,   754,   755,   756,   757,   758,   759,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   760,   761,     0,
       0,     0,     0,     0,  1461,     0,     0,     0,     0,     0,
       0,     0,   749,     0,   750,   751,   752,   753,   754,   755,
     756,   757,   758,   759,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   760,   761,     0,     0,     0,     0,   719,
     720,   721,   722,   723,   709,   710,   726,   727,   728,   729,
       0,   731,   732,   733,   734,     0,     0,     0,     0,   735,
       0,   737,   738,     0,     0,     0,     0,   741,   742,   743,
       0,     0,     0,   747,     0,     0,     0,     0,     0,   709,
     710,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1472,     0,     0,     0,     0,     0,     0,     0,
     749,     0,   750,   751,   752,   753,   754,   755,   756,   757,
     758,   759,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   760,   761,     0,     0,     0,     0,   719,   720,   721,
     722,   723,     0,     0,   726,   727,   728,   729,     0,   731,
     732,   733,   734,     0,     0,     0,     0,   735,     0,   737,
     738,     0,     0,     0,     0,   741,   742,   743,     0,     0,
       0,   747,   719,   720,   721,   722,   723,   709,   710,   726,
     727,   728,   729,     0,   731,   732,   733,   734,     0,     0,
       0,     0,   735,     0,   737,   738,     0,     0,     0,     0,
     741,   742,   743,     0,     0,     0,  -734,     0,   749,     0,
     750,   751,   752,   753,   754,   755,   756,   757,   758,   759,
       0,   709,   710,     0,     0,     0,     0,     0,     0,   760,
     761,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   749,     0,   750,   751,   752,   753,   754,
     755,   756,   757,   758,   759,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   760,   761,     0,     0,     0,     0,
     719,   720,   721,   722,   723,     0,     0,   726,   727,   728,
     729,     0,   731,   732,   733,   734,     0,     0,     0,     0,
     735,     0,   737,   738,     0,     0,     0,     0,   741,   742,
     743,     0,     0,     0,     0,     0,     0,     0,   709,   710,
       0,     0,     0,     0,   719,   720,   721,   722,   723,     0,
       0,   726,   727,   728,   729,     0,   731,   732,   733,   734,
       0,     0,   709,   710,   735,     0,   737,   738,     0,     0,
       0,   749,   741,   750,   751,   752,   753,   754,   755,   756,
     757,   758,   759,     0,     0,     0,   709,   710,     0,     0,
       0,     0,   760,   761,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   750,   751,   752,
     753,   754,   755,   756,   757,   758,   759,     0,     0,     0,
       0,   719,   720,   721,   722,   723,   760,   761,   726,   727,
     728,   729,     0,   731,   732,   733,   734,     0,     0,     0,
       0,   735,     0,   737,   738,   719,   720,   721,   722,   723,
       0,     0,   726,   727,   728,   729,     0,   731,   732,   733,
     734,     0,     0,     0,     0,   735,     0,   737,   738,   719,
     720,   721,   722,   723,     0,     0,   726,   727,   728,   729,
       0,   731,   732,   733,   734,     0,     0,     0,     0,   735,
       0,   737,   738,     0,   750,   751,   752,   753,   754,   755,
     756,   757,   758,   759,     0,     0,     0,     0,     0,     0,
       0,     0,   920,   760,   761,     0,     0,     0,     0,     0,
     752,   753,   754,   755,   756,   757,   758,   759,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   760,   761,     0,
       0,     0,     0,   924,     0,   753,   754,   755,   756,   757,
     758,   759,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   760,   761,   310,   311,   312,     0,   314,   315,   316,
     317,   318,   456,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,     0,   332,   333,   334,     0,     0,
     337,   338,   339,   340,   310,   311,   312,     0,   314,   315,
     316,   317,   318,   456,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,     0,   332,   333,   334,     0,
       0,   337,   338,   339,   340,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1148,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   921,
       0,     0,     0,     0,     0,     0,     0,     0,   922,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     925,     0,   181,     0,     0,     0,   310,   311,   312,   926,
     314,   315,   316,   317,   318,   456,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,     0,   332,   333,
     334,     0,     0,   337,   338,   339,   340,   182,     0,   183,
       0,   184,   185,   186,   187,   188,     0,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,     0,   200,
     201,   202,     0,     0,   203,   204,   205,   206,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   207,   208,     0,     0,     0,     0,     0,
       0,     0,  1149,     0,     0,     0,     0,     0,     0,     0,
       0,  1150,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   209
};

static const yytype_int16 yycheck[] =
{
      14,    15,   381,   211,   560,   611,   369,   632,   828,   580,
     161,   687,   579,   689,   436,   790,   892,   792,   909,   794,
     376,   877,   369,   527,   103,     7,    56,  1283,   225,    32,
       0,   410,    62,    19,    19,   574,     6,   576,    19,   578,
     464,   465,   126,   127,   436,    21,   124,   551,   139,    63,
      64,    65,    18,    19,   164,   616,   129,    14,    15,    29,
     152,    31,   189,    33,   141,   142,   143,    32,   460,    39,
     492,   157,   225,    33,   164,  1376,   124,    45,   124,    49,
      56,   208,   140,   174,   149,    55,     4,     5,   134,   436,
     104,   105,   106,   107,   123,   126,   127,   126,   127,   164,
     210,   124,    62,   176,   182,   183,    24,   185,    78,   124,
     188,   134,    30,   460,   206,   676,  1372,   140,   208,   134,
       6,   205,   206,    49,   155,   686,  1427,   185,   140,   124,
     100,   101,   209,   164,  1406,   183,   104,   183,   186,   134,
     189,   176,   188,   208,   182,   175,   297,   174,   174,    67,
      68,   186,   164,   369,    20,    21,   832,   124,   662,   208,
     183,   583,   182,    49,   182,    91,   176,   134,   183,   207,
     367,  1443,   564,   133,   205,   206,   205,   206,   152,   855,
     140,   208,   100,   101,   381,   188,   176,   207,   183,    20,
      21,   209,   868,   175,   182,   871,   182,   182,   164,   209,
     157,   182,   624,   625,   164,   162,   176,   164,    20,    21,
     167,   225,    61,   410,   367,   412,   183,   564,   194,   298,
     436,   209,   140,   188,   206,   195,   182,   649,   124,   205,
     652,   439,   206,   625,   183,   209,   206,   174,   134,   199,
    1096,   206,    14,    15,   460,   453,   164,   212,   208,   115,
     116,   208,   631,   209,   124,   182,   174,   123,   182,   125,
     126,   127,   128,   655,   134,   185,   182,   133,   188,  1125,
     189,   189,   182,   210,   211,   654,   698,   140,   625,   164,
     165,   166,   209,   707,   115,   116,   149,   183,   206,   208,
     182,   182,   123,   209,   125,   126,   127,   128,   140,   209,
      46,   164,   133,   115,   116,   189,   182,   149,   655,   182,
     195,   123,   154,   183,   126,   127,   128,   209,   209,    73,
      66,   133,   164,    77,   208,   191,   192,   193,   194,   195,
     344,   207,     4,     5,   182,     7,   209,   182,   189,   205,
     206,   763,    96,    97,    98,    99,   182,   189,   564,   705,
     174,   149,  1243,   367,   164,   155,   182,   208,   372,   207,
     185,   185,  1137,    35,   164,   190,   164,   381,   565,   566,
     567,   207,   569,   570,   205,   206,   573,   176,   575,   918,
     577,   207,   579,   208,   776,   157,   149,   164,   168,   169,
     162,   190,   164,   205,   206,   167,   410,   176,   412,   176,
     185,   164,   906,   185,    56,   190,   828,   615,   190,   625,
      62,   190,   565,   566,   567,    56,   569,   570,   432,   164,
     573,    62,   575,   208,   577,   185,   208,   185,   636,   776,
     822,   176,   190,  1309,   631,    63,    64,    65,   140,   655,
    1046,   176,   176,   174,   823,   824,   176,  1222,   176,   176,
     208,   182,   831,   210,   211,   190,   190,   654,  1233,    11,
     190,   164,   190,   190,  1035,   673,  1033,   141,   186,   143,
      22,    23,   190,   176,   155,   822,   104,   105,   106,   107,
     155,   860,   861,   164,   863,   693,    56,   208,   867,   164,
     869,   870,    62,   872,   155,   711,   712,   713,   714,   715,
     716,   717,   718,   164,   928,   929,   930,   155,   724,   725,
     176,   205,   709,   710,   730,   182,   164,   183,   185,    56,
      56,   188,   164,   739,   740,    62,    62,    56,   744,   745,
     746,    56,   748,    62,  1354,  1355,   176,    62,   735,   157,
     186,   176,   182,  1318,   190,  1365,  1366,   182,   168,   169,
     186,   565,   566,   567,   190,   569,   570,   164,  1097,   573,
     776,   575,   186,   577,   186,   579,   190,   186,   190,   186,
     186,   190,   186,   190,   190,   176,   190,   186,   186,  1118,
     176,   190,   190,   164,  1209,   168,   169,   170,   171,   164,
     141,   142,   143,   164,   165,   166,   168,   169,   170,    20,
      21,   164,   884,   800,   886,   164,   822,   621,    10,    11,
      12,   808,    86,    87,   811,   168,   164,   631,   174,  1295,
     771,    56,   176,   820,   168,   141,   823,   824,   104,  1051,
     827,   174,   206,   830,   831,   174,   183,    34,    34,  1245,
     654,   174,   208,   174,   164,   853,   206,   800,   175,   164,
     164,   164,    32,    21,   164,   808,   174,   207,   811,   185,
      42,   207,   185,   860,   861,   174,   863,   820,   208,   164,
     867,   134,   869,   870,   827,   872,   690,   830,   189,    59,
      60,   189,   189,   189,   208,  1107,   189,   884,   189,   886,
    1112,   208,   189,   189,   115,   116,   189,   189,   189,  1238,
     186,  1080,   123,   208,   125,   126,   127,   128,   208,   164,
     164,   164,   133,  1319,   189,   164,   206,   189,   189,   208,
     208,   189,   189,  1145,   208,   176,   157,   189,   189,    32,
     189,   884,   189,   886,  1359,  1157,    36,   189,  1117,  1415,
    1416,   121,  1418,   208,   124,   125,   176,   211,  1111,   176,
     208,     9,   208,   176,   134,   769,    59,    60,   208,   176,
     208,   208,   176,   176,  1111,   176,    65,   206,   175,   189,
     208,  1327,   193,   194,   195,   207,    42,   164,   164,   208,
     175,   209,   207,   176,   205,   206,   800,   176,   176,   208,
     987,   175,   175,   185,   808,    42,   176,   811,   208,   129,
      13,  1223,   183,   183,   184,   182,   820,   185,   188,   823,
     824,   191,   157,   827,   174,   164,   830,   831,   121,   182,
     211,   124,   125,   182,     7,  1022,   206,   206,   164,  1435,
     164,   134,   212,   190,   209,   164,  1033,   175,  1394,   209,
     182,   207,   164,   208,   208,   189,   860,   861,   209,   863,
     208,   206,   208,   867,   189,   869,   870,   208,   872,     1,
     208,  1283,   208,   175,    66,   208,   176,   208,   164,  1022,
     884,   190,   886,   190,   190,    42,  1255,   209,   164,   164,
     183,   184,   209,  1080,   208,   188,   209,   208,   191,   209,
     164,   209,   189,   208,   208,  1111,   176,   208,   176,   208,
     208,   164,   209,   206,   208,   164,    32,    18,   164,   212,
     164,  1119,   164,    24,   208,   208,   208,   208,   208,    30,
    1117,   208,   190,   190,   208,   208,   207,   209,    32,    40,
     209,   176,   209,   209,   209,    52,  1358,    48,   209,   209,
     209,   175,  1364,   207,   176,    79,   675,  1326,   209,   209,
    1372,     1,    63,    44,   209,    59,    60,  1379,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,  1199,   133,    82,  1315,  1192,  1316,   223,    80,   769,
     894,  1316,   369,  1316,  1316,  1316,  1428,     1,  1022,  1304,
    1332,   378,   510,  1275,  1351,  1278,   420,   121,  1352,  1033,
     387,   125,  1169,   463,   464,   465,  1247,  1235,    52,   140,
     397,   432,   432,  1375,    -1,    -1,    -1,   549,    -1,  1192,
     407,    -1,   153,    -1,    -1,    -1,  1254,    -1,    -1,   416,
      -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,  1255,   426,
    1257,    20,    21,    -1,    -1,    -1,  1080,    -1,    -1,   436,
     510,    -1,   176,    -1,   441,    -1,   443,    -1,    -1,    -1,
     184,    -1,    -1,    -1,   188,   452,   190,   191,    -1,    -1,
      -1,    -1,    -1,   460,   461,   462,  1304,   537,    -1,   210,
      -1,   212,   206,  1117,  1257,    -1,    -1,    -1,   212,    -1,
      -1,    -1,    -1,   480,    -1,    -1,   483,    -1,    -1,   486,
     487,   488,   489,   490,    -1,    -1,    -1,    -1,    -1,  1326,
      -1,    -1,   499,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1343,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,    32,
      -1,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,   135,   136,  1192,  1387,
      -1,    -1,   141,    -1,   143,    -1,    59,    60,    -1,   629,
    1343,    -1,    -1,    -1,    -1,    -1,    -1,   564,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   657,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   592,    -1,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,    -1,    -1,    -1,
      -1,  1255,    -1,  1257,    -1,    -1,   205,   206,   121,    -1,
      -1,    -1,   125,    -1,    -1,    -1,  1464,    -1,   625,    -1,
      -1,    -1,    -1,    -1,    -1,   632,   633,   707,   635,    -1,
      -1,    -1,    -1,   640,    -1,    -1,    -1,    -1,    -1,   646,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   655,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   666,
     667,   668,   669,   670,   671,    -1,    -1,    -1,    -1,    -1,
      -1,   184,  1326,    -1,    -1,   188,    -1,   190,   191,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1343,
      -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,   778,   212,
      -1,    -1,    -1,    -1,   711,   712,   713,   714,   715,   716,
     717,   718,   719,   720,    -1,    -1,   723,   724,   725,   726,
     727,   728,  1376,   730,   731,    -1,   733,   734,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,   748,   749,   750,   751,   752,   753,   754,   755,   756,
     757,   758,   759,    -1,   761,    -1,    -1,    -1,   765,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,   776,
      -1,    -1,    -1,  1427,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    59,    60,    -1,   875,    -1,    -1,    -1,    -1,
      -1,    -1,    59,    60,    -1,    -1,    -1,    -1,   815,    -1,
      -1,    59,    60,    -1,    -1,   822,    -1,    -1,    -1,    -1,
      -1,    -1,   829,    -1,    -1,    -1,    -1,    -1,   835,   836,
     837,   838,   839,   840,   841,   842,   843,   844,   845,   846,
     847,   848,   849,   850,   851,   852,    -1,    -1,   928,   929,
     930,    -1,    -1,   933,   121,   935,    -1,   937,   125,   939,
      -1,   941,    -1,   943,   121,   945,    -1,   947,   125,    -1,
      -1,    -1,   952,   121,   954,    -1,    -1,   125,    -1,    -1,
     960,   888,    -1,    -1,   891,   892,    -1,    -1,    -1,   896,
      -1,    -1,   972,    -1,   974,    -1,    -1,    -1,   905,   979,
      -1,   981,    -1,   983,     9,    -1,   986,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    21,   184,    -1,    -1,
      -1,   188,    -1,   190,   191,    -1,    -1,   184,    -1,    -1,
      -1,   188,    -1,   190,   191,    -1,   184,  1017,    -1,   206,
     188,    -1,   190,   191,    -1,   212,    -1,    -1,    -1,   206,
      -1,    -1,    -1,    -1,    -1,   212,    20,    21,   206,    -1,
      -1,    -1,    -1,    -1,   212,    -1,    -1,    -1,  1048,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1000,    -1,    -1,    -1,  1004,    -1,    -1,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,   134,
     135,   136,   137,   138,    -1,    -1,   141,   142,   143,   144,
     145,   146,   147,    -1,    -1,    -1,    -1,    -1,    11,   113,
     114,   115,   116,   117,    -1,    -1,   120,    20,    21,   123,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,   135,   136,    -1,    -1,    -1,    -1,  1084,   183,   184,
      -1,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,    20,    21,  1111,   210,   211,    -1,  1115,  1116,
      -1,    -1,    -1,    -1,    -1,  1122,    -1,    -1,    -1,  1126,
    1127,  1128,    -1,  1130,    -1,   189,   190,   191,   192,   193,
     194,   195,  1139,    -1,  1141,    -1,    -1,    -1,    -1,  1146,
      -1,   205,   206,    -1,    -1,  1152,    -1,    -1,    -1,  1156,
     113,   114,   115,   116,   117,    -1,    -1,   120,   121,   122,
     123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
     133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,
     143,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,  1196,
      -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116,    -1,
      -1,    -1,  1209,    -1,    -1,   123,    32,   125,   126,   127,
     128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,    32,
      -1,   184,    -1,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,    59,    60,    -1,    -1,    -1,    -1,    -1,
      20,    21,   205,   206,    -1,    -1,    59,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1263,    -1,  1265,    -1,
      -1,    -1,    -1,  1270,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,   194,   195,    -1,    -1,
      -1,    -1,    -1,    -1,  1291,    -1,    -1,   205,   206,  1296,
    1297,  1298,    -1,    -1,    -1,   121,  1303,    -1,    -1,   125,
    1307,    -1,  1309,  1310,    -1,    -1,    -1,    -1,   121,  1316,
      -1,    -1,   125,    -1,    -1,  1322,  1323,  1324,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1334,    -1,    -1,
      -1,    -1,    -1,   113,   114,   115,   116,   117,    -1,    -1,
     120,   121,   122,   123,    -1,   125,   126,   127,   128,    -1,
      -1,    -1,  1359,   133,    -1,   135,   136,    -1,   184,    -1,
      -1,    -1,   188,    -1,   190,   191,    -1,    -1,    -1,    -1,
      -1,   184,    -1,  1380,    -1,   188,    -1,   190,   191,    -1,
     206,  1388,  1389,    -1,    -1,    -1,   212,    -1,  1395,    -1,
      -1,  1398,    -1,   206,    -1,    -1,    -1,    -1,    -1,   212,
      -1,    -1,    -1,    -1,    -1,    -1,  1413,   187,   188,   189,
     190,   191,   192,   193,   194,   195,    32,    -1,    -1,    -1,
      -1,    -1,  1429,    -1,    -1,   205,   206,    -1,    -1,  1436,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    59,    60,    -1,  1453,    -1,    -1,    -1,
       1,  1458,  1459,     4,     5,     6,    -1,     8,     9,    10,
      -1,    12,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    25,    26,    27,    28,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    -1,    -1,
      41,    -1,    43,    44,    -1,    -1,    47,    -1,    49,    50,
      51,    -1,    53,    54,    -1,   121,    57,    58,    -1,   125,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,
      71,    -1,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      91,    92,    93,    -1,    -1,    96,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,   116,    -1,    -1,   184,    -1,
      -1,    -1,   188,    -1,   190,   191,    -1,    -1,    -1,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     206,    -1,    -1,    -1,    -1,    -1,   212,   148,   149,   150,
     151,   152,    -1,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,    -1,    -1,    -1,
      -1,    -1,    -1,   174,   175,   176,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     191,   192,   193,    -1,   195,    -1,    -1,   198,   199,    -1,
      -1,    -1,    -1,    -1,    -1,   206,    -1,   208,     1,   210,
     211,     4,     5,     6,    -1,     8,     9,    10,    -1,    12,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    25,    26,    27,    28,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    37,    38,    -1,    -1,    41,    -1,
      43,    44,    -1,    -1,    47,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    -1,    57,    58,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    67,    68,    69,    70,    71,    -1,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    91,    92,
      93,    -1,    -1,    96,    97,    98,    99,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,   149,   150,   151,   152,
      -1,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,
      -1,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,
     193,    -1,   195,    -1,    -1,   198,   199,    -1,    -1,     4,
       5,    -1,    -1,   206,    -1,   208,    -1,   210,   211,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    67,    68,    69,    70,    71,    -1,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    91,    92,    93,    -1,
      -1,    96,    97,    98,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,   149,   150,   151,   152,    -1,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,   174,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   191,   192,   193,    -1,
     195,    -1,    -1,   198,   199,    -1,    -1,     4,     5,    -1,
      -1,   206,    -1,   208,    -1,   210,   211,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    69,    70,    71,    -1,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    92,    93,    -1,    -1,    96,
      97,    98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,   149,   150,   151,   152,    -1,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,
      -1,   198,   199,    -1,    -1,     4,     5,    -1,    -1,   206,
      -1,   208,    -1,   210,   211,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    25,    26,    27,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    50,    51,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,
      69,    70,    71,    -1,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    91,    92,    93,    -1,    -1,    96,    97,    98,
      99,   100,   101,   102,   103,    -1,   712,   713,   714,   715,
     716,   717,   718,    -1,    -1,    -1,   115,   116,   724,   725,
      -1,    -1,    -1,    -1,   730,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   739,   740,    -1,    -1,    -1,   744,   745,
     746,   140,   748,    -1,    -1,    -1,    -1,    -1,    -1,   148,
     149,   150,   151,   152,    -1,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,    -1,
      -1,    -1,    -1,    -1,    -1,   174,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,   198,
     199,    -1,    -1,     4,     5,    -1,    -1,   206,    -1,   208,
      -1,   210,   211,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,
      71,    -1,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      91,    92,    93,    -1,    -1,    96,    97,    98,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,   150,
     151,   152,    -1,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,    -1,    -1,    -1,
      -1,    -1,    -1,   174,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     191,   192,   193,    -1,   195,    -1,    -1,   198,   199,    -1,
      -1,     4,     5,    -1,    -1,   206,    -1,   208,    -1,   210,
     211,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    67,    68,    69,    70,    71,    -1,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    91,    92,
      93,    -1,    -1,    96,    97,    98,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,   149,   150,   151,   152,
      -1,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,
      -1,   174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,
     193,    -1,   195,    -1,    -1,   198,   199,    -1,    -1,     4,
       5,    -1,    -1,   206,    -1,   208,    -1,   210,   211,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    67,    68,    69,    70,    71,    -1,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    91,    92,    93,    -1,
      -1,    96,    97,    98,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,   149,   150,   151,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,   174,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   191,   192,   193,    -1,
     195,    -1,    -1,   198,   199,    -1,    -1,     4,     5,    -1,
      -1,   206,    -1,   208,   209,   210,   211,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    69,    70,    71,    -1,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    92,    93,    -1,    -1,    96,
      97,    98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,   149,   150,   151,   152,    32,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    32,   174,    -1,    -1,
      -1,    -1,    -1,    59,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,
      -1,   198,   199,    59,    60,    -1,    -1,     4,     5,   206,
      -1,   208,   209,   210,   211,    12,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,   121,    -1,    44,    -1,   125,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    64,    -1,   125,
      67,    68,    69,    70,    71,    -1,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    92,    93,    -1,    -1,    96,
      97,    98,    99,   100,   101,   102,    -1,    -1,   184,    -1,
      -1,    -1,   188,    -1,   190,   191,    -1,    -1,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,
     206,    -1,   188,    -1,   190,   191,   212,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
     206,   148,   149,   150,   151,   152,   212,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,
      -1,   198,   199,    -1,    -1,     4,     5,    -1,    -1,   206,
      -1,   208,    -1,   210,   211,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,
      69,    70,    71,    -1,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    91,    92,    93,    -1,    -1,    96,    97,    98,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,
     149,   150,   151,   152,    -1,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,    -1,
      -1,    -1,    -1,    -1,    -1,   174,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,   198,
     199,    -1,    -1,     4,     5,    -1,    -1,   206,   207,   208,
      -1,   210,   211,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    60,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,
      71,    -1,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      91,    92,    93,    -1,    -1,    96,    97,    98,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,   150,
     151,   152,    -1,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,    -1,    -1,    -1,
      -1,    -1,    -1,   174,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     191,   192,   193,    -1,   195,    -1,    -1,   198,   199,    -1,
      -1,     4,     5,    -1,    -1,   206,    -1,   208,    -1,   210,
     211,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    67,    68,    69,    70,    71,    -1,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    91,    92,
      93,    -1,    -1,    96,    97,    98,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,   149,   150,   151,   152,
      -1,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,
      -1,   174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,
     193,    -1,   195,    -1,    -1,   198,   199,    -1,    -1,     4,
       5,    -1,    -1,   206,    -1,   208,    -1,   210,   211,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    32,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      -1,    59,    60,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    67,    68,    69,    70,    71,    -1,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    91,    92,    93,    -1,
      -1,    96,    97,    98,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   116,    -1,   121,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,   149,   150,   151,   152,    -1,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,    32,   174,
      -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,    -1,    -1,
     188,    -1,   190,   191,   189,    -1,   191,   192,   193,    -1,
     195,    -1,    -1,   198,   199,    59,    60,    -1,   206,     4,
       5,   206,    -1,   208,   212,   210,   211,    12,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    32,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      -1,    59,    60,    -1,    -1,    -1,    -1,   121,    -1,    64,
      -1,   125,    67,    68,    69,    70,    71,    -1,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    91,    92,    93,    -1,
      -1,    96,    97,    98,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   116,    -1,   121,    -1,    -1,    -1,   125,    -1,    -1,
     184,    -1,    -1,    -1,   188,    -1,   190,   191,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,   206,   148,   149,   150,   151,   152,   212,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,    32,   174,
      -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,    -1,    -1,
     188,    -1,   190,   191,    -1,    -1,   191,   192,   193,    -1,
     195,    -1,    -1,   198,   199,    59,    60,    -1,   206,     4,
       5,   206,    -1,   208,   212,   210,   211,    12,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    64,
      -1,   125,    67,    68,    69,    70,    71,    -1,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    91,    92,    93,    -1,
      -1,    96,    97,    98,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     184,    -1,    -1,    -1,   188,    -1,   190,   191,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,   206,   148,   149,   150,   151,   152,   212,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,   174,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   191,   192,   193,    -1,
     195,    -1,    -1,   198,   199,    -1,    -1,     4,     5,    -1,
      -1,   206,    -1,   208,    -1,   210,   211,    14,    15,    16,
      17,    18,    -1,    -1,    21,    -1,    -1,    24,    -1,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    69,    70,    71,    -1,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    92,    93,    -1,    -1,    96,
      97,    98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,   149,   150,   151,   152,    -1,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,
      -1,   198,   199,    -1,    -1,     4,     5,    -1,    -1,   206,
      -1,   208,    -1,   210,   211,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,
      69,    70,    71,    -1,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    91,    92,    93,    -1,    -1,    96,    97,    98,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,
     149,   150,   151,   152,    -1,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,    -1,
      -1,    -1,    -1,    -1,    -1,   174,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,   198,
     199,    -1,    -1,     4,     5,    -1,    -1,   206,    -1,   208,
     209,   210,   211,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,
      71,    -1,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      91,    92,    93,    -1,    -1,    96,    97,    98,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,   150,
     151,   152,    -1,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,    -1,    -1,    -1,
      -1,    -1,    -1,   174,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,
     191,   192,   193,    -1,   195,    -1,    -1,   198,   199,    -1,
      -1,     4,     5,    -1,    -1,   206,    -1,   208,    -1,   210,
     211,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    67,    68,    69,    70,    71,    -1,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    91,    92,
      93,    -1,    -1,    96,    97,    98,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,   149,   150,   151,   152,
      -1,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,
      -1,   174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,
     193,    -1,   195,    -1,    -1,   198,   199,    -1,    -1,     4,
       5,    -1,    -1,   206,    -1,   208,   209,   210,   211,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    67,    68,    69,    70,    71,    -1,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    91,    92,    93,    -1,
      -1,    96,    97,    98,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,   149,   150,   151,   152,    -1,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,   174,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   191,   192,   193,    -1,
     195,    -1,    -1,   198,   199,    -1,    -1,     4,     5,    -1,
      -1,   206,    -1,   208,   209,   210,   211,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    69,    70,    71,    -1,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    92,    93,    -1,    -1,    96,
      97,    98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,   149,   150,   151,   152,    -1,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,
      -1,   198,   199,    -1,    -1,     4,     5,    -1,    -1,   206,
      -1,   208,   209,   210,   211,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,
      69,    70,    71,    -1,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    91,    92,    93,    -1,    -1,    96,    97,    98,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,
     149,   150,   151,   152,    -1,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,    -1,
      -1,    -1,    -1,    -1,    -1,   174,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   191,   192,   193,    -1,   195,    -1,    -1,   198,
     199,    -1,    -1,     4,     5,    -1,    -1,   206,    -1,   208,
     209,   210,   211,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,
      71,    -1,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      91,    92,    93,    -1,    -1,    96,    97,    98,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,   150,
     151,   152,    -1,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,    -1,    -1,    -1,
      -1,    -1,    -1,   174,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     191,   192,   193,    -1,   195,    -1,    -1,   198,   199,    -1,
      -1,     4,     5,    -1,    -1,   206,    -1,   208,    -1,   210,
     211,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    67,    68,    69,    70,    71,    -1,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    91,    92,
      93,    -1,    -1,    96,    97,    98,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,   149,   150,   151,   152,
      -1,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,
      -1,   174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,
     193,    -1,   195,    -1,    -1,   198,   199,    -1,    -1,     4,
       5,    -1,    -1,   206,    -1,   208,    -1,   210,   211,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    67,    68,    69,    70,    71,    -1,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    91,    92,    93,    -1,
      -1,    96,    97,    98,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,   149,   150,   151,   152,    -1,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,   174,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   191,   192,   193,    -1,
     195,    -1,    -1,   198,   199,    -1,    -1,     4,     5,    -1,
      -1,   206,    -1,   208,    -1,   210,   211,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    69,    70,    71,    -1,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    92,    93,    -1,    -1,    96,
      97,    98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,   149,   150,   151,   152,    -1,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,   192,   193,    -1,   195,    -1,
      -1,   198,   199,    -1,    -1,     4,     5,    -1,    -1,   206,
      -1,   208,    -1,   210,   211,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,
      32,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    -1,    59,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,
      69,    70,    71,    -1,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    91,    92,    93,    -1,    -1,    96,    97,    98,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,   121,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    59,    60,    -1,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,
     149,   150,   151,   152,    -1,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,    -1,
      -1,    -1,    -1,    18,    -1,   174,    -1,    -1,    -1,    24,
      -1,    -1,   184,    -1,    -1,    30,   188,    -1,   190,   191,
      -1,    -1,   191,   192,   193,    40,   195,   121,    -1,   198,
     199,   125,    -1,    48,   206,    -1,    -1,   206,    -1,   208,
     212,   210,   211,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,    -1,    -1,    -1,
     184,    -1,    -1,    -1,   188,    -1,    -1,   191,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    18,   206,    -1,    -1,    -1,    -1,    24,   212,    -1,
      -1,    -1,    -1,    30,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   153,    -1,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,   176,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   210,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,   164,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,
     116,   117,    -1,   210,   120,   121,   122,   123,    -1,   125,
     126,   127,   128,    -1,    -1,    20,    21,   133,    -1,   135,
     136,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,   140,
      -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,
     206,    -1,    -1,    -1,   210,   211,    -1,    -1,    -1,   210,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,    20,    21,    -1,   133,   134,
     135,   136,   137,   138,    -1,    -1,   141,   142,   143,   144,
     145,   146,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,   184,
      -1,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,    -1,    -1,    -1,   210,   211,    -1,    -1,    -1,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    20,    21,    -1,    -1,   133,   134,
     135,   136,   137,   138,    -1,    -1,   141,   142,   143,   144,
     145,   146,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,   184,
      -1,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,    -1,    -1,    -1,   210,   211,    -1,    -1,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,
     136,   137,   138,    -1,    -1,   141,   142,   143,   144,   145,
     146,   147,    -1,    -1,    -1,   113,   114,   115,   116,   117,
      20,    21,   120,   121,   122,   123,    -1,   125,   126,   127,
     128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,
     176,    -1,    -1,   141,   142,   143,    -1,   183,   184,   147,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,
     206,    -1,    -1,    -1,   210,   211,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,   206,    -1,
      -1,   209,    -1,   113,   114,   115,   116,   117,    20,    21,
     120,   121,   122,   123,    -1,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,
      -1,   141,   142,   143,    -1,    -1,    -1,   147,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   184,    -1,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   205,   206,    -1,    -1,   209,
      -1,   113,   114,   115,   116,   117,    -1,    -1,   120,   121,
     122,   123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,
     142,   143,    -1,    -1,    -1,   147,   113,   114,   115,   116,
     117,    20,    21,   120,   121,   122,   123,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,
      -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,
     147,    -1,   184,    -1,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,   206,    -1,    -1,   209,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,   206,
      -1,    -1,   209,    -1,   113,   114,   115,   116,   117,    20,
      21,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,
      -1,    -1,   141,   142,   143,    -1,    -1,    -1,   147,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   184,    -1,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   205,   206,    -1,    -1,
     209,    -1,   113,   114,   115,   116,   117,    -1,    -1,   120,
     121,   122,   123,    -1,   125,   126,   127,   128,    -1,    -1,
      -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,
     141,   142,   143,    -1,    -1,    -1,   147,   113,   114,   115,
     116,   117,    20,    21,   120,   121,   122,   123,    -1,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,
     136,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,
      -1,   147,    -1,   184,    -1,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   205,   206,    -1,    -1,   209,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,
     206,    -1,    -1,   209,    -1,   113,   114,   115,   116,   117,
      20,    21,   120,   121,   122,   123,    -1,   125,   126,   127,
     128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,
      -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,   147,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,   206,    -1,
      -1,   209,    -1,   113,   114,   115,   116,   117,    -1,    -1,
     120,   121,   122,   123,    -1,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,
      -1,   141,   142,   143,    -1,    -1,    -1,   147,   113,   114,
     115,   116,   117,    20,    21,   120,   121,   122,   123,    -1,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,
     135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,
      -1,    -1,   147,    -1,   184,    -1,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   205,   206,    -1,    -1,   209,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,
      -1,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,    -1,    -1,   209,    -1,   113,   114,   115,   116,
     117,    20,    21,   120,   121,   122,   123,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,
      -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,
     147,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,   206,
      -1,    -1,   209,    -1,   113,   114,   115,   116,   117,    -1,
      -1,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,
      -1,    -1,   141,   142,   143,    -1,    -1,    -1,   147,   113,
     114,   115,   116,   117,    20,    21,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,
      -1,    -1,    -1,   147,    -1,   184,    -1,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   205,   206,    -1,    -1,
     209,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     184,    -1,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   205,   206,    -1,    -1,   209,    -1,   113,   114,   115,
     116,   117,    20,    21,   120,   121,   122,   123,    -1,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,
     136,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,
      -1,   147,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,
     206,    -1,    -1,   209,    -1,   113,   114,   115,   116,   117,
      -1,    -1,   120,   121,   122,   123,    -1,   125,   126,   127,
     128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,
      -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,   147,
     113,   114,   115,   116,   117,    20,    21,   120,   121,   122,
     123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
     133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,
     143,    -1,    -1,    -1,   147,    -1,   184,    -1,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,   206,    -1,
      -1,   209,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   184,    -1,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   205,   206,    -1,    -1,   209,    -1,   113,   114,
     115,   116,   117,    20,    21,   120,   121,   122,   123,    -1,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,
     135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,
      -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,
      -1,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,    -1,    -1,   209,    -1,   113,   114,   115,   116,
     117,    -1,    -1,   120,   121,   122,   123,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,
      -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,
     147,   113,   114,   115,   116,   117,    20,    21,   120,   121,
     122,   123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,
     142,   143,    -1,    -1,    -1,   147,    -1,   184,    -1,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,   206,
      -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   184,    -1,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,   206,    -1,    -1,   209,    -1,   113,
     114,   115,   116,   117,    20,    21,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,
      -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     184,    -1,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   205,   206,    -1,    -1,   209,    -1,   113,   114,   115,
     116,   117,    -1,    -1,   120,   121,   122,   123,    -1,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,
     136,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,
      -1,   147,   113,   114,   115,   116,   117,    20,    21,   120,
     121,   122,   123,    -1,   125,   126,   127,   128,    -1,    -1,
      -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,
     141,   142,   143,    -1,    -1,    -1,   147,    -1,   184,    -1,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,
     206,    -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   184,    -1,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   205,   206,    -1,    -1,   209,    -1,
     113,   114,   115,   116,   117,    20,    21,   120,   121,   122,
     123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
     133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,
     143,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   184,    -1,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   205,   206,    -1,    -1,   209,    -1,   113,   114,
     115,   116,   117,    -1,    -1,   120,   121,   122,   123,    -1,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,
     135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,
      -1,    -1,   147,   113,   114,   115,   116,   117,    20,    21,
     120,   121,   122,   123,    -1,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,
      -1,   141,   142,   143,    -1,    -1,    -1,   147,    -1,   184,
      -1,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,    -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   184,    -1,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   205,   206,    -1,    -1,   209,
      -1,   113,   114,   115,   116,   117,    20,    21,   120,   121,
     122,   123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,
     142,   143,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   184,    -1,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,   206,    -1,    -1,   209,    -1,   113,
     114,   115,   116,   117,    -1,    -1,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,
      -1,    -1,    -1,   147,   113,   114,   115,   116,   117,    20,
      21,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,
      -1,    -1,   141,   142,   143,    -1,    -1,    -1,   147,    -1,
     184,    -1,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   205,   206,    -1,    -1,   209,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   184,    -1,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   205,   206,    -1,    -1,
     209,    -1,   113,   114,   115,   116,   117,    20,    21,   120,
     121,   122,   123,    -1,   125,   126,   127,   128,    -1,    -1,
      -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,
     141,   142,   143,    -1,    -1,    -1,   147,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   184,    -1,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   205,   206,    -1,    -1,   209,    -1,
     113,   114,   115,   116,   117,    -1,    -1,   120,   121,   122,
     123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
     133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,
     143,    -1,    -1,    -1,   147,   113,   114,   115,   116,   117,
      20,    21,   120,   121,   122,   123,    -1,   125,   126,   127,
     128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,
      -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,   147,
      -1,   184,    -1,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   205,   206,   207,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,   206,   207,
      -1,    -1,    -1,   113,   114,   115,   116,   117,    20,    21,
     120,   121,   122,   123,    -1,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,
      -1,   141,   142,   143,    -1,    -1,    -1,   147,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   184,    -1,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   205,   206,   207,    -1,    -1,
      -1,   113,   114,   115,   116,   117,    -1,    -1,   120,   121,
     122,   123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,
     142,   143,    -1,    -1,    -1,   147,   113,   114,   115,   116,
     117,    20,    21,   120,   121,   122,   123,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,
      -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,
     147,    -1,   184,    -1,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,   206,   207,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,   206,
     207,    -1,    -1,    -1,   113,   114,   115,   116,   117,    -1,
      -1,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
      20,    21,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,
      -1,    -1,   141,   142,   143,    -1,    -1,    37,   147,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   184,    -1,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   205,   206,   207,    -1,
      -1,    -1,    -1,    -1,    -1,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,    -1,    -1,   125,   126,   127,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,    -1,
      -1,   141,   142,   143,   144,   145,   146,   147,   113,   114,
     115,   116,   117,    20,    21,   120,   121,   122,   123,    -1,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,
     135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,
      -1,    -1,   147,    -1,   184,    -1,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,    -1,    -1,   198,   199,
      -1,    -1,    -1,    -1,    -1,   205,   206,    -1,    -1,   174,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,
      -1,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,    -1,    -1,    -1,    -1,   113,   114,   115,   116,
     117,    20,    21,   120,   121,   122,   123,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,
      -1,    -1,   139,    -1,   141,   142,   143,    -1,    -1,    -1,
     147,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,   206,
      -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,    -1,
      -1,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,
      -1,    -1,   141,   142,   143,    -1,    -1,    -1,   147,   113,
     114,   115,   116,   117,    20,    21,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,
      -1,    -1,    -1,   147,    -1,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   205,   206,    -1,    -1,
      -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     184,    -1,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   205,   206,    -1,    -1,    -1,    -1,   113,   114,   115,
     116,   117,    20,    21,   120,   121,   122,   123,    -1,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,
     136,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,
      -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,
     206,    -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,
      -1,    -1,   120,   121,   122,   123,    -1,   125,   126,   127,
     128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,
      -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,   147,
      -1,   113,   114,   115,   116,   117,    20,    21,   120,   121,
     122,   123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,   135,   136,    -1,    -1,    -1,   176,   141,
     142,   143,    -1,    -1,    -1,   147,   184,    -1,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,   206,    -1,
      -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   184,    -1,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,   206,    -1,    -1,    -1,    -1,   113,
     114,   115,   116,   117,    20,    21,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,
      -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     184,    -1,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   205,   206,    -1,    -1,    -1,    -1,   113,   114,   115,
     116,   117,    -1,    -1,   120,   121,   122,   123,    -1,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,
     136,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,
      -1,   147,   113,   114,   115,   116,   117,    20,    21,   120,
     121,   122,   123,    -1,   125,   126,   127,   128,    -1,    -1,
      -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,
     141,   142,   143,    -1,    -1,    -1,   147,    -1,   184,    -1,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   205,
     206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   184,    -1,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   205,   206,    -1,    -1,    -1,    -1,
     113,   114,   115,   116,   117,    -1,    -1,   120,   121,   122,
     123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
     133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,    -1,
      -1,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
      -1,    -1,    20,    21,   133,    -1,   135,   136,    -1,    -1,
      -1,   184,   141,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,   205,   206,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,    -1,    -1,    -1,
      -1,   113,   114,   115,   116,   117,   205,   206,   120,   121,
     122,   123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,   135,   136,   113,   114,   115,   116,   117,
      -1,    -1,   120,   121,   122,   123,    -1,   125,   126,   127,
     128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,   113,
     114,   115,   116,   117,    -1,    -1,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,   135,   136,    -1,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    18,   205,   206,    -1,    -1,    -1,    -1,    -1,
     188,   189,   190,   191,   192,   193,   194,   195,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,   206,    -1,
      -1,    -1,    -1,    18,    -1,   189,   190,   191,   192,   193,
     194,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   205,   206,    69,    70,    71,    -1,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    -1,    91,    92,    93,    -1,    -1,
      96,    97,    98,    99,    69,    70,    71,    -1,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    91,    92,    93,    -1,
      -1,    96,    97,    98,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    34,    -1,    -1,    -1,    69,    70,    71,   164,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    91,    92,
      93,    -1,    -1,    96,    97,    98,    99,    69,    -1,    71,
      -1,    73,    74,    75,    76,    77,    -1,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    91,
      92,    93,    -1,    -1,    96,    97,    98,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   164
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   214,     0,     6,    29,    31,    33,    39,    49,    55,
      78,   100,   101,   176,   195,   206,   215,   218,   224,   226,
     227,   232,   261,   265,   298,   376,   383,   387,   398,   442,
     447,   452,    18,    19,   164,   254,   255,   256,   157,   233,
     234,   164,   165,   166,   195,   228,   229,   230,   174,   384,
     164,   210,   217,    56,    62,   379,   379,   379,   140,   164,
     284,    33,    62,   133,   199,   208,   257,   258,   259,   260,
     284,   176,     4,     5,     7,    35,   395,    61,   374,   183,
     182,   185,   182,   229,    21,    56,   194,   205,   231,   379,
     380,   382,   380,   374,   453,   443,   448,   164,   140,   225,
     259,   259,   259,   208,   141,   142,   143,   182,   207,    56,
      62,   266,   268,    56,    62,   385,    56,    62,   396,    56,
      62,   375,    14,    15,   157,   162,   164,   167,   208,   220,
     255,   157,   234,   164,   228,   228,   164,   176,   175,   380,
     176,    56,    62,   216,   164,   164,   164,   164,   168,   223,
     209,   256,   259,   259,   259,   259,   269,   164,   386,   399,
     174,   377,   168,   169,   219,    14,    15,   157,   162,   164,
     220,   252,   253,   231,   381,   176,   454,   444,   449,   168,
     209,    34,    69,    71,    73,    74,    75,    76,    77,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      91,    92,    93,    96,    97,    98,    99,   115,   116,   164,
     264,   267,   174,   185,   104,   393,   394,   372,   206,   348,
     168,   169,   170,   182,   209,   183,   174,   174,   174,    20,
      21,    37,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     125,   126,   127,   134,   135,   136,   137,   138,   141,   142,
     143,   144,   145,   146,   147,   184,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   198,   199,   205,   206,
      34,    34,   208,   262,   174,   270,   388,    73,    77,    96,
      97,    98,    99,   403,   164,   400,   175,   373,   206,   149,
     164,   370,   371,   252,    18,    24,    30,    40,    48,    63,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   153,   210,   284,   402,   404,   405,   408,
     413,   441,   455,   445,   450,   164,   164,   164,   207,    21,
     164,   207,   152,   209,   348,   358,   359,   185,   263,   273,
     378,   174,   185,   392,   174,   397,   348,   256,   208,    42,
     182,   185,   188,   369,   189,   189,   189,   208,   189,   189,
     208,   189,   189,   189,   189,   189,   189,   208,   284,    32,
      59,    60,   121,   125,   184,   188,   191,   206,   212,   412,
     186,   407,   362,   365,   164,   134,   208,     6,    49,   297,
     176,   209,   441,     1,     4,     5,     8,     9,    10,    12,
      14,    15,    16,    17,    18,    24,    25,    26,    27,    28,
      30,    37,    38,    41,    43,    44,    47,    50,    51,    53,
      54,    57,    58,    64,    67,    68,    78,   100,   101,   102,
     103,   115,   116,   130,   131,   132,   148,   149,   150,   151,
     152,   154,   156,   157,   158,   159,   160,   161,   162,   163,
     165,   166,   167,   174,   175,   176,   191,   192,   193,   198,
     199,   206,   208,   210,   211,   222,   224,   235,   236,   239,
     242,   243,   245,   247,   248,   249,   250,   271,   272,   274,
     278,   283,   284,   285,   289,   290,   291,   292,   293,   294,
     295,   296,   298,   302,   303,   310,   313,   316,   321,   324,
     325,   327,   328,   329,   331,   336,   339,   340,   347,   402,
     457,   460,   470,   473,   479,   482,   164,   176,   389,   390,
     284,   354,   371,   207,    64,   165,   278,   340,   164,   164,
     413,   124,   134,   183,   368,   414,   419,   421,   340,   423,
     417,   164,   409,   425,   427,   429,   431,   433,   435,   437,
     439,   340,   189,   208,    32,   188,    32,   188,   206,   212,
     207,   340,   206,   212,   413,   164,   176,   456,   164,   176,
     360,   441,   446,   164,   176,   363,   451,   340,   149,   164,
     367,   401,   358,   189,   189,   340,   246,   189,   286,   404,
     457,   208,   189,   189,   208,   124,   283,   314,   325,   340,
     270,   189,   208,    60,   340,   208,   340,   164,   189,   189,
     208,   176,   189,   157,    57,   340,   208,   270,   189,   208,
     189,   189,   208,   189,   189,   124,   283,   340,   340,   340,
     211,   270,   316,   320,   320,   320,   208,   208,   208,   208,
     208,   208,   189,   340,   189,   221,    12,   340,   469,   478,
      36,   340,   340,   340,   340,   340,    12,   314,   340,   314,
     211,   176,   176,   340,     9,   316,   322,   164,   208,   176,
     176,   176,   176,   176,    65,   299,   261,   129,   176,    20,
      21,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   133,   134,   135,   136,   137,
     138,   141,   142,   143,   144,   145,   146,   147,   183,   184,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     205,   206,   322,   208,   176,   183,   175,   378,   175,   206,
     261,   355,   207,   209,    42,   176,   368,   283,   340,   441,
     441,   411,   441,   209,   441,   441,   209,   164,   406,   441,
     262,   441,   262,   441,   262,   360,   361,   363,   364,   209,
     416,   275,   314,   207,   207,   340,   176,   175,   185,   176,
     175,   185,   176,   175,   209,   208,    42,   124,   182,   183,
     185,   188,   366,   461,   463,   270,   401,   287,   208,   284,
     311,   474,   314,   283,   314,   182,   105,   106,   107,   108,
     109,   110,   111,   112,   118,   119,   124,   137,   138,   144,
     145,   146,   183,    13,   413,   478,   340,   340,   270,   183,
     304,   306,   340,   308,   185,   157,   340,   476,   314,   467,
     471,   314,   465,   413,   283,   340,   211,   261,   340,   340,
     340,   340,   340,   340,   279,   270,   281,   170,   171,   219,
     401,   139,   182,   481,   174,   401,   182,   481,   481,   149,
     154,   189,   284,   330,   270,   244,   323,   155,   164,   206,
     209,   314,   458,   459,   208,   301,   371,   206,   319,   320,
      18,   155,   164,   402,    18,   155,   164,   402,   130,   131,
     132,   271,   326,   340,   326,   340,   326,   340,   326,   340,
     326,   340,   326,   340,   326,   340,   326,   340,   340,   340,
     340,   326,   340,   326,   340,   340,   340,   340,   164,   326,
     340,   340,   155,   164,   340,   340,   340,   402,   340,   340,
     340,   326,   340,   326,   340,   340,   340,   340,   326,   340,
     326,   340,   326,   340,   340,   326,   340,    21,   340,   340,
     340,   340,   340,   340,   340,   340,   340,   340,   340,   126,
     127,   155,   164,   205,   206,   337,   340,   209,   314,   340,
     391,   260,     7,   348,   353,   164,   283,   340,   176,   190,
     190,   190,   176,   190,   190,   176,   190,   190,   263,   190,
     263,   190,   263,   176,   190,   176,   190,   277,   441,   209,
     207,   441,   441,   340,   164,   164,   441,   283,   340,   413,
     413,    19,   441,   209,   314,   459,   189,   340,   441,   413,
     481,   340,   340,   340,   340,   340,   340,   340,   340,   340,
     340,   340,   340,   340,   340,   340,   340,   340,   340,   270,
     176,   190,   481,   209,   251,   413,   413,   209,   413,   209,
     413,   481,   413,   413,   481,   413,   190,   319,   209,   209,
     209,   209,   209,   209,   277,   277,   340,    19,   340,   469,
     175,   273,    19,   207,   209,   208,   208,   332,   334,    11,
      22,    23,   237,   238,   340,   261,   208,   134,   366,   458,
     182,   209,   182,   209,   164,   300,   206,   262,   189,   208,
     189,   208,   320,   320,   320,   208,   208,   207,    18,   155,
     164,   402,   185,   155,   164,   340,   208,   208,   155,   164,
     340,     1,   207,   209,   176,   175,   207,    56,    62,   351,
      66,   352,   176,   176,   415,   420,   422,   441,   424,   418,
     164,   410,   426,   190,   430,   190,   434,   190,   360,   438,
     363,   440,   176,   190,   209,    42,   366,   190,   190,   314,
     190,   209,   209,   164,   209,   190,   190,   209,   413,   208,
     209,   340,   190,   190,   190,   190,   209,   190,   190,   209,
     190,   319,   262,   208,   190,   190,   172,   314,   175,   314,
     340,   340,   413,   262,   270,   340,    11,   240,   319,   340,
     340,   340,   207,   206,   155,   164,   182,   209,   124,   134,
     183,   188,   317,   318,   263,   341,   340,   343,   340,   209,
     314,   340,   189,   208,   340,   208,   207,   340,   209,   314,
     208,   207,   338,   176,    46,   352,    45,   104,   349,   428,
     432,   436,   441,   208,   164,   340,   462,   464,   270,   288,
     176,   190,   312,   475,   190,   478,   305,   307,   309,   477,
     468,   472,   466,   208,   263,   209,   314,   280,   282,   176,
     176,   209,   209,   190,   263,   270,   241,   176,   262,   209,
     209,   458,   208,   134,   366,   164,   185,   369,   164,   164,
     164,   164,   182,   207,   139,   270,   315,   413,   209,   441,
     209,   209,   209,   345,   340,   340,   209,   209,   340,    32,
     350,   349,   351,   275,   208,   208,   164,   340,   208,   208,
     481,   340,   340,   340,   208,   208,   208,   209,   340,   315,
     209,   340,   208,   276,   469,   340,   333,   190,   238,    25,
     103,   242,   290,   291,   292,   294,   340,   263,   134,   366,
     207,   340,   340,   340,   413,   368,   317,   207,   124,   340,
     190,   190,   441,   209,   209,   209,   356,   350,   367,   209,
     164,   459,   459,   190,   209,   314,   478,   209,   314,   459,
     459,   209,   275,   176,   480,   480,   330,   335,   124,   124,
     340,   270,   340,   340,   209,   209,   368,   340,   340,   342,
     344,   190,   267,   357,   209,   209,   340,   481,   481,   481,
     209,   209,   209,    52,   175,   207,   330,   340,   134,   366,
     340,   176,   346,   176,   267,   209,   209,   209,   209,   340,
     340,   340,   176,   270
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   213,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   215,   216,   216,
     216,   217,   217,   218,   219,   219,   219,   219,   220,   221,
     221,   221,   222,   223,   223,   225,   224,   226,   227,   228,
     228,   228,   229,   229,   229,   229,   230,   230,   231,   231,
     232,   233,   233,   234,   234,   235,   236,   236,   237,   237,
     238,   238,   238,   239,   239,   240,   241,   240,   242,   242,
     242,   242,   242,   243,   244,   243,   246,   245,   247,   248,
     249,   251,   250,   252,   252,   252,   252,   252,   252,   253,
     253,   254,   254,   254,   255,   255,   255,   255,   255,   255,
     255,   255,   256,   256,   257,   257,   257,   258,   258,   259,
     259,   259,   259,   259,   259,   259,   260,   260,   261,   261,
     262,   262,   262,   263,   263,   264,   264,   264,   264,   264,
     264,   264,   264,   264,   264,   264,   264,   264,   264,   264,
     264,   264,   264,   264,   264,   264,   264,   264,   264,   264,
     264,   264,   264,   264,   264,   264,   264,   264,   264,   264,
     264,   264,   264,   264,   264,   264,   264,   264,   264,   264,
     264,   264,   264,   264,   264,   264,   264,   264,   264,   264,
     264,   264,   264,   264,   264,   264,   264,   264,   264,   264,
     264,   264,   264,   264,   264,   264,   264,   264,   264,   264,
     264,   264,   264,   264,   264,   264,   264,   264,   264,   264,
     265,   266,   266,   266,   267,   269,   268,   270,   270,   271,
     271,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   273,   273,   273,   274,   275,   275,   276,   276,
     277,   277,   279,   280,   278,   281,   282,   278,   283,   283,
     283,   283,   283,   284,   284,   284,   285,   285,   287,   288,
     286,   286,   289,   289,   289,   289,   289,   290,   291,   292,
     292,   292,   293,   293,   293,   294,   294,   295,   295,   295,
     296,   297,   297,   297,   298,   298,   299,   299,   300,   300,
     301,   301,   302,   302,   304,   305,   303,   306,   307,   303,
     308,   309,   303,   311,   312,   310,   313,   313,   313,   314,
     314,   315,   315,   315,   316,   316,   316,   317,   317,   317,
     317,   318,   318,   319,   319,   320,   320,   321,   323,   322,
     324,   324,   324,   324,   324,   324,   324,   325,   325,   325,
     325,   325,   325,   325,   325,   325,   325,   325,   325,   325,
     325,   325,   325,   325,   325,   325,   326,   326,   326,   326,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   328,   328,   329,
     329,   330,   330,   331,   332,   333,   331,   334,   335,   331,
     336,   336,   336,   336,   337,   338,   336,   339,   339,   339,
     339,   339,   340,   340,   340,   340,   340,   340,   340,   340,
     340,   340,   340,   340,   340,   340,   340,   340,   340,   340,
     340,   340,   340,   340,   340,   340,   340,   340,   340,   340,
     340,   340,   340,   340,   340,   340,   340,   340,   340,   340,
     340,   340,   340,   340,   340,   340,   340,   340,   340,   340,
     340,   340,   340,   340,   340,   340,   340,   340,   341,   342,
     340,   340,   340,   340,   343,   344,   340,   340,   340,   345,
     346,   340,   340,   340,   340,   340,   340,   340,   340,   340,
     340,   340,   340,   340,   340,   340,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   348,   348,   349,   349,   349,   350,   350,   351,
     351,   351,   352,   352,   353,   354,   355,   354,   356,   354,
     357,   354,   354,   358,   358,   359,   359,   360,   360,   361,
     361,   362,   362,   362,   363,   364,   364,   365,   365,   365,
     366,   366,   367,   367,   367,   367,   367,   367,   368,   368,
     368,   369,   369,   370,   370,   370,   370,   370,   371,   371,
     371,   371,   371,   372,   373,   372,   374,   374,   375,   375,
     375,   376,   377,   376,   378,   378,   378,   378,   379,   379,
     379,   381,   380,   382,   382,   383,   384,   383,   385,   385,
     385,   386,   388,   389,   387,   390,   391,   387,   392,   392,
     393,   393,   394,   395,   395,   396,   396,   396,   397,   397,
     399,   400,   398,   401,   401,   401,   401,   401,   402,   402,
     402,   402,   402,   402,   402,   402,   402,   402,   402,   402,
     402,   402,   402,   402,   402,   402,   402,   402,   402,   402,
     402,   402,   402,   402,   402,   403,   403,   403,   403,   403,
     403,   404,   405,   405,   405,   406,   406,   407,   407,   407,
     409,   410,   408,   411,   411,   412,   412,   413,   413,   413,
     413,   413,   413,   414,   415,   413,   413,   413,   416,   413,
     413,   413,   413,   413,   413,   413,   413,   413,   413,   413,
     413,   413,   417,   418,   413,   413,   419,   420,   413,   421,
     422,   413,   423,   424,   413,   413,   425,   426,   413,   427,
     428,   413,   413,   429,   430,   413,   431,   432,   413,   413,
     433,   434,   413,   435,   436,   413,   437,   438,   413,   439,
     440,   413,   441,   441,   441,   443,   444,   445,   446,   442,
     448,   449,   450,   451,   447,   453,   454,   455,   456,   452,
     457,   457,   457,   457,   457,   458,   458,   458,   458,   458,
     458,   458,   458,   459,   461,   462,   460,   463,   464,   460,
     465,   466,   460,   467,   468,   460,   469,   469,   470,   471,
     472,   470,   473,   473,   474,   475,   473,   473,   476,   477,
     473,   478,   478,   479,   479,   479,   479,   480,   480,   481,
     481,   482,   482
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
       3,     0,     1,     1,     3,     0,     4,     3,     7,     2,
       2,     1,     1,     1,     1,     2,     2,     1,     1,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     1,     2,
       2,     2,     0,     2,     2,     3,     0,     1,     0,     3,
       1,     3,     0,     0,     7,     0,     0,     7,     3,     2,
       2,     2,     1,     1,     3,     2,     2,     3,     0,     0,
       5,     1,     2,     4,     5,     5,     2,     1,     1,     1,
       2,     3,     2,     2,     3,     2,     3,     2,     2,     3,
       4,     1,     1,     0,     1,     1,     1,     0,     1,     3,
       8,     7,     3,     3,     0,     0,     7,     0,     0,     7,
       0,     0,     7,     0,     0,     6,     5,     8,    10,     1,
       3,     1,     2,     3,     1,     1,     2,     2,     2,     2,
       2,     1,     3,     0,     5,     1,     6,     6,     0,     7,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     6,     8,     5,
       6,     1,     4,     3,     0,     0,     8,     0,     0,     9,
       3,     4,     5,     6,     0,     0,     5,     3,     4,     4,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     2,     4,     4,     5,     4,     5,     3,     4,     1,
       1,     2,     4,     4,     7,     8,     3,     5,     0,     0,
       8,     3,     3,     3,     0,     0,     8,     3,     4,     0,
       0,     9,     4,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     2,     4,     1,     4,     4,     4,     4,
       4,     1,     6,     7,     6,     6,     7,     7,     6,     7,
       6,     6,     0,     5,     0,     1,     1,     0,     1,     0,
       1,     1,     0,     1,     5,     0,     0,     4,     0,     9,
       0,    10,     5,     3,     4,     1,     3,     1,     3,     1,
       3,     0,     2,     3,     3,     1,     3,     0,     2,     3,
       1,     1,     1,     2,     3,     5,     3,     3,     1,     1,
       1,     0,     1,     1,     4,     3,     3,     5,     4,     6,
       5,     5,     4,     0,     0,     4,     0,     1,     0,     1,
       1,     6,     0,     6,     0,     2,     3,     5,     0,     1,
       1,     0,     5,     2,     3,     4,     0,     4,     0,     1,
       1,     1,     0,     0,     9,     0,     0,    11,     0,     2,
       0,     1,     3,     1,     1,     0,     1,     1,     0,     3,
       0,     0,     7,     1,     4,     3,     3,     5,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     4,     1,     3,     0,     2,     3,
       0,     0,     6,     1,     3,     3,     4,     1,     1,     1,
       1,     2,     3,     0,     0,     6,     4,     5,     0,     9,
       4,     2,     2,     3,     2,     3,     2,     2,     3,     3,
       3,     2,     0,     0,     6,     2,     0,     0,     6,     0,
       0,     6,     0,     0,     6,     1,     0,     0,     6,     0,
       0,     7,     1,     0,     0,     6,     0,     0,     7,     1,
       0,     0,     6,     0,     0,     7,     0,     0,     6,     0,
       0,     6,     1,     3,     3,     0,     0,     0,     0,    10,
       0,     0,     0,     0,    10,     0,     0,     0,     0,    10,
       1,     1,     1,     1,     1,     3,     3,     5,     5,     6,
       6,     8,     8,     1,     0,     0,     9,     0,     0,     9,
       0,     0,     9,     0,     0,     6,     3,     1,     5,     0,
       0,     9,     4,     5,     0,     0,    10,     5,     0,     0,
      10,     1,     3,     4,     5,     8,    10,     0,     3,     0,
       1,     9,     9
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
        for ( auto & opt : *(yyvsp[0].aaList) ) {
            if ( opt.name=="indenting" && opt.type==Type::tInt ) {
                if (opt.iValue != 0 && opt.iValue != 2 && opt.iValue != 4 && opt.iValue != 8) { //this is error
                    yyextra->das_tab_size = yyextra->das_def_tab_size;
                } else {
                    yyextra->das_tab_size = opt.iValue ? opt.iValue : yyextra->das_def_tab_size;//0 is default
                }
                yyextra->g_FileAccessStack.back()->tabSize = yyextra->das_tab_size;
            } else if ( opt.name=="gen2_make_syntax" && opt.type==Type::tBool ) {
                yyextra->das_gen2_make_syntax = opt.bValue;
            }
        }
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

  case 75: /* expression_if_then_else: expression_if_one_liner "if" $@3 expr expression_else_one_liner "end of expression"  */
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

  case 108: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 109: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 110: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 111: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 112: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 113: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 217: /* expression_block: "begin of code block" expressions "end of code block"  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 218: /* expression_block: "begin of code block" expressions "end of code block" "finally" "begin of code block" expressions "end of code block"  */
                                                                                          {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 219: /* expr_call_pipe: expr expr_full_block_assumed_piped  */
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

  case 220: /* expr_call_pipe: expression_keyword expr_full_block_assumed_piped  */
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

  case 221: /* expression_any: "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 222: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 223: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 224: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 225: /* expression_any: expr_assign "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 226: /* expression_any: expression_delete "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 227: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 228: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 229: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 230: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 231: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 232: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 233: /* expression_any: expression_break "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 234: /* expression_any: expression_continue "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 235: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 236: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 237: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 238: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 239: /* expression_any: expression_label "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 240: /* expression_any: expression_goto "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 241: /* expression_any: "pass" "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 242: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 243: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 244: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 245: /* expr_keyword: "keyword" expr expression_block  */
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

  case 246: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 247: /* optional_expr_list: expr_list  */
                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 248: /* optional_expr_list_in_braces: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 249: /* optional_expr_list_in_braces: '(' optional_expr_list ')'  */
                                              { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 250: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 251: /* type_declaration_no_options_list: type_declaration_no_options_list "end of expression" type_declaration  */
                                                                           {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 252: /* $@7: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 253: /* $@8: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 254: /* expression_keyword: "keyword" '<' $@7 type_declaration_no_options_list '>' $@8 expr  */
                                                                                                                                                     {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 255: /* $@9: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 256: /* $@10: %empty  */
                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 257: /* expression_keyword: "type function" '<' $@9 type_declaration_no_options_list '>' $@10 optional_expr_list_in_braces  */
                                                                                                                                                                                   {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 258: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 259: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 260: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 261: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 262: /* expr_pipe: expr_call_pipe  */
                             {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 263: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 264: /* name_in_namespace: "name" "::" "name"  */
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

  case 265: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 266: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 267: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 268: /* $@11: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 269: /* $@12: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 270: /* new_type_declaration: '<' $@11 type_declaration '>' $@12  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 271: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 272: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 273: /* expr_new: "new" new_type_declaration '(' ')'  */
                                                               {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
    break;

  case 274: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 275: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 276: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 277: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 278: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 279: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 280: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 281: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 282: /* expression_return: expression_return_no_pipe "end of expression"  */
                                              {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 283: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 284: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 285: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 286: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 287: /* expression_yield: expression_yield_no_pipe "end of expression"  */
                                             {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 288: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 289: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 290: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 291: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 292: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 293: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 294: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 295: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 296: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 297: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 298: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 299: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 300: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 301: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr "end of expression"  */
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

  case 302: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 303: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 304: /* $@13: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 305: /* $@14: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 306: /* expr_cast: "cast" '<' $@13 type_declaration_no_options '>' $@14 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 307: /* $@15: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 308: /* $@16: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 309: /* expr_cast: "upcast" '<' $@15 type_declaration_no_options '>' $@16 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 310: /* $@17: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 311: /* $@18: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 312: /* expr_cast: "reinterpret" '<' $@17 type_declaration_no_options '>' $@18 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 313: /* $@19: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 314: /* $@20: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 315: /* expr_type_decl: "type" '<' $@19 type_declaration '>' $@20  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 316: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 317: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 318: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" "end of expression" "name" '>' expr ')'  */
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

  case 319: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 320: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 321: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 322: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 323: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 324: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 325: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 326: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 327: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 328: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 329: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 330: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 331: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 332: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 333: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 334: /* optional_capture_list: '[' '[' capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 335: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 336: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 337: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 338: /* $@21: %empty  */
                             {  yyextra->das_need_oxford_comma = false; }
    break;

  case 339: /* expr_full_block_assumed_piped: block_or_lambda $@21 optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 340: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 341: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 342: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 343: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 344: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 345: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 346: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 347: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 348: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 349: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 350: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 351: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 352: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 353: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 354: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 355: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 356: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 357: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 358: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 359: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 360: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 361: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 362: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 363: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 364: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 365: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 366: /* expr_assign_pipe_right: "@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 367: /* expr_assign_pipe_right: "@@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 368: /* expr_assign_pipe_right: "$ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 369: /* expr_assign_pipe_right: expr_call_pipe  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 370: /* expr_assign_pipe: expr '=' expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 371: /* expr_assign_pipe: expr "<-" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 372: /* expr_assign_pipe: expr "&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 373: /* expr_assign_pipe: expr "|=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 374: /* expr_assign_pipe: expr "^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 375: /* expr_assign_pipe: expr "&&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 376: /* expr_assign_pipe: expr "||=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 377: /* expr_assign_pipe: expr "^^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 378: /* expr_assign_pipe: expr "+=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 379: /* expr_assign_pipe: expr "-=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 380: /* expr_assign_pipe: expr "*=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 381: /* expr_assign_pipe: expr "/=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 382: /* expr_assign_pipe: expr "%=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 383: /* expr_assign_pipe: expr "<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 384: /* expr_assign_pipe: expr ">>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 385: /* expr_assign_pipe: expr "<<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 386: /* expr_assign_pipe: expr ">>>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 387: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 388: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 389: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 390: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 391: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 392: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 393: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 394: /* $@22: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 395: /* $@23: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 396: /* func_addr_expr: '@' '@' '<' $@22 type_declaration_no_options '>' $@23 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 397: /* $@24: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 398: /* $@25: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 399: /* func_addr_expr: '@' '@' '<' $@24 optional_function_argument_list optional_function_type '>' $@25 func_addr_name  */
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

  case 400: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 401: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 402: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 403: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 404: /* $@26: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 405: /* $@27: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 406: /* expr_field: expr '.' $@26 error $@27  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 407: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 408: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
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

  case 409: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 410: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 411: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 412: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 413: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 414: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 415: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 416: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 417: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 418: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 419: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 420: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 421: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 422: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 423: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 424: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 425: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 426: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 427: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 428: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 429: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 430: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 431: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 432: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 433: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 434: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 435: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 436: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 437: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 438: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 439: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 440: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 441: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 442: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 443: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 444: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 445: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 446: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 447: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 448: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 449: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 450: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 451: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 452: /* expr: '(' expr_list optional_comma ')'  */
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

  case 453: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 454: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 455: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 456: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 457: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 458: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 459: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 460: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 461: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 462: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 463: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 464: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 465: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 466: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 467: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 468: /* $@28: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 469: /* $@29: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 470: /* expr: expr "is" "type" '<' $@28 type_declaration_no_options '>' $@29  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 471: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 472: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 473: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 474: /* $@30: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 475: /* $@31: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 476: /* expr: expr "as" "type" '<' $@30 type_declaration '>' $@31  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 477: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 478: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 479: /* $@32: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 480: /* $@33: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 481: /* expr: expr '?' "as" "type" '<' $@32 type_declaration '>' $@33  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 482: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 483: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 484: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 485: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 486: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 487: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 488: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 489: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 490: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 491: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 492: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 493: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 494: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 495: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 496: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 497: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 498: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 499: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 500: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 501: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 502: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 503: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 504: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 505: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 506: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 507: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 508: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 509: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 510: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 511: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 512: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 513: /* optional_field_annotation: '[' '[' annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); }
    break;

  case 514: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 515: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 516: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 517: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 518: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 519: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 520: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 521: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 522: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 523: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 524: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 525: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 526: /* $@34: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 527: /* struct_variable_declaration_list: struct_variable_declaration_list $@34 structure_variable_declaration "end of expression"  */
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

  case 528: /* $@35: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 529: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@35 function_declaration_header "end of expression"  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 530: /* $@36: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 531: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@36 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 532: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' "end of expression"  */
                                                                                 {
        das2_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 533: /* function_argument_declaration: optional_field_annotation kwd_let_var_or_nothing variable_declaration  */
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

  case 534: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 535: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 536: /* function_argument_list: function_argument_list "end of expression" function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 537: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 538: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 539: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 540: /* tuple_type_list: tuple_type_list "end of expression" tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 541: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 542: /* tuple_alias_type_list: tuple_alias_type_list "end of expression"  */
                                      {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 543: /* tuple_alias_type_list: tuple_alias_type_list tuple_type "end of expression"  */
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

  case 544: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 545: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 546: /* variant_type_list: variant_type_list "end of expression" variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 547: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 548: /* variant_alias_type_list: variant_alias_type_list "end of expression"  */
                                        {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 549: /* variant_alias_type_list: variant_alias_type_list variant_type "end of expression"  */
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

  case 550: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 551: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 552: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 553: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 554: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 555: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 556: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 557: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 558: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 559: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 560: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 561: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 562: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 563: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 564: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 565: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 566: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 567: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 568: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options "end of expression"  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 569: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 570: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 571: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr "end of expression"  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 572: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 573: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 574: /* $@37: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 575: /* global_variable_declaration_list: global_variable_declaration_list $@37 optional_field_annotation let_variable_declaration  */
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

  case 576: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 577: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 578: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 579: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 580: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 581: /* global_let: kwd_let optional_shared optional_public_or_private_variable "begin of code block" global_variable_declaration_list "end of code block"  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 582: /* $@38: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 583: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@38 optional_field_annotation let_variable_declaration  */
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

  case 584: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 585: /* enum_list: enum_list "end of expression"  */
                          {
        (yyval.pEnum) = (yyvsp[-1].pEnum);
    }
    break;

  case 586: /* enum_list: enum_list "name" "end of expression"  */
                                     {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt(scanner,(yylsp[-1]))) ) {
            das2_yyerror(scanner,"enumeration already declared " + *(yyvsp[-1].s), tokAt(scanner,(yylsp[-1])),
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

  case 587: /* enum_list: enum_list "name" '=' expr "end of expression"  */
                                                     {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt(scanner,(yylsp[-3]))) ) {
            das2_yyerror(scanner,"enumeration value already declared " + *(yyvsp[-3].s), tokAt(scanner,(yylsp[-3])),
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

  case 588: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 589: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 590: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 591: /* $@39: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 592: /* single_alias: optional_public_or_private_alias "name" $@39 '=' type_declaration  */
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

  case 596: /* $@40: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 598: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 599: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 600: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 601: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 602: /* $@41: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 603: /* $@42: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 604: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name "begin of code block" $@41 enum_list $@42 "end of code block"  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-2].pEnum),Type::tInt);
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
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 607: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration "begin of code block" $@43 enum_list $@44 "end of code block"  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-2].pEnum),(yyvsp[-5].type));
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

  case 618: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 619: /* optional_struct_variable_declaration_list: "begin of code block" struct_variable_declaration_list "end of code block"  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 620: /* $@45: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 621: /* $@46: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 622: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@45 structure_name $@46 optional_struct_variable_declaration_list  */
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

  case 623: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 624: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 625: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 626: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 627: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 628: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 629: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 630: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 631: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 632: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 633: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 634: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 635: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 636: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 637: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 638: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 639: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 640: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 641: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 642: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 643: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 644: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 645: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 646: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 647: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 648: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 649: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 650: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 651: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 652: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 653: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 654: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 655: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 656: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 657: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 658: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 659: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 660: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 661: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 662: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 663: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 664: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 665: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 666: /* bitfield_bits: bitfield_bits "end of expression" "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 667: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 668: /* bitfield_alias_bits: bitfield_alias_bits "end of expression"  */
                                      {
        (yyval.pNameList) = (yyvsp[-1].pNameList);
    }
    break;

  case 669: /* bitfield_alias_bits: bitfield_alias_bits "name" "end of expression"  */
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

  case 670: /* $@47: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 671: /* $@48: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 672: /* bitfield_type_declaration: "bitfield" '<' $@47 bitfield_bits '>' $@48  */
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

  case 673: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 674: /* table_type_pair: type_declaration "end of expression" type_declaration  */
                                                                          {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 675: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 676: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 677: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 678: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 679: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 680: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 681: /* type_declaration_no_options: type_declaration_no_options dim_list  */
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

  case 682: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 683: /* $@49: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 684: /* $@50: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 685: /* type_declaration_no_options: "type" '<' $@49 type_declaration '>' $@50  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 686: /* type_declaration_no_options: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 687: /* type_declaration_no_options: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 688: /* $@51: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 689: /* type_declaration_no_options: '$' name_in_namespace '<' $@51 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 690: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 691: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 692: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 693: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 694: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 695: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 696: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 697: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 698: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 699: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 700: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 701: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 702: /* $@52: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 703: /* $@53: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 704: /* type_declaration_no_options: "smart_ptr" '<' $@52 type_declaration '>' $@53  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 705: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 706: /* $@54: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 707: /* $@55: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 708: /* type_declaration_no_options: "array" '<' $@54 type_declaration '>' $@55  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 709: /* $@56: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 710: /* $@57: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 711: /* type_declaration_no_options: "table" '<' $@56 table_type_pair '>' $@57  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 712: /* $@58: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 713: /* $@59: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 714: /* type_declaration_no_options: "iterator" '<' $@58 type_declaration '>' $@59  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 715: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 716: /* $@60: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 717: /* $@61: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 718: /* type_declaration_no_options: "block" '<' $@60 type_declaration '>' $@61  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 719: /* $@62: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 720: /* $@63: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 721: /* type_declaration_no_options: "block" '<' $@62 optional_function_argument_list optional_function_type '>' $@63  */
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

  case 722: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 723: /* $@64: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 724: /* $@65: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 725: /* type_declaration_no_options: "function" '<' $@64 type_declaration '>' $@65  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 726: /* $@66: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 727: /* $@67: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 728: /* type_declaration_no_options: "function" '<' $@66 optional_function_argument_list optional_function_type '>' $@67  */
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

  case 729: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 730: /* $@68: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 731: /* $@69: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 732: /* type_declaration_no_options: "lambda" '<' $@68 type_declaration '>' $@69  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 733: /* $@70: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 734: /* $@71: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 735: /* type_declaration_no_options: "lambda" '<' $@70 optional_function_argument_list optional_function_type '>' $@71  */
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

  case 736: /* $@72: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 737: /* $@73: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 738: /* type_declaration_no_options: "tuple" '<' $@72 tuple_type_list '>' $@73  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 739: /* $@74: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 740: /* $@75: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 741: /* type_declaration_no_options: "variant" '<' $@74 variant_type_list '>' $@75  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 742: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 743: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 744: /* type_declaration: type_declaration '|' '#'  */
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

  case 745: /* $@76: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 746: /* $@77: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 747: /* $@78: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 748: /* $@79: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 749: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@76 "name" $@77 "begin of code block" $@78 tuple_alias_type_list $@79 "end of code block"  */
          {
        auto vtype = make_smart<TypeDecl>(Type::tTuple);
        vtype->alias = *(yyvsp[-6].s);
        vtype->at = tokAt(scanner,(yylsp[-6]));
        vtype->isPrivateAlias = !(yyvsp[-8].b);
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

  case 750: /* $@80: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 751: /* $@81: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 752: /* $@82: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 753: /* $@83: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 754: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@80 "name" $@81 "begin of code block" $@82 variant_alias_type_list $@83 "end of code block"  */
          {
        auto vtype = make_smart<TypeDecl>(Type::tVariant);
        vtype->alias = *(yyvsp[-6].s);
        vtype->at = tokAt(scanner,(yylsp[-6]));
        vtype->isPrivateAlias = !(yyvsp[-8].b);
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

  case 755: /* $@84: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 756: /* $@85: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 757: /* $@86: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 758: /* $@87: %empty  */
                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 759: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@84 "name" $@85 "begin of code block" $@86 bitfield_alias_bits $@87 "end of code block"  */
          {
        auto btype = make_smart<TypeDecl>(Type::tBitfield);
        btype->alias = *(yyvsp[-6].s);
        btype->at = tokAt(scanner,(yylsp[-6]));
        btype->argNames = *(yyvsp[-2].pNameList);
        btype->isPrivateAlias = !(yyvsp[-8].b);
        if ( btype->argNames.size()>32 ) {
            das2_yyerror(scanner,"only 32 different bits are allowed in a bitfield",tokAt(scanner,(yylsp[-6])),
                CompilationError::invalid_type);
        }
        if ( !yyextra->g_Program->addAlias(btype) ) {
            das2_yyerror(scanner,"type alias is already defined "+*(yyvsp[-6].s),tokAt(scanner,(yylsp[-6])),
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

  case 760: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 761: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 762: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 763: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 764: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 765: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 766: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 767: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 768: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 769: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 770: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 771: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 772: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 773: /* make_struct_single: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 774: /* $@88: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 775: /* $@89: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 776: /* make_struct_decl: "struct" '<' $@88 type_declaration_no_options '>' $@89 '(' make_struct_single ')'  */
                                                                                                                                                                                                           {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 777: /* $@90: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 778: /* $@91: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 779: /* make_struct_decl: "class" '<' $@90 type_declaration_no_options '>' $@91 '(' make_struct_single ')'  */
                                                                                                                                                                                                          {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 780: /* $@92: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 781: /* $@93: %empty  */
                                                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 782: /* make_struct_decl: "variant" '<' $@92 type_declaration_no_options '>' $@93 '(' make_struct_single ')'  */
                                                                                                                                                                                                             {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 783: /* $@94: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 784: /* $@95: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 785: /* make_struct_decl: "default" '<' $@94 type_declaration_no_options '>' $@95  */
                                                                                                                                      {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-5]));
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->useInitializer = true;
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 786: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 787: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 788: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 789: /* $@96: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 790: /* $@97: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 791: /* make_tuple_call: "tuple" '<' $@96 type_declaration_no_options '>' $@97 '(' make_struct_single ')'  */
                                                                                                                                                                                                           {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 792: /* make_dim_decl: '[' expr_list optional_comma ']'  */
                                                          {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 793: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
                                                                   {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 794: /* $@98: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 795: /* $@99: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 796: /* make_dim_decl: "array" '<' $@98 type_declaration_no_options '>' $@99 '(' expr_list optional_comma ')'  */
                                                                                                                                                                              {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-9])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 797: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 798: /* $@100: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 799: /* $@101: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 800: /* make_dim_decl: "fixed_array" '<' $@100 type_declaration_no_options '>' $@101 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        if ( !mka->makeType->dim.size() ) mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 801: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 802: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 803: /* make_table_decl: "begin of code block" expr_map_tuple_list optional_comma "end of code block"  */
                                                                    {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 804: /* make_table_decl: "table" '(' expr_map_tuple_list optional_comma ')'  */
                                                                             {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 805: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' expr_map_tuple_list optional_comma ')'  */
                                                                                                                       {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-7])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-7])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 806: /* make_table_decl: "table" '<' type_declaration_no_options "end of expression" type_declaration_no_options '>' '(' expr_map_tuple_list optional_comma ')'  */
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

  case 807: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 808: /* array_comprehension_where: "end of expression" "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 809: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 810: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 811: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 812: /* array_comprehension: "begin of code block" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
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


