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
#define YYSTYPE         DAS_YYSTYPE
#define YYLTYPE         DAS_YYLTYPE
/* Substitute the variable and function names.  */
#define yyparse         das_yyparse
#define yylex           das_yylex
#define yyerror         das_yyerror
#define yydebug         das_yydebug
#define yynerrs         das_yynerrs

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

    union DAS_YYSTYPE;
    struct DAS_YYLTYPE;

    #define YY_NO_UNISTD_H
    #include "lex.yy.h"

    void das_yyerror ( DAS_YYLTYPE * lloc, yyscan_t scanner, const string & error );
    void das_yyfatalerror ( DAS_YYLTYPE * lloc, yyscan_t scanner, const string & error, CompilationError cerr );
    int yylex ( DAS_YYSTYPE *lvalp, DAS_YYLTYPE *llocp, yyscan_t scanner );
    void yybegin ( const char * str );

    void das_yybegin_reader ( yyscan_t yyscanner );
    void das_yyend_reader ( yyscan_t yyscanner );

    namespace das { class Module; }
    void das_collect_keywords ( das::Module * mod, yyscan_t yyscanner );

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

#include "ds_parser.hpp"
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
  YYSYMBOL_DAS_WITH = 40,                  /* "with"  */
  YYSYMBOL_DAS_AKA = 41,                   /* "aka"  */
  YYSYMBOL_DAS_ASSUME = 42,                /* "assume"  */
  YYSYMBOL_DAS_CAST = 43,                  /* "cast"  */
  YYSYMBOL_DAS_OVERRIDE = 44,              /* "override"  */
  YYSYMBOL_DAS_ABSTRACT = 45,              /* "abstract"  */
  YYSYMBOL_DAS_UPCAST = 46,                /* "upcast"  */
  YYSYMBOL_DAS_ITERATOR = 47,              /* "iterator"  */
  YYSYMBOL_DAS_VAR = 48,                   /* "var"  */
  YYSYMBOL_DAS_ADDR = 49,                  /* "addr"  */
  YYSYMBOL_DAS_CONTINUE = 50,              /* "continue"  */
  YYSYMBOL_DAS_WHERE = 51,                 /* "where"  */
  YYSYMBOL_DAS_PASS = 52,                  /* "pass"  */
  YYSYMBOL_DAS_REINTERPRET = 53,           /* "reinterpret"  */
  YYSYMBOL_DAS_MODULE = 54,                /* "module"  */
  YYSYMBOL_DAS_PUBLIC = 55,                /* "public"  */
  YYSYMBOL_DAS_LABEL = 56,                 /* "label"  */
  YYSYMBOL_DAS_GOTO = 57,                  /* "goto"  */
  YYSYMBOL_DAS_IMPLICIT = 58,              /* "implicit"  */
  YYSYMBOL_DAS_EXPLICIT = 59,              /* "explicit"  */
  YYSYMBOL_DAS_SHARED = 60,                /* "shared"  */
  YYSYMBOL_DAS_PRIVATE = 61,               /* "private"  */
  YYSYMBOL_DAS_SMART_PTR = 62,             /* "smart_ptr"  */
  YYSYMBOL_DAS_UNSAFE = 63,                /* "unsafe"  */
  YYSYMBOL_DAS_INSCOPE = 64,               /* "inscope"  */
  YYSYMBOL_DAS_TBOOL = 65,                 /* "bool"  */
  YYSYMBOL_DAS_TVOID = 66,                 /* "void"  */
  YYSYMBOL_DAS_TSTRING = 67,               /* "string"  */
  YYSYMBOL_DAS_TAUTO = 68,                 /* "auto"  */
  YYSYMBOL_DAS_TINT = 69,                  /* "int"  */
  YYSYMBOL_DAS_TINT2 = 70,                 /* "int2"  */
  YYSYMBOL_DAS_TINT3 = 71,                 /* "int3"  */
  YYSYMBOL_DAS_TINT4 = 72,                 /* "int4"  */
  YYSYMBOL_DAS_TUINT = 73,                 /* "uint"  */
  YYSYMBOL_DAS_TBITFIELD = 74,             /* "bitfield"  */
  YYSYMBOL_DAS_TUINT2 = 75,                /* "uint2"  */
  YYSYMBOL_DAS_TUINT3 = 76,                /* "uint3"  */
  YYSYMBOL_DAS_TUINT4 = 77,                /* "uint4"  */
  YYSYMBOL_DAS_TFLOAT = 78,                /* "float"  */
  YYSYMBOL_DAS_TFLOAT2 = 79,               /* "float2"  */
  YYSYMBOL_DAS_TFLOAT3 = 80,               /* "float3"  */
  YYSYMBOL_DAS_TFLOAT4 = 81,               /* "float4"  */
  YYSYMBOL_DAS_TRANGE = 82,                /* "range"  */
  YYSYMBOL_DAS_TURANGE = 83,               /* "urange"  */
  YYSYMBOL_DAS_TRANGE64 = 84,              /* "range64"  */
  YYSYMBOL_DAS_TURANGE64 = 85,             /* "urange64"  */
  YYSYMBOL_DAS_TBLOCK = 86,                /* "block"  */
  YYSYMBOL_DAS_TINT64 = 87,                /* "int64"  */
  YYSYMBOL_DAS_TUINT64 = 88,               /* "uint64"  */
  YYSYMBOL_DAS_TDOUBLE = 89,               /* "double"  */
  YYSYMBOL_DAS_TFUNCTION = 90,             /* "function"  */
  YYSYMBOL_DAS_TLAMBDA = 91,               /* "lambda"  */
  YYSYMBOL_DAS_TINT8 = 92,                 /* "int8"  */
  YYSYMBOL_DAS_TUINT8 = 93,                /* "uint8"  */
  YYSYMBOL_DAS_TINT16 = 94,                /* "int16"  */
  YYSYMBOL_DAS_TUINT16 = 95,               /* "uint16"  */
  YYSYMBOL_DAS_TTUPLE = 96,                /* "tuple"  */
  YYSYMBOL_DAS_TVARIANT = 97,              /* "variant"  */
  YYSYMBOL_DAS_GENERATOR = 98,             /* "generator"  */
  YYSYMBOL_DAS_YIELD = 99,                 /* "yield"  */
  YYSYMBOL_DAS_SEALED = 100,               /* "sealed"  */
  YYSYMBOL_ADDEQU = 101,                   /* "+="  */
  YYSYMBOL_SUBEQU = 102,                   /* "-="  */
  YYSYMBOL_DIVEQU = 103,                   /* "/="  */
  YYSYMBOL_MULEQU = 104,                   /* "*="  */
  YYSYMBOL_MODEQU = 105,                   /* "%="  */
  YYSYMBOL_ANDEQU = 106,                   /* "&="  */
  YYSYMBOL_OREQU = 107,                    /* "|="  */
  YYSYMBOL_XOREQU = 108,                   /* "^="  */
  YYSYMBOL_SHL = 109,                      /* "<<"  */
  YYSYMBOL_SHR = 110,                      /* ">>"  */
  YYSYMBOL_ADDADD = 111,                   /* "++"  */
  YYSYMBOL_SUBSUB = 112,                   /* "--"  */
  YYSYMBOL_LEEQU = 113,                    /* "<="  */
  YYSYMBOL_SHLEQU = 114,                   /* "<<="  */
  YYSYMBOL_SHREQU = 115,                   /* ">>="  */
  YYSYMBOL_GREQU = 116,                    /* ">="  */
  YYSYMBOL_EQUEQU = 117,                   /* "=="  */
  YYSYMBOL_NOTEQU = 118,                   /* "!="  */
  YYSYMBOL_RARROW = 119,                   /* "->"  */
  YYSYMBOL_LARROW = 120,                   /* "<-"  */
  YYSYMBOL_QQ = 121,                       /* "??"  */
  YYSYMBOL_QDOT = 122,                     /* "?."  */
  YYSYMBOL_QBRA = 123,                     /* "?["  */
  YYSYMBOL_LPIPE = 124,                    /* "<|"  */
  YYSYMBOL_LBPIPE = 125,                   /* " <|"  */
  YYSYMBOL_LLPIPE = 126,                   /* "$ <|"  */
  YYSYMBOL_LAPIPE = 127,                   /* "@ <|"  */
  YYSYMBOL_LFPIPE = 128,                   /* "@@ <|"  */
  YYSYMBOL_RPIPE = 129,                    /* "|>"  */
  YYSYMBOL_CLONEEQU = 130,                 /* ":="  */
  YYSYMBOL_ROTL = 131,                     /* "<<<"  */
  YYSYMBOL_ROTR = 132,                     /* ">>>"  */
  YYSYMBOL_ROTLEQU = 133,                  /* "<<<="  */
  YYSYMBOL_ROTREQU = 134,                  /* ">>>="  */
  YYSYMBOL_MAPTO = 135,                    /* "=>"  */
  YYSYMBOL_COLCOL = 136,                   /* "::"  */
  YYSYMBOL_ANDAND = 137,                   /* "&&"  */
  YYSYMBOL_OROR = 138,                     /* "||"  */
  YYSYMBOL_XORXOR = 139,                   /* "^^"  */
  YYSYMBOL_ANDANDEQU = 140,                /* "&&="  */
  YYSYMBOL_OROREQU = 141,                  /* "||="  */
  YYSYMBOL_XORXOREQU = 142,                /* "^^="  */
  YYSYMBOL_DOTDOT = 143,                   /* ".."  */
  YYSYMBOL_MTAG_E = 144,                   /* "$$"  */
  YYSYMBOL_MTAG_I = 145,                   /* "$i"  */
  YYSYMBOL_MTAG_V = 146,                   /* "$v"  */
  YYSYMBOL_MTAG_B = 147,                   /* "$b"  */
  YYSYMBOL_MTAG_A = 148,                   /* "$a"  */
  YYSYMBOL_MTAG_T = 149,                   /* "$t"  */
  YYSYMBOL_MTAG_C = 150,                   /* "$c"  */
  YYSYMBOL_MTAG_F = 151,                   /* "$f"  */
  YYSYMBOL_MTAG_DOTDOTDOT = 152,           /* "..."  */
  YYSYMBOL_BRABRAB = 153,                  /* "[["  */
  YYSYMBOL_BRACBRB = 154,                  /* "[{"  */
  YYSYMBOL_CBRCBRB = 155,                  /* "{{"  */
  YYSYMBOL_INTEGER = 156,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 157,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 158,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 159,    /* "unsigned long integer constant"  */
  YYSYMBOL_UNSIGNED_INT8 = 160,            /* "unsigned int8 constant"  */
  YYSYMBOL_FLOAT = 161,                    /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 162,                   /* "double constant"  */
  YYSYMBOL_NAME = 163,                     /* "name"  */
  YYSYMBOL_KEYWORD = 164,                  /* "keyword"  */
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
  YYSYMBOL_POST_INC = 192,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 193,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 194,                    /* DEREF  */
  YYSYMBOL_195_ = 195,                     /* '.'  */
  YYSYMBOL_196_ = 196,                     /* '['  */
  YYSYMBOL_197_ = 197,                     /* ']'  */
  YYSYMBOL_198_ = 198,                     /* '('  */
  YYSYMBOL_199_ = 199,                     /* ')'  */
  YYSYMBOL_200_ = 200,                     /* '$'  */
  YYSYMBOL_201_ = 201,                     /* '@'  */
  YYSYMBOL_202_ = 202,                     /* ';'  */
  YYSYMBOL_203_ = 203,                     /* '{'  */
  YYSYMBOL_204_ = 204,                     /* '}'  */
  YYSYMBOL_205_ = 205,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 206,                 /* $accept  */
  YYSYMBOL_program = 207,                  /* program  */
  YYSYMBOL_optional_public_or_private_module = 208, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 209,              /* module_name  */
  YYSYMBOL_module_declaration = 210,       /* module_declaration  */
  YYSYMBOL_character_sequence = 211,       /* character_sequence  */
  YYSYMBOL_string_constant = 212,          /* string_constant  */
  YYSYMBOL_string_builder_body = 213,      /* string_builder_body  */
  YYSYMBOL_string_builder = 214,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 215, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 216,              /* expr_reader  */
  YYSYMBOL_217_1 = 217,                    /* $@1  */
  YYSYMBOL_options_declaration = 218,      /* options_declaration  */
  YYSYMBOL_require_declaration = 219,      /* require_declaration  */
  YYSYMBOL_keyword_or_name = 220,          /* keyword_or_name  */
  YYSYMBOL_require_module_name = 221,      /* require_module_name  */
  YYSYMBOL_require_module = 222,           /* require_module  */
  YYSYMBOL_is_public_module = 223,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 224,       /* expect_declaration  */
  YYSYMBOL_expect_list = 225,              /* expect_list  */
  YYSYMBOL_expect_error = 226,             /* expect_error  */
  YYSYMBOL_expression_label = 227,         /* expression_label  */
  YYSYMBOL_expression_goto = 228,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 229,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 230,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 231,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 232, /* expression_else_one_liner  */
  YYSYMBOL_233_2 = 233,                    /* $@2  */
  YYSYMBOL_expression_if_one_liner = 234,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 235,  /* expression_if_then_else  */
  YYSYMBOL_236_3 = 236,                    /* $@3  */
  YYSYMBOL_expression_for_loop = 237,      /* expression_for_loop  */
  YYSYMBOL_expression_unsafe = 238,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 239,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 240,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 241,    /* expression_with_alias  */
  YYSYMBOL_242_4 = 242,                    /* $@4  */
  YYSYMBOL_annotation_argument_value = 243, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 244, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 245, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 246,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 247, /* annotation_argument_list  */
  YYSYMBOL_annotation_declaration_name = 248, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 249, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 250,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 251,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 252, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 253, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 254,   /* optional_function_type  */
  YYSYMBOL_function_name = 255,            /* function_name  */
  YYSYMBOL_global_function_declaration = 256, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 257, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 258, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 259,     /* function_declaration  */
  YYSYMBOL_260_5 = 260,                    /* $@5  */
  YYSYMBOL_expression_block = 261,         /* expression_block  */
  YYSYMBOL_expression_any = 262,           /* expression_any  */
  YYSYMBOL_expressions = 263,              /* expressions  */
  YYSYMBOL_expr_keyword = 264,             /* expr_keyword  */
  YYSYMBOL_expression_keyword = 265,       /* expression_keyword  */
  YYSYMBOL_266_6 = 266,                    /* $@6  */
  YYSYMBOL_267_7 = 267,                    /* $@7  */
  YYSYMBOL_expr_pipe = 268,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 269,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 270,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 271,     /* new_type_declaration  */
  YYSYMBOL_272_8 = 272,                    /* $@8  */
  YYSYMBOL_273_9 = 273,                    /* $@9  */
  YYSYMBOL_expr_new = 274,                 /* expr_new  */
  YYSYMBOL_expression_break = 275,         /* expression_break  */
  YYSYMBOL_expression_continue = 276,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 277, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 278,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 279, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 280,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 281,     /* expression_try_catch  */
  YYSYMBOL_kwd_let = 282,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 283,        /* optional_in_scope  */
  YYSYMBOL_expression_let = 284,           /* expression_let  */
  YYSYMBOL_expr_cast = 285,                /* expr_cast  */
  YYSYMBOL_286_10 = 286,                   /* $@10  */
  YYSYMBOL_287_11 = 287,                   /* $@11  */
  YYSYMBOL_288_12 = 288,                   /* $@12  */
  YYSYMBOL_289_13 = 289,                   /* $@13  */
  YYSYMBOL_290_14 = 290,                   /* $@14  */
  YYSYMBOL_291_15 = 291,                   /* $@15  */
  YYSYMBOL_expr_type_decl = 292,           /* expr_type_decl  */
  YYSYMBOL_293_16 = 293,                   /* $@16  */
  YYSYMBOL_294_17 = 294,                   /* $@17  */
  YYSYMBOL_expr_type_info = 295,           /* expr_type_info  */
  YYSYMBOL_expr_list = 296,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 297,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 298,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 299,            /* capture_entry  */
  YYSYMBOL_capture_list = 300,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 301,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 302,               /* expr_block  */
  YYSYMBOL_expr_numeric_const = 303,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 304,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe = 305,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 306,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 307,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 308,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 309,           /* func_addr_expr  */
  YYSYMBOL_310_18 = 310,                   /* $@18  */
  YYSYMBOL_311_19 = 311,                   /* $@19  */
  YYSYMBOL_312_20 = 312,                   /* $@20  */
  YYSYMBOL_313_21 = 313,                   /* $@21  */
  YYSYMBOL_expr_field = 314,               /* expr_field  */
  YYSYMBOL_315_22 = 315,                   /* $@22  */
  YYSYMBOL_316_23 = 316,                   /* $@23  */
  YYSYMBOL_expr = 317,                     /* expr  */
  YYSYMBOL_318_24 = 318,                   /* $@24  */
  YYSYMBOL_319_25 = 319,                   /* $@25  */
  YYSYMBOL_320_26 = 320,                   /* $@26  */
  YYSYMBOL_321_27 = 321,                   /* $@27  */
  YYSYMBOL_322_28 = 322,                   /* $@28  */
  YYSYMBOL_323_29 = 323,                   /* $@29  */
  YYSYMBOL_expr_mtag = 324,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 325, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 326,        /* optional_override  */
  YYSYMBOL_optional_constant = 327,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 328, /* optional_public_or_private_member_variable  */
  YYSYMBOL_structure_variable_declaration = 329, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 330, /* struct_variable_declaration_list  */
  YYSYMBOL_331_30 = 331,                   /* $@30  */
  YYSYMBOL_332_31 = 332,                   /* $@31  */
  YYSYMBOL_333_32 = 333,                   /* $@32  */
  YYSYMBOL_function_argument_declaration = 334, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 335,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 336,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 337,          /* tuple_type_list  */
  YYSYMBOL_variant_type = 338,             /* variant_type  */
  YYSYMBOL_variant_type_list = 339,        /* variant_type_list  */
  YYSYMBOL_copy_or_move = 340,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 341,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 342,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 343,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 344, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 345, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 346, /* global_variable_declaration_list  */
  YYSYMBOL_347_33 = 347,                   /* $@33  */
  YYSYMBOL_optional_shared = 348,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 349, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 350,               /* global_let  */
  YYSYMBOL_351_34 = 351,                   /* $@34  */
  YYSYMBOL_enum_list = 352,                /* enum_list  */
  YYSYMBOL_single_alias = 353,             /* single_alias  */
  YYSYMBOL_354_35 = 354,                   /* $@35  */
  YYSYMBOL_alias_list = 355,               /* alias_list  */
  YYSYMBOL_alias_declaration = 356,        /* alias_declaration  */
  YYSYMBOL_optional_public_or_private_enum = 357, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 358,                /* enum_name  */
  YYSYMBOL_enum_declaration = 359,         /* enum_declaration  */
  YYSYMBOL_optional_structure_parent = 360, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 361,          /* optional_sealed  */
  YYSYMBOL_structure_name = 362,           /* structure_name  */
  YYSYMBOL_class_or_struct = 363,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 364, /* optional_public_or_private_structure  */
  YYSYMBOL_structure_declaration = 365,    /* structure_declaration  */
  YYSYMBOL_366_36 = 366,                   /* $@36  */
  YYSYMBOL_367_37 = 367,                   /* $@37  */
  YYSYMBOL_variable_name_with_pos_list = 368, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 369,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 370, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 371, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 372,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 373,            /* bitfield_bits  */
  YYSYMBOL_bitfield_type_declaration = 374, /* bitfield_type_declaration  */
  YYSYMBOL_375_38 = 375,                   /* $@38  */
  YYSYMBOL_376_39 = 376,                   /* $@39  */
  YYSYMBOL_table_type_pair = 377,          /* table_type_pair  */
  YYSYMBOL_type_declaration_no_options = 378, /* type_declaration_no_options  */
  YYSYMBOL_379_40 = 379,                   /* $@40  */
  YYSYMBOL_380_41 = 380,                   /* $@41  */
  YYSYMBOL_381_42 = 381,                   /* $@42  */
  YYSYMBOL_382_43 = 382,                   /* $@43  */
  YYSYMBOL_383_44 = 383,                   /* $@44  */
  YYSYMBOL_384_45 = 384,                   /* $@45  */
  YYSYMBOL_385_46 = 385,                   /* $@46  */
  YYSYMBOL_386_47 = 386,                   /* $@47  */
  YYSYMBOL_387_48 = 387,                   /* $@48  */
  YYSYMBOL_388_49 = 388,                   /* $@49  */
  YYSYMBOL_389_50 = 389,                   /* $@50  */
  YYSYMBOL_390_51 = 390,                   /* $@51  */
  YYSYMBOL_391_52 = 391,                   /* $@52  */
  YYSYMBOL_392_53 = 392,                   /* $@53  */
  YYSYMBOL_393_54 = 393,                   /* $@54  */
  YYSYMBOL_394_55 = 394,                   /* $@55  */
  YYSYMBOL_395_56 = 395,                   /* $@56  */
  YYSYMBOL_396_57 = 396,                   /* $@57  */
  YYSYMBOL_397_58 = 397,                   /* $@58  */
  YYSYMBOL_398_59 = 398,                   /* $@59  */
  YYSYMBOL_399_60 = 399,                   /* $@60  */
  YYSYMBOL_400_61 = 400,                   /* $@61  */
  YYSYMBOL_401_62 = 401,                   /* $@62  */
  YYSYMBOL_402_63 = 402,                   /* $@63  */
  YYSYMBOL_type_declaration = 403,         /* type_declaration  */
  YYSYMBOL_variant_alias_declaration = 404, /* variant_alias_declaration  */
  YYSYMBOL_405_64 = 405,                   /* $@64  */
  YYSYMBOL_406_65 = 406,                   /* $@65  */
  YYSYMBOL_bitfield_alias_declaration = 407, /* bitfield_alias_declaration  */
  YYSYMBOL_408_66 = 408,                   /* $@66  */
  YYSYMBOL_make_decl = 409,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 410,       /* make_struct_fields  */
  YYSYMBOL_make_struct_dim = 411,          /* make_struct_dim  */
  YYSYMBOL_optional_block = 412,           /* optional_block  */
  YYSYMBOL_make_struct_decl = 413,         /* make_struct_decl  */
  YYSYMBOL_make_tuple = 414,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 415,           /* make_map_tuple  */
  YYSYMBOL_make_dim = 416,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 417,            /* make_dim_decl  */
  YYSYMBOL_make_table = 418,               /* make_table  */
  YYSYMBOL_make_table_decl = 419,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 420, /* array_comprehension_where  */
  YYSYMBOL_array_comprehension = 421       /* array_comprehension  */
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
         || (defined DAS_YYLTYPE_IS_TRIVIAL && DAS_YYLTYPE_IS_TRIVIAL \
             && defined DAS_YYSTYPE_IS_TRIVIAL && DAS_YYSTYPE_IS_TRIVIAL)))

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
#define YYLAST   11696

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  206
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  216
/* YYNRULES -- Number of rules.  */
#define YYNRULES  703
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1236

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   433


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
       2,     2,     2,   189,     2,   205,   200,   185,   178,     2,
     198,   199,   183,   182,   172,   181,   195,   184,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   175,   202,
     179,   173,   180,   174,   201,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   196,     2,   197,   177,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   203,   176,   204,   188,     2,     2,     2,
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
     191,   192,   193,   194
};

