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
  YYSYMBOL_expression_if_one_liner = 241,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 242,  /* expression_if_then_else  */
  YYSYMBOL_expression_for_loop = 243,      /* expression_for_loop  */
  YYSYMBOL_expression_unsafe = 244,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 245,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 246,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 247,    /* expression_with_alias  */
  YYSYMBOL_annotation_argument_value = 248, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 249, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 250, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 251,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 252, /* annotation_argument_list  */
  YYSYMBOL_annotation_declaration_name = 253, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 254, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 255,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 256,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 257, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 258, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 259,   /* optional_function_type  */
  YYSYMBOL_function_name = 260,            /* function_name  */
  YYSYMBOL_global_function_declaration = 261, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 262, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 263, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 264,     /* function_declaration  */
  YYSYMBOL_265_2 = 265,                    /* $@2  */
  YYSYMBOL_expression_block = 266,         /* expression_block  */
  YYSYMBOL_expr_call_pipe = 267,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 268,           /* expression_any  */
  YYSYMBOL_expressions = 269,              /* expressions  */
  YYSYMBOL_expr_keyword = 270,             /* expr_keyword  */
  YYSYMBOL_optional_expr_list = 271,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_list_in_braces = 272, /* optional_expr_list_in_braces  */
  YYSYMBOL_type_declaration_no_options_list = 273, /* type_declaration_no_options_list  */
  YYSYMBOL_expression_keyword = 274,       /* expression_keyword  */
  YYSYMBOL_275_3 = 275,                    /* $@3  */
  YYSYMBOL_276_4 = 276,                    /* $@4  */
  YYSYMBOL_277_5 = 277,                    /* $@5  */
  YYSYMBOL_278_6 = 278,                    /* $@6  */
  YYSYMBOL_expr_pipe = 279,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 280,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 281,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 282,     /* new_type_declaration  */
  YYSYMBOL_283_7 = 283,                    /* $@7  */
  YYSYMBOL_284_8 = 284,                    /* $@8  */
  YYSYMBOL_expr_new = 285,                 /* expr_new  */
  YYSYMBOL_expression_break = 286,         /* expression_break  */
  YYSYMBOL_expression_continue = 287,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 288, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 289,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 290, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 291,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 292,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 293,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 294,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 295,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 296,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 297, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 298,           /* expression_let  */
  YYSYMBOL_expr_cast = 299,                /* expr_cast  */
  YYSYMBOL_300_9 = 300,                    /* $@9  */
  YYSYMBOL_301_10 = 301,                   /* $@10  */
  YYSYMBOL_302_11 = 302,                   /* $@11  */
  YYSYMBOL_303_12 = 303,                   /* $@12  */
  YYSYMBOL_304_13 = 304,                   /* $@13  */
  YYSYMBOL_305_14 = 305,                   /* $@14  */
  YYSYMBOL_expr_type_decl = 306,           /* expr_type_decl  */
  YYSYMBOL_307_15 = 307,                   /* $@15  */
  YYSYMBOL_308_16 = 308,                   /* $@16  */
  YYSYMBOL_expr_type_info = 309,           /* expr_type_info  */
  YYSYMBOL_expr_list = 310,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 311,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 312,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 313,            /* capture_entry  */
  YYSYMBOL_capture_list = 314,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 315,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 316,               /* expr_block  */
  YYSYMBOL_expr_full_block = 317,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 318, /* expr_full_block_assumed_piped  */
  YYSYMBOL_expr_numeric_const = 319,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 320,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe_right = 321,   /* expr_assign_pipe_right  */
  YYSYMBOL_expr_assign_pipe = 322,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 323,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 324,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 325,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 326,           /* func_addr_expr  */
  YYSYMBOL_327_17 = 327,                   /* $@17  */
  YYSYMBOL_328_18 = 328,                   /* $@18  */
  YYSYMBOL_329_19 = 329,                   /* $@19  */
  YYSYMBOL_330_20 = 330,                   /* $@20  */
  YYSYMBOL_expr_field = 331,               /* expr_field  */
  YYSYMBOL_332_21 = 332,                   /* $@21  */
  YYSYMBOL_333_22 = 333,                   /* $@22  */
  YYSYMBOL_expr_call = 334,                /* expr_call  */
  YYSYMBOL_expr = 335,                     /* expr  */
  YYSYMBOL_336_23 = 336,                   /* $@23  */
  YYSYMBOL_337_24 = 337,                   /* $@24  */
  YYSYMBOL_338_25 = 338,                   /* $@25  */
  YYSYMBOL_339_26 = 339,                   /* $@26  */
  YYSYMBOL_340_27 = 340,                   /* $@27  */
  YYSYMBOL_341_28 = 341,                   /* $@28  */
  YYSYMBOL_expr_mtag = 342,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 343, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 344,        /* optional_override  */
  YYSYMBOL_optional_constant = 345,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 346, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 347, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 348, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 349, /* struct_variable_declaration_list  */
  YYSYMBOL_350_29 = 350,                   /* $@29  */
  YYSYMBOL_351_30 = 351,                   /* $@30  */
  YYSYMBOL_352_31 = 352,                   /* $@31  */
  YYSYMBOL_function_argument_declaration = 353, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 354,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 355,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 356,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 357,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 358,             /* variant_type  */
  YYSYMBOL_variant_type_list = 359,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 360,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 361,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 362,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 363,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 364,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 365, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 366, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 367, /* global_variable_declaration_list  */
  YYSYMBOL_368_32 = 368,                   /* $@32  */
  YYSYMBOL_optional_shared = 369,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 370, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 371,               /* global_let  */
  YYSYMBOL_372_33 = 372,                   /* $@33  */
  YYSYMBOL_enum_list = 373,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 374, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 375,             /* single_alias  */
  YYSYMBOL_376_34 = 376,                   /* $@34  */
  YYSYMBOL_alias_list = 377,               /* alias_list  */
  YYSYMBOL_alias_declaration = 378,        /* alias_declaration  */
  YYSYMBOL_optional_public_or_private_enum = 379, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 380,                /* enum_name  */
  YYSYMBOL_enum_declaration = 381,         /* enum_declaration  */
  YYSYMBOL_382_35 = 382,                   /* $@35  */
  YYSYMBOL_383_36 = 383,                   /* $@36  */
  YYSYMBOL_384_37 = 384,                   /* $@37  */
  YYSYMBOL_385_38 = 385,                   /* $@38  */
  YYSYMBOL_optional_structure_parent = 386, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 387,          /* optional_sealed  */
  YYSYMBOL_structure_name = 388,           /* structure_name  */
  YYSYMBOL_class_or_struct = 389,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 390, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 391, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 392,    /* structure_declaration  */
  YYSYMBOL_393_39 = 393,                   /* $@39  */
  YYSYMBOL_394_40 = 394,                   /* $@40  */
  YYSYMBOL_variable_name_with_pos_list = 395, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 396,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 397, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 398, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 399,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 400,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 401,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 402, /* bitfield_type_declaration  */
  YYSYMBOL_403_41 = 403,                   /* $@41  */
  YYSYMBOL_404_42 = 404,                   /* $@42  */
  YYSYMBOL_table_type_pair = 405,          /* table_type_pair  */
  YYSYMBOL_dim_list = 406,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 407, /* type_declaration_no_options  */
  YYSYMBOL_408_43 = 408,                   /* $@43  */
  YYSYMBOL_409_44 = 409,                   /* $@44  */
  YYSYMBOL_410_45 = 410,                   /* $@45  */
  YYSYMBOL_411_46 = 411,                   /* $@46  */
  YYSYMBOL_412_47 = 412,                   /* $@47  */
  YYSYMBOL_413_48 = 413,                   /* $@48  */
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
  YYSYMBOL_type_declaration = 435,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 436,  /* tuple_alias_declaration  */
  YYSYMBOL_437_70 = 437,                   /* $@70  */
  YYSYMBOL_438_71 = 438,                   /* $@71  */
  YYSYMBOL_439_72 = 439,                   /* $@72  */
  YYSYMBOL_variant_alias_declaration = 440, /* variant_alias_declaration  */
  YYSYMBOL_441_73 = 441,                   /* $@73  */
  YYSYMBOL_442_74 = 442,                   /* $@74  */
  YYSYMBOL_443_75 = 443,                   /* $@75  */
  YYSYMBOL_bitfield_alias_declaration = 444, /* bitfield_alias_declaration  */
  YYSYMBOL_445_76 = 445,                   /* $@76  */
  YYSYMBOL_446_77 = 446,                   /* $@77  */
  YYSYMBOL_447_78 = 447,                   /* $@78  */
  YYSYMBOL_make_decl = 448,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 449,       /* make_struct_fields  */
  YYSYMBOL_make_struct_single = 450,       /* make_struct_single  */
  YYSYMBOL_make_struct_decl = 451,         /* make_struct_decl  */
  YYSYMBOL_452_79 = 452,                   /* $@79  */
  YYSYMBOL_453_80 = 453,                   /* $@80  */
  YYSYMBOL_454_81 = 454,                   /* $@81  */
  YYSYMBOL_455_82 = 455,                   /* $@82  */
  YYSYMBOL_456_83 = 456,                   /* $@83  */
  YYSYMBOL_457_84 = 457,                   /* $@84  */
  YYSYMBOL_458_85 = 458,                   /* $@85  */
  YYSYMBOL_459_86 = 459,                   /* $@86  */
  YYSYMBOL_make_map_tuple = 460,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 461,          /* make_tuple_call  */
  YYSYMBOL_462_87 = 462,                   /* $@87  */
  YYSYMBOL_463_88 = 463,                   /* $@88  */
  YYSYMBOL_make_dim_decl = 464,            /* make_dim_decl  */
  YYSYMBOL_465_89 = 465,                   /* $@89  */
  YYSYMBOL_466_90 = 466,                   /* $@90  */
  YYSYMBOL_467_91 = 467,                   /* $@91  */
  YYSYMBOL_468_92 = 468,                   /* $@92  */
  YYSYMBOL_expr_map_tuple_list = 469,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 470,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 471, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 472,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 473       /* array_comprehension  */
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
#define YYLAST   12204

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  213
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  261
/* YYNRULES -- Number of rules.  */
#define YYNRULES  800
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1454

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
     595,   604,   612,   628,   633,   641,   641,   680,   698,   702,
     703,   704,   708,   711,   715,   721,   730,   733,   739,   740,
     744,   748,   749,   753,   756,   762,   768,   771,   777,   778,
     782,   783,   784,   793,   794,   798,   799,   805,   806,   807,
     808,   809,   813,   819,   825,   831,   839,   849,   858,   865,
     866,   867,   868,   869,   870,   874,   879,   887,   888,   889,
     893,   894,   895,   896,   897,   898,   899,   900,   906,   909,
     915,   916,   917,   921,   934,   952,   955,   963,   974,   985,
     996,   999,  1006,  1010,  1017,  1018,  1022,  1023,  1024,  1028,
    1031,  1038,  1042,  1043,  1044,  1045,  1046,  1047,  1048,  1049,
    1050,  1051,  1052,  1053,  1054,  1055,  1056,  1057,  1058,  1059,
    1060,  1061,  1062,  1063,  1064,  1065,  1066,  1067,  1068,  1069,
    1070,  1071,  1072,  1073,  1074,  1075,  1076,  1077,  1078,  1079,
    1080,  1081,  1082,  1083,  1084,  1085,  1086,  1087,  1088,  1089,
    1090,  1091,  1092,  1093,  1094,  1095,  1096,  1097,  1098,  1099,
    1100,  1101,  1102,  1103,  1104,  1105,  1106,  1107,  1108,  1109,
    1110,  1111,  1112,  1113,  1114,  1115,  1116,  1117,  1118,  1119,
    1120,  1121,  1122,  1123,  1124,  1125,  1130,  1148,  1149,  1150,
    1154,  1160,  1160,  1177,  1181,  1192,  1201,  1213,  1214,  1215,
    1216,  1217,  1218,  1219,  1220,  1221,  1222,  1223,  1224,  1225,
    1226,  1227,  1228,  1229,  1230,  1231,  1232,  1233,  1237,  1242,
    1248,  1254,  1265,  1266,  1270,  1271,  1275,  1279,  1286,  1286,
    1286,  1292,  1292,  1292,  1301,  1335,  1338,  1341,  1344,  1350,
    1351,  1362,  1366,  1369,  1377,  1377,  1377,  1380,  1386,  1389,
    1392,  1396,  1403,  1409,  1413,  1417,  1420,  1423,  1431,  1434,
    1437,  1445,  1448,  1456,  1459,  1462,  1470,  1476,  1477,  1478,
    1482,  1483,  1487,  1488,  1492,  1497,  1505,  1511,  1523,  1526,
    1532,  1532,  1532,  1535,  1535,  1535,  1540,  1540,  1540,  1548,
    1548,  1548,  1554,  1564,  1575,  1590,  1593,  1599,  1600,  1607,
    1618,  1619,  1620,  1624,  1625,  1626,  1627,  1631,  1636,  1644,
    1645,  1649,  1654,  1661,  1668,  1676,  1677,  1678,  1679,  1680,
    1681,  1682,  1686,  1687,  1688,  1689,  1690,  1691,  1692,  1693,
    1694,  1695,  1696,  1697,  1698,  1699,  1700,  1701,  1702,  1703,
    1704,  1708,  1709,  1710,  1711,  1716,  1717,  1718,  1719,  1720,
    1721,  1722,  1723,  1724,  1725,  1726,  1727,  1728,  1729,  1730,
    1731,  1732,  1737,  1744,  1756,  1761,  1771,  1775,  1782,  1785,
    1785,  1785,  1790,  1790,  1790,  1803,  1807,  1811,  1816,  1823,
    1823,  1823,  1830,  1834,  1843,  1847,  1850,  1856,  1857,  1858,
    1859,  1860,  1861,  1862,  1863,  1864,  1865,  1866,  1867,  1868,
    1869,  1870,  1871,  1872,  1873,  1874,  1875,  1876,  1877,  1878,
    1879,  1880,  1881,  1882,  1883,  1884,  1885,  1886,  1887,  1888,
    1889,  1890,  1891,  1897,  1898,  1899,  1900,  1901,  1914,  1915,
    1916,  1917,  1918,  1919,  1920,  1921,  1922,  1923,  1924,  1925,
    1928,  1931,  1932,  1935,  1935,  1935,  1938,  1943,  1947,  1951,
    1951,  1951,  1956,  1959,  1963,  1963,  1963,  1968,  1971,  1972,
    1973,  1974,  1975,  1976,  1977,  1978,  1979,  1981,  1985,  1989,
    1990,  1991,  1992,  1993,  1994,  1995,  1999,  2003,  2007,  2011,
    2015,  2019,  2023,  2027,  2031,  2038,  2039,  2043,  2044,  2045,
    2049,  2050,  2054,  2055,  2056,  2060,  2061,  2065,  2076,  2079,
    2079,  2098,  2097,  2111,  2110,  2126,  2135,  2145,  2146,  2150,
    2153,  2162,  2163,  2167,  2170,  2173,  2189,  2198,  2199,  2203,
    2206,  2209,  2223,  2224,  2228,  2234,  2240,  2243,  2247,  2253,
    2262,  2263,  2264,  2268,  2269,  2273,  2280,  2285,  2294,  2300,
    2311,  2314,  2319,  2324,  2332,  2343,  2346,  2346,  2366,  2367,
    2371,  2372,  2373,  2377,  2380,  2380,  2398,  2401,  2404,  2419,
    2438,  2439,  2440,  2445,  2445,  2471,  2472,  2476,  2477,  2481,
    2482,  2483,  2487,  2497,  2502,  2497,  2514,  2519,  2514,  2534,
    2535,  2539,  2540,  2544,  2550,  2551,  2555,  2556,  2557,  2561,
    2564,  2570,  2575,  2570,  2589,  2596,  2601,  2610,  2616,  2627,
    2628,  2629,  2630,  2631,  2632,  2633,  2634,  2635,  2636,  2637,
    2638,  2639,  2640,  2641,  2642,  2643,  2644,  2645,  2646,  2647,
    2648,  2649,  2650,  2651,  2652,  2653,  2657,  2658,  2659,  2660,
    2661,  2662,  2666,  2677,  2681,  2688,  2700,  2707,  2716,  2721,
    2724,  2737,  2737,  2737,  2750,  2754,  2761,  2765,  2772,  2773,
    2774,  2775,  2776,  2791,  2797,  2797,  2797,  2801,  2806,  2813,
    2813,  2820,  2824,  2828,  2833,  2838,  2843,  2848,  2852,  2856,
    2861,  2865,  2869,  2874,  2874,  2874,  2880,  2887,  2887,  2887,
    2892,  2892,  2892,  2898,  2898,  2898,  2903,  2907,  2907,  2907,
    2912,  2912,  2912,  2921,  2925,  2925,  2925,  2930,  2930,  2930,
    2939,  2943,  2943,  2943,  2948,  2948,  2948,  2957,  2957,  2957,
    2963,  2963,  2963,  2972,  2975,  2986,  3002,  3007,  3012,  3002,
    3037,  3042,  3048,  3037,  3073,  3078,  3083,  3073,  3113,  3114,
    3115,  3116,  3117,  3121,  3128,  3135,  3141,  3147,  3154,  3161,
    3167,  3177,  3185,  3185,  3185,  3193,  3193,  3193,  3200,  3200,
    3200,  3207,  3207,  3207,  3218,  3224,  3230,  3236,  3236,  3236,
    3246,  3254,  3262,  3262,  3262,  3270,  3277,  3277,  3277,  3287,
    3290,  3296,  3304,  3312,  3320,  3333,  3334,  3338,  3339,  3344,
    3347
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
  "if_or_static_if", "expression_else_one_liner",
  "expression_if_one_liner", "expression_if_then_else",
  "expression_for_loop", "expression_unsafe", "expression_while_loop",
  "expression_with", "expression_with_alias", "annotation_argument_value",
  "annotation_argument_value_list", "annotation_argument_name",
  "annotation_argument", "annotation_argument_list",
  "annotation_declaration_name", "annotation_declaration_basic",
  "annotation_declaration", "annotation_list", "optional_annotation_list",
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
  "optional_public_or_private_variable", "global_let", "$@33", "enum_list",
  "optional_public_or_private_alias", "single_alias", "$@34", "alias_list",
  "alias_declaration", "optional_public_or_private_enum", "enum_name",
  "enum_declaration", "$@35", "$@36", "$@37", "$@38",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "optional_struct_variable_declaration_list", "structure_declaration",
  "$@39", "$@40", "variable_name_with_pos_list", "basic_type_declaration",
  "enum_basic_type_declaration", "structure_type_declaration",
  "auto_type_declaration", "bitfield_bits", "bitfield_alias_bits",
  "bitfield_type_declaration", "$@41", "$@42", "table_type_pair",
  "dim_list", "type_declaration_no_options", "$@43", "$@44", "$@45",
  "$@46", "$@47", "$@48", "$@49", "$@50", "$@51", "$@52", "$@53", "$@54",
  "$@55", "$@56", "$@57", "$@58", "$@59", "$@60", "$@61", "$@62", "$@63",
  "$@64", "$@65", "$@66", "$@67", "$@68", "$@69", "type_declaration",
  "tuple_alias_declaration", "$@70", "$@71", "$@72",
  "variant_alias_declaration", "$@73", "$@74", "$@75",
  "bitfield_alias_declaration", "$@76", "$@77", "$@78", "make_decl",
  "make_struct_fields", "make_struct_single", "make_struct_decl", "$@79",
  "$@80", "$@81", "$@82", "$@83", "$@84", "$@85", "$@86", "make_map_tuple",
  "make_tuple_call", "$@87", "$@88", "make_dim_decl", "$@89", "$@90",
  "$@91", "$@92", "expr_map_tuple_list", "make_table_decl",
  "array_comprehension_where", "optional_comma", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1279)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-725)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1279,    41, -1279, -1279,    27,  -125,   -39,    -3, -1279,   -82,
     213,   213,   213, -1279,   -64,   102, -1279, -1279,   -93, -1279,
   -1279, -1279,   373, -1279,    38, -1279, -1279, -1279, -1279, -1279,
   -1279, -1279, -1279, -1279, -1279,   -15, -1279,   154,   347,   294,
   -1279, -1279, -1279, -1279,   -39, -1279,     7, -1279, -1279, -1279,
     213,    45,   353, -1279, -1279,    38,   392,   403,   416,   421,
     397, -1279, -1279, -1279,   102,   102,   102,   369, -1279,   495,
      99, -1279, -1279, -1279, -1279,   332,   478,   504, -1279,   512,
      15,    27,   433,  -125,   388,   434, -1279,   499,   499, -1279,
     423,    33, -1279, -1279,   516, -1279, -1279, -1279, -1279,   449,
     452, -1279, -1279,   -55,    27,   102,   102,   102,   102, -1279,
   -1279, -1279, -1279, -1279, -1279, -1279,   453, -1279, -1279, -1279,
   -1279, -1279,   442, -1279, -1279, -1279, -1279, -1279,   292,    40,
   -1279, -1279, -1279, -1279,   565, -1279, -1279, -1279, -1279,   455,
     445, -1279, -1279, -1279,   472,   475,   476, -1279, -1279,   461,
   -1279,   -13, -1279,   188,   511,   495, 12040, -1279,    97,   551,
   -1279,   469, -1279, -1279,   498, -1279, -1279, -1279, -1279, -1279,
   -1279, -1279,     1, -1279, -1279,  7633, -1279, -1279, -1279, -1279,
   -1279, 10742, -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279,
   -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279,
   -1279, -1279, -1279, -1279, -1279, -1279, -1279,   627,   639, -1279,
     468,   503, -1279,   322, -1279,   518, -1279,   513,    27,   -11,
   -1279, -1279, -1279,    40, -1279,   494,   497,   505,   485,   515,
     520, -1279, -1279, -1279,   488, -1279, -1279, -1279, -1279, -1279,
     521, -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279,
   -1279, -1279,   522, -1279, -1279, -1279,   526,   529, -1279, -1279,
   -1279, -1279,   531,   532,   506,   -64, -1279, -1279, -1279, -1279,
   -1279,  1770,   519, -1279, -1279, -1279,   558,   560, -1279, -1279,
   -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279,
   -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279,   562,   523,
   -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279,
   -1279, -1279,   702, -1279, -1279, -1279, -1279, -1279, -1279, -1279,
   -1279, -1279, -1279, -1279, -1279,   563,   527, -1279, -1279,   -79,
     543, -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279,
     555,   548,   564, -1279,   469,   310,   533,   697,   351, -1279,
   -1279, -1279, -1279, -1279,  6945, -1279, -1279,   576, -1279,   196,
     215,   216, -1279, -1279,  6945,   179, -1279, -1279, -1279,     6,
   -1279, -1279, -1279,    12,  3773, -1279,   538,  7429,   264,  7525,
     270, -1279, -1279, -1279, -1279,   581,   612, -1279,   544, -1279,
      55, -1279,   -78,  7633, -1279,  1564,   291, -1279,   -64, -1279,
   -1279, -1279,   -11, -1279,  6945,   587,   589,  7633, -1279,   -16,
    7633,  7633,  7633,   567,   569, -1279, -1279,   119,   552,   570,
     184, -1279,   225,   561,   574,   577,   568,   583,   249,   586,
   -1279,   250,   588,   591,  6945,  6945,   571,   575,   578,   579,
     580,   582, -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279,
     595,   596, -1279,  3973,  6945,  6945,  6945,  6945,  6945,  4173,
    6945, -1279,   592, -1279, -1279, -1279,   584, -1279, -1279, -1279,
   -1279,   598, -1279, -1279, -1279, -1279, -1279, -1279, -1279,  8092,
   -1279,   593, -1279, -1279, -1279, -1279, -1279, -1279,  7633,  7633,
     601,   606,  7633,   468,  7633,   468,  7633,   468,  7729,   625,
    8190, -1279,  6945, -1279, -1279, -1279, -1279,   600, -1279, -1279,
   10269,  6945, -1279,  1770,   622, -1279,   638,   -56, -1279,   640,
     519,   642,   633, -1279,   643,   645, -1279, -1279,  6945, -1279,
   -1279,    50,   -50, -1279,   519, -1279,  6945, -1279, -1279,    50,
    2187, -1279,   503,  4371,  6945,   665, -1279,   654,   664,  4569,
     503,  2385,   -77,   -77,   -77,  4767,   799, -1279,   666,   667,
    6945,   841, -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279,
   -1279,   278, -1279,   675,   677,   679,   681, -1279,   682, -1279,
   -1279,   794, -1279,   -60, -1279,  7251,   311, -1279,   685, -1279,
   -1279,    20, -1279,  8288, -1279,   820,   133, -1279, -1279, -1279,
    2781,   174,   287,   674,   293, -1279, -1279, -1279,   658, -1279,
   -1279,   -64, -1279, -1279,  6945,  7633,  6945,  6945, -1279, -1279,
    6945, -1279, -1279,  6945, -1279, -1279,  6945, -1279,  7633,    47,
      47,  6945,  6945,  6945,  6945,  6945,  6945, -1279, -1279,   456,
      50, 10777, -1279,   683,    47,    47,   -69,    47,    47,    50,
     686, 11436,   686,   151,  3177,   661, 11886, 11917,  6945,  6945,
   -1279, -1279,  6945,  6945,  6945,  6945,   707,  6945,   271,  6945,
    6945,  6945,  6945,  6945,  6945,  6945,  6945,  6945,  4965,  6945,
    6945,  6945,  6945,  6945,  6945,  6945,  6945,  6945,  6945,   106,
    6945, -1279,  5163,   316,   359, -1279, -1279,    86,   406,   543,
     414,   543,   417,   543, -1279,   123, -1279,   176, -1279,  7633,
     663,   691, -1279, -1279, 10367, -1279, -1279,  7633, -1279, -1279,
    7633, -1279, -1279,  8323,   670,   832, -1279,   -76, -1279, 10875,
      21,  2781, -1279,   691,   746,  7835,   866,  6945, 11436, 10875,
     698, -1279,   699,   725, 11436, -1279,  2781, -1279,  7835,   676,
   -1279,   598, -1279, -1279, -1279, 10875,   712, -1279, -1279, 10875,
    6945,   598, -1279, -1279, -1279, -1279, -1279, -1279, -1279,   -87,
     -77, -1279,  2979,  2979,  2979,  2979,  2979,  2979,  2979,  2979,
    2979,  2979,  2979,  6945,  2979,  2979,  2979,  2979,  2979,  2979,
   -1279, -1279,  6945, -1279,   291, -1279,   878,   469, -1279,   724,
   -1279,  2781, -1279,  7945, -1279, -1279, -1279,  7633,  7633,  7633,
    7633,  3375,  5361,  7633,  7633,   686,   817,   683,  8421,  7633,
    7633,  8519,  7633,  7633,   686,  7633,  7633,   686,  7633,   559,
    8554,  8652,  8750,  8785,  8883,  8981,  7633,  7633, -1279,  6945,
     379,    23,  6945,  6945,   714,    24,  6945,   684,   687,   700,
     704,   217, -1279, -1279,   713,   -23,  3575, -1279,    39,   708,
     706,   688,   468,   703,   715, -1279, -1279,   718,   717, -1279,
   -1279,   370,   370,  1726,  1726, 11748, 11748,   719,   169,   721,
   -1279, 10402,   -48,   -48,   593,   370,   370,  1291,   711, 11613,
   11471, 12009, 10973,  7291, 11700, 11724,  1726,  1726,   507,   507,
     169,   169,   169,   277,  6945,   722,   726,   317,  6945,   918,
   10500, -1279,    81, -1279, -1279,   729, -1279, -1279,   730, -1279,
     732, -1279,   742,  7729, -1279,   625, -1279,   177,   519, -1279,
    6945, -1279,   519,   519, -1279,  6945,   769, -1279,   771, -1279,
    7633, -1279,  2781, -1279,  6945, -1279,   691,  6945,  6945,  6945,
    6945,  6945,  6945,  6945,  6945,  6945,  6945,  6945,  6945,  6945,
    6945,  6945,  6945,  6945,   503, 11436, -1279,  6945, -1279, -1279,
   -1279,  7835, -1279,   661, -1279, -1279,   448,  1324,   661,   772,
   -1279, -1279, -1279,   -77,   -77,   -77, -1279, -1279,  7724, -1279,
    7724, -1279,  7724, -1279,  7724, -1279,  7724, -1279,  7724, -1279,
    7724, -1279,  7724, -1279,  7724, -1279,  7724, -1279,  7724, 11436,
   -1279,  7724, -1279,  7724, -1279,  7724, -1279,  7724, -1279,  7724,
   -1279,  7724, 11008,   762,   525,   873,   767, -1279, -1279,  8054,
   -1279, -1279, -1279, -1279,   519,   851,   881,   419, -1279,    98,
     736,   783,  9016,   420,   892,   739,  7633,   741,   747, -1279,
    1739,  1758, -1279,  1924,  3496,   748,  3516,  3770,   749,  3914,
     661, -1279, -1279, -1279, -1279, -1279,   751,   241,   253, 11106,
    6945, 11436, -1279, -1279,  6945, 11436, -1279, -1279,  6945,  6945,
    7633,   468,  6945,  6945,  6945,   136,  7143, -1279,   344, -1279,
     -68,   543, -1279,  6945, -1279,  6945,  5559,  6945, -1279,   766,
     752, -1279, -1279,  6945,   754, -1279, 10598,  6945,  5757,   757,
   -1279, 10633, -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279,
   -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279,  7633,
     758,  9114, -1279,   927,    -2, -1279,  7835,    48, 11436, 11436,
   11436, 11436, 11436, 11436, 11436, 11436, 11436, 11436, 11436, 11436,
   11436, 11436, 11436, 11436, 11436, -1279, 11436,   468,  1989,   503,
   -1279, -1279,  6945, -1279,  2583,   795,   468, -1279,   138, -1279,
   -1279, -1279, -1279, -1279, -1279, -1279,   194, -1279,    36, -1279,
   -1279, -1279, -1279, -1279, -1279, -1279,   254, -1279, -1279, -1279,
   -1279,  3970,  6945, -1279, -1279, -1279, -1279, -1279, -1279, -1279,
   -1279, -1279, -1279,   765,  5955, -1279, -1279, -1279,   412,   413,
    9212,  9247,  4114,   543,  9345, 11436, 11436,   761,  3575,   770,
     104,   811,   813,   815,   816, -1279,   137,    90,  7633,  9443,
    7633,  9478, -1279,   153,  9576, -1279,  6945, 11569,  6945, -1279,
    9674, -1279,   167,  6945, -1279, -1279, -1279, -1279, -1279,   519,
    6945, -1279,   818,  6945, -1279,   543, -1279, -1279, 10875,  6153,
    6351, -1279, -1279, -1279, -1279, -1279, 11436, -1279,   543,   819,
     319,   949,    36, -1279, -1279,   525,   776,   779, -1279,   824,
    6945, -1279,   781,   782,   683,  6945,  6945,  6945,   784, -1279,
     787,   788,  6549, -1279,   173,  6945,   789,  6945,  6945, -1279,
   -1279, -1279,   808,   115, -1279,   152,  6945,  6945,  6945, -1279,
   -1279, -1279, -1279,   -68,   792,  6747, -1279, -1279,  7140, -1279,
     422, -1279, -1279, -1279,  7633,  9709,  9807, -1279, -1279,  9905,
     791, -1279, 11436,    90,   448,  6945,  6945, 11436,   503, -1279,
    7633,   -16, -1279, -1279,   949,    50,   380,   380,   814,  9940,
    6945,  6945,   800,  1726,  1726,  1726,  6945,   380,   380, -1279,
   10038, -1279,  1726,  6945, -1279,   827, 11204,   208, -1279,  6945,
    6945,   801, 10136, 11436, 11436, -1279, -1279,  6945, 11471, -1279,
   -1279,   425, -1279, -1279, -1279, -1279, -1279, -1279, 11436, -1279,
     657,  6945, 12040, -1279, -1279,   144,   802,   803,  6945, -1279,
     686,   683, -1279,   686,   805,   806, -1279,   807,   967,   845,
     821, -1279,   208, 11436, 11436, -1279,   155, 11569, -1279, -1279,
   -1279,  6945, 11240,   846, 12040, -1279, -1279, 10171,   812,   822,
     825, -1279, -1279, -1279,  6945, -1279, -1279, -1279,  6945,  6945,
   -1279, 11338, -1279, -1279,   503, -1279, -1279, -1279, -1279, 11436,
   11436, 11436, -1279, -1279
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   114,     1,   290,     0,     0,     0,   580,   291,     0,
     580,   580,   580,    16,     0,     0,    15,     3,     0,    10,
       9,     8,     0,     7,   568,     6,    11,     5,     4,    13,
      12,    14,    88,    89,    87,    96,    98,    37,    53,    50,
      51,    39,    40,    41,     0,    42,    48,    38,   582,   581,
     580,     0,     0,    22,    21,   568,     0,     0,     0,     0,
     259,    35,   101,   102,     0,     0,     0,   103,   105,   112,
       0,   100,    17,   605,   604,   207,   589,   606,   569,   570,
       0,     0,     0,     0,    43,     0,    49,     0,     0,    46,
       0,   580,   583,   588,    18,   744,   736,   740,   261,     0,
       0,   111,   106,     0,     0,     0,     0,     0,     0,   115,
     209,   208,   211,   206,   591,   590,     0,   608,   607,   611,
     572,   571,   574,    94,    95,    92,    93,    91,     0,     0,
      90,    99,    54,    52,    48,    45,    44,   585,   587,     0,
       0,    19,    20,    23,     0,     0,     0,   260,    33,    36,
     110,     0,   107,   108,   109,   113,     0,   592,     0,   601,
     565,   505,    24,    25,     0,    83,    84,    81,    82,    80,
      79,    85,     0,    47,   586,     0,   745,   737,   741,    34,
     104,     0,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,     0,     0,   121,
     116,     0,   593,     0,   602,     0,   612,   566,     0,     0,
      26,    27,    28,     0,    97,     0,     0,     0,     0,     0,
       0,   619,   639,   620,   653,   621,   625,   626,   627,   628,
     645,   632,   633,   634,   635,   636,   637,   638,   640,   641,
     642,   643,   706,   624,   631,   644,   713,   720,   622,   629,
     623,   630,     0,     0,     0,     0,   652,   668,   671,   669,
     670,   733,   584,   658,   533,   539,   175,   176,   173,   124,
     125,   127,   126,   128,   129,   130,   131,   157,   158,   155,
     156,   148,   159,   160,   149,   146,   147,   174,   168,     0,
     172,   161,   162,   163,   164,   135,   136,   137,   132,   133,
     134,   145,     0,   151,   152,   150,   143,   144,   139,   138,
     140,   141,   142,   123,   122,   167,     0,   153,   154,   505,
     119,   238,   212,   576,   646,   649,   647,   650,   648,   651,
       0,   599,   609,   573,   505,     0,     0,   555,   553,   575,
      86,   674,   697,   700,     0,   703,   693,     0,   661,   707,
     714,   721,   727,   730,     0,     0,   683,   688,   682,     0,
     696,   692,   685,     0,     0,   687,   672,     0,   746,   738,
     742,   177,   178,   171,   166,   179,   169,   165,     0,   117,
     289,   527,     0,     0,   210,     0,   594,   596,     0,   603,
     518,   613,     0,   506,     0,     0,     0,     0,   554,     0,
       0,     0,     0,     0,     0,   413,   414,     0,     0,     0,
       0,   407,     0,     0,     0,     0,     0,     0,     0,     0,
     645,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   494,   335,   337,   336,   338,   339,   340,   341,
       0,     0,    29,     0,     0,     0,     0,     0,     0,     0,
       0,   320,   321,   411,   410,   488,   408,   481,   480,   479,
     478,   114,   484,   409,   483,   482,   454,   415,   455,     0,
     416,     0,   412,   748,   752,   749,   750,   751,     0,     0,
       0,     0,     0,   116,     0,   116,     0,   116,     0,     0,
       0,   679,   242,   690,   691,   684,   686,     0,   689,   673,
       0,     0,   735,   734,     0,   659,     0,   259,   534,     0,
     529,     0,     0,   540,     0,     0,   180,   170,     0,   287,
     288,     0,   505,   118,   120,   240,     0,    63,    64,     0,
     275,   273,     0,     0,     0,     0,   274,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   213,   217,     0,     0,
       0,     0,   233,   228,   225,   224,   226,   227,   258,   239,
     219,   488,   218,     0,    70,    71,    68,   231,    69,   232,
     234,   293,   223,     0,   220,   342,     0,   577,     0,   576,
     600,   519,   567,     0,   557,   558,     0,   551,   552,   550,
       0,     0,     0,     0,   664,   762,   765,   264,   268,   267,
     272,     0,   309,   782,     0,     0,     0,     0,   300,   303,
       0,   306,   786,     0,   771,   777,     0,   768,     0,   443,
     444,     0,     0,     0,     0,     0,     0,   248,   251,     0,
       0,   775,   789,   797,   420,   419,   456,   418,   417,     0,
     797,   315,   797,   322,     0,   329,     0,     0,     0,     0,
     445,   446,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   399,
       0,   677,     0,     0,     0,   654,   656,     0,     0,   119,
       0,   119,     0,   119,   531,     0,   537,     0,   655,     0,
       0,   243,   681,   666,     0,   660,   747,     0,   535,   739,
       0,   541,   743,     0,     0,   614,   525,   544,   528,     0,
       0,     0,   279,   276,     0,   315,     0,     0,   262,     0,
       0,   237,     0,     0,    57,    75,     0,   284,   281,   321,
     331,   114,   257,   255,   256,     0,     0,   235,   236,     0,
       0,   114,   216,   222,   229,   230,   278,   283,   292,     0,
       0,   221,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     215,   578,     0,   595,   597,   610,     0,   505,   556,     0,
     560,     0,   564,   342,   675,   698,   701,     0,     0,     0,
       0,     0,     0,     0,     0,   797,     0,   797,     0,     0,
       0,     0,     0,     0,   797,     0,     0,   797,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    32,     0,
      30,     0,     0,   798,     0,     0,   798,     0,     0,     0,
       0,   389,   386,   388,     0,   259,     0,   402,     0,   761,
       0,     0,   116,     0,     0,   467,   466,     0,     0,   468,
     472,   421,   422,   434,   435,   432,   433,     0,   461,     0,
     452,     0,   485,   486,   487,   423,   424,   439,   440,   441,
     442,     0,     0,   437,   438,   436,   430,   431,   426,   425,
     427,   428,   429,     0,     0,     0,   395,     0,     0,     0,
       0,   405,     0,   704,   694,     0,   662,   708,     0,   715,
       0,   722,     0,     0,   728,     0,   731,     0,   246,   678,
       0,   667,   530,   536,   526,     0,     0,   543,     0,   542,
       0,   545,     0,    76,     0,   280,   277,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   263,    77,     0,    55,    56,
     285,   282,   322,   329,   241,   238,    60,    65,   329,     0,
     299,   298,   254,     0,     0,     0,   364,   373,   352,   374,
     353,   376,   355,   375,   354,   377,   356,   367,   346,   368,
     347,   369,   348,   378,   357,   379,   358,   366,   344,   345,
     380,   359,   381,   360,   370,   349,   371,   350,   372,   351,
     365,   343,     0,     0,   512,   515,     0,   559,   562,   342,
     563,   676,   699,   702,   665,     0,     0,     0,   269,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   457,
       0,     0,   458,     0,     0,     0,     0,     0,     0,     0,
     329,   489,   490,   491,   492,   493,     0,     0,     0,     0,
       0,   774,   790,   791,     0,   316,   780,   447,     0,     0,
       0,   116,     0,     0,     0,     0,     0,   404,     0,   403,
       0,   119,   463,     0,   469,     0,     0,     0,   450,     0,
       0,   473,   477,     0,     0,   453,     0,     0,     0,     0,
     396,     0,   400,   448,   406,   705,   695,   657,   663,   709,
     711,   716,   718,   723,   725,   532,   729,   538,   732,     0,
       0,     0,   616,   617,   546,   549,   548,     0,   352,   353,
     355,   354,   356,   346,   347,   348,   357,   358,   344,   359,
     360,   349,   350,   351,   343,   286,    78,   116,     0,     0,
      58,    59,     0,    72,     0,     0,   116,   294,     0,   363,
     361,   362,   579,   598,   513,   514,   515,   516,   507,   520,
     561,   763,   766,   265,   270,   271,     0,   312,   310,   783,
     781,     0,     0,   792,   301,   304,   307,   787,   785,   772,
     778,   776,   769,     0,     0,   249,   252,    31,     0,     0,
       0,     0,     0,   119,     0,   754,   753,     0,     0,     0,
       0,     0,     0,     0,     0,   327,     0,     0,     0,     0,
       0,     0,   384,     0,     0,   474,     0,   462,     0,   451,
       0,   397,     0,     0,   449,   401,   712,   719,   726,   247,
     242,   615,     0,     0,    74,   119,   214,    61,     0,   275,
       0,    66,    70,    71,    68,    69,    67,    73,   119,     0,
     553,   510,   507,   508,   509,   512,     0,     0,   266,     0,
       0,   311,     0,     0,   797,     0,     0,     0,     0,   773,
       0,     0,     0,   495,     0,     0,   244,     0,     0,   387,
     504,   390,     0,     0,   382,     0,     0,     0,     0,   325,
     326,   324,   323,     0,     0,     0,   317,   333,     0,   503,
       0,   501,   385,   498,     0,     0,     0,   497,   398,     0,
       0,   618,   547,     0,    60,     0,     0,   281,     0,   295,
       0,     0,   511,   521,   510,     0,     0,     0,     0,     0,
       0,     0,     0,   302,   305,   308,     0,     0,     0,   459,
       0,   496,   250,   242,   253,   795,   795,     0,   393,     0,
       0,     0,     0,   756,   755,   328,   330,     0,   318,   464,
     470,     0,   502,   500,   499,   680,   332,    62,   282,   334,
       0,     0,     0,   523,   517,     0,     0,     0,     0,   313,
     797,   797,   793,   797,     0,     0,   460,     0,     0,     0,
       0,   391,     0,   758,   757,   383,     0,   319,   465,   471,
     475,     0,     0,     0,     0,   764,   767,     0,     0,     0,
       0,   779,   770,   245,     0,   800,   799,   394,     0,     0,
     476,     0,   297,   522,     0,   314,   784,   794,   788,   796,
     760,   759,   296,   524
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1279, -1279, -1279, -1279, -1279, -1279,   387,   947, -1279, -1279,
   -1279,  1029, -1279, -1279, -1279,   471,   989, -1279,   902, -1279,
   -1279,   954, -1279, -1279, -1279,  -296, -1279, -1279,  -124, -1279,
   -1279, -1279, -1279, -1279, -1279,   828, -1279, -1279,   969,   -73,
   -1279, -1279,   305, -1279,  -447,  -479,  -682, -1279, -1279, -1279,
   -1278, -1279, -1279,  -209,  -385, -1279,    77, -1279, -1211, -1279,
    -267,  -263, -1279, -1279, -1279, -1279,  -515,   -14, -1279, -1279,
   -1279, -1279, -1279,  -111,  -110,  -109, -1279,  -108, -1279, -1279,
   -1279,  1057, -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279,
   -1279, -1279, -1279, -1279, -1279, -1279, -1279,  -389,  -274,   183,
    -253, -1279,  -913,  -520, -1279,   490, -1279,  -391,   508, -1279,
   -1279, -1279, -1264, -1279, -1279, -1279, -1279, -1279, -1279, -1279,
   -1279, -1279,   590, -1279, -1279, -1279, -1279, -1279, -1279, -1279,
    -146,  -205,  -276,  -202,  -106, -1279, -1279, -1279, -1279, -1279,
     542, -1279,  -478, -1279, -1279,  -473, -1279, -1279,  -719,  -270,
    -593,  -193, -1279,  -380, -1279, -1279,  1023, -1279, -1279, -1279,
     492,   680,    14, -1279, -1279, -1279, -1279, -1279, -1279, -1279,
   -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279,
   -1279, -1279,  -490,  -166, -1279,   662, -1279, -1279, -1279, -1279,
   -1279, -1279, -1279, -1279,  -372, -1279, -1279, -1279, -1279, -1279,
   -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279,
   -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279, -1279,
   -1279, -1279,  -138, -1279, -1279, -1279, -1279, -1279, -1279, -1279,
   -1279, -1279, -1279, -1279, -1279,   668,  -833,  -805, -1279, -1279,
   -1279, -1279, -1279, -1279, -1279, -1279, -1279,  -831, -1279, -1279,
   -1279, -1279, -1279, -1279, -1279, -1279,  -609, -1279,  -282,  -639,
   -1279
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    16,   143,    55,    17,   164,   170,   639,   463,
     149,   464,   100,    19,    20,    45,    46,    47,    89,    21,
      39,    40,   558,   559,  1162,  1163,   560,  1165,   561,   562,
     563,   564,   565,   566,   567,   171,   172,    35,    36,    37,
      67,    68,    69,    70,    22,   330,   394,   210,    23,   112,
     211,   113,   156,   750,   986,   569,   395,   570,   710,  1364,
     927,   465,   836,  1295,   837,  1296,   572,   466,   573,   608,
     810,  1278,   467,   574,   575,   576,   577,   578,   579,   580,
     531,   581,   769,  1168,   980,   582,   468,   819,  1285,   820,
    1286,   822,  1287,   469,   813,  1281,   470,   650,  1317,   471,
    1225,  1226,   862,   752,   472,   790,   473,   734,   987,   584,
     474,   475,   853,   476,  1080,  1367,  1081,  1412,   477,   909,
    1245,   478,   651,  1228,  1418,  1230,  1419,  1324,  1440,   480,
     390,  1275,  1343,  1176,  1178,  1026,   591,   797,  1392,  1424,
     391,   392,   519,   705,   379,   524,   707,   380,  1084,   726,
     600,   409,   348,   349,   217,   344,    79,   122,    25,   161,
     396,    51,    52,   140,    91,    26,   116,   158,    27,   333,
     588,   589,  1023,   399,   215,   216,    77,   119,   401,    28,
     159,   342,   727,   481,   340,   268,   269,   697,   378,   270,
     491,  1118,   603,   376,   271,   410,  1031,   709,   489,  1116,
     411,  1032,   412,  1033,   488,  1115,   492,  1119,   493,  1246,
     494,  1121,   495,  1247,   496,  1123,   497,  1248,   498,  1126,
     499,  1128,   520,    29,   145,   274,   521,    30,   146,   275,
     525,    31,   144,   273,   516,   482,   859,   860,   483,   808,
    1276,   809,  1277,   828,  1291,   825,  1289,   642,   484,   826,
    1290,   485,   814,  1282,   823,  1288,   643,   486,  1409,   844,
     487
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      61,    71,   332,   801,   583,   513,  1040,   817,   942,   267,
     568,   847,  1072,   848,   699,   219,   701,   918,   703,   920,
     704,   922,   592,  1085,   655,   732,   706,  -114,    85,   123,
     124,   151,    38,   753,   754,   596,   747,   272,   503,  1330,
     944,     2,  1070,  1074,   505,    32,    33,     3,   937,   730,
      71,    71,    71,    48,   165,   166,  1221,   668,   669,    49,
    1157,   529,   346,    86,    90,  1166,  1222,   656,   657,   770,
       4,   652,     5,   388,     6,   666,    59,   347,   668,   669,
       7,  1273,    53,    72,    99,   802,   105,   106,   107,    48,
       8,    71,    71,    71,    71,    49,     9,   331,   532,    78,
      60,   937,   388,  1411,   530,   139,   938,   939,   597,   940,
     218,  1083,   941,   711,  1423,  1223,   771,    99,   598,    10,
    1224,   979,   937,   413,   414,    41,    42,    43,    54,   717,
     389,   533,   571,   461,   749,    62,   689,   690,   346,   218,
    1274,    11,    12,   420,   796,   345,  1444,  1203,  1437,   422,
     841,   733,  1407,   347,   150,   568,    44,   689,   690,   845,
     939,   266,   660,   661,    63,   366,   568,   599,    80,    81,
     666,    50,   125,   668,   669,   670,  1045,   126,  1048,   127,
     671,   939,   128,   223,   377,  1055,   428,   429,  1058,   656,
     657,    34,   367,   368,   504,   795,   180,   167,   402,   724,
     506,    87,   168,   938,   169,   938,   938,   128,   138,    92,
     224,   267,    88,   267,   725,   568,   945,    13,   507,   431,
     432,  1086,   331,   129,   508,   815,    15,   267,   937,  1315,
     930,   970,   903,   904,   824,    64,    14,   827,  1307,   937,
    1271,   267,    59,   816,   267,   267,   267,    15,  1087,  1369,
     982,   365,   689,   690,   369,   534,   829,   597,   370,    59,
    1177,   905,   915,   930,   331,   858,    60,   598,   937,    48,
     906,   212,   601,   602,   604,    49,   916,   571,  1083,   937,
     930,   108,   213,    60,   660,   661,  1028,   939,   571,  1438,
    1114,    59,   666,   453,   667,   668,   669,   670,   939,   923,
     849,    65,   671,   912,   973,   850,   109,  1184,   607,   800,
      66,   907,   908,   924,   978,    60,   599,   371,  1088,  1313,
    1269,   372,   267,   267,   373,   459,   267,   939,   267,   105,
     267,   107,   267,   736,  1088,   930,    81,   571,   939,   374,
     851,   745,   946,  1217,  1314,   375,   568,  1270,    59,   930,
     693,   694,   925,  1129,   698,   930,   700,   849,   702,  1371,
     377,   568,  1322,   266,   804,   266,   926,  1130,   501,   101,
     102,   103,    60,   613,   689,   690,  1328,    73,    74,   266,
      75,  -710,  1361,  1091,   590,  1305,  -710,   502,   110,   981,
     656,   657,   614,   266,   111,   334,   266,   266,   266,   335,
    -717,  -724,  -392,   266,  -710,  -717,  -724,  -392,    76,  1227,
     152,   153,   154,   155,   615,  1253,   568,  1129,   336,   337,
     338,   339,  1039,  -717,  -724,  -392,   879,  1135,   514,  1129,
    1279,  1205,  1104,   616,   522,   880,  1035,  1036,   622,   625,
     515,  1105,  1044,  1206,  1280,  1125,   523,  1050,  1051,   267,
    1053,  1054,  1127,  1056,  1057,   586,  1059,   623,   626,  1159,
     162,   163,   267,  1169,  1170,  1171,  1365,   587,   571,   807,
    1160,  1161,  1109,   377,   266,   266,    83,   805,   266,   377,
     266,  1110,   266,   571,   266,   660,   661,   791,   461,   749,
     866,   870,    81,   666,   792,   667,   668,   669,   670,  1219,
     403,  1308,   377,   671,  1340,   884,   913,   408,  1220,   571,
     571,   571,   571,   571,   571,   571,   571,   571,   571,   571,
     943,   571,   571,   571,   571,   571,   571,   656,   657,    93,
     966,  1302,    82,   406,   114,   854,   407,    99,   571,   408,
     115,  1396,  1397,   267,  1395,   377,   974,   220,   221,   914,
     976,   267,  1404,  1405,   267,  1137,    95,   568,   135,   136,
     117,   684,   685,   686,   687,   688,   118,    96,   120,  1067,
    1068,   928,   141,  1333,   121,   689,   690,   104,   142,   932,
      97,  1174,   933,  1284,  1370,    98,  1338,  1175,  1297,  1298,
     132,   366,   377,    88,   930,   930,   917,   812,   134,   137,
     377,   266,  1213,   377,   919,   377,   377,   921,   377,  1183,
    1188,   377,  1380,   147,   266,  1420,   160,   157,   367,   368,
     148,    86,   660,   661,   162,   163,   838,   839,   175,   179,
     666,   174,   667,   668,   669,   670,   105,   106,   107,   852,
     671,   267,   267,   267,   267,  1352,   176,   267,   267,   177,
     178,  1025,   105,   267,   267,   214,   267,   267,   218,   267,
     267,   327,   267,    41,    42,    43,   220,   221,   222,  1034,
     267,   267,  1037,   328,  1191,  1043,   329,   331,  1255,   571,
     369,  1208,   341,   351,   370,  1209,   352,  1268,   343,   366,
      56,    57,    58,   354,   353,   266,   357,  1439,   928,   928,
     686,   687,   688,   266,   355,   377,   266,  1233,  1212,   356,
     358,   359,   689,   690,   364,   360,   367,   368,   361,  1242,
     362,   363,   381,   385,   382,  1102,   383,   386,   393,   397,
     384,   656,   657,   398,   387,   751,   751,   751,   400,   405,
     490,   404,  1401,   371,   511,   526,   527,   372,  1391,  1060,
     373,   594,   528,   595,   761,  1155,   605,   267,   606,   612,
     611,  1428,  1429,   618,  1430,   374,   619,   583,   761,   617,
     696,   375,   621,   568,   267,   624,   620,   627,   369,   631,
     628,   597,   370,   632,   637,   638,   633,   634,   635,   522,
     636,   598,   654,   266,   266,   266,   266,  1421,   715,   266,
     266,   692,  1134,   653,    15,   266,   266,   712,   266,   266,
     695,   266,   266,   716,   266,  1294,   718,   719,   720,   721,
     722,   742,   266,   266,   658,   659,   660,   661,   662,   740,
     741,   663,   664,   665,   666,   756,   667,   668,   669,   670,
     599,   371,   757,   758,   671,   372,   672,   673,   373,   366,
     760,   763,   674,   764,   676,   765,  1318,   766,   767,   768,
     793,   711,   799,   374,   806,   843,   811,   861,   846,   375,
     733,   877,   929,   930,   936,   770,   367,   368,   935,   964,
     267,   967,   969,   366,   968,  1024,   975,   972,  1027,  1073,
    1088,  1076,  1092,  1117,  1090,   571,  1077,   679,   680,   681,
     682,   683,   684,   685,   686,   687,   688,  1094,  1078,   266,
     367,   368,  1079,   366,   267,  1089,   689,   690,   761,  1112,
    1120,  1082,  1122,  1093,   366,  1095,   266,  1096,  1254,  1097,
    1107,   761,  1124,  1132,  1108,  1133,  1167,  1173,   369,  1177,
     367,   368,   370,  1179,   479,  1185,   946,  1186,  1190,  1192,
    1257,   367,   368,   751,   500,  1235,  1193,  1198,  1201,  1204,
    1236,  1400,  1238,   267,   510,  1243,  1250,  1403,  1390,  1252,
    1304,  1267,   369,  1292,   711,  1309,   370,  1310,  1306,  1311,
    1312,  1342,  1331,  1339,  1346,   585,   761,  1347,  1348,  1350,
    1351,  1249,  1356,  1046,   593,  1357,  1358,  1363,  1368,  1376,
    1385,   371,   369,  1408,  1398,   372,   370,  1047,   373,  1402,
    1415,  1425,  1426,   369,  1431,  1432,  1433,   370,  1316,  1434,
    1435,  1446,  1443,   374,   629,   630,   840,   130,  1436,   375,
      18,  1447,   266,    84,  1448,   371,   173,   133,  1387,   372,
    1261,  1181,   373,   641,   644,   645,   646,   647,   648,  1334,
     131,   350,  1158,  1262,  1263,  1264,  1265,   374,    24,  1386,
    1375,   762,   267,   375,   267,   371,   266,  1344,  1393,   372,
    1272,  1182,   373,  1345,   728,  1394,   371,  1341,    94,   609,
     372,   794,  1189,   373,  1410,   610,     0,   374,     0,     0,
       0,     0,  1320,   375,     0,     0,     0,     0,   374,     0,
       0,   714,     0,     0,   375,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   266,     0,     0,   723,     0,
       0,     0,     0,     0,  1316,     0,   729,     0,     0,  1389,
     735,     0,     0,   738,   739,     0,     0,     0,     0,   744,
       0,   748,     0,     0,     0,   755,     0,     0,     0,     0,
     759,     0,     0,     0,   761,     0,     0,     0,   267,     0,
       0,     0,     0,     0,     0,     0,   751,   751,   751,     0,
       0,   761,     0,   761,   267,   761,     0,   761,     0,   761,
       0,   761,     0,   761,     0,   761,  1381,   761,     0,   761,
     803,   761,     0,     0,   761,     0,   761,     0,   761,     0,
     761,     0,   761,     0,   761,     0,   641,   818,     0,     0,
     821,     0,   761,     0,   266,     0,   266,     0,     0,     0,
       0,   830,   831,   832,   833,   834,   835,     0,     0,     0,
       0,     0,     0,     0,     0,  1453,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   871,   872,
       0,     0,   873,   874,   875,   876,     0,   878,     0,   881,
     882,   883,   885,   886,   887,   888,   889,   890,   892,   893,
     894,   895,   896,   897,   898,   899,   900,   901,   902,     0,
     910,   989,   991,   993,   995,   997,   999,  1001,  1003,  1005,
    1007,     0,  1010,  1012,  1014,  1016,  1018,  1020,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     266,   656,   657,     0,     0,     0,     0,     0,     0,   761,
       0,   735,     0,     0,     0,     0,   266,   965,     0,     0,
       0,     0,     0,     0,     0,  1164,   971,     0,     0,     0,
       0,     0,     0,     0,   656,   657,     0,     0,     0,     0,
     977,     0,     0,   852,     0,     0,     0,     0,     0,     0,
       0,     0,   988,   990,   992,   994,   996,   998,  1000,  1002,
    1004,  1006,  1008,  1009,  1011,  1013,  1015,  1017,  1019,  1021,
       0,     0,  1022,     0,     0,     0,     0,     0,     0,     0,
       0,  1029,     0,     0,     0,     0,     0,     0,   852,     0,
       0,     0,  1042,     0,   658,   659,   660,   661,   662,     0,
       0,   663,   664,   665,   666,     0,   667,   668,   669,   670,
       0,     0,     0,     0,   671,     0,   672,   673,     0,  1069,
       0,     0,  1071,   641,     0,     0,  1075,   658,   659,   660,
     661,   662,     0,     0,   663,   664,   665,   666,     0,   667,
     668,   669,   670,     0,     0,     0,     0,   671,     0,   672,
     673,     0,     0,     0,     0,   674,   675,   676,     0,     0,
       0,   677,     0,     0,     0,     0,     0,   679,   680,   681,
     682,   683,   684,   685,   686,   687,   688,     0,     0,     0,
       0,     0,     0,     0,  1106,     0,   689,   690,  1111,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   678,     0,
     679,   680,   681,   682,   683,   684,   685,   686,   687,   688,
    1075,     0,     0,     0,     0,  1131,     0,     0,     0,   689,
     690,     0,  1136,     0,     0,     0,     0,  1138,  1139,  1140,
    1141,  1142,  1143,  1144,  1145,  1146,  1147,  1148,  1149,  1150,
    1151,  1152,  1153,  1154,     0,     0,     0,  1156,     0,     0,
       0,     0,     0,     0,     0,   535,     0,     0,   413,   414,
       3,     0,   536,   537,   538,     0,   539,     0,   415,   416,
     417,   418,   419,     0,     0,     0,     0,     0,   420,   540,
     421,   541,   542,     0,   422,     0,     0,     0,     0,     0,
       0,   543,   423,     0,     0,   544,     0,   545,   424,     0,
       0,   425,     0,     8,   426,   546,     0,   547,   427,     0,
       0,   548,   549,     0,     0,     0,     0,     0,   550,     0,
       0,   428,   429,   231,   232,   233,     0,   235,   236,   237,
     238,   239,   430,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,     0,   253,   254,   255,     0,     0,
     258,   259,   260,   261,   431,   432,   433,   551,  1210,  1211,
       0,     0,  1214,  1215,  1216,     0,  1075,     0,     0,   434,
     435,     0,     0,  1229,     0,  1231,     0,  1234,     0,     0,
       0,     0,     0,  1237,   552,   553,   554,  1240,     0,     0,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,   436,   437,   438,   439,   440,     0,   441,     0,
     442,   443,   444,   445,   446,   447,   448,   449,    60,   555,
     451,   452,     0,     0,     0,     0,     0,     0,   453,   556,
     557,     0,     0,     0,     0,     0,   656,   657,   585,     0,
       0,     0,  1258,     0,  1266,   454,   455,   456,     0,    14,
       0,     0,   457,   458,     0,     0,     0,     0,     0,     0,
     459,   366,   460,     0,   461,   462,     0,     0,     0,     0,
       0,     0,   641,     0,     0,     0,     0,     0,     0,     0,
     366,     0,     0,     0,     0,     0,     0,     0,   367,   368,
       0,     0,   366,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   367,   368,     0,
       0,     0,     0,     0,     0,     0,  1325,     0,  1326,   367,
     368,     0,     0,  1329,     0,     0,     0,     0,     0,   658,
     659,   660,   661,  1332,     0,     0,     0,     0,     0,   666,
    1337,   667,   668,   669,   670,     0,     0,     0,     0,   671,
     369,   672,   673,     0,   370,     0,     0,     0,     0,     0,
    1349,     0,     0,     0,     0,  1353,  1354,  1355,     0,   369,
       0,     0,  1360,   370,     0,  1362,     0,   641,  1366,     0,
       0,   369,     0,     0,     0,   370,  1372,  1373,  1374,     0,
       0,     0,     0,     0,     0,  1378,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   684,   685,   686,
     687,   688,     0,   371,     0,     0,  1388,   372,     0,  1194,
     373,   689,   690,     0,     0,     0,     0,     0,     0,     0,
       0,   641,   371,     0,     0,   374,   372,     0,  1195,   373,
       0,   375,     0,     0,   371,     0,   366,     0,   372,  1413,
    1414,   373,     0,     0,   374,     0,     0,  1417,     0,     0,
     375,     0,     0,     0,     0,     0,   374,     0,     0,     0,
       0,  1422,   375,   367,   368,     0,     0,     0,  1427,     0,
     535,     0,     0,   413,   414,     3,     0,   536,   537,   538,
       0,   539,     0,   415,   416,   417,   418,   419,     0,     0,
       0,  1441,     0,   420,   540,   421,   541,   542,     0,   422,
       0,     0,     0,     0,  1449,     0,   543,   423,  1450,  1451,
     544,     0,   545,   424,     0,     0,   425,     0,     8,   426,
     546,     0,   547,   427,     0,   369,   548,   549,     0,   370,
       0,     0,     0,   550,     0,     0,   428,   429,   231,   232,
     233,     0,   235,   236,   237,   238,   239,   430,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,     0,
     253,   254,   255,     0,     0,   258,   259,   260,   261,   431,
     432,   433,   551,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   434,   435,     0,     0,   371,     0,
       0,     0,   372,     0,  1196,   373,     0,     0,     0,   552,
     553,   554,     0,     0,     0,     0,     0,     0,     0,    59,
     374,     0,     0,     0,     0,     0,   375,   436,   437,   438,
     439,   440,     0,   441,     0,   442,   443,   444,   445,   446,
     447,   448,   449,    60,   555,   451,   452,     0,     0,     0,
       0,     0,     0,   453,  1256,   557,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     454,   455,   456,     0,    14,     0,     0,   457,   458,     0,
       0,   413,   414,     0,     0,   459,     0,   460,     0,   461,
     462,   415,   416,   417,   418,   419,     0,     0,     0,     0,
       0,   420,     0,   421,     0,     0,     0,   422,     0,     0,
       0,     0,     0,     0,     0,   423,     0,     0,     0,     0,
       0,   424,     0,     0,   425,     0,     0,   426,     0,     0,
       0,   427,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   428,   429,   231,   232,   233,     0,
     235,   236,   237,   238,   239,   430,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,     0,   253,   254,
     255,     0,     0,   258,   259,   260,   261,   431,   432,   433,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,   435,     0,     0,     0,     0,     0,     0,
       0,   731,     0,     0,     0,     0,     0,   552,   553,   554,
       0,     0,     0,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,   436,   437,   438,   439,   440,
       0,   441,     0,   442,   443,   444,   445,   446,   447,   448,
     449,    60,   450,   451,   452,     0,     0,     0,     0,     0,
       0,   453,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   454,   455,
     456,     0,    14,     0,     0,   457,   458,     0,     0,   413,
     414,     0,     0,   459,     0,   460,     0,   461,   462,   415,
     416,   417,   418,   419,     0,     0,     0,     0,     0,   420,
       0,   421,     0,     0,     0,   422,     0,     0,     0,     0,
       0,     0,     0,   423,     0,     0,     0,     0,     0,   424,
       0,     0,   425,     0,     0,   426,     0,     0,     0,   427,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   428,   429,   231,   232,   233,     0,   235,   236,
     237,   238,   239,   430,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,     0,   253,   254,   255,     0,
       0,   258,   259,   260,   261,   431,   432,   433,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     434,   435,     0,     0,     0,     0,     0,     0,     0,   746,
       0,     0,     0,     0,     0,   552,   553,   554,     0,     0,
       0,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     0,     0,   436,   437,   438,   439,   440,     0,   441,
       0,   442,   443,   444,   445,   446,   447,   448,   449,    60,
     450,   451,   452,     0,     0,     0,     0,     0,     0,   453,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   454,   455,   456,     0,
      14,     0,     0,   457,   458,     0,     0,   413,   414,     0,
       0,   459,     0,   460,     0,   461,   462,   415,   416,   417,
     418,   419,     0,     0,     0,     0,     0,   420,  1259,   421,
     541,     0,     0,   422,     0,     0,     0,     0,     0,     0,
       0,   423,     0,     0,     0,     0,     0,   424,     0,     0,
     425,     0,     0,   426,   546,     0,     0,   427,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     428,   429,   231,   232,   233,     0,   235,   236,   237,   238,
     239,   430,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,     0,   253,   254,   255,     0,     0,   258,
     259,   260,   261,   431,   432,   433,  1260,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,   435,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,   436,   437,   438,   439,   440,     0,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,    60,   450,   451,
     452,     0,     0,     0,     0,     0,     0,   453,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   454,   455,   456,     0,    14,     0,
       0,   457,   458,     0,     0,   413,   414,     0,     0,   459,
       0,   460,     0,   461,   462,   415,   416,   417,   418,   419,
       0,     0,     0,     0,     0,   420,     0,   421,     0,     0,
       0,   422,     0,     0,     0,     0,     0,     0,     0,   423,
       0,     0,     0,     0,     0,   424,     0,     0,   425,     0,
       0,   426,     0,     0,     0,   427,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   428,   429,
     231,   232,   233,     0,   235,   236,   237,   238,   239,   430,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,     0,   253,   254,   255,     0,     0,   258,   259,   260,
     261,   431,   432,   433,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   434,   435,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   552,   553,   554,     0,     0,     0,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,   436,
     437,   438,   439,   440,     0,   441,     0,   442,   443,   444,
     445,   446,   447,   448,   449,    60,   450,   451,   452,     0,
       0,     0,     0,     0,     0,   453,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   454,   455,   456,     0,    14,     0,     0,   457,
     458,     0,     0,   413,   414,     0,     0,   459,     0,   460,
       0,   461,   462,   415,   416,   417,   418,   419,     0,     0,
       0,     0,     0,   420,     0,   421,     0,     0,     0,   422,
       0,     0,     0,     0,     0,     0,     0,   423,     0,     0,
       0,     0,     0,   424,     0,     0,   425,     0,     0,   426,
       0,     0,     0,   427,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   428,   429,   231,   232,
     233,     0,   235,   236,   237,   238,   239,   430,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,     0,
     253,   254,   255,     0,     0,   258,   259,   260,   261,   431,
     432,   433,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   434,   435,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   983,
     984,   985,     0,     0,     0,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     0,     0,   436,   437,   438,
     439,   440,     0,   441,     0,   442,   443,   444,   445,   446,
     447,   448,   449,    60,   450,   451,   452,     0,     0,     0,
       0,     0,     0,   453,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     454,   455,   456,     0,    14,     0,     0,   457,   458,     0,
       0,   413,   414,     0,     0,   459,     0,   460,     0,   461,
     462,   415,   416,   417,   418,   419,     0,     0,     0,     0,
       0,   420,     0,   421,     0,     0,     0,   422,     0,     0,
       0,     0,     0,     0,     0,   423,     0,     0,     0,     0,
       0,   424,     0,     0,   425,     0,     0,   426,     0,     0,
       0,   427,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   428,   429,   231,   232,   233,     0,
     235,   236,   237,   238,   239,   430,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,     0,   253,   254,
     255,     0,     0,   258,   259,   260,   261,   431,   432,   433,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,   435,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,   436,   437,   438,   439,   440,
       0,   441,   854,   442,   443,   444,   445,   446,   447,   448,
     449,   855,   450,   451,   452,     0,     0,     0,     0,     0,
       0,   453,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   454,   455,
     456,     0,    14,     0,     0,   457,   458,     0,     0,   413,
     414,     0,     0,   856,     0,   460,   857,   461,   462,   415,
     416,   417,   418,   419,     0,     0,     0,     0,     0,   420,
       0,   421,     0,     0,     0,   422,     0,     0,     0,     0,
       0,     0,     0,   423,     0,     0,     0,     0,     0,   424,
       0,     0,   425,     0,     0,   426,     0,     0,     0,   427,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   428,   429,   231,   232,   233,     0,   235,   236,
     237,   238,   239,   430,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,     0,   253,   254,   255,     0,
       0,   258,   259,   260,   261,   431,   432,   433,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     434,   435,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     0,     0,   436,   437,   438,   439,   440,   366,   441,
     854,   442,   443,   444,   445,   446,   447,   448,   449,   855,
     450,   451,   452,     0,     0,     0,     0,     0,   366,   453,
       0,     0,     0,     0,     0,   367,   368,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   454,   455,   456,     0,
      14,     0,     0,   457,   458,   367,   368,     0,     0,   413,
     414,   459,     0,   460,  1038,   461,   462,   649,     0,   415,
     416,   417,   418,   419,     0,     0,     0,     0,     0,   420,
       0,   421,     0,     0,     0,   422,     0,     0,     0,     0,
       0,     0,     0,   423,     0,     0,     0,   369,     0,   424,
       0,   370,   425,     0,     0,   426,     0,     0,     0,   427,
       0,     0,     0,     0,     0,     0,     0,   369,     0,     0,
       0,   370,   428,   429,   231,   232,   233,     0,   235,   236,
     237,   238,   239,   430,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,     0,   253,   254,   255,     0,
       0,   258,   259,   260,   261,   431,   432,   433,     0,     0,
     371,     0,     0,     0,   372,     0,  1197,   373,     0,     0,
     434,   435,     0,     0,     0,     0,     0,     0,     0,     0,
     371,     0,   374,     0,   372,     0,  1199,   373,   375,     0,
       0,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     0,   374,   436,   437,   438,   439,   440,   375,   441,
     854,   442,   443,   444,   445,   446,   447,   448,   449,   855,
     450,   451,   452,     0,     0,     0,     0,     0,     0,   453,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   454,   455,   456,     0,
      14,     0,     0,   457,   458,     0,     0,   413,   414,     0,
       0,   459,     0,   460,     0,   461,   462,   415,   416,   417,
     418,   419,     0,     0,     0,     0,     0,   420,     0,   421,
       0,     0,   366,   422,     0,     0,     0,     0,     0,     0,
       0,   423,     0,     0,     0,     0,     0,   424,     0,     0,
     425,     0,     0,   426,     0,     0,     0,   427,     0,   367,
     368,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     428,   429,   231,   232,   233,     0,   235,   236,   237,   238,
     239,   430,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,     0,   253,   254,   255,     0,     0,   258,
     259,   260,   261,   431,   432,   433,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,   435,
       0,   369,     0,     0,     0,   370,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,   436,   437,   438,   439,   440,     0,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,    60,   450,   451,
     452,     0,     0,     0,     0,     0,   366,   453,     0,     0,
       0,     0,     0,     0,   371,     0,     0,     0,   372,     0,
    1200,   373,     0,     0,   454,   455,   456,     0,    14,     0,
       0,   457,   458,   367,   368,     0,   374,   413,   414,   459,
     509,   460,   375,   461,   462,   640,     0,   415,   416,   417,
     418,   419,     0,     0,     0,     0,     0,   420,     0,   421,
       0,     0,   366,   422,     0,     0,     0,     0,     0,     0,
       0,   423,     0,     0,     0,     0,     0,   424,     0,     0,
     425,     0,     0,   426,     0,     0,     0,   427,     0,   367,
     368,     0,     0,     0,     0,   369,     0,     0,     0,   370,
     428,   429,   231,   232,   233,     0,   235,   236,   237,   238,
     239,   430,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,     0,   253,   254,   255,     0,     0,   258,
     259,   260,   261,   431,   432,   433,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,   435,
       0,   369,     0,     0,     0,   370,     0,     0,   371,     0,
       0,     0,   372,     0,  1202,   373,     0,     0,     0,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
     374,   436,   437,   438,   439,   440,   375,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,    60,   450,   451,
     452,     0,     0,     0,     0,     0,   366,   453,     0,     0,
       0,     0,     0,     0,   371,     0,     0,     0,   372,     0,
    1283,   373,     0,     0,   454,   455,   456,     0,    14,     0,
       0,   457,   458,   367,   368,     0,   374,   413,   414,   459,
       0,   460,   375,   461,   462,   649,     0,   415,   416,   417,
     418,   419,     0,     0,     0,     0,     0,   420,     0,   421,
       0,     0,     0,   422,     0,     0,     0,     0,     0,     0,
       0,   423,     0,     0,     0,     0,     0,   424,     0,     0,
     425,     0,     0,   426,     0,     0,     0,   427,     0,     0,
       0,     0,     0,     0,     0,   369,     0,     0,     0,   370,
     428,   429,   231,   232,   233,     0,   235,   236,   237,   238,
     239,   430,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,     0,   253,   254,   255,     0,     0,   258,
     259,   260,   261,   431,   432,   433,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,   435,
       0,     0,     0,     0,     0,     0,     0,     0,   371,     0,
       0,     0,   372,     0,  1301,   373,     0,     0,     0,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
     374,   436,   437,   438,   439,   440,   375,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,    60,   450,   451,
     452,     0,     0,     0,     0,     0,     0,   453,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   454,   455,   456,     0,    14,     0,
       0,   457,   458,     0,     0,   413,   414,     0,     0,   459,
       0,   460,     0,   461,   462,   415,   416,   417,   418,   419,
       0,     0,     0,     0,     0,   420,     0,   421,     0,     0,
       0,   422,     0,     0,     0,     0,     0,     0,     0,   423,
       0,     0,     0,     0,     0,   424,     0,     0,   425,     0,
       0,   426,     0,     0,     0,   427,     0,     0,     0,     0,
       0,   737,     0,     0,     0,     0,     0,     0,   428,   429,
     231,   232,   233,     0,   235,   236,   237,   238,   239,   430,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,     0,   253,   254,   255,     0,     0,   258,   259,   260,
     261,   431,   432,   433,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   434,   435,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,   436,
     437,   438,   439,   440,     0,   441,     0,   442,   443,   444,
     445,   446,   447,   448,   449,    60,   450,   451,   452,     0,
       0,     0,     0,     0,     0,   453,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   454,   455,   456,     0,    14,     0,     0,   457,
     458,     0,     0,   413,   414,     0,     0,   459,     0,   460,
       0,   461,   462,   415,   416,   417,   418,   419,     0,     0,
       0,     0,     0,   420,     0,   421,     0,     0,     0,   422,
       0,     0,     0,     0,     0,     0,     0,   423,     0,     0,
       0,     0,     0,   424,     0,     0,   425,     0,     0,   426,
       0,     0,     0,   427,     0,     0,   743,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   428,   429,   231,   232,
     233,     0,   235,   236,   237,   238,   239,   430,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,     0,
     253,   254,   255,     0,     0,   258,   259,   260,   261,   431,
     432,   433,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   434,   435,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     0,     0,   436,   437,   438,
     439,   440,     0,   441,     0,   442,   443,   444,   445,   446,
     447,   448,   449,    60,   450,   451,   452,     0,     0,     0,
       0,     0,     0,   453,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     454,   455,   456,     0,    14,     0,     0,   457,   458,     0,
       0,   413,   414,     0,     0,   459,     0,   460,     0,   461,
     462,   415,   416,   417,   418,   419,     0,     0,     0,     0,
       0,   420,     0,   421,     0,     0,     0,   422,     0,     0,
       0,     0,     0,     0,     0,   423,     0,     0,     0,     0,
       0,   424,     0,     0,   425,     0,     0,   426,     0,     0,
       0,   427,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   428,   429,   231,   232,   233,     0,
     235,   236,   237,   238,   239,   430,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,     0,   253,   254,
     255,     0,     0,   258,   259,   260,   261,   431,   432,   433,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,   435,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,   436,   437,   438,   439,   440,
       0,   441,     0,   442,   443,   444,   445,   446,   447,   448,
     449,    60,   450,   451,   452,     0,     0,     0,     0,     0,
       0,   453,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   637,     0,   454,   455,
     456,     0,    14,     0,     0,   457,   458,     0,     0,   413,
     414,     0,     0,   459,     0,   460,     0,   461,   462,   415,
     416,   417,   418,   419,     0,     0,   891,     0,     0,   420,
       0,   421,     0,     0,     0,   422,     0,     0,     0,     0,
       0,     0,     0,   423,     0,     0,     0,     0,     0,   424,
       0,     0,   425,     0,     0,   426,     0,     0,     0,   427,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   428,   429,   231,   232,   233,     0,   235,   236,
     237,   238,   239,   430,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,     0,   253,   254,   255,     0,
       0,   258,   259,   260,   261,   431,   432,   433,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     434,   435,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     0,     0,   436,   437,   438,   439,   440,     0,   441,
       0,   442,   443,   444,   445,   446,   447,   448,   449,    60,
     450,   451,   452,     0,     0,     0,     0,     0,     0,   453,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   454,   455,   456,     0,
      14,     0,     0,   457,   458,     0,     0,   413,   414,     0,
       0,   459,     0,   460,     0,   461,   462,   415,   416,   417,
     418,   419,     0,     0,     0,     0,     0,   420,     0,   421,
       0,     0,     0,   422,     0,     0,     0,     0,     0,     0,
       0,   423,     0,     0,     0,     0,     0,   424,     0,     0,
     425,     0,     0,   426,     0,     0,     0,   427,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     428,   429,   231,   232,   233,     0,   235,   236,   237,   238,
     239,   430,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,     0,   253,   254,   255,     0,     0,   258,
     259,   260,   261,   431,   432,   433,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,   435,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,   436,   437,   438,   439,   440,     0,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,    60,   450,   451,
     452,     0,     0,     0,     0,     0,     0,   453,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   454,   455,   456,     0,    14,     0,
       0,   457,   458,     0,     0,   413,   414,     0,     0,   459,
       0,   460,   911,   461,   462,   415,   416,   417,   418,   419,
       0,     0,     0,     0,     0,   420,     0,   421,     0,     0,
       0,   422,     0,     0,     0,     0,     0,     0,     0,   423,
       0,     0,     0,     0,     0,   424,     0,     0,   425,     0,
       0,   426,     0,     0,     0,   427,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   428,   429,
     231,   232,   233,     0,   235,   236,   237,   238,   239,   430,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,     0,   253,   254,   255,     0,     0,   258,   259,   260,
     261,   431,   432,   433,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   434,   435,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,   436,
     437,   438,   439,   440,     0,   441,     0,   442,   443,   444,
     445,   446,   447,   448,   449,    60,   450,   451,   452,     0,
       0,     0,     0,     0,     0,   453,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1041,     0,   454,   455,   456,     0,    14,     0,     0,   457,
     458,     0,     0,   413,   414,     0,     0,   459,     0,   460,
       0,   461,   462,   415,   416,   417,   418,   419,     0,     0,
       0,     0,     0,   420,     0,   421,     0,     0,     0,   422,
       0,     0,     0,     0,     0,     0,     0,   423,     0,     0,
       0,     0,     0,   424,     0,     0,   425,     0,     0,   426,
       0,     0,     0,   427,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   428,   429,   231,   232,
     233,     0,   235,   236,   237,   238,   239,   430,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,     0,
     253,   254,   255,     0,     0,   258,   259,   260,   261,   431,
     432,   433,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   434,   435,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     0,     0,   436,   437,   438,
     439,   440,     0,   441,     0,   442,   443,   444,   445,   446,
     447,   448,   449,    60,   450,   451,   452,     0,     0,     0,
       0,     0,     0,   453,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     454,   455,   456,     0,    14,     0,     0,   457,   458,     0,
       0,   413,   414,     0,     0,   459,     0,   460,  1232,   461,
     462,   415,   416,   417,   418,   419,     0,     0,     0,     0,
       0,   420,     0,   421,     0,     0,     0,   422,     0,     0,
       0,     0,     0,     0,     0,   423,     0,     0,     0,     0,
       0,   424,     0,     0,   425,     0,     0,   426,     0,     0,
       0,   427,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   428,   429,   231,   232,   233,     0,
     235,   236,   237,   238,   239,   430,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,     0,   253,   254,
     255,     0,     0,   258,   259,   260,   261,   431,   432,   433,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,   435,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,   436,   437,   438,   439,   440,
       0,   441,     0,   442,   443,   444,   445,   446,   447,   448,
     449,    60,   450,   451,   452,     0,     0,     0,     0,     0,
       0,   453,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   454,   455,
     456,     0,    14,     0,     0,   457,   458,     0,     0,   413,
     414,     0,     0,   459,     0,   460,  1241,   461,   462,   415,
     416,   417,   418,   419,     0,     0,     0,     0,     0,   420,
       0,   421,     0,     0,     0,   422,     0,     0,     0,     0,
       0,     0,     0,   423,     0,     0,     0,     0,     0,   424,
       0,     0,   425,     0,     0,   426,     0,     0,     0,   427,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   428,   429,   231,   232,   233,     0,   235,   236,
     237,   238,   239,   430,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,     0,   253,   254,   255,     0,
       0,   258,   259,   260,   261,   431,   432,   433,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     434,   435,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     0,     0,   436,   437,   438,   439,   440,     0,   441,
       0,   442,   443,   444,   445,   446,   447,   448,   449,    60,
     450,   451,   452,     0,     0,     0,     0,     0,     0,   453,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   454,   455,   456,     0,
      14,     0,     0,   457,   458,     0,     0,   413,   414,     0,
       0,   459,     0,   460,  1293,   461,   462,   415,   416,   417,
     418,   419,     0,     0,     0,     0,     0,   420,     0,   421,
       0,     0,     0,   422,     0,     0,     0,     0,     0,     0,
       0,   423,     0,     0,     0,     0,     0,   424,     0,     0,
     425,     0,     0,   426,     0,     0,     0,   427,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     428,   429,   231,   232,   233,     0,   235,   236,   237,   238,
     239,   430,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,     0,   253,   254,   255,     0,     0,   258,
     259,   260,   261,   431,   432,   433,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,   435,
       0,     0,     0,     0,     0,     0,     0,  1335,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,   436,   437,   438,   439,   440,     0,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,    60,   450,   451,
     452,     0,     0,     0,     0,     0,     0,   453,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   454,   455,   456,     0,    14,     0,
       0,   457,   458,     0,     0,   413,   414,     0,     0,   459,
       0,   460,     0,   461,   462,   415,   416,   417,   418,   419,
       0,     0,     0,     0,     0,   420,     0,   421,     0,     0,
       0,   422,     0,     0,     0,     0,     0,     0,     0,   423,
       0,     0,     0,     0,     0,   424,     0,     0,   425,     0,
       0,   426,     0,     0,     0,   427,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   428,   429,
     231,   232,   233,     0,   235,   236,   237,   238,   239,   430,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,     0,   253,   254,   255,     0,     0,   258,   259,   260,
     261,   431,   432,   433,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   434,   435,     0,     0,
       0,     0,     0,     0,     0,  1336,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,   436,
     437,   438,   439,   440,     0,   441,     0,   442,   443,   444,
     445,   446,   447,   448,   449,    60,   450,   451,   452,     0,
       0,     0,     0,     0,     0,   453,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   454,   455,   456,     0,    14,     0,     0,   457,
     458,     0,     0,   413,   414,     0,     0,   459,     0,   460,
       0,   461,   462,   415,   416,   417,   418,   419,     0,     0,
       0,     0,     0,   420,     0,   421,     0,     0,     0,   422,
       0,     0,     0,     0,     0,     0,     0,   423,     0,     0,
       0,     0,     0,   424,     0,     0,   425,     0,     0,   426,
       0,     0,     0,   427,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   428,   429,   231,   232,
     233,     0,   235,   236,   237,   238,   239,   430,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,     0,
     253,   254,   255,     0,     0,   258,   259,   260,   261,   431,
     432,   433,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   434,   435,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     0,     0,   436,   437,   438,
     439,   440,     0,   441,     0,   442,   443,   444,   445,   446,
     447,   448,   449,    60,   450,   451,   452,     0,     0,     0,
       0,     0,     0,   453,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     454,   455,   456,     0,    14,     0,     0,   457,   458,     0,
       0,   413,   414,     0,     0,   459,     0,   460,  1359,   461,
     462,   415,   416,   417,   418,   419,     0,     0,     0,     0,
       0,   420,     0,   421,     0,     0,     0,   422,     0,     0,
       0,     0,     0,     0,     0,   423,     0,     0,     0,     0,
       0,   424,     0,     0,   425,     0,     0,   426,     0,     0,
       0,   427,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   428,   429,   231,   232,   233,     0,
     235,   236,   237,   238,   239,   430,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,     0,   253,   254,
     255,     0,     0,   258,   259,   260,   261,   431,   432,   433,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,   435,     0,     0,     0,     0,     0,     0,
       0,  1377,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,   436,   437,   438,   439,   440,
       0,   441,     0,   442,   443,   444,   445,   446,   447,   448,
     449,    60,   450,   451,   452,     0,     0,     0,     0,     0,
       0,   453,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   454,   455,
     456,     0,    14,     0,     0,   457,   458,     0,     0,   413,
     414,     0,     0,   459,     0,   460,     0,   461,   462,   415,
     416,   417,   418,   419,     0,     0,     0,     0,     0,   420,
       0,   421,     0,     0,     0,   422,     0,     0,     0,     0,
       0,     0,     0,   423,     0,     0,     0,     0,     0,   424,
       0,     0,   425,     0,     0,   426,     0,     0,     0,   427,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   428,   429,   231,   232,   233,     0,   235,   236,
     237,   238,   239,   430,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,     0,   253,   254,   255,     0,
       0,   258,   259,   260,   261,   431,   432,   433,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     434,   435,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     0,     0,   436,   437,   438,   439,   440,     0,   441,
       0,   442,   443,   444,   445,   446,   447,   448,   449,    60,
     450,   451,   452,     0,     0,     0,     0,     0,     0,   453,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   454,   455,   456,     0,
      14,     0,     0,   457,   458,     0,     0,   413,   414,     0,
       0,   459,     0,   460,     0,   461,   462,   415,   416,   417,
     418,   419,     0,     0,     0,     0,     0,   420,     0,   421,
       0,     0,   366,   422,     0,     0,     0,     0,     0,     0,
       0,   423,     0,     0,     0,     0,     0,   424,     0,     0,
     425,     0,     0,   426,     0,     0,     0,   427,     0,   367,
     368,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     428,   429,   231,   232,   233,     0,   235,   236,   237,   238,
     239,   430,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,     0,   253,   254,   255,     0,     0,   258,
     259,   260,   261,   431,   432,   433,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,   435,
     -67,   369,     0,     0,     0,   370,     0,     0,     0,     0,
       0,   656,   657,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,   436,   437,   438,   439,   440,     0,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,    60,   450,   451,
     452,   656,   657,     0,     0,     0,     0,   453,     0,     0,
       0,     0,     0,     0,   371,     0,     0,     0,   372,     0,
    1379,   373,     0,     0,   454,   455,   456,     0,    14,     0,
       0,   457,   458,     0,     0,     0,   374,     0,     0,  1218,
       0,   460,   375,   461,   462,     0,   772,   773,   774,   775,
     776,   777,   778,   779,   658,   659,   660,   661,   662,   780,
     781,   663,   664,   665,   666,   782,   667,   668,   669,   670,
       0,     0,     0,     0,   671,   783,   672,   673,   784,   785,
       0,     0,   674,   675,   676,   786,   787,   788,   677,     0,
       0,     0,     0,     0,   658,   659,   660,   661,   662,     0,
       0,   663,   664,   665,   666,     0,   667,   668,   669,   670,
       0,     0,     0,     0,   671,     0,   672,   673,     0,     0,
       0,     0,     0,     0,   789,   678,     0,   679,   680,   681,
     682,   683,   684,   685,   686,   687,   688,   225,     0,     0,
       0,     0,     0,   226,     0,     0,   689,   690,     0,   227,
       0,   461,   749,     0,     0,     0,     0,     0,     0,   228,
       0,     0,     0,     0,     0,     0,     0,   229,   680,   681,
     682,   683,   684,   685,   686,   687,   688,     0,     0,     0,
       0,     0,   230,     0,     0,     0,   689,   690,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   225,     0,     0,     0,     0,     0,   226,
       0,     0,     0,     0,     0,   227,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   228,     0,     0,     0,    59,
       0,     0,     0,   229,     0,     0,     0,     0,     0,     0,
       0,     0,   264,     0,     0,     0,     0,     0,   230,     0,
       0,     0,     0,    60,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   265,
       0,   512,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   225,     0,     0,     0,     0,     0,   226,     0,     0,
       0,     0,     0,   227,     0,    59,     0,     0,     0,     0,
       0,     0,     0,   228,     0,     0,     0,     0,   264,     0,
       0,   229,     0,     0,     0,     0,     0,     0,     0,   517,
       0,     0,     0,     0,     0,     0,   230,     0,     0,     0,
       0,   518,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   265,     0,     0,     0,     0,
       0,     0,     0,     0,   656,   657,     0,   225,     0,     0,
       0,     0,     0,   226,     0,     0,     0,     0,     0,   227,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   228,
       0,     0,     0,    59,     0,     0,     0,   229,     0,     0,
       0,     0,     0,     0,     0,     0,   264,     0,     0,     0,
       0,     0,   230,     0,     0,     0,     0,    60,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,     0,     0,     0,     0,     0,     0,   658,   659,   660,
     661,   662,     0,   265,   663,   664,   665,   666,     0,   667,
     668,   669,   670,     0,     0,   656,   657,   671,     0,   672,
     673,     0,     0,     0,     0,   674,   675,   676,     0,    59,
       0,   677,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   264,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   517,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   678,     0,
     679,   680,   681,   682,   683,   684,   685,   686,   687,   688,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   689,
     690,     0,     0,     0,   461,   749,     0,     0,     0,   265,
     947,   948,   949,   950,   951,   952,   953,   954,   658,   659,
     660,   661,   662,   955,   956,   663,   664,   665,   666,   957,
     667,   668,   669,   670,  -342,   656,   657,     0,   671,   783,
     672,   673,   958,   959,     0,     0,   674,   675,   676,   960,
     961,   962,   677,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   963,   678,
       0,   679,   680,   681,   682,   683,   684,   685,   686,   687,
     688,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     689,   690,     0,     0,     0,   461,   749,     0,     0,     0,
     947,   948,   949,   950,   951,   952,   953,   954,   658,   659,
     660,   661,   662,   955,   956,   663,   664,   665,   666,   957,
     667,   668,   669,   670,   656,   657,     0,     0,   671,   783,
     672,   673,   958,   959,     0,     0,   674,   675,   676,   960,
     961,   962,   677,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   656,   657,     0,     0,     0,     0,     0,     0,
       0,  1030,     0,     0,     0,     0,     0,     0,   963,   678,
       0,   679,   680,   681,   682,   683,   684,   685,   686,   687,
     688,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     689,   690,     0,     0,     0,   461,   749,     0,     0,   947,
     948,   949,   950,   951,   952,   953,   954,   658,   659,   660,
     661,   662,   955,   956,   663,   664,   665,   666,   957,   667,
     668,   669,   670,     0,     0,     0,     0,   671,   783,   672,
     673,   958,   959,     0,     0,   674,   675,   676,   960,   961,
     962,   677,     0,     0,     0,   658,   659,   660,   661,   662,
     656,   657,   663,   664,   665,   666,     0,   667,   668,   669,
     670,     0,     0,     0,     0,   671,     0,   672,   673,     0,
    1180,     0,     0,   674,   675,   676,     0,   963,   678,   677,
     679,   680,   681,   682,   683,   684,   685,   686,   687,   688,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   689,
     690,     0,     0,     0,   461,   749,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   678,     0,   679,   680,
     681,   682,   683,   684,   685,   686,   687,   688,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   689,   690,     0,
       0,   691,     0,   658,   659,   660,   661,   662,   656,   657,
     663,   664,   665,   666,     0,   667,   668,   669,   670,     0,
       0,     0,     0,   671,     0,   672,   673,     0,     0,     0,
       0,   674,   675,   676,     0,     0,     0,   677,     0,     0,
       0,     0,     0,   656,   657,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   678,     0,   679,   680,   681,   682,
     683,   684,   685,   686,   687,   688,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   689,   690,     0,     0,   708,
       0,   658,   659,   660,   661,   662,     0,     0,   663,   664,
     665,   666,     0,   667,   668,   669,   670,     0,     0,     0,
       0,   671,     0,   672,   673,     0,     0,     0,     0,   674,
     675,   676,     0,     0,     0,   677,   658,   659,   660,   661,
     662,   656,   657,   663,   664,   665,   666,     0,   667,   668,
     669,   670,     0,     0,     0,     0,   671,     0,   672,   673,
       0,     0,     0,     0,   674,   675,   676,     0,     0,     0,
     677,     0,   678,     0,   679,   680,   681,   682,   683,   684,
     685,   686,   687,   688,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   689,   690,     0,     0,   798,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   678,     0,   679,
     680,   681,   682,   683,   684,   685,   686,   687,   688,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   689,   690,
       0,     0,   934,     0,   658,   659,   660,   661,   662,   656,
     657,   663,   664,   665,   666,     0,   667,   668,   669,   670,
       0,     0,     0,     0,   671,     0,   672,   673,     0,     0,
       0,     0,   674,   675,   676,     0,     0,     0,   677,     0,
       0,     0,     0,     0,   656,   657,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   678,     0,   679,   680,   681,
     682,   683,   684,   685,   686,   687,   688,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   689,   690,     0,     0,
    1049,     0,   658,   659,   660,   661,   662,     0,     0,   663,
     664,   665,   666,     0,   667,   668,   669,   670,     0,     0,
       0,     0,   671,     0,   672,   673,     0,     0,     0,     0,
     674,   675,   676,     0,     0,     0,   677,   658,   659,   660,
     661,   662,   656,   657,   663,   664,   665,   666,     0,   667,
     668,   669,   670,     0,     0,     0,     0,   671,     0,   672,
     673,     0,     0,     0,     0,   674,   675,   676,     0,     0,
       0,   677,     0,   678,     0,   679,   680,   681,   682,   683,
     684,   685,   686,   687,   688,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   689,   690,     0,     0,  1052,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   678,     0,
     679,   680,   681,   682,   683,   684,   685,   686,   687,   688,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   689,
     690,     0,     0,  1061,     0,   658,   659,   660,   661,   662,
     656,   657,   663,   664,   665,   666,     0,   667,   668,   669,
     670,     0,     0,     0,     0,   671,     0,   672,   673,     0,
       0,     0,     0,   674,   675,   676,     0,     0,     0,   677,
       0,     0,     0,     0,     0,   656,   657,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   678,     0,   679,   680,
     681,   682,   683,   684,   685,   686,   687,   688,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   689,   690,     0,
       0,  1062,     0,   658,   659,   660,   661,   662,     0,     0,
     663,   664,   665,   666,     0,   667,   668,   669,   670,     0,
       0,     0,     0,   671,     0,   672,   673,     0,     0,     0,
       0,   674,   675,   676,     0,     0,     0,   677,   658,   659,
     660,   661,   662,   656,   657,   663,   664,   665,   666,     0,
     667,   668,   669,   670,     0,     0,     0,     0,   671,     0,
     672,   673,     0,     0,     0,     0,   674,   675,   676,     0,
       0,     0,   677,     0,   678,     0,   679,   680,   681,   682,
     683,   684,   685,   686,   687,   688,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   689,   690,     0,     0,  1063,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   678,
       0,   679,   680,   681,   682,   683,   684,   685,   686,   687,
     688,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     689,   690,     0,     0,  1064,     0,   658,   659,   660,   661,
     662,   656,   657,   663,   664,   665,   666,     0,   667,   668,
     669,   670,     0,     0,     0,     0,   671,     0,   672,   673,
       0,     0,     0,     0,   674,   675,   676,     0,     0,     0,
     677,     0,     0,     0,     0,     0,   656,   657,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   678,     0,   679,
     680,   681,   682,   683,   684,   685,   686,   687,   688,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   689,   690,
       0,     0,  1065,     0,   658,   659,   660,   661,   662,     0,
       0,   663,   664,   665,   666,     0,   667,   668,   669,   670,
       0,     0,     0,     0,   671,     0,   672,   673,     0,     0,
       0,     0,   674,   675,   676,     0,     0,     0,   677,   658,
     659,   660,   661,   662,   656,   657,   663,   664,   665,   666,
       0,   667,   668,   669,   670,     0,     0,     0,     0,   671,
       0,   672,   673,     0,     0,     0,     0,   674,   675,   676,
       0,     0,     0,   677,     0,   678,     0,   679,   680,   681,
     682,   683,   684,   685,   686,   687,   688,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   689,   690,     0,     0,
    1066,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     678,     0,   679,   680,   681,   682,   683,   684,   685,   686,
     687,   688,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   689,   690,     0,     0,  1187,     0,   658,   659,   660,
     661,   662,   656,   657,   663,   664,   665,   666,     0,   667,
     668,   669,   670,     0,     0,     0,     0,   671,     0,   672,
     673,     0,     0,     0,     0,   674,   675,   676,     0,     0,
       0,   677,     0,     0,     0,     0,     0,   656,   657,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   678,     0,
     679,   680,   681,   682,   683,   684,   685,   686,   687,   688,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   689,
     690,     0,     0,  1251,     0,   658,   659,   660,   661,   662,
       0,     0,   663,   664,   665,   666,     0,   667,   668,   669,
     670,     0,     0,     0,     0,   671,     0,   672,   673,     0,
       0,     0,     0,   674,   675,   676,     0,     0,     0,   677,
     658,   659,   660,   661,   662,   656,   657,   663,   664,   665,
     666,     0,   667,   668,   669,   670,     0,     0,     0,     0,
     671,     0,   672,   673,     0,     0,     0,     0,   674,   675,
     676,     0,     0,     0,   677,     0,   678,     0,   679,   680,
     681,   682,   683,   684,   685,   686,   687,   688,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   689,   690,     0,
       0,  1299,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   678,     0,   679,   680,   681,   682,   683,   684,   685,
     686,   687,   688,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   689,   690,     0,     0,  1300,     0,   658,   659,
     660,   661,   662,   656,   657,   663,   664,   665,   666,     0,
     667,   668,   669,   670,     0,     0,     0,     0,   671,     0,
     672,   673,     0,     0,     0,     0,   674,   675,   676,     0,
       0,     0,   677,     0,     0,     0,     0,     0,   656,   657,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   678,
       0,   679,   680,   681,   682,   683,   684,   685,   686,   687,
     688,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     689,   690,     0,     0,  1303,     0,   658,   659,   660,   661,
     662,     0,     0,   663,   664,   665,   666,     0,   667,   668,
     669,   670,     0,     0,     0,     0,   671,     0,   672,   673,
       0,     0,     0,     0,   674,   675,   676,     0,     0,     0,
     677,   658,   659,   660,   661,   662,   656,   657,   663,   664,
     665,   666,     0,   667,   668,   669,   670,     0,     0,     0,
       0,   671,     0,   672,   673,     0,     0,     0,     0,   674,
     675,   676,     0,     0,     0,   677,     0,   678,     0,   679,
     680,   681,   682,   683,   684,   685,   686,   687,   688,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   689,   690,
       0,     0,  1319,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   678,     0,   679,   680,   681,   682,   683,   684,
     685,   686,   687,   688,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   689,   690,     0,     0,  1321,     0,   658,
     659,   660,   661,   662,   656,   657,   663,   664,   665,   666,
       0,   667,   668,   669,   670,     0,     0,     0,     0,   671,
       0,   672,   673,     0,     0,     0,     0,   674,   675,   676,
       0,     0,     0,   677,     0,     0,     0,     0,     0,   656,
     657,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     678,     0,   679,   680,   681,   682,   683,   684,   685,   686,
     687,   688,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   689,   690,     0,     0,  1323,     0,   658,   659,   660,
     661,   662,     0,     0,   663,   664,   665,   666,     0,   667,
     668,   669,   670,     0,     0,     0,     0,   671,     0,   672,
     673,     0,     0,     0,     0,   674,   675,   676,     0,     0,
       0,   677,   658,   659,   660,   661,   662,   656,   657,   663,
     664,   665,   666,     0,   667,   668,   669,   670,     0,     0,
       0,     0,   671,     0,   672,   673,     0,     0,     0,     0,
     674,   675,   676,     0,     0,     0,   677,     0,   678,     0,
     679,   680,   681,   682,   683,   684,   685,   686,   687,   688,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   689,
     690,     0,     0,  1327,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   678,     0,   679,   680,   681,   682,   683,
     684,   685,   686,   687,   688,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   689,   690,     0,     0,  1382,     0,
     658,   659,   660,   661,   662,   656,   657,   663,   664,   665,
     666,     0,   667,   668,   669,   670,     0,     0,     0,     0,
     671,     0,   672,   673,     0,     0,     0,     0,   674,   675,
     676,     0,     0,     0,   677,     0,     0,     0,     0,     0,
     656,   657,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   678,     0,   679,   680,   681,   682,   683,   684,   685,
     686,   687,   688,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   689,   690,     0,     0,  1383,     0,   658,   659,
     660,   661,   662,     0,     0,   663,   664,   665,   666,     0,
     667,   668,   669,   670,     0,     0,     0,     0,   671,     0,
     672,   673,     0,     0,     0,     0,   674,   675,   676,     0,
       0,     0,   677,   658,   659,   660,   661,   662,   656,   657,
     663,   664,   665,   666,     0,   667,   668,   669,   670,     0,
       0,     0,     0,   671,     0,   672,   673,     0,     0,     0,
       0,   674,   675,   676,     0,     0,     0,   677,     0,   678,
       0,   679,   680,   681,   682,   683,   684,   685,   686,   687,
     688,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     689,   690,     0,     0,  1384,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   678,     0,   679,   680,   681,   682,
     683,   684,   685,   686,   687,   688,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   689,   690,     0,     0,  1399,
       0,   658,   659,   660,   661,   662,   656,   657,   663,   664,
     665,   666,     0,   667,   668,   669,   670,     0,     0,     0,
       0,   671,     0,   672,   673,     0,     0,     0,     0,   674,
     675,   676,     0,     0,     0,   677,     0,     0,     0,     0,
       0,   656,   657,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   678,     0,   679,   680,   681,   682,   683,   684,
     685,   686,   687,   688,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   689,   690,     0,     0,  1406,     0,   658,
     659,   660,   661,   662,     0,     0,   663,   664,   665,   666,
       0,   667,   668,   669,   670,     0,     0,     0,     0,   671,
       0,   672,   673,     0,     0,     0,     0,   674,   675,   676,
       0,     0,     0,   677,   658,   659,   660,   661,   662,   656,
     657,   663,   664,   665,   666,     0,   667,   668,   669,   670,
       0,     0,     0,     0,   671,     0,   672,   673,     0,     0,
       0,     0,   674,   675,   676,     0,     0,     0,   677,     0,
     678,     0,   679,   680,   681,   682,   683,   684,   685,   686,
     687,   688,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   689,   690,     0,     0,  1416,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   678,     0,   679,   680,   681,
     682,   683,   684,   685,   686,   687,   688,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   689,   690,     0,     0,
    1445,     0,   658,   659,   660,   661,   662,   656,   657,   663,
     664,   665,   666,     0,   667,   668,   669,   670,     0,     0,
       0,     0,   671,     0,   672,   673,     0,     0,     0,     0,
     674,   675,   676,     0,     0,     0,   677,     0,     0,     0,
       0,     0,   656,   657,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   678,     0,   679,   680,   681,   682,   683,
     684,   685,   686,   687,   688,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   689,   690,   713,     0,     0,     0,
     658,   659,   660,   661,   662,     0,     0,   663,   664,   665,
     666,     0,   667,   668,   669,   670,     0,     0,     0,     0,
     671,     0,   672,   673,     0,     0,     0,     0,   674,   675,
     676,     0,     0,     0,   677,   658,   659,   660,   661,   662,
     656,   657,   663,   664,   665,   666,     0,   667,   668,   669,
     670,     0,     0,     0,     0,   671,     0,   672,   673,     0,
       0,     0,     0,   674,   675,   676,     0,     0,     0,   677,
       0,   678,     0,   679,   680,   681,   682,   683,   684,   685,
     686,   687,   688,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   689,   690,   931,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   678,     0,   679,   680,
     681,   682,   683,   684,   685,   686,   687,   688,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   689,   690,  1098,
       0,     0,     0,   658,   659,   660,   661,   662,   656,   657,
     663,   664,   665,   666,     0,   667,   668,   669,   670,     0,
       0,     0,     0,   671,     0,   672,   673,     0,     0,     0,
       0,   674,   675,   676,     0,     0,     0,   677,     0,     0,
       0,     0,     0,   656,   657,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   678,     0,   679,   680,   681,   682,
     683,   684,   685,   686,   687,   688,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   689,   690,  1113,     0,     0,
       0,   658,   659,   660,   661,   662,     0,     0,   663,   664,
     665,   666,     0,   667,   668,   669,   670,     0,     0,     0,
       0,   671,     0,   672,   673,     0,     0,     0,     0,   674,
     675,   676,     0,     0,     0,   677,   658,   659,   660,   661,
     662,     0,     0,   663,   664,   665,   666,     0,   667,   668,
     669,   670,   276,   277,     0,     0,   671,     0,   672,   673,
       0,     0,     0,     0,   674,   675,   676,     0,     0,   278,
     677,     0,   678,     0,   679,   680,   681,   682,   683,   684,
     685,   686,   687,   688,     0,     0,     0,   656,   657,     0,
       0,     0,     0,   689,   690,  1239,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   678,     0,   679,
     680,   681,   682,   683,   684,   685,   686,   687,   688,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   689,   690,
    1244,     0,     0,     0,     0,     0,     0,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,     0,     0,   297,   298,   299,
       0,     0,     0,     0,     0,     0,   300,   301,   302,   303,
     304,     0,     0,   305,   306,   307,   308,   309,   310,   311,
     658,   659,   660,   661,   662,   656,   657,   663,   664,   665,
     666,     0,   667,   668,   669,   670,     0,     0,     0,     0,
     671,     0,   672,   673,     0,     0,   842,     0,   674,   675,
     676,     0,     0,     0,   677,     0,   312,     0,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,     0,     0,
     323,   324,     0,     0,     0,     0,     0,   325,   326,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   678,     0,   679,   680,   681,   682,   683,   684,   685,
     686,   687,   688,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   689,   690,     0,     0,     0,     0,   658,   659,
     660,   661,   662,   656,   657,   663,   664,   665,   666,     0,
     667,   668,   669,   670,     0,     0,     0,     0,   671,     0,
     672,   673,     0,     0,     0,     0,   674,   675,   676,     0,
       0,     0,   677,     0,     0,     0,     0,     0,   656,   657,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   331,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   678,
       0,   679,   680,   681,   682,   683,   684,   685,   686,   687,
     688,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     689,   690,     0,     0,     0,     0,   658,   659,   660,   661,
     662,     0,     0,   663,   664,   665,   666,     0,   667,   668,
     669,   670,     0,     0,     0,     0,   671,     0,   672,   673,
       0,     0,     0,     0,   674,   675,   676,     0,     0,     0,
     677,   658,   659,   660,   661,   662,   656,   657,   663,   664,
     665,   666,     0,   667,   668,   669,   670,     0,     0,     0,
       0,   671,     0,   672,   673,     0,     0,     0,     0,   674,
     675,   676,     0,     0,     0,   677,     0,   678,  1103,   679,
     680,   681,   682,   683,   684,   685,   686,   687,   688,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   689,   690,
       0,     0,     0,     0,  1172,     0,     0,     0,     0,     0,
       0,     0,   678,     0,   679,   680,   681,   682,   683,   684,
     685,   686,   687,   688,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   689,   690,     0,     0,     0,     0,   658,
     659,   660,   661,   662,   656,   657,   663,   664,   665,   666,
       0,   667,   668,   669,   670,     0,     0,     0,     0,   671,
       0,   672,   673,     0,     0,     0,     0,   674,   675,   676,
       0,     0,     0,   677,     0,     0,     0,     0,     0,     0,
     656,   657,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1207,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     678,     0,   679,   680,   681,   682,   683,   684,   685,   686,
     687,   688,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   689,   690,     0,     0,     0,     0,   658,   659,   660,
     661,   662,     0,     0,   663,   664,   665,   666,     0,   667,
     668,   669,   670,     0,     0,     0,     0,   671,     0,   672,
     673,     0,     0,     0,     0,   674,   675,   676,     0,     0,
       0,   677,     0,   658,   659,   660,   661,   662,   656,   657,
     663,   664,   665,   666,     0,   667,   668,   669,   670,     0,
       0,     0,     0,   671,     0,   672,   673,     0,     0,     0,
    1408,   674,   675,   676,     0,     0,     0,   677,   678,     0,
     679,   680,   681,   682,   683,   684,   685,   686,   687,   688,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   689,
     690,     0,     0,     0,     0,     0,  1442,     0,     0,     0,
       0,     0,     0,     0,   678,     0,   679,   680,   681,   682,
     683,   684,   685,   686,   687,   688,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   689,   690,     0,     0,     0,
       0,   658,   659,   660,   661,   662,   656,   657,   663,   664,
     665,   666,     0,   667,   668,   669,   670,     0,     0,     0,
       0,   671,     0,   672,   673,     0,     0,     0,     0,   674,
     675,   676,     0,     0,     0,   677,     0,     0,     0,     0,
       0,   656,   657,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1452,     0,     0,     0,     0,     0,
       0,     0,   678,     0,   679,   680,   681,   682,   683,   684,
     685,   686,   687,   688,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   689,   690,     0,     0,     0,     0,   658,
     659,   660,   661,   662,     0,     0,   663,   664,   665,   666,
       0,   667,   668,   669,   670,     0,     0,     0,     0,   671,
       0,   672,   673,     0,     0,     0,     0,   674,   675,   676,
       0,     0,     0,   677,   658,   659,   660,   661,   662,   656,
     657,   663,   664,   665,   666,     0,   667,   668,   669,   670,
       0,     0,     0,     0,   671,     0,   672,   673,     0,     0,
       0,     0,   674,   675,   676,     0,     0,     0,  -725,     0,
     678,     0,   679,   680,   681,   682,   683,   684,   685,   686,
     687,   688,     0,   656,   657,     0,     0,     0,     0,     0,
       0,   689,   690,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   678,     0,   679,   680,   681,
     682,   683,   684,   685,   686,   687,   688,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   689,   690,     0,     0,
       0,     0,   658,   659,   660,   661,   662,     0,     0,   663,
     664,   665,   666,     0,   667,   668,   669,   670,     0,     0,
       0,     0,   671,     0,   672,   673,     0,     0,     0,     0,
     674,   675,   676,     0,     0,     0,     0,     0,     0,     0,
     656,   657,     0,     0,     0,     0,   658,   659,   660,   661,
     662,     0,     0,   663,   664,   665,   666,     0,   667,   668,
     669,   670,     0,     0,   656,   657,   671,     0,   672,   673,
       0,     0,     0,   678,   674,   679,   680,   681,   682,   683,
     684,   685,   686,   687,   688,     0,     0,     0,   656,   657,
       0,     0,     0,     0,   689,   690,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   679,
     680,   681,   682,   683,   684,   685,   686,   687,   688,     0,
       0,     0,     0,   658,   659,   660,   661,   662,   689,   690,
     663,   664,   665,   666,     0,   667,   668,   669,   670,     0,
       0,     0,     0,   671,     0,   672,   673,   658,   659,   660,
     661,   662,     0,     0,   663,   664,   665,   666,     0,   667,
     668,   669,   670,     0,     0,     0,     0,   671,     0,   672,
     673,   658,   659,   660,   661,   662,     0,     0,   663,     0,
       0,   666,     0,   667,   668,   669,   670,     0,     0,     0,
       0,   671,     0,   672,   673,     0,     0,     0,   681,   682,
     683,   684,   685,   686,   687,   688,     0,     0,     0,     0,
       0,     0,     0,     0,   863,   689,   690,     0,     0,     0,
       0,     0,     0,   682,   683,   684,   685,   686,   687,   688,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   689,
     690,     0,     0,     0,     0,   867,     0,   682,   683,   684,
     685,   686,   687,   688,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   689,   690,   231,   232,   233,     0,   235,
     236,   237,   238,   239,   430,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   253,   254,   255,
       0,     0,   258,   259,   260,   261,   231,   232,   233,     0,
     235,   236,   237,   238,   239,   430,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,     0,   253,   254,
     255,     0,     0,   258,   259,   260,   261,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1099,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   864,     0,     0,     0,     0,     0,     0,     0,     0,
     865,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   868,     0,   181,     0,     0,     0,   231,   232,
     233,   869,   235,   236,   237,   238,   239,   430,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,     0,
     253,   254,   255,     0,     0,   258,   259,   260,   261,   182,
       0,   183,     0,   184,   185,   186,   187,   188,     0,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
       0,   200,   201,   202,     0,     0,   203,   204,   205,   206,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   207,   208,     0,     0,     0,
       0,     0,     0,     0,  1100,     0,     0,     0,     0,     0,
       0,     0,     0,  1101,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   209
};