#if DAS_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   491,   491,   492,   493,   494,   495,   496,   497,   498,
     499,   500,   501,   502,   503,   507,   508,   509,   513,   514,
     518,   536,   537,   538,   539,   543,   547,   552,   561,   569,
     585,   590,   598,   598,   628,   649,   653,   654,   658,   661,
     665,   671,   680,   683,   689,   690,   694,   698,   699,   703,
     706,   712,   718,   721,   727,   728,   732,   733,   734,   743,
     744,   748,   749,   749,   755,   756,   757,   758,   759,   763,
     769,   769,   775,   781,   789,   799,   808,   808,   815,   816,
     817,   818,   819,   820,   824,   829,   837,   838,   839,   843,
     844,   845,   846,   847,   848,   849,   850,   856,   859,   865,
     866,   867,   871,   879,   892,   895,   903,   914,   925,   936,
     939,   946,   950,   957,   958,   962,   963,   964,   968,   971,
     978,   982,   983,   984,   985,   986,   987,   988,   989,   990,
     991,   992,   993,   994,   995,   996,   997,   998,   999,  1000,
    1001,  1002,  1003,  1004,  1005,  1006,  1007,  1008,  1009,  1010,
    1011,  1012,  1013,  1014,  1015,  1016,  1017,  1018,  1019,  1020,
    1021,  1022,  1023,  1024,  1025,  1026,  1027,  1028,  1029,  1030,
    1031,  1032,  1033,  1034,  1035,  1036,  1037,  1038,  1039,  1040,
    1041,  1042,  1043,  1044,  1045,  1046,  1047,  1048,  1049,  1050,
    1051,  1052,  1053,  1054,  1055,  1056,  1057,  1058,  1059,  1060,
    1061,  1062,  1063,  1064,  1069,  1091,  1092,  1093,  1097,  1103,
    1103,  1120,  1124,  1135,  1136,  1137,  1138,  1139,  1140,  1141,
    1142,  1143,  1144,  1145,  1146,  1147,  1148,  1149,  1150,  1151,
    1152,  1153,  1154,  1155,  1159,  1164,  1170,  1176,  1187,  1187,
    1187,  1198,  1232,  1235,  1238,  1244,  1245,  1256,  1260,  1263,
    1271,  1271,  1271,  1274,  1280,  1283,  1286,  1290,  1296,  1300,
    1304,  1307,  1310,  1318,  1321,  1324,  1332,  1335,  1343,  1346,
    1349,  1357,  1363,  1364,  1368,  1369,  1373,  1379,  1379,  1379,
    1382,  1382,  1382,  1387,  1387,  1387,  1395,  1395,  1395,  1401,
    1411,  1422,  1437,  1440,  1446,  1447,  1454,  1465,  1466,  1467,
    1471,  1472,  1473,  1474,  1478,  1483,  1491,  1492,  1496,  1501,
    1508,  1509,  1510,  1511,  1512,  1513,  1514,  1518,  1519,  1520,
    1521,  1522,  1523,  1524,  1525,  1526,  1527,  1528,  1529,  1530,
    1531,  1532,  1533,  1534,  1535,  1536,  1540,  1541,  1542,  1543,
    1544,  1545,  1549,  1556,  1568,  1573,  1583,  1587,  1594,  1597,
    1597,  1597,  1602,  1602,  1602,  1615,  1619,  1623,  1623,  1623,
    1630,  1631,  1632,  1633,  1634,  1635,  1636,  1637,  1638,  1639,
    1640,  1641,  1642,  1643,  1644,  1645,  1646,  1647,  1648,  1649,
    1650,  1651,  1652,  1653,  1654,  1655,  1656,  1657,  1658,  1659,
    1660,  1661,  1662,  1663,  1664,  1665,  1671,  1672,  1673,  1674,
    1675,  1676,  1677,  1678,  1679,  1680,  1681,  1682,  1683,  1687,
    1691,  1694,  1697,  1698,  1699,  1700,  1703,  1706,  1707,  1710,
    1710,  1710,  1713,  1718,  1722,  1726,  1726,  1726,  1731,  1734,
    1738,  1738,  1738,  1743,  1746,  1747,  1748,  1749,  1750,  1751,
    1752,  1753,  1754,  1755,  1756,  1761,  1765,  1766,  1767,  1768,
    1769,  1770,  1771,  1775,  1779,  1783,  1787,  1791,  1795,  1799,
    1803,  1807,  1814,  1815,  1819,  1820,  1821,  1825,  1826,  1830,
    1831,  1832,  1836,  1846,  1849,  1849,  1868,  1867,  1882,  1881,
    1894,  1903,  1909,  1914,  1924,  1925,  1929,  1932,  1941,  1942,
    1946,  1955,  1956,  1961,  1962,  1966,  1972,  1978,  1981,  1985,
    1991,  2000,  2001,  2002,  2006,  2007,  2011,  2018,  2023,  2032,
    2038,  2049,  2052,  2057,  2062,  2070,  2081,  2084,  2084,  2104,
    2105,  2109,  2110,  2111,  2115,  2118,  2118,  2137,  2140,  2149,
    2162,  2162,  2183,  2184,  2188,  2189,  2193,  2194,  2195,  2199,
    2209,  2216,  2226,  2227,  2231,  2232,  2236,  2242,  2243,  2247,
    2248,  2249,  2253,  2258,  2253,  2270,  2277,  2282,  2291,  2297,
    2308,  2309,  2310,  2311,  2312,  2313,  2314,  2315,  2316,  2317,
    2318,  2319,  2320,  2321,  2322,  2323,  2324,  2325,  2326,  2327,
    2328,  2329,  2330,  2331,  2332,  2333,  2334,  2338,  2339,  2340,
    2341,  2342,  2343,  2347,  2358,  2362,  2369,  2381,  2388,  2397,
    2397,  2397,  2410,  2414,  2421,  2422,  2423,  2424,  2425,  2439,
    2445,  2449,  2453,  2458,  2463,  2468,  2473,  2477,  2481,  2486,
    2490,  2494,  2499,  2499,  2499,  2505,  2512,  2512,  2512,  2517,
    2517,  2517,  2523,  2523,  2523,  2528,  2532,  2532,  2532,  2537,
    2537,  2537,  2546,  2550,  2550,  2550,  2555,  2555,  2555,  2564,
    2568,  2568,  2568,  2573,  2573,  2573,  2582,  2582,  2582,  2588,
    2588,  2588,  2597,  2600,  2611,  2627,  2627,  2627,  2651,  2651,
    2671,  2672,  2673,  2674,  2678,  2685,  2692,  2698,  2707,  2712,
    2719,  2720,  2724,  2730,  2737,  2745,  2752,  2760,  2772,  2775,
    2781,  2795,  2801,  2807,  2812,  2819,  2824,  2834,  2839,  2846,
    2858,  2859,  2863,  2866
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
  "\"finally\"", "\"delete\"", "\"deref\"", "\"typedef\"", "\"with\"",
  "\"aka\"", "\"assume\"", "\"cast\"", "\"override\"", "\"abstract\"",
  "\"upcast\"", "\"iterator\"", "\"var\"", "\"addr\"", "\"continue\"",
  "\"where\"", "\"pass\"", "\"reinterpret\"", "\"module\"", "\"public\"",
  "\"label\"", "\"goto\"", "\"implicit\"", "\"explicit\"", "\"shared\"",
  "\"private\"", "\"smart_ptr\"", "\"unsafe\"", "\"inscope\"", "\"bool\"",
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
  "\"?[\"", "\"<|\"", "\" <|\"", "\"$ <|\"", "\"@ <|\"", "\"@@ <|\"",
  "\"|>\"", "\":=\"", "\"<<<\"", "\">>>\"", "\"<<<=\"", "\">>>=\"",
  "\"=>\"", "\"::\"", "\"&&\"", "\"||\"", "\"^^\"", "\"&&=\"", "\"||=\"",
  "\"^^=\"", "\"..\"", "\"$$\"", "\"$i\"", "\"$v\"", "\"$b\"", "\"$a\"",
  "\"$t\"", "\"$c\"", "\"$f\"", "\"...\"", "\"[[\"", "\"[{\"", "\"{{\"",
  "\"integer constant\"", "\"long integer constant\"",
  "\"unsigned integer constant\"", "\"unsigned long integer constant\"",
  "\"unsigned int8 constant\"", "\"floating point constant\"",
  "\"double constant\"", "\"name\"", "\"keyword\"",
  "\"start of the string\"", "STRING_CHARACTER", "STRING_CHARACTER_ESC",
  "\"end of the string\"", "\"{\"", "\"}\"",
  "\"end of failed eader macro\"", "','", "'='", "'?'", "':'", "'|'",
  "'^'", "'&'", "'<'", "'>'", "'-'", "'+'", "'*'", "'/'", "'%'",
  "UNARY_MINUS", "UNARY_PLUS", "'~'", "'!'", "PRE_INC", "PRE_DEC",
  "POST_INC", "POST_DEC", "DEREF", "'.'", "'['", "']'", "'('", "')'",
  "'$'", "'@'", "';'", "'{'", "'}'", "'#'", "$accept", "program",
  "optional_public_or_private_module", "module_name", "module_declaration",
  "character_sequence", "string_constant", "string_builder_body",
  "string_builder", "reader_character_sequence", "expr_reader", "$@1",
  "options_declaration", "require_declaration", "keyword_or_name",
  "require_module_name", "require_module", "is_public_module",
  "expect_declaration", "expect_list", "expect_error", "expression_label",
  "expression_goto", "elif_or_static_elif", "expression_else",
  "if_or_static_if", "expression_else_one_liner", "$@2",
  "expression_if_one_liner", "expression_if_then_else", "$@3",
  "expression_for_loop", "expression_unsafe", "expression_while_loop",
  "expression_with", "expression_with_alias", "$@4",
  "annotation_argument_value", "annotation_argument_value_list",
  "annotation_argument_name", "annotation_argument",
  "annotation_argument_list", "annotation_declaration_name",
  "annotation_declaration_basic", "annotation_declaration",
  "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "global_function_declaration",
  "optional_public_or_private_function", "function_declaration_header",
  "function_declaration", "$@5", "expression_block", "expression_any",
  "expressions", "expr_keyword", "expression_keyword", "$@6", "$@7",
  "expr_pipe", "name_in_namespace", "expression_delete",
  "new_type_declaration", "$@8", "$@9", "expr_new", "expression_break",
  "expression_continue", "expression_return_no_pipe", "expression_return",
  "expression_yield_no_pipe", "expression_yield", "expression_try_catch",
  "kwd_let", "optional_in_scope", "expression_let", "expr_cast", "$@10",
  "$@11", "$@12", "$@13", "$@14", "$@15", "expr_type_decl", "$@16", "$@17",
  "expr_type_info", "expr_list", "block_or_simple_block",
  "block_or_lambda", "capture_entry", "capture_list",
  "optional_capture_list", "expr_block", "expr_numeric_const",
  "expr_assign", "expr_assign_pipe", "expr_named_call", "expr_method_call",
  "func_addr_name", "func_addr_expr", "$@18", "$@19", "$@20", "$@21",
  "expr_field", "$@22", "$@23", "expr", "$@24", "$@25", "$@26", "$@27",
  "$@28", "$@29", "expr_mtag", "optional_field_annotation",
  "optional_override", "optional_constant",
  "optional_public_or_private_member_variable",
  "structure_variable_declaration", "struct_variable_declaration_list",
  "$@30", "$@31", "$@32", "function_argument_declaration",
  "function_argument_list", "tuple_type", "tuple_type_list",
  "variant_type", "variant_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list", "let_variable_declaration",
  "global_variable_declaration_list", "$@33", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@34", "enum_list",
  "single_alias", "$@35", "alias_list", "alias_declaration",
  "optional_public_or_private_enum", "enum_name", "enum_declaration",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "structure_declaration", "$@36", "$@37", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_type_declaration", "$@38", "$@39", "table_type_pair",
  "type_declaration_no_options", "$@40", "$@41", "$@42", "$@43", "$@44",
  "$@45", "$@46", "$@47", "$@48", "$@49", "$@50", "$@51", "$@52", "$@53",
  "$@54", "$@55", "$@56", "$@57", "$@58", "$@59", "$@60", "$@61", "$@62",
  "$@63", "type_declaration", "variant_alias_declaration", "$@64", "$@65",
  "bitfield_alias_declaration", "$@66", "make_decl", "make_struct_fields",
  "make_struct_dim", "optional_block", "make_struct_decl", "make_tuple",
  "make_map_tuple", "make_dim", "make_dim_decl", "make_table",
  "make_table_decl", "array_comprehension_where", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-933)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-654)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -933,    29,  -933,  -933,    46,   -23,   102,   -87,  -933,   -94,
    -933,  -933,    -6,  -933,  -933,  -933,  -933,  -933,   326,  -933,
      89,  -933,  -933,  -933,  -933,  -933,  -933,  -933,  -933,  -933,
     -37,  -933,    35,    11,    44,  -933,  -933,  -933,   102,  -933,
      -5,  -933,  -933,    63,  -933,  -933,  -933,    89,   113,   126,
    -933,  -933,    -6,   169,   230,    -6,    -6,   176,  -933,   451,
      -2,  -933,  -933,  -933,   391,   407,   439,  -933,   459,    19,
      46,   241,   -23,   237,   284,  -933,   355,   355,  -933,   373,
     304,  -104,   462,   346,  -933,  -933,  -933,   394,  -933,   -41,
      46,    -6,    -6,    -6,    -6,  -933,  -933,  -933,  -933,  -933,
    -933,  -933,   399,  -933,  -933,  -933,  -933,  -933,   380,  -933,
    -933,  -933,  -933,  -933,   440,    58,  -933,  -933,  -933,  -933,
     545,  -933,  -933, 11345,  -933,  -933,   410,  -933,  -933,  -933,
     454,   419,  -933,  -933,   -26,  -933,   430,   488,   451, 11533,
    -933,   -82,   531,  -933,   482,  -933,  -933,   460,  -933,  -933,
    -933,  -933,  -933,  -933,  -933,   157,  -933,   457,   458,   463,
     468,  -933,  -933,  -933,   441,  -933,  -933,  -933,  -933,  -933,
     469,  -933,  -933,  -933,  -933,  -933,  -933,  -933,  -933,  -933,
    -933,  -933,   470,  -933,  -933,  -933,   471,   472,  -933,  -933,
    -933,  -933,   473,   474,   443,  -933,  -933,  -933,  -933,  -933,
    1684,   479,  -933,  -933,   436,   483,  -933, 10102,  -933,  -933,
    -933,  -933,  -933,  -933,  -933,  -933,  -933,  -933,  -933,  -933,
    -933,  -933,  -933,  -933,  -933,  -933,  -933,  -933,  -933,  -933,
    -933,  -933,  -933,   623,   626,  -933,   465,   466,   409,  -933,
    -933,   498,  -933,   464,    46,   118,  -933,  -933,  -933,    58,
    -933,  -933,  -933,  -933,  -933,   503,  -933,   301,   309,   317,
    -933,  -933,  6472,  -933,  -933,  -933,    13,  -933,  -933,  -933,
      10,  3656,  -933,   226,   -97,   505,  -933,   481,   518,   519,
    -933,  -933,  -933,  -933,  -933,  -933,  -933,  -933,  -933,  -933,
    -933,  -933,  -933,  -933,  -933,  -933,  -933,  -933,  -933,  -933,
     521,   495,  -933,  -933,  -933,  -933,  -933,  -933,  -933,  -933,
    -933,  -933,  -933,  -933,   672,  -933,  -933,  -933,  -933,  -933,
    -933,  -933,  -933,  -933,  -933,  -933,  -933,   522,   501,  -933,
    -933,    21,   529,  -933,  -933,  -933,  -933,  -933,  -933,  -933,
    -933,   502,   -96,   533,   506,  -933,   482,   205,   497,   665,
     333,  -933,  -933, 11345, 11345, 11345, 11345,   511,   454, 11345,
     465, 11345,   465, 11345,   465, 11444,   483,  -933,  -933,   245,
     513,   535,  -933,   517,   537,   540,   523,   541,   526,  -933,
     546,  6472,  6472,   528,   530,   532,   534,   551,   552,  -933,
   11019, 11118,  6472,  -933,  -933,  -933,  -933,  -933,  -933,  -933,
     548,  -933,  6472,  6472,  6472,   202,  6472,  6472,  6472,  -933,
     538,  -933,  -933,  -933,  -933,   -22,  -933,  -933,  -933,  -933,
     542,  -933,  -933,  -933,  -933,  -933,  -933,  1428,  -933,   553,
    -933,  -933,  -933,  -933,  -933,  -933,  -933,  -933,  -933,   547,
    -933,  -933,  9514,  -933,  1684,  -933,  -933, 11345,   -75,  -933,
    -933,  -933,  -933,   568,  -933,  -933,   554,  -933,     3,  -933,
    -128, 11345,  -933,  1625,  -933,  -103,  -933,   202,  -933,  -933,
     118,   557,  6472,   594,   595, 11345,  -933,    57,   360,   579,
      81,   372,   383,  -933,   -30,   388,   529,   395,   529,   396,
     529,   -73,  -933,   165,   479,   193,  -933,   562,  -933,  -933,
     202,  -933,  6472,  -933,  -933,  6472,  -933,  6472, 11345,   314,
     314,  6472,  6472,  6472,  6472,  6472,  6472,   192,  2008,   192,
    2184,   348,  -933,   416,  -933,   426,   314,   314,   -28,  -933,
     314,   314,  7070,   101,  -933,  3480,   611, 10276, 11151,  6472,
    6472,  -933,  -933,  6472,  6472,  6472,  6472,   602,  6472,   173,
    6472,  6472,  6472,  6472,  6472,  6472,  6472,  6472,  6472,  3809,
    6472,  6472,  6472,  6472,  6472,  6472,  6472,  6472,  6472,  6472,
     -31,  6472,  -933,  3962,  -933,  -933,   479,  -933,  -933,  -933,
    6472,   192,   570,   725,  -933,   -63,  -933,   253,   479,  -933,
    6472,  -933,  -933,   192,  2536,  -933,   466,  4138,  6472,   606,
    -933,   575,   614,  4291,   292,  2689,  -116,  -116,  -116,  4444,
    -933,   736,   576,   577,  6472,   773,  -933,  -933,  -933,  -933,
    -933,  -933,  -933,  -933,  -933,   581,   584,   585,   586,  -933,
     590,  -933,  -933,   729,  -933,   -71,  -933,  1321,   -62,  6472,
    -933,  -933,    14,  -933,  -933,  7164,  -933,   753,   819,  -933,
    -933,  -933,  3021,  -933,  -933, 11345,  -933,  -933,  -933,   632,
    -933,   616,  -933,   617,  -933,   618, 11345,  -933, 11444,  -933,
     483, 11345,  4620,  4796, 11345,  7258, 11345, 11345,  7352, 11345,
    7446,   654,  7540,  7634,  7728,  7822,  7916,  8010,     6,  -116,
     154,  2360,  4972, 10201,   627,   -13,   620,   641,   340,    22,
    5148,   -13,   428,  6472,  6472,   621, 11345,  -933,  6472,   467,
     652,  -933,   628,   629,   318,  -933,  -933,   658,  -933,   186,
   10468,    51,   465,   643,   633,  -933,  -933,   645,   638,  -933,
    -933,    16,    16,  1636,  1636, 10880, 10880,   648,    32,   650,
    -933,  9608,   -33,   -33,    16,    16,  1093, 10686, 10767, 10573,
   11246, 10282,   624, 10799,   492,  1636,  1636,   578,   578,    32,
      32,    32,   220,  6472,   651,  -933,   288,  6472,   840,  9702,
    -933,   204,  8104,  -933,  6472,   667,  -933,   680,  -933, 11345,
    -933,  3021,  -933,   966,    30,  3021,  -933,   727,  9995,   841,
    6472, 10468,   966,   683,  -933,   678,   701, 10468,  -933,  3021,
    -933,  9995,   657,  -933,  -933,  -933,   966,   660,  -933,  -933,
     966,  -933,  -933,  -933,  -933,  -933,  -933,  -933,   118,  -116,
    -933,  6472,  6472,  6472,  6472,  6472,  6472,  6472,  6472,  6472,
    6472,  3174,  6472,  6472,  6472,  6472,  6472,  6472,  3327,  -933,
    6882,    -6,  -933,   855,   482,  -933,   702,  -933,  3021,  -933,
    6672,  -933,  -933,   479,  -933,  -933,  -933,  -933,  -933,  -933,
    -933,  -933,  -933,   479,  -933,  -933,  -933,   401,  -933,   214,
     703,  8198,   404,  -933,   659,   664,  -933,   829,  -933,   611,
    -933,  -933,  -933,  -933,  -933,   669,  6472,  -933,  6472,  6472,
      -4,  6472,   705,   658,   673,   675,  6472,   676,  6472,  6472,
     658,   670,   679, 10468,  -933,  -933,  1003, 10387,  -933,   709,
    6472,  6472, 11345,   465,   152,   218,  5324,  -933,   716,   718,
     719,   720,  -933,   247,   529,  -933,  6472,  -933,  6472,  5485,
    6472,  -933,   706,   686,  -933,  -933,  6472,   688,  -933,  9796,
    6472,   691,  -933,  9890,  -933,  -933,  6472,  -933,  -933,  8292,
    -933,   849,   -59,  -933,  9995,  -933,  6472,  -933,  9995,  6472,
    6472,   466, 10468,  -933,  -933,  -933,  -933,  -933,  9995,  -933,
    -933,  -933,   431,  6472,  -933,  -933, 10468, 10468, 10468, 10468,
   10468, 10468, 10468, 10468, 10468, 10468,  -116,  -116,  -116, 10468,
   10468, 10468, 10468, 10468, 10468, 10468,  -116,  -116,  -116, 10468,
    -933,   276,   480,    34,   689,  -933,  -933,  6777,  -933,  -933,
    -933,  -933,  -933,  -933,   238,  -933,  -933,  -933,  -933,  -933,
     694,  5661,    75, 10468, 10468,   -13,   696, 10468,   155,   627,
     697,  -933, 10468,  -933,   641,    76,   -13,   698,  -933,  -933,
    -933,  -933,  8386,  8480,  1106,   529,   699,   658, 10468,  -933,
    -933,  -933,  -933,    51,   700,   -79, 11345,  8574, 11345,  8668,
    -933,   224,  8762,  -933,  6472, 10654,  6472,  -933,  8856,  6472,
    -933,  -933,  -933,   733,  6472,    77,  -933,  6472,  1829,   466,
    -933,  -933,  6472,  -933,  6567,  -933,  -933,  -933,  -933,  -933,
    -933,   704,  -933,  -933,    60,  -933,  -933,   480,  -933,  -933,
    -933,  6472,   738,  -933,  6472,  6472,  6472,  5837,  -933,   242,
    6472,   707,   708,  6472,  6472,  -933,  6472,   695,  -933,  6472,
    -933,  -933,  -933,   722,  -933,   278,  -933,  -933,  6013,  -933,
    -933,  1196,  -933,   406,  -933,  -933,  -933, 11345,  8950,  9044,
    -933,  9138,  -933, 10468,   466, 10468,  -933,  -933,   966,  -933,
     712,  -933,   871,   871,   192,  9232,   728,  1636,  1636,  1636,
    -933,  9326,  -933,  6976,   710,  -933, 10468, 10468,  6976,   714,
    1636,   123,  -933,   726,  6472, 10573,  -933,  -933,   411,  -933,
    -933,  -933,  -933,   431,  2868,  -933,  -933,  -933,  -933,  -933,
    -933,  6472,  -933,   858,   721,  -933,   713,  -933,  -933,   123,
    -933, 10654,  -933,  -933,  -933,  -933,  6166,  6319,  -933,  -933,
    -933,  -933,  -933, 10468, 11533, 11533,  9420,  6472,   730,   732,
    -933,  -933,  6472, 10468,  6472, 10468,   731,   466,  -933, 10468,
    -933,  -933, 10468, 10468,  -933,  -933
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   113,     1,   272,     0,     0,     0,     0,   273,     0,
     668,   665,     0,    14,     3,    10,     9,     8,     0,     7,
     519,     6,    11,     5,     4,    12,    13,    87,    88,    86,
      95,    97,    34,    49,    46,    47,    36,    37,     0,    38,
      44,    35,   530,     0,   535,    19,    18,   519,     0,     0,
     100,   101,     0,     0,   245,     0,     0,   102,   104,   111,
       0,    99,   548,   547,   205,   536,   549,   520,   521,     0,
       0,     0,     0,    39,     0,    45,     0,     0,    42,     0,
       0,     0,    15,     0,   666,   110,   247,     0,   105,     0,
       0,     0,     0,     0,     0,   114,   207,   206,   209,   204,
     538,   537,     0,   551,   550,   552,   523,   522,   525,    93,
      94,    91,    92,    90,     0,     0,    89,    98,    50,    48,
      44,    41,    40,     0,   532,   534,     0,    16,    17,    20,
       0,     0,   246,   109,     0,   106,   107,   108,   112,     0,
     539,     0,   544,   516,   462,    21,    22,     0,    82,    83,
      80,    81,    79,    78,    84,     0,    43,     0,     0,     0,
       0,   560,   580,   561,   594,   562,   566,   567,   568,   569,
     586,   573,   574,   575,   576,   577,   578,   579,   581,   582,
     583,   584,   635,   565,   572,   585,   642,   649,   563,   570,
     564,   571,     0,     0,     0,   593,   604,   607,   605,   606,
     662,   531,   533,   597,     0,     0,   103,     0,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,     0,     0,   120,   115,     0,     0,   527,
     545,     0,   553,   517,     0,     0,    23,    24,    25,     0,
      96,   626,   629,   632,   622,     0,   599,   636,   643,   650,
     656,   659,     0,   612,   617,   611,     0,   625,   621,   614,
       0,     0,   616,     0,     0,     0,   491,     0,   173,   174,
     171,   123,   124,   126,   125,   127,   128,   129,   130,   156,
     157,   154,   155,   147,   158,   159,   148,   145,   146,   172,
     167,     0,   170,   160,   161,   162,   163,   134,   135,   136,
     131,   132,   133,   144,     0,   150,   151,   149,   142,   143,
     138,   137,   139,   140,   141,   122,   121,   166,     0,   152,
     153,   462,   118,   234,   210,   587,   590,   588,   591,   589,
     592,     0,     0,   542,     0,   524,   462,     0,     0,   506,
     504,   526,    85,     0,     0,     0,     0,     0,     0,     0,
     115,     0,   115,     0,   115,     0,     0,   366,   367,     0,
       0,     0,   360,     0,     0,     0,     0,     0,     0,   586,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   451,
       0,     0,     0,   310,   312,   311,   313,   314,   315,   316,
       0,    26,     0,     0,     0,     0,     0,     0,     0,   297,
     298,   364,   363,   308,   445,   361,   437,   436,   435,   434,
     113,   440,   362,   439,   438,   407,   368,     0,   369,     0,
     365,   670,   671,   672,   673,   619,   620,   613,   615,     0,
     618,   609,     0,   664,   663,   598,   669,     0,     0,   175,
     176,   169,   165,   177,   168,   164,     0,   116,     0,   484,
       0,     0,   208,     0,   527,     0,   540,     0,   546,   473,
       0,     0,     0,     0,     0,     0,   505,     0,     0,     0,
     602,     0,     0,   595,     0,     0,   118,     0,   118,     0,
     118,   245,   488,     0,   486,     0,   250,   254,   253,   257,
       0,   286,     0,   277,   280,     0,   283,     0,     0,   396,
     397,     0,     0,     0,     0,     0,     0,     0,   680,     0,
       0,   692,   697,     0,   238,     0,   373,   372,   412,    32,
     371,   370,     0,   299,   443,     0,   306,     0,     0,     0,
       0,   398,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     357,     0,   596,     0,   610,   608,   490,   667,   492,   178,
       0,     0,     0,   555,   481,   495,   117,   462,   119,   236,
       0,    59,    60,     0,   260,   258,     0,     0,     0,     0,
     259,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     213,   211,     0,     0,     0,     0,   229,   224,   221,   220,
     222,   223,   235,   215,   214,     0,    67,    68,    65,   227,
      66,   228,   230,   275,   219,     0,   216,   317,     0,     0,
     528,   543,   474,   518,   463,     0,   508,   509,     0,   502,
     503,   501,     0,   627,   630,     0,   633,   623,   600,     0,
     637,     0,   644,     0,   651,     0,     0,   657,     0,   660,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     245,     0,     0,   688,   678,   680,     0,   693,     0,     0,
       0,   680,     0,     0,     0,     0,     0,    29,     0,    27,
       0,   400,     0,     0,   349,   346,   348,     0,   408,     0,
     292,     0,   115,     0,     0,   423,   422,     0,     0,   424,
     428,   374,   375,   387,   388,   385,   386,     0,   417,     0,
     405,     0,   441,   442,   376,   377,   392,   393,   394,   395,
       0,     0,   390,   391,   389,   383,   384,   379,   378,   380,
     381,   382,     0,     0,     0,   355,     0,     0,     0,     0,
     410,     0,     0,   482,     0,     0,   494,     0,   493,     0,
     496,     0,   485,     0,     0,     0,   264,     0,   261,     0,
       0,   248,     0,     0,   233,     0,     0,    53,    73,     0,
     269,   266,   298,   244,   242,   243,     0,     0,   231,   232,
       0,    70,   218,   225,   226,   263,   268,   274,     0,     0,
     217,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   541,
       0,     0,   554,     0,   462,   507,     0,   511,     0,   515,
     317,   628,   631,   603,   634,   624,   601,   638,   640,   645,
     647,   652,   654,   487,   658,   489,   661,     0,   255,     0,
       0,     0,     0,   413,     0,     0,   414,     0,   444,   306,
     446,   447,   448,   449,   450,     0,     0,   681,     0,     0,
     680,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   691,   698,   699,     0,     0,    30,    33,
       0,     0,     0,   115,     0,     0,     0,   409,     0,     0,
       0,     0,   304,     0,   118,   419,     0,   425,     0,     0,
       0,   403,     0,     0,   429,   433,     0,     0,   406,     0,
       0,     0,   356,     0,   358,   401,     0,   411,   483,     0,
     557,   558,   497,   500,   499,    74,     0,   265,   262,     0,
       0,     0,   249,    75,    76,    51,    52,   270,   267,   299,
     237,   234,    56,     0,   276,   241,   327,   328,   330,   329,
     331,   321,   322,   323,   332,   333,     0,     0,     0,   319,
     320,   334,   335,   324,   325,   326,     0,     0,     0,   318,
     529,     0,   469,   464,     0,   510,   513,   317,   514,   641,
     648,   655,   251,   256,     0,   289,   287,   278,   281,   284,
       0,     0,     0,   675,   674,   680,     0,   689,     0,   679,
       0,   683,   690,   695,   694,     0,   680,     0,   696,   239,
      28,    31,     0,     0,     0,   118,     0,     0,   293,   302,
     303,   301,   300,     0,     0,     0,     0,     0,     0,     0,
     344,     0,     0,   430,     0,   418,     0,   404,     0,     0,
     402,   359,   556,     0,     0,     0,   271,     0,     0,     0,
      54,    55,     0,    69,    61,   341,   339,   340,   338,   336,
     337,   114,   470,   471,   464,   465,   466,   469,   475,   512,
     252,     0,     0,   288,     0,     0,     0,     0,   452,     0,
       0,     0,     0,     0,     0,   682,     0,     0,   686,     0,
     347,   461,   350,     0,   342,     0,   305,   307,     0,   294,
     309,     0,   460,     0,   458,   345,   455,     0,     0,     0,
     454,     0,   559,   498,     0,    77,   212,    57,     0,    62,
       0,   480,   467,   467,     0,     0,     0,   279,   282,   285,
     415,     0,   453,   700,     0,   684,   677,   676,   700,     0,
     240,     0,   353,     0,     0,   295,   420,   426,     0,   459,
     457,   456,    72,    56,     0,    71,   468,   476,   478,   472,
     290,     0,   416,     0,     0,   685,     0,   687,   351,     0,
     343,   296,   421,   427,   431,    58,   260,     0,    63,    67,
      68,    65,    66,    64,     0,     0,     0,     0,     0,     0,
     354,   432,     0,   261,     0,   266,     0,     0,   291,   701,
     702,   703,   262,   267,   477,   479
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -933,  -933,  -933,  -933,  -933,   405,   862,  -933,  -933,  -933,
    -933,  -933,  -933,  -933,   520,   894,  -933,   814,  -933,  -933,
     866,  -933,  -933,  -933,  -242,  -933,  -933,  -933,  -241,  -933,
    -933,  -933,  -933,  -933,  -933,  -933,  -933,   693,  -933,  -933,
     874,   -51,  -933,  -933,   336,   114,  -401,  -352,  -479,  -933,
    -933,  -933,  -676,  -933,  -933,  -232,  -933,     7,  -933,  -933,
    -933,  -933,  -577,   -12,  -933,  -933,  -933,  -933,  -933,  -239,
    -204,  -203,  -933,  -202,  -933,  -933,   982,  -933,  -933,  -933,
    -933,  -933,  -933,  -933,  -933,  -933,  -933,  -933,  -933,  -933,
    -547,  -933,  -933,   -69,  -933,   106,  -576,  -933,  -459,  -933,
    -933,  -933,  -932,  -933,  -933,  -933,  -933,  -933,  -933,  -933,
    -933,   408,  -933,  -933,  -933,  -933,  -933,  -933,  -933,  -141,
    -105,  -163,  -106,  -933,  -933,  -933,  -933,  -933,   412,  -933,
     328,  -933,  -425,   635,  -572,  -575,   356,  -933,  -933,  -455,
    -933,  -933,   947,  -933,  -933,  -933,   544,   104,  -933,  -933,
    -933,  -933,  -933,  -933,  -933,  -933,  -933,  -933,  -933,  -933,
    -933,  -933,  -471,  -121,  -933,   649,  -933,   656,  -933,  -933,
    -933,  -933,  -272,  -933,  -933,  -933,  -933,  -933,  -933,  -933,
    -933,  -933,  -933,  -933,  -933,  -933,  -933,  -933,  -933,  -933,
    -933,  -933,  -933,  -933,  -933,  -933,  -933,  -101,  -933,  -933,
    -933,  -933,  -933,   661,  -697,  -506,  -677,  -933,   121,   308,
     500,  -933,  -933,  -933,  -145,  -933
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   129,    47,    14,   147,   153,   525,   411,   909,
     412,   710,    15,    16,    39,    40,    41,    78,    17,    34,
      35,   612,   613,  1082,  1083,   614,  1150,  1184,   615,   616,
     973,   617,   618,   619,   620,   621,  1077,   154,   155,    30,
      31,    32,    57,    58,    59,    60,    18,   332,   462,   236,
      19,    98,   237,    99,   139,   413,   622,   463,   623,   414,
     706,  1119,   624,   415,   625,   497,   671,  1100,   416,   626,
     627,   628,   629,   630,   631,   632,   633,   818,   634,   417,
     676,  1104,   677,  1105,   679,  1106,   418,   674,  1103,   419,
     719,  1130,   420,   922,   923,   722,   421,   422,   787,   636,
     423,   424,   716,   425,   912,  1171,   913,  1199,   426,   768,
    1071,   720,  1056,  1202,  1058,  1203,  1137,  1221,   428,   458,
    1097,  1187,  1094,  1004,   642,   844,  1214,  1215,   459,   460,
     492,   493,   276,   277,   889,   584,   652,   477,   350,   351,
     243,   346,    68,   108,    21,   144,   342,    44,    79,    81,
      22,   102,   141,    23,   468,   241,   242,    66,   105,    24,
     142,   344,   585,   429,   341,   197,   198,   204,   199,   358,
     856,   479,   200,   356,   855,   353,   851,   354,   852,   355,
     854,   359,   857,   360,  1009,   361,   859,   362,  1010,   363,
     861,   364,  1011,   365,   864,   366,   866,   494,    25,    49,
     131,    26,    48,   430,   694,   695,   696,   431,   697,   522,
     698,   432,   523,   433,  1194,   434
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      61,   444,   196,   245,   635,   334,   773,   661,   486,   663,
     488,   665,   490,   781,   701,   643,    74,   786,   894,   536,
     915,  -113,   201,   578,   901,   886,   771,    50,   800,     2,
     803,   804,   805,   109,   110,     3,   537,   538,   689,   134,
      61,   899,   437,    61,    61,   435,   688,   689,   699,   956,
      75,   581,   537,   538,   819,    51,  1128,   776,     4,    42,
       5,   776,     6,    87,    27,    28,   445,   465,     7,    45,
     639,   586,   148,   149,   587,   849,    42,     8,  1095,    61,
      61,    61,    61,     9,   409,   802,   547,   333,   275,   549,
     550,   762,   763,   238,   549,   550,    91,    92,    93,   640,
     125,   465,   666,    10,  1095,  1152,    46,   446,   466,   777,
     778,   195,   779,   887,   778,   780,    43,   273,   518,   520,
     764,   239,   784,    52,   333,   869,    11,   541,   542,   577,
      53,   820,   765,    33,  1096,   547,    69,   548,   549,   550,
     551,   534,   839,   541,   542,   552,    70,    80,   582,    67,
     658,   547,   196,   548,   549,   550,   551,    54,   133,   914,
    1096,   552,   570,   571,   766,   767,   583,   570,   571,   456,
      94,   918,   659,   206,   244,   111,   535,   649,   777,    76,
     112,   919,   113,    55,   114,   126,    71,   650,   438,   893,
      77,   436,    56,   347,   777,    95,  1029,   565,   566,   567,
     568,   569,   777,   648,   953,   470,   439,    70,   957,    29,
     841,   570,   571,  1026,   150,   440,    72,   115,   842,   151,
     457,   152,   967,   114,   920,    12,    42,   570,   571,   921,
     651,    13,   196,   196,   196,   196,   681,    53,   196,  1198,
     196,   843,   196,   975,   196,   578,   712,   946,   946,   946,
     157,   713,   478,   480,   481,   482,   158,   273,   485,    53,
     487,   195,   489,   348,    54,    36,    37,  1220,   712,   196,
     196,  1006,   776,   159,   776,   776,    83,  1110,  1116,  1144,
     714,   349,   888,   655,   888,  1113,    54,    38,   160,    84,
      87,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   739,   778,   196,   778,   778,   249,
      62,    63,    86,    64,   537,   538,   740,   582,    53,  1022,
     196,   195,   195,   195,   195,   667,   576,   195,  1111,   195,
    1125,   195,  1035,   195,   196,   583,   250,   195,   916,  1117,
     588,    65,    53,   974,   789,    54,    87,   668,   537,   538,
     924,   937,   798,   669,    90,   194,   946,    70,   195,   195,
    1074,    53,  1061,   938,  1025,   917,   946,   196,    85,    54,
     892,    88,    89,   529,  1036,   670,   946,   118,   390,   391,
     392,   456,   471,   947,   874,   875,   244,   877,    54,  1075,
    1085,  1086,  1087,  1013,   946,  1046,   726,   730,  1101,  1053,
    1088,  1089,  1090,  1135,   496,   541,   542,   135,   136,   137,
     138,   443,    77,   547,   906,   195,   549,   550,   551,   941,
    1102,  1162,  1079,   552,  1054,  1055,    96,   120,    94,   195,
     892,   942,    97,  1080,  1081,   641,  1114,   539,   540,   541,
     542,   543,   100,   195,   544,   545,   546,   547,   101,   548,
     549,   550,   551,  1091,  1109,  1173,  -639,   552,   335,   553,
     554,  -639,   336,   703,  -646,   555,   556,   557,   673,  -646,
     507,   558,  -653,  -352,   103,   333,   195,  -653,  -352,  -639,
     104,   337,   338,   339,   340,   474,   124,  -646,   475,   570,
     571,   476,   537,   538,   106,  -653,  -352,   127,    36,    37,
     107,   715,   559,   128,   560,   561,   562,   563,   564,   565,
     566,   567,   568,   569,   196,  1092,   273,   897,  1226,  1227,
     653,  1093,   898,   570,   571,   196,   123,   196,   273,   130,
     196,   955,   656,   196,   853,   196,   196,   132,   196,   273,
     963,  1045,   140,   657,   273,   863,  1123,    91,   660,    93,
     867,   273,   273,   872,   970,   662,   664,   273,   972,  1189,
     273,  1012,   273,   143,  1016,   196,  1177,   273,    91,    92,
      93,  1204,   145,   146,   707,   708,   121,   122,   537,   538,
      75,   539,   540,   541,   542,   543,   145,   146,   544,   545,
     546,   547,   202,   548,   549,   550,   551,   203,   704,   635,
     705,   552,   205,   553,   554,    91,   246,   247,   248,   935,
     898,   240,   902,   246,   247,   244,   251,   252,   274,   255,
    1044,   262,   253,   195,   537,   538,   275,   254,   256,   257,
     258,   259,   260,   261,   195,   273,   195,   329,   196,   195,
     330,   343,   195,   331,   195,   195,   357,   195,   345,   333,
     427,   563,   564,   565,   566,   567,   568,   569,   952,   442,
     447,   449,   450,   448,   451,   454,   263,   570,   571,   541,
     542,   263,   452,   453,   195,   472,   263,   547,   455,   548,
     549,   550,   551,  1003,   461,   464,   473,   552,   467,   469,
     483,   500,   264,   265,   501,   502,   503,   264,   265,   504,
     506,   505,   264,   265,   507,   508,   511,   524,   512,  1076,
     513,   579,   514,   539,   540,   541,   542,   543,    12,   533,
     544,   545,   546,   547,   574,   548,   549,   550,   551,   515,
     516,   573,   580,   552,   644,   553,   554,   646,   647,   654,
     672,   567,   568,   569,   721,   737,   775,   195,   774,   793,
     795,   266,   807,   570,   571,   267,   266,   794,   808,   809,
     267,   266,   811,   812,  1131,   267,   813,   814,   815,   509,
     510,   196,   816,   817,   846,   445,   858,   860,   862,   892,
     521,   561,   562,   563,   564,   565,   566,   567,   568,   569,
     526,   527,   528,   896,   530,   531,   532,   895,   908,   570,
     571,   914,   925,  1129,   927,   905,   910,   911,   268,    61,
     950,   926,   269,   268,   879,   270,   928,   269,   268,  1017,
     270,   944,   269,   951,  1018,   270,   929,  1147,   930,   940,
     271,   263,   819,   965,   961,   271,   964,   966,   969,   272,
     271,   263,  1002,   971,   272,  1005,  1014,  1021,  1028,   272,
    1030,   637,  1031,  1033,  1037,  1041,  1038,   264,   265,  1049,
     645,  1050,  1051,  1052,  1064,  1063,  1066,   264,   265,  1069,
    1073,  1098,  1107,  1112,  1115,  1118,  1142,  1127,  1124,  1169,
     195,  1156,  1172,  1186,  1164,  1165,  1151,  1195,  1191,  1217,
     675,  1197,  1182,   678,  1185,   680,  1183,  1219,  1218,   682,
     683,   684,   685,   686,   687,  1200,   693,  1230,   693,  1231,
     709,   116,    73,  1234,   156,   196,   266,   196,   119,   649,
     267,  1205,   352,  1208,   117,  1209,   266,   731,   732,   650,
     267,   733,   734,   735,   736,  1001,   738,  1133,   741,   742,
     743,   744,   745,   746,   747,   748,   749,   751,   752,   753,
     754,   755,   756,   757,   758,   759,   760,   761,  1078,   769,
    1210,  1211,  1212,    20,  1126,  1020,   537,   538,   772,  1153,
    1188,  1154,   651,   268,    82,  1235,   865,   269,   783,   782,
     270,   495,   788,   268,   848,   791,   792,   269,   638,  1019,
     270,   797,   904,   801,   484,   271,   196,   806,   498,  1034,
     702,   847,   810,  1196,   272,   271,     0,     0,     0,     0,
     499,     0,     0,     0,   272,   263,  1178,     0,     0,     0,
       0,     0,     0,     0,   195,     0,   195,   840,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     850,   264,   265,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   539,   540,   541,   542,   543,
       0,   871,   544,   545,   546,   547,     0,   548,   549,   550,
     551,     0,     0,     0,     0,   552,     0,   553,   554,   526,
     532,     0,     0,   555,   556,   557,     0,     0,   532,   558,
       0,   903,   521,   537,   538,     0,   907,     0,     0,     0,
     266,     0,     0,     0,   267,   195,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   263,     0,
     559,     0,   560,   561,   562,   563,   564,   565,   566,   567,
     568,   569,     0,     0,     0,     0,     0,     0,     0,   715,
       0,   570,   571,     0,   264,   265,     0,     0,     0,   333,
       0,   939,     0,     0,     0,   943,     0,   268,     0,     0,
       0,   269,   949,  1039,   270,     0,     0,   715,     0,   954,
       0,     0,     0,   958,     0,     0,     0,     0,   962,   271,
       0,     0,   539,   540,   541,   542,   543,   968,   272,   544,
     545,   546,   547,     0,   548,   549,   550,   551,     0,     0,
       0,     0,   552,   266,   553,   554,     0,   267,   263,   976,
     977,   978,   979,   980,   981,   982,   983,   984,   985,   989,
     990,   991,   992,   993,   994,   995,   999,     0,     0,     0,
       0,     0,     0,     0,   264,   265,  1007,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   560,
     561,   562,   563,   564,   565,   566,   567,   568,   569,     0,
     268,     0,     0,     0,   269,     0,  1122,   270,   570,   571,
       0,     0,     0,     0,     0,     0,  1023,  1024,     0,  1027,
       0,     0,   271,     0,  1032,     0,   693,     0,     0,     0,
       0,   272,     0,   266,     0,     0,     0,   267,  1042,  1043,
       0,     0,     0,     0,  1048,     0,     0,     0,     0,     0,
     -64,     0,     0,     0,  1057,     0,  1059,     0,  1062,     0,
       0,   537,   538,     0,  1065,     0,     0,     0,  1068,     0,
       0,     0,     0,     0,  1048,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   989,   999,     0,
     268,     0,     0,     0,   269,     0,  1176,   270,     0,     0,
       0,  1084,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   271,     0,     0,     0,     0,     0,     0,     0,
       0,   272,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   821,   822,   823,   824,   825,   826,   827,   828,
     539,   540,   541,   542,   543,   829,   830,   544,   545,   546,
     547,   831,   548,   549,   550,   551,     0,     0,   537,   538,
     552,   832,   553,   554,   833,   834,     0,     0,   555,   556,
     557,   835,   836,   837,   558,     0,     0,     0,     0,     0,
       0,     0,  1138,     0,  1139,     0,     0,  1141,     0,     0,
       0,     0,  1143,     0,     0,  1145,   637,     0,     0,     0,
    1148,     0,     0,     0,   838,   559,     0,   560,   561,   562,
     563,   564,   565,   566,   567,   568,   569,     0,     0,  1155,
       0,     0,  1157,  1158,  1159,  1161,   570,   571,  1163,     0,
       0,  1166,  1167,     0,  1168,     0,     0,  1170,     0,     0,
       0,     0,     0,     0,     0,     0,  1175,   539,   540,   541,
     542,   543,     0,     0,   544,   545,   546,   547,     0,   548,
     549,   550,   551,     0,     0,     0,     0,   552,     0,   553,
     554,     0,     0,     0,     0,   555,   556,   557,     0,     0,
       0,   558,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1201,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1213,     0,     0,     0,     0,     0,     0,  1216,
       0,     0,   559,     0,   560,   561,   562,   563,   564,   565,
     566,   567,   568,   569,  1223,  1225,     0,     0,     0,     0,
       0,     0,     0,   570,   571,  1229,   589,   572,     0,     0,
    1232,     3,  1233,   590,   591,   592,     0,   593,     0,   367,
     368,   369,   370,   371,     0,     0,     0,     0,     0,     0,
     594,   372,   595,   596,     0,     0,   537,   538,     0,     0,
       0,     0,   597,   373,     0,   598,     0,   599,   374,     0,
       0,   375,     0,     8,   376,   600,     0,   601,   377,     0,
       0,   602,   603,     0,     0,     0,     0,     0,   604,     0,
     161,   162,   163,     0,   165,   166,   167,   168,   169,   379,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,     0,   183,   184,   185,     0,   263,   188,   189,   190,
     191,     0,     0,   380,   605,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   381,   382,     0,     0,
       0,     0,   264,   265,     0,   539,   540,   541,   542,     0,
       0,   606,   607,   608,     0,   547,     0,   548,   549,   550,
     551,    53,     0,     0,     0,   552,     0,   553,   554,   383,
     384,   385,   386,   387,     0,   388,     0,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,    54,   609,
     401,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   266,     0,     0,     0,   267,   402,   403,   404,     0,
     405,     0,     0,   406,   407,     0,     0,   565,   566,   567,
     568,   569,     0,   408,     0,   409,   410,   610,   333,   611,
     589,   570,   571,     0,     0,     3,     0,   590,   591,   592,
       0,   593,     0,   367,   368,   369,   370,   371,     0,     0,
       0,     0,     0,     0,   594,   372,   595,   596,   268,     0,
       0,     0,   269,     0,     0,   270,   597,   373,     0,   598,
       0,   599,   374,     0,     0,   375,     0,     8,   376,   600,
     271,   601,   377,     0,     0,   602,   603,     0,     0,   272,
       0,     0,   604,     0,   161,   162,   163,     0,   165,   166,
     167,   168,   169,   379,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,     0,   183,   184,   185,     0,
       0,   188,   189,   190,   191,     0,     0,   380,   605,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     381,   382,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   606,   607,   608,     0,     0,
       0,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,   383,   384,   385,   386,   387,     0,   388,
       0,   389,   390,   391,   392,   393,   394,   395,   396,   397,
     398,   399,    54,   609,   401,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     402,   403,   404,     0,   405,     0,     0,   406,   407,     0,
       0,     0,   367,   368,   369,   370,   371,   408,     0,   409,
     410,   610,   333,  1146,   372,     0,     0,     0,     0,     0,
     263,     0,     0,     0,     0,     0,   373,     0,     0,     0,
       0,   374,     0,     0,   375,     0,     0,   376,     0,   689,
       0,   377,     0,     0,     0,     0,   264,   265,     0,     0,
       0,   378,     0,   161,   162,   163,     0,   165,   166,   167,
     168,   169,   379,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,     0,   183,   184,   185,     0,     0,
     188,   189,   190,   191,     0,     0,   380,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   381,
     382,     0,     0,     0,     0,   266,     0,     0,     0,   267,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,   383,   384,   385,   386,   387,     0,   388,     0,
     389,   390,   391,   392,   393,   394,   395,   396,   397,   398,
     399,   690,   400,   401,     0,     0,     0,     0,     0,     0,
       0,     0,   268,     0,     0,     0,   269,     0,     0,   691,
     403,   404,     0,   405,     0,     0,   406,   407,   367,   368,
     369,   370,   371,     0,   271,     0,   692,     0,   409,   410,
     372,   333,     0,   272,     0,     0,   263,     0,     0,     0,
       0,     0,   373,     0,     0,     0,     0,   374,     0,     0,
     375,     0,     0,   376,     0,     0,     0,   377,     0,     0,
       0,     0,   264,   265,     0,     0,     0,   378,     0,   161,
     162,   163,     0,   165,   166,   167,   168,   169,   379,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
       0,   183,   184,   185,     0,     0,   188,   189,   190,   191,
       0,     0,   380,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   381,   382,     0,     0,     0,
       0,   266,     0,     0,     0,   267,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,   383,   384,
     385,   386,   387,     0,   388,     0,   389,   390,   391,   392,
     393,   394,   395,   396,   397,   398,   399,   690,   400,   401,
       0,     0,     0,     0,     0,     0,     0,     0,   268,     0,
       0,     0,   269,     0,     0,   691,   403,   404,     0,   405,
       0,     0,   406,   407,   367,   368,   369,   370,   371,     0,
     271,     0,   700,     0,   409,   410,   372,   333,     0,   272,
       0,     0,   437,     0,     0,     0,     0,     0,   373,     0,
       0,     0,     0,   374,     0,     0,   375,     0,     0,   376,
       0,     0,     0,   377,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   378,     0,   161,   162,   163,     0,   165,
     166,   167,   168,   169,   379,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,     0,   183,   184,   185,
       0,     0,   188,   189,   190,   191,     0,     0,   380,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   381,   382,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,     0,     0,     0,
       0,     0,     0,     0,   383,   384,   385,   386,   387,     0,
     388,     0,   389,   390,   391,   392,   393,   394,   395,   396,
     397,   398,   399,    54,   400,   401,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   438,     0,
       0,   402,   403,   404,     0,   405,     0,     0,   406,   407,
     367,   368,   369,   370,   371,     0,   439,     0,   408,     0,
     409,   410,   372,   333,     0,   440,     0,     0,     0,     0,
       0,     0,     0,     0,   373,     0,     0,     0,     0,   374,
       0,     0,   375,     0,     0,   376,     0,     0,     0,   377,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   378,
       0,   161,   162,   163,     0,   165,   166,   167,   168,   169,
     379,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,     0,   183,   184,   185,     0,     0,   188,   189,
     190,   191,     0,     0,   380,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   381,   382,     0,
       0,     0,     0,     0,     0,     0,   785,     0,     0,     0,
       0,     0,   606,   607,   608,     0,     0,     0,     0,     0,
       0,     0,    53,     0,     0,     0,     0,     0,     0,     0,
     383,   384,   385,   386,   387,     0,   388,     0,   389,   390,
     391,   392,   393,   394,   395,   396,   397,   398,   399,    54,
     400,   401,     0,   367,   368,   369,   370,   371,     0,     0,
       0,     0,     0,     0,     0,   372,     0,   402,   403,   404,
       0,   405,     0,     0,   406,   407,     0,   373,     0,     0,
       0,     0,   374,     0,   408,   375,   409,   410,   376,   333,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,     0,   161,   162,   163,     0,   165,   166,
     167,   168,   169,   379,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,     0,   183,   184,   185,     0,
       0,   188,   189,   190,   191,     0,     0,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     381,   382,     0,     0,     0,     0,     0,     0,     0,   799,
       0,     0,     0,     0,     0,   606,   607,   608,     0,     0,
       0,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,   383,   384,   385,   386,   387,     0,   388,
       0,   389,   390,   391,   392,   393,   394,   395,   396,   397,
     398,   399,    54,   400,   401,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     402,   403,   404,     0,   405,     0,     0,   406,   407,     0,
       0,     0,   367,   368,   369,   370,   371,   408,     0,   409,
     410,     0,   333,  1206,   372,   595,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   373,     0,     0,     0,
       0,   374,     0,     0,   375,     0,     0,   376,   600,     0,
       0,   377,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,   161,   162,   163,     0,   165,   166,   167,
     168,   169,   379,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,     0,   183,   184,   185,     0,     0,
     188,   189,   190,   191,     0,     0,   380,  1207,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   381,
     382,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,   383,   384,   385,   386,   387,     0,   388,     0,
     389,   390,   391,   392,   393,   394,   395,   396,   397,   398,
     399,    54,   400,   401,     0,   367,   368,   369,   370,   371,
       0,     0,     0,     0,     0,     0,     0,   372,     0,   402,
     403,   404,     0,   405,     0,     0,   406,   407,     0,   373,
       0,     0,     0,     0,   374,     0,   408,   375,   409,   410,
     376,   333,     0,     0,   377,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   378,     0,   161,   162,   163,     0,
     165,   166,   167,   168,   169,   379,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,     0,   183,   184,
     185,     0,     0,   188,   189,   190,   191,     0,     0,   380,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   381,   382,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   606,   607,   608,
       0,     0,     0,     0,     0,     0,     0,    53,     0,     0,
       0,     0,     0,     0,     0,   383,   384,   385,   386,   387,
       0,   388,     0,   389,   390,   391,   392,   393,   394,   395,
     396,   397,   398,   399,    54,   400,   401,     0,   367,   368,
     369,   370,   371,     0,     0,     0,     0,     0,     0,     0,
     372,     0,   402,   403,   404,     0,   405,     0,     0,   406,
     407,     0,   373,     0,     0,     0,     0,   374,     0,   408,
     375,   409,   410,   376,   333,     0,     0,   377,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,     0,   161,
     162,   163,     0,   165,   166,   167,   168,   169,   379,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
       0,   183,   184,   185,     0,     0,   188,   189,   190,   191,
       0,     0,   380,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   381,   382,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     986,   987,   988,     0,     0,     0,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,   383,   384,
     385,   386,   387,     0,   388,     0,   389,   390,   391,   392,
     393,   394,   395,   396,   397,   398,   399,    54,   400,   401,
       0,   367,   368,   369,   370,   371,     0,     0,     0,     0,
       0,     0,     0,   372,     0,   402,   403,   404,     0,   405,
       0,     0,   406,   407,     0,   373,     0,     0,     0,     0,
     374,     0,   408,   375,   409,   410,   376,   333,     0,     0,
     377,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     378,     0,   161,   162,   163,     0,   165,   166,   167,   168,
     169,   379,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,     0,   183,   184,   185,     0,     0,   188,
     189,   190,   191,     0,     0,   380,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   381,   382,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   996,   997,   998,     0,     0,     0,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
       0,   383,   384,   385,   386,   387,     0,   388,     0,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
      54,   400,   401,     0,   367,   368,   369,   370,   371,     0,
       0,     0,     0,     0,     0,     0,   372,     0,   402,   403,
     404,     0,   405,     0,     0,   406,   407,     0,   373,     0,
       0,     0,     0,   374,     0,   408,   375,   409,   410,   376,
     333,     0,     0,   377,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   378,     0,   161,   162,   163,     0,   165,
     166,   167,   168,   169,   379,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,     0,   183,   184,   185,
       0,     0,   188,   189,   190,   191,     0,     0,   380,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   381,   382,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,     0,     0,     0,
       0,     0,     0,     0,   383,   384,   385,   386,   387,     0,
     388,     0,   389,   390,   391,   392,   393,   394,   395,   396,
     397,   398,   399,    54,   400,   401,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   402,   403,   404,     0,   405,     0,     0,   406,   407,
     367,   368,   369,   370,   371,     0,   717,     0,   408,   718,
     409,   410,   372,   333,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   373,     0,     0,     0,     0,   374,
       0,     0,   375,     0,     0,   376,     0,     0,     0,   377,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   378,
       0,   161,   162,   163,     0,   165,   166,   167,   168,   169,
     379,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,     0,   183,   184,   185,     0,     0,   188,   189,
     190,   191,     0,     0,   380,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   381,   382,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,     0,     0,     0,     0,     0,     0,     0,
     383,   384,   385,   386,   387,     0,   388,     0,   389,   390,
     391,   392,   393,   394,   395,   396,   397,   398,   399,    54,
     400,   401,     0,   367,   368,   369,   370,   371,     0,     0,
     750,     0,     0,     0,     0,   372,     0,   402,   403,   404,
       0,   405,     0,     0,   406,   407,     0,   373,     0,     0,
       0,     0,   374,   441,   408,   375,   409,   410,   376,   333,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,     0,   161,   162,   163,     0,   165,   166,
     167,   168,   169,   379,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,     0,   183,   184,   185,     0,
       0,   188,   189,   190,   191,     0,     0,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     381,   382,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,   383,   384,   385,   386,   387,     0,   388,
       0,   389,   390,   391,   392,   393,   394,   395,   396,   397,
     398,   399,    54,   400,   401,     0,   367,   368,   369,   370,
     371,     0,     0,     0,     0,     0,     0,     0,   372,     0,
     402,   403,   404,     0,   405,     0,     0,   406,   407,     0,
     373,     0,     0,     0,     0,   374,     0,   408,   375,   409,
     410,   376,   333,     0,     0,   377,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   378,     0,   161,   162,   163,
       0,   165,   166,   167,   168,   169,   379,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,     0,   183,
     184,   185,     0,     0,   188,   189,   190,   191,     0,     0,
     380,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   381,   382,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    53,     0,
       0,     0,     0,     0,     0,     0,   383,   384,   385,   386,
     387,     0,   388,     0,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,    54,   400,   401,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   402,   403,   404,     0,   405,     0,     0,
     406,   407,   367,   368,   369,   370,   371,     0,     0,     0,
     408,   770,   409,   410,   372,   333,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   373,     0,     0,     0,
       0,   374,     0,     0,   375,     0,     0,   376,     0,     0,
       0,   377,     0,     0,     0,     0,     0,   790,     0,     0,
       0,   378,     0,   161,   162,   163,     0,   165,   166,   167,
     168,   169,   379,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,     0,   183,   184,   185,     0,     0,
     188,   189,   190,   191,     0,     0,   380,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   381,
     382,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,   383,   384,   385,   386,   387,     0,   388,     0,
     389,   390,   391,   392,   393,   394,   395,   396,   397,   398,
     399,    54,   400,   401,     0,   367,   368,   369,   370,   371,
       0,     0,     0,     0,     0,     0,     0,   372,     0,   402,
     403,   404,     0,   405,     0,     0,   406,   407,     0,   373,
       0,     0,     0,     0,   374,     0,   408,   375,   409,   410,
     376,   333,     0,     0,   377,     0,     0,   796,     0,     0,
       0,     0,     0,     0,   378,     0,   161,   162,   163,     0,
     165,   166,   167,   168,   169,   379,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,     0,   183,   184,
     185,     0,     0,   188,   189,   190,   191,     0,     0,   380,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   381,   382,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    53,     0,     0,
       0,     0,     0,     0,     0,   383,   384,   385,   386,   387,
       0,   388,     0,   389,   390,   391,   392,   393,   394,   395,
     396,   397,   398,   399,    54,   400,   401,     0,   367,   368,
     369,   370,   371,     0,     0,     0,     0,     0,     0,     0,
     372,     0,   402,   403,   404,     0,   405,     0,     0,   406,
     407,     0,   373,     0,     0,     0,     0,   374,     0,   408,
     375,   409,   410,   376,   333,     0,     0,   377,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,     0,   161,
     162,   163,     0,   165,   166,   167,   168,   169,   379,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
       0,   183,   184,   185,     0,     0,   188,   189,   190,   191,
       0,     0,   380,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   381,   382,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,   383,   384,
     385,   386,   387,     0,   388,     0,   389,   390,   391,   392,
     393,   394,   395,   396,   397,   398,   399,    54,   400,   401,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   524,     0,   402,   403,   404,     0,   405,
       0,     0,   406,   407,   367,   368,   369,   370,   371,     0,
       0,     0,   408,     0,   409,   410,   372,   333,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   373,     0,
       0,     0,     0,   374,     0,     0,   375,     0,     0,   376,
       0,     0,     0,   377,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   378,     0,   161,   162,   163,     0,   165,
     166,   167,   168,   169,   379,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,     0,   183,   184,   185,
       0,     0,   188,   189,   190,   191,     0,     0,   380,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   381,   382,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,     0,     0,     0,
       0,     0,     0,     0,   383,   384,   385,   386,   387,     0,
     388,     0,   389,   390,   391,   392,   393,   394,   395,   396,
     397,   398,   399,    54,   400,   401,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   402,   403,   404,     0,   405,     0,     0,   406,   407,
     367,   368,   369,   370,   371,     0,     0,     0,   408,   868,
     409,   410,   372,   333,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   373,     0,     0,     0,     0,   374,
       0,     0,   375,     0,     0,   376,     0,     0,     0,   377,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   378,
       0,   161,   162,   163,     0,   165,   166,   167,   168,   169,
     379,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,     0,   183,   184,   185,     0,     0,   188,   189,
     190,   191,     0,     0,   380,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   381,   382,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,     0,     0,     0,     0,     0,     0,     0,
     383,   384,   385,   386,   387,     0,   388,     0,   389,   390,
     391,   392,   393,   394,   395,   396,   397,   398,   399,    54,
     400,   401,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   870,     0,   402,   403,   404,
       0,   405,     0,     0,   406,   407,   367,   368,   369,   370,
     371,     0,     0,     0,   408,     0,   409,   410,   372,   333,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     373,     0,     0,     0,     0,   374,     0,     0,   375,     0,
       0,   376,     0,     0,     0,   377,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   378,     0,   161,   162,   163,
       0,   165,   166,   167,   168,   169,   379,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,     0,   183,
     184,   185,     0,     0,   188,   189,   190,   191,     0,     0,
     380,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   381,   382,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    53,     0,
       0,     0,     0,     0,     0,     0,   383,   384,   385,   386,
     387,     0,   388,     0,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,    54,   400,   401,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   402,   403,   404,     0,   405,     0,     0,
     406,   407,   367,   368,   369,   370,   371,     0,     0,     0,
     408,   890,   409,   410,   372,   333,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   373,     0,     0,     0,
       0,   374,     0,     0,   375,     0,     0,   376,     0,     0,
       0,   377,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,   161,   162,   163,     0,   165,   166,   167,
     168,   169,   379,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,     0,   183,   184,   185,     0,     0,
     188,   189,   190,   191,     0,     0,   380,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   381,
     382,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,   383,   384,   385,   386,   387,     0,   388,     0,
     389,   390,   391,   392,   393,   394,   395,   396,   397,   398,
     399,    54,   400,   401,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   402,
     403,   404,     0,   405,     0,     0,   406,   407,   367,   368,
     369,   370,   371,     0,     0,     0,   408,   900,   409,   410,
     372,   333,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   373,     0,     0,     0,     0,   374,     0,     0,
     375,     0,     0,   376,     0,     0,     0,   377,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,     0,   161,
     162,   163,     0,   165,   166,   167,   168,   169,   379,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
       0,   183,   184,   185,     0,     0,   188,   189,   190,   191,
       0,     0,   380,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   381,   382,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,   383,   384,
     385,   386,   387,     0,   388,     0,   389,   390,   391,   392,
     393,   394,   395,   396,   397,   398,   399,    54,   400,   401,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   367,
     368,   369,   370,   371,     0,   402,   403,   404,     0,   405,
       0,   372,   406,   407,     0,     0,     0,     0,     0,     0,
    1047,     0,   408,   373,   409,   410,     0,   333,   374,     0,
       0,   375,     0,     0,   376,     0,     0,     0,   377,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   378,     0,
     161,   162,   163,     0,   165,   166,   167,   168,   169,   379,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,     0,   183,   184,   185,     0,     0,   188,   189,   190,
     191,     0,     0,   380,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   381,   382,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,     0,     0,     0,     0,     0,     0,     0,   383,
     384,   385,   386,   387,     0,   388,     0,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,    54,   400,
     401,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   402,   403,   404,     0,
     405,     0,     0,   406,   407,   367,   368,   369,   370,   371,
       0,     0,     0,   408,  1060,   409,   410,   372,   333,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   373,
       0,     0,     0,     0,   374,     0,     0,   375,     0,     0,
     376,     0,     0,     0,   377,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   378,     0,   161,   162,   163,     0,
     165,   166,   167,   168,   169,   379,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,     0,   183,   184,
     185,     0,     0,   188,   189,   190,   191,     0,     0,   380,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   381,   382,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    53,     0,     0,
       0,     0,     0,     0,     0,   383,   384,   385,   386,   387,
       0,   388,     0,   389,   390,   391,   392,   393,   394,   395,
     396,   397,   398,   399,    54,   400,   401,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   402,   403,   404,     0,   405,     0,     0,   406,
     407,   367,   368,   369,   370,   371,     0,     0,     0,   408,
    1108,   409,   410,   372,   333,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   373,     0,     0,     0,     0,
     374,     0,     0,   375,     0,     0,   376,     0,     0,     0,
     377,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     378,     0,   161,   162,   163,     0,   165,   166,   167,   168,
     169,   379,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,     0,   183,   184,   185,     0,     0,   188,
     189,   190,   191,     0,     0,   380,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   381,   382,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
       0,   383,   384,   385,   386,   387,     0,   388,     0,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
      54,   400,   401,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   402,   403,
     404,     0,   405,     0,     0,   406,   407,   367,   368,   369,
     370,   371,     0,     0,     0,   408,  1160,   409,   410,   372,
     333,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   373,     0,     0,     0,     0,   374,     0,     0,   375,
       0,     0,   376,     0,     0,     0,   377,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   378,     0,   161,   162,
     163,     0,   165,   166,   167,   168,   169,   379,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,     0,
     183,   184,   185,     0,     0,   188,   189,   190,   191,     0,
       0,   380,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   381,   382,     0,     0,     0,     0,
       0,     0,     0,  1174,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,   383,   384,   385,
     386,   387,     0,   388,     0,   389,   390,   391,   392,   393,
     394,   395,   396,   397,   398,   399,    54,   400,   401,     0,
     367,   368,   369,   370,   371,     0,     0,     0,     0,     0,
       0,     0,   372,     0,   402,   403,   404,     0,   405,     0,
       0,   406,   407,     0,   373,     0,     0,     0,     0,   374,
       0,   408,   375,   409,   410,   376,   333,     0,     0,   377,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   378,
       0,   161,   162,   163,     0,   165,   166,   167,   168,   169,
     379,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,     0,   183,   184,   185,     0,     0,   188,   189,
     190,   191,     0,     0,   380,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   381,   382,     0,
       0,     0,     0,     0,     0,     0,  1222,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,     0,     0,     0,     0,     0,     0,     0,
     383,   384,   385,   386,   387,     0,   388,     0,   389,   390,
     391,   392,   393,   394,   395,   396,   397,   398,   399,    54,
     400,   401,     0,   367,   368,   369,   370,   371,     0,     0,
       0,     0,     0,     0,     0,   372,     0,   402,   403,   404,
       0,   405,     0,     0,   406,   407,     0,   373,     0,     0,
       0,     0,   374,     0,   408,   375,   409,   410,   376,   333,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,     0,   161,   162,   163,     0,   165,   166,
     167,   168,   169,   379,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,     0,   183,   184,   185,     0,
       0,   188,   189,   190,   191,     0,     0,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     381,   382,     0,     0,     0,     0,     0,     0,     0,  1224,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,   383,   384,   385,   386,   387,     0,   388,
       0,   389,   390,   391,   392,   393,   394,   395,   396,   397,
     398,   399,    54,   400,   401,     0,   367,   368,   369,   370,
     371,     0,     0,     0,     0,     0,     0,     0,   372,     0,
     402,   403,   404,     0,   405,     0,     0,   406,   407,     0,
     373,     0,     0,     0,     0,   374,     0,   408,   375,   409,
     410,   376,   333,     0,     0,   377,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   378,     0,   161,   162,   163,
       0,   165,   166,   167,   168,   169,   379,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,     0,   183,
     184,   185,     0,     0,   188,   189,   190,   191,     0,     0,
     380,     0,     0,     0,     0,     0,     0,     0,  1149,     0,
       0,     0,     0,   381,   382,     0,     0,   537,   538,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    53,     0,
       0,     0,     0,     0,     0,     0,   383,   384,   385,   386,
     387,     0,   388,     0,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,    54,   400,   401,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   402,   403,   404,     0,   405,     0,     0,
     406,   407,     0,     0,     0,     0,     0,     0,     0,     0,
     408,     0,   409,   410,     0,   333,   539,   540,   541,   542,
     543,     0,     0,   544,   545,   546,   547,     0,   548,   549,
     550,   551,   537,   538,     0,     0,   552,     0,   553,   554,
       0,     0,     0,     0,   555,   556,   557,     0,     0,     0,
     558,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   559,     0,   560,   561,   562,   563,   564,   565,   566,
     567,   568,   569,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   570,   571,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   821,   822,   823,   824,   825,   826,   827,
     828,   539,   540,   541,   542,   543,   829,   830,   544,   545,
     546,   547,   959,   548,   549,   550,   551,   537,   538,     0,
       0,   552,   832,   553,   554,   833,   834,     0,     0,   555,
     556,   557,   835,   836,   837,   558,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   960,   559,     0,   560,   561,
     562,   563,   564,   565,   566,   567,   568,   569,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   570,   571,     0,
       0,     0,     0,     0,  1008,     0,     0,     0,   821,   822,
     823,   824,   825,   826,   827,   828,   539,   540,   541,   542,
     543,   829,   830,   544,   545,   546,   547,   959,   548,   549,
     550,   551,   537,   538,     0,     0,   552,   832,   553,   554,
     833,   834,     0,     0,   555,   556,   557,   835,   836,   837,
     558,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     960,   559,     0,   560,   561,   562,   563,   564,   565,   566,
     567,   568,   569,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   570,   571,     0,     0,     0,     0,     0,  1099,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   539,   540,   541,   542,   543,   537,   538,   544,   545,
     546,   547,     0,   548,   549,   550,   551,     0,     0,     0,
       0,   552,     0,   553,   554,     0,     0,     0,     0,   555,
     556,   557,     0,     0,     0,   558,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,   560,   561,
     562,   563,   564,   565,   566,   567,   568,   569,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   570,   571,     0,
       0,     0,     0,     0,  1000,   539,   540,   541,   542,   543,
     537,   538,   544,   545,   546,   547,     0,   548,   549,   550,
     551,     0,     0,     0,     0,   552,     0,   553,   554,     0,
       0,     0,     0,   555,   556,   557,     0,     0,     0,   558,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     559,     0,   560,   561,   562,   563,   564,   565,   566,   567,
     568,   569,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   570,   571,     0,     0,     0,     0,     0,  1193,   539,
     540,   541,   542,   543,   537,   538,   544,   545,   546,   547,
       0,   548,   549,   550,   551,     0,     0,     0,     0,   552,
       0,   553,   554,     0,     0,     0,     0,   555,   556,   557,
       0,     0,     0,   558,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   559,     0,   560,   561,   562,   563,
     564,   565,   566,   567,   568,   569,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   570,   571,     0,     0,   711,
       0,     0,     0,   539,   540,   541,   542,   543,   537,   538,
     544,   545,   546,   547,     0,   548,   549,   550,   551,     0,
       0,     0,     0,   552,     0,   553,   554,     0,     0,     0,
       0,   555,   556,   557,     0,     0,     0,   558,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   559,     0,
     560,   561,   562,   563,   564,   565,   566,   567,   568,   569,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   570,
     571,     0,     0,   845,     0,     0,     0,   539,   540,   541,
     542,   543,   537,   538,   544,   545,   546,   547,     0,   548,
     549,   550,   551,     0,     0,     0,     0,   552,     0,   553,
     554,     0,     0,     0,     0,   555,   556,   557,     0,     0,
       0,   558,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   559,     0,   560,   561,   562,   563,   564,   565,
     566,   567,   568,   569,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   570,   571,     0,     0,   873,     0,     0,
       0,   539,   540,   541,   542,   543,   537,   538,   544,   545,
     546,   547,     0,   548,   549,   550,   551,     0,     0,     0,
       0,   552,     0,   553,   554,     0,     0,     0,     0,   555,
     556,   557,     0,     0,     0,   558,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,   560,   561,
     562,   563,   564,   565,   566,   567,   568,   569,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   570,   571,     0,
       0,   876,     0,     0,     0,   539,   540,   541,   542,   543,
     537,   538,   544,   545,   546,   547,     0,   548,   549,   550,
     551,     0,     0,     0,     0,   552,     0,   553,   554,     0,
       0,     0,     0,   555,   556,   557,     0,     0,     0,   558,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     559,     0,   560,   561,   562,   563,   564,   565,   566,   567,
     568,   569,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   570,   571,     0,     0,   878,     0,     0,     0,   539,
     540,   541,   542,   543,   537,   538,   544,   545,   546,   547,
       0,   548,   549,   550,   551,     0,     0,     0,     0,   552,
       0,   553,   554,     0,     0,     0,     0,   555,   556,   557,
       0,     0,     0,   558,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   559,     0,   560,   561,   562,   563,
     564,   565,   566,   567,   568,   569,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   570,   571,     0,     0,   880,
       0,     0,     0,   539,   540,   541,   542,   543,   537,   538,
     544,   545,   546,   547,     0,   548,   549,   550,   551,     0,
       0,     0,     0,   552,     0,   553,   554,     0,     0,     0,
       0,   555,   556,   557,     0,     0,     0,   558,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   559,     0,
     560,   561,   562,   563,   564,   565,   566,   567,   568,   569,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   570,
     571,     0,     0,   881,     0,     0,     0,   539,   540,   541,
     542,   543,   537,   538,   544,   545,   546,   547,     0,   548,
     549,   550,   551,     0,     0,     0,     0,   552,     0,   553,
     554,     0,     0,     0,     0,   555,   556,   557,     0,     0,
       0,   558,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   559,     0,   560,   561,   562,   563,   564,   565,
     566,   567,   568,   569,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   570,   571,     0,     0,   882,     0,     0,
       0,   539,   540,   541,   542,   543,   537,   538,   544,   545,
     546,   547,     0,   548,   549,   550,   551,     0,     0,     0,
       0,   552,     0,   553,   554,     0,     0,     0,     0,   555,
     556,   557,     0,     0,     0,   558,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,   560,   561,
     562,   563,   564,   565,   566,   567,   568,   569,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   570,   571,     0,
       0,   883,     0,     0,     0,   539,   540,   541,   542,   543,
     537,   538,   544,   545,   546,   547,     0,   548,   549,   550,
     551,     0,     0,     0,     0,   552,     0,   553,   554,     0,
       0,     0,     0,   555,   556,   557,     0,     0,     0,   558,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     559,     0,   560,   561,   562,   563,   564,   565,   566,   567,
     568,   569,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   570,   571,     0,     0,   884,     0,     0,     0,   539,
     540,   541,   542,   543,   537,   538,   544,   545,   546,   547,
       0,   548,   549,   550,   551,     0,     0,     0,     0,   552,
       0,   553,   554,     0,     0,     0,     0,   555,   556,   557,
       0,     0,     0,   558,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   559,     0,   560,   561,   562,   563,
     564,   565,   566,   567,   568,   569,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   570,   571,     0,     0,   885,
       0,     0,     0,   539,   540,   541,   542,   543,   537,   538,
     544,   545,   546,   547,     0,   548,   549,   550,   551,     0,
       0,     0,     0,   552,     0,   553,   554,     0,     0,     0,
       0,   555,   556,   557,     0,     0,     0,   558,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   559,     0,
     560,   561,   562,   563,   564,   565,   566,   567,   568,   569,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   570,
     571,     0,     0,   948,     0,     0,     0,   539,   540,   541,
     542,   543,   537,   538,   544,   545,   546,   547,     0,   548,
     549,   550,   551,     0,     0,     0,     0,   552,     0,   553,
     554,     0,     0,     0,     0,   555,   556,   557,     0,     0,
       0,   558,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   559,     0,   560,   561,   562,   563,   564,   565,
     566,   567,   568,   569,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   570,   571,     0,     0,  1015,     0,     0,
       0,   539,   540,   541,   542,   543,   537,   538,   544,   545,
     546,   547,     0,   548,   549,   550,   551,     0,     0,     0,
       0,   552,     0,   553,   554,     0,     0,     0,     0,   555,
     556,   557,     0,     0,     0,   558,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,   560,   561,
     562,   563,   564,   565,   566,   567,   568,   569,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   570,   571,     0,
       0,  1072,     0,     0,     0,   539,   540,   541,   542,   543,
     537,   538,   544,   545,   546,   547,     0,   548,   549,   550,
     551,     0,     0,     0,     0,   552,     0,   553,   554,     0,
       0,     0,     0,   555,   556,   557,     0,     0,     0,   558,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     559,     0,   560,   561,   562,   563,   564,   565,   566,   567,
     568,   569,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   570,   571,     0,     0,  1120,     0,     0,     0,   539,
     540,   541,   542,   543,   537,   538,   544,   545,   546,   547,
       0,   548,   549,   550,   551,     0,     0,     0,     0,   552,
       0,   553,   554,     0,     0,     0,     0,   555,   556,   557,
       0,     0,     0,   558,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   559,     0,   560,   561,   562,   563,
     564,   565,   566,   567,   568,   569,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   570,   571,     0,     0,  1121,
       0,     0,     0,   539,   540,   541,   542,   543,   537,   538,
     544,   545,   546,   547,     0,   548,   549,   550,   551,     0,
       0,     0,     0,   552,     0,   553,   554,     0,     0,     0,
       0,   555,   556,   557,     0,     0,     0,   558,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   559,     0,
     560,   561,   562,   563,   564,   565,   566,   567,   568,   569,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   570,
     571,     0,     0,  1132,     0,     0,     0,   539,   540,   541,
     542,   543,   537,   538,   544,   545,   546,   547,     0,   548,
     549,   550,   551,     0,     0,     0,     0,   552,     0,   553,
     554,     0,     0,     0,     0,   555,   556,   557,     0,     0,
       0,   558,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   559,     0,   560,   561,   562,   563,   564,   565,
     566,   567,   568,   569,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   570,   571,     0,     0,  1134,     0,     0,
       0,   539,   540,   541,   542,   543,   537,   538,   544,   545,
     546,   547,     0,   548,   549,   550,   551,     0,     0,     0,
       0,   552,     0,   553,   554,     0,     0,     0,     0,   555,
     556,   557,     0,     0,     0,   558,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,   560,   561,
     562,   563,   564,   565,   566,   567,   568,   569,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   570,   571,     0,
       0,  1136,     0,     0,     0,   539,   540,   541,   542,   543,
     537,   538,   544,   545,   546,   547,     0,   548,   549,   550,
     551,     0,     0,     0,     0,   552,     0,   553,   554,     0,
       0,     0,     0,   555,   556,   557,     0,     0,     0,   558,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     559,     0,   560,   561,   562,   563,   564,   565,   566,   567,
     568,   569,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   570,   571,     0,     0,  1140,     0,     0,     0,   539,
     540,   541,   542,   543,   537,   538,   544,   545,   546,   547,
       0,   548,   549,   550,   551,     0,     0,     0,     0,   552,
       0,   553,   554,     0,     0,     0,     0,   555,   556,   557,
       0,     0,     0,   558,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   559,     0,   560,   561,   562,   563,
     564,   565,   566,   567,   568,   569,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   570,   571,     0,     0,  1179,
       0,     0,     0,   539,   540,   541,   542,   543,   537,   538,
     544,   545,   546,   547,     0,   548,   549,   550,   551,     0,
       0,     0,     0,   552,     0,   553,   554,     0,     0,     0,
       0,   555,   556,   557,     0,     0,     0,   558,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   559,     0,
     560,   561,   562,   563,   564,   565,   566,   567,   568,   569,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   570,
     571,     0,     0,  1180,     0,     0,     0,   539,   540,   541,
     542,   543,   537,   538,   544,   545,   546,   547,     0,   548,
     549,   550,   551,     0,     0,     0,     0,   552,     0,   553,
     554,     0,     0,     0,     0,   555,   556,   557,     0,     0,
       0,   558,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   559,     0,   560,   561,   562,   563,   564,   565,
     566,   567,   568,   569,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   570,   571,     0,     0,  1181,     0,     0,
       0,   539,   540,   541,   542,   543,   537,   538,   544,   545,
     546,   547,     0,   548,   549,   550,   551,     0,     0,     0,
       0,   552,     0,   553,   554,     0,     0,     0,     0,   555,
     556,   557,     0,     0,     0,   558,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,   560,   561,
     562,   563,   564,   565,   566,   567,   568,   569,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   570,   571,     0,
       0,  1190,     0,     0,     0,   539,   540,   541,   542,   543,
     537,   538,   544,   545,   546,   547,     0,   548,   549,   550,
     551,     0,     0,     0,     0,   552,     0,   553,   554,     0,
       0,     0,     0,   555,   556,   557,     0,     0,     0,   558,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     559,     0,   560,   561,   562,   563,   564,   565,   566,   567,
     568,   569,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   570,   571,     0,     0,  1192,     0,     0,     0,   539,
     540,   541,   542,   543,   537,   538,   544,   545,   546,   547,
       0,   548,   549,   550,   551,     0,     0,     0,     0,   552,
       0,   553,   554,     0,     0,     0,     0,   555,   556,   557,
       0,     0,     0,   558,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   559,     0,   560,   561,   562,   563,
     564,   565,   566,   567,   568,   569,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   570,   571,     0,     0,  1228,
       0,     0,     0,   539,   540,   541,   542,   543,   537,   538,
     544,   545,   546,   547,     0,   548,   549,   550,   551,     0,
       0,     0,     0,   552,     0,   553,   554,     0,     0,     0,
       0,   555,   556,   557,     0,     0,     0,   558,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   559,     0,
     560,   561,   562,   563,   564,   565,   566,   567,   568,   569,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   570,
     571,   575,     0,     0,     0,     0,     0,   539,   540,   541,
     542,   543,   537,   538,   544,   545,   546,   547,     0,   548,
     549,   550,   551,     0,     0,     0,     0,   552,     0,   553,
     554,     0,     0,     0,     0,   555,   556,   557,     0,     0,
       0,   558,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   559,     0,   560,   561,   562,   563,   564,   565,
     566,   567,   568,   569,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   570,   571,   931,     0,     0,     0,     0,
       0,   539,   540,   541,   542,   543,   537,   538,   544,   545,
     546,   547,     0,   548,   549,   550,   551,     0,     0,     0,
       0,   552,     0,   553,   554,     0,     0,     0,     0,   555,
     556,   557,     0,     0,     0,   558,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,   560,   561,
     562,   563,   564,   565,   566,   567,   568,   569,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   570,   571,   945,
       0,     0,     0,     0,     0,   539,   540,   541,   542,   543,
     537,   538,   544,   545,   546,   547,     0,   548,   549,   550,
     551,     0,     0,     0,     0,   552,     0,   553,   554,     0,
       0,     0,     0,   555,   556,   557,     0,     0,     0,   558,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     559,     0,   560,   561,   562,   563,   564,   565,   566,   567,
     568,   569,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   570,   571,  1067,     0,     0,     0,     0,     0,   539,
     540,   541,   542,   543,     0,     0,   544,   545,   546,   547,
       0,   548,   549,   550,   551,   537,   538,     0,     0,   552,
       0,   553,   554,     0,     0,     0,     0,   555,   556,   557,
       0,     0,     0,   558,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   559,     0,   560,   561,   562,   563,
     564,   565,   566,   567,   568,   569,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   570,   571,  1070,     0,     0,
       0,     0,     0,     0,     0,     0,   821,   822,   823,   824,
     825,   826,   827,   828,   539,   540,   541,   542,   543,   829,
     830,   544,   545,   546,   547,   959,   548,   549,   550,   551,
    -317,     0,   278,   279,   552,   832,   553,   554,   833,   834,
       0,     0,   555,   556,   557,   835,   836,   837,   558,   280,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   960,   559,
       0,   560,   561,   562,   563,   564,   565,   566,   567,   568,
     569,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     570,   571,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   294,   295,   296,   297,
     298,   537,   538,   299,   300,   301,     0,     0,     0,     0,
       0,     0,   302,   303,   304,   305,   306,     0,     0,   307,
     308,   309,   310,   311,   312,   313,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   314,     0,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,     0,     0,
     325,   326,     0,     0,   723,     0,     0,   327,   328,     0,
       0,     0,   537,   538,     0,     0,     0,     0,     0,     0,
     539,   540,   541,   542,   543,     0,     0,   544,   545,   546,
     547,     0,   548,   549,   550,   551,     0,     0,     0,     0,
     552,     0,   553,   554,     0,     0,   891,     0,   555,   556,
     557,   161,   162,   163,   558,   165,   166,   167,   168,   169,
     379,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,     0,   183,   184,   185,     0,     0,   188,   189,
     190,   191,     0,     0,     0,   559,     0,   560,   561,   562,
     563,   564,   565,   566,   567,   568,   569,     0,     0,     0,
       0,   539,   540,   541,   542,   543,   570,   571,   544,   545,
     546,   547,     0,   548,   549,   550,   551,   537,   538,     0,
       0,   552,     0,   553,   554,     0,     0,     0,     0,   555,
     556,   557,     0,     0,     0,   558,     0,   724,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   725,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,   936,   560,   561,
     562,   563,   564,   565,   566,   567,   568,   569,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   570,   571,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   537,   538,
       0,     0,     0,     0,     0,     0,   539,   540,   541,   542,
     543,     0,     0,   544,   545,   546,   547,     0,   548,   549,
     550,   551,     0,     0,     0,     0,   552,     0,   553,   554,
       0,     0,     0,     0,   555,   556,   557,     0,     0,     0,
     558,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1040,     0,     0,
       0,   559,     0,   560,   561,   562,   563,   564,   565,   566,
     567,   568,   569,     0,     0,     0,     0,   539,   540,   541,
     542,   543,   570,   571,   544,   545,   546,   547,     0,   548,
     549,   550,   551,   537,   538,     0,     0,   552,     0,   553,
     554,     0,     0,     0,     0,   555,   556,   557,     0,     0,
       0,   558,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   559,     0,   560,   561,   562,   563,   564,   565,
     566,   567,   568,   569,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   570,   571,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   537,   538,     0,     0,     0,     0,
       0,     0,   539,   540,   541,   542,   543,     0,     0,   544,
     545,   546,   547,     0,   548,   549,   550,   551,     0,     0,
       0,     0,   552,     0,   553,   554,   537,   538,     0,     0,
     555,   556,   557,     0,     0,     0,  -654,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   559,     0,   560,
     561,   562,   563,   564,   565,   566,   567,   568,   569,     0,
       0,     0,     0,   539,   540,   541,   542,   543,   570,   571,
     544,   545,   546,   547,     0,   548,   549,   550,   551,     0,
       0,     0,     0,   552,     0,   553,   554,   537,   538,     0,
       0,   555,   556,   557,     0,   539,   540,   541,   542,   543,
       0,     0,   544,   545,   546,   547,     0,   548,   549,   550,
     551,     0,     0,     0,     0,   552,     0,   553,   554,   537,
     538,     0,     0,   555,     0,   557,     0,     0,   559,     0,
     560,   561,   562,   563,   564,   565,   566,   567,   568,   569,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   570,
     571,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   560,   561,   562,   563,   564,   565,   566,   567,
     568,   569,     0,     0,     0,     0,   539,   540,   541,   542,
     543,   570,   571,   544,   545,   546,   547,     0,   548,   549,
     550,   551,     0,     0,     0,     0,   552,     0,   553,   554,
     537,   538,     0,     0,   555,     0,     0,     0,   539,   540,
     541,   542,   543,     0,     0,   544,   545,   546,   547,     0,
     548,   549,   550,   551,     0,     0,     0,     0,   552,     0,
     553,   554,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   560,   561,   562,   563,   564,   565,   566,
     567,   568,   569,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   570,   571,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   562,   563,   564,
     565,   566,   567,   568,   569,     0,     0,     0,     0,   539,
     540,   541,   542,   543,   570,   571,   544,     0,     0,   547,
       0,   548,   549,   550,   551,     0,     0,     0,     0,   552,
       0,   553,   554,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   517,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   157,     0,     0,     0,     0,     0,   158,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   563,
     564,   565,   566,   567,   568,   569,   159,     0,     0,     0,
       0,     0,     0,     0,     0,   570,   571,     0,     0,     0,
       0,   160,     0,     0,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     519,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   157,     0,     0,     0,     0,     0,   158,     0,
       0,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   159,     0,     0,   194,   727,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     160,     0,    54,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   161,   162,   163,     0,
     165,   166,   167,   168,   169,   379,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,     0,   183,   184,
     185,     0,     0,   188,   189,   190,   191,     0,     0,     0,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   932,     0,     0,   194,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    54,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   728,     0,     0,     0,     0,     0,     0,     0,
       0,   161,   162,   163,   729,   165,   166,   167,   168,   169,
     379,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,     0,   183,   184,   185,     0,     0,   188,   189,
     190,   191,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   157,
       0,     0,     0,     0,     0,   158,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   159,     0,     0,     0,     0,   933,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   160,     0,   934,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   157,     0,
       0,     0,     0,     0,   158,     0,     0,     0,     0,     0,
       0,    53,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   159,     0,     0,   194,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   160,     0,    54,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   207,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   194,     0,     0,     0,     0,   208,     0,
     209,     0,   210,   211,   212,   213,   214,   491,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,     0,
     226,   227,   228,     0,     0,   229,   230,   231,   232,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   233,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   235
};

static const yytype_int16 yycheck[] =
{
      12,   273,   123,   144,   463,   237,   581,   486,   360,   488,
     362,   490,   364,   585,   520,   470,    21,   594,   695,   420,
     717,     7,   123,   448,   701,    19,   573,    33,   605,     0,
     606,   607,   608,    14,    15,     6,    20,    21,    51,    90,
      52,    19,    32,    55,    56,    32,   517,    51,   519,    19,
      55,    48,    20,    21,   125,    61,   135,   120,    29,   163,
      31,   120,    33,   136,    18,    19,   163,   163,    39,   163,
     173,   199,    14,    15,   202,   652,   163,    48,    44,    91,
      92,    93,    94,    54,   200,   201,   119,   203,   163,   122,
     123,   122,   123,   175,   122,   123,   137,   138,   139,   202,
     204,   163,   175,    74,    44,    45,   200,   204,   204,   172,
     173,   123,   175,   689,   173,   178,   203,   176,   390,   391,
     151,   203,   593,   129,   203,   672,    97,   111,   112,   204,
     136,   202,   163,   156,   100,   119,   173,   121,   122,   123,
     124,   163,   204,   111,   112,   129,   172,    43,   145,    60,
     180,   119,   273,   121,   122,   123,   124,   163,   199,   163,
     100,   129,   195,   196,   195,   196,   163,   195,   196,   148,
     172,   120,   202,   199,   153,   156,   198,   120,   172,   184,
     161,   130,   163,   189,   165,    81,   175,   130,   178,   202,
     195,   178,   198,   244,   172,   197,   893,   181,   182,   183,
     184,   185,   172,   475,   781,   346,   196,   172,   785,   163,
     196,   195,   196,   890,   156,   205,   172,   198,   204,   161,
     199,   163,   799,   165,   173,   196,   163,   195,   196,   178,
     173,   202,   353,   354,   355,   356,   508,   136,   359,  1171,
     361,   642,   363,   819,   365,   670,   145,   172,   172,   172,
      24,   150,   353,   354,   355,   356,    30,   176,   359,   136,
     361,   273,   363,   145,   163,   163,   164,  1199,   145,   390,
     391,   848,   120,    47,   120,   120,   163,   202,   202,   202,
     179,   163,   130,   202,   130,   130,   163,   185,    62,   163,
     136,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,   151,   173,   447,   173,   173,   172,
       4,     5,   163,     7,    20,    21,   163,   145,   136,   886,
     461,   353,   354,   355,   356,   180,   447,   359,  1025,   361,
    1047,   363,   899,   365,   475,   163,   199,   369,   172,  1036,
     461,    35,   136,   818,   596,   163,   136,   202,    20,    21,
     722,   151,   604,   180,   198,   149,   172,   172,   390,   391,
     952,   136,   929,   163,   890,   199,   172,   508,    52,   163,
     172,    55,    56,   405,   900,   202,   172,   156,   153,   154,
     155,   148,   197,   199,   676,   677,   153,   679,   163,   956,
     986,   987,   988,   199,   172,   197,   537,   538,   180,   172,
     996,   997,   998,   199,   179,   111,   112,    91,    92,    93,
      94,   205,   195,   119,   706,   447,   122,   123,   124,   151,
     202,   199,    11,   129,   197,   924,    55,   163,   172,   461,
     172,   163,    61,    22,    23,   467,  1028,   109,   110,   111,
     112,   113,    55,   475,   116,   117,   118,   119,    61,   121,
     122,   123,   124,   197,  1021,   197,   175,   129,    69,   131,
     132,   180,    73,   135,   175,   137,   138,   139,   500,   180,
     198,   143,   175,   175,    55,   203,   508,   180,   180,   198,
      61,    92,    93,    94,    95,   172,   202,   198,   175,   195,
     196,   178,    20,    21,    55,   198,   198,    55,   163,   164,
      61,   533,   174,    61,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   655,    55,   176,   197,  1214,  1215,
     180,    61,   202,   195,   196,   666,   173,   668,   176,   203,
     671,   783,   180,   674,   655,   676,   677,   163,   679,   176,
     792,   913,   163,   180,   176,   666,  1045,   137,   180,   139,
     671,   176,   176,   674,   806,   180,   180,   176,   810,  1154,
     176,   180,   176,   203,   180,   706,   180,   176,   137,   138,
     139,   180,   166,   167,   168,   169,    76,    77,    20,    21,
      55,   109,   110,   111,   112,   113,   166,   167,   116,   117,
     118,   119,   202,   121,   122,   123,   124,   163,   202,  1078,
     204,   129,   203,   131,   132,   137,   166,   167,   168,   750,
     202,   100,   204,   166,   167,   153,   179,   179,   202,   198,
     912,   198,   179,   655,    20,    21,   163,   179,   179,   179,
     179,   179,   179,   179,   666,   176,   668,    34,   779,   671,
      34,   163,   674,   198,   676,   677,   163,   679,   204,   203,
     262,   179,   180,   181,   182,   183,   184,   185,   779,   271,
     175,   163,   163,   202,   163,   163,    32,   195,   196,   111,
     112,    32,   197,    21,   706,   198,    32,   119,   197,   121,
     122,   123,   124,   844,   175,   203,    41,   129,   175,   203,
     199,   198,    58,    59,   179,   198,   179,    58,    59,   179,
     179,   198,    58,    59,   198,   179,   198,   179,   198,   961,
     198,   163,   198,   109,   110,   111,   112,   113,   196,   201,
     116,   117,   118,   119,   197,   121,   122,   123,   124,   198,
     198,   198,   198,   129,   197,   131,   132,   163,   163,   180,
     198,   183,   184,   185,   153,   163,    41,   779,   198,   163,
     156,   117,    36,   195,   196,   121,   117,   202,   202,   202,
     121,   117,     9,   202,  1056,   121,   202,   202,   202,   381,
     382,   912,   202,    64,    41,   163,   180,   180,   180,   172,
     392,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     402,   403,   404,   172,   406,   407,   408,   197,   166,   195,
     196,   163,   179,  1055,   179,   204,   198,   198,   174,   841,
     163,   198,   178,   174,   180,   181,   198,   178,   174,   180,
     181,     1,   178,   163,   180,   181,   198,  1079,   198,   198,
     196,    32,   125,   175,    13,   196,   173,   156,   201,   205,
     196,    32,     7,   203,   205,   163,   163,   198,   163,   205,
     197,   463,   197,   197,   204,   166,   197,    58,    59,   163,
     472,   163,   163,   163,   198,   179,   198,    58,    59,   198,
      41,   202,   198,   197,   197,   197,   163,   197,   199,   204,
     912,   163,   180,    32,   197,   197,   202,   197,   180,    51,
     502,   197,  1144,   505,   202,   507,  1148,   204,   197,   511,
     512,   513,   514,   515,   516,   199,   518,   197,   520,   197,
     525,    69,    38,   202,   120,  1056,   117,  1058,    72,   120,
     121,  1183,   249,  1184,    70,  1184,   117,   539,   540,   130,
     121,   543,   544,   545,   546,   841,   548,  1058,   550,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,   561,
     562,   563,   564,   565,   566,   567,   568,   569,   971,   571,
    1184,  1184,  1184,     1,  1053,   879,    20,    21,   580,  1094,
    1153,  1097,   173,   174,    47,  1227,   668,   178,   590,   587,
     181,   366,   594,   174,   648,   597,   598,   178,   464,   180,
     181,   603,   704,   605,   358,   196,  1137,   609,   369,   898,
     520,   202,   614,  1168,   205,   196,    -1,    -1,    -1,    -1,
     369,    -1,    -1,    -1,   205,    32,  1137,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1056,    -1,  1058,   639,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     652,    58,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      -1,   673,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,   691,
     692,    -1,    -1,   137,   138,   139,    -1,    -1,   700,   143,
      -1,   703,   704,    20,    21,    -1,   708,    -1,    -1,    -1,
     117,    -1,    -1,    -1,   121,  1137,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
     174,    -1,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1171,
      -1,   195,   196,    -1,    58,    59,    -1,    -1,    -1,   203,
      -1,   763,    -1,    -1,    -1,   767,    -1,   174,    -1,    -1,
      -1,   178,   774,   180,   181,    -1,    -1,  1199,    -1,   781,
      -1,    -1,    -1,   785,    -1,    -1,    -1,    -1,   790,   196,
      -1,    -1,   109,   110,   111,   112,   113,   799,   205,   116,
     117,   118,   119,    -1,   121,   122,   123,   124,    -1,    -1,
      -1,    -1,   129,   117,   131,   132,    -1,   121,    32,   821,
     822,   823,   824,   825,   826,   827,   828,   829,   830,   831,
     832,   833,   834,   835,   836,   837,   838,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    59,   848,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,    -1,
     174,    -1,    -1,    -1,   178,    -1,   180,   181,   195,   196,
      -1,    -1,    -1,    -1,    -1,    -1,   888,   889,    -1,   891,
      -1,    -1,   196,    -1,   896,    -1,   898,    -1,    -1,    -1,
      -1,   205,    -1,   117,    -1,    -1,    -1,   121,   910,   911,
      -1,    -1,    -1,    -1,   916,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    -1,    -1,   926,    -1,   928,    -1,   930,    -1,
      -1,    20,    21,    -1,   936,    -1,    -1,    -1,   940,    -1,
      -1,    -1,    -1,    -1,   946,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   959,   960,    -1,
     174,    -1,    -1,    -1,   178,    -1,   180,   181,    -1,    -1,
      -1,   973,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   196,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,    -1,    -1,    20,    21,
     129,   130,   131,   132,   133,   134,    -1,    -1,   137,   138,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1064,    -1,  1066,    -1,    -1,  1069,    -1,    -1,
      -1,    -1,  1074,    -1,    -1,  1077,  1078,    -1,    -1,    -1,
    1082,    -1,    -1,    -1,   173,   174,    -1,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,    -1,    -1,  1101,
      -1,    -1,  1104,  1105,  1106,  1107,   195,   196,  1110,    -1,
      -1,  1113,  1114,    -1,  1116,    -1,    -1,  1119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1128,   109,   110,   111,
     112,   113,    -1,    -1,   116,   117,   118,   119,    -1,   121,
     122,   123,   124,    -1,    -1,    -1,    -1,   129,    -1,   131,
     132,    -1,    -1,    -1,    -1,   137,   138,   139,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1184,    -1,    -1,    -1,    -1,    -1,    -1,  1191,
      -1,    -1,   174,    -1,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,  1206,  1207,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,  1217,     1,   199,    -1,    -1,
    1222,     6,  1224,     8,     9,    10,    -1,    12,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    37,    38,    -1,    40,    -1,    42,    43,    -1,
      -1,    46,    -1,    48,    49,    50,    -1,    52,    53,    -1,
      -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    -1,    87,    88,    89,    -1,    32,    92,    93,    94,
      95,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,
      -1,    -1,    58,    59,    -1,   109,   110,   111,   112,    -1,
      -1,   126,   127,   128,    -1,   119,    -1,   121,   122,   123,
     124,   136,    -1,    -1,    -1,   129,    -1,   131,   132,   144,
     145,   146,   147,   148,    -1,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,   121,   181,   182,   183,    -1,
     185,    -1,    -1,   188,   189,    -1,    -1,   181,   182,   183,
     184,   185,    -1,   198,    -1,   200,   201,   202,   203,   204,
       1,   195,   196,    -1,    -1,     6,    -1,     8,     9,    10,
      -1,    12,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,   174,    -1,
      -1,    -1,   178,    -1,    -1,   181,    37,    38,    -1,    40,
      -1,    42,    43,    -1,    -1,    46,    -1,    48,    49,    50,
     196,    52,    53,    -1,    -1,    56,    57,    -1,    -1,   205,
      -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    -1,    87,    88,    89,    -1,
      -1,    92,    93,    94,    95,    -1,    -1,    98,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   144,   145,   146,   147,   148,    -1,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     181,   182,   183,    -1,   185,    -1,    -1,   188,   189,    -1,
      -1,    -1,    14,    15,    16,    17,    18,   198,    -1,   200,
     201,   202,   203,   204,    26,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,    51,
      -1,    53,    -1,    -1,    -1,    -1,    58,    59,    -1,    -1,
      -1,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    87,    88,    89,    -1,    -1,
      92,    93,    94,    95,    -1,    -1,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
     112,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,   146,   147,   148,    -1,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,    -1,    -1,   178,    -1,    -1,   181,
     182,   183,    -1,   185,    -1,    -1,   188,   189,    14,    15,
      16,    17,    18,    -1,   196,    -1,   198,    -1,   200,   201,
      26,   203,    -1,   205,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    58,    59,    -1,    -1,    -1,    63,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      -1,    87,    88,    89,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,
     146,   147,   148,    -1,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
      -1,    -1,   178,    -1,    -1,   181,   182,   183,    -1,   185,
      -1,    -1,   188,   189,    14,    15,    16,    17,    18,    -1,
     196,    -1,   198,    -1,   200,   201,    26,   203,    -1,   205,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    -1,    87,    88,    89,
      -1,    -1,    92,    93,    94,    95,    -1,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   144,   145,   146,   147,   148,    -1,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      -1,   181,   182,   183,    -1,   185,    -1,    -1,   188,   189,
      14,    15,    16,    17,    18,    -1,   196,    -1,   198,    -1,
     200,   201,    26,   203,    -1,   205,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    -1,    87,    88,    89,    -1,    -1,    92,    93,
      94,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     144,   145,   146,   147,   148,    -1,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,   181,   182,   183,
      -1,   185,    -1,    -1,   188,   189,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,   198,    46,   200,   201,    49,   203,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    -1,    87,    88,    89,    -1,
      -1,    92,    93,    94,    95,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,   126,   127,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   144,   145,   146,   147,   148,    -1,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     181,   182,   183,    -1,   185,    -1,    -1,   188,   189,    -1,
      -1,    -1,    14,    15,    16,    17,    18,   198,    -1,   200,
     201,    -1,   203,    25,    26,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    87,    88,    89,    -1,    -1,
      92,    93,    94,    95,    -1,    -1,    98,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,   146,   147,   148,    -1,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   181,
     182,   183,    -1,   185,    -1,    -1,   188,   189,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,   198,    46,   200,   201,
      49,   203,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    -1,    87,    88,
      89,    -1,    -1,    92,    93,    94,    95,    -1,    -1,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   144,   145,   146,   147,   148,
      -1,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    -1,   181,   182,   183,    -1,   185,    -1,    -1,   188,
     189,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   198,
      46,   200,   201,    49,   203,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      -1,    87,    88,    89,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,
     146,   147,   148,    -1,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    -1,   181,   182,   183,    -1,   185,
      -1,    -1,   188,   189,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,   198,    46,   200,   201,    49,   203,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    66,    67,    -1,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    -1,    87,    88,    89,    -1,    -1,    92,
      93,    94,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,   127,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   144,   145,   146,   147,   148,    -1,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   181,   182,
     183,    -1,   185,    -1,    -1,   188,   189,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,   198,    46,   200,   201,    49,
     203,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    -1,    87,    88,    89,
      -1,    -1,    92,    93,    94,    95,    -1,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   144,   145,   146,   147,   148,    -1,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,   182,   183,    -1,   185,    -1,    -1,   188,   189,
      14,    15,    16,    17,    18,    -1,   196,    -1,   198,   199,
     200,   201,    26,   203,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    -1,    87,    88,    89,    -1,    -1,    92,    93,
      94,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     144,   145,   146,   147,   148,    -1,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    26,    -1,   181,   182,   183,
      -1,   185,    -1,    -1,   188,   189,    -1,    38,    -1,    -1,
      -1,    -1,    43,   197,   198,    46,   200,   201,    49,   203,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    -1,    87,    88,    89,    -1,
      -1,    92,    93,    94,    95,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   144,   145,   146,   147,   148,    -1,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
     181,   182,   183,    -1,   185,    -1,    -1,   188,   189,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,   198,    46,   200,
     201,    49,   203,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    66,    67,
      -1,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    -1,    87,
      88,    89,    -1,    -1,    92,    93,    94,    95,    -1,    -1,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   144,   145,   146,   147,
     148,    -1,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,   183,    -1,   185,    -1,    -1,
     188,   189,    14,    15,    16,    17,    18,    -1,    -1,    -1,
     198,   199,   200,   201,    26,   203,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    59,    -1,    -1,
      -1,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    87,    88,    89,    -1,    -1,
      92,    93,    94,    95,    -1,    -1,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,   146,   147,   148,    -1,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   181,
     182,   183,    -1,   185,    -1,    -1,   188,   189,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,   198,    46,   200,   201,
      49,   203,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    -1,    87,    88,
      89,    -1,    -1,    92,    93,    94,    95,    -1,    -1,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   144,   145,   146,   147,   148,
      -1,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    -1,   181,   182,   183,    -1,   185,    -1,    -1,   188,
     189,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   198,
      46,   200,   201,    49,   203,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      -1,    87,    88,    89,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,
     146,   147,   148,    -1,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   179,    -1,   181,   182,   183,    -1,   185,
      -1,    -1,   188,   189,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   198,    -1,   200,   201,    26,   203,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    -1,    87,    88,    89,
      -1,    -1,    92,    93,    94,    95,    -1,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   144,   145,   146,   147,   148,    -1,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,   182,   183,    -1,   185,    -1,    -1,   188,   189,
      14,    15,    16,    17,    18,    -1,    -1,    -1,   198,   199,
     200,   201,    26,   203,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    -1,    87,    88,    89,    -1,    -1,    92,    93,
      94,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     144,   145,   146,   147,   148,    -1,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   179,    -1,   181,   182,   183,
      -1,   185,    -1,    -1,   188,   189,    14,    15,    16,    17,
      18,    -1,    -1,    -1,   198,    -1,   200,   201,    26,   203,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,
      -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    66,    67,
      -1,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    -1,    87,
      88,    89,    -1,    -1,    92,    93,    94,    95,    -1,    -1,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   144,   145,   146,   147,
     148,    -1,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,   183,    -1,   185,    -1,    -1,
     188,   189,    14,    15,    16,    17,    18,    -1,    -1,    -1,
     198,   199,   200,   201,    26,   203,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    87,    88,    89,    -1,    -1,
      92,    93,    94,    95,    -1,    -1,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,   146,   147,   148,    -1,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
     182,   183,    -1,   185,    -1,    -1,   188,   189,    14,    15,
      16,    17,    18,    -1,    -1,    -1,   198,   199,   200,   201,
      26,   203,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      -1,    87,    88,    89,    -1,    -1,    92,    93,    94,    95,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,
     146,   147,   148,    -1,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,   181,   182,   183,    -1,   185,
      -1,    26,   188,   189,    -1,    -1,    -1,    -1,    -1,    -1,
     196,    -1,   198,    38,   200,   201,    -1,   203,    43,    -1,
      -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    -1,    87,    88,    89,    -1,    -1,    92,    93,    94,
      95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,
     145,   146,   147,   148,    -1,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,   182,   183,    -1,
     185,    -1,    -1,   188,   189,    14,    15,    16,    17,    18,
      -1,    -1,    -1,   198,   199,   200,   201,    26,   203,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    -1,    87,    88,
      89,    -1,    -1,    92,    93,    94,    95,    -1,    -1,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   144,   145,   146,   147,   148,
      -1,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,   182,   183,    -1,   185,    -1,    -1,   188,
     189,    14,    15,    16,    17,    18,    -1,    -1,    -1,   198,
     199,   200,   201,    26,   203,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    66,    67,    -1,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    -1,    87,    88,    89,    -1,    -1,    92,
      93,    94,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   144,   145,   146,   147,   148,    -1,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,   182,
     183,    -1,   185,    -1,    -1,   188,   189,    14,    15,    16,
      17,    18,    -1,    -1,    -1,   198,   199,   200,   201,    26,
     203,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    66,
      67,    -1,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    -1,
      87,    88,    89,    -1,    -1,    92,    93,    94,    95,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,   146,
     147,   148,    -1,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,   181,   182,   183,    -1,   185,    -1,
      -1,   188,   189,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,   198,    46,   200,   201,    49,   203,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    -1,    87,    88,    89,    -1,    -1,    92,    93,
      94,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     144,   145,   146,   147,   148,    -1,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,   181,   182,   183,
      -1,   185,    -1,    -1,   188,   189,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,   198,    46,   200,   201,    49,   203,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    66,    67,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    -1,    87,    88,    89,    -1,
      -1,    92,    93,    94,    95,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   144,   145,   146,   147,   148,    -1,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
     181,   182,   183,    -1,   185,    -1,    -1,   188,   189,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,   198,    46,   200,
     201,    49,   203,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    66,    67,
      -1,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    -1,    87,
      88,    89,    -1,    -1,    92,    93,    94,    95,    -1,    -1,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,
      -1,    -1,    -1,   111,   112,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   144,   145,   146,   147,
     148,    -1,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,   183,    -1,   185,    -1,    -1,
     188,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     198,    -1,   200,   201,    -1,   203,   109,   110,   111,   112,
     113,    -1,    -1,   116,   117,   118,   119,    -1,   121,   122,
     123,   124,    20,    21,    -1,    -1,   129,    -1,   131,   132,
      -1,    -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   174,    -1,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,    20,    21,    -1,
      -1,   129,   130,   131,   132,   133,   134,    -1,    -1,   137,
     138,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   173,   174,    -1,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,    -1,
      -1,    -1,    -1,    -1,   202,    -1,    -1,    -1,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,    20,    21,    -1,    -1,   129,   130,   131,   132,
     133,   134,    -1,    -1,   137,   138,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,   174,    -1,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,    -1,    -1,    -1,    -1,    -1,   202,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,   113,    20,    21,   116,   117,
     118,   119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,
      -1,   129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,
     138,   139,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,    -1,
      -1,    -1,    -1,    -1,   202,   109,   110,   111,   112,   113,
      20,    21,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,
      -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,   196,    -1,    -1,    -1,    -1,    -1,   202,   109,
     110,   111,   112,   113,    20,    21,   116,   117,   118,   119,
      -1,   121,   122,   123,   124,    -1,    -1,    -1,    -1,   129,
      -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,   139,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,    -1,    -1,   199,
      -1,    -1,    -1,   109,   110,   111,   112,   113,    20,    21,
     116,   117,   118,   119,    -1,   121,   122,   123,   124,    -1,
      -1,    -1,    -1,   129,    -1,   131,   132,    -1,    -1,    -1,
      -1,   137,   138,   139,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,    -1,    -1,   199,    -1,    -1,    -1,   109,   110,   111,
     112,   113,    20,    21,   116,   117,   118,   119,    -1,   121,
     122,   123,   124,    -1,    -1,    -1,    -1,   129,    -1,   131,
     132,    -1,    -1,    -1,    -1,   137,   138,   139,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,    -1,    -1,   199,    -1,    -1,
      -1,   109,   110,   111,   112,   113,    20,    21,   116,   117,
     118,   119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,
      -1,   129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,
     138,   139,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,    -1,
      -1,   199,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      20,    21,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,
      -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,   196,    -1,    -1,   199,    -1,    -1,    -1,   109,
     110,   111,   112,   113,    20,    21,   116,   117,   118,   119,
      -1,   121,   122,   123,   124,    -1,    -1,    -1,    -1,   129,
      -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,   139,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,    -1,    -1,   199,
      -1,    -1,    -1,   109,   110,   111,   112,   113,    20,    21,
     116,   117,   118,   119,    -1,   121,   122,   123,   124,    -1,
      -1,    -1,    -1,   129,    -1,   131,   132,    -1,    -1,    -1,
      -1,   137,   138,   139,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,    -1,    -1,   199,    -1,    -1,    -1,   109,   110,   111,
     112,   113,    20,    21,   116,   117,   118,   119,    -1,   121,
     122,   123,   124,    -1,    -1,    -1,    -1,   129,    -1,   131,
     132,    -1,    -1,    -1,    -1,   137,   138,   139,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,    -1,    -1,   199,    -1,    -1,
      -1,   109,   110,   111,   112,   113,    20,    21,   116,   117,
     118,   119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,
      -1,   129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,
     138,   139,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,    -1,
      -1,   199,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      20,    21,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,
      -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,   196,    -1,    -1,   199,    -1,    -1,    -1,   109,
     110,   111,   112,   113,    20,    21,   116,   117,   118,   119,
      -1,   121,   122,   123,   124,    -1,    -1,    -1,    -1,   129,
      -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,   139,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,    -1,    -1,   199,
      -1,    -1,    -1,   109,   110,   111,   112,   113,    20,    21,
     116,   117,   118,   119,    -1,   121,   122,   123,   124,    -1,
      -1,    -1,    -1,   129,    -1,   131,   132,    -1,    -1,    -1,
      -1,   137,   138,   139,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,    -1,    -1,   199,    -1,    -1,    -1,   109,   110,   111,
     112,   113,    20,    21,   116,   117,   118,   119,    -1,   121,
     122,   123,   124,    -1,    -1,    -1,    -1,   129,    -1,   131,
     132,    -1,    -1,    -1,    -1,   137,   138,   139,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,    -1,    -1,   199,    -1,    -1,
      -1,   109,   110,   111,   112,   113,    20,    21,   116,   117,
     118,   119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,
      -1,   129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,
     138,   139,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,    -1,
      -1,   199,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      20,    21,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,
      -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,   196,    -1,    -1,   199,    -1,    -1,    -1,   109,
     110,   111,   112,   113,    20,    21,   116,   117,   118,   119,
      -1,   121,   122,   123,   124,    -1,    -1,    -1,    -1,   129,
      -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,   139,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,    -1,    -1,   199,
      -1,    -1,    -1,   109,   110,   111,   112,   113,    20,    21,
     116,   117,   118,   119,    -1,   121,   122,   123,   124,    -1,
      -1,    -1,    -1,   129,    -1,   131,   132,    -1,    -1,    -1,
      -1,   137,   138,   139,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,    -1,    -1,   199,    -1,    -1,    -1,   109,   110,   111,
     112,   113,    20,    21,   116,   117,   118,   119,    -1,   121,
     122,   123,   124,    -1,    -1,    -1,    -1,   129,    -1,   131,
     132,    -1,    -1,    -1,    -1,   137,   138,   139,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,    -1,    -1,   199,    -1,    -1,
      -1,   109,   110,   111,   112,   113,    20,    21,   116,   117,
     118,   119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,
      -1,   129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,
     138,   139,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,    -1,
      -1,   199,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      20,    21,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,
      -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,   196,    -1,    -1,   199,    -1,    -1,    -1,   109,
     110,   111,   112,   113,    20,    21,   116,   117,   118,   119,
      -1,   121,   122,   123,   124,    -1,    -1,    -1,    -1,   129,
      -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,   139,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,    -1,    -1,   199,
      -1,    -1,    -1,   109,   110,   111,   112,   113,    20,    21,
     116,   117,   118,   119,    -1,   121,   122,   123,   124,    -1,
      -1,    -1,    -1,   129,    -1,   131,   132,    -1,    -1,    -1,
      -1,   137,   138,   139,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,    -1,    -1,   199,    -1,    -1,    -1,   109,   110,   111,
     112,   113,    20,    21,   116,   117,   118,   119,    -1,   121,
     122,   123,   124,    -1,    -1,    -1,    -1,   129,    -1,   131,
     132,    -1,    -1,    -1,    -1,   137,   138,   139,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,    -1,    -1,   199,    -1,    -1,
      -1,   109,   110,   111,   112,   113,    20,    21,   116,   117,
     118,   119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,
      -1,   129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,
     138,   139,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,    -1,
      -1,   199,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      20,    21,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,
      -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,   196,    -1,    -1,   199,    -1,    -1,    -1,   109,
     110,   111,   112,   113,    20,    21,   116,   117,   118,   119,
      -1,   121,   122,   123,   124,    -1,    -1,    -1,    -1,   129,
      -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,   139,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,    -1,    -1,   199,
      -1,    -1,    -1,   109,   110,   111,   112,   113,    20,    21,
     116,   117,   118,   119,    -1,   121,   122,   123,   124,    -1,
      -1,    -1,    -1,   129,    -1,   131,   132,    -1,    -1,    -1,
      -1,   137,   138,   139,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,   197,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,
     112,   113,    20,    21,   116,   117,   118,   119,    -1,   121,
     122,   123,   124,    -1,    -1,    -1,    -1,   129,    -1,   131,
     132,    -1,    -1,    -1,    -1,   137,   138,   139,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,   197,    -1,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,   113,    20,    21,   116,   117,
     118,   119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,
      -1,   129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,
     138,   139,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
      20,    21,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,    -1,
      -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,   196,   197,    -1,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,   113,    -1,    -1,   116,   117,   118,   119,
      -1,   121,   122,   123,   124,    20,    21,    -1,    -1,   129,
      -1,   131,   132,    -1,    -1,    -1,    -1,   137,   138,   139,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,    20,    21,   129,   130,   131,   132,   133,   134,
      -1,    -1,   137,   138,   139,   140,   141,   142,   143,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,   174,
      -1,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,   196,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,    20,    21,   121,   122,   123,    -1,    -1,    -1,    -1,
      -1,    -1,   130,   131,   132,   133,   134,    -1,    -1,   137,
     138,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
     188,   189,    -1,    -1,    18,    -1,    -1,   195,   196,    -1,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
     109,   110,   111,   112,   113,    -1,    -1,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,    -1,    -1,    -1,    -1,
     129,    -1,   131,   132,    -1,    -1,   135,    -1,   137,   138,
     139,    65,    66,    67,   143,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    -1,    87,    88,    89,    -1,    -1,    92,    93,
      94,    95,    -1,    -1,    -1,   174,    -1,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,   113,   195,   196,   116,   117,
     118,   119,    -1,   121,   122,   123,   124,    20,    21,    -1,
      -1,   129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,
     138,   139,    -1,    -1,    -1,   143,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,
     113,    -1,    -1,   116,   117,   118,   119,    -1,   121,   122,
     123,   124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,
      -1,    -1,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,
      -1,   174,    -1,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,    -1,    -1,    -1,    -1,   109,   110,   111,
     112,   113,   195,   196,   116,   117,   118,   119,    -1,   121,
     122,   123,   124,    20,    21,    -1,    -1,   129,    -1,   131,
     132,    -1,    -1,    -1,    -1,   137,   138,   139,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,   109,   110,   111,   112,   113,    -1,    -1,   116,
     117,   118,   119,    -1,   121,   122,   123,   124,    -1,    -1,
      -1,    -1,   129,    -1,   131,   132,    20,    21,    -1,    -1,
     137,   138,   139,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,    -1,
      -1,    -1,    -1,   109,   110,   111,   112,   113,   195,   196,
     116,   117,   118,   119,    -1,   121,   122,   123,   124,    -1,
      -1,    -1,    -1,   129,    -1,   131,   132,    20,    21,    -1,
      -1,   137,   138,   139,    -1,   109,   110,   111,   112,   113,
      -1,    -1,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,    20,
      21,    -1,    -1,   137,    -1,   139,    -1,    -1,   174,    -1,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,   109,   110,   111,   112,
     113,   195,   196,   116,   117,   118,   119,    -1,   121,   122,
     123,   124,    -1,    -1,    -1,    -1,   129,    -1,   131,   132,
      20,    21,    -1,    -1,   137,    -1,    -1,    -1,   109,   110,
     111,   112,   113,    -1,    -1,   116,   117,   118,   119,    -1,
     121,   122,   123,   124,    -1,    -1,    -1,    -1,   129,    -1,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,   179,   180,
     181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,   113,   195,   196,   116,    -1,    -1,   119,
      -1,   121,   122,   123,   124,    -1,    -1,    -1,    -1,   129,
      -1,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   179,
     180,   181,   182,   183,   184,   185,    47,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,    -1,    -1,    -1,
      -1,    62,    -1,    -1,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,   149,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    -1,   163,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    65,    66,    67,    -1,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    -1,    87,    88,
      89,    -1,    -1,    92,    93,    94,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    66,    67,   163,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    -1,    87,    88,    89,    -1,    -1,    92,    93,
      94,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    47,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,   163,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    47,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,   163,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    65,    -1,
      67,    -1,    69,    70,    71,    72,    73,   163,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    -1,
      87,    88,    89,    -1,    -1,    92,    93,    94,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   163
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   207,     0,     6,    29,    31,    33,    39,    48,    54,
      74,    97,   196,   202,   210,   218,   219,   224,   252,   256,
     282,   350,   356,   359,   365,   404,   407,    18,    19,   163,
     245,   246,   247,   156,   225,   226,   163,   164,   185,   220,
     221,   222,   163,   203,   353,   163,   200,   209,   408,   405,
      33,    61,   129,   136,   163,   189,   198,   248,   249,   250,
     251,   269,     4,     5,     7,    35,   363,    60,   348,   173,
     172,   175,   172,   221,    21,    55,   184,   195,   223,   354,
     353,   355,   348,   163,   163,   250,   163,   136,   250,   250,
     198,   137,   138,   139,   172,   197,    55,    61,   257,   259,
      55,    61,   357,    55,    61,   364,    55,    61,   349,    14,
      15,   156,   161,   163,   165,   198,   212,   246,   156,   226,
     163,   220,   220,   173,   202,   204,   353,    55,    61,   208,
     203,   406,   163,   199,   247,   250,   250,   250,   250,   260,
     163,   358,   366,   203,   351,   166,   167,   211,    14,    15,
     156,   161,   163,   212,   243,   244,   223,    24,    30,    47,
      62,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,   149,   269,   369,   371,   372,   374,
     378,   403,   202,   163,   373,   203,   199,    34,    65,    67,
      69,    70,    71,    72,    73,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    87,    88,    89,    92,
      93,    94,    95,   111,   112,   163,   255,   258,   175,   203,
     100,   361,   362,   346,   153,   325,   166,   167,   168,   172,
     199,   179,   179,   179,   179,   198,   179,   179,   179,   179,
     179,   179,   198,    32,    58,    59,   117,   121,   174,   178,
     181,   196,   205,   176,   202,   163,   338,   339,    20,    21,
      37,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   121,
     122,   123,   130,   131,   132,   133,   134,   137,   138,   139,
     140,   141,   142,   143,   174,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   188,   189,   195,   196,    34,
      34,   198,   253,   203,   261,    69,    73,    92,    93,    94,
      95,   370,   352,   163,   367,   204,   347,   247,   145,   163,
     344,   345,   243,   381,   383,   385,   379,   163,   375,   387,
     389,   391,   393,   395,   397,   399,   401,    14,    15,    16,
      17,    18,    26,    38,    43,    46,    49,    53,    63,    74,
      98,   111,   112,   144,   145,   146,   147,   148,   150,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     164,   165,   181,   182,   183,   185,   188,   189,   198,   200,
     201,   214,   216,   261,   265,   269,   274,   285,   292,   295,
     298,   302,   303,   306,   307,   309,   314,   317,   324,   369,
     409,   413,   417,   419,   421,    32,   178,    32,   178,   196,
     205,   197,   317,   205,   378,   163,   204,   175,   202,   163,
     163,   163,   197,    21,   163,   197,   148,   199,   325,   334,
     335,   175,   254,   263,   203,   163,   204,   175,   360,   203,
     325,   197,   198,    41,   172,   175,   178,   343,   403,   377,
     403,   403,   403,   199,   373,   403,   253,   403,   253,   403,
     253,   163,   336,   337,   403,   339,   179,   271,   371,   409,
     198,   179,   198,   179,   179,   198,   179,   198,   179,   317,
     317,   198,   198,   198,   198,   198,   198,    12,   378,    12,
     378,   317,   415,   418,   179,   213,   317,   317,   317,   269,
     317,   317,   317,   201,   163,   198,   252,    20,    21,   109,
     110,   111,   112,   113,   116,   117,   118,   119,   121,   122,
     123,   124,   129,   131,   132,   137,   138,   139,   143,   174,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     195,   196,   199,   198,   197,   197,   403,   204,   338,   163,
     198,    48,   145,   163,   341,   368,   199,   202,   403,     1,
       8,     9,    10,    12,    25,    27,    28,    37,    40,    42,
      50,    52,    56,    57,    63,    99,   126,   127,   128,   164,
     202,   204,   227,   228,   231,   234,   235,   237,   238,   239,
     240,   241,   262,   264,   268,   270,   275,   276,   277,   278,
     279,   280,   281,   282,   284,   304,   305,   317,   352,   173,
     202,   269,   330,   345,   197,   317,   163,   163,   378,   120,
     130,   173,   342,   180,   180,   202,   180,   180,   180,   202,
     180,   254,   180,   254,   180,   254,   175,   180,   202,   180,
     202,   272,   198,   269,   293,   317,   286,   288,   317,   290,
     317,   378,   317,   317,   317,   317,   317,   317,   368,    51,
     163,   181,   198,   317,   410,   411,   412,   414,   416,   368,
     198,   411,   416,   135,   202,   204,   266,   168,   169,   211,
     217,   199,   145,   150,   179,   269,   308,   196,   199,   296,
     317,   153,   301,    18,   151,   163,   369,    18,   151,   163,
     369,   317,   317,   317,   317,   317,   317,   163,   317,   151,
     163,   317,   317,   317,   317,   317,   317,   317,   317,   317,
      21,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   122,   123,   151,   163,   195,   196,   315,   317,
     199,   296,   317,   341,   198,    41,   120,   172,   173,   175,
     178,   340,   334,   317,   368,   120,   268,   304,   317,   261,
      59,   317,   317,   163,   202,   156,    56,   317,   261,   120,
     268,   317,   201,   302,   302,   302,   317,    36,   202,   202,
     317,     9,   202,   202,   202,   202,   202,    64,   283,   125,
     202,   101,   102,   103,   104,   105,   106,   107,   108,   114,
     115,   120,   130,   133,   134,   140,   141,   142,   173,   204,
     317,   196,   204,   252,   331,   199,    41,   202,   342,   268,
     317,   382,   384,   403,   386,   380,   376,   388,   180,   392,
     180,   396,   180,   403,   400,   336,   402,   403,   199,   296,
     179,   317,   403,   199,   378,   378,   199,   378,   199,   180,
     199,   199,   199,   199,   199,   199,    19,   302,   130,   340,
     199,   135,   172,   202,   412,   197,   172,   197,   202,    19,
     199,   412,   204,   317,   415,   204,   378,   317,   166,   215,
     198,   198,   310,   312,   163,   410,   172,   199,   120,   130,
     173,   178,   299,   300,   253,   179,   198,   179,   198,   198,
     198,   197,    18,   151,   163,   369,   175,   151,   163,   317,
     198,   151,   163,   317,     1,   197,   172,   199,   199,   317,
     163,   163,   403,   268,   317,   261,    19,   268,   317,   120,
     173,    13,   317,   261,   173,   175,   156,   268,   317,   201,
     261,   203,   261,   236,   345,   302,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   126,   127,   128,   317,
     317,   317,   317,   317,   317,   317,   126,   127,   128,   317,
     202,   251,     7,   325,   329,   163,   268,   317,   202,   390,
     394,   398,   180,   199,   163,   199,   180,   180,   180,   180,
     301,   198,   296,   317,   317,   411,   412,   317,   163,   410,
     197,   197,   317,   197,   414,   296,   411,   204,   197,   180,
     170,   166,   317,   317,   378,   253,   197,   196,   317,   163,
     163,   163,   163,   172,   197,   254,   318,   317,   320,   317,
     199,   296,   317,   179,   198,   317,   198,   197,   317,   198,
     197,   316,   199,    41,   340,   296,   261,   242,   263,    11,
      22,    23,   229,   230,   317,   302,   302,   302,   302,   302,
     302,   197,    55,    61,   328,    44,   100,   326,   202,   202,
     273,   180,   202,   294,   287,   289,   291,   198,   199,   296,
     202,   412,   197,   130,   340,   197,   202,   412,   197,   267,
     199,   199,   180,   254,   199,   410,   299,   197,   135,   261,
     297,   378,   199,   403,   199,   199,   199,   322,   317,   317,
     199,   317,   163,   317,   202,   317,   204,   261,   317,    11,
     232,   202,    45,   326,   328,   317,   163,   317,   317,   317,
     199,   317,   199,   317,   197,   197,   317,   317,   317,   204,
     317,   311,   180,   197,   120,   317,   180,   180,   403,   199,
     199,   199,   261,   261,   233,   202,    32,   327,   327,   341,
     199,   180,   199,   202,   420,   197,   420,   197,   308,   313,
     199,   317,   319,   321,   180,   230,    25,    99,   234,   275,
     276,   277,   279,   317,   332,   333,   317,    51,   197,   204,
     308,   323,   120,   317,   120,   317,   258,   258,   199,   317,
     197,   197,   317,   317,   202,   261
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   206,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   208,   208,   208,   209,   209,
     210,   211,   211,   211,   211,   212,   213,   213,   213,   214,
     215,   215,   217,   216,   218,   219,   220,   220,   221,   221,
     221,   221,   222,   222,   223,   223,   224,   225,   225,   226,
     226,   227,   228,   228,   229,   229,   230,   230,   230,   231,
     231,   232,   233,   232,   234,   234,   234,   234,   234,   235,
     236,   235,   237,   238,   239,   240,   242,   241,   243,   243,
     243,   243,   243,   243,   244,   244,   245,   245,   245,   246,
     246,   246,   246,   246,   246,   246,   246,   247,   247,   248,
     248,   248,   249,   249,   250,   250,   250,   250,   250,   250,
     250,   251,   251,   252,   252,   253,   253,   253,   254,   254,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   256,   257,   257,   257,   258,   260,
     259,   261,   261,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   263,   263,   263,   264,   266,   267,
     265,   268,   268,   268,   268,   269,   269,   269,   270,   270,
     272,   273,   271,   271,   274,   274,   274,   274,   275,   276,
     277,   277,   277,   278,   278,   278,   279,   279,   280,   280,
     280,   281,   282,   282,   283,   283,   284,   286,   287,   285,
     288,   289,   285,   290,   291,   285,   293,   294,   292,   295,
     295,   295,   296,   296,   297,   297,   297,   298,   298,   298,
     299,   299,   299,   299,   300,   300,   301,   301,   302,   302,
     303,   303,   303,   303,   303,   303,   303,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   305,   305,   305,   305,
     305,   305,   306,   306,   307,   307,   308,   308,   309,   310,
     311,   309,   312,   313,   309,   314,   314,   315,   316,   314,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   318,
     319,   317,   317,   317,   317,   320,   321,   317,   317,   317,
     322,   323,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   325,   325,   326,   326,   326,   327,   327,   328,
     328,   328,   329,   330,   331,   330,   332,   330,   333,   330,
     330,   334,   334,   334,   335,   335,   336,   336,   337,   337,
     338,   339,   339,   340,   340,   341,   341,   341,   341,   341,
     341,   342,   342,   342,   343,   343,   344,   344,   344,   344,
     344,   345,   345,   345,   345,   345,   346,   347,   346,   348,
     348,   349,   349,   349,   350,   351,   350,   352,   352,   352,
     354,   353,   355,   355,   356,   356,   357,   357,   357,   358,
     359,   359,   360,   360,   361,   361,   362,   363,   363,   364,
     364,   364,   366,   367,   365,   368,   368,   368,   368,   368,
     369,   369,   369,   369,   369,   369,   369,   369,   369,   369,
     369,   369,   369,   369,   369,   369,   369,   369,   369,   369,
     369,   369,   369,   369,   369,   369,   369,   370,   370,   370,
     370,   370,   370,   371,   372,   372,   372,   373,   373,   375,
     376,   374,   377,   377,   378,   378,   378,   378,   378,   378,
     378,   378,   378,   378,   378,   378,   378,   378,   378,   378,
     378,   378,   379,   380,   378,   378,   381,   382,   378,   383,
     384,   378,   385,   386,   378,   378,   387,   388,   378,   389,
     390,   378,   378,   391,   392,   378,   393,   394,   378,   378,
     395,   396,   378,   397,   398,   378,   399,   400,   378,   401,
     402,   378,   403,   403,   403,   405,   406,   404,   408,   407,
     409,   409,   409,   409,   410,   410,   410,   410,   411,   411,
     412,   412,   413,   413,   413,   413,   413,   413,   414,   414,
     414,   415,   415,   416,   416,   417,   417,   418,   418,   419,
     420,   420,   421,   421
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     0,     1,     1,     1,     1,
       4,     1,     1,     2,     2,     3,     0,     2,     4,     3,
       1,     2,     0,     4,     2,     2,     1,     1,     1,     2,
       3,     3,     2,     4,     0,     1,     2,     1,     3,     1,
       3,     3,     3,     2,     1,     1,     0,     2,     4,     1,
       1,     0,     0,     3,     1,     1,     1,     1,     1,     4,
       0,     6,     6,     2,     3,     3,     0,     5,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     1,     5,     1,     3,     1,
       1,     1,     1,     4,     1,     2,     3,     3,     3,     3,
       2,     1,     3,     0,     3,     0,     2,     3,     0,     2,
       1,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     3,     3,     2,     2,     3,     3,
       2,     2,     2,     2,     2,     3,     3,     3,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     0,     1,     1,     3,     0,
       4,     3,     7,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     2,     2,     1,     1,     1,
       1,     2,     2,     2,     0,     2,     2,     3,     0,     0,
       7,     3,     2,     2,     2,     1,     3,     2,     2,     3,
       0,     0,     5,     1,     2,     4,     5,     2,     1,     1,
       1,     2,     3,     2,     2,     3,     2,     3,     2,     2,
       3,     4,     1,     1,     1,     0,     3,     0,     0,     7,
       0,     0,     7,     0,     0,     7,     0,     0,     6,     5,
       8,    10,     1,     3,     1,     2,     3,     1,     1,     2,
       2,     2,     2,     2,     1,     3,     0,     4,     1,     6,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     4,     4,     4,     4,
       4,     4,     6,     8,     5,     6,     1,     4,     3,     0,
       0,     8,     0,     0,     9,     3,     4,     0,     0,     5,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     2,
       3,     4,     5,     4,     5,     3,     4,     1,     3,     4,
       3,     4,     2,     4,     4,     7,     8,     3,     5,     0,
       0,     8,     3,     3,     3,     0,     0,     8,     3,     4,
       0,     0,     9,     4,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     2,     4,     1,     4,     4,     4,     4,
       4,     1,     6,     7,     6,     6,     7,     7,     6,     7,
       6,     6,     0,     4,     0,     1,     1,     0,     1,     0,
       1,     1,     4,     0,     0,     4,     0,     9,     0,     9,
       5,     2,     3,     4,     1,     3,     1,     3,     1,     3,
       3,     1,     3,     1,     1,     1,     2,     3,     5,     3,
       3,     1,     1,     1,     0,     1,     1,     4,     3,     3,
       5,     4,     6,     5,     5,     4,     0,     0,     4,     0,
       1,     0,     1,     1,     6,     0,     6,     0,     3,     5,
       0,     4,     2,     3,     4,     2,     0,     1,     1,     1,
       7,     9,     0,     2,     0,     1,     3,     1,     1,     0,
       1,     1,     0,     0,     9,     1,     4,     3,     3,     5,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     4,     1,     3,     0,
       0,     6,     1,     3,     1,     1,     1,     1,     4,     3,
       4,     2,     2,     3,     2,     3,     2,     2,     3,     3,
       3,     2,     0,     0,     6,     2,     0,     0,     6,     0,
       0,     6,     0,     0,     6,     1,     0,     0,     6,     0,
       0,     7,     1,     0,     0,     6,     0,     0,     7,     1,
       0,     0,     6,     0,     0,     7,     0,     0,     6,     0,
       0,     6,     1,     3,     3,     0,     0,     8,     0,     7,
       1,     1,     1,     1,     3,     3,     5,     5,     1,     3,
       0,     2,     6,     5,     7,     8,     6,     8,     1,     3,
       3,     3,     1,     1,     3,     5,     5,     1,     3,     4,
       0,     3,    10,    10
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = DAS_YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == DAS_YYEMPTY)                                        \
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
   Use DAS_YYerror or DAS_YYUNDEF. */