static const yytype_int16 yycheck[] =
{
      14,    15,   211,   596,   395,   377,   811,   616,   727,   175,
     395,   650,   843,   652,   493,   161,   495,   699,   497,   701,
     498,   703,   402,   856,   471,   540,   499,     7,    21,    14,
      15,   104,   157,   553,   554,   407,   551,   175,    32,  1250,
      19,     0,    19,    19,    32,    18,    19,     6,   124,   539,
      64,    65,    66,    56,    14,    15,   124,   126,   127,    62,
     973,     6,   149,    56,    50,   978,   134,    20,    21,   129,
      29,   460,    31,   152,    33,   123,   140,   164,   126,   127,
      39,    45,   164,   176,   140,   600,   141,   142,   143,    56,
      49,   105,   106,   107,   108,    62,    55,   174,   176,    61,
     164,   124,   152,  1367,    49,    91,   182,   183,   124,   185,
     189,   134,   188,   502,  1392,   183,   176,   140,   134,    78,
     188,   208,   124,     4,     5,   164,   165,   166,   210,   185,
     209,   209,   395,   210,   211,    33,   205,   206,   149,   189,
     104,   100,   101,    24,   591,   218,  1424,  1060,  1412,    30,
     640,   540,  1363,   164,   209,   540,   195,   205,   206,   649,
     183,   175,   115,   116,    62,    32,   551,   183,   183,   182,
     123,   174,   157,   126,   127,   128,   815,   162,   817,   164,
     133,   183,   167,   182,   186,   824,    67,    68,   827,    20,
      21,   164,    59,    60,   188,   175,   209,   157,   344,   149,
     188,   194,   162,   182,   164,   182,   182,   167,   175,   164,
     209,   377,   205,   379,   164,   600,   731,   176,   206,   100,
     101,   182,   174,   208,   212,   614,   206,   393,   124,   139,
     182,   746,   126,   127,   623,   133,   195,   626,   134,   124,
      46,   407,   140,   615,   410,   411,   412,   206,   209,   134,
     770,   265,   205,   206,   121,   393,   628,   124,   125,   140,
      66,   155,   176,   182,   174,   654,   164,   134,   124,    56,
     164,   174,   410,   411,   412,    62,   190,   540,   134,   124,
     182,   182,   185,   164,   115,   116,   801,   183,   551,   134,
     209,   140,   123,   174,   125,   126,   127,   128,   183,   176,
     149,   199,   133,   692,   751,   154,   207,   209,   189,   176,
     208,   205,   206,   190,   761,   164,   183,   184,   182,   182,
     182,   188,   488,   489,   191,   206,   492,   183,   494,   141,
     496,   143,   498,   542,   182,   182,   182,   600,   183,   206,
     189,   550,   731,   207,   207,   212,   731,   209,   140,   182,
     488,   489,   176,   176,   492,   182,   494,   149,   496,   207,
     186,   746,   209,   377,   190,   379,   190,   190,   189,    64,
      65,    66,   164,   189,   205,   206,   209,     4,     5,   393,
       7,   185,   209,   862,   398,  1218,   190,   208,    56,   769,
      20,    21,   208,   407,    62,    73,   410,   411,   412,    77,
     185,   185,   185,   417,   208,   190,   190,   190,    35,  1091,
     105,   106,   107,   108,   189,  1134,   801,   176,    96,    97,
      98,    99,   811,   208,   208,   208,   155,   942,   164,   176,
     176,   190,   155,   208,   164,   164,   808,   809,   189,   189,
     176,   164,   814,   190,   190,   923,   176,   819,   820,   615,
     822,   823,   925,   825,   826,   164,   828,   208,   208,    11,
     168,   169,   628,   983,   984,   985,  1297,   176,   731,   176,
      22,    23,   155,   186,   488,   489,   182,   190,   492,   186,
     494,   164,   496,   746,   498,   115,   116,   176,   210,   211,
     656,   657,   182,   123,   183,   125,   126,   127,   128,   155,
     190,  1220,   186,   133,   185,   671,   190,   188,   164,   772,
     773,   774,   775,   776,   777,   778,   779,   780,   781,   782,
     729,   784,   785,   786,   787,   788,   789,    20,    21,   176,
     739,  1213,   185,   182,    56,   155,   185,   140,   801,   188,
      62,  1346,  1347,   709,   164,   186,   755,   168,   169,   190,
     759,   717,  1357,  1358,   720,   944,   164,   942,    87,    88,
      56,   191,   192,   193,   194,   195,    62,   164,    56,   836,
     837,   709,    56,  1255,    62,   205,   206,   208,    62,   717,
     164,    56,   720,  1192,  1303,   164,  1268,    62,   176,   176,
     157,    32,   186,   205,   182,   182,   190,   611,   164,   176,
     186,   615,  1081,   186,   190,   186,   186,   190,   186,   190,
     190,   186,   190,   164,   628,   190,   174,   164,    59,    60,
     168,    56,   115,   116,   168,   169,   170,   171,   183,   168,
     123,   176,   125,   126,   127,   128,   141,   142,   143,   653,
     133,   807,   808,   809,   810,  1284,   174,   813,   814,   174,
     174,   797,   141,   819,   820,   104,   822,   823,   189,   825,
     826,    34,   828,   164,   165,   166,   168,   169,   170,   807,
     836,   837,   810,    34,  1046,   813,   208,   174,  1157,   942,
     121,  1070,   164,   189,   125,  1074,   189,  1166,   175,    32,
      10,    11,    12,   208,   189,   709,   208,  1416,   836,   837,
     193,   194,   195,   717,   189,   186,   720,  1096,  1080,   189,
     189,   189,   205,   206,   208,   189,    59,    60,   189,  1108,
     189,   189,   164,    21,   164,   891,   164,   164,   185,   174,
     207,    20,    21,   185,   207,   552,   553,   554,   174,    42,
     164,   208,  1351,   184,   206,   164,   134,   188,  1341,   190,
     191,   164,   208,   164,   571,   964,   189,   923,   189,   189,
     208,  1400,  1401,   189,  1403,   206,   189,  1158,   585,   208,
     164,   212,   189,  1158,   940,   189,   208,   189,   121,   208,
     189,   124,   125,   208,   189,   189,   208,   208,   208,   164,
     208,   134,   208,   807,   808,   809,   810,  1390,   176,   813,
     814,   208,   940,   211,   206,   819,   820,   207,   822,   823,
     209,   825,   826,   175,   828,  1204,   176,   175,   185,   176,
     175,   157,   836,   837,   113,   114,   115,   116,   117,   164,
     176,   120,   121,   122,   123,    36,   125,   126,   127,   128,
     183,   184,   176,   176,   133,   188,   135,   136,   191,    32,
       9,   176,   141,   176,   143,   176,  1228,   176,   176,    65,
     175,  1250,    42,   206,   190,   182,   208,   206,   182,   212,
    1259,   164,   209,   182,    42,   129,    59,    60,   208,    13,
    1046,   183,   157,    32,   185,     7,   174,   211,   164,   175,
     182,   207,   189,   164,   206,  1158,   209,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   189,   208,   923,
      59,    60,   208,    32,  1080,   209,   205,   206,   735,     1,
     190,   208,   190,   208,    32,   208,   940,   208,  1137,   208,
     208,   748,   190,   164,   208,   164,   164,   175,   121,    66,
      59,    60,   125,   176,   354,   209,  1335,   164,   209,   208,
    1159,    59,    60,   770,   364,   189,   209,   209,   209,   208,
     208,  1350,   208,  1129,   374,   208,   208,  1356,  1340,    42,
     209,   176,   121,   208,  1363,   164,   125,   164,   208,   164,
     164,    32,   164,   164,   208,   395,   803,   208,   164,   208,
     208,  1129,   208,   176,   404,   208,   208,   208,   190,   207,
     209,   184,   121,   176,   190,   188,   125,   190,   191,   209,
     209,   209,   209,   121,   209,   209,   209,   125,  1227,    52,
     175,   209,   176,   206,   434,   435,   639,    80,   207,   212,
       1,   209,  1046,    44,   209,   184,   134,    83,  1334,   188,
    1164,   190,   191,   453,   454,   455,   456,   457,   458,  1258,
      81,   223,   975,  1164,  1164,  1164,  1164,   206,     1,  1333,
    1313,   571,  1228,   212,  1230,   184,  1080,  1272,  1344,   188,
    1176,   190,   191,  1275,   532,  1345,   184,  1270,    55,   417,
     188,   589,   190,   191,  1366,   417,    -1,   206,    -1,    -1,
      -1,    -1,  1230,   212,    -1,    -1,    -1,    -1,   206,    -1,
      -1,   511,    -1,    -1,   212,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1129,    -1,    -1,   528,    -1,
      -1,    -1,    -1,    -1,  1333,    -1,   536,    -1,    -1,  1338,
     540,    -1,    -1,   543,   544,    -1,    -1,    -1,    -1,   549,
      -1,   551,    -1,    -1,    -1,   555,    -1,    -1,    -1,    -1,
     560,    -1,    -1,    -1,   971,    -1,    -1,    -1,  1324,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   983,   984,   985,    -1,
      -1,   988,    -1,   990,  1340,   992,    -1,   994,    -1,   996,
      -1,   998,    -1,  1000,    -1,  1002,  1324,  1004,    -1,  1006,
     600,  1008,    -1,    -1,  1011,    -1,  1013,    -1,  1015,    -1,
    1017,    -1,  1019,    -1,  1021,    -1,   616,   617,    -1,    -1,
     620,    -1,  1029,    -1,  1228,    -1,  1230,    -1,    -1,    -1,
      -1,   631,   632,   633,   634,   635,   636,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1444,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   658,   659,
      -1,    -1,   662,   663,   664,   665,    -1,   667,    -1,   669,
     670,   671,   672,   673,   674,   675,   676,   677,   678,   679,
     680,   681,   682,   683,   684,   685,   686,   687,   688,    -1,
     690,   773,   774,   775,   776,   777,   778,   779,   780,   781,
     782,    -1,   784,   785,   786,   787,   788,   789,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1324,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,  1136,
      -1,   731,    -1,    -1,    -1,    -1,  1340,   737,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    11,   746,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
     760,    -1,    -1,  1367,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   772,   773,   774,   775,   776,   777,   778,   779,
     780,   781,   782,   783,   784,   785,   786,   787,   788,   789,
      -1,    -1,   792,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   801,    -1,    -1,    -1,    -1,    -1,    -1,  1412,    -1,
      -1,    -1,   812,    -1,   113,   114,   115,   116,   117,    -1,
      -1,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,   839,
      -1,    -1,   842,   843,    -1,    -1,   846,   113,   114,   115,
     116,   117,    -1,    -1,   120,   121,   122,   123,    -1,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,
     136,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,
      -1,   147,    -1,    -1,    -1,    -1,    -1,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   904,    -1,   205,   206,   908,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     930,    -1,    -1,    -1,    -1,   935,    -1,    -1,    -1,   205,
     206,    -1,   942,    -1,    -1,    -1,    -1,   947,   948,   949,
     950,   951,   952,   953,   954,   955,   956,   957,   958,   959,
     960,   961,   962,   963,    -1,    -1,    -1,   967,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     1,    -1,    -1,     4,     5,
       6,    -1,     8,     9,    10,    -1,    12,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,    25,
      26,    27,    28,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    38,    -1,    -1,    41,    -1,    43,    44,    -1,
      -1,    47,    -1,    49,    50,    51,    -1,    53,    54,    -1,
      -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    67,    68,    69,    70,    71,    -1,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    -1,    91,    92,    93,    -1,    -1,
      96,    97,    98,    99,   100,   101,   102,   103,  1078,  1079,
      -1,    -1,  1082,  1083,  1084,    -1,  1086,    -1,    -1,   115,
     116,    -1,    -1,  1093,    -1,  1095,    -1,  1097,    -1,    -1,
      -1,    -1,    -1,  1103,   130,   131,   132,  1107,    -1,    -1,
      -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   148,   149,   150,   151,   152,    -1,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,    -1,    -1,    -1,    -1,    -1,    -1,   174,   175,
     176,    -1,    -1,    -1,    -1,    -1,    20,    21,  1158,    -1,
      -1,    -1,  1162,    -1,  1164,   191,   192,   193,    -1,   195,
      -1,    -1,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,
     206,    32,   208,    -1,   210,   211,    -1,    -1,    -1,    -1,
      -1,    -1,  1192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,    60,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1236,    -1,  1238,    59,
      60,    -1,    -1,  1243,    -1,    -1,    -1,    -1,    -1,   113,
     114,   115,   116,  1253,    -1,    -1,    -1,    -1,    -1,   123,
    1260,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
     121,   135,   136,    -1,   125,    -1,    -1,    -1,    -1,    -1,
    1280,    -1,    -1,    -1,    -1,  1285,  1286,  1287,    -1,   121,
      -1,    -1,  1292,   125,    -1,  1295,    -1,  1297,  1298,    -1,
      -1,   121,    -1,    -1,    -1,   125,  1306,  1307,  1308,    -1,
      -1,    -1,    -1,    -1,    -1,  1315,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,   193,
     194,   195,    -1,   184,    -1,    -1,  1336,   188,    -1,   190,
     191,   205,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1351,   184,    -1,    -1,   206,   188,    -1,   190,   191,
      -1,   212,    -1,    -1,   184,    -1,    32,    -1,   188,  1369,
    1370,   191,    -1,    -1,   206,    -1,    -1,  1377,    -1,    -1,
     212,    -1,    -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,
      -1,  1391,   212,    59,    60,    -1,    -1,    -1,  1398,    -1,
       1,    -1,    -1,     4,     5,     6,    -1,     8,     9,    10,
      -1,    12,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,  1421,    -1,    24,    25,    26,    27,    28,    -1,    30,
      -1,    -1,    -1,    -1,  1434,    -1,    37,    38,  1438,  1439,
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
      -1,     4,     5,    -1,    -1,   206,    -1,   208,    -1,   210,
     211,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    71,    -1,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    91,    92,
      93,    -1,    -1,    96,    97,    98,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    25,    26,
      27,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    50,    51,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    71,    -1,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    92,    93,    -1,    -1,    96,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    71,    -1,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    91,    92,    93,    -1,    -1,    96,    97,    98,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,
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
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
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
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    71,    -1,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    91,    92,
      93,    -1,    -1,    96,    97,    98,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,   149,   150,   151,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,
      -1,   174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,
     193,    -1,   195,    -1,    -1,   198,   199,    -1,    -1,     4,
       5,    -1,    -1,   206,    -1,   208,   209,   210,   211,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    71,    -1,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    91,    92,    93,    -1,
      -1,    96,    97,    98,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,   149,   150,   151,   152,    32,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,    32,   174,
      -1,    -1,    -1,    -1,    -1,    59,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   191,   192,   193,    -1,
     195,    -1,    -1,   198,   199,    59,    60,    -1,    -1,     4,
       5,   206,    -1,   208,   209,   210,   211,    12,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,   121,    -1,    44,
      -1,   125,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,   125,    67,    68,    69,    70,    71,    -1,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    91,    92,    93,    -1,
      -1,    96,    97,    98,    99,   100,   101,   102,    -1,    -1,
     184,    -1,    -1,    -1,   188,    -1,   190,   191,    -1,    -1,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     184,    -1,   206,    -1,   188,    -1,   190,   191,   212,    -1,
      -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    -1,   206,   148,   149,   150,   151,   152,   212,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,   174,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   191,   192,   193,    -1,
     195,    -1,    -1,   198,   199,    -1,    -1,     4,     5,    -1,
      -1,   206,    -1,   208,    -1,   210,   211,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,
      -1,    -1,    32,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    59,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    71,    -1,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    92,    93,    -1,    -1,    96,
      97,    98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,
      -1,   121,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,   149,   150,   151,   152,    -1,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    32,   174,    -1,    -1,
      -1,    -1,    -1,    -1,   184,    -1,    -1,    -1,   188,    -1,
     190,   191,    -1,    -1,   191,   192,   193,    -1,   195,    -1,
      -1,   198,   199,    59,    60,    -1,   206,     4,     5,   206,
     207,   208,   212,   210,   211,    12,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,
      -1,    -1,    32,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    59,
      60,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,   125,
      67,    68,    69,    70,    71,    -1,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    92,    93,    -1,    -1,    96,
      97,    98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,
      -1,   121,    -1,    -1,    -1,   125,    -1,    -1,   184,    -1,
      -1,    -1,   188,    -1,   190,   191,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
     206,   148,   149,   150,   151,   152,   212,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    32,   174,    -1,    -1,
      -1,    -1,    -1,    -1,   184,    -1,    -1,    -1,   188,    -1,
     190,   191,    -1,    -1,   191,   192,   193,    -1,   195,    -1,
      -1,   198,   199,    59,    60,    -1,   206,     4,     5,   206,
      -1,   208,   212,   210,   211,    12,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,   125,
      67,    68,    69,    70,    71,    -1,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    92,    93,    -1,    -1,    96,
      97,    98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,
      -1,    -1,   188,    -1,   190,   191,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
     206,   148,   149,   150,   151,   152,   212,   154,    -1,   156,
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
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
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
      -1,   210,   211,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    -1,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
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
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    71,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,   191,   192,
     193,    -1,   195,    -1,    -1,   198,   199,    -1,    -1,     4,
       5,    -1,    -1,   206,    -1,   208,    -1,   210,   211,    14,
      15,    16,    17,    18,    -1,    -1,    21,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,   206,    -1,   208,    -1,   210,   211,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
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
     189,    -1,   191,   192,   193,    -1,   195,    -1,    -1,   198,
     199,    -1,    -1,     4,     5,    -1,    -1,   206,    -1,   208,
      -1,   210,   211,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
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
      -1,     4,     5,    -1,    -1,   206,    -1,   208,   209,   210,
     211,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    71,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    71,    -1,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    92,    93,    -1,    -1,    96,
      97,    98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    71,    -1,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    91,    92,    93,    -1,    -1,    96,    97,    98,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,
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
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
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
      -1,     4,     5,    -1,    -1,   206,    -1,   208,   209,   210,
     211,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    71,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,   206,    -1,   208,    -1,   210,   211,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,
      -1,    -1,    32,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    59,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    71,    -1,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    92,    93,    -1,    -1,    96,
      97,    98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,
       9,   121,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,   149,   150,   151,   152,    -1,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    20,    21,    -1,    -1,    -1,    -1,   174,    -1,    -1,
      -1,    -1,    -1,    -1,   184,    -1,    -1,    -1,   188,    -1,
     190,   191,    -1,    -1,   191,   192,   193,    -1,   195,    -1,
      -1,   198,   199,    -1,    -1,    -1,   206,    -1,    -1,   206,
      -1,   208,   212,   210,   211,    -1,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,   134,   135,   136,   137,   138,
      -1,    -1,   141,   142,   143,   144,   145,   146,   147,    -1,
      -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,    -1,
      -1,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,
      -1,    -1,    -1,    -1,   183,   184,    -1,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,   205,   206,    -1,    30,
      -1,   210,   211,    -1,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,   187,   188,
     189,   190,   191,   192,   193,   194,   195,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,   205,   206,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,   140,
      -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   153,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,   164,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,
      -1,   212,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
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
      -1,    -1,    -1,    -1,   205,   206,   207,    -1,    -1,    -1,
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
     117,    -1,    -1,   120,   121,   122,   123,    -1,   125,   126,
     127,   128,    20,    21,    -1,    -1,   133,    -1,   135,   136,
      -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,    37,
     147,    -1,   184,    -1,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,   205,   206,   207,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,   206,
     207,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,    -1,    -1,   125,   126,   127,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,
     138,    -1,    -1,   141,   142,   143,   144,   145,   146,   147,
     113,   114,   115,   116,   117,    20,    21,   120,   121,   122,
     123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
     133,    -1,   135,   136,    -1,    -1,   139,    -1,   141,   142,
     143,    -1,    -1,    -1,   147,    -1,   184,    -1,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,    -1,    -1,
     198,   199,    -1,    -1,    -1,    -1,    -1,   205,   206,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   184,    -1,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   205,   206,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,    20,    21,   120,   121,   122,   123,    -1,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,
     135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,
      -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,
      -1,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,    -1,    -1,    -1,    -1,   113,   114,   115,   116,
     117,    -1,    -1,   120,   121,   122,   123,    -1,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,   136,
      -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,    -1,
     147,   113,   114,   115,   116,   117,    20,    21,   120,   121,
     122,   123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,
     142,   143,    -1,    -1,    -1,   147,    -1,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,   206,
      -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   184,    -1,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,   206,    -1,    -1,    -1,    -1,   113,
     114,   115,   116,   117,    20,    21,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,
      -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     184,    -1,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   205,   206,    -1,    -1,    -1,    -1,   113,   114,   115,
     116,   117,    -1,    -1,   120,   121,   122,   123,    -1,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,
     136,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,    -1,
      -1,   147,    -1,   113,   114,   115,   116,   117,    20,    21,
     120,   121,   122,   123,    -1,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,
     176,   141,   142,   143,    -1,    -1,    -1,   147,   184,    -1,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,
     206,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   184,    -1,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   205,   206,    -1,    -1,    -1,
      -1,   113,   114,   115,   116,   117,    20,    21,   120,   121,
     122,   123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,   141,
     142,   143,    -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   184,    -1,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,   206,    -1,    -1,    -1,    -1,   113,
     114,   115,   116,   117,    -1,    -1,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,    -1,    -1,    -1,    -1,   133,
      -1,   135,   136,    -1,    -1,    -1,    -1,   141,   142,   143,
      -1,    -1,    -1,   147,   113,   114,   115,   116,   117,    20,
      21,   120,   121,   122,   123,    -1,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,
      -1,    -1,   141,   142,   143,    -1,    -1,    -1,   147,    -1,
     184,    -1,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,   205,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   184,    -1,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   205,   206,    -1,    -1,
      -1,    -1,   113,   114,   115,   116,   117,    -1,    -1,   120,
     121,   122,   123,    -1,   125,   126,   127,   128,    -1,    -1,
      -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,    -1,
     141,   142,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,   113,   114,   115,   116,
     117,    -1,    -1,   120,   121,   122,   123,    -1,   125,   126,
     127,   128,    -1,    -1,    20,    21,   133,    -1,   135,   136,
      -1,    -1,    -1,   184,   141,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,   205,   206,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,    -1,
      -1,    -1,    -1,   113,   114,   115,   116,   117,   205,   206,
     120,   121,   122,   123,    -1,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,   133,    -1,   135,   136,   113,   114,   115,
     116,   117,    -1,    -1,   120,   121,   122,   123,    -1,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,   133,    -1,   135,
     136,   113,   114,   115,   116,   117,    -1,    -1,   120,    -1,
      -1,   123,    -1,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   133,    -1,   135,   136,    -1,    -1,    -1,   188,   189,
     190,   191,   192,   193,   194,   195,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    18,   205,   206,    -1,    -1,    -1,
      -1,    -1,    -1,   189,   190,   191,   192,   193,   194,   195,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,
     206,    -1,    -1,    -1,    -1,    18,    -1,   189,   190,   191,
     192,   193,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,   206,    69,    70,    71,    -1,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    91,    92,    93,
      -1,    -1,    96,    97,    98,    99,    69,    70,    71,    -1,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    91,    92,
      93,    -1,    -1,    96,    97,    98,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    34,    -1,    -1,    -1,    69,    70,
      71,   164,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      91,    92,    93,    -1,    -1,    96,    97,    98,    99,    69,
      -1,    71,    -1,    73,    74,    75,    76,    77,    -1,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    91,    92,    93,    -1,    -1,    96,    97,    98,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   164
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   214,     0,     6,    29,    31,    33,    39,    49,    55,
      78,   100,   101,   176,   195,   206,   215,   218,   224,   226,
     227,   232,   257,   261,   294,   371,   378,   381,   392,   436,
     440,   444,    18,    19,   164,   250,   251,   252,   157,   233,
     234,   164,   165,   166,   195,   228,   229,   230,    56,    62,
     174,   374,   375,   164,   210,   217,   374,   374,   374,   140,
     164,   280,    33,    62,   133,   199,   208,   253,   254,   255,
     256,   280,   176,     4,     5,     7,    35,   389,    61,   369,
     183,   182,   185,   182,   229,    21,    56,   194,   205,   231,
     375,   377,   164,   176,   369,   164,   164,   164,   164,   140,
     225,   255,   255,   255,   208,   141,   142,   143,   182,   207,
      56,    62,   262,   264,    56,    62,   379,    56,    62,   390,
      56,    62,   370,    14,    15,   157,   162,   164,   167,   208,
     220,   251,   157,   234,   164,   228,   228,   176,   175,   375,
     376,    56,    62,   216,   445,   437,   441,   164,   168,   223,
     209,   252,   255,   255,   255,   255,   265,   164,   380,   393,
     174,   372,   168,   169,   219,    14,    15,   157,   162,   164,
     220,   248,   249,   231,   176,   183,   174,   174,   174,   168,
     209,    34,    69,    71,    73,    74,    75,    76,    77,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      91,    92,    93,    96,    97,    98,    99,   115,   116,   164,
     260,   263,   174,   185,   104,   387,   388,   367,   189,   343,
     168,   169,   170,   182,   209,    18,    24,    30,    40,    48,
      63,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   153,   210,   280,   396,   398,   399,
     402,   407,   435,   446,   438,   442,    20,    21,    37,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   125,   126,   127,
     134,   135,   136,   137,   138,   141,   142,   143,   144,   145,
     146,   147,   184,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   198,   199,   205,   206,    34,    34,   208,
     258,   174,   266,   382,    73,    77,    96,    97,    98,    99,
     397,   164,   394,   175,   368,   252,   149,   164,   365,   366,
     248,   189,   189,   189,   208,   189,   189,   208,   189,   189,
     189,   189,   189,   189,   208,   280,    32,    59,    60,   121,
     125,   184,   188,   191,   206,   212,   406,   186,   401,   357,
     360,   164,   164,   164,   207,    21,   164,   207,   152,   209,
     343,   353,   354,   185,   259,   269,   373,   174,   185,   386,
     174,   391,   343,   190,   208,    42,   182,   185,   188,   364,
     408,   413,   415,     4,     5,    14,    15,    16,    17,    18,
      24,    26,    30,    38,    44,    47,    50,    54,    67,    68,
      78,   100,   101,   102,   115,   116,   148,   149,   150,   151,
     152,   154,   156,   157,   158,   159,   160,   161,   162,   163,
     165,   166,   167,   174,   191,   192,   193,   198,   199,   206,
     208,   210,   211,   222,   224,   274,   280,   285,   299,   306,
     309,   312,   317,   319,   323,   324,   326,   331,   334,   335,
     342,   396,   448,   451,   461,   464,   470,   473,   417,   411,
     164,   403,   419,   421,   423,   425,   427,   429,   431,   433,
     335,   189,   208,    32,   188,    32,   188,   206,   212,   207,
     335,   206,   212,   407,   164,   176,   447,   164,   176,   355,
     435,   439,   164,   176,   358,   443,   164,   134,   208,     6,
      49,   293,   176,   209,   435,     1,     8,     9,    10,    12,
      25,    27,    28,    37,    41,    43,    51,    53,    57,    58,
      64,   103,   130,   131,   132,   165,   175,   176,   235,   236,
     239,   241,   242,   243,   244,   245,   246,   247,   267,   268,
     270,   274,   279,   281,   286,   287,   288,   289,   290,   291,
     292,   294,   298,   320,   322,   335,   164,   176,   383,   384,
     280,   349,   366,   335,   164,   164,   407,   124,   134,   183,
     363,   435,   435,   405,   435,   189,   189,   189,   282,   398,
     448,   208,   189,   189,   208,   189,   208,   208,   189,   189,
     208,   189,   189,   208,   189,   189,   208,   189,   189,   335,
     335,   208,   208,   208,   208,   208,   208,   189,   189,   221,
      12,   335,   460,   469,   335,   335,   335,   335,   335,    12,
     310,   335,   310,   211,   208,   257,    20,    21,   113,   114,
     115,   116,   117,   120,   121,   122,   123,   125,   126,   127,
     128,   133,   135,   136,   141,   142,   143,   147,   184,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   205,
     206,   209,   208,   435,   435,   209,   164,   400,   435,   258,
     435,   258,   435,   258,   355,   356,   358,   359,   209,   410,
     271,   310,   207,   207,   335,   176,   175,   185,   176,   175,
     185,   176,   175,   335,   149,   164,   362,   395,   353,   335,
     395,   124,   279,   310,   320,   335,   266,    60,   335,   335,
     164,   176,   157,    57,   335,   266,   124,   279,   335,   211,
     266,   312,   316,   316,   316,   335,    36,   176,   176,   335,
       9,   312,   318,   176,   176,   176,   176,   176,    65,   295,
     129,   176,   105,   106,   107,   108,   109,   110,   111,   112,
     118,   119,   124,   134,   137,   138,   144,   145,   146,   183,
     318,   176,   183,   175,   373,   175,   257,   350,   209,    42,
     176,   363,   279,   335,   190,   190,   190,   176,   452,   454,
     283,   208,   280,   307,   465,   310,   407,   469,   335,   300,
     302,   335,   304,   467,   310,   458,   462,   310,   456,   407,
     335,   335,   335,   335,   335,   335,   275,   277,   170,   171,
     219,   395,   139,   182,   472,   395,   182,   472,   472,   149,
     154,   189,   280,   325,   155,   164,   206,   209,   310,   449,
     450,   206,   315,    18,   155,   164,   396,    18,   155,   164,
     396,   335,   335,   335,   335,   335,   335,   164,   335,   155,
     164,   335,   335,   335,   396,   335,   335,   335,   335,   335,
     335,    21,   335,   335,   335,   335,   335,   335,   335,   335,
     335,   335,   335,   126,   127,   155,   164,   205,   206,   332,
     335,   209,   310,   190,   190,   176,   190,   190,   259,   190,
     259,   190,   259,   176,   190,   176,   190,   273,   435,   209,
     182,   207,   435,   435,   209,   208,    42,   124,   182,   183,
     185,   188,   361,   266,    19,   279,   310,   105,   106,   107,
     108,   109,   110,   111,   112,   118,   119,   124,   137,   138,
     144,   145,   146,   183,    13,   335,   266,   183,   185,   157,
     279,   335,   211,   257,   266,   174,   266,   335,   257,   208,
     297,   366,   316,   130,   131,   132,   267,   321,   335,   321,
     335,   321,   335,   321,   335,   321,   335,   321,   335,   321,
     335,   321,   335,   321,   335,   321,   335,   321,   335,   335,
     321,   335,   321,   335,   321,   335,   321,   335,   321,   335,
     321,   335,   335,   385,     7,   343,   348,   164,   279,   335,
     176,   409,   414,   416,   435,   407,   407,   435,   209,   310,
     450,   189,   335,   435,   407,   472,   176,   190,   472,   209,
     407,   407,   209,   407,   407,   472,   407,   407,   472,   407,
     190,   209,   209,   209,   209,   209,   209,   273,   273,   335,
      19,   335,   460,   175,    19,   335,   207,   209,   208,   208,
     327,   329,   208,   134,   361,   449,   182,   209,   182,   209,
     206,   258,   189,   208,   189,   208,   208,   208,   207,    18,
     155,   164,   396,   185,   155,   164,   335,   208,   208,   155,
     164,   335,     1,   207,   209,   418,   412,   164,   404,   420,
     190,   424,   190,   428,   190,   355,   432,   358,   434,   176,
     190,   335,   164,   164,   435,   279,   335,   310,   335,   335,
     335,   335,   335,   335,   335,   335,   335,   335,   335,   335,
     335,   335,   335,   335,   335,   266,   335,   315,   269,    11,
      22,    23,   237,   238,    11,   240,   315,   164,   296,   316,
     316,   316,   176,   175,    56,    62,   346,    66,   347,   176,
     176,   190,   190,   190,   209,   209,   164,   209,   190,   190,
     209,   407,   208,   209,   190,   190,   190,   190,   209,   190,
     190,   209,   190,   315,   208,   190,   190,   172,   310,   310,
     335,   335,   407,   258,   335,   335,   335,   207,   206,   155,
     164,   124,   134,   183,   188,   313,   314,   259,   336,   335,
     338,   335,   209,   310,   335,   189,   208,   335,   208,   207,
     335,   209,   310,   208,   207,   333,   422,   426,   430,   435,
     208,   209,    42,   361,   266,   258,   175,   266,   335,    25,
     103,   241,   286,   287,   288,   290,   335,   176,   258,   182,
     209,    46,   347,    45,   104,   344,   453,   455,   284,   176,
     190,   308,   466,   190,   469,   301,   303,   305,   468,   459,
     463,   457,   208,   209,   310,   276,   278,   176,   176,   209,
     209,   190,   259,   209,   209,   449,   208,   134,   361,   164,
     164,   164,   164,   182,   207,   139,   266,   311,   407,   209,
     435,   209,   209,   209,   340,   335,   335,   209,   209,   335,
     271,   164,   335,   259,   266,   124,   124,   335,   259,   164,
     185,   364,    32,   345,   344,   346,   208,   208,   164,   335,
     208,   208,   472,   335,   335,   335,   208,   208,   208,   209,
     335,   209,   335,   208,   272,   460,   335,   328,   190,   134,
     361,   207,   335,   335,   335,   313,   207,   124,   335,   190,
     190,   435,   209,   209,   209,   209,   311,   238,   335,   266,
     407,   363,   351,   345,   362,   164,   450,   450,   190,   209,
     310,   469,   209,   310,   450,   450,   209,   271,   176,   471,
     471,   325,   330,   335,   335,   209,   209,   335,   337,   339,
     190,   363,   335,   263,   352,   209,   209,   335,   472,   472,
     472,   209,   209,   209,    52,   175,   207,   325,   134,   361,
     341,   335,   176,   176,   263,   209,   209,   209,   209,   335,
     335,   335,   176,   266
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
     238,   238,   238,   239,   239,   240,   240,   241,   241,   241,
     241,   241,   242,   242,   243,   244,   245,   246,   247,   248,
     248,   248,   248,   248,   248,   249,   249,   250,   250,   250,
     251,   251,   251,   251,   251,   251,   251,   251,   252,   252,
     253,   253,   253,   254,   254,   255,   255,   255,   255,   255,
     255,   255,   256,   256,   257,   257,   258,   258,   258,   259,
     259,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   261,   262,   262,   262,
     263,   265,   264,   266,   266,   267,   267,   268,   268,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   269,   269,
     269,   270,   271,   271,   272,   272,   273,   273,   275,   276,
     274,   277,   278,   274,   279,   279,   279,   279,   279,   280,
     280,   280,   281,   281,   283,   284,   282,   282,   285,   285,
     285,   285,   285,   286,   287,   288,   288,   288,   289,   289,
     289,   290,   290,   291,   291,   291,   292,   293,   293,   293,
     294,   294,   295,   295,   296,   296,   297,   297,   298,   298,
     300,   301,   299,   302,   303,   299,   304,   305,   299,   307,
     308,   306,   309,   309,   309,   310,   310,   311,   311,   311,
     312,   312,   312,   313,   313,   313,   313,   314,   314,   315,
     315,   316,   316,   317,   318,   319,   319,   319,   319,   319,
     319,   319,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   321,   321,   321,   321,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   323,   323,   324,   324,   325,   325,   326,   327,
     328,   326,   329,   330,   326,   331,   331,   331,   331,   332,
     333,   331,   334,   334,   334,   334,   334,   335,   335,   335,
     335,   335,   335,   335,   335,   335,   335,   335,   335,   335,
     335,   335,   335,   335,   335,   335,   335,   335,   335,   335,
     335,   335,   335,   335,   335,   335,   335,   335,   335,   335,
     335,   335,   335,   335,   335,   335,   335,   335,   335,   335,
     335,   335,   335,   335,   335,   335,   335,   335,   335,   335,
     335,   335,   335,   336,   337,   335,   335,   335,   335,   338,
     339,   335,   335,   335,   340,   341,   335,   335,   335,   335,
     335,   335,   335,   335,   335,   335,   335,   335,   335,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   343,   343,   344,   344,   344,
     345,   345,   346,   346,   346,   347,   347,   348,   349,   350,
     349,   351,   349,   352,   349,   353,   353,   354,   354,   355,
     355,   356,   356,   357,   357,   357,   358,   359,   359,   360,
     360,   360,   361,   361,   362,   362,   362,   362,   362,   362,
     363,   363,   363,   364,   364,   365,   365,   365,   365,   365,
     366,   366,   366,   366,   366,   367,   368,   367,   369,   369,
     370,   370,   370,   371,   372,   371,   373,   373,   373,   373,
     374,   374,   374,   376,   375,   377,   377,   378,   378,   379,
     379,   379,   380,   382,   383,   381,   384,   385,   381,   386,
     386,   387,   387,   388,   389,   389,   390,   390,   390,   391,
     391,   393,   394,   392,   395,   395,   395,   395,   395,   396,
     396,   396,   396,   396,   396,   396,   396,   396,   396,   396,
     396,   396,   396,   396,   396,   396,   396,   396,   396,   396,
     396,   396,   396,   396,   396,   396,   397,   397,   397,   397,
     397,   397,   398,   399,   399,   399,   400,   400,   401,   401,
     401,   403,   404,   402,   405,   405,   406,   406,   407,   407,
     407,   407,   407,   407,   408,   409,   407,   407,   407,   410,
     407,   407,   407,   407,   407,   407,   407,   407,   407,   407,
     407,   407,   407,   411,   412,   407,   407,   413,   414,   407,
     415,   416,   407,   417,   418,   407,   407,   419,   420,   407,
     421,   422,   407,   407,   423,   424,   407,   425,   426,   407,
     407,   427,   428,   407,   429,   430,   407,   431,   432,   407,
     433,   434,   407,   435,   435,   435,   437,   438,   439,   436,
     441,   442,   443,   440,   445,   446,   447,   444,   448,   448,
     448,   448,   448,   449,   449,   449,   449,   449,   449,   449,
     449,   450,   452,   453,   451,   454,   455,   451,   456,   457,
     451,   458,   459,   451,   460,   460,   461,   462,   463,   461,
     464,   464,   465,   466,   464,   464,   467,   468,   464,   469,
     469,   470,   470,   470,   470,   471,   471,   472,   472,   473,
     473
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
       1,     1,     1,     1,     4,     1,     2,     3,     3,     3,
       3,     2,     1,     3,     0,     3,     0,     2,     3,     0,
       2,     1,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     3,     3,     2,     2,     3,
       4,     3,     2,     2,     2,     2,     2,     3,     3,     3,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     0,     1,     1,
       3,     0,     4,     3,     7,     2,     2,     1,     1,     1,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     1,     2,     2,     2,     0,     2,
       2,     3,     0,     1,     0,     3,     1,     3,     0,     0,
       7,     0,     0,     7,     3,     2,     2,     2,     1,     1,
       3,     2,     2,     3,     0,     0,     5,     1,     2,     4,
       5,     5,     2,     1,     1,     1,     2,     3,     2,     2,
       3,     2,     3,     2,     2,     3,     4,     1,     1,     0,
       1,     1,     1,     0,     1,     3,     8,     7,     3,     3,
       0,     0,     7,     0,     0,     7,     0,     0,     7,     0,
       0,     6,     5,     8,    10,     1,     3,     1,     2,     3,
       1,     1,     2,     2,     2,     2,     2,     1,     3,     0,
       5,     1,     6,     6,     6,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     6,     8,     5,     6,     1,     4,     3,     0,
       0,     8,     0,     0,     9,     3,     4,     5,     6,     0,
       0,     5,     3,     4,     4,     3,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     4,     4,     5,
       4,     5,     3,     4,     1,     1,     2,     4,     4,     7,
       8,     3,     5,     0,     0,     8,     3,     3,     3,     0,
       0,     8,     3,     4,     0,     0,     9,     4,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     1,     4,
       4,     4,     4,     4,     1,     6,     7,     6,     6,     7,
       7,     6,     7,     6,     6,     0,     3,     0,     1,     1,
       0,     1,     0,     1,     1,     0,     1,     5,     0,     0,
       4,     0,     9,     0,    10,     3,     4,     1,     3,     1,
       3,     1,     3,     0,     2,     3,     3,     1,     3,     0,
       2,     3,     1,     1,     1,     2,     3,     5,     3,     3,
       1,     1,     1,     0,     1,     1,     4,     3,     3,     5,
       4,     6,     5,     5,     4,     0,     0,     4,     0,     1,
       0,     1,     1,     6,     0,     6,     0,     2,     3,     5,
       0,     1,     1,     0,     5,     2,     3,     4,     3,     0,
       1,     1,     1,     0,     0,     9,     0,     0,    11,     0,
       2,     0,     1,     3,     1,     1,     0,     1,     1,     0,
       3,     0,     0,     7,     1,     4,     3,     3,     5,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     4,     1,     3,     0,     2,
       3,     0,     0,     6,     1,     3,     3,     4,     1,     1,
       1,     1,     2,     3,     0,     0,     6,     4,     5,     0,
       9,     4,     2,     2,     3,     2,     3,     2,     2,     3,
       3,     3,     2,     0,     0,     6,     2,     0,     0,     6,
       0,     0,     6,     0,     0,     6,     1,     0,     0,     6,
       0,     0,     7,     1,     0,     0,     6,     0,     0,     7,
       1,     0,     0,     6,     0,     0,     7,     0,     0,     6,
       0,     0,     6,     1,     3,     3,     0,     0,     0,     9,
       0,     0,     0,     9,     0,     0,     0,     9,     1,     1,
       1,     1,     1,     3,     3,     5,     5,     6,     6,     8,
       8,     1,     0,     0,     9,     0,     0,     9,     0,     0,
       9,     0,     0,     6,     3,     1,     5,     0,     0,     9,
       4,     5,     0,     0,    10,     5,     0,     0,    10,     1,
       3,     4,     5,     8,    10,     0,     3,     0,     1,     9,
       9
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

  case 100: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 101: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 102: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 103: /* annotation_declaration_basic: annotation_declaration_name  */
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

  case 104: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 105: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 106: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das2_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 107: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 108: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 109: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 110: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 111: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 112: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 113: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 114: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 115: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 116: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 117: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 118: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 119: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 120: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 121: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 122: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 123: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 124: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 125: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 126: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 127: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 128: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 129: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 130: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 131: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 132: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 133: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 134: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 135: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 136: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 137: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 138: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 139: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 140: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 141: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 142: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 143: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 144: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 145: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 146: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 147: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 148: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 149: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 150: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 151: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 152: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 153: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 154: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 155: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 156: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 157: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 158: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 159: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 160: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 161: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 162: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 163: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 164: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 165: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 166: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 167: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 168: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 169: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 170: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 171: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 172: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 173: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 174: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 175: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 176: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 177: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 178: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 179: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 180: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 181: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 182: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 183: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 184: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 185: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 186: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 187: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 188: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 189: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 190: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 191: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 192: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 193: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 194: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 195: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 196: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 197: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 198: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 199: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 200: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 201: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 202: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 203: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 204: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 205: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 206: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 207: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 208: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 209: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 210: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 211: /* $@2: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 212: /* function_declaration: optional_public_or_private_function $@2 function_declaration_header expression_block  */
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

  case 213: /* expression_block: "begin of code block" expressions "end of code block"  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 214: /* expression_block: "begin of code block" expressions "end of code block" "finally" "begin of code block" expressions "end of code block"  */
                                                                                          {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 215: /* expr_call_pipe: expr expr_full_block_assumed_piped  */
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

  case 216: /* expr_call_pipe: expression_keyword expr_full_block_assumed_piped  */
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

  case 217: /* expression_any: "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 218: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 219: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 220: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 221: /* expression_any: expr_assign "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 222: /* expression_any: expression_delete "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 223: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 224: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 225: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 226: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 227: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 228: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 229: /* expression_any: expression_break "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 230: /* expression_any: expression_continue "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 231: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 232: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 233: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 234: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 235: /* expression_any: expression_label "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 236: /* expression_any: expression_goto "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 237: /* expression_any: "pass" "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 238: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 239: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 240: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 241: /* expr_keyword: "keyword" expr expression_block  */
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

  case 242: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 243: /* optional_expr_list: expr_list  */
                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 244: /* optional_expr_list_in_braces: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 245: /* optional_expr_list_in_braces: '(' optional_expr_list ')'  */
                                              { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 246: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 247: /* type_declaration_no_options_list: type_declaration_no_options_list "end of expression" type_declaration  */
                                                                           {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 248: /* $@3: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 249: /* $@4: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 250: /* expression_keyword: "keyword" '<' $@3 type_declaration_no_options_list '>' $@4 expr  */
                                                                                                                                                     {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 251: /* $@5: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 252: /* $@6: %empty  */
                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 253: /* expression_keyword: "type function" '<' $@5 type_declaration_no_options_list '>' $@6 optional_expr_list_in_braces  */
                                                                                                                                                                                   {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 254: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 255: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 256: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 257: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 258: /* expr_pipe: expr_call_pipe  */
                             {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 259: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 260: /* name_in_namespace: "name" "::" "name"  */
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

  case 261: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 262: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 263: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 264: /* $@7: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 265: /* $@8: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 266: /* new_type_declaration: '<' $@7 type_declaration '>' $@8  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 267: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 268: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 269: /* expr_new: "new" new_type_declaration '(' ')'  */
                                                               {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
    break;

  case 270: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 271: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 272: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 273: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 274: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 275: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 276: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 277: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 278: /* expression_return: expression_return_no_pipe "end of expression"  */
                                              {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 279: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 280: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 281: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 282: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 283: /* expression_yield: expression_yield_no_pipe "end of expression"  */
                                             {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 284: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 285: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 286: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 287: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 288: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 289: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 290: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 291: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 292: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 293: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 294: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 295: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 296: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 297: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr "end of expression"  */
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

  case 298: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 299: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 300: /* $@9: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 301: /* $@10: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 302: /* expr_cast: "cast" '<' $@9 type_declaration_no_options '>' $@10 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 303: /* $@11: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 304: /* $@12: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 305: /* expr_cast: "upcast" '<' $@11 type_declaration_no_options '>' $@12 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 306: /* $@13: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 307: /* $@14: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 308: /* expr_cast: "reinterpret" '<' $@13 type_declaration_no_options '>' $@14 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 309: /* $@15: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 310: /* $@16: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 311: /* expr_type_decl: "type" '<' $@15 type_declaration '>' $@16  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 312: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 313: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 314: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" "end of expression" "name" '>' expr ')'  */
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

  case 315: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 316: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 317: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 318: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 319: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 320: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 321: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 322: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 323: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 324: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 325: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 326: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 327: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 328: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 329: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 330: /* optional_capture_list: '[' '[' capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 331: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 332: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 333: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 334: /* expr_full_block_assumed_piped: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 335: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 336: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 337: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 338: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 339: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 340: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 341: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 342: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 343: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 344: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 345: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 346: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 347: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 348: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 349: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 350: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 351: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 352: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 353: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 354: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 355: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 356: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 357: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 358: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 359: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 360: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 361: /* expr_assign_pipe_right: "@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 362: /* expr_assign_pipe_right: "@@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 363: /* expr_assign_pipe_right: "$ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 364: /* expr_assign_pipe_right: expr_call_pipe  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 365: /* expr_assign_pipe: expr '=' expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 366: /* expr_assign_pipe: expr "<-" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 367: /* expr_assign_pipe: expr "&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 368: /* expr_assign_pipe: expr "|=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 369: /* expr_assign_pipe: expr "^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 370: /* expr_assign_pipe: expr "&&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 371: /* expr_assign_pipe: expr "||=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 372: /* expr_assign_pipe: expr "^^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 373: /* expr_assign_pipe: expr "+=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 374: /* expr_assign_pipe: expr "-=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 375: /* expr_assign_pipe: expr "*=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 376: /* expr_assign_pipe: expr "/=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 377: /* expr_assign_pipe: expr "%=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 378: /* expr_assign_pipe: expr "<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 379: /* expr_assign_pipe: expr ">>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 380: /* expr_assign_pipe: expr "<<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 381: /* expr_assign_pipe: expr ">>>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 382: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 383: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 384: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 385: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 386: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 387: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 388: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 389: /* $@17: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 390: /* $@18: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 391: /* func_addr_expr: '@' '@' '<' $@17 type_declaration_no_options '>' $@18 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 392: /* $@19: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 393: /* $@20: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 394: /* func_addr_expr: '@' '@' '<' $@19 optional_function_argument_list optional_function_type '>' $@20 func_addr_name  */
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

  case 395: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 396: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 397: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 398: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 399: /* $@21: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 400: /* $@22: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 401: /* expr_field: expr '.' $@21 error $@22  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 402: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 403: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
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

  case 404: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 405: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 406: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 407: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 408: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 409: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 410: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 411: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 412: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 413: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 414: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 415: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 416: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 417: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 418: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 419: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 420: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 421: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 422: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 423: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 424: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 425: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 426: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 427: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 428: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 429: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 430: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 431: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 432: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 433: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 434: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 435: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 436: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 437: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 438: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 439: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 440: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 441: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 442: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 443: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 444: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 445: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 446: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 447: /* expr: '(' expr_list optional_comma ')'  */
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

  case 448: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 449: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 450: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 451: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 452: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 453: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 454: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 455: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 456: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 457: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 458: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 459: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 460: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 461: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 462: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 463: /* $@23: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 464: /* $@24: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 465: /* expr: expr "is" "type" '<' $@23 type_declaration_no_options '>' $@24  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 466: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 467: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 468: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 469: /* $@25: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 470: /* $@26: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 471: /* expr: expr "as" "type" '<' $@25 type_declaration '>' $@26  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 472: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 473: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 474: /* $@27: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 475: /* $@28: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 476: /* expr: expr '?' "as" "type" '<' $@27 type_declaration '>' $@28  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 477: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 478: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 479: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 480: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 481: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 482: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 483: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 484: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 485: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 486: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 487: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 488: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 489: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 490: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 491: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 492: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 493: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 494: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 495: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 496: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 497: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 498: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 499: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 500: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 501: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 502: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 503: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 504: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 505: /* optional_field_annotation: %empty  */
                                                { (yyval.aaList) = nullptr; }
    break;

  case 506: /* optional_field_annotation: '<' annotation_argument_list '>'  */
                                                { (yyval.aaList) = (yyvsp[-1].aaList); }
    break;

  case 507: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 508: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 509: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 510: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 511: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 512: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 513: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 514: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 515: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 516: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 517: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 518: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 519: /* $@29: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 520: /* struct_variable_declaration_list: struct_variable_declaration_list $@29 structure_variable_declaration "end of expression"  */
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

  case 521: /* $@30: %empty  */
                                                                                                                     {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 522: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@30 function_declaration_header "end of expression"  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 523: /* $@31: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 524: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@31 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 525: /* function_argument_declaration: optional_field_annotation kwd_let_var_or_nothing variable_declaration  */
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

  case 526: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 527: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 528: /* function_argument_list: function_argument_list "end of expression" function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 529: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 530: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 531: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 532: /* tuple_type_list: tuple_type_list "end of expression" tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 533: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 534: /* tuple_alias_type_list: tuple_alias_type_list "end of expression"  */
                                      {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 535: /* tuple_alias_type_list: tuple_alias_type_list tuple_type "end of expression"  */
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

  case 536: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 537: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 538: /* variant_type_list: variant_type_list "end of expression" variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 539: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 540: /* variant_alias_type_list: variant_alias_type_list "end of expression"  */
                                        {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 541: /* variant_alias_type_list: variant_alias_type_list variant_type "end of expression"  */
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

  case 542: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 543: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 544: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 545: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 546: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 547: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 548: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 549: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 550: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 551: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 552: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 553: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 554: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 555: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 556: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 557: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 558: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 559: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 560: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options "end of expression"  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 561: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 562: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 563: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr "end of expression"  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 564: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 565: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 566: /* $@32: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 567: /* global_variable_declaration_list: global_variable_declaration_list $@32 optional_field_annotation let_variable_declaration  */
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

  case 568: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 569: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 570: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 571: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 572: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 573: /* global_let: kwd_let optional_shared optional_public_or_private_variable "begin of code block" global_variable_declaration_list "end of code block"  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 574: /* $@33: %empty  */
                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 575: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@33 optional_field_annotation let_variable_declaration  */
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

  case 576: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 577: /* enum_list: enum_list "end of expression"  */
                          {
        (yyval.pEnum) = (yyvsp[-1].pEnum);
    }
    break;

  case 578: /* enum_list: enum_list "name" "end of expression"  */
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

  case 579: /* enum_list: enum_list "name" '=' expr "end of expression"  */
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

  case 580: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 581: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 582: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 583: /* $@34: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 584: /* single_alias: optional_public_or_private_alias "name" $@34 '=' type_declaration  */
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

  case 589: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 590: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 591: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 592: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 593: /* $@35: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 594: /* $@36: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 595: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name "begin of code block" $@35 enum_list $@36 "end of code block"  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-2].pEnum),Type::tInt);
    }
    break;

  case 596: /* $@37: %empty  */
                                                                                                                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 597: /* $@38: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 598: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration "begin of code block" $@37 enum_list $@38 "end of code block"  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-2].pEnum),(yyvsp[-5].type));
    }
    break;

  case 599: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 600: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 601: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 602: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 603: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 604: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 605: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 606: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 607: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 608: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 609: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 610: /* optional_struct_variable_declaration_list: "begin of code block" struct_variable_declaration_list "end of code block"  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 611: /* $@39: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 612: /* $@40: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 613: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@39 structure_name $@40 optional_struct_variable_declaration_list  */
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

  case 614: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 615: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 616: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 617: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 618: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 619: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 620: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 621: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 622: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 623: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 624: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 625: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 626: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 627: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 628: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 629: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 630: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 631: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 632: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 633: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 634: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 635: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 636: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 637: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 638: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 639: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 640: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 641: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 642: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 643: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 644: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 645: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 646: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 647: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 648: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 649: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 650: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 651: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 652: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 653: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 654: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 655: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 656: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 657: /* bitfield_bits: bitfield_bits "end of expression" "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 658: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 659: /* bitfield_alias_bits: bitfield_alias_bits "end of expression"  */
                                      {
        (yyval.pNameList) = (yyvsp[-1].pNameList);
    }
    break;

  case 660: /* bitfield_alias_bits: bitfield_alias_bits "name" "end of expression"  */
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

  case 661: /* $@41: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 662: /* $@42: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 663: /* bitfield_type_declaration: "bitfield" '<' $@41 bitfield_bits '>' $@42  */
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

  case 664: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 665: /* table_type_pair: type_declaration "end of expression" type_declaration  */
                                                                          {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 666: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 667: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 668: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 669: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 670: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 671: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 672: /* type_declaration_no_options: type_declaration_no_options dim_list  */
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

  case 673: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 674: /* $@43: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 675: /* $@44: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 676: /* type_declaration_no_options: "type" '<' $@43 type_declaration '>' $@44  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 677: /* type_declaration_no_options: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 678: /* type_declaration_no_options: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 679: /* $@45: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 680: /* type_declaration_no_options: '$' name_in_namespace '<' $@45 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 681: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 682: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 683: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 684: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 685: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 686: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 687: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 688: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 689: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 690: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 691: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 692: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 693: /* $@46: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 694: /* $@47: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 695: /* type_declaration_no_options: "smart_ptr" '<' $@46 type_declaration '>' $@47  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 696: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 697: /* $@48: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 698: /* $@49: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 699: /* type_declaration_no_options: "array" '<' $@48 type_declaration '>' $@49  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 700: /* $@50: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 701: /* $@51: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 702: /* type_declaration_no_options: "table" '<' $@50 table_type_pair '>' $@51  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 703: /* $@52: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 704: /* $@53: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 705: /* type_declaration_no_options: "iterator" '<' $@52 type_declaration '>' $@53  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 706: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 707: /* $@54: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 708: /* $@55: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 709: /* type_declaration_no_options: "block" '<' $@54 type_declaration '>' $@55  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 710: /* $@56: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 711: /* $@57: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 712: /* type_declaration_no_options: "block" '<' $@56 optional_function_argument_list optional_function_type '>' $@57  */
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

  case 713: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 714: /* $@58: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 715: /* $@59: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 716: /* type_declaration_no_options: "function" '<' $@58 type_declaration '>' $@59  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 717: /* $@60: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 718: /* $@61: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 719: /* type_declaration_no_options: "function" '<' $@60 optional_function_argument_list optional_function_type '>' $@61  */
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

  case 720: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 721: /* $@62: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 722: /* $@63: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 723: /* type_declaration_no_options: "lambda" '<' $@62 type_declaration '>' $@63  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 724: /* $@64: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 725: /* $@65: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 726: /* type_declaration_no_options: "lambda" '<' $@64 optional_function_argument_list optional_function_type '>' $@65  */
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

  case 727: /* $@66: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 728: /* $@67: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 729: /* type_declaration_no_options: "tuple" '<' $@66 tuple_type_list '>' $@67  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 730: /* $@68: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 731: /* $@69: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 732: /* type_declaration_no_options: "variant" '<' $@68 variant_type_list '>' $@69  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 733: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 734: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 735: /* type_declaration: type_declaration '|' '#'  */
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

  case 736: /* $@70: %empty  */
                                                                      {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 737: /* $@71: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 738: /* $@72: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 739: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias "name" $@70 "begin of code block" $@71 tuple_alias_type_list $@72 "end of code block"  */
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

  case 740: /* $@73: %empty  */
                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 741: /* $@74: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 742: /* $@75: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 743: /* variant_alias_declaration: "variant" optional_public_or_private_alias "name" $@73 "begin of code block" $@74 variant_alias_type_list $@75 "end of code block"  */
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

  case 744: /* $@76: %empty  */
                                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 745: /* $@77: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 746: /* $@78: %empty  */
                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 747: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias "name" $@76 "begin of code block" $@77 bitfield_alias_bits $@78 "end of code block"  */
          {
        auto btype = make_smart<TypeDecl>(Type::tBitfield);
        btype->alias = *(yyvsp[-6].s);
        btype->at = tokAt(scanner,(yylsp[-6]));
        btype->argNames = *(yyvsp[-2].pNameList);
        btype->isPrivateAlias = !(yyvsp[-7].b);
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

  case 748: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 749: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 750: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 751: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 752: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 753: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 754: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 755: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 756: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 757: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 758: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 759: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 760: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 761: /* make_struct_single: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 762: /* $@79: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 763: /* $@80: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 764: /* make_struct_decl: "struct" '<' $@79 type_declaration_no_options '>' $@80 '(' make_struct_single ')'  */
                                                                                                                                                                     {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 765: /* $@81: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 766: /* $@82: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 767: /* make_struct_decl: "class" '<' $@81 type_declaration_no_options '>' $@82 '(' make_struct_single ')'  */
                                                                                                                                                                    {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 768: /* $@83: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 769: /* $@84: %empty  */
                                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 770: /* make_struct_decl: "variant" '<' $@83 type_declaration_no_options '>' $@84 '(' make_struct_single ')'  */
                                                                                                                                                                       {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 771: /* $@85: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 772: /* $@86: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 773: /* make_struct_decl: "default" '<' $@85 type_declaration_no_options '>' $@86  */
                                                                                                                                      {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-5]));
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->useInitializer = true;
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
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

  case 776: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 777: /* $@87: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 778: /* $@88: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 779: /* make_tuple_call: "tuple" '<' $@87 type_declaration_no_options '>' $@88 '(' make_struct_single ')'  */
                                                                                                                                                                     {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 780: /* make_dim_decl: '[' expr_list optional_comma ']'  */
                                                          {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 781: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
                                                                   {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 782: /* $@89: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 783: /* $@90: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 784: /* make_dim_decl: "array" '<' $@89 type_declaration_no_options '>' $@90 '(' expr_list optional_comma ')'  */
                                                                                                                                                                              {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-9])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 785: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 786: /* $@91: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 787: /* $@92: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 788: /* make_dim_decl: "fixed_array" '<' $@91 type_declaration_no_options '>' $@92 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        if ( !mka->makeType->dim.size() ) mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 789: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 790: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 791: /* make_table_decl: "begin of code block" expr_map_tuple_list optional_comma "end of code block"  */
                                                                    {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 792: /* make_table_decl: "table" '(' expr_map_tuple_list optional_comma ')'  */
                                                                             {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 793: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' expr_map_tuple_list optional_comma ')'  */
                                                                                                                       {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-7])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-7])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 794: /* make_table_decl: "table" '<' type_declaration_no_options "end of expression" type_declaration_no_options '>' '(' expr_map_tuple_list optional_comma ')'  */
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

  case 795: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 796: /* array_comprehension_where: "end of expression" "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 797: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 798: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 799: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 800: /* array_comprehension: "begin of code block" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
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