#define YYERRCODE DAS_YYUNDEF

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
#if DAS_YYDEBUG

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

#  elif defined DAS_YYLTYPE_IS_TRIVIAL && DAS_YYLTYPE_IS_TRIVIAL

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
#else /* !DAS_YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !DAS_YYDEBUG */


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

    case YYSYMBOL_expression_any: /* expression_any  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expressions: /* expressions  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_keyword: /* expr_keyword  */
            { delete ((*yyvaluep).pExpression); }
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

    case YYSYMBOL_expr_numeric_const: /* expr_numeric_const  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_assign: /* expr_assign  */
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

    case YYSYMBOL_variant_type: /* variant_type  */
            { delete ((*yyvaluep).pVarDecl); }
        break;

    case YYSYMBOL_variant_type_list: /* variant_type_list  */
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

    case YYSYMBOL_bitfield_type_declaration: /* bitfield_type_declaration  */
            { delete ((*yyvaluep).pTypeDecl); }
        break;

    case YYSYMBOL_table_type_pair: /* table_type_pair  */
            { delete ((*yyvaluep).aTypePair).firstType; delete ((*yyvaluep).aTypePair).secondType; }
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

    case YYSYMBOL_make_struct_dim: /* make_struct_dim  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_optional_block: /* optional_block  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_struct_decl: /* make_struct_decl  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_tuple: /* make_tuple  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_map_tuple: /* make_map_tuple  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_dim: /* make_dim  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_dim_decl: /* make_dim_decl  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_make_table: /* make_table  */
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
# if defined DAS_YYLTYPE_IS_TRIVIAL && DAS_YYLTYPE_IS_TRIVIAL
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

  yychar = DAS_YYEMPTY; /* Cause a token to be read.  */

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
  if (yychar == DAS_YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, &yylloc, scanner);
    }

  if (yychar <= DAS_YYEOF)
    {
      yychar = DAS_YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == DAS_YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = DAS_YYUNDEF;
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
  yychar = DAS_YYEMPTY;
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
  case 15: /* optional_public_or_private_module: %empty  */
                        { (yyval.b) = yyextra->g_Program->policies.default_module_public; }
    break;

  case 16: /* optional_public_or_private_module: "public"  */
                        { (yyval.b) = true; }
    break;

  case 17: /* optional_public_or_private_module: "private"  */
                        { (yyval.b) = false; }
    break;

  case 18: /* module_name: '$'  */
                    { (yyval.s) = new string("$"); }
    break;

  case 19: /* module_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 20: /* module_declaration: "module" module_name optional_shared optional_public_or_private_module  */
                                                                                                      {
        yyextra->g_Program->thisModuleName = *(yyvsp[-2].s);
        yyextra->g_Program->thisModule->isPublic = (yyvsp[0].b);
        yyextra->g_Program->thisModule->isModule = true;
        if ( yyextra->g_Program->thisModule->name.empty() ) {
            yyextra->g_Program->thisModule->name = *(yyvsp[-2].s);
        } else if ( yyextra->g_Program->thisModule->name != *(yyvsp[-2].s) ){
            das_yyerror(scanner,"this module already has a name " + yyextra->g_Program->thisModule->name,tokAt(scanner,(yylsp[-2])),
                CompilationError::module_already_has_a_name);
        }
        if ( !yyextra->g_Program->policies.ignore_shared_modules ) {
            yyextra->g_Program->promoteToBuiltin = (yyvsp[-1].b);
        }
        delete (yyvsp[-2].s);
    }
    break;

  case 21: /* character_sequence: STRING_CHARACTER  */
                                                            { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
    break;

  case 22: /* character_sequence: STRING_CHARACTER_ESC  */
                                                            { (yyval.s) = new string(); *(yyval.s) += "\\\\"; }
    break;

  case 23: /* character_sequence: character_sequence STRING_CHARACTER  */
                                                            { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
    break;

  case 24: /* character_sequence: character_sequence STRING_CHARACTER_ESC  */
                                                            { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += "\\\\"; }
    break;

  case 25: /* string_constant: "start of the string" character_sequence "end of the string"  */
                                                           { (yyval.s) = (yyvsp[-1].s); }
    break;

  case 26: /* string_builder_body: %empty  */
        {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 27: /* string_builder_body: string_builder_body character_sequence  */
                                                           {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror(scanner,"invalid escape sequence",tokAt(scanner,(yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt(scanner,(yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 28: /* string_builder_body: string_builder_body "{" expr "}"  */
                                                                                {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 29: /* string_builder: "start of the string" string_builder_body "end of the string"  */
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

  case 30: /* reader_character_sequence: STRING_CHARACTER  */
                               {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
    break;

  case 31: /* reader_character_sequence: reader_character_sequence STRING_CHARACTER  */
                                                                {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
    break;

  case 32: /* $@1: %empty  */
                                        {
        auto macros = yyextra->g_Program->getReaderMacro(*(yyvsp[0].s));
        if ( macros.size()==0 ) {
            das_yyerror(scanner,"reader macro " + *(yyvsp[0].s) + " not found",tokAt(scanner,(yylsp[0])),
                CompilationError::unsupported_read_macro);
        } else if ( macros.size()>1 ) {
            string options;
            for ( auto & x : macros ) {
                options += "\t" + x->module->name + "::" + x->name + "\n";
            }
            das_yyerror(scanner,"too many options for the reader macro " + *(yyvsp[0].s) +  "\n" + options, tokAt(scanner,(yylsp[0])),
                CompilationError::unsupported_read_macro);
        } else if ( yychar != '~' ) {
            das_yyerror(scanner,"expecting ~ after the reader macro", tokAt(scanner,(yylsp[0])),
                CompilationError::syntax_error);
        } else {
            yyextra->g_ReaderMacro = macros.back().get();
            yyextra->g_ReaderExpr = new ExprReader(tokAt(scanner,(yylsp[-1])),yyextra->g_ReaderMacro);
            yyclearin ;
            das_yybegin_reader(scanner);
        }
    }
    break;

  case 33: /* expr_reader: '%' name_in_namespace $@1 reader_character_sequence  */
                                     {
        yyextra->g_ReaderExpr->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = yyextra->g_ReaderExpr;
        delete (yyvsp[-2].s);
        yyextra->g_ReaderMacro = nullptr;
        yyextra->g_ReaderExpr = nullptr;
    }
    break;

  case 34: /* options_declaration: "options" annotation_argument_list  */
                                                   {
        if ( yyextra->g_Program->options.size() ) {
            yyextra->g_Program->options.insert ( yyextra->g_Program->options.begin(),
                (yyvsp[0].aaList)->begin(), (yyvsp[0].aaList)->end() );
        } else {
            swap ( yyextra->g_Program->options, *(yyvsp[0].aaList) );
        }
        auto opt = yyextra->g_Program->options.find("indenting", tInt);
        if (opt)
        {
            if (opt->iValue != 0 && opt->iValue != 2 && opt->iValue != 4 && opt->iValue != 8)//this is error
                yyextra->das_tab_size = yyextra->das_def_tab_size;
            else
                yyextra->das_tab_size = opt->iValue ? opt->iValue : yyextra->das_def_tab_size;//0 is default
            yyextra->g_FileAccessStack.back()->tabSize = yyextra->das_tab_size;
        }
        delete (yyvsp[0].aaList);
    }
    break;

  case 36: /* keyword_or_name: "name"  */
                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 37: /* keyword_or_name: "keyword"  */
                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 38: /* require_module_name: keyword_or_name  */
                              {
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 39: /* require_module_name: '%' require_module_name  */
                                     {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 40: /* require_module_name: require_module_name '.' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 41: /* require_module_name: require_module_name '/' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 42: /* require_module: require_module_name is_public_module  */
                                                         {
        ast_requireModule(scanner,(yyvsp[-1].s),nullptr,(yyvsp[0].b),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 43: /* require_module: require_module_name "as" "name" is_public_module  */
                                                                              {
        ast_requireModule(scanner,(yyvsp[-3].s),(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 44: /* is_public_module: %empty  */
                    { (yyval.b) = false; }
    break;

  case 45: /* is_public_module: "public"  */
                    { (yyval.b) = true; }
    break;

  case 49: /* expect_error: "integer constant"  */
                   {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
    break;

  case 50: /* expect_error: "integer constant" ':' "integer constant"  */
                                      {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
    break;

  case 51: /* expression_label: "label" "integer constant" ':'  */
                                          {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }
    break;

  case 52: /* expression_goto: "goto" "label" "integer constant"  */
                                                {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }
    break;

  case 53: /* expression_goto: "goto" expr  */
                               {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 54: /* elif_or_static_elif: "elif"  */
                          { (yyval.b) = false; }
    break;

  case 55: /* elif_or_static_elif: "static_elif"  */
                          { (yyval.b) = true; }
    break;

  case 56: /* expression_else: %empty  */
                                                           { (yyval.pExpression) = nullptr; }
    break;

  case 57: /* expression_else: "else" expression_block  */
                                                           { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 58: /* expression_else: elif_or_static_elif expr expression_block expression_else  */
                                                                                          {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 59: /* if_or_static_if: "if"  */
                        { (yyval.b) = false; }
    break;

  case 60: /* if_or_static_if: "static_if"  */
                        { (yyval.b) = true; }
    break;

  case 61: /* expression_else_one_liner: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 62: /* $@2: %empty  */
                      { yyextra->das_need_oxford_comma = true; }
    break;

  case 63: /* expression_else_one_liner: "else" $@2 expression_if_one_liner  */
                                                                                                 {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 64: /* expression_if_one_liner: expr  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 65: /* expression_if_one_liner: expression_return_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 66: /* expression_if_one_liner: expression_yield_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 67: /* expression_if_one_liner: expression_break  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 68: /* expression_if_one_liner: expression_continue  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 69: /* expression_if_then_else: if_or_static_if expr expression_block expression_else  */
                                                                                      {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 70: /* $@3: %empty  */
                                                     { yyextra->das_need_oxford_comma = true; }
    break;

  case 71: /* expression_if_then_else: expression_if_one_liner "if" $@3 expr expression_else_one_liner ';'  */
                                                                                                                                                   {
        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr(ast_wrapInBlock((yyvsp[-5].pExpression))),(yyvsp[-1].pExpression) ? ExpressionPtr(ast_wrapInBlock((yyvsp[-1].pExpression))) : nullptr);
    }
    break;

  case 72: /* expression_for_loop: "for" variable_name_with_pos_list "in" expr_list ';' expression_block  */
                                                                                                           {
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-4].pNameWithPosList),(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 73: /* expression_unsafe: "unsafe" expression_block  */
                                                 {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 74: /* expression_while_loop: "while" expr expression_block  */
                                                               {
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 75: /* expression_with: "with" expr expression_block  */
                                                         {
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
    break;

  case 76: /* $@4: %empty  */
                                        { yyextra->das_need_oxford_comma=true; }
    break;

  case 77: /* expression_with_alias: "assume" "name" '=' $@4 expr  */
                                                                                               {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-4])), *(yyvsp[-3].s), (yyvsp[0].pExpression) );
        delete (yyvsp[-3].s);
    }
    break;

  case 78: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 79: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 80: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 81: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 82: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 83: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 84: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 85: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 86: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 87: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 88: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 89: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 90: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 91: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 92: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 93: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 94: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 95: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 96: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 97: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 98: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
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
        if ( auto ann = findAnnotation(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
    break;

  case 103: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
                                                                                 {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[-3]));
        if ( auto ann = findAnnotation(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3]))) ) {
            (yyval.fa)->annotation = ann;
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
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 106: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
                                                                              {
        if ( !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation); }
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::And,(yyvsp[-2].fa),(yyvsp[0].fa));
    }
    break;

  case 107: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
                                                                            {
        if ( !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation); }
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Or,(yyvsp[-2].fa),(yyvsp[0].fa));
    }
    break;

  case 108: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
                                                                              {
        if ( !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation); }
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
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

  case 169: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 170: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 171: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 172: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 173: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 174: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 175: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 176: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 177: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 178: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 179: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 180: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 181: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 182: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 183: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 184: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 185: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 186: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 187: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 188: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 189: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 190: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 191: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 192: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 193: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 194: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 195: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 196: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 197: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 198: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 199: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 200: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 201: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 202: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 203: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 204: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
                                                                                {
        (yyvsp[0].pFuncDecl)->atDecl = tokRangeAt(scanner,(yylsp[-1]),(yylsp[0]));
        assignDefaultArguments((yyvsp[0].pFuncDecl));
        runFunctionAnnotations(scanner, (yyvsp[0].pFuncDecl), (yyvsp[-2].faList), tokAt(scanner,(yylsp[-2])));
        if ( (yyvsp[0].pFuncDecl)->isGeneric() ) {
            if ( !yyextra->g_Program->addGeneric((yyvsp[0].pFuncDecl)) ) {
                das_yyerror(scanner,"generic function is already defined " +
                    (yyvsp[0].pFuncDecl)->getMangledName(),(yyvsp[0].pFuncDecl)->at,
                        CompilationError::function_already_declared);
            }
        } else {
            if ( !yyextra->g_Program->addFunction((yyvsp[0].pFuncDecl)) ) {
                das_yyerror(scanner,"function is already defined " +
                    (yyvsp[0].pFuncDecl)->getMangledName(),(yyvsp[0].pFuncDecl)->at,
                        CompilationError::function_already_declared);
            }
        }
        (yyvsp[0].pFuncDecl)->delRef();
    }
    break;

  case 205: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 206: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 207: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 208: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 209: /* $@5: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 210: /* function_declaration: optional_public_or_private_function $@5 function_declaration_header expression_block  */
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

  case 211: /* expression_block: '{' expressions '}'  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 212: /* expression_block: '{' expressions '}' "finally" '{' expressions '}'  */
                                                                                          {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 213: /* expression_any: ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 214: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 215: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 216: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 217: /* expression_any: expr_assign ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 218: /* expression_any: expression_delete ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 219: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 220: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 221: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 222: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 223: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 224: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 225: /* expression_any: expression_break ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 226: /* expression_any: expression_continue ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 227: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 228: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 229: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 230: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 231: /* expression_any: expression_label ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 232: /* expression_any: expression_goto ';'  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 233: /* expression_any: "pass" ';'  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 234: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 235: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 236: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 237: /* expr_keyword: "keyword" expr expression_block  */
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

  case 238: /* $@6: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 239: /* $@7: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 240: /* expression_keyword: "keyword" '<' $@6 type_declaration_no_options '>' $@7 expr  */
                                                                                                                                               {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),*(yyvsp[-6].s));
        auto td = new ExprTypeDecl(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCall->arguments.push_back(ExpressionPtr(td));
        pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 241: /* expr_pipe: expr_assign " <|" expr_block  */
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
                das_yyerror(scanner,"can't pipe into [[ make structure ]]. it already has where closure",
                    tokAt(scanner,(yylsp[-1])),CompilationError::cant_pipe);
                delete (yyvsp[0].pExpression);
            } else {
                pMS->block = (yyvsp[0].pExpression);
            }
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        } else {
            das_yyerror(scanner,"can only pipe into function call or [[ make structure ]]",
                tokAt(scanner,(yylsp[-1])),CompilationError::cant_pipe);
            delete (yyvsp[0].pExpression);
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        }
    }
    break;

  case 242: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 243: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 244: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 245: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 246: /* name_in_namespace: "name" "::" "name"  */
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

  case 247: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 248: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 249: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 250: /* $@8: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 251: /* $@9: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 252: /* new_type_declaration: '<' $@8 type_declaration '>' $@9  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 253: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 254: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 255: /* expr_new: "new" new_type_declaration '(' ')'  */
                                                               {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
    break;

  case 256: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 257: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 258: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 259: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 260: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 261: /* expression_return_no_pipe: "return" expr  */
                                      {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 262: /* expression_return_no_pipe: "return" "<-" expr  */
                                             {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 263: /* expression_return: expression_return_no_pipe ';'  */
                                              {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 264: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 265: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 266: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 267: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 268: /* expression_yield: expression_yield_no_pipe ';'  */
                                             {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 269: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 270: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 271: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 272: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 273: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 274: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 275: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 276: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 277: /* $@10: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 278: /* $@11: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 279: /* expr_cast: "cast" '<' $@10 type_declaration_no_options '>' $@11 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 280: /* $@12: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 281: /* $@13: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 282: /* expr_cast: "upcast" '<' $@12 type_declaration_no_options '>' $@13 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 283: /* $@14: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 284: /* $@15: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 285: /* expr_cast: "reinterpret" '<' $@14 type_declaration_no_options '>' $@15 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 286: /* $@16: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 287: /* $@17: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 288: /* expr_type_decl: "type" '<' $@16 type_declaration '>' $@17  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 289: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 290: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 291: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" ';' "name" '>' expr ')'  */
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

  case 292: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 293: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 294: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 295: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 296: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 297: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 298: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 299: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 300: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 301: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 302: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 303: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
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

  case 307: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 308: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 309: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 310: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 311: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 312: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 313: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 314: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 315: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 316: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 317: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 318: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 319: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 320: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 321: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 322: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 323: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 324: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 325: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 326: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 327: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 328: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 329: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 330: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 331: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 332: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 333: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 334: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 335: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 336: /* expr_assign_pipe: expr '=' "@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 337: /* expr_assign_pipe: expr '=' "@@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 338: /* expr_assign_pipe: expr '=' "$ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 339: /* expr_assign_pipe: expr "<-" "@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 340: /* expr_assign_pipe: expr "<-" "@@ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 341: /* expr_assign_pipe: expr "<-" "$ <|" expr_block  */
                                                          { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 342: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 343: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 344: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 345: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 346: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 347: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 348: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 349: /* $@18: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 350: /* $@19: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 351: /* func_addr_expr: '@' '@' '<' $@18 type_declaration_no_options '>' $@19 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 352: /* $@20: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 353: /* $@21: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 354: /* func_addr_expr: '@' '@' '<' $@20 optional_function_argument_list optional_function_type '>' $@21 func_addr_name  */
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

  case 355: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 356: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 357: /* $@22: %empty  */
                               { yyextra->das_supress_errors=true; }
    break;

  case 358: /* $@23: %empty  */
                                                                           { yyextra->das_supress_errors=false; }
    break;

  case 359: /* expr_field: expr '.' $@22 error $@23  */
                                                                                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 360: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 361: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 362: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 363: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 364: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 365: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 366: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 367: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 368: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 369: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 370: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 371: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 372: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 373: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 374: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 375: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 376: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 377: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 378: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 379: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 380: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 381: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 382: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 383: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 384: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 385: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 386: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 387: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 388: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 389: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 390: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 391: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 392: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 393: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 394: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 395: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 396: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 397: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 398: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 399: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 400: /* expr: '(' expr ')'  */
                                                 { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 401: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 402: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 403: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 404: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 405: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 406: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 407: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 408: /* expr: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
    break;

  case 409: /* expr: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
    break;

  case 410: /* expr: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 411: /* expr: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 412: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 413: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 414: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 415: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 416: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 417: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 418: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 419: /* $@24: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 420: /* $@25: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 421: /* expr: expr "is" "type" '<' $@24 type_declaration_no_options '>' $@25  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 422: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 423: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 424: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 425: /* $@26: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 426: /* $@27: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 427: /* expr: expr "as" "type" '<' $@26 type_declaration '>' $@27  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 428: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 429: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 430: /* $@28: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 431: /* $@29: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 432: /* expr: expr '?' "as" "type" '<' $@28 type_declaration '>' $@29  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 433: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 434: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 435: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 436: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 437: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 438: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 439: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 440: /* expr: expr_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 441: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 442: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 443: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 444: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 445: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 446: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 447: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 448: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 449: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 450: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 451: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 452: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 453: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 454: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 455: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 456: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 457: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 458: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 459: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 460: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 461: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 462: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 463: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); }
    break;

  case 464: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 465: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 466: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 467: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 468: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 469: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 470: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 471: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 472: /* structure_variable_declaration: optional_field_annotation optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                 {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-3].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 473: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 474: /* $@30: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 475: /* struct_variable_declaration_list: struct_variable_declaration_list $@30 structure_variable_declaration ';'  */
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

  case 476: /* $@31: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 477: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@31 function_declaration_header ';'  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 478: /* $@32: %empty  */
                                                                                                                               {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 479: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_override optional_constant $@32 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-6]),(yylsp[0])),tokAt(scanner,(yylsp[-7])));
            }
    break;

  case 480: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' ';'  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 481: /* function_argument_declaration: optional_field_annotation variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
    break;

  case 482: /* function_argument_declaration: optional_field_annotation "var" variable_declaration  */
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
    break;

  case 483: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 484: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 485: /* function_argument_list: function_argument_list ';' function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 486: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 487: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 488: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 489: /* tuple_type_list: tuple_type_list ';' tuple_type  */
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 490: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 491: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 492: /* variant_type_list: variant_type_list ';' variant_type  */
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 493: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 494: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 495: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 496: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 497: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 498: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 499: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 500: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
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

  case 511: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options ';'  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 512: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr ';'  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 513: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 514: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr ';'  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 515: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 516: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 517: /* $@33: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 518: /* global_variable_declaration_list: global_variable_declaration_list $@33 optional_field_annotation let_variable_declaration  */
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

  case 524: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 525: /* $@34: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 526: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@34 optional_field_annotation let_variable_declaration  */
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

  case 527: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 528: /* enum_list: enum_list "name" ';'  */
                                     {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt(scanner,(yylsp[-1]))) ) {
            das_yyerror(scanner,"enumeration already declared " + *(yyvsp[-1].s), tokAt(scanner,(yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
    break;

  case 529: /* enum_list: enum_list "name" '=' expr ';'  */
                                                     {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt(scanner,(yylsp[-3]))) ) {
            das_yyerror(scanner,"enumeration value already declared " + *(yyvsp[-3].s), tokAt(scanner,(yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
    break;

  case 530: /* $@35: %empty  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 531: /* single_alias: "name" $@35 '=' type_declaration  */
                                  {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !yyextra->g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror(scanner,"type alias is already defined "+*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterAlias((yyvsp[-3].s)->c_str(),pubename);
        }
        delete (yyvsp[-3].s);
    }
    break;

  case 536: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 537: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 538: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 539: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 540: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name '{' enum_list '}'  */
                                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-3].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-6].faList),tokAt(scanner,(yylsp[-6])),(yyvsp[-4].b),(yyvsp[-3].pEnum),(yyvsp[-1].pEnum),Type::tInt);
    }
    break;

  case 541: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration '{' enum_list '}'  */
                                                                                                                                                                       {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-1].pEnum),(yyvsp[-3].type));
    }
    break;

  case 542: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 543: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 544: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 545: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 546: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 547: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 548: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 549: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 550: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 551: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 552: /* $@36: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 553: /* $@37: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 554: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@36 structure_name $@37 '{' struct_variable_declaration_list '}'  */
                                                                                                                                                   {
        if ( (yyvsp[-4].pStructure) ) {
            ast_structureDeclaration ( scanner, (yyvsp[-8].faList), tokAt(scanner,(yylsp[-7])), (yyvsp[-4].pStructure), tokAt(scanner,(yylsp[-4])), (yyvsp[-1].pVarDeclList) );
            if ( !yyextra->g_CommentReaders.empty() ) {
                auto tak = tokAt(scanner,(yylsp[-7]));
                for ( auto & crd : yyextra->g_CommentReaders ) crd->afterStructure((yyvsp[-4].pStructure),tak);
            }
        }
    }
    break;

  case 555: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 556: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 557: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 558: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 559: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 560: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 561: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 562: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 563: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 564: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 565: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 566: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 567: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 568: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 569: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 570: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 571: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 572: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 573: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 574: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 575: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 576: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 577: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 578: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 579: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 580: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 581: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 582: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 583: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 584: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 585: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 586: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 587: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 588: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 589: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 590: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 591: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 592: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 593: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 594: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 595: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 596: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 597: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 598: /* bitfield_bits: bitfield_bits ';' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 599: /* $@38: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 600: /* $@39: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 601: /* bitfield_type_declaration: "bitfield" '<' $@38 bitfield_bits '>' $@39  */
                                                                                                                             {
            (yyval.pTypeDecl) = new TypeDecl(Type::tBitfield);
            (yyval.pTypeDecl)->argNames = *(yyvsp[-2].pNameList);
            if ( (yyval.pTypeDecl)->argNames.size()>32 ) {
                das_yyerror(scanner,"only 32 different bits are allowed in a bitfield",tokAt(scanner,(yylsp[-5])),
                    CompilationError::invalid_type);
            }
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
            delete (yyvsp[-2].pNameList);
    }
    break;

  case 602: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 603: /* table_type_pair: type_declaration ';' type_declaration  */
                                                                          {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 604: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 605: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 606: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 607: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 608: /* type_declaration_no_options: type_declaration_no_options '[' expr ']'  */
                                                                    {
        int32_t dI = TypeDecl::dimConst;
        if ( (yyvsp[-1].pExpression)->rtti_isConstant() ) {                // note: this shortcut is here so we don`t get extra infer pass on every array
            auto cI = (ExprConst *) (yyvsp[-1].pExpression);
            auto bt = cI->baseType;
            if ( bt==Type::tInt || bt==Type::tUInt ) {
                dI = cast<int32_t>::to(cI->value);
            }
        }
        (yyvsp[-3].pTypeDecl)->dim.push_back(dI);
        (yyvsp[-3].pTypeDecl)->dimExpr.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
        (yyvsp[-3].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 609: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 610: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 611: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 612: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 613: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 614: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 615: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 616: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 617: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 618: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 619: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 620: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 621: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 622: /* $@40: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 623: /* $@41: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 624: /* type_declaration_no_options: "smart_ptr" '<' $@40 type_declaration '>' $@41  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 625: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 626: /* $@42: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 627: /* $@43: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 628: /* type_declaration_no_options: "array" '<' $@42 type_declaration '>' $@43  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 629: /* $@44: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 630: /* $@45: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 631: /* type_declaration_no_options: "table" '<' $@44 table_type_pair '>' $@45  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 632: /* $@46: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 633: /* $@47: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 634: /* type_declaration_no_options: "iterator" '<' $@46 type_declaration '>' $@47  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 635: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 636: /* $@48: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 637: /* $@49: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 638: /* type_declaration_no_options: "block" '<' $@48 type_declaration '>' $@49  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 639: /* $@50: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 640: /* $@51: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 641: /* type_declaration_no_options: "block" '<' $@50 optional_function_argument_list optional_function_type '>' $@51  */
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

  case 642: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 643: /* $@52: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 644: /* $@53: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 645: /* type_declaration_no_options: "function" '<' $@52 type_declaration '>' $@53  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 646: /* $@54: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 647: /* $@55: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 648: /* type_declaration_no_options: "function" '<' $@54 optional_function_argument_list optional_function_type '>' $@55  */
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

  case 649: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 650: /* $@56: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 651: /* $@57: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 652: /* type_declaration_no_options: "lambda" '<' $@56 type_declaration '>' $@57  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 653: /* $@58: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 654: /* $@59: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 655: /* type_declaration_no_options: "lambda" '<' $@58 optional_function_argument_list optional_function_type '>' $@59  */
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

  case 656: /* $@60: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 657: /* $@61: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 658: /* type_declaration_no_options: "tuple" '<' $@60 tuple_type_list '>' $@61  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 659: /* $@62: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 660: /* $@63: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 661: /* type_declaration_no_options: "variant" '<' $@62 variant_type_list '>' $@63  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 662: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 663: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 664: /* type_declaration: type_declaration '|' '#'  */
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

  case 665: /* $@64: %empty  */
                     { yyextra->das_need_oxford_comma=false; }
    break;

  case 666: /* $@65: %empty  */
                                                                           {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 667: /* variant_alias_declaration: "variant" $@64 "name" $@65 '{' variant_type_list ';' '}'  */
                                          {
        auto vtype = make_smart<TypeDecl>(Type::tVariant);
        vtype->alias = *(yyvsp[-5].s);
        vtype->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, vtype.get(), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        if ( !yyextra->g_Program->addAlias(vtype) ) {
            das_yyerror(scanner,"type alias is already defined "+*(yyvsp[-5].s),tokAt(scanner,(yylsp[-5])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-5]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariant((yyvsp[-5].s)->c_str(),atvname);
        }
        delete (yyvsp[-5].s);
    }
    break;

  case 668: /* $@66: %empty  */
                      { yyextra->das_need_oxford_comma=false; }
    break;

  case 669: /* bitfield_alias_declaration: "bitfield" $@66 "name" '{' bitfield_bits ';' '}'  */
                                                                                                            {
        auto btype = make_smart<TypeDecl>(Type::tBitfield);
        btype->alias = *(yyvsp[-4].s);
        btype->at = tokAt(scanner,(yylsp[-4]));
        btype->argNames = *(yyvsp[-2].pNameList);
        if ( btype->argNames.size()>32 ) {
            das_yyerror(scanner,"only 32 different bits are allowed in a bitfield",tokAt(scanner,(yylsp[-4])),
                CompilationError::invalid_type);
        }
        if ( !yyextra->g_Program->addAlias(btype) ) {
            das_yyerror(scanner,"type alias is already defined "+*(yyvsp[-4].s),tokAt(scanner,(yylsp[-4])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-4].s);
        delete (yyvsp[-2].pNameList);
    }
    break;

  case 670: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 671: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 672: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 673: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 674: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 675: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 676: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 677: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 678: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 679: /* make_struct_dim: make_struct_dim ';' make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 680: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 681: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 682: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block ']' ']'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 683: /* make_struct_decl: "[[" type_declaration_no_options optional_block ']' ']'  */
                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = msd;
    }
    break;

  case 684: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block ']' ']'  */
                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = msd;
    }
    break;

  case 685: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block ']' ']'  */
                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 686: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block '}' ']'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 687: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block '}' ']'  */
                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-7]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 688: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 689: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 690: /* make_tuple: make_tuple ',' expr  */
                                      {
        ExprMakeTuple * mt;
        if ( (yyvsp[-2].pExpression)->rtti_isMakeTuple() ) {
            mt = static_cast<ExprMakeTuple *>((yyvsp[-2].pExpression));
        } else {
            mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-2])));
            mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        }
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 691: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 692: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 693: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 694: /* make_dim: make_dim ';' make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 695: /* make_dim_decl: "[[" type_declaration_no_options make_dim ']' ']'  */
                                                                             {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 696: /* make_dim_decl: "[{" type_declaration_no_options make_dim '}' ']'  */
                                                                             {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
    break;

  case 697: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 698: /* make_table: make_table ';' make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 699: /* make_table_decl: "{{" make_table '}' '}'  */
                                              {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
    break;

  case 700: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 701: /* array_comprehension_where: ';' "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 702: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true);
    }
    break;

  case 703: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where '}' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),false);
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
  yytoken = yychar == DAS_YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
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

      if (yychar <= DAS_YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == DAS_YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, scanner);
          yychar = DAS_YYEMPTY;
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
  if (yychar != DAS_YYEMPTY)
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



void das_yyfatalerror ( DAS_YYLTYPE * lloc, yyscan_t scanner, const string & error, CompilationError cerr ) {
    yyextra->g_Program->error(error,"","",LineInfo(yyextra->g_FileAccessStack.back(),
        lloc->first_column,lloc->first_line,lloc->last_column,lloc->last_line),cerr);
}

void das_yyerror ( DAS_YYLTYPE * lloc, yyscan_t scanner, const string & error ) {
    if ( !yyextra->das_supress_errors ) {
        yyextra->g_Program->error(error,"","",LineInfo(yyextra->g_FileAccessStack.back(),
            lloc->first_column,lloc->first_line,lloc->last_column,lloc->last_line),
                CompilationError::syntax_error);
    }
}

LineInfo tokAt ( yyscan_t scanner, const struct DAS_YYLTYPE & li ) {
    return LineInfo(yyextra->g_FileAccessStack.back(),
        li.first_column,li.first_line,
        li.last_column,li.last_line);
}

LineInfo tokRangeAt ( yyscan_t scanner, const struct DAS_YYLTYPE & li, const struct DAS_YYLTYPE & lie ) {
    return LineInfo(yyextra->g_FileAccessStack.back(),
        li.first_column,li.first_line,
        lie.last_column,lie.last_line);
}


