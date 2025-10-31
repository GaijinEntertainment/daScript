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
    void das_accept_sequence ( yyscan_t yyscanner, const char * seq, size_t seqLen, int lineNo, FileInfo * info );

    namespace das { class Module; }
    void das_collect_keywords ( das::Module * mod, yyscan_t yyscanner );

    void das_strfmt ( yyscan_t yyscanner );

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
  YYSYMBOL_LBPIPE = 132,                   /* " <|"  */
  YYSYMBOL_LLPIPE = 133,                   /* "$ <|"  */
  YYSYMBOL_LAPIPE = 134,                   /* "@ <|"  */
  YYSYMBOL_LFPIPE = 135,                   /* "@@ <|"  */
  YYSYMBOL_RPIPE = 136,                    /* "|>"  */
  YYSYMBOL_CLONEEQU = 137,                 /* ":="  */
  YYSYMBOL_ROTL = 138,                     /* "<<<"  */
  YYSYMBOL_ROTR = 139,                     /* ">>>"  */
  YYSYMBOL_ROTLEQU = 140,                  /* "<<<="  */
  YYSYMBOL_ROTREQU = 141,                  /* ">>>="  */
  YYSYMBOL_MAPTO = 142,                    /* "=>"  */
  YYSYMBOL_COLCOL = 143,                   /* "::"  */
  YYSYMBOL_ANDAND = 144,                   /* "&&"  */
  YYSYMBOL_OROR = 145,                     /* "||"  */
  YYSYMBOL_XORXOR = 146,                   /* "^^"  */
  YYSYMBOL_ANDANDEQU = 147,                /* "&&="  */
  YYSYMBOL_OROREQU = 148,                  /* "||="  */
  YYSYMBOL_XORXOREQU = 149,                /* "^^="  */
  YYSYMBOL_DOTDOT = 150,                   /* ".."  */
  YYSYMBOL_MTAG_E = 151,                   /* "$$"  */
  YYSYMBOL_MTAG_I = 152,                   /* "$i"  */
  YYSYMBOL_MTAG_V = 153,                   /* "$v"  */
  YYSYMBOL_MTAG_B = 154,                   /* "$b"  */
  YYSYMBOL_MTAG_A = 155,                   /* "$a"  */
  YYSYMBOL_MTAG_T = 156,                   /* "$t"  */
  YYSYMBOL_MTAG_C = 157,                   /* "$c"  */
  YYSYMBOL_MTAG_F = 158,                   /* "$f"  */
  YYSYMBOL_MTAG_DOTDOTDOT = 159,           /* "..."  */
  YYSYMBOL_BRABRAB = 160,                  /* "[["  */
  YYSYMBOL_BRACBRB = 161,                  /* "[{"  */
  YYSYMBOL_CBRCBRB = 162,                  /* "{{"  */
  YYSYMBOL_OPEN_BRACE = 163,               /* "new scope"  */
  YYSYMBOL_CLOSE_BRACE = 164,              /* "close scope"  */
  YYSYMBOL_SEMICOLON = 165,                /* "end of line"  */
  YYSYMBOL_INTEGER = 166,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 167,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 168,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 169,    /* "unsigned long integer constant"  */
  YYSYMBOL_UNSIGNED_INT8 = 170,            /* "unsigned int8 constant"  */
  YYSYMBOL_DAS_FLOAT = 171,                /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 172,                   /* "double constant"  */
  YYSYMBOL_NAME = 173,                     /* "name"  */
  YYSYMBOL_KEYWORD = 174,                  /* "keyword"  */
  YYSYMBOL_TYPE_FUNCTION = 175,            /* "type function"  */
  YYSYMBOL_BEGIN_STRING = 176,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 177,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 178,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 179,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 180,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 181,          /* "}"  */
  YYSYMBOL_END_OF_READ = 182,              /* "end of failed eader macro"  */
  YYSYMBOL_183_begin_of_code_block_ = 183, /* "begin of code block"  */
  YYSYMBOL_184_end_of_code_block_ = 184,   /* "end of code block"  */
  YYSYMBOL_185_end_of_expression_ = 185,   /* "end of expression"  */
  YYSYMBOL_SEMICOLON_CUR_CUR = 186,        /* ";}}"  */
  YYSYMBOL_SEMICOLON_CUR_SQR = 187,        /* ";}]"  */
  YYSYMBOL_SEMICOLON_SQR_SQR = 188,        /* ";]]"  */
  YYSYMBOL_COMMA_SQR_SQR = 189,            /* ",]]"  */
  YYSYMBOL_COMMA_CUR_SQR = 190,            /* ",}]"  */
  YYSYMBOL_191_ = 191,                     /* ','  */
  YYSYMBOL_192_ = 192,                     /* '='  */
  YYSYMBOL_193_ = 193,                     /* '?'  */
  YYSYMBOL_194_ = 194,                     /* ':'  */
  YYSYMBOL_195_ = 195,                     /* '|'  */
  YYSYMBOL_196_ = 196,                     /* '^'  */
  YYSYMBOL_197_ = 197,                     /* '&'  */
  YYSYMBOL_198_ = 198,                     /* '<'  */
  YYSYMBOL_199_ = 199,                     /* '>'  */
  YYSYMBOL_200_ = 200,                     /* '-'  */
  YYSYMBOL_201_ = 201,                     /* '+'  */
  YYSYMBOL_202_ = 202,                     /* '*'  */
  YYSYMBOL_203_ = 203,                     /* '/'  */
  YYSYMBOL_204_ = 204,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 205,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 206,               /* UNARY_PLUS  */
  YYSYMBOL_207_ = 207,                     /* '~'  */
  YYSYMBOL_208_ = 208,                     /* '!'  */
  YYSYMBOL_PRE_INC = 209,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 210,                  /* PRE_DEC  */
  YYSYMBOL_POST_INC = 211,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 212,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 213,                    /* DEREF  */
  YYSYMBOL_214_ = 214,                     /* '.'  */
  YYSYMBOL_215_ = 215,                     /* '['  */
  YYSYMBOL_216_ = 216,                     /* ']'  */
  YYSYMBOL_217_ = 217,                     /* '('  */
  YYSYMBOL_218_ = 218,                     /* ')'  */
  YYSYMBOL_219_ = 219,                     /* '$'  */
  YYSYMBOL_220_ = 220,                     /* '@'  */
  YYSYMBOL_221_ = 221,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 222,                 /* $accept  */
  YYSYMBOL_program = 223,                  /* program  */
  YYSYMBOL_top_level_reader_macro = 224,   /* top_level_reader_macro  */
  YYSYMBOL_optional_public_or_private_module = 225, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 226,              /* module_name  */
  YYSYMBOL_optional_not_required = 227,    /* optional_not_required  */
  YYSYMBOL_module_declaration = 228,       /* module_declaration  */
  YYSYMBOL_character_sequence = 229,       /* character_sequence  */
  YYSYMBOL_string_constant = 230,          /* string_constant  */
  YYSYMBOL_format_string = 231,            /* format_string  */
  YYSYMBOL_optional_format_string = 232,   /* optional_format_string  */
  YYSYMBOL_233_1 = 233,                    /* $@1  */
  YYSYMBOL_string_builder_body = 234,      /* string_builder_body  */
  YYSYMBOL_string_builder = 235,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 236, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 237,              /* expr_reader  */
  YYSYMBOL_238_2 = 238,                    /* $@2  */
  YYSYMBOL_options_declaration = 239,      /* options_declaration  */
  YYSYMBOL_require_declaration = 240,      /* require_declaration  */
  YYSYMBOL_keyword_or_name = 241,          /* keyword_or_name  */
  YYSYMBOL_require_module_name = 242,      /* require_module_name  */
  YYSYMBOL_require_module = 243,           /* require_module  */
  YYSYMBOL_is_public_module = 244,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 245,       /* expect_declaration  */
  YYSYMBOL_expect_list = 246,              /* expect_list  */
  YYSYMBOL_expect_error = 247,             /* expect_error  */
  YYSYMBOL_expression_label = 248,         /* expression_label  */
  YYSYMBOL_expression_goto = 249,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 250,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 251,          /* expression_else  */
  YYSYMBOL_semicolon = 252,                /* semicolon  */
  YYSYMBOL_if_or_static_if = 253,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 254, /* expression_else_one_liner  */
  YYSYMBOL_255_3 = 255,                    /* $@3  */
  YYSYMBOL_expression_if_one_liner = 256,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 257,  /* expression_if_then_else  */
  YYSYMBOL_258_4 = 258,                    /* $@4  */
  YYSYMBOL_expression_for_loop = 259,      /* expression_for_loop  */
  YYSYMBOL_260_5 = 260,                    /* $@5  */
  YYSYMBOL_expression_unsafe = 261,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 262,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 263,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 264,    /* expression_with_alias  */
  YYSYMBOL_265_6 = 265,                    /* $@6  */
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
  YYSYMBOL_optional_function_argument_list = 277, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 278,   /* optional_function_type  */
  YYSYMBOL_function_name = 279,            /* function_name  */
  YYSYMBOL_global_function_declaration = 280, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 281, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 282, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 283,     /* function_declaration  */
  YYSYMBOL_284_7 = 284,                    /* $@7  */
  YYSYMBOL_open_block = 285,               /* open_block  */
  YYSYMBOL_close_block = 286,              /* close_block  */
  YYSYMBOL_expression_block = 287,         /* expression_block  */
  YYSYMBOL_expr_call_pipe = 288,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 289,           /* expression_any  */
  YYSYMBOL_expressions = 290,              /* expressions  */
  YYSYMBOL_expr_keyword = 291,             /* expr_keyword  */
  YYSYMBOL_optional_expr_list = 292,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_list_in_braces = 293, /* optional_expr_list_in_braces  */
  YYSYMBOL_optional_expr_map_tuple_list = 294, /* optional_expr_map_tuple_list  */
  YYSYMBOL_type_declaration_no_options_list = 295, /* type_declaration_no_options_list  */
  YYSYMBOL_expression_keyword = 296,       /* expression_keyword  */
  YYSYMBOL_297_8 = 297,                    /* $@8  */
  YYSYMBOL_298_9 = 298,                    /* $@9  */
  YYSYMBOL_299_10 = 299,                   /* $@10  */
  YYSYMBOL_300_11 = 300,                   /* $@11  */
  YYSYMBOL_expr_pipe = 301,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 302,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 303,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 304,     /* new_type_declaration  */
  YYSYMBOL_305_12 = 305,                   /* $@12  */
  YYSYMBOL_306_13 = 306,                   /* $@13  */
  YYSYMBOL_expr_new = 307,                 /* expr_new  */
  YYSYMBOL_expression_break = 308,         /* expression_break  */
  YYSYMBOL_expression_continue = 309,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 310, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 311,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 312, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 313,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 314,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 315,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 316,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 317,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 318,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 319, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 320,           /* expression_let  */
  YYSYMBOL_expr_cast = 321,                /* expr_cast  */
  YYSYMBOL_322_14 = 322,                   /* $@14  */
  YYSYMBOL_323_15 = 323,                   /* $@15  */
  YYSYMBOL_324_16 = 324,                   /* $@16  */
  YYSYMBOL_325_17 = 325,                   /* $@17  */
  YYSYMBOL_326_18 = 326,                   /* $@18  */
  YYSYMBOL_327_19 = 327,                   /* $@19  */
  YYSYMBOL_expr_type_decl = 328,           /* expr_type_decl  */
  YYSYMBOL_329_20 = 329,                   /* $@20  */
  YYSYMBOL_330_21 = 330,                   /* $@21  */
  YYSYMBOL_expr_type_info = 331,           /* expr_type_info  */
  YYSYMBOL_expr_list = 332,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 333,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 334,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 335,            /* capture_entry  */
  YYSYMBOL_capture_list = 336,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 337,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 338,               /* expr_block  */
  YYSYMBOL_expr_full_block = 339,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 340, /* expr_full_block_assumed_piped  */
  YYSYMBOL_341_22 = 341,                   /* $@22  */
  YYSYMBOL_expr_numeric_const = 342,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 343,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe_right = 344,   /* expr_assign_pipe_right  */
  YYSYMBOL_expr_assign_pipe = 345,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 346,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 347,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 348,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 349,           /* func_addr_expr  */
  YYSYMBOL_350_23 = 350,                   /* $@23  */
  YYSYMBOL_351_24 = 351,                   /* $@24  */
  YYSYMBOL_352_25 = 352,                   /* $@25  */
  YYSYMBOL_353_26 = 353,                   /* $@26  */
  YYSYMBOL_expr_field = 354,               /* expr_field  */
  YYSYMBOL_355_27 = 355,                   /* $@27  */
  YYSYMBOL_356_28 = 356,                   /* $@28  */
  YYSYMBOL_expr_call = 357,                /* expr_call  */
  YYSYMBOL_expr = 358,                     /* expr  */
  YYSYMBOL_359_29 = 359,                   /* $@29  */
  YYSYMBOL_360_30 = 360,                   /* $@30  */
  YYSYMBOL_361_31 = 361,                   /* $@31  */
  YYSYMBOL_362_32 = 362,                   /* $@32  */
  YYSYMBOL_363_33 = 363,                   /* $@33  */
  YYSYMBOL_364_34 = 364,                   /* $@34  */
  YYSYMBOL_expr_mtag = 365,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 366, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 367,        /* optional_override  */
  YYSYMBOL_optional_constant = 368,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 369, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 370, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 371, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 372, /* struct_variable_declaration_list  */
  YYSYMBOL_373_35 = 373,                   /* $@35  */
  YYSYMBOL_374_36 = 374,                   /* $@36  */
  YYSYMBOL_375_37 = 375,                   /* $@37  */
  YYSYMBOL_function_argument_declaration_no_type = 376, /* function_argument_declaration_no_type  */
  YYSYMBOL_function_argument_declaration_type = 377, /* function_argument_declaration_type  */
  YYSYMBOL_function_argument_list = 378,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 379,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 380,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 381,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 382,             /* variant_type  */
  YYSYMBOL_variant_type_list = 383,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 384,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 385,             /* copy_or_move  */
  YYSYMBOL_variable_declaration_no_type = 386, /* variable_declaration_no_type  */
  YYSYMBOL_variable_declaration_type = 387, /* variable_declaration_type  */
  YYSYMBOL_variable_declaration = 388,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 389,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 390,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 391, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 392, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 393, /* global_variable_declaration_list  */
  YYSYMBOL_394_38 = 394,                   /* $@38  */
  YYSYMBOL_optional_shared = 395,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 396, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 397,               /* global_let  */
  YYSYMBOL_398_39 = 398,                   /* $@39  */
  YYSYMBOL_enum_list = 399,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 400, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 401,             /* single_alias  */
  YYSYMBOL_402_40 = 402,                   /* $@40  */
  YYSYMBOL_alias_list = 403,               /* alias_list  */
  YYSYMBOL_alias_declaration = 404,        /* alias_declaration  */
  YYSYMBOL_405_41 = 405,                   /* $@41  */
  YYSYMBOL_optional_public_or_private_enum = 406, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 407,                /* enum_name  */
  YYSYMBOL_enum_declaration = 408,         /* enum_declaration  */
  YYSYMBOL_409_42 = 409,                   /* $@42  */
  YYSYMBOL_410_43 = 410,                   /* $@43  */
  YYSYMBOL_411_44 = 411,                   /* $@44  */
  YYSYMBOL_412_45 = 412,                   /* $@45  */
  YYSYMBOL_optional_structure_parent = 413, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 414,          /* optional_sealed  */
  YYSYMBOL_structure_name = 415,           /* structure_name  */
  YYSYMBOL_class_or_struct = 416,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 417, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 418, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 419,    /* structure_declaration  */
  YYSYMBOL_420_46 = 420,                   /* $@46  */
  YYSYMBOL_421_47 = 421,                   /* $@47  */
  YYSYMBOL_variable_name_with_pos_list = 422, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 423,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 424, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 425, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 426,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 427,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 428,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_basic_type_declaration = 429, /* bitfield_basic_type_declaration  */
  YYSYMBOL_bitfield_type_declaration = 430, /* bitfield_type_declaration  */
  YYSYMBOL_431_48 = 431,                   /* $@48  */
  YYSYMBOL_432_49 = 432,                   /* $@49  */
  YYSYMBOL_c_or_s = 433,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 434,          /* table_type_pair  */
  YYSYMBOL_dim_list = 435,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 436, /* type_declaration_no_options  */
  YYSYMBOL_437_50 = 437,                   /* $@50  */
  YYSYMBOL_438_51 = 438,                   /* $@51  */
  YYSYMBOL_439_52 = 439,                   /* $@52  */
  YYSYMBOL_440_53 = 440,                   /* $@53  */
  YYSYMBOL_441_54 = 441,                   /* $@54  */
  YYSYMBOL_442_55 = 442,                   /* $@55  */
  YYSYMBOL_443_56 = 443,                   /* $@56  */
  YYSYMBOL_444_57 = 444,                   /* $@57  */
  YYSYMBOL_445_58 = 445,                   /* $@58  */
  YYSYMBOL_446_59 = 446,                   /* $@59  */
  YYSYMBOL_447_60 = 447,                   /* $@60  */
  YYSYMBOL_448_61 = 448,                   /* $@61  */
  YYSYMBOL_449_62 = 449,                   /* $@62  */
  YYSYMBOL_450_63 = 450,                   /* $@63  */
  YYSYMBOL_451_64 = 451,                   /* $@64  */
  YYSYMBOL_452_65 = 452,                   /* $@65  */
  YYSYMBOL_453_66 = 453,                   /* $@66  */
  YYSYMBOL_454_67 = 454,                   /* $@67  */
  YYSYMBOL_455_68 = 455,                   /* $@68  */
  YYSYMBOL_456_69 = 456,                   /* $@69  */
  YYSYMBOL_457_70 = 457,                   /* $@70  */
  YYSYMBOL_458_71 = 458,                   /* $@71  */
  YYSYMBOL_459_72 = 459,                   /* $@72  */
  YYSYMBOL_460_73 = 460,                   /* $@73  */
  YYSYMBOL_461_74 = 461,                   /* $@74  */
  YYSYMBOL_462_75 = 462,                   /* $@75  */
  YYSYMBOL_463_76 = 463,                   /* $@76  */
  YYSYMBOL_type_declaration = 464,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 465,  /* tuple_alias_declaration  */
  YYSYMBOL_466_77 = 466,                   /* $@77  */
  YYSYMBOL_467_78 = 467,                   /* $@78  */
  YYSYMBOL_468_79 = 468,                   /* $@79  */
  YYSYMBOL_469_80 = 469,                   /* $@80  */
  YYSYMBOL_variant_alias_declaration = 470, /* variant_alias_declaration  */
  YYSYMBOL_471_81 = 471,                   /* $@81  */
  YYSYMBOL_472_82 = 472,                   /* $@82  */
  YYSYMBOL_473_83 = 473,                   /* $@83  */
  YYSYMBOL_474_84 = 474,                   /* $@84  */
  YYSYMBOL_bitfield_alias_declaration = 475, /* bitfield_alias_declaration  */
  YYSYMBOL_476_85 = 476,                   /* $@85  */
  YYSYMBOL_477_86 = 477,                   /* $@86  */
  YYSYMBOL_478_87 = 478,                   /* $@87  */
  YYSYMBOL_479_88 = 479,                   /* $@88  */
  YYSYMBOL_make_decl = 480,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 481,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 482,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 483,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim = 484,          /* make_struct_dim  */
  YYSYMBOL_make_struct_dim_list = 485,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 486,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 487, /* optional_make_struct_dim_decl  */
  YYSYMBOL_optional_block = 488,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_cur_cur = 489, /* optional_trailing_semicolon_cur_cur  */
  YYSYMBOL_optional_trailing_semicolon_cur_sqr = 490, /* optional_trailing_semicolon_cur_sqr  */
  YYSYMBOL_optional_trailing_semicolon_sqr_sqr = 491, /* optional_trailing_semicolon_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_sqr_sqr = 492, /* optional_trailing_delim_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_cur_sqr = 493, /* optional_trailing_delim_cur_sqr  */
  YYSYMBOL_use_initializer = 494,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 495,         /* make_struct_decl  */
  YYSYMBOL_496_89 = 496,                   /* $@89  */
  YYSYMBOL_497_90 = 497,                   /* $@90  */
  YYSYMBOL_498_91 = 498,                   /* $@91  */
  YYSYMBOL_499_92 = 499,                   /* $@92  */
  YYSYMBOL_500_93 = 500,                   /* $@93  */
  YYSYMBOL_501_94 = 501,                   /* $@94  */
  YYSYMBOL_502_95 = 502,                   /* $@95  */
  YYSYMBOL_503_96 = 503,                   /* $@96  */
  YYSYMBOL_make_tuple = 504,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 505,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 506,          /* make_tuple_call  */
  YYSYMBOL_507_97 = 507,                   /* $@97  */
  YYSYMBOL_508_98 = 508,                   /* $@98  */
  YYSYMBOL_make_dim = 509,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 510,            /* make_dim_decl  */
  YYSYMBOL_511_99 = 511,                   /* $@99  */
  YYSYMBOL_512_100 = 512,                  /* $@100  */
  YYSYMBOL_513_101 = 513,                  /* $@101  */
  YYSYMBOL_514_102 = 514,                  /* $@102  */
  YYSYMBOL_515_103 = 515,                  /* $@103  */
  YYSYMBOL_516_104 = 516,                  /* $@104  */
  YYSYMBOL_517_105 = 517,                  /* $@105  */
  YYSYMBOL_518_106 = 518,                  /* $@106  */
  YYSYMBOL_519_107 = 519,                  /* $@107  */
  YYSYMBOL_520_108 = 520,                  /* $@108  */
  YYSYMBOL_make_table = 521,               /* make_table  */
  YYSYMBOL_expr_map_tuple_list = 522,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 523,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 524, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 525,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 526       /* array_comprehension  */
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
#define YYLAST   15030

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  222
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  305
/* YYNRULES -- Number of rules.  */
#define YYNRULES  938
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1753

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   449


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
       2,     2,     2,   208,     2,   221,   219,   204,   197,     2,
     217,   218,   202,   201,   191,   200,   214,   203,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   194,   185,
     198,   192,   199,   193,   220,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   215,     2,   216,   196,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   183,   195,   184,   207,     2,     2,     2,
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
     175,   176,   177,   178,   179,   180,   181,   182,   186,   187,
     188,   189,   190,   205,   206,   209,   210,   211,   212,   213
};

#if DAS_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   565,   565,   566,   571,   572,   573,   574,   575,   576,
     577,   578,   579,   580,   581,   582,   583,   587,   593,   594,
     595,   599,   600,   604,   605,   609,   628,   629,   630,   631,
     635,   636,   640,   641,   645,   646,   646,   650,   655,   664,
     679,   695,   700,   708,   708,   747,   773,   777,   778,   779,
     783,   786,   790,   796,   805,   808,   814,   815,   819,   823,
     824,   828,   831,   837,   843,   846,   852,   853,   857,   858,
     859,   868,   869,   873,   874,   878,   879,   879,   885,   886,
     887,   888,   889,   893,   899,   899,   905,   905,   911,   919,
     929,   938,   938,   942,   948,   949,   950,   951,   952,   953,
     957,   962,   970,   971,   972,   976,   977,   978,   979,   980,
     981,   982,   983,   989,   992,   998,  1001,  1004,  1010,  1011,
    1012,  1013,  1017,  1034,  1056,  1059,  1069,  1084,  1099,  1114,
    1117,  1124,  1128,  1135,  1136,  1140,  1141,  1142,  1146,  1149,
    1156,  1160,  1161,  1162,  1163,  1164,  1165,  1166,  1167,  1168,
    1169,  1170,  1171,  1172,  1173,  1174,  1175,  1176,  1177,  1178,
    1179,  1180,  1181,  1182,  1183,  1184,  1185,  1186,  1187,  1188,
    1189,  1190,  1191,  1192,  1193,  1194,  1195,  1196,  1197,  1198,
    1199,  1200,  1201,  1202,  1203,  1204,  1205,  1206,  1207,  1208,
    1209,  1210,  1211,  1212,  1213,  1214,  1215,  1216,  1217,  1218,
    1219,  1220,  1221,  1222,  1223,  1224,  1225,  1226,  1227,  1228,
    1229,  1230,  1231,  1232,  1233,  1234,  1235,  1236,  1237,  1238,
    1239,  1240,  1241,  1242,  1243,  1244,  1245,  1246,  1247,  1248,
    1249,  1250,  1251,  1252,  1253,  1254,  1259,  1277,  1278,  1279,
    1283,  1289,  1289,  1307,  1308,  1311,  1312,  1315,  1319,  1330,
    1339,  1348,  1354,  1355,  1356,  1357,  1358,  1359,  1360,  1361,
    1362,  1363,  1364,  1365,  1366,  1367,  1368,  1369,  1370,  1371,
    1372,  1373,  1374,  1378,  1383,  1389,  1395,  1406,  1407,  1411,
    1412,  1416,  1417,  1421,  1425,  1432,  1432,  1432,  1438,  1438,
    1438,  1447,  1481,  1484,  1487,  1490,  1496,  1497,  1508,  1512,
    1515,  1523,  1523,  1523,  1526,  1532,  1535,  1539,  1543,  1550,
    1557,  1563,  1567,  1571,  1574,  1577,  1585,  1588,  1591,  1599,
    1602,  1610,  1613,  1616,  1624,  1630,  1631,  1632,  1636,  1637,
    1641,  1642,  1646,  1651,  1659,  1665,  1671,  1677,  1683,  1692,
    1701,  1710,  1722,  1725,  1731,  1731,  1731,  1734,  1734,  1734,
    1739,  1739,  1739,  1747,  1747,  1747,  1753,  1763,  1774,  1787,
    1797,  1808,  1823,  1826,  1832,  1833,  1840,  1851,  1852,  1853,
    1857,  1858,  1859,  1860,  1861,  1865,  1870,  1878,  1879,  1880,
    1884,  1889,  1896,  1903,  1903,  1912,  1913,  1914,  1915,  1916,
    1917,  1918,  1922,  1923,  1924,  1925,  1926,  1927,  1928,  1929,
    1930,  1931,  1932,  1933,  1934,  1935,  1936,  1937,  1938,  1939,
    1940,  1944,  1945,  1946,  1947,  1952,  1953,  1954,  1955,  1956,
    1957,  1958,  1959,  1960,  1961,  1962,  1963,  1964,  1965,  1966,
    1967,  1968,  1973,  1980,  1992,  1997,  2007,  2011,  2018,  2021,
    2021,  2021,  2026,  2026,  2026,  2039,  2043,  2047,  2052,  2059,
    2068,  2073,  2080,  2080,  2080,  2087,  2091,  2100,  2108,  2116,
    2120,  2123,  2129,  2130,  2131,  2132,  2133,  2134,  2135,  2136,
    2137,  2138,  2139,  2140,  2141,  2142,  2143,  2144,  2145,  2146,
    2147,  2148,  2149,  2150,  2151,  2152,  2153,  2154,  2155,  2156,
    2157,  2158,  2159,  2160,  2161,  2162,  2163,  2164,  2170,  2171,
    2172,  2173,  2174,  2187,  2196,  2197,  2198,  2199,  2200,  2201,
    2202,  2203,  2204,  2205,  2206,  2207,  2210,  2213,  2214,  2217,
    2217,  2217,  2220,  2225,  2229,  2233,  2233,  2233,  2238,  2241,
    2245,  2245,  2245,  2250,  2253,  2254,  2255,  2256,  2257,  2258,
    2259,  2260,  2261,  2263,  2267,  2268,  2273,  2277,  2278,  2279,
    2280,  2281,  2282,  2283,  2287,  2291,  2295,  2299,  2303,  2307,
    2311,  2315,  2319,  2326,  2327,  2328,  2332,  2333,  2334,  2338,
    2339,  2343,  2344,  2345,  2349,  2350,  2354,  2365,  2368,  2371,
    2371,  2390,  2389,  2405,  2404,  2418,  2427,  2439,  2448,  2458,
    2459,  2460,  2461,  2462,  2466,  2469,  2478,  2479,  2483,  2486,
    2489,  2505,  2514,  2515,  2519,  2522,  2525,  2539,  2540,  2544,
    2550,  2556,  2565,  2568,  2575,  2578,  2584,  2585,  2586,  2590,
    2591,  2595,  2602,  2607,  2616,  2622,  2633,  2636,  2641,  2646,
    2654,  2665,  2668,  2671,  2671,  2691,  2692,  2696,  2697,  2698,
    2702,  2705,  2705,  2724,  2727,  2730,  2745,  2764,  2765,  2766,
    2771,  2771,  2797,  2798,  2802,  2803,  2803,  2807,  2808,  2809,
    2813,  2823,  2828,  2823,  2840,  2845,  2840,  2860,  2861,  2865,
    2866,  2870,  2876,  2877,  2878,  2879,  2883,  2884,  2885,  2889,
    2892,  2898,  2903,  2898,  2923,  2930,  2935,  2944,  2950,  2961,
    2962,  2963,  2964,  2965,  2966,  2967,  2968,  2969,  2970,  2971,
    2972,  2973,  2974,  2975,  2976,  2977,  2978,  2979,  2980,  2981,
    2982,  2983,  2984,  2985,  2986,  2987,  2991,  2992,  2993,  2994,
    2995,  2996,  2997,  2998,  3002,  3013,  3017,  3024,  3036,  3043,
    3052,  3057,  3060,  3070,  3083,  3084,  3085,  3086,  3087,  3091,
    3091,  3091,  3105,  3106,  3110,  3114,  3121,  3125,  3132,  3133,
    3134,  3135,  3136,  3151,  3157,  3157,  3157,  3161,  3166,  3173,
    3173,  3180,  3184,  3188,  3193,  3198,  3203,  3208,  3212,  3216,
    3221,  3225,  3229,  3234,  3234,  3234,  3240,  3247,  3247,  3247,
    3252,  3252,  3252,  3258,  3258,  3258,  3263,  3268,  3268,  3268,
    3273,  3273,  3273,  3282,  3287,  3287,  3287,  3292,  3292,  3292,
    3301,  3306,  3306,  3306,  3311,  3311,  3311,  3320,  3320,  3320,
    3326,  3326,  3326,  3335,  3338,  3349,  3365,  3365,  3370,  3375,
    3365,  3400,  3400,  3405,  3411,  3400,  3436,  3436,  3441,  3446,
    3436,  3486,  3487,  3488,  3489,  3490,  3494,  3501,  3508,  3514,
    3520,  3527,  3534,  3540,  3549,  3552,  3558,  3566,  3571,  3578,
    3583,  3590,  3595,  3601,  3602,  3606,  3607,  3612,  3613,  3617,
    3618,  3622,  3623,  3627,  3628,  3629,  3633,  3634,  3635,  3639,
    3640,  3644,  3650,  3657,  3665,  3672,  3680,  3689,  3689,  3689,
    3697,  3697,  3697,  3704,  3704,  3704,  3715,  3715,  3715,  3726,
    3729,  3735,  3749,  3755,  3761,  3767,  3767,  3767,  3781,  3786,
    3793,  3812,  3817,  3824,  3824,  3824,  3834,  3834,  3834,  3848,
    3848,  3848,  3862,  3871,  3871,  3871,  3891,  3898,  3898,  3898,
    3908,  3913,  3920,  3923,  3929,  3948,  3957,  3965,  3985,  4010,
    4011,  4015,  4016,  4021,  4024,  4027,  4030,  4033,  4036
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
  "\"<-\"", "\"??\"", "\"?.\"", "\"?[\"", "\"<|\"", "\" <|\"", "\"$ <|\"",
  "\"@ <|\"", "\"@@ <|\"", "\"|>\"", "\":=\"", "\"<<<\"", "\">>>\"",
  "\"<<<=\"", "\">>>=\"", "\"=>\"", "\"::\"", "\"&&\"", "\"||\"", "\"^^\"",
  "\"&&=\"", "\"||=\"", "\"^^=\"", "\"..\"", "\"$$\"", "\"$i\"", "\"$v\"",
  "\"$b\"", "\"$a\"", "\"$t\"", "\"$c\"", "\"$f\"", "\"...\"", "\"[[\"",
  "\"[{\"", "\"{{\"", "\"new scope\"", "\"close scope\"",
  "\"end of line\"", "\"integer constant\"", "\"long integer constant\"",
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
  "optional_public_or_private_module", "module_name",
  "optional_not_required", "module_declaration", "character_sequence",
  "string_constant", "format_string", "optional_format_string", "$@1",
  "string_builder_body", "string_builder", "reader_character_sequence",
  "expr_reader", "$@2", "options_declaration", "require_declaration",
  "keyword_or_name", "require_module_name", "require_module",
  "is_public_module", "expect_declaration", "expect_list", "expect_error",
  "expression_label", "expression_goto", "elif_or_static_elif",
  "expression_else", "semicolon", "if_or_static_if",
  "expression_else_one_liner", "$@3", "expression_if_one_liner",
  "expression_if_then_else", "$@4", "expression_for_loop", "$@5",
  "expression_unsafe", "expression_while_loop", "expression_with",
  "expression_with_alias", "$@6", "annotation_argument_value",
  "annotation_argument_value_list", "annotation_argument_name",
  "annotation_argument", "annotation_argument_list",
  "metadata_argument_list", "annotation_declaration_name",
  "annotation_declaration_basic", "annotation_declaration",
  "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "global_function_declaration",
  "optional_public_or_private_function", "function_declaration_header",
  "function_declaration", "$@7", "open_block", "close_block",
  "expression_block", "expr_call_pipe", "expression_any", "expressions",
  "expr_keyword", "optional_expr_list", "optional_expr_list_in_braces",
  "optional_expr_map_tuple_list", "type_declaration_no_options_list",
  "expression_keyword", "$@8", "$@9", "$@10", "$@11", "expr_pipe",
  "name_in_namespace", "expression_delete", "new_type_declaration", "$@12",
  "$@13", "expr_new", "expression_break", "expression_continue",
  "expression_return_no_pipe", "expression_return",
  "expression_yield_no_pipe", "expression_yield", "expression_try_catch",
  "kwd_let_var_or_nothing", "kwd_let", "optional_in_scope",
  "tuple_expansion", "tuple_expansion_variable_declaration",
  "expression_let", "expr_cast", "$@14", "$@15", "$@16", "$@17", "$@18",
  "$@19", "expr_type_decl", "$@20", "$@21", "expr_type_info", "expr_list",
  "block_or_simple_block", "block_or_lambda", "capture_entry",
  "capture_list", "optional_capture_list", "expr_block", "expr_full_block",
  "expr_full_block_assumed_piped", "$@22", "expr_numeric_const",
  "expr_assign", "expr_assign_pipe_right", "expr_assign_pipe",
  "expr_named_call", "expr_method_call", "func_addr_name",
  "func_addr_expr", "$@23", "$@24", "$@25", "$@26", "expr_field", "$@27",
  "$@28", "expr_call", "expr", "$@29", "$@30", "$@31", "$@32", "$@33",
  "$@34", "expr_mtag", "optional_field_annotation", "optional_override",
  "optional_constant", "optional_public_or_private_member_variable",
  "optional_static_member_variable", "structure_variable_declaration",
  "struct_variable_declaration_list", "$@35", "$@36", "$@37",
  "function_argument_declaration_no_type",
  "function_argument_declaration_type", "function_argument_list",
  "tuple_type", "tuple_type_list", "tuple_alias_type_list", "variant_type",
  "variant_type_list", "variant_alias_type_list", "copy_or_move",
  "variable_declaration_no_type", "variable_declaration_type",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list", "let_variable_declaration",
  "global_variable_declaration_list", "$@38", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@39", "enum_list",
  "optional_public_or_private_alias", "single_alias", "$@40", "alias_list",
  "alias_declaration", "$@41", "optional_public_or_private_enum",
  "enum_name", "enum_declaration", "$@42", "$@43", "$@44", "$@45",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "optional_struct_variable_declaration_list", "structure_declaration",
  "$@46", "$@47", "variable_name_with_pos_list", "basic_type_declaration",
  "enum_basic_type_declaration", "structure_type_declaration",
  "auto_type_declaration", "bitfield_bits", "bitfield_alias_bits",
  "bitfield_basic_type_declaration", "bitfield_type_declaration", "$@48",
  "$@49", "c_or_s", "table_type_pair", "dim_list",
  "type_declaration_no_options", "$@50", "$@51", "$@52", "$@53", "$@54",
  "$@55", "$@56", "$@57", "$@58", "$@59", "$@60", "$@61", "$@62", "$@63",
  "$@64", "$@65", "$@66", "$@67", "$@68", "$@69", "$@70", "$@71", "$@72",
  "$@73", "$@74", "$@75", "$@76", "type_declaration",
  "tuple_alias_declaration", "$@77", "$@78", "$@79", "$@80",
  "variant_alias_declaration", "$@81", "$@82", "$@83", "$@84",
  "bitfield_alias_declaration", "$@85", "$@86", "$@87", "$@88",
  "make_decl", "make_struct_fields", "make_variant_dim",
  "make_struct_single", "make_struct_dim", "make_struct_dim_list",
  "make_struct_dim_decl", "optional_make_struct_dim_decl",
  "optional_block", "optional_trailing_semicolon_cur_cur",
  "optional_trailing_semicolon_cur_sqr",
  "optional_trailing_semicolon_sqr_sqr", "optional_trailing_delim_sqr_sqr",
  "optional_trailing_delim_cur_sqr", "use_initializer", "make_struct_decl",
  "$@89", "$@90", "$@91", "$@92", "$@93", "$@94", "$@95", "$@96",
  "make_tuple", "make_map_tuple", "make_tuple_call", "$@97", "$@98",
  "make_dim", "make_dim_decl", "$@99", "$@100", "$@101", "$@102", "$@103",
  "$@104", "$@105", "$@106", "$@107", "$@108", "make_table",
  "expr_map_tuple_list", "make_table_decl", "array_comprehension_where",
  "optional_comma", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1551)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-805)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1551,   126, -1551, -1551,    50,   -31,   172,   379, -1551,   -68,
     296,   296,   296, -1551, -1551,   -37,    16, -1551, -1551,   533,
   -1551, -1551, -1551, -1551,   185, -1551,   107, -1551, -1551, -1551,
   -1551, -1551, -1551, -1551, -1551, -1551, -1551,   227, -1551,    -6,
      75,   127, -1551, -1551, -1551, -1551,   172, -1551,    27, -1551,
   -1551, -1551,   296,   296, -1551, -1551,   107, -1551, -1551, -1551,
   -1551, -1551,   215,   318, -1551, -1551, -1551, -1551,    16,    16,
      16,   337, -1551,   776,   -87, -1551, -1551, -1551, -1551,   585,
     719,   230,   720, -1551,   734,    49,    50,   424,   -31,   416,
     431, -1551,   750,   750, -1551,   459,   533,    -3,   533,   744,
     471,   473,   506, -1551,   520,   549, -1551, -1551,   -64,    50,
      16,    16,    16,    16, -1551, -1551, -1551, -1551, -1551, -1551,
   -1551,   575, -1551, -1551, -1551, -1551, -1551, -1551, -1551,   379,
   -1551, -1551, -1551, -1551, -1551,   749,   130, -1551, -1551, -1551,
   -1551,   693, -1551, -1551, -1551, -1551, -1551, -1551, -1551,   533,
   -1551, -1551, -1551,   544, -1551, -1551, -1551, -1551, -1551,   591,
   -1551,   175, -1551,   214,   631,   776, 14857, -1551,   237,   683,
   -1551,    -7, -1551, -1551, -1551,   754, -1551, -1551, -1551, -1551,
   -1551, -1551, -1551,   199, -1551,   640, -1551,   771, -1551,   672,
     379,   379, -1551, -1551, 13715, -1551, -1551, -1551, -1551, -1551,
   -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551,
   -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551,
     833,   843, -1551,   664,   379,   953, -1551, -1551,   710, -1551,
     543,    50,    50,   155,   113, -1551, -1551, -1551,   130, -1551,
   10495, -1551,   641,   379, -1551, -1551,   723,   726, -1551, -1551,
   -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551,
   -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551,   727,   670,
   -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551,
   -1551, -1551,   881, -1551, -1551, -1551, -1551, -1551, -1551, -1551,
   -1551, -1551, -1551, -1551, -1551,   731,   692, -1551, -1551,    92,
     718, -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551,
   -1551,   379, -1551,   724,   379, -1551, -1551,    -7,   -78, -1551,
      50, -1551,   725,   901,   590, -1551, -1551,   758,   763,   765,
     747,   767,   773, -1551, -1551, -1551,   755, -1551, -1551, -1551,
   -1551, -1551,    83, -1551, -1551, -1551, -1551, -1551, -1551, -1551,
   -1551, -1551, -1551, -1551,   775, -1551, -1551, -1551,   777,   779,
   -1551, -1551, -1551, -1551,   781,   784,   757,   -37, -1551, -1551,
   -1551, -1551, -1551,   869,   791, -1551, -1551, -1551, -1551, -1551,
   -1551, -1551, -1551, -1551, -1551, -1551,   815,  1417, -1551,   788,
   -1551,   276,   533,   345,   774, 10495, -1551,  2611, -1551,   521,
     -37, -1551, -1551, -1551,   113,   780, -1551,  9629,   828,   837,
   10495, -1551,   -15, -1551, -1551, -1551,  9629, -1551, -1551,   840,
     816,   460,   530,   534, -1551, -1551,  9629,   245, -1551, -1551,
   -1551,    13, -1551, -1551, -1551,    29,  5917, -1551,   805, 10243,
   -1551, 10346,   472, -1551, -1551, -1551, -1551, -1551, -1551, -1551,
   -1551, -1551, -1551, -1551, -1551, -1551,  9629, -1551, -1551,   198,
     115,   115,   115, -1551,   791, -1551,   827,   829,  9629, -1551,
   -1551, -1551, -1551, -1551,   975,   -90,   835,    47,  3231, -1551,
   -1551,   379,   279,  6123,   809,  9629,    28,   836,   838,   818,
   -1551,   533,   839,   872,  6329,   -73,   436,   844, -1551,   555,
     850,   851,  3437,  9629,  9629,   119,   119,   119,   841,   847,
     853,   855,   858,   859, -1551,  2184, 10140,  6537, -1551, -1551,
   -1551, -1551, -1551, -1551, -1551,  6743,   857, -1551,  6951,  9629,
    9629,  9629,  9629,  9629,  5093,  7157, -1551,   860, -1551, -1551,
     533,   533, -1551,  9629,  1046, -1551, -1551, -1551, -1551, -1551,
   -1551,  1020, -1551, -1551, -1551,   579, -1551,   -77,   533, -1551,
     533,   533,   533, -1551,   533, -1551, -1551,   994, -1551, -1551,
   -1551, -1551,   846, -1551, -1551,   192, -1551, -1551, -1551, -1551,
   -1551, -1551,  9948, -1551,   862, -1551, -1551, -1551, -1551, -1551,
   -1551, -1551,   326, -1551,   553, -1551,    34, -1551, -1551,   864,
     885,   887, -1551, 10838, -1551,  1043,   546, -1551, -1551, -1551,
    3849, 10495, 10495, 10495, 10949, 10495, 10495,   870, -1551, 10495,
     664, 10495,   664, 10495,   664, 10598,   914, 10987, -1551,  9629,
   -1551, -1551, -1551, -1551,   877, -1551, -1551, 13198,  9629, -1551,
     869,   584,   -51, -1551, -1551,   519, -1551,   791,   553,   902,
     519, -1551,   553, 11098,   883,  1058, -1551, -1551,   -60, -1551,
   -1551, -1551, -1551, -1551, 13757,   198, -1551,   894, -1551, -1551,
     -37,   562, -1551,   912,   918,   921, -1551,  9629,  3849, -1551,
     929,   980, 10803,  1108, 10495,  9629,  9629, 14240,  9629, 13757,
     950,   932, -1551, -1551,  9629, -1551, -1551,   931,   960, 14240,
    9629, -1551, -1551,  9629, -1551, -1551,  9629, -1551, 10495,  3849,
   -1551, 10803,   973,   973,   907, -1551,   846, -1551, -1551, -1551,
    9629,  9629,  9629,  9629,  9629,  9629,   198,  1974,   198,  2817,
     198, 13943, -1551,   753, -1551, 13757, -1551,   713,   198,   944,
   -1551,   938,   973,   973,    74,   973,   973,   198,  1119,   922,
     943, 14240,   923,   -29,   943,   955,   926,   266, -1551, -1551,
   13757, -1551,   379, -1551, -1551, -1551,  4055, -1551, -1551, -1551,
   -1551, -1551, -1551,   -45,    54,   119, -1551, 14634, 14665,  4261,
    4261,  4261,  4261,  4261,  4261,  4261,  4261,  9629,  9629, -1551,
   -1551,  9629,  4261,  4261,  9629,  9629,  9629,   968,  4261,  9629,
      95,  9629,  9629,  9629,  9629,  9629,  9629,  4261,  4261,  9629,
    9629,  9629,  4261,  4261,  4261,  9629,  4261,  7363,  9629,  9629,
    9629,  9629,  9629,  9629,  9629,  9629,  9629,  9629, 14795,  9629,
   -1551,  7569,   521,  9629, -1551, -1551,    16, -1551,  1141, -1551,
      -7, 10495, -1551,   978, -1551,  3849, -1551, 10682,   470,   496,
     954,   487, -1551,   644,   654, -1551,   979,   659,   718,   665,
     718,   666,   718, -1551,   157, -1551,   294, -1551, 10495,   936,
   -1551, -1551, 13309,   408, -1551,   553, 10495, -1551, -1551, 10495,
   -1551, -1551, -1551,  9629,   982, -1551,   983, -1551, 10495, -1551,
    9629, 10495, 10495, -1551,    36, 10495,  5299,  7775,   986,  9629,
   10495, -1551, -1551, -1551, 10495,   943, -1551,   929,  9629,  9629,
    9629,  9629,  9629,  9629,  9629,  9629,  9629,  9629,  9629,  9629,
    9629,  9629,  9629,  9629,  9629,  9629,   379,   874,   942, 14240,
   11133, -1551,   969, -1551, 10495, 10495, 11244, 10495, -1551, -1551,
   11282, 10495,   943, 10495, 10598,   943,   914,   595, -1551, 10803,
   -1551,    54, 11393, 11428, 11539, 11577, 11688, 11723,    43,   119,
    3024,  4469,  5505, 13980,   971,    -2,   163,   974,   242,    46,
    5711,    -2,   264,    48,  9629,   987,  9629, -1551, -1551, 10495,
   -1551, 10495, -1551,  9629,   667,    51, -1551,  9629, -1551,    52,
     198, -1551,  9629, -1551,  9629,  9629,  9629,   946,   239, -1551,
   -1551,   949,   957,   542, -1551, -1551,   762,  9629, -1551,   846,
      39,  4677, -1551,   247,   959,  1005,  1005, -1551, -1551,   962,
     170,   664, -1551,   984,   964, -1551, -1551,   985,   967, -1551,
   -1551,   119,   119,   119, -1551, -1551,  2375, -1551,  2375, -1551,
    2375, -1551,  2375, -1551,  2375, -1551,  2375, -1551,  2375, -1551,
    2375,  1253,  1253,  1121, -1551,  2375, -1551,  2375,  1121,  1093,
    1093,   972, -1551,  2375,   893,   976, -1551, 13347,   169,   169,
     862, 14240,  1253,  1253, -1551,  2375, -1551,  2375, 14463, 10639,
    9995, -1551,  2375, -1551,  2375, -1551,  2375, 14333, -1551,  2375,
   14764, 14073, 14489,  1484,  2206,  1121,  1121,  1373,  1373,   893,
     893,   893,   359,  9629,   977,   988,   485,  9629,  1185,   989,
   13458, -1551,   269,   553, 13850,    60,   751,  1125,   533,   970,
   -1551, -1551, 10682, -1551, -1551, -1551, -1551, 10495, -1551, -1551,
   -1551,   196, -1551,   996, -1551,   997, -1551,   998, -1551, 10598,
   -1551,   914,   324,   791, -1551, -1551,  9629, -1551, -1551,   791,
     791, 11834, -1551,  1156,   -25, 14240,  1353,  1886,  9629,   668,
     523,   277,   990,  1000,  1031, 11872,   334, 11983,   674, 10495,
   10598,   914,  2180,  1002, 14240, 14240, 14240, 14240, 14240, 14240,
   14240, 14240, 14240, 14240, 14240, 14240, 14240, 14240, 14240, 14240,
   14240, 14240, -1551,  1008, 10495, -1551, -1551, 10495,  9629,  2287,
    2333, -1551,  4258, -1551,  4466,  1012,  4674,   329,  1015,   386,
      54,   664, -1551, -1551, -1551, -1551, -1551,  1009,  9629, -1551,
    4885, 13198,    38,  9629,   239,   523,   163, -1551, -1551,   991,
   -1551,  9629,  9629, -1551,   999, -1551,  9629,   523,   606,  1018,
   -1551, -1551,  9629, 14240, -1551, -1551,   390,   393, 14110,  9629,
   -1551,  9629,    57, 12018, 14240, 14240, -1551,  1019,   177,  9629,
    9629, 10495,   664,   379, -1551, -1551,  9629, -1551,  1684,  2611,
      54,   186, -1551,  1023,    93,  9835, -1551, -1551, -1551,   173,
     316,   170,  1041,  1070,  1027,  1072,  1073, -1551,   238,   718,
   -1551,  9629, -1551,  9629, -1551, -1551, -1551,  7981,  9629, -1551,
    1050,  1036, -1551, -1551,  9629,  1037, -1551, 13493,  9629,  8187,
    1038, -1551, 13604, -1551,  8393, -1551, -1551, -1551, -1551,   533,
   -1551, -1551,   687, -1551,   134, -1551,    54, -1551, -1551, -1551,
   -1551,   791, -1551, -1551, -1551,  1083, -1551, -1551, -1551, -1551,
   -1551, -1551, -1551, -1551, -1551, -1551,  1044, 10495, 13850, -1551,
    1085,  9629, -1551, -1551,   231, -1551,  1045, -1551, -1551, -1551,
     411, -1551,  1092,  1049, -1551, -1551,  4882,   448,   476, -1551,
   -1551,  9629,  6740,   791, 14240, -1551, -1551, -1551, -1551, -1551,
   -1551, -1551, -1551, -1551,   605,   718,  8599,   395,    -2,   163,
   14240,   971, -1551, -1551, 14240,   974, -1551,   627,    -2,  1053,
   -1551, -1551, -1551, -1551,   635, -1551, -1551, -1551,  1089,   642,
     649,  9629,   188,  9629,  9629,  9629, 12129, 12167,  9832,   718,
   -1551, 13757, -1551,   533, -1551,   664, -1551,  1054,  4677,  1098,
    1057,   400,   354, -1551, -1551,  1103, -1551, -1551,   170,  1061,
     202, 10495, 12278, 10495, 12313, -1551,   365, 12424, -1551,  9629,
   14370,  9629, -1551, 12462,  4677, -1551,   396,  9629, -1551, -1551,
   -1551,   417, -1551,  1245,   134, -1551, -1551,   751,  1065, -1551,
   -1551, -1551, -1551, -1551,  9629,   791, -1551, -1551, 14240,  1066,
    1067, -1551, -1551, -1551,  9629,  1112,  1087,  9629, -1551, -1551,
   -1551, -1551,  1071,  1069,  1081,  9629,  9629,  9629,  1082,  1219,
    1084,  1086,  8805, -1551,   202, -1551,   418,  9629,   163, -1551,
    9629,   606, -1551,  9629,  9629,  1088, -1551, -1551,  9629,  9629,
     650,  9629,  9629, 12573, 14240, 14240, -1551, -1551, -1551,  1091,
     762,  3643, -1551,   718, -1551,   308, -1551,   508, 10495,   -15,
   -1551,  1094, -1551, -1551,  9011, -1551, -1551, 10328, -1551,   676,
   -1551, -1551, -1551, 10495, 12608, 12719, -1551,   410, -1551, 12757,
   -1551, -1551, -1551,  1245,   198,  1095,  1219,  1219, 12868,  1101,
    1097, 12903,  1099,  1100,  1102,  9629, -1551,  9629,  1121,  1121,
    1121,  9629, -1551, -1551,  1219,  1219, -1551, 13014, -1551, -1551,
   14203, -1551, 14203, -1551,  1117,  1121,  9629, -1551,  1127,  1117,
   14203,  9629, 14240, 14240,   194,   283, -1551, -1551,  9217,  9423,
   -1551, -1551, -1551, -1551, -1551, 14240,   379,  1109, 10495,   -15,
    1766,  3849, -1551,  9629, 14333, -1551, -1551,   685, -1551, -1551,
    1110, -1551, 14857, -1551, -1551, -1551, -1551, -1551,   132,   132,
   -1551,  9629,  9629, -1551,  1219,  1219,   523,  1111,  1113,   943,
     132,   523, -1551,  1256,  1090,  1126,  1131,  1135, -1551,  1146,
    1116, 14203,  9629,  9629, -1551,   283,  9629,  9629, 14240, -1551,
   -1551,  1766,  3849,  3849, -1551, 10682, 14370, -1551, -1551, -1551,
   -1551,   533, 14857,   523,   971,  1142, -1551,  1122,  1123, 13052,
   13163,   132,   132,   971,  1124, -1551, -1551,  1128,  1132,  1133,
    9629,  1118,  1136,  1153, -1551,  1137, -1551, -1551,  1138, 14240,
   14240, -1551, 14240,  3849, -1551, 10682, -1551, 10682, -1551, -1551,
   -1551,   379,   420,  1139, -1551, -1551, -1551, -1551, -1551,  1140,
    1143, -1551, -1551, -1551, -1551, 14240, -1551, -1551, -1551, -1551,
   -1551, -1551, 10682, -1551, -1551, -1551, -1551,   523, -1551, -1551,
   -1551,   432, -1551
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   133,     1,   328,     0,     0,     0,   655,   329,     0,
     647,   647,   647,    71,    72,     0,     0,    15,     3,     0,
      10,     9,     8,    16,     0,     7,   635,     6,    11,     5,
       4,    13,    12,    14,   103,   104,   102,   111,   113,    45,
      61,    58,    59,    47,    48,    49,     0,    50,    56,    46,
     244,   243,   647,   647,    22,    21,   635,   649,   648,   826,
     816,   821,     0,   296,    43,   119,   120,   121,     0,     0,
       0,   122,   124,   131,     0,   118,    17,   673,   672,   237,
     657,     0,   676,   636,   637,     0,     0,     0,     0,    51,
       0,    57,     0,     0,    54,     0,     0,   647,     0,    18,
       0,     0,     0,   298,     0,     0,   130,   125,     0,     0,
       0,     0,     0,     0,   134,   239,   238,   241,   236,   659,
     658,     0,   675,   674,   678,   677,   681,   639,   638,   641,
     109,   110,   107,   108,   106,     0,     0,   105,   114,    62,
      60,    56,    53,    52,   650,   652,   246,   245,   654,     0,
     656,    19,    20,    23,   827,   817,   822,   297,    41,    44,
     129,     0,   126,   127,   128,   132,     0,   660,     0,   669,
     631,   563,    26,    27,    31,     0,    98,    99,    96,    97,
      95,    94,   100,     0,    55,     0,   653,     0,    25,   734,
       0,     0,    42,   123,     0,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
       0,     0,   140,   135,     0,     0,   661,   670,     0,   682,
     633,     0,     0,   565,     0,    28,    29,    30,     0,   112,
       0,    24,     0,     0,   818,   823,   205,   206,   203,   143,
     144,   146,   145,   147,   148,   149,   150,   176,   177,   174,
     175,   167,   178,   179,   168,   165,   166,   204,   187,     0,
     202,   180,   181,   182,   183,   154,   155,   156,   151,   152,
     153,   164,     0,   170,   171,   169,   162,   163,   158,   157,
     159,   160,   161,   142,   141,   186,     0,   172,   173,   563,
     138,   273,   242,   716,   719,   722,   723,   717,   720,   718,
     721,     0,   643,   667,   679,   632,   640,   563,     0,   115,
       0,   117,     0,   621,   619,   642,   101,     0,     0,     0,
       0,     0,     0,   689,   709,   690,   725,   691,   695,   696,
     697,   698,   715,   702,   703,   704,   705,   706,   707,   708,
     710,   711,   712,   713,   786,   694,   701,   714,   793,   800,
     692,   699,   693,   700,     0,     0,     0,     0,   724,   748,
     751,   749,   750,   813,   651,   737,   738,   735,   736,   828,
     598,   604,   207,   208,   201,   185,   209,   188,   184,     0,
     136,   327,   589,   590,     0,     0,   240,     0,   664,   662,
       0,   671,   577,   683,     0,     0,   116,     0,     0,     0,
       0,   620,     0,   754,   777,   780,     0,   783,   773,     0,
       0,   787,   794,   801,   807,   810,     0,     0,   763,   768,
     762,     0,   776,   772,   765,     0,     0,   767,   752,     0,
     730,   819,   824,   210,   190,   191,   193,   192,   194,   195,
     196,   197,   189,   198,   199,   200,     0,   325,   326,     0,
     563,   563,   563,   137,   139,   275,     0,     0,     0,    73,
      74,    86,   468,   469,     0,     0,     0,     0,   313,   462,
     311,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     312,     0,     0,     0,     0,     0,     0,     0,   715,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   552,     0,     0,     0,   385,   387,
     386,   388,   389,   390,   391,     0,     0,    37,   281,     0,
       0,     0,     0,     0,   277,     0,   367,   368,   466,   465,
       0,     0,   252,     0,     0,   268,   263,   260,   259,   261,
     262,   247,   295,   274,   254,   546,   253,   463,     0,   537,
      81,    82,    79,   266,    80,   267,   269,   331,   258,   536,
     535,   534,   133,   540,   464,     0,   255,   539,   538,   510,
     470,   511,   392,   471,     0,   467,   831,   835,   832,   833,
     834,   643,     0,   644,     0,   668,   579,   634,   564,     0,
       0,     0,   546,     0,   623,   624,     0,   617,   618,   616,
       0,     0,     0,     0,     0,     0,     0,     0,   739,     0,
     135,     0,   135,     0,   135,     0,     0,     0,   759,   277,
     770,   771,   764,   766,     0,   769,   753,     0,     0,   815,
     814,   829,   296,   742,   743,     0,   599,   594,     0,     0,
       0,   605,     0,     0,     0,   684,   586,   587,   609,   591,
     593,   592,   877,   880,     0,     0,   301,   305,   304,   310,
       0,     0,   353,     0,     0,     0,   913,     0,     0,   317,
     314,     0,   362,     0,     0,   281,     0,   299,     0,     0,
       0,     0,   344,   347,     0,   272,   350,     0,     0,    65,
       0,    88,   917,     0,   886,   895,     0,   883,     0,     0,
     322,   319,   498,   499,   368,   380,   133,   294,   292,   293,
       0,     0,     0,     0,     0,     0,     0,   855,     0,     0,
       0,   893,   920,     0,   285,     0,   288,     0,     0,     0,
     922,   931,   475,   474,   512,   473,   472,     0,     0,     0,
     931,   362,     0,   296,   931,   931,     0,   369,   270,   271,
       0,    84,     0,   383,   250,   544,     0,   257,   264,   265,
     316,   321,   330,     0,   377,     0,   256,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   500,
     501,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   452,     0,
     249,     0,   665,     0,   645,   663,     0,   578,     0,   680,
     563,     0,   622,     0,   626,     0,   630,   392,     0,     0,
       0,   744,   757,     0,     0,   726,     0,     0,   138,     0,
     138,     0,   138,   596,     0,   602,     0,   727,     0,     0,
     761,   746,     0,     0,   731,     0,     0,   600,   820,     0,
     606,   825,   588,     0,     0,   608,     0,   607,     0,   610,
       0,     0,     0,    89,     0,     0,   869,     0,     0,     0,
       0,   903,   906,   909,     0,   931,   318,   315,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   300,
       0,    90,     0,    91,     0,     0,     0,     0,    63,    64,
       0,     0,   931,     0,     0,   931,     0,     0,   323,   320,
     369,   377,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   889,   847,   855,     0,   898,     0,     0,
       0,   855,     0,     0,     0,     0,     0,   858,   925,     0,
     276,     0,    40,     0,    38,     0,   924,   932,   282,     0,
       0,   900,   932,   278,     0,     0,     0,     0,   932,   846,
     503,     0,     0,   439,   436,   438,    68,     0,   273,   133,
       0,   277,   455,     0,     0,     0,     0,   343,   342,     0,
       0,   135,   291,     0,     0,   523,   522,     0,     0,   524,
     528,     0,     0,     0,   414,   423,   402,   424,   403,   426,
     405,   425,   404,   427,   406,   417,   396,   418,   397,   419,
     398,   476,   477,   489,   428,   407,   429,   408,   490,   487,
     488,     0,   416,   394,   517,     0,   508,     0,   541,   542,
     543,   395,   478,   479,   430,   409,   431,   410,   494,   495,
     496,   420,   399,   421,   400,   422,   401,   497,   415,   393,
       0,     0,   492,   493,   491,   485,   486,   481,   480,   482,
     483,   484,     0,     0,     0,   445,     0,     0,     0,     0,
       0,   460,     0,     0,     0,     0,   571,   574,     0,     0,
     625,   628,   392,   629,   755,   778,   781,     0,   784,   774,
     728,     0,   788,     0,   795,     0,   802,     0,   808,     0,
     811,     0,     0,   283,   758,   747,     0,   732,   830,   595,
     601,     0,   686,   687,   612,   611,     0,     0,     0,     0,
     870,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   363,   402,   403,   405,   404,   406,
     396,   397,   398,   407,   408,   394,   409,   410,   399,   400,
     401,   393,   324,     0,     0,   926,   513,     0,     0,     0,
       0,   514,     0,   545,     0,     0,     0,     0,     0,     0,
     377,   135,   547,   548,   549,   550,   551,     0,     0,   856,
       0,   362,   855,     0,     0,     0,     0,   864,   865,     0,
     872,     0,     0,   862,     0,   901,     0,     0,     0,     0,
     860,   902,     0,   892,   857,   921,     0,     0,    34,     0,
     923,     0,     0,     0,   837,   836,   502,     0,     0,     0,
       0,     0,   135,     0,    66,    67,     0,    83,    75,     0,
     377,     0,   456,     0,     0,     0,   459,   457,   332,     0,
       0,     0,     0,     0,     0,     0,     0,   375,     0,   138,
     519,     0,   525,     0,   413,   411,   412,     0,     0,   506,
       0,     0,   529,   533,     0,     0,   509,     0,     0,     0,
       0,   446,     0,   453,     0,   504,   461,   666,   646,   134,
     572,   573,   574,   575,   566,   580,   377,   627,   756,   779,
     782,   745,   785,   775,   740,     0,   789,   791,   796,   798,
     803,   805,   809,   597,   812,   603,     0,     0,     0,   685,
       0,     0,   878,   881,     0,   302,     0,   307,   308,   306,
       0,   356,     0,     0,   359,   354,     0,     0,     0,   914,
     912,   281,     0,    93,    92,   345,   348,   351,   918,   916,
     887,   896,   894,   884,     0,   138,     0,     0,   855,     0,
     890,   848,   871,   863,   891,   899,   861,     0,   855,     0,
     867,   868,   875,   859,     0,   286,   289,    35,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   138,
      69,     0,    76,     0,   248,   135,   458,     0,   277,     0,
       0,   619,     0,   372,   373,     0,   371,   370,     0,     0,
       0,     0,     0,     0,     0,   434,     0,     0,   530,     0,
     518,     0,   507,     0,   277,   447,     0,     0,   505,   454,
     450,     0,   585,   569,   566,   567,   568,   571,     0,   741,
     729,   792,   799,   806,   277,   284,   733,   688,   613,     0,
       0,    87,   303,   309,     0,     0,     0,     0,   355,   904,
     907,   910,     0,     0,     0,     0,     0,     0,     0,   869,
       0,     0,     0,   251,     0,   553,     0,     0,     0,   873,
       0,     0,   866,     0,     0,   279,    32,    39,     0,     0,
       0,     0,     0,     0,   839,   838,   437,   562,   440,     0,
      68,     0,    85,   138,   432,     0,   333,   619,     0,     0,
     379,     0,   376,   378,     0,   364,   382,     0,   561,     0,
     559,   435,   556,     0,     0,     0,   555,     0,   448,     0,
     451,   570,   581,   569,     0,     0,   869,   869,     0,     0,
       0,     0,     0,     0,     0,   277,   927,   281,   346,   349,
     352,     0,   870,   888,   869,   869,   515,     0,   381,   554,
     929,   874,   929,   876,   929,   287,   277,   290,    36,   929,
     929,     0,   841,   840,     0,     0,   443,    70,   313,     0,
      77,    81,    82,    79,    80,    78,     0,     0,     0,     0,
       0,     0,   374,     0,   365,   520,   526,     0,   560,   558,
       0,   557,     0,   583,   615,   614,   576,   760,   854,   854,
     357,     0,     0,   360,   869,   869,   844,     0,     0,   931,
     854,   844,   516,     0,     0,     0,     0,   931,    33,     0,
       0,   929,     0,     0,   441,     0,     0,     0,   319,   384,
     433,     0,     0,     0,   341,   392,   366,   521,   527,   531,
     449,     0,     0,     0,   851,   931,   853,     0,     0,     0,
       0,   854,   854,   845,     0,   915,   928,     0,     0,     0,
       0,     0,     0,     0,   932,     0,   937,   933,     0,   843,
     842,   444,   320,     0,   339,   392,   337,   392,   340,   532,
     582,     0,     0,   932,   852,   879,   882,   358,   361,     0,
       0,   911,   919,   897,   885,   930,   935,   936,   938,   280,
     934,   335,   392,   338,   336,   584,   849,     0,   905,   908,
     334,     0,   850
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1551, -1551, -1551, -1551, -1551, -1551, -1551,   602,  1258, -1551,
   -1551, -1551, -1551, -1551, -1551,  1343, -1551, -1551, -1551,   759,
    1303, -1551,  1223, -1551, -1551,  1272, -1551, -1551, -1551,  -165,
      -1, -1551, -1551, -1551,  -162, -1551, -1551, -1551, -1551, -1551,
   -1551, -1551, -1551, -1551,  1147, -1551, -1551,   -57,   -36, -1551,
   -1551, -1551,   457,   537,  -552,  -596,  -803, -1551, -1551, -1551,
   -1511, -1551, -1551,    18,  -224,  -223,  -393, -1551,   358, -1551,
    -618, -1551,  -672,  -432,  -310, -1551, -1551, -1551, -1551,  -471,
       6, -1551, -1551, -1551, -1551, -1551,  -161,  -156,  -153, -1551,
    -151, -1551, -1551, -1551,  1375, -1551,   371, -1551, -1551, -1551,
   -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551,
     210,  -116,  -227,   -48,   110,  -916,  -467, -1551,  -546, -1551,
   -1551,  -392,  1190, -1551, -1551, -1551, -1411, -1551, -1551, -1551,
   -1551, -1551, -1551, -1551, -1551, -1551,   941, -1551, -1551, -1551,
   -1551, -1551, -1551, -1551,  -155,   -72,  -167,   -69,    78, -1551,
   -1551, -1551, -1551, -1551, -1551, -1551,   491,  -424,  -903, -1551,
    -440,  -912, -1551,  -646,  -163,  -160, -1551,  -603, -1353, -1551,
    -371, -1551, -1551,  1346, -1551, -1551, -1551,   808,   958,    77,
   -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551,
   -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551,
    -627,  -179, -1551,   933, -1551, -1551, -1551,  1063, -1551, -1551,
   -1551,  -263, -1551, -1551,  -391, -1551, -1551, -1551, -1551, -1551,
   -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551,
   -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551,
   -1551, -1551,  -208, -1551, -1551, -1551, -1551, -1551, -1551, -1551,
   -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551,   934,  -684,
    -245,  -739,  -706, -1551, -1551, -1550,  -935, -1551, -1551, -1551,
   -1189,  -101, -1311, -1551, -1551, -1551, -1551, -1551, -1551, -1551,
   -1551, -1551,   179,  -509, -1551, -1551, -1551,   686, -1551, -1551,
   -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551, -1551,
   -1551, -1551, -1506,  -740, -1551
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    17,   153,    56,   188,    18,   175,   181,  1598,
    1408,  1516,   737,   538,   159,   539,   105,    20,    21,    47,
      48,    49,    94,    22,    41,    42,   540,   541,  1266,  1267,
     644,   543,  1423,  1531,   544,   545,  1007,   546,   665,   547,
     548,   549,   550,  1198,   182,   183,    37,    38,    39,   233,
      71,    72,    73,    74,    24,   300,   396,   223,    25,   117,
     224,   118,   166,   301,   148,   715,  1034,   553,   397,   554,
     749,  1597,   739,  1142,   602,   979,  1514,   981,  1515,   556,
     557,   558,   667,   895,  1482,   559,   560,   561,   562,   563,
     564,   565,   566,   459,   567,   773,  1279,  1017,   568,   569,
     934,  1495,   935,  1496,   937,  1497,   570,   900,  1488,   571,
     750,  1546,   572,  1287,  1288,  1021,   717,   573,   830,  1009,
     574,   681,  1035,   576,   577,   578,  1005,   579,  1261,  1605,
    1262,  1665,   580,  1108,  1459,   581,   751,  1441,  1677,  1443,
    1678,  1553,  1719,   583,   391,  1467,  1562,  1322,  1324,  1118,
     596,   840,  1632,  1682,   392,   393,   394,   863,   864,   441,
     865,   866,   442,   996,   656,   657,  1636,   610,   412,   324,
     325,   230,   317,    84,   129,    27,   171,   399,    95,    96,
     185,    97,    28,    53,   121,   168,    29,   312,   594,   591,
    1113,   401,   228,   229,    82,   126,   403,    30,   169,   314,
     658,   584,   311,   370,   371,  1131,   641,   243,   372,   856,
    1469,  1139,   850,   438,   373,   611,  1328,   868,   616,  1333,
     612,  1329,   613,  1330,   615,  1332,   619,  1336,   620,  1471,
     621,  1338,   622,  1472,   623,  1340,   624,  1473,   625,  1342,
     626,  1344,   647,    31,   101,   190,   380,   648,    32,   102,
     191,   381,   652,    33,   100,   189,   440,   875,   585,   755,
    1694,   756,   965,  1685,  1686,  1687,   966,   978,  1241,  1235,
    1230,  1402,  1163,   586,   891,  1479,   892,  1480,   946,  1501,
     943,  1499,   967,   740,   587,   944,  1500,   968,   588,  1169,
    1572,  1170,  1573,  1171,  1574,   904,  1492,   941,  1498,   733,
     741,   589,  1654,   988,   590
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      23,   302,   650,   845,   552,   575,   316,   679,   732,   764,
     993,   869,   890,   928,   997,   999,   234,   645,    76,   606,
     774,    64,    75,   971,   858,    52,   860,  1014,   862,   138,
    1226,   710,   374,   597,  1209,  1211,  1238,  1392,   894,   718,
     719,  1207,  -133,   964,   838,   964,   630,   690,   640,    90,
      65,   959,   673,    62,    57,  1133,  1158,  1135,  1019,  1137,
      58,   369,   632,  1218,   130,   131,  1236,   885,  1242,    34,
      35,  1249,  1251,   161,    75,    75,    75,  1411,  1539,    66,
     110,   111,   112,    63,    91,   552,  1655,   555,  1656,  1688,
      50,   959,   104,  1659,  1660,   145,   765,   150,   885,   958,
    1698,   969,   885,   973,   113,    54,    62,   322,   995,   552,
      51,   985,   607,    86,   104,  1015,    75,    75,    75,    75,
     989,  1681,   608,    67,   727,   729,     2,   670,   323,   114,
      98,   886,   887,     3,   888,    40,    63,   889,   405,   846,
     766,  1729,  1730,   876,   700,   176,   177,   170,   186,   674,
     675,    55,    68,   231,   160,  1708,     4,  1162,     5,    62,
       6,   146,   404,   887,   951,  1173,     7,   887,   555,    83,
     439,  1721,  1016,   551,   149,   319,     8,   609,   646,   651,
    1465,   147,     9,  1225,  1619,    86,   226,   464,  1583,    63,
      77,    78,   555,    79,  1664,   318,   752,   752,   146,    13,
    1509,   691,  1205,   800,   801,  1208,    10,   906,   244,   245,
     631,  1271,  1271,   232,  1020,   132,   369,   552,   147,    14,
     133,    80,   134,    36,    69,   135,   633,   886,    11,    12,
      92,   369,   321,    70,   886,   122,   123,   886,   948,   886,
    1466,    93,   886,   886,   634,   676,   368,   389,   886,   836,
     635,   113,   231,  1065,  1711,  1638,  1639,  1272,   683,  1368,
     369,   379,   369,   406,   677,   322,   136,  1367,  1066,    87,
     389,  1273,   701,  1650,  1651,   231,  1319,   242,   716,   716,
     716,  -734,    50,   457,  1224,   552,   323,  1389,   828,   829,
     752,    13,    81,   927,  1384,   797,   178,  1282,   800,   801,
     555,   179,    51,   180,   885,  1271,   135,  1283,  1022,  1427,
     390,    14,   232,   885,  1414,   885,   552,   947,    88,  1591,
      13,   885,    13,   995,   775,  1521,   458,  1274,   763,   398,
      15,  1662,   402,  1691,  1692,   232,   369,   369,   536,   714,
      14,    16,    14,  1284,  1544,    43,    44,    45,   643,  1683,
     654,  1227,  1228,    57,  1425,   763,  1138,    13,   110,    58,
     112,    13,  1285,  1252,  1429,    50,    86,  1286,   555,   887,
     835,   655,   839,   427,  1121,   320,    46,    14,   887,  1229,
     887,    14,   877,   828,   829,    51,   887,   880,   103,  1430,
     238,   460,   462,   193,    50,  1334,   542,  1257,   593,   555,
      50,   368,  1018,   848,   849,   851,   595,   853,   854,    62,
    1468,   857,  1258,   859,    51,   861,   368,   239,  1001,    85,
      51,  1356,   908,  1002,   878,  1289,    62,  1232,   881,  1438,
    1233,   225,   369,   369,   369,  1001,   369,   369,  1275,    63,
     369,   893,   369,   628,   369,   368,   369,   368,  1239,  1232,
    1119,  1240,   552,  1508,  1439,   763,    63,  1270,  1234,    13,
     908,   104,   629,  1511,  1003,  1276,   931,  1245,   908,   555,
     555,   555,   555,   555,   555,   555,   555,   684,  1250,    14,
     368,   671,   555,   555,   763,   643,  1440,  1316,   555,    13,
     695,    13,  1219,  1140,    13,  1357,   685,   555,   555,  1224,
    1156,  1157,   555,   555,   555,   369,   555,  1429,  1351,    14,
      13,    14,   980,  1172,    14,   643,  1388,  1305,   833,  1362,
     643,   368,   368,  1346,  1617,   106,   107,   108,  1381,   369,
      14,  1398,  1306,  1363,  1431,   555,   461,  1006,   964,   758,
     759,  1391,    50,  1199,  1200,  1438,  1202,  1246,   716,  1247,
    1204,    13,  1206,   964,   109,    13,   908,   767,    13,   768,
     769,   770,    51,   771,  1294,  1295,  1296,   162,   163,   164,
     165,    14,  1540,    13,   776,    14,    13,   643,    14,   428,
    1507,   643,  1504,  1551,   643,  1383,   908,   908,  1127,  1405,
     139,   834,  1406,    14,  1538,   837,    14,   411,  1026,  1030,
    1146,  1224,   643,  1141,   141,   844,   429,   430,   908,   908,
    1484,  1224,  1415,    13,  1558,  1385,  1529,   368,   368,   368,
     763,   368,   368,  1224,  1070,   368,  1630,   368,   428,   368,
      93,   368,   144,    14,   702,  1560,  1589,    13,  1746,   643,
     874,    13,   115,  1310,   154,   649,   155,  1490,   116,  1109,
    1752,  1148,    13,   703,  -790,   429,   430,    14,  1311,  -790,
    1143,    14,   369,   643,  1194,   439,  1419,   643,  1149,  1124,
     431,  1150,    14,   607,   432,  1491,   897,  -790,   643,   156,
    1154,   752,   439,   608,    13,  1117,    13,  1159,   680,   369,
     368,   439,  1168,   157,   592,  1125,  1271,   369,    13,  1493,
     369,  1345,  1618,  1192,    14,   411,    14,   146,   315,   369,
     643,    13,   369,   369,   368,  1343,   369,   146,    14,   431,
     375,   369,   763,   432,  -797,   369,   158,   147,  -804,  -797,
    1616,    14,   716,  -804,  1463,   376,  -442,   147,   609,   433,
     377,  -442,   378,   434,  1535,   754,   435,  -797,   167,    13,
      91,  -804,   187,   705,  1323,   369,   369,   873,   369,  -442,
     898,   436,   369,  1004,   369,   369,  1522,   437,   192,    14,
    1557,  1143,   706,  1143,  1263,   110,   119,   124,  1366,   899,
    1008,   409,   120,   125,   410,  1264,  1265,   411,   433,   227,
    1399,   127,   434,  1400,  1210,   435,  1401,   128,   536,   714,
     369,   151,   369,  1372,   716,   716,   716,   152,  1320,   763,
     436,   763,  1510,   763,  1321,   763,   437,   763,   908,   763,
    1513,   763,  1502,   763,   536,   714,   908,  1518,   763,  1533,
     763,   593,   240,   908,  1519,  1601,   763,   241,  1503,   439,
     908,   908,    75,  1128,   235,   236,  1123,   368,   763,   439,
     763,   142,   143,  1129,   439,   763,  1565,   763,  1132,   763,
     439,   439,   763,   439,  1134,  1136,   242,  1355,   297,   439,
    1418,   439,  1147,  1365,   368,  1626,   552,   575,   298,  1347,
     439,   299,   368,   313,  1679,   368,   385,   905,   907,  1317,
     172,   173,   982,   983,   368,   763,   382,   368,   368,   383,
     384,   368,   428,   386,   387,  1648,   368,   428,   388,  1697,
     368,  1303,   395,   942,   777,   778,   945,  1705,   400,  1331,
     110,   111,   112,    43,    44,    45,   172,   173,   174,   429,
     430,   235,   236,   237,   429,   430,  1621,   975,   976,   977,
     368,   368,   407,   368,   408,  1724,  1141,   368,   369,   368,
     368,   659,   660,   661,  1684,  1684,   413,  1647,  1663,   555,
     369,   414,  1693,   415,   416,   417,  1684,  1693,    59,    60,
      61,   418,   419,   421,   426,   422,  1013,   423,  1657,   424,
     466,   467,   425,  1347,  1347,   368,   439,   368,   443,  1373,
     369,   369,   463,   431,   777,   778,   598,   432,   431,  1722,
     477,   604,   432,   428,  1594,   456,   482,  1684,  1684,  1599,
     605,   789,   790,   617,   618,   369,  1672,  1673,   369,   797,
     638,   799,   800,   801,   802,   662,   688,   663,   303,   803,
     429,   430,   304,   672,   692,   694,   693,   696,   697,    13,
    1420,  1112,   704,   496,   497,  1424,   305,   306,   707,   708,
    1547,   307,   308,   309,   310,   736,   761,   762,   720,    14,
     772,    16,   433,  1751,   721,   643,   434,   433,  1713,   435,
     722,   434,   723,  1193,   435,   724,   725,   499,   500,   831,
     757,   700,   369,   841,   436,   734,   843,   649,   855,   436,
     437,   789,   790,   870,   431,   437,   879,  1485,   432,   797,
     883,   884,   800,   801,   802,  1141,  1161,   828,   829,   803,
     901,   896,   775,  1318,   777,   778,   902,  1325,    62,   903,
     908,  1327,   926,   932,   933,   938,   939,   950,   986,   987,
    1335,  1481,   990,   368,   992,   515,   516,   517,   991,  1475,
     994,  1061,   777,   778,  1000,   368,   998,  1620,    63,  1116,
    1674,  1120,  1130,  1126,  1144,  1152,  1153,   763,   528,  1166,
    1195,  1197,  1224,   433,  1256,  1231,  1259,   434,   369,  1326,
     435,  1244,   754,   666,  1260,   368,   368,  1277,  1278,  1281,
     754,  1291,  1290,  1292,  1293,   436,  1313,   828,   829,  1297,
     534,   437,  1323,  1298,  1308,  1337,  1339,  1341,  1530,  1350,
     368,  1714,  1716,   368,  1360,  1309,  1314,  1393,  1358,   787,
     788,   789,   790,   791,  1433,  1396,   794,  1545,  1359,   797,
    1370,   799,   800,   801,   802,  1371,  1386,  1671,   552,   803,
    1379,   805,   806,  1382,  1403,  1549,  1413,   787,   788,   789,
     790,  1426,  1741,  1434,  1435,  1436,  1437,   797,  1448,   799,
     800,   801,   802,  1449,  1451,  1457,  1470,   803,  1477,   805,
     806,  1474,   369,  1483,   369,  1486,  1487,   368,   542,  1512,
    1517,  1536,  1534,  1537,   777,   778,  1541,  1543,  1561,   552,
     552,  1545,  1502,  1566,  1567,  1569,  1570,  1576,  1575,  1582,
    1606,   821,   822,   823,   824,   825,   826,   827,  1577,  1581,
    1641,  1584,  1653,  1585,  1658,  1596,  1701,   828,   829,  1700,
    1702,   555,  1622,  1637,  1642,  1703,  1644,  1645,  1462,  1646,
     552,   823,   824,   825,   826,   827,  1704,  1670,  1680,  1695,
    1706,  1696,  1707,  1723,  1736,   828,   829,  1738,   582,   984,
    1725,  1726,  1731,   137,    19,  1627,  1732,  1476,   603,    89,
    1733,  1734,  1737,   368,  1740,  1739,  1747,   614,  1748,   369,
     140,  1749,   555,   555,   184,  1607,  1269,   627,  1354,  1610,
    1611,   789,   790,  1115,   369,  1612,    26,   637,  1613,   797,
    1614,   799,   800,   801,   802,   326,   428,  1280,  1588,   803,
    1542,  1432,  1563,  1669,   777,   778,  1633,   653,  1564,   832,
    1464,  1634,    99,   555,  1635,   420,  1699,   668,   669,   664,
    1593,  1395,     0,   429,   430,   972,     0,     0,     0,   682,
       0,     0,  1532,     0,   687,     0,   689,     0,  1387,     0,
       0,     0,     0,     0,     0,   699,     0,     0,     0,   369,
       0,     0,     0,   711,   712,   713,  1397,   368,   763,   368,
       0,     0,  1404,   823,   824,   825,   826,   827,   731,  1409,
       0,  1410,     0,     0,     0,     0,   735,   828,   829,   731,
     742,   743,   744,   745,   746,     0,     0,   431,     0,     0,
       0,   432,     0,     0,   760,     0,     0,     0,   763,     0,
     763,   789,   790,     0,     0,     0,     0,     0,  1745,   797,
       0,   799,   800,   801,   802,   777,   778,  1446,     0,   803,
       0,     0,     0,     0,     0,   763,     0,     0,     0,  1456,
       0,     0,     0,     0,  1461,   444,   445,   446,   447,   448,
     449,   450,   451,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   368,     0,   433,     0,     0,     0,
     434,   847,  1352,   435,   452,     0,     0,     0,     0,   368,
       0,     0,     0,     0,   453,   454,   455,     0,   436,     0,
       0,     0,     0,     0,   437,   825,   826,   827,     0,   872,
       0,     0,     0,     0,     0,     0,     0,   828,   829,     0,
       0,     0,     0,     0,     0,     0,  1506,     0,     0,     0,
     787,   788,   789,   790,   791,     0,     0,   794,   795,   796,
     797,  1004,   799,   800,   801,   802,     0,     0,     0,   682,
     803,  1520,   805,   806,   368,     0,   731,   929,     0,   930,
       0,     0,     0,     0,     0,   936,     0,     0,     0,     0,
       0,   940,     0,     0,     0,     0,     0,     0,     0,     0,
     949,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   952,   953,   954,   955,   956,   957,     0,   963,     0,
     963,  1004,     0,     0,  1718,     0,     0,     0,     0,     0,
    1720,   820,   821,   822,   823,   824,   825,   826,   827,     0,
       0,     0,     0,     0,     0,     0,  1422,     0,   828,   829,
       0,     0,     0,     0,     0,   777,   778,     0,     0,     0,
       0,     0,     0,     0,  1743,     0,  1744,     0,     0,     0,
    1036,  1038,  1040,  1042,  1044,  1046,  1048,  1050,  1051,  1052,
       0,     0,  1053,  1055,  1057,  1058,  1059,  1060,     0,  1063,
    1064,  1750,  1067,  1068,  1069,  1071,  1072,  1073,  1075,  1077,
    1078,  1079,  1080,  1082,  1084,  1086,  1087,  1089,  1091,  1092,
    1093,  1094,  1095,  1096,  1097,  1098,  1099,  1100,  1101,     0,
    1110,     0,     0,     0,  1114,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1122,     0,     0,     0,
       0,  1649,     0,     0,     0,     0,     0,     0,     0,   428,
     787,   788,   789,   790,   791,     0,     0,   794,   795,   796,
     797,     0,   799,   800,   801,   802,     0,     0,   680,     0,
     803,     0,   805,   806,  1151,     0,   429,   430,   809,   810,
     811,  1155,     0,     0,   815,     0,     0,     0,  1165,     0,
    1167,     0,     0,     0,     0,     0,     0,     0,     0,  1174,
    1175,  1176,  1177,  1178,  1179,  1180,  1181,  1182,  1183,  1184,
    1185,  1186,  1187,  1188,  1189,  1190,  1191,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   907,   817,     0,   818,
     819,   820,   821,   822,   823,   824,   825,   826,   827,     0,
     431,     0,     0,   607,   432,     0,     0,     0,   828,   829,
       0,   742,  1221,   608,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1243,     0,   731,     0,   428,
       0,     0,     0,     0,  1248,     0,     0,     0,   731,     0,
       0,     0,     0,  1174,     0,  1253,  1254,  1255,     0,     0,
       0,     0,     0,     0,     0,     0,   429,   430,  1268,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   609,   433,
       0,     0,     0,   434,     0,     0,   435,     0,     0,     0,
    1037,  1039,  1041,  1043,  1045,  1047,  1049,     0,     0,   466,
     467,   436,  1054,  1056,     0,     0,     0,   437,  1062,   472,
     473,   474,   475,   476,     0,     0,     0,  1074,  1076,   477,
       0,   479,  1081,  1083,  1085,   482,  1088,   428,     0,     0,
     431,     0,     0,   484,   432,     0,     0,     0,     0,   487,
       0,     0,   488,     0,     0,   489,     0,   959,     0,   492,
       0,     0,     0,     0,   429,   430,     0,     0,     0,   599,
       0,     0,   496,   497,  1307,   333,   334,   335,  1312,   337,
     338,   339,   340,   341,   498,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,     0,   355,   356,   357,
       0,     0,   360,   361,   362,   363,   499,   500,   600,   433,
       0,     0,     0,   434,     0,  1353,   435,  1348,     0,     0,
       0,     0,   503,   504,     0,     0,     0,     0,   431,     0,
       0,   436,   432,     0,     0,     0,     0,   437,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   508,   509,   510,   511,   512,
       0,   513,   752,   514,   515,   516,   517,     0,     0,  1374,
     518,   519,   520,   521,   522,   523,   524,   753,   601,   526,
     527,     0,     0,     0,     0,     0,     0,   528,     0,     0,
       0,     0,     0,     0,  1390,     0,     0,   433,     0,     0,
       0,   434,  1394,   963,   960,   530,   531,     0,    15,     0,
       0,   532,   533,     0,     0,     0,     0,     0,     0,   961,
       0,   962,     0,   536,   537,   437,     0,   726,     0,     0,
    1416,  1417,     0,   327,     0,     0,     0,  1421,     0,   328,
     582,     0,     0,   428,     0,   329,  1174,     0,     0,     0,
       0,     0,     0,     0,     0,   330,     0,   777,   778,     0,
       0,     0,  1442,   331,  1444,     0,     0,     0,     0,  1447,
     429,   430,     0,     0,     0,  1450,     0,     0,   332,  1453,
       0,     0,     0,     0,     0,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,   365,     0,     0,
       0,     0,  1478,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   431,     0,     0,     0,   432,     0,
       0,     0,   731,     0,     0,     0,     0,     0,     0,     0,
     428,     0,   787,   788,   789,   790,   791,    62,     0,   794,
     795,   796,   797,     0,   799,   800,   801,   802,     0,     0,
     366,     0,   803,     0,   805,   806,     0,   429,   430,     0,
       0,     0,     0,     0,  1523,  1524,  1525,    63,     0,     0,
       0,     0,     0,     0,     0,     0,   428,     0,     0,     0,
       0,     0,     0,   433,     0,     0,     0,   434,     0,  1369,
     435,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1554,     0,  1555,   429,   430,   436,   777,   778,  1559,     0,
       0,   437,     0,   367,   821,   822,   823,   824,   825,   826,
     827,   431,     0,     0,     0,   432,     0,     0,     0,     0,
     828,   829,     0,     0,     0,  1568,     0,     0,  1571,     0,
       0,     0,     0,     0,     0,     0,  1578,  1579,  1580,     0,
       0,     0,     0,  1587,     0,     0,     0,     0,  1590,     0,
       0,  1592,     0,     0,   731,  1595,     0,   431,     0,   731,
    1600,   432,  1602,  1603,     0,     0,     0,     0,     0,     0,
       0,     0,  1615,     0,     0,     0,     0,     0,     0,     0,
     433,     0,     0,     0,   434,  1624,  1375,   435,     0,     0,
       0,   787,   788,   789,   790,   791,     0,     0,   794,   795,
     796,   797,   436,   799,   800,   801,   802,     0,   437,     0,
       0,   803,     0,   805,   806,     0,     0,     0,   731,   809,
     810,   811,     0,     0,     0,   815,   433,     0,     0,     0,
     434,     0,  1376,   435,     0,     0,     0,     0,     0,     0,
       0,     0,  1661,     0,     0,     0,     0,     0,   436,     0,
    1668,     0,     0,     0,   437,     0,     0,     0,     0,     0,
       0,     0,  1675,     0,  1676,     0,     0,     0,   817,     0,
     818,   819,   820,   821,   822,   823,   824,   825,   826,   827,
       0,     0,  1689,  1690,     0,     0,     0,     0,     0,   828,
     829,     0,     0,     0,   536,   714,     0,     0,     0,     0,
       0,     0,     0,  1709,  1710,     0,     0,     0,  1712,     0,
       0,     0,   465,  1715,  1717,     0,   466,   467,     3,     0,
     468,   469,   470,     0,   471,     0,   472,   473,   474,   475,
     476,     0,     0,     0,     0,     0,   477,   478,   479,   480,
     481,  1735,   482,     0,     0,     0,     0,     0,     0,   483,
     484,     0,     0,   485,  1742,   486,   487,     0,     0,   488,
       0,     8,   489,   490,     0,   491,   492,     0,     0,   493,
     494,     0,     0,     0,     0,     0,   495,     0,     0,   496,
     497,     0,   333,   334,   335,     0,   337,   338,   339,   340,
     341,   498,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,     0,   355,   356,   357,     0,     0,   360,
     361,   362,   363,   499,   500,   501,   502,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   503,
     504,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   505,   506,   507,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   508,   509,   510,   511,   512,     0,   513,     0,
     514,   515,   516,   517,     0,   146,    13,   518,   519,   520,
     521,   522,   523,   524,    63,   525,   526,   527,     0,     0,
       0,     0,     0,     0,   528,   147,    14,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   529,   530,   531,     0,    15,     0,     0,   532,   533,
       0,     0,   466,   467,     0,     0,   534,     0,   535,     0,
     536,   537,   472,   473,   474,   475,   476,     0,     0,     0,
       0,     0,   477,     0,   479,     0,     0,     0,   482,     0,
     428,     0,     0,     0,     0,     0,   484,     0,     0,     0,
       0,     0,   487,     0,     0,   488,     0,     0,   489,     0,
       0,     0,   492,     0,     0,     0,     0,   429,   430,     0,
       0,     0,   599,     0,     0,   496,   497,     0,   333,   334,
     335,     0,   337,   338,   339,   340,   341,   498,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,     0,
     355,   356,   357,     0,     0,   360,   361,   362,   363,   499,
     500,   600,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   503,   504,     0,     0,     0,
       0,   431,     0,     0,     0,   432,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   508,   509,
     510,   511,   512,     0,   513,   752,   514,   515,   516,   517,
       0,     0,     0,   518,   519,   520,   521,   522,   523,   524,
     753,   601,   526,   527,     0,     0,     0,     0,     0,     0,
     528,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     433,     0,     0,     0,   434,     0,     0,   960,   530,   531,
       0,    15,     0,     0,   532,   533,     0,     0,     0,   466,
     467,     0,   961,     0,   970,     0,   536,   537,   437,   472,
     473,   474,   475,   476,     0,     0,     0,     0,     0,   477,
       0,   479,     0,     0,     0,   482,     0,   632,     0,     0,
       0,     0,     0,   484,     0,     0,     0,     0,     0,   487,
       0,     0,   488,     0,     0,   489,     0,     0,     0,   492,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   599,
       0,     0,   496,   497,     0,   333,   334,   335,     0,   337,
     338,   339,   340,   341,   498,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,     0,   355,   356,   357,
       0,     0,   360,   361,   362,   363,   499,   500,   600,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   503,   504,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   508,   509,   510,   511,   512,
       0,   513,     0,   514,   515,   516,   517,     0,     0,     0,
     518,   519,   520,   521,   522,   523,   524,    63,   601,   526,
     527,     0,     0,     0,     0,     0,     0,   528,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   633,     0,     0,   529,   530,   531,     0,    15,     0,
       0,   532,   533,     0,     0,     0,   466,   467,     0,  1220,
       0,   535,     0,   536,   537,   635,   472,   473,   474,   475,
     476,     0,     0,     0,     0,     0,   477,     0,   479,     0,
       0,     0,   482,     0,     0,     0,     0,     0,     0,     0,
     484,     0,     0,     0,     0,     0,   487,     0,     0,   488,
       0,     0,   489,     0,     0,     0,   492,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   599,     0,     0,   496,
     497,     0,   333,   334,   335,     0,   337,   338,   339,   340,
     341,   498,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,     0,   355,   356,   357,     0,     0,   360,
     361,   362,   363,   499,   500,   501,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   503,
     504,     0,     0,     0,     0,     0,     0,     0,   678,     0,
       0,     0,     0,     0,   505,   506,   507,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   508,   509,   510,   511,   512,     0,   513,     0,
     514,   515,   516,   517,     0,     0,     0,   518,   519,   520,
     521,   522,   523,   524,    63,   601,   526,   527,     0,     0,
       0,     0,     0,     0,   528,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   529,   530,   531,     0,    15,     0,     0,   532,   533,
       0,     0,   466,   467,     0,     0,   534,     0,   535,     0,
     536,   537,   472,   473,   474,   475,   476,     0,     0,     0,
       0,     0,   477,     0,   479,     0,     0,     0,   482,     0,
       0,     0,     0,     0,     0,     0,   484,     0,     0,     0,
       0,     0,   487,     0,     0,   488,     0,     0,   489,     0,
       0,     0,   492,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   599,     0,     0,   496,   497,     0,   333,   334,
     335,     0,   337,   338,   339,   340,   341,   498,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,     0,
     355,   356,   357,     0,     0,   360,   361,   362,   363,   499,
     500,   501,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   503,   504,     0,     0,     0,
       0,     0,     0,     0,   709,     0,     0,     0,     0,     0,
     505,   506,   507,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   508,   509,
     510,   511,   512,     0,   513,     0,   514,   515,   516,   517,
       0,     0,     0,   518,   519,   520,   521,   522,   523,   524,
      63,   601,   526,   527,     0,     0,     0,     0,     0,     0,
     528,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   529,   530,   531,
       0,    15,     0,     0,   532,   533,     0,     0,   466,   467,
       0,     0,   534,     0,   535,     0,   536,   537,   472,   473,
     474,   475,   476,     0,     0,     0,     0,     0,   477,  1608,
     479,   480,     0,     0,   482,     0,     0,     0,     0,     0,
       0,     0,   484,     0,     0,     0,     0,     0,   487,     0,
       0,   488,     0,     0,   489,   490,     0,     0,   492,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   599,     0,
       0,   496,   497,     0,   333,   334,   335,     0,   337,   338,
     339,   340,   341,   498,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,     0,   355,   356,   357,     0,
       0,   360,   361,   362,   363,   499,   500,   600,  1609,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   503,   504,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   508,   509,   510,   511,   512,     0,
     513,     0,   514,   515,   516,   517,     0,     0,     0,   518,
     519,   520,   521,   522,   523,   524,    63,   601,   526,   527,
       0,     0,     0,     0,     0,     0,   528,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   529,   530,   531,     0,    15,     0,     0,
     532,   533,     0,     0,   466,   467,     0,     0,   534,     0,
     535,     0,   536,   537,   472,   473,   474,   475,   476,     0,
       0,     0,     0,     0,   477,     0,   479,     0,     0,     0,
     482,     0,     0,     0,     0,     0,     0,     0,   484,     0,
       0,     0,     0,     0,   487,     0,     0,   488,     0,     0,
     489,     0,     0,     0,   492,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   599,     0,     0,   496,   497,     0,
     333,   334,   335,     0,   337,   338,   339,   340,   341,   498,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
     353,     0,   355,   356,   357,     0,     0,   360,   361,   362,
     363,   499,   500,   501,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   503,   504,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   505,   506,   507,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     508,   509,   510,   511,   512,     0,   513,     0,   514,   515,
     516,   517,     0,     0,     0,   518,   519,   520,   521,   522,
     523,   524,    63,   601,   526,   527,     0,     0,     0,     0,
       0,     0,   528,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   529,
     530,   531,     0,    15,     0,     0,   532,   533,     0,     0,
     466,   467,     0,     0,   534,     0,   535,     0,   536,   537,
     472,   473,   474,   475,   476,     0,     0,     0,     0,     0,
     477,     0,   479,     0,     0,     0,   482,     0,     0,     0,
       0,     0,     0,     0,   484,     0,     0,     0,     0,     0,
     487,     0,     0,   488,     0,     0,   489,     0,     0,     0,
     492,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     599,     0,     0,   496,   497,  1010,   333,   334,   335,     0,
     337,   338,   339,   340,   341,   498,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,     0,   355,   356,
     357,     0,     0,   360,   361,   362,   363,   499,   500,   600,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   503,   504,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   508,   509,   510,   511,
     512,     0,   513,   752,   514,   515,   516,   517,     0,     0,
       0,   518,   519,   520,   521,   522,   523,   524,   753,   601,
     526,   527,     0,     0,     0,     0,     0,     0,   528,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   529,   530,   531,     0,    15,
       0,     0,   532,   533,     0,     0,   466,   467,     0,     0,
    1011,     0,   535,  1012,   536,   537,   472,   473,   474,   475,
     476,     0,     0,     0,     0,     0,   477,     0,   479,     0,
       0,   428,   482,     0,     0,     0,     0,     0,     0,     0,
     484,     0,     0,     0,     0,     0,   487,     0,     0,   488,
       0,     0,   489,     0,     0,     0,   492,     0,   429,   430,
       0,     0,     0,     0,     0,     0,   599,     0,     0,   496,
     497,     0,   333,   334,   335,     0,   337,   338,   339,   340,
     341,   498,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,     0,   355,   356,   357,     0,     0,   360,
     361,   362,   363,   499,   500,   501,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   503,
     504,     0,   431,     0,     0,     0,   432,     0,     0,     0,
       0,     0,     0,     0,  1031,  1032,  1033,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   508,   509,   510,   511,   512,     0,   513,     0,
     514,   515,   516,   517,     0,     0,     0,   518,   519,   520,
     521,   522,   523,   524,    63,   601,   526,   527,     0,     0,
       0,     0,     0,     0,   528,     0,     0,     0,     0,     0,
       0,   433,     0,     0,     0,   434,     0,  1377,   435,     0,
       0,   529,   530,   531,     0,    15,     0,     0,   532,   533,
       0,     0,     0,   436,   466,   467,   534,     0,   535,   437,
     536,   537,   747,     0,   472,   473,   474,   475,   476,     0,
       0,     0,     0,     0,   477,     0,   479,     0,     0,   428,
     482,     0,     0,     0,     0,     0,     0,     0,   484,     0,
       0,     0,     0,     0,   487,     0,     0,   488,   748,     0,
     489,     0,     0,     0,   492,     0,   429,   430,     0,     0,
       0,     0,     0,     0,   599,     0,     0,   496,   497,     0,
     333,   334,   335,     0,   337,   338,   339,   340,   341,   498,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
     353,     0,   355,   356,   357,     0,     0,   360,   361,   362,
     363,   499,   500,   600,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   503,   504,     0,
     431,     0,     0,     0,   432,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     508,   509,   510,   511,   512,     0,   513,     0,   514,   515,
     516,   517,     0,     0,     0,   518,   519,   520,   521,   522,
     523,   524,    63,   601,   526,   527,     0,     0,     0,     0,
       0,     0,   528,     0,     0,     0,     0,     0,     0,   433,
       0,     0,     0,   434,     0,  1378,   435,     0,     0,   529,
     530,   531,     0,    15,     0,     0,   532,   533,     0,     0,
       0,   436,   466,   467,   534,   636,   535,   437,   536,   537,
     747,     0,   472,   473,   474,   475,   476,     0,     0,     0,
       0,     0,   477,     0,   479,     0,     0,   428,   482,     0,
       0,     0,     0,     0,     0,     0,   484,     0,     0,     0,
       0,     0,   487,     0,     0,   488,   748,     0,   489,     0,
       0,     0,   492,     0,   429,   430,     0,     0,     0,     0,
       0,     0,   599,     0,     0,   496,   497,     0,   333,   334,
     335,     0,   337,   338,   339,   340,   341,   498,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,     0,
     355,   356,   357,     0,     0,   360,   361,   362,   363,   499,
     500,   600,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   503,   504,     0,   431,     0,
       0,     0,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   508,   509,
     510,   511,   512,     0,   513,   752,   514,   515,   516,   517,
       0,     0,     0,   518,   519,   520,   521,   522,   523,   524,
     753,   601,   526,   527,     0,     0,     0,     0,     0,     0,
     528,     0,     0,     0,     0,     0,     0,   433,     0,     0,
       0,   434,     0,  1380,   435,     0,     0,   529,   530,   531,
       0,    15,     0,     0,   532,   533,     0,     0,     0,   436,
     466,   467,   534,     0,   535,   437,   536,   537,   747,     0,
     472,   473,   474,   475,   476,     0,     0,     0,     0,     0,
     477,     0,   479,     0,     0,   428,   482,     0,     0,     0,
       0,     0,     0,     0,   484,     0,     0,     0,     0,     0,
     487,     0,     0,   488,   748,     0,   489,     0,     0,     0,
     492,     0,   429,   430,     0,     0,     0,     0,     0,     0,
     599,     0,     0,   496,   497,     0,   333,   334,   335,     0,
     337,   338,   339,   340,   341,   498,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,     0,   355,   356,
     357,     0,     0,   360,   361,   362,   363,   499,   500,   600,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   503,   504,     0,   431,     0,     0,     0,
     432,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   508,   509,   510,   511,
     512,     0,   513,     0,   514,   515,   516,   517,     0,     0,
       0,   518,   519,   520,   521,   522,   523,   524,    63,   601,
     526,   527,     0,     0,     0,     0,     0,     0,   528,     0,
       0,     0,     0,     0,     0,   433,     0,     0,     0,   434,
       0,  1489,   435,     0,     0,   529,   530,   531,     0,    15,
       0,     0,   532,   533,     0,     0,     0,   436,   466,   467,
     534,   870,   535,   437,   536,   537,   747,     0,   472,   473,
     474,   475,   476,     0,     0,     0,     0,     0,   477,     0,
     479,     0,     0,     0,   482,     0,     0,     0,     0,     0,
       0,     0,   484,     0,     0,     0,     0,     0,   487,     0,
       0,   488,   748,     0,   489,     0,     0,     0,   492,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   599,     0,
       0,   496,   497,     0,   333,   334,   335,     0,   337,   338,
     339,   340,   341,   498,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,     0,   355,   356,   357,     0,
       0,   360,   361,   362,   363,   499,   500,   600,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   503,   504,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   508,   509,   510,   511,   512,     0,
     513,     0,   514,   515,   516,   517,     0,     0,     0,   518,
     519,   520,   521,   522,   523,   524,    63,   601,   526,   527,
       0,     0,     0,     0,     0,     0,   528,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   529,   530,   531,     0,    15,     0,     0,
     532,   533,     0,     0,   466,   467,     0,     0,   534,     0,
     535,     0,   536,   537,   472,   473,   474,   475,   476,     0,
       0,     0,     0,     0,   477,     0,   479,     0,     0,     0,
     482,     0,     0,     0,     0,     0,     0,     0,   484,     0,
       0,     0,     0,     0,   487,     0,     0,   488,     0,     0,
     489,     0,     0,     0,   492,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   599,     0,     0,   496,   497,  1160,
     333,   334,   335,     0,   337,   338,   339,   340,   341,   498,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
     353,     0,   355,   356,   357,     0,     0,   360,   361,   362,
     363,   499,   500,   600,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   503,   504,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     508,   509,   510,   511,   512,     0,   513,   752,   514,   515,
     516,   517,     0,     0,     0,   518,   519,   520,   521,   522,
     523,   524,   753,   601,   526,   527,     0,     0,     0,     0,
       0,     0,   528,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   529,
     530,   531,     0,    15,     0,     0,   532,   533,     0,     0,
     466,   467,     0,     0,   534,     0,   535,     0,   536,   537,
     472,   473,   474,   475,   476,     0,     0,     0,     0,     0,
     477,     0,   479,     0,     0,     0,   482,     0,     0,     0,
       0,     0,     0,     0,   484,     0,     0,     0,     0,     0,
     487,     0,     0,   488,     0,     0,   489,     0,     0,     0,
     492,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     599,     0,     0,   496,   497,     0,   333,   334,   335,     0,
     337,   338,   339,   340,   341,   498,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,     0,   355,   356,
     357,     0,     0,   360,   361,   362,   363,   499,   500,   600,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   503,   504,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   508,   509,   510,   511,
     512,     0,   513,   752,   514,   515,   516,   517,     0,     0,
       0,   518,   519,   520,   521,   522,   523,   524,   753,   601,
     526,   527,     0,     0,     0,     0,     0,     0,   528,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   529,   530,   531,     0,    15,
       0,     0,   532,   533,     0,     0,   466,   467,     0,     0,
     534,     0,   535,  1222,   536,   537,   472,   473,   474,   475,
     476,     0,     0,     0,     0,     0,   477,     0,   479,     0,
       0,     0,   482,     0,     0,     0,     0,     0,     0,     0,
     484,     0,     0,     0,     0,     0,   487,     0,     0,   488,
       0,     0,   489,     0,     0,     0,   492,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   599,     0,     0,   496,
     497,     0,   333,   334,   335,     0,   337,   338,   339,   340,
     341,   498,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,     0,   355,   356,   357,     0,     0,   360,
     361,   362,   363,   499,   500,   600,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   503,
     504,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   508,   509,   510,   511,   512,     0,   513,   752,
     514,   515,   516,   517,     0,     0,     0,   518,   519,   520,
     521,   522,   523,   524,   753,   601,   526,   527,     0,     0,
       0,     0,     0,     0,   528,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   529,   530,   531,     0,    15,     0,     0,   532,   533,
       0,     0,   466,   467,     0,     0,   534,     0,   535,  1237,
     536,   537,   472,   473,   474,   475,   476,     0,     0,     0,
       0,     0,   477,     0,   479,     0,     0,     0,   482,     0,
       0,     0,     0,     0,     0,     0,   484,     0,     0,     0,
       0,     0,   487,     0,     0,   488,     0,     0,   489,     0,
       0,     0,   492,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   599,     0,     0,   496,   497,     0,   333,   334,
     335,     0,   337,   338,   339,   340,   341,   498,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,     0,
     355,   356,   357,     0,     0,   360,   361,   362,   363,   499,
     500,   600,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   503,   504,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   508,   509,
     510,   511,   512,     0,   513,     0,   514,   515,   516,   517,
       0,     0,     0,   518,   519,   520,   521,   522,   523,   524,
      63,   601,   526,   527,     0,     0,     0,     0,     0,     0,
     528,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   529,   530,   531,
       0,    15,     0,     0,   532,   533,     0,     0,   466,   467,
       0,     0,   534,   636,   535,     0,   536,   537,   472,   473,
     474,   475,   476,     0,     0,     0,     0,     0,   477,     0,
     479,     0,     0,     0,   482,     0,     0,     0,     0,     0,
       0,     0,   484,     0,     0,     0,     0,     0,   487,     0,
       0,   488,     0,     0,   489,     0,     0,     0,   492,     0,
       0,     0,     0,     0,   686,     0,     0,     0,   599,     0,
       0,   496,   497,     0,   333,   334,   335,     0,   337,   338,
     339,   340,   341,   498,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,     0,   355,   356,   357,     0,
       0,   360,   361,   362,   363,   499,   500,   600,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   503,   504,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   508,   509,   510,   511,   512,     0,
     513,     0,   514,   515,   516,   517,     0,     0,     0,   518,
     519,   520,   521,   522,   523,   524,    63,   601,   526,   527,
       0,     0,     0,     0,     0,     0,   528,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   529,   530,   531,     0,    15,     0,     0,
     532,   533,     0,     0,   466,   467,     0,     0,   534,     0,
     535,     0,   536,   537,   472,   473,   474,   475,   476,     0,
       0,     0,     0,     0,   477,     0,   479,     0,     0,     0,
     482,     0,     0,     0,     0,     0,     0,     0,   484,     0,
       0,     0,     0,     0,   487,     0,     0,   488,     0,     0,
     489,     0,     0,     0,   492,     0,     0,   698,     0,     0,
       0,     0,     0,     0,   599,     0,     0,   496,   497,     0,
     333,   334,   335,     0,   337,   338,   339,   340,   341,   498,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
     353,     0,   355,   356,   357,     0,     0,   360,   361,   362,
     363,   499,   500,   600,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   503,   504,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     508,   509,   510,   511,   512,     0,   513,     0,   514,   515,
     516,   517,     0,     0,     0,   518,   519,   520,   521,   522,
     523,   524,    63,   601,   526,   527,     0,     0,     0,     0,
       0,     0,   528,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   529,
     530,   531,     0,    15,     0,     0,   532,   533,     0,     0,
       0,     0,   466,   467,   534,     0,   535,     0,   536,   537,
     730,     0,   472,   473,   474,   475,   476,     0,     0,     0,
       0,     0,   477,     0,   479,     0,     0,     0,   482,     0,
       0,     0,     0,     0,     0,     0,   484,     0,     0,     0,
       0,     0,   487,     0,     0,   488,     0,     0,   489,     0,
       0,     0,   492,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   599,     0,     0,   496,   497,     0,   333,   334,
     335,     0,   337,   338,   339,   340,   341,   498,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,     0,
     355,   356,   357,     0,     0,   360,   361,   362,   363,   499,
     500,   600,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   503,   504,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   508,   509,
     510,   511,   512,     0,   513,     0,   514,   515,   516,   517,
       0,     0,     0,   518,   519,   520,   521,   522,   523,   524,
      63,   601,   526,   527,     0,     0,     0,     0,     0,     0,
     528,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   529,   530,   531,
       0,    15,     0,     0,   532,   533,     0,     0,   466,   467,
       0,     0,   534,     0,   535,     0,   536,   537,   472,   473,
     474,   475,   476,     0,     0,     0,     0,     0,   477,     0,
     479,     0,     0,   428,   482,     0,     0,     0,     0,     0,
       0,     0,   484,     0,     0,     0,     0,     0,   487,     0,
       0,   488,     0,     0,   489,     0,     0,     0,   492,     0,
     429,   430,     0,     0,     0,     0,     0,     0,   599,     0,
       0,   496,   497,     0,   333,   334,   335,     0,   337,   338,
     339,   340,   341,   498,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,     0,   355,   356,   357,     0,
       0,   360,   361,   362,   363,   499,   500,   600,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   503,   504,     0,   431,     0,     0,     0,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   508,   509,   510,   511,   512,     0,
     513,     0,   514,   515,   516,   517,     0,     0,     0,   518,
     519,   520,   521,   522,   523,   524,    63,   601,   526,   527,
       0,     0,     0,     0,     0,     0,   528,     0,     0,     0,
       0,     0,     0,   433,     0,     0,     0,   434,     0,  1494,
     435,   734,     0,   529,   530,   531,     0,    15,     0,     0,
     532,   533,     0,     0,     0,   436,   466,   467,   534,     0,
     535,   437,   536,   537,   738,     0,   472,   473,   474,   475,
     476,     0,     0,     0,     0,     0,   477,     0,   479,     0,
       0,     0,   482,     0,     0,     0,     0,     0,     0,     0,
     484,     0,     0,     0,     0,     0,   487,     0,     0,   488,
       0,     0,   489,     0,     0,     0,   492,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   599,     0,     0,   496,
     497,     0,   333,   334,   335,     0,   337,   338,   339,   340,
     341,   498,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,     0,   355,   356,   357,     0,     0,   360,
     361,   362,   363,   499,   500,   600,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   503,
     504,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   508,   509,   510,   511,   512,     0,   513,     0,
     514,   515,   516,   517,     0,     0,     0,   518,   519,   520,
     521,   522,   523,   524,    63,   601,   526,   527,     0,     0,
       0,     0,     0,     0,   528,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   529,   530,   531,     0,    15,     0,     0,   532,   533,
       0,     0,   466,   467,     0,     0,   534,     0,   535,     0,
     536,   537,   472,   473,   474,   475,   476,     0,     0,     0,
       0,     0,   477,     0,   479,     0,     0,     0,   482,     0,
       0,     0,     0,     0,     0,     0,   484,     0,     0,     0,
       0,     0,   487,     0,     0,   488,     0,     0,   489,     0,
       0,     0,   492,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   599,     0,     0,   496,   497,     0,   333,   334,
     335,     0,   337,   338,   339,   340,   341,   498,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,     0,
     355,   356,   357,     0,     0,   360,   361,   362,   363,   499,
     500,   600,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   503,   504,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   508,   509,
     510,   511,   512,     0,   513,   752,   514,   515,   516,   517,
       0,     0,     0,   518,   519,   520,   521,   522,   523,   524,
     753,   601,   526,   527,     0,     0,     0,     0,     0,     0,
     528,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   529,   530,   531,
       0,    15,     0,     0,   532,   533,     0,     0,   466,   467,
       0,     0,   534,     0,   535,     0,   536,   537,   472,   473,
     474,   475,   476,     0,     0,  1090,     0,     0,   477,     0,
     479,     0,     0,     0,   482,     0,     0,     0,     0,     0,
       0,     0,   484,     0,     0,     0,     0,     0,   487,     0,
       0,   488,     0,     0,   489,     0,     0,     0,   492,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   599,     0,
       0,   496,   497,     0,   333,   334,   335,     0,   337,   338,
     339,   340,   341,   498,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,     0,   355,   356,   357,     0,
       0,   360,   361,   362,   363,   499,   500,   600,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   503,   504,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   508,   509,   510,   511,   512,     0,
     513,     0,   514,   515,   516,   517,     0,     0,     0,   518,
     519,   520,   521,   522,   523,   524,    63,   601,   526,   527,
       0,     0,     0,     0,     0,     0,   528,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   529,   530,   531,     0,    15,     0,     0,
     532,   533,     0,     0,   466,   467,     0,     0,   534,     0,
     535,     0,   536,   537,   472,   473,   474,   475,   476,     0,
       0,     0,     0,     0,   477,     0,   479,     0,     0,     0,
     482,     0,     0,     0,     0,     0,     0,     0,   484,     0,
       0,     0,     0,     0,   487,     0,     0,   488,     0,     0,
     489,     0,     0,     0,   492,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   599,     0,     0,   496,   497,     0,
     333,   334,   335,     0,   337,   338,   339,   340,   341,   498,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
     353,     0,   355,   356,   357,     0,     0,   360,   361,   362,
     363,   499,   500,   600,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   503,   504,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     508,   509,   510,   511,   512,     0,   513,     0,   514,   515,
     516,   517,     0,     0,     0,   518,   519,   520,   521,   522,
     523,   524,    63,   601,   526,   527,     0,     0,     0,     0,
       0,     0,   528,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   529,
     530,   531,     0,    15,     0,     0,   532,   533,     0,     0,
     466,   467,     0,     0,   534,     0,   535,  1111,   536,   537,
     472,   473,   474,   475,   476,     0,     0,     0,     0,     0,
     477,     0,   479,     0,     0,     0,   482,     0,     0,     0,
       0,     0,     0,     0,   484,     0,     0,     0,     0,     0,
     487,     0,     0,   488,     0,     0,   489,     0,     0,     0,
     492,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     599,     0,     0,   496,   497,     0,   333,   334,   335,     0,
     337,   338,   339,   340,   341,   498,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,     0,   355,   356,
     357,     0,     0,   360,   361,   362,   363,   499,   500,   600,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   503,   504,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   508,   509,   510,   511,
     512,     0,   513,     0,   514,   515,   516,   517,     0,     0,
       0,   518,   519,   520,   521,   522,   523,   524,    63,   601,
     526,   527,     0,     0,     0,     0,     0,     0,   528,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1164,     0,   529,   530,   531,     0,    15,
       0,     0,   532,   533,     0,     0,   466,   467,     0,     0,
     534,     0,   535,     0,   536,   537,   472,   473,   474,   475,
     476,     0,     0,     0,     0,     0,   477,     0,   479,     0,
       0,     0,   482,     0,     0,     0,     0,     0,     0,     0,
     484,     0,     0,     0,     0,     0,   487,     0,     0,   488,
       0,     0,   489,     0,     0,     0,   492,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   599,     0,     0,   496,
     497,     0,   333,   334,   335,     0,   337,   338,   339,   340,
     341,   498,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,     0,   355,   356,   357,     0,     0,   360,
     361,   362,   363,   499,   500,   600,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   503,
     504,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   508,   509,   510,   511,   512,     0,   513,     0,
     514,   515,   516,   517,     0,     0,     0,   518,   519,   520,
     521,   522,   523,   524,    63,   601,   526,   527,     0,     0,
       0,     0,     0,     0,   528,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   529,   530,   531,     0,    15,     0,     0,   532,   533,
       0,     0,   466,   467,     0,     0,   534,     0,   535,  1445,
     536,   537,   472,   473,   474,   475,   476,     0,     0,     0,
       0,     0,   477,     0,   479,     0,     0,     0,   482,     0,
       0,     0,     0,     0,     0,     0,   484,     0,     0,     0,
       0,     0,   487,     0,     0,   488,     0,     0,   489,     0,
       0,     0,   492,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   599,     0,     0,   496,   497,     0,   333,   334,
     335,     0,   337,   338,   339,   340,   341,   498,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,     0,
     355,   356,   357,     0,     0,   360,   361,   362,   363,   499,
     500,   600,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   503,   504,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   508,   509,
     510,   511,   512,     0,   513,     0,   514,   515,   516,   517,
       0,     0,     0,   518,   519,   520,   521,   522,   523,   524,
      63,   601,   526,   527,     0,     0,     0,     0,     0,     0,
     528,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   529,   530,   531,
       0,    15,     0,     0,   532,   533,     0,     0,   466,   467,
       0,     0,  1454,     0,   535,  1455,   536,   537,   472,   473,
     474,   475,   476,     0,     0,     0,     0,     0,   477,     0,
     479,     0,     0,     0,   482,     0,     0,     0,     0,     0,
       0,     0,   484,     0,     0,     0,     0,     0,   487,     0,
       0,   488,     0,     0,   489,     0,     0,     0,   492,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   599,     0,
       0,   496,   497,     0,   333,   334,   335,     0,   337,   338,
     339,   340,   341,   498,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,     0,   355,   356,   357,     0,
       0,   360,   361,   362,   363,   499,   500,   600,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   503,   504,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   508,   509,   510,   511,   512,     0,
     513,     0,   514,   515,   516,   517,     0,     0,     0,   518,
     519,   520,   521,   522,   523,   524,    63,   601,   526,   527,
       0,     0,     0,     0,     0,     0,   528,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   529,   530,   531,     0,    15,     0,     0,
     532,   533,     0,     0,   466,   467,     0,     0,   534,     0,
     535,  1460,   536,   537,   472,   473,   474,   475,   476,     0,
       0,     0,     0,     0,   477,     0,   479,     0,     0,     0,
     482,     0,     0,     0,     0,     0,     0,     0,   484,     0,
       0,     0,     0,     0,   487,     0,     0,   488,     0,     0,
     489,     0,     0,     0,   492,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   599,     0,     0,   496,   497,     0,
     333,   334,   335,     0,   337,   338,   339,   340,   341,   498,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
     353,     0,   355,   356,   357,     0,     0,   360,   361,   362,
     363,   499,   500,   600,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   503,   504,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     508,   509,   510,   511,   512,     0,   513,     0,   514,   515,
     516,   517,     0,     0,     0,   518,   519,   520,   521,   522,
     523,   524,    63,   601,   526,   527,     0,     0,     0,     0,
       0,     0,   528,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   529,
     530,   531,     0,    15,     0,     0,   532,   533,     0,     0,
     466,   467,     0,     0,   534,     0,   535,  1505,   536,   537,
     472,   473,   474,   475,   476,     0,     0,     0,     0,     0,
     477,     0,   479,     0,     0,     0,   482,     0,     0,     0,
       0,     0,     0,     0,   484,     0,     0,     0,     0,     0,
     487,     0,     0,   488,     0,     0,   489,     0,     0,     0,
     492,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     599,     0,     0,   496,   497,     0,   333,   334,   335,     0,
     337,   338,   339,   340,   341,   498,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,     0,   355,   356,
     357,     0,     0,   360,   361,   362,   363,   499,   500,   600,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   503,   504,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   508,   509,   510,   511,
     512,     0,   513,     0,   514,   515,   516,   517,     0,     0,
       0,   518,   519,   520,   521,   522,   523,   524,    63,   601,
     526,   527,     0,     0,     0,     0,     0,     0,   528,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   529,   530,   531,     0,    15,
       0,     0,   532,   533,     0,     0,   466,   467,     0,     0,
     534,     0,   535,  1586,   536,   537,   472,   473,   474,   475,
     476,     0,     0,     0,     0,     0,   477,     0,   479,     0,
       0,     0,   482,     0,     0,     0,     0,     0,     0,     0,
     484,     0,     0,     0,     0,     0,   487,     0,     0,   488,
       0,     0,   489,     0,     0,     0,   492,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   599,     0,     0,   496,
     497,     0,   333,   334,   335,     0,   337,   338,   339,   340,
     341,   498,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,     0,   355,   356,   357,     0,     0,   360,
     361,   362,   363,   499,   500,   600,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   503,
     504,     0,     0,     0,     0,     0,     0,     0,  1623,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   508,   509,   510,   511,   512,     0,   513,     0,
     514,   515,   516,   517,     0,     0,     0,   518,   519,   520,
     521,   522,   523,   524,    63,   601,   526,   527,     0,     0,
       0,     0,     0,     0,   528,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   529,   530,   531,     0,    15,     0,     0,   532,   533,
       0,     0,   466,   467,     0,     0,   534,     0,   535,     0,
     536,   537,   472,   473,   474,   475,   476,     0,     0,     0,
       0,     0,   477,     0,   479,     0,     0,     0,   482,     0,
       0,     0,     0,     0,     0,     0,   484,     0,     0,     0,
       0,     0,   487,     0,     0,   488,     0,     0,   489,     0,
       0,     0,   492,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   599,     0,     0,   496,   497,     0,   333,   334,
     335,     0,   337,   338,   339,   340,   341,   498,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,     0,
     355,   356,   357,     0,     0,   360,   361,   362,   363,   499,
     500,   600,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   503,   504,     0,     0,     0,
       0,     0,     0,     0,  1666,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   508,   509,
     510,   511,   512,     0,   513,     0,   514,   515,   516,   517,
       0,     0,     0,   518,   519,   520,   521,   522,   523,   524,
      63,   601,   526,   527,     0,     0,     0,     0,     0,     0,
     528,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   529,   530,   531,
       0,    15,     0,     0,   532,   533,     0,     0,   466,   467,
       0,     0,   534,     0,   535,     0,   536,   537,   472,   473,
     474,   475,   476,     0,     0,     0,     0,     0,   477,     0,
     479,     0,     0,     0,   482,     0,     0,     0,     0,     0,
       0,     0,   484,     0,     0,     0,     0,     0,   487,     0,
       0,   488,     0,     0,   489,     0,     0,     0,   492,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   599,     0,
       0,   496,   497,     0,   333,   334,   335,     0,   337,   338,
     339,   340,   341,   498,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,     0,   355,   356,   357,     0,
       0,   360,   361,   362,   363,   499,   500,   600,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   503,   504,     0,     0,     0,     0,     0,     0,     0,
    1667,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   508,   509,   510,   511,   512,     0,
     513,     0,   514,   515,   516,   517,     0,     0,     0,   518,
     519,   520,   521,   522,   523,   524,    63,   601,   526,   527,
       0,     0,     0,     0,     0,     0,   528,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   529,   530,   531,     0,    15,     0,     0,
     532,   533,     0,     0,   466,   467,     0,     0,   534,     0,
     535,     0,   536,   537,   472,   473,   474,   475,   476,     0,
       0,     0,     0,     0,   477,     0,   479,     0,     0,     0,
     482,     0,     0,     0,     0,     0,     0,     0,   484,     0,
       0,     0,     0,     0,   487,     0,     0,   488,     0,     0,
     489,     0,     0,     0,   492,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   599,     0,     0,   496,   497,     0,
     333,   334,   335,     0,   337,   338,   339,   340,   341,   498,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
     353,     0,   355,   356,   357,     0,     0,   360,   361,   362,
     363,   499,   500,   600,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   503,   504,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     508,   509,   510,   511,   512,     0,   513,     0,   514,   515,
     516,   517,     0,     0,     0,   518,   519,   520,   521,   522,
     523,   524,    63,   601,   526,   527,     0,     0,     0,     0,
       0,     0,   528,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   529,
     530,   531,     0,    15,     0,     0,   532,   533,     0,     0,
     466,   467,     0,     0,   534,     0,   535,     0,   536,   537,
     472,   473,   474,   475,   476,     0,     0,     0,     0,     0,
     477,     0,   479,     0,     0,   428,   482,     0,     0,     0,
       0,     0,     0,     0,   484,     0,     0,     0,     0,     0,
     487,     0,     0,   488,     0,     0,   489,     0,     0,     0,
     492,     0,   429,   430,     0,     0,     0,     0,     0,     0,
     599,     0,     0,   496,   497,     0,   333,   334,   335,     0,
     337,   338,   339,   340,   341,   498,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,     0,   355,   356,
     357,     0,     0,   360,   361,   362,   363,   499,   500,   600,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   503,   504,     0,   431,     0,   -78,     0,
     432,     0,     0,     0,     0,     0,     0,     0,     0,   777,
     778,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   508,   509,   510,   511,
     512,     0,   513,     0,   514,   515,   516,   517,     0,     0,
       0,   518,   519,   520,   521,   522,   523,   524,    63,   601,
     526,   527,     0,     0,     0,     0,   777,   778,   528,     0,
       0,     0,     0,     0,     0,   433,     0,     0,     0,   434,
       0,  1528,   435,     0,     0,   529,   530,   531,     0,    15,
       0,     0,   532,   533,     0,     0,     0,   436,     0,     0,
    1428,     0,   535,   437,   536,   537,   779,   780,   781,   782,
     783,   784,   785,   786,   787,   788,   789,   790,   791,   792,
     793,   794,   795,   796,   797,   798,   799,   800,   801,   802,
       0,     0,     0,     0,   803,   804,   805,   806,   807,   808,
       0,     0,   809,   810,   811,   812,   813,   814,   815,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   787,   788,   789,   790,   791,     0,     0,   794,   795,
     796,   797,     0,   799,   800,   801,   802,     0,     0,     0,
       0,   803,     0,   805,   806,     0,     0,     0,     0,   809,
     816,   817,     0,   818,   819,   820,   821,   822,   823,   824,
     825,   826,   827,   728,     0,     0,     0,     0,     0,   327,
       0,     0,   828,   829,     0,   328,     0,   536,   714,     0,
       0,   329,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   330,     0,     0,     0,     0,     0,     0,     0,   331,
     818,   819,   820,   821,   822,   823,   824,   825,   826,   827,
       0,     0,     0,     0,   332,     0,     0,     0,     0,   828,
     829,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   327,     0,     0,     0,     0,     0,   328,     0,
       0,     0,     0,     0,   329,     0,     0,     0,     0,     0,
       0,     0,     0,    62,   330,     0,     0,     0,     0,     0,
       0,     0,   331,     0,     0,     0,   366,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   332,     0,     0,
       0,     0,     0,    63,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   367,
       0,   428,     0,     0,     0,   327,     0,     0,     0,     0,
       0,   328,     0,     0,     0,     0,     0,   329,     0,     0,
       0,     0,     0,     0,     0,     0,    62,   330,   429,   430,
       0,     0,     0,     0,     0,   331,     0,     0,     0,   366,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     332,     0,     0,     0,     0,     0,    63,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
       0,     0,   431,     0,     0,     0,   432,     0,     0,     0,
       0,     0,   367,     0,   639,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   366,     0,     0,     0,     0,     0,     0,     0,
       0,    13,     0,     0,   327,     0,     0,     0,     0,   642,
     328,   433,     0,     0,     0,   434,   329,  1625,   435,     0,
       0,    14,     0,     0,     0,     0,   330,   643,     0,     0,
       0,     0,     0,   436,   331,     0,     0,     0,     0,   437,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   332,
       0,     0,     0,     0,     0,   367,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   364,   365,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   327,     0,     0,
       0,     0,     0,   328,     0,     0,     0,     0,     0,   329,
       0,     0,     0,     0,     0,     0,     0,     0,    62,   330,
       0,     0,     0,     0,     0,     0,     0,   331,     0,     0,
       0,   366,     0,     0,     0,     0,     0,     0,     0,     0,
     777,   778,   332,     0,     0,     0,     0,     0,    63,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   365,     0,   777,   778,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   367,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   366,   787,   788,   789,   790,   791,
       0,     0,   794,   795,   796,   797,     0,   799,   800,   801,
     802,   642,     0,     0,     0,   803,     0,   805,   806,     0,
       0,     0,     0,   809,     0,   811,     0,     0,     0,     0,
     909,   910,   911,   912,   913,   914,   915,   916,   787,   788,
     789,   790,   791,   917,   918,   794,   795,   796,   797,   919,
     799,   800,   801,   802,     0,     0,     0,   367,   803,   804,
     805,   806,   920,   921,   777,   778,   809,   810,   811,   922,
     923,   924,   815,     0,   818,   819,   820,   821,   822,   823,
     824,   825,   826,   827,     0,     0,     0,    13,     0,     0,
       0,     0,     0,   828,   829,     0,     0,     0,     0,   777,
     778,     0,     0,     0,     0,     0,     0,    14,     0,     0,
       0,     0,     0,     0,   925,   817,     0,   818,   819,   820,
     821,   822,   823,   824,   825,   826,   827,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   828,   829,     0,     0,
       0,   536,   714,     0,     0,     0,     0,     0,     0,     0,
       0,   909,   910,   911,   912,   913,   914,   915,   916,   787,
     788,   789,   790,   791,   917,   918,   794,   795,   796,   797,
     919,   799,   800,   801,   802,  -392,     0,     0,     0,   803,
     804,   805,   806,   920,   921,     0,     0,   809,   810,   811,
     922,   923,   924,   815,   787,   788,   789,   790,   791,     0,
       0,   794,   795,   796,   797,     0,   799,   800,   801,   802,
     777,   778,     0,     0,   803,     0,   805,   806,     0,     0,
       0,     0,   809,   810,   811,     0,     0,     0,   815,     0,
       0,     0,     0,     0,     0,   925,   817,     0,   818,   819,
     820,   821,   822,   823,   824,   825,   826,   827,   777,   778,
       0,     0,     0,     0,     0,     0,     0,   828,   829,     0,
       0,     0,   536,   714,     0,     0,     0,     0,     0,     0,
       0,   817,     0,   818,   819,   820,   821,   822,   823,   824,
     825,   826,   827,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   828,   829,     0,     0,   842,     0,     0,     0,
       0,     0,     0,     0,     0,   787,   788,   789,   790,   791,
       0,     0,   794,   795,   796,   797,     0,   799,   800,   801,
     802,     0,     0,     0,     0,   803,     0,   805,   806,     0,
       0,     0,     0,   809,   810,   811,     0,     0,     0,   815,
       0,     0,     0,   787,   788,   789,   790,   791,     0,     0,
     794,   795,   796,   797,     0,   799,   800,   801,   802,   777,
     778,     0,     0,   803,     0,   805,   806,     0,     0,     0,
       0,   809,   810,   811,     0,     0,     0,   815,     0,     0,
       0,     0,   817,     0,   818,   819,   820,   821,   822,   823,
     824,   825,   826,   827,   777,   778,     0,     0,     0,     0,
       0,     0,     0,   828,   829,     0,     0,   852,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     817,     0,   818,   819,   820,   821,   822,   823,   824,   825,
     826,   827,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   828,   829,     0,     0,   867,     0,     0,     0,     0,
       0,     0,     0,     0,   787,   788,   789,   790,   791,     0,
       0,   794,   795,   796,   797,     0,   799,   800,   801,   802,
       0,     0,     0,     0,   803,     0,   805,   806,     0,     0,
       0,     0,   809,   810,   811,     0,     0,     0,   815,   787,
     788,   789,   790,   791,     0,     0,   794,   795,   796,   797,
       0,   799,   800,   801,   802,   777,   778,     0,     0,   803,
       0,   805,   806,     0,     0,     0,     0,   809,   810,   811,
       0,     0,     0,   815,     0,     0,     0,     0,     0,     0,
       0,   817,     0,   818,   819,   820,   821,   822,   823,   824,
     825,   826,   827,   777,   778,     0,     0,     0,     0,     0,
       0,     0,   828,   829,     0,     0,   882,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   817,     0,   818,   819,
     820,   821,   822,   823,   824,   825,   826,   827,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   828,   829,     0,
       0,  1196,     0,     0,     0,     0,     0,     0,     0,     0,
     787,   788,   789,   790,   791,     0,     0,   794,   795,   796,
     797,     0,   799,   800,   801,   802,     0,     0,     0,     0,
     803,     0,   805,   806,     0,     0,     0,     0,   809,   810,
     811,     0,     0,     0,   815,     0,     0,     0,   787,   788,
     789,   790,   791,     0,     0,   794,   795,   796,   797,     0,
     799,   800,   801,   802,   777,   778,     0,     0,   803,     0,
     805,   806,     0,     0,     0,     0,   809,   810,   811,     0,
       0,     0,   815,     0,     0,     0,     0,   817,     0,   818,
     819,   820,   821,   822,   823,   824,   825,   826,   827,   777,
     778,     0,     0,     0,     0,     0,     0,     0,   828,   829,
       0,     0,  1201,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   817,     0,   818,   819,   820,
     821,   822,   823,   824,   825,   826,   827,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   828,   829,     0,     0,
    1203,     0,     0,     0,     0,     0,     0,     0,     0,   787,
     788,   789,   790,   791,     0,     0,   794,   795,   796,   797,
       0,   799,   800,   801,   802,     0,     0,     0,     0,   803,
       0,   805,   806,     0,     0,     0,     0,   809,   810,   811,
       0,     0,     0,   815,   787,   788,   789,   790,   791,     0,
       0,   794,   795,   796,   797,     0,   799,   800,   801,   802,
     777,   778,     0,     0,   803,     0,   805,   806,     0,     0,
       0,     0,   809,   810,   811,     0,     0,     0,   815,     0,
       0,     0,     0,     0,     0,     0,   817,     0,   818,   819,
     820,   821,   822,   823,   824,   825,   826,   827,   777,   778,
       0,     0,     0,     0,     0,     0,     0,   828,   829,     0,
       0,  1212,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   817,     0,   818,   819,   820,   821,   822,   823,   824,
     825,   826,   827,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   828,   829,     0,     0,  1213,     0,     0,     0,
       0,     0,     0,     0,     0,   787,   788,   789,   790,   791,
       0,     0,   794,   795,   796,   797,     0,   799,   800,   801,
     802,     0,     0,     0,     0,   803,     0,   805,   806,     0,
       0,     0,     0,   809,   810,   811,     0,     0,     0,   815,
       0,     0,     0,   787,   788,   789,   790,   791,     0,     0,
     794,   795,   796,   797,     0,   799,   800,   801,   802,   777,
     778,     0,     0,   803,     0,   805,   806,     0,     0,     0,
       0,   809,   810,   811,     0,     0,     0,   815,     0,     0,
       0,     0,   817,     0,   818,   819,   820,   821,   822,   823,
     824,   825,   826,   827,   777,   778,     0,     0,     0,     0,
       0,     0,     0,   828,   829,     0,     0,  1214,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     817,     0,   818,   819,   820,   821,   822,   823,   824,   825,
     826,   827,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   828,   829,     0,     0,  1215,     0,     0,     0,     0,
       0,     0,     0,     0,   787,   788,   789,   790,   791,     0,
       0,   794,   795,   796,   797,     0,   799,   800,   801,   802,
       0,     0,     0,     0,   803,     0,   805,   806,     0,     0,
       0,     0,   809,   810,   811,     0,     0,     0,   815,   787,
     788,   789,   790,   791,     0,     0,   794,   795,   796,   797,
       0,   799,   800,   801,   802,   777,   778,     0,     0,   803,
       0,   805,   806,     0,     0,     0,     0,   809,   810,   811,
       0,     0,     0,   815,     0,     0,     0,     0,     0,     0,
       0,   817,     0,   818,   819,   820,   821,   822,   823,   824,
     825,   826,   827,   777,   778,     0,     0,     0,     0,     0,
       0,     0,   828,   829,     0,     0,  1216,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   817,     0,   818,   819,
     820,   821,   822,   823,   824,   825,   826,   827,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   828,   829,     0,
       0,  1217,     0,     0,     0,     0,     0,     0,     0,     0,
     787,   788,   789,   790,   791,     0,     0,   794,   795,   796,
     797,     0,   799,   800,   801,   802,     0,     0,     0,     0,
     803,     0,   805,   806,     0,     0,     0,     0,   809,   810,
     811,     0,     0,     0,   815,     0,     0,     0,   787,   788,
     789,   790,   791,     0,     0,   794,   795,   796,   797,     0,
     799,   800,   801,   802,   777,   778,     0,     0,   803,     0,
     805,   806,     0,     0,     0,     0,   809,   810,   811,     0,
       0,     0,   815,     0,     0,     0,     0,   817,     0,   818,
     819,   820,   821,   822,   823,   824,   825,   826,   827,   777,
     778,     0,     0,     0,     0,     0,     0,     0,   828,   829,
       0,     0,  1349,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   817,     0,   818,   819,   820,
     821,   822,   823,   824,   825,   826,   827,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   828,   829,     0,     0,
    1361,     0,     0,     0,     0,     0,     0,     0,     0,   787,
     788,   789,   790,   791,     0,     0,   794,   795,   796,   797,
       0,   799,   800,   801,   802,     0,     0,     0,     0,   803,
       0,   805,   806,     0,     0,     0,     0,   809,   810,   811,
       0,     0,     0,   815,   787,   788,   789,   790,   791,     0,
       0,   794,   795,   796,   797,     0,   799,   800,   801,   802,
     777,   778,     0,     0,   803,     0,   805,   806,     0,     0,
       0,     0,   809,   810,   811,     0,     0,     0,   815,     0,
       0,     0,     0,     0,     0,     0,   817,     0,   818,   819,
     820,   821,   822,   823,   824,   825,   826,   827,   777,   778,
       0,     0,     0,     0,     0,     0,     0,   828,   829,     0,
       0,  1364,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   817,     0,   818,   819,   820,   821,   822,   823,   824,
     825,   826,   827,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   828,   829,     0,     0,  1412,     0,     0,     0,
       0,     0,     0,     0,     0,   787,   788,   789,   790,   791,
       0,     0,   794,   795,   796,   797,     0,   799,   800,   801,
     802,     0,     0,     0,     0,   803,     0,   805,   806,     0,
       0,     0,     0,   809,   810,   811,     0,     0,     0,   815,
       0,     0,     0,   787,   788,   789,   790,   791,     0,     0,
     794,   795,   796,   797,     0,   799,   800,   801,   802,   777,
     778,     0,     0,   803,     0,   805,   806,     0,     0,     0,
       0,   809,   810,   811,     0,     0,     0,   815,     0,     0,
       0,     0,   817,     0,   818,   819,   820,   821,   822,   823,
     824,   825,   826,   827,   777,   778,     0,     0,     0,     0,
       0,     0,     0,   828,   829,     0,     0,  1526,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     817,     0,   818,   819,   820,   821,   822,   823,   824,   825,
     826,   827,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   828,   829,     0,     0,  1527,     0,     0,     0,     0,
       0,     0,     0,     0,   787,   788,   789,   790,   791,     0,
       0,   794,   795,   796,   797,     0,   799,   800,   801,   802,
       0,     0,     0,     0,   803,     0,   805,   806,     0,     0,
       0,     0,   809,   810,   811,     0,     0,     0,   815,   787,
     788,   789,   790,   791,     0,     0,   794,   795,   796,   797,
       0,   799,   800,   801,   802,   777,   778,     0,     0,   803,
       0,   805,   806,     0,     0,     0,     0,   809,   810,   811,
       0,     0,     0,   815,     0,     0,     0,     0,     0,     0,
       0,   817,     0,   818,   819,   820,   821,   822,   823,   824,
     825,   826,   827,   777,   778,     0,     0,     0,     0,     0,
       0,     0,   828,   829,     0,     0,  1548,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   817,     0,   818,   819,
     820,   821,   822,   823,   824,   825,   826,   827,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   828,   829,     0,
       0,  1550,     0,     0,     0,     0,     0,     0,     0,     0,
     787,   788,   789,   790,   791,     0,     0,   794,   795,   796,
     797,     0,   799,   800,   801,   802,     0,     0,     0,     0,
     803,     0,   805,   806,     0,     0,     0,     0,   809,   810,
     811,     0,     0,     0,   815,     0,     0,     0,   787,   788,
     789,   790,   791,     0,     0,   794,   795,   796,   797,     0,
     799,   800,   801,   802,   777,   778,     0,     0,   803,     0,
     805,   806,     0,     0,     0,     0,   809,   810,   811,     0,
       0,     0,   815,     0,     0,     0,     0,   817,     0,   818,
     819,   820,   821,   822,   823,   824,   825,   826,   827,   777,
     778,     0,     0,     0,     0,     0,     0,     0,   828,   829,
       0,     0,  1552,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   817,     0,   818,   819,   820,
     821,   822,   823,   824,   825,   826,   827,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   828,   829,     0,     0,
    1556,     0,     0,     0,     0,     0,     0,     0,     0,   787,
     788,   789,   790,   791,     0,     0,   794,   795,   796,   797,
       0,   799,   800,   801,   802,     0,     0,     0,     0,   803,
       0,   805,   806,     0,     0,     0,     0,   809,   810,   811,
       0,     0,     0,   815,   787,   788,   789,   790,   791,     0,
       0,   794,   795,   796,   797,     0,   799,   800,   801,   802,
     777,   778,     0,     0,   803,     0,   805,   806,     0,     0,
       0,     0,   809,   810,   811,     0,     0,     0,   815,     0,
       0,     0,     0,     0,     0,     0,   817,     0,   818,   819,
     820,   821,   822,   823,   824,   825,   826,   827,   777,   778,
       0,     0,     0,     0,     0,     0,     0,   828,   829,     0,
       0,  1604,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   817,     0,   818,   819,   820,   821,   822,   823,   824,
     825,   826,   827,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   828,   829,     0,     0,  1628,     0,     0,     0,
       0,     0,     0,     0,     0,   787,   788,   789,   790,   791,
       0,     0,   794,   795,   796,   797,     0,   799,   800,   801,
     802,     0,     0,     0,     0,   803,     0,   805,   806,     0,
       0,     0,     0,   809,   810,   811,     0,     0,     0,   815,
       0,     0,     0,   787,   788,   789,   790,   791,     0,     0,
     794,   795,   796,   797,     0,   799,   800,   801,   802,   777,
     778,     0,     0,   803,     0,   805,   806,     0,     0,     0,
       0,   809,   810,   811,     0,     0,     0,   815,     0,     0,
       0,     0,   817,     0,   818,   819,   820,   821,   822,   823,
     824,   825,   826,   827,   777,   778,     0,     0,     0,     0,
       0,     0,     0,   828,   829,     0,     0,  1629,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     817,     0,   818,   819,   820,   821,   822,   823,   824,   825,
     826,   827,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   828,   829,     0,     0,  1631,     0,     0,     0,     0,
       0,     0,     0,     0,   787,   788,   789,   790,   791,     0,
       0,   794,   795,   796,   797,     0,   799,   800,   801,   802,
       0,     0,     0,     0,   803,     0,   805,   806,     0,     0,
       0,     0,   809,   810,   811,     0,     0,     0,   815,   787,
     788,   789,   790,   791,     0,     0,   794,   795,   796,   797,
       0,   799,   800,   801,   802,   777,   778,     0,     0,   803,
       0,   805,   806,     0,     0,     0,     0,   809,   810,   811,
       0,     0,     0,   815,     0,     0,     0,     0,     0,     0,
       0,   817,     0,   818,   819,   820,   821,   822,   823,   824,
     825,   826,   827,   777,   778,     0,     0,     0,     0,     0,
       0,     0,   828,   829,     0,     0,  1640,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   817,     0,   818,   819,
     820,   821,   822,   823,   824,   825,   826,   827,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   828,   829,     0,
       0,  1643,     0,     0,     0,     0,     0,     0,     0,     0,
     787,   788,   789,   790,   791,     0,     0,   794,   795,   796,
     797,     0,   799,   800,   801,   802,     0,     0,     0,     0,
     803,     0,   805,   806,     0,     0,     0,     0,   809,   810,
     811,     0,     0,     0,   815,     0,     0,     0,   787,   788,
     789,   790,   791,     0,     0,   794,   795,   796,   797,     0,
     799,   800,   801,   802,   777,   778,     0,     0,   803,     0,
     805,   806,     0,     0,     0,     0,   809,   810,   811,     0,
       0,     0,   815,     0,     0,     0,     0,   817,     0,   818,
     819,   820,   821,   822,   823,   824,   825,   826,   827,   777,
     778,     0,     0,     0,     0,     0,     0,     0,   828,   829,
       0,     0,  1652,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   817,     0,   818,   819,   820,
     821,   822,   823,   824,   825,   826,   827,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   828,   829,     0,     0,
    1727,     0,     0,     0,     0,     0,     0,     0,     0,   787,
     788,   789,   790,   791,     0,     0,   794,   795,   796,   797,
       0,   799,   800,   801,   802,     0,     0,     0,     0,   803,
       0,   805,   806,     0,     0,     0,     0,   809,   810,   811,
       0,     0,     0,   815,   787,   788,   789,   790,   791,     0,
       0,   794,   795,   796,   797,     0,   799,   800,   801,   802,
     777,   778,     0,     0,   803,     0,   805,   806,     0,     0,
       0,     0,   809,   810,   811,     0,     0,     0,   815,     0,
       0,     0,     0,     0,     0,     0,   817,     0,   818,   819,
     820,   821,   822,   823,   824,   825,   826,   827,   777,   778,
       0,     0,     0,     0,     0,     0,     0,   828,   829,     0,
       0,  1728,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   817,     0,   818,   819,   820,   821,   822,   823,   824,
     825,   826,   827,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   828,   829,   871,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   787,   788,   789,   790,   791,
       0,     0,   794,   795,   796,   797,     0,   799,   800,   801,
     802,     0,     0,     0,     0,   803,     0,   805,   806,     0,
       0,     0,     0,   809,   810,   811,     0,     0,     0,   815,
       0,     0,     0,   787,   788,   789,   790,   791,     0,     0,
     794,   795,   796,   797,     0,   799,   800,   801,   802,   777,
     778,     0,     0,   803,     0,   805,   806,     0,     0,     0,
       0,   809,   810,   811,     0,     0,     0,   815,     0,     0,
       0,     0,   817,     0,   818,   819,   820,   821,   822,   823,
     824,   825,   826,   827,   777,   778,     0,     0,     0,     0,
       0,     0,     0,   828,   829,  1145,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     817,     0,   818,   819,   820,   821,   822,   823,   824,   825,
     826,   827,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   828,   829,  1299,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   787,   788,   789,   790,   791,     0,
       0,   794,   795,   796,   797,     0,   799,   800,   801,   802,
       0,     0,     0,     0,   803,     0,   805,   806,     0,     0,
       0,     0,   809,   810,   811,     0,     0,     0,   815,   787,
     788,   789,   790,   791,     0,     0,   794,   795,   796,   797,
       0,   799,   800,   801,   802,   777,   778,     0,     0,   803,
       0,   805,   806,     0,     0,     0,     0,   809,   810,   811,
       0,     0,     0,   815,     0,     0,     0,     0,     0,     0,
       0,   817,     0,   818,   819,   820,   821,   822,   823,   824,
     825,   826,   827,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   828,   829,  1315,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   817,     0,   818,   819,
     820,   821,   822,   823,   824,   825,   826,   827,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   828,   829,  1452,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     787,   788,   789,   790,   791,     0,     0,   794,   795,   796,
     797,     0,   799,   800,   801,   802,   246,   247,     0,     0,
     803,     0,   805,   806,     0,     0,     0,     0,   809,   810,
     811,     0,     0,   248,   815,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   777,   778,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   817,     0,   818,
     819,   820,   821,   822,   823,   824,   825,   826,   827,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   828,   829,
    1458,     0,     0,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,     0,     0,   267,   268,   269,     0,     0,     0,     0,
       0,     0,   270,   271,   272,   273,   274,     0,     0,   275,
     276,   277,   278,   279,   280,   281,     0,     0,     0,     0,
       0,   777,   778,   787,   788,   789,   790,   791,     0,     0,
     794,   795,   796,   797,     0,   799,   800,   801,   802,     0,
       0,     0,     0,   803,     0,   805,   806,     0,     0,     0,
       0,   809,   810,   811,     0,     0,     0,   815,   282,     0,
     283,   284,   285,   286,   287,   288,   289,   290,   291,   292,
      50,     0,   293,   294,     0,     0,     0,     0,     0,   295,
     296,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     817,     0,   818,   819,   820,   821,   822,   823,   824,   825,
     826,   827,     0,     0,   777,   778,   787,   788,   789,   790,
     791,   828,   829,   794,   795,   796,   797,     0,   799,   800,
     801,   802,     0,     0,     0,     0,   803,     0,   805,   806,
       0,     0,     0,     0,   809,   810,   811,     0,     0,     0,
     815,   777,   778,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    13,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    14,     0,     0,     0,     0,
       0,     0,     0,   817,     0,   818,   819,   820,   821,   822,
     823,   824,   825,   826,   827,     0,     0,     0,     0,   787,
     788,   789,   790,   791,   828,   829,   794,   795,   796,   797,
       0,   799,   800,   801,   802,     0,     0,     0,     0,   803,
       0,   805,   806,     0,     0,   974,     0,   809,   810,   811,
       0,     0,     0,   815,   777,   778,   787,   788,   789,   790,
     791,     0,     0,   794,   795,   796,   797,     0,   799,   800,
     801,   802,     0,     0,     0,     0,   803,     0,   805,   806,
       0,     0,  1223,     0,   809,   810,   811,     0,     0,     0,
     815,   777,   778,     0,     0,     0,   817,     0,   818,   819,
     820,   821,   822,   823,   824,   825,   826,   827,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   828,   829,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   817,     0,   818,   819,   820,   821,   822,
     823,   824,   825,   826,   827,     0,     0,     0,     0,   787,
     788,   789,   790,   791,   828,   829,   794,   795,   796,   797,
       0,   799,   800,   801,   802,     0,     0,     0,     0,   803,
       0,   805,   806,     0,     0,     0,     0,   809,   810,   811,
       0,     0,     0,   815,   777,   778,   787,   788,   789,   790,
     791,     0,     0,   794,   795,   796,   797,     0,   799,   800,
     801,   802,     0,     0,     0,     0,   803,     0,   805,   806,
       0,     0,     0,     0,   809,   810,   811,     0,     0,     0,
     815,   777,   778,     0,     0,     0,   817,  1304,   818,   819,
     820,   821,   822,   823,   824,   825,   826,   827,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   828,   829,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   817,  1407,   818,   819,   820,   821,   822,
     823,   824,   825,   826,   827,     0,     0,     0,     0,   787,
     788,   789,   790,   791,   828,   829,   794,   795,   796,   797,
       0,   799,   800,   801,   802,     0,     0,     0,     0,   803,
       0,   805,   806,     0,     0,     0,     0,   809,   810,   811,
       0,     0,     0,   815,   777,   778,   787,   788,   789,   790,
     791,     0,     0,   794,   795,   796,   797,     0,   799,   800,
     801,   802,     0,     0,     0,     0,   803,     0,   805,   806,
       0,     0,     0,     0,   809,   810,   811,     0,  1653,     0,
     815,   777,   778,     0,     0,     0,   817,     0,   818,   819,
     820,   821,   822,   823,   824,   825,   826,   827,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   828,   829,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   817,     0,   818,   819,   820,   821,   822,
     823,   824,   825,   826,   827,     0,     0,     0,     0,   787,
     788,   789,   790,   791,   828,   829,   794,   795,   796,   797,
       0,   799,   800,   801,   802,     0,     0,     0,     0,   803,
       0,   805,   806,     0,     0,     0,     0,   809,   810,   811,
       0,     0,     0,  -805,   777,   778,   787,   788,   789,   790,
     791,     0,     0,   794,   795,   796,   797,     0,   799,   800,
     801,   802,     0,     0,     0,     0,   803,     0,   805,   806,
     777,   778,     0,     0,   809,   810,   811,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   817,     0,   818,   819,
     820,   821,   822,   823,   824,   825,   826,   827,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   828,   829,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   817,     0,   818,   819,   820,   821,   822,
     823,   824,   825,   826,   827,     0,     0,     0,     0,   787,
     788,   789,   790,   791,   828,   829,   794,   795,   796,   797,
       0,   799,   800,   801,   802,     0,     0,     0,     0,   803,
       0,   805,   806,     0,     0,   787,   788,   789,   790,   791,
       0,     0,   794,   795,   796,   797,     0,   799,   800,   801,
     802,     0,     0,     0,     0,   803,     0,   805,   806,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1023,     0,     0,     0,     0,   818,   819,
     820,   821,   822,   823,   824,   825,   826,   827,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   828,   829,     0,
       0,     0,     0,     0,  1027,   819,   820,   821,   822,   823,
     824,   825,   826,   827,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   828,   829,   333,   334,   335,     0,   337,
     338,   339,   340,   341,   498,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,     0,   355,   356,   357,
       0,     0,   360,   361,   362,   363,   333,   334,   335,     0,
     337,   338,   339,   340,   341,   498,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,     0,   355,   356,
     357,     0,     0,   360,   361,   362,   363,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1300,     0,     0,     0,     0,     0,     0,
       0,     0,  1024,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1025,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1028,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   333,   334,   335,  1029,   337,
     338,   339,   340,   341,   498,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,     0,   355,   356,   357,
       0,     0,   360,   361,   362,   363,   333,   334,   335,     0,
     337,   338,   339,   340,   341,   498,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,     0,   355,   356,
     357,     0,   194,   360,   361,   362,   363,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1301,     0,  1102,  1103,     0,     0,   195,     0,
     196,     0,   197,   198,   199,   200,   201,  1302,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,     0,
     213,   214,   215,  1104,     0,   216,   217,   218,   219,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1105,     0,
       0,     0,     0,     0,     0,   220,   221,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1106,
    1107,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     222
};

static const yytype_int16 yycheck[] =
{
       1,   224,   442,   606,   397,   397,   230,   478,   517,   555,
     750,   629,   658,   685,   754,   755,   171,   441,    19,   410,
     572,    15,    16,   729,   620,     7,   622,   766,   624,    86,
     965,   502,   240,   404,   946,   951,   971,  1226,   665,   506,
     507,   944,     8,   727,   596,   729,    33,    19,   439,    22,
      34,    53,     5,   143,    57,   858,    20,   860,     4,   862,
      63,   240,    33,    20,    15,    16,    20,   127,    20,    19,
      20,    20,    20,   109,    68,    69,    70,    20,  1431,    63,
     144,   145,   146,   173,    57,   478,  1592,   397,  1594,  1639,
     163,    53,   143,  1599,  1600,    96,   173,    98,   127,   726,
    1650,   728,   127,   730,   191,   173,   143,   152,   137,   502,
     183,   738,   127,   191,   143,   160,   110,   111,   112,   113,
     747,  1632,   137,   107,   515,   516,     0,   217,   173,   216,
      53,   191,   192,     7,   194,   166,   173,   197,   216,   610,
     217,  1691,  1692,   194,   217,    15,    16,   129,   149,   102,
     103,   219,   136,   160,   218,  1661,    30,   896,    32,   143,
      34,   164,   317,   192,   716,   905,    40,   192,   478,    62,
     195,  1682,   217,   397,    97,   232,    50,   192,   441,   442,
      46,   184,    56,   185,  1537,   191,   168,   395,  1499,   173,
       5,     6,   502,     8,  1605,   231,   158,   158,   164,   165,
    1389,   173,   942,   129,   130,   945,    80,   678,   190,   191,
     197,   173,   173,   220,   160,   166,   395,   610,   184,   185,
     171,    36,   173,   173,   208,   176,   197,   191,   102,   103,
     203,   410,   233,   217,   191,     5,     6,   191,   709,   191,
     106,   214,   191,   191,   215,   198,   240,   155,   191,   215,
     221,   191,   160,   158,  1665,  1566,  1567,   218,   481,  1171,
     439,   243,   441,   320,   217,   152,   217,  1170,   173,   194,
     155,  1010,   495,  1584,  1585,   160,   216,   194,   505,   506,
     507,   198,   163,     7,   191,   678,   173,  1222,   214,   215,
     158,   165,   107,   684,  1210,   126,   166,   127,   129,   130,
     610,   171,   183,   173,   127,   173,   176,   137,   775,   216,
     218,   185,   220,   127,   137,   127,   709,   708,   191,  1508,
     165,   127,   165,   137,   132,   137,    50,  1011,   555,   311,
     204,   137,   314,  1644,  1645,   220,   515,   516,   219,   220,
     185,   215,   185,   173,   142,   173,   174,   175,   191,   217,
     152,   188,   189,    57,  1270,   582,   199,   165,   144,    63,
     146,   165,   192,   990,   191,   163,   191,   197,   678,   192,
     594,   173,   596,   367,   845,   220,   204,   185,   192,   216,
     192,   185,   645,   214,   215,   183,   192,   650,   173,   216,
     191,   392,   393,   218,   163,   199,   397,   158,   399,   709,
     163,   395,   773,   611,   612,   613,   400,   615,   616,   143,
    1326,   619,   173,   621,   183,   623,   410,   218,   152,   192,
     183,  1160,   191,   157,   648,  1021,   143,   185,   652,   191,
     188,   194,   611,   612,   613,   152,   615,   616,   191,   173,
     619,   664,   621,   198,   623,   439,   625,   441,   184,   185,
     841,   187,   845,  1388,   216,   682,   173,  1009,   216,   165,
     191,   143,   217,  1398,   198,   218,   689,   976,   191,   779,
     780,   781,   782,   783,   784,   785,   786,   198,   987,   185,
     474,   475,   792,   793,   711,   191,  1289,   218,   798,   165,
     491,   165,   959,   199,   165,   218,   217,   807,   808,   191,
     891,   892,   812,   813,   814,   684,   816,   191,  1154,   185,
     165,   185,   735,   904,   185,   191,  1222,   158,   192,   185,
     191,   515,   516,   199,   216,    68,    69,    70,   199,   708,
     185,  1237,   173,   199,   218,   845,   191,   760,  1222,   540,
     541,  1225,   163,   934,   935,   191,   937,   979,   775,   981,
     941,   165,   943,  1237,   217,   165,   191,   558,   165,   560,
     561,   562,   183,   564,  1031,  1032,  1033,   110,   111,   112,
     113,   185,   218,   165,   575,   185,   165,   191,   185,    33,
     185,   191,  1385,   218,   191,   199,   191,   191,   851,   199,
     166,   592,   199,   185,   194,   596,   185,   197,   777,   778,
     192,   191,   191,   866,   173,   606,    60,    61,   191,   191,
     199,   191,  1258,   165,   218,  1211,  1419,   611,   612,   613,
     847,   615,   616,   191,   803,   619,   216,   621,    33,   623,
     214,   625,   173,   185,   198,   218,   218,   165,   218,   191,
     641,   165,    57,   158,   173,   173,   173,   199,    63,   828,
     218,   875,   165,   217,   194,    60,    61,   185,   173,   199,
     868,   185,   841,   191,   927,   195,  1262,   191,   876,   199,
     124,   879,   185,   127,   128,   199,   670,   217,   191,   173,
     888,   158,   195,   137,   165,   840,   165,   895,   478,   868,
     684,   195,   900,   173,   173,   199,   173,   876,   165,  1371,
     879,  1141,   194,   926,   185,   197,   185,   164,   165,   888,
     191,   165,   891,   892,   708,  1139,   895,   164,   185,   124,
      79,   900,   949,   128,   194,   904,   177,   184,   194,   199,
    1533,   185,   959,   199,    47,    94,   194,   184,   192,   193,
      99,   199,   101,   197,  1428,   535,   200,   217,   173,   165,
      57,   217,   208,   198,    67,   934,   935,   173,   937,   217,
     198,   215,   941,   757,   943,   944,  1412,   221,   177,   185,
    1454,   979,   217,   981,    12,   144,    57,    57,  1169,   217,
     762,   191,    63,    63,   194,    23,    24,   197,   193,   106,
     184,    57,   197,   187,   199,   200,   190,    63,   219,   220,
     979,    57,   981,  1194,  1031,  1032,  1033,    63,    57,  1036,
     215,  1038,   185,  1040,    63,  1042,   221,  1044,   191,  1046,
     185,  1048,   217,  1050,   219,   220,   191,   185,  1055,  1425,
    1057,   832,   192,   191,   185,   185,  1063,    66,  1384,   195,
     191,   191,   836,   199,   177,   178,   847,   841,  1075,   195,
    1077,    92,    93,   199,   195,  1082,  1474,  1084,   199,  1086,
     195,   195,  1089,   195,   199,   199,   194,   199,    35,   195,
    1261,   195,   873,   199,   868,   199,  1269,  1269,    35,  1142,
     195,   217,   876,   173,   199,   879,   216,   677,   678,  1113,
     177,   178,   179,   180,   888,  1122,   173,   891,   892,   173,
     173,   895,    33,    22,   173,  1577,   900,    33,   216,  1649,
     904,  1090,   194,   703,    21,    22,   706,  1657,   194,  1127,
     144,   145,   146,   173,   174,   175,   177,   178,   179,    60,
      61,   177,   178,   179,    60,    61,  1539,   184,   185,   186,
     934,   935,   217,   937,    43,  1685,  1209,   941,  1127,   943,
     944,   460,   461,   462,  1638,  1639,   198,  1575,  1604,  1269,
    1139,   198,  1646,   198,   217,   198,  1650,  1651,    10,    11,
      12,   198,   217,   198,   217,   198,   766,   198,  1596,   198,
       5,     6,   198,  1246,  1247,   979,   195,   981,   173,  1197,
    1169,  1170,   218,   124,    21,    22,   216,   128,   124,  1683,
      25,   173,   128,    33,  1513,   217,    31,  1691,  1692,  1518,
     173,   118,   119,   173,   198,  1194,  1619,  1620,  1197,   126,
     215,   128,   129,   130,   131,   198,   217,   198,    75,   136,
      60,    61,    79,   198,   198,   217,   198,   198,   166,   165,
    1263,   831,   198,    68,    69,  1269,    93,    94,   198,   198,
    1441,    98,    99,   100,   101,   198,    10,    37,   217,   185,
      66,   215,   193,  1747,   217,   191,   197,   193,  1671,   200,
     217,   197,   217,   199,   200,   217,   217,   102,   103,   217,
     220,   217,  1261,   198,   215,   198,    43,   173,   218,   215,
     221,   118,   119,   216,   124,   221,   194,  1360,   128,   126,
     217,    43,   129,   130,   131,  1368,   896,   214,   215,   136,
     198,   217,   132,  1114,    21,    22,   198,  1118,   143,   198,
     191,  1122,    14,   173,   192,   194,   166,   220,   184,   191,
    1131,  1354,    13,  1127,   191,   160,   161,   162,   216,  1347,
     217,   173,    21,    22,   218,  1139,   191,  1538,   173,     8,
    1621,   173,   173,   199,   218,   173,   173,  1384,   183,   173,
     218,   192,   191,   193,   218,   191,   217,   197,  1347,   199,
     200,   184,   962,   198,   217,  1169,  1170,   218,   173,   217,
     970,   217,   198,   198,   217,   215,     1,   214,   215,   217,
     215,   221,    67,   217,   217,   199,   199,   199,  1421,    43,
    1194,  1672,  1673,  1197,   173,   217,   217,   216,   218,   116,
     117,   118,   119,   120,   173,   216,   123,  1440,   218,   126,
     218,   128,   129,   130,   131,   217,   217,  1618,  1621,   136,
     218,   138,   139,   218,   216,  1443,   217,   116,   117,   118,
     119,   218,  1713,   173,   217,   173,   173,   126,   198,   128,
     129,   130,   131,   217,   217,   217,   173,   136,   173,   138,
     139,   217,  1441,   218,  1443,   173,   217,  1261,  1269,   216,
     181,   173,   218,   216,    21,    22,   173,   216,    33,  1672,
    1673,  1504,   217,   217,   217,   173,   199,   218,   217,    70,
     199,   198,   199,   200,   201,   202,   203,   204,   217,   217,
     199,   217,   185,   217,   177,   217,   216,   214,   215,    53,
     184,  1621,   218,   218,   217,   184,   217,   217,  1319,   217,
    1713,   200,   201,   202,   203,   204,   191,   218,   218,   218,
     184,   218,   216,   191,   216,   214,   215,   184,   397,   737,
     218,   218,   218,    85,     1,  1553,   218,  1348,   407,    46,
     218,   218,   216,  1347,   216,   218,   217,   416,   218,  1538,
      88,   218,  1672,  1673,   141,  1530,  1008,   426,  1158,  1531,
    1531,   118,   119,   836,  1553,  1531,     1,   436,  1531,   126,
    1531,   128,   129,   130,   131,   238,    33,  1016,  1504,   136,
    1438,  1281,  1464,  1616,    21,    22,  1563,   456,  1467,   591,
    1322,  1564,    56,  1713,  1564,   342,  1651,   474,   474,   468,
    1511,  1232,    -1,    60,    61,   729,    -1,    -1,    -1,   478,
      -1,    -1,  1423,    -1,   483,    -1,   485,    -1,  1218,    -1,
      -1,    -1,    -1,    -1,    -1,   494,    -1,    -1,    -1,  1618,
      -1,    -1,    -1,   502,   503,   504,  1236,  1441,  1675,  1443,
      -1,    -1,  1242,   200,   201,   202,   203,   204,   517,  1249,
      -1,  1251,    -1,    -1,    -1,    -1,   525,   214,   215,   528,
     529,   530,   531,   532,   533,    -1,    -1,   124,    -1,    -1,
      -1,   128,    -1,    -1,   543,    -1,    -1,    -1,  1715,    -1,
    1717,   118,   119,    -1,    -1,    -1,    -1,    -1,  1721,   126,
      -1,   128,   129,   130,   131,    21,    22,  1297,    -1,   136,
      -1,    -1,    -1,    -1,    -1,  1742,    -1,    -1,    -1,  1309,
      -1,    -1,    -1,    -1,  1314,   108,   109,   110,   111,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1538,    -1,   193,    -1,    -1,    -1,
     197,   610,   199,   200,   137,    -1,    -1,    -1,    -1,  1553,
      -1,    -1,    -1,    -1,   147,   148,   149,    -1,   215,    -1,
      -1,    -1,    -1,    -1,   221,   202,   203,   204,    -1,   638,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1386,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,  1605,   128,   129,   130,   131,    -1,    -1,    -1,   678,
     136,  1411,   138,   139,  1618,    -1,   685,   686,    -1,   688,
      -1,    -1,    -1,    -1,    -1,   694,    -1,    -1,    -1,    -1,
      -1,   700,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     709,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   720,   721,   722,   723,   724,   725,    -1,   727,    -1,
     729,  1665,    -1,    -1,  1675,    -1,    -1,    -1,    -1,    -1,
    1681,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,   214,   215,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1715,    -1,  1717,    -1,    -1,    -1,
     779,   780,   781,   782,   783,   784,   785,   786,   787,   788,
      -1,    -1,   791,   792,   793,   794,   795,   796,    -1,   798,
     799,  1742,   801,   802,   803,   804,   805,   806,   807,   808,
     809,   810,   811,   812,   813,   814,   815,   816,   817,   818,
     819,   820,   821,   822,   823,   824,   825,   826,   827,    -1,
     829,    -1,    -1,    -1,   833,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   845,    -1,    -1,    -1,
      -1,  1581,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,  1608,    -1,
     136,    -1,   138,   139,   883,    -1,    60,    61,   144,   145,
     146,   890,    -1,    -1,   150,    -1,    -1,    -1,   897,    -1,
     899,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   908,
     909,   910,   911,   912,   913,   914,   915,   916,   917,   918,
     919,   920,   921,   922,   923,   924,   925,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1666,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
     124,    -1,    -1,   127,   128,    -1,    -1,    -1,   214,   215,
      -1,   960,   961,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   974,    -1,   976,    -1,    33,
      -1,    -1,    -1,    -1,   983,    -1,    -1,    -1,   987,    -1,
      -1,    -1,    -1,   992,    -1,   994,   995,   996,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    60,    61,  1007,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   192,   193,
      -1,    -1,    -1,   197,    -1,    -1,   200,    -1,    -1,    -1,
     780,   781,   782,   783,   784,   785,   786,    -1,    -1,     5,
       6,   215,   792,   793,    -1,    -1,    -1,   221,   798,    15,
      16,    17,    18,    19,    -1,    -1,    -1,   807,   808,    25,
      -1,    27,   812,   813,   814,    31,   816,    33,    -1,    -1,
     124,    -1,    -1,    39,   128,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    60,    61,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,  1103,    71,    72,    73,  1107,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   193,
      -1,    -1,    -1,   197,    -1,   199,   200,  1146,    -1,    -1,
      -1,    -1,   118,   119,    -1,    -1,    -1,    -1,   124,    -1,
      -1,   215,   128,    -1,    -1,    -1,    -1,   221,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,    -1,    -1,  1198,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,
      -1,    -1,    -1,    -1,  1223,    -1,    -1,   193,    -1,    -1,
      -1,   197,  1231,  1232,   200,   201,   202,    -1,   204,    -1,
      -1,   207,   208,    -1,    -1,    -1,    -1,    -1,    -1,   215,
      -1,   217,    -1,   219,   220,   221,    -1,    13,    -1,    -1,
    1259,  1260,    -1,    19,    -1,    -1,    -1,  1266,    -1,    25,
    1269,    -1,    -1,    33,    -1,    31,  1275,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    -1,    21,    22,    -1,
      -1,    -1,  1291,    49,  1293,    -1,    -1,    -1,    -1,  1298,
      60,    61,    -1,    -1,    -1,  1304,    -1,    -1,    64,  1308,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,  1351,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,   128,    -1,
      -1,    -1,  1371,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,   116,   117,   118,   119,   120,   143,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
     156,    -1,   136,    -1,   138,   139,    -1,    60,    61,    -1,
      -1,    -1,    -1,    -1,  1413,  1414,  1415,   173,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,    -1,    -1,   197,    -1,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1449,    -1,  1451,    60,    61,   215,    21,    22,  1457,    -1,
      -1,   221,    -1,   219,   198,   199,   200,   201,   202,   203,
     204,   124,    -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,
     214,   215,    -1,    -1,    -1,  1484,    -1,    -1,  1487,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1495,  1496,  1497,    -1,
      -1,    -1,    -1,  1502,    -1,    -1,    -1,    -1,  1507,    -1,
      -1,  1510,    -1,    -1,  1513,  1514,    -1,   124,    -1,  1518,
    1519,   128,  1521,  1522,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1531,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,    -1,    -1,   197,  1544,   199,   200,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,   215,   128,   129,   130,   131,    -1,   221,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,  1577,   144,
     145,   146,    -1,    -1,    -1,   150,   193,    -1,    -1,    -1,
     197,    -1,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1601,    -1,    -1,    -1,    -1,    -1,   215,    -1,
    1609,    -1,    -1,    -1,   221,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1621,    -1,  1623,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      -1,    -1,  1641,  1642,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,    -1,   219,   220,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1662,  1663,    -1,    -1,    -1,  1667,    -1,
      -1,    -1,     1,  1672,  1673,    -1,     5,     6,     7,    -1,
       9,    10,    11,    -1,    13,    -1,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      29,  1700,    31,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      39,    -1,    -1,    42,  1713,    44,    45,    -1,    -1,    48,
      -1,    50,    51,    52,    -1,    54,    55,    -1,    -1,    58,
      59,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,   154,   155,    -1,   157,    -1,
     159,   160,   161,   162,    -1,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,    -1,    -1,
      -1,    -1,    -1,    -1,   183,   184,   185,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,   208,
      -1,    -1,     5,     6,    -1,    -1,   215,    -1,   217,    -1,
     219,   220,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    60,    61,    -1,
      -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
      -1,    -1,    -1,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,    -1,    -1,   197,    -1,    -1,   200,   201,   202,
      -1,   204,    -1,    -1,   207,   208,    -1,    -1,    -1,     5,
       6,    -1,   215,    -1,   217,    -1,   219,   220,   221,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    33,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,
      -1,   157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   197,    -1,    -1,   200,   201,   202,    -1,   204,    -1,
      -1,   207,   208,    -1,    -1,    -1,     5,     6,    -1,   215,
      -1,   217,    -1,   219,   220,   221,    15,    16,    17,    18,
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
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,   154,   155,    -1,   157,    -1,
     159,   160,   161,   162,    -1,    -1,    -1,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,   208,
      -1,    -1,     5,     6,    -1,    -1,   215,    -1,   217,    -1,
     219,   220,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,   154,   155,    -1,   157,    -1,   159,   160,   161,   162,
      -1,    -1,    -1,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,
      -1,   204,    -1,    -1,   207,   208,    -1,    -1,     5,     6,
      -1,    -1,   215,    -1,   217,    -1,   219,   220,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    52,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,
     157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,
     207,   208,    -1,    -1,     5,     6,    -1,    -1,   215,    -1,
     217,    -1,   219,   220,    15,    16,    17,    18,    19,    -1,
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
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,   154,   155,    -1,   157,    -1,   159,   160,
     161,   162,    -1,    -1,    -1,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,
     201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,
       5,     6,    -1,    -1,   215,    -1,   217,    -1,   219,   220,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,    -1,    -1,
      -1,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   200,   201,   202,    -1,   204,
      -1,    -1,   207,   208,    -1,    -1,     5,     6,    -1,    -1,
     215,    -1,   217,   218,   219,   220,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    33,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
     119,    -1,   124,    -1,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,   154,   155,    -1,   157,    -1,
     159,   160,   161,   162,    -1,    -1,    -1,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,    -1,    -1,   197,    -1,   199,   200,    -1,
      -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,   208,
      -1,    -1,    -1,   215,     5,     6,   215,    -1,   217,   221,
     219,   220,    13,    -1,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    33,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    -1,    -1,    -1,    55,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,
     124,    -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,   154,   155,    -1,   157,    -1,   159,   160,
     161,   162,    -1,    -1,    -1,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,   193,
      -1,    -1,    -1,   197,    -1,   199,   200,    -1,    -1,   200,
     201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,
      -1,   215,     5,     6,   215,   216,   217,   221,   219,   220,
      13,    -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
      -1,    -1,    -1,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,    -1,
      -1,   197,    -1,   199,   200,    -1,    -1,   200,   201,   202,
      -1,   204,    -1,    -1,   207,   208,    -1,    -1,    -1,   215,
       5,     6,   215,    -1,   217,   221,   219,   220,    13,    -1,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    33,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,   119,    -1,   124,    -1,    -1,    -1,
     128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,
     155,    -1,   157,    -1,   159,   160,   161,   162,    -1,    -1,
      -1,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,
      -1,    -1,    -1,    -1,    -1,   193,    -1,    -1,    -1,   197,
      -1,   199,   200,    -1,    -1,   200,   201,   202,    -1,   204,
      -1,    -1,   207,   208,    -1,    -1,    -1,   215,     5,     6,
     215,   216,   217,   221,   219,   220,    13,    -1,    15,    16,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,
     157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,
     207,   208,    -1,    -1,     5,     6,    -1,    -1,   215,    -1,
     217,    -1,   219,   220,    15,    16,    17,    18,    19,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,    -1,    -1,    -1,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,
     201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,
       5,     6,    -1,    -1,   215,    -1,   217,    -1,   219,   220,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,    -1,    -1,
      -1,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   200,   201,   202,    -1,   204,
      -1,    -1,   207,   208,    -1,    -1,     5,     6,    -1,    -1,
     215,    -1,   217,   218,   219,   220,    15,    16,    17,    18,
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
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,    -1,    -1,    -1,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,   208,
      -1,    -1,     5,     6,    -1,    -1,   215,    -1,   217,   218,
     219,   220,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,   154,   155,    -1,   157,    -1,   159,   160,   161,   162,
      -1,    -1,    -1,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,
      -1,   204,    -1,    -1,   207,   208,    -1,    -1,     5,     6,
      -1,    -1,   215,   216,   217,    -1,   219,   220,    15,    16,
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
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,
     157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,
     207,   208,    -1,    -1,     5,     6,    -1,    -1,   215,    -1,
     217,    -1,   219,   220,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    -1,    -1,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,   154,   155,    -1,   157,    -1,   159,   160,
     161,   162,    -1,    -1,    -1,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,
     201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,
      -1,    -1,     5,     6,   215,    -1,   217,    -1,   219,   220,
      13,    -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,   154,   155,    -1,   157,    -1,   159,   160,   161,   162,
      -1,    -1,    -1,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,
      -1,   204,    -1,    -1,   207,   208,    -1,    -1,     5,     6,
      -1,    -1,   215,    -1,   217,    -1,   219,   220,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    33,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,   119,    -1,   124,    -1,    -1,    -1,   128,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,
     157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,    -1,    -1,   197,    -1,   199,
     200,   198,    -1,   200,   201,   202,    -1,   204,    -1,    -1,
     207,   208,    -1,    -1,    -1,   215,     5,     6,   215,    -1,
     217,   221,   219,   220,    13,    -1,    15,    16,    17,    18,
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
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,   154,   155,    -1,   157,    -1,
     159,   160,   161,   162,    -1,    -1,    -1,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,   208,
      -1,    -1,     5,     6,    -1,    -1,   215,    -1,   217,    -1,
     219,   220,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
      -1,    -1,    -1,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,
      -1,   204,    -1,    -1,   207,   208,    -1,    -1,     5,     6,
      -1,    -1,   215,    -1,   217,    -1,   219,   220,    15,    16,
      17,    18,    19,    -1,    -1,    22,    -1,    -1,    25,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,
     157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,
     207,   208,    -1,    -1,     5,     6,    -1,    -1,   215,    -1,
     217,    -1,   219,   220,    15,    16,    17,    18,    19,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,   154,   155,    -1,   157,    -1,   159,   160,
     161,   162,    -1,    -1,    -1,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,
     201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,
       5,     6,    -1,    -1,   215,    -1,   217,   218,   219,   220,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,
     155,    -1,   157,    -1,   159,   160,   161,   162,    -1,    -1,
      -1,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   198,    -1,   200,   201,   202,    -1,   204,
      -1,    -1,   207,   208,    -1,    -1,     5,     6,    -1,    -1,
     215,    -1,   217,    -1,   219,   220,    15,    16,    17,    18,
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
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,   154,   155,    -1,   157,    -1,
     159,   160,   161,   162,    -1,    -1,    -1,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,   208,
      -1,    -1,     5,     6,    -1,    -1,   215,    -1,   217,   218,
     219,   220,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,   154,   155,    -1,   157,    -1,   159,   160,   161,   162,
      -1,    -1,    -1,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,
      -1,   204,    -1,    -1,   207,   208,    -1,    -1,     5,     6,
      -1,    -1,   215,    -1,   217,   218,   219,   220,    15,    16,
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
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,
     157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,
     207,   208,    -1,    -1,     5,     6,    -1,    -1,   215,    -1,
     217,   218,   219,   220,    15,    16,    17,    18,    19,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,   154,   155,    -1,   157,    -1,   159,   160,
     161,   162,    -1,    -1,    -1,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,
     201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,
       5,     6,    -1,    -1,   215,    -1,   217,   218,   219,   220,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,
     155,    -1,   157,    -1,   159,   160,   161,   162,    -1,    -1,
      -1,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   200,   201,   202,    -1,   204,
      -1,    -1,   207,   208,    -1,    -1,     5,     6,    -1,    -1,
     215,    -1,   217,   218,   219,   220,    15,    16,    17,    18,
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
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,   154,   155,    -1,   157,    -1,
     159,   160,   161,   162,    -1,    -1,    -1,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,   208,
      -1,    -1,     5,     6,    -1,    -1,   215,    -1,   217,    -1,
     219,   220,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,   154,   155,    -1,   157,    -1,   159,   160,   161,   162,
      -1,    -1,    -1,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,
      -1,   204,    -1,    -1,   207,   208,    -1,    -1,     5,     6,
      -1,    -1,   215,    -1,   217,    -1,   219,   220,    15,    16,
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
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,
     157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,
     207,   208,    -1,    -1,     5,     6,    -1,    -1,   215,    -1,
     217,    -1,   219,   220,    15,    16,    17,    18,    19,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,   154,   155,    -1,   157,    -1,   159,   160,
     161,   162,    -1,    -1,    -1,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,
     201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,
       5,     6,    -1,    -1,   215,    -1,   217,    -1,   219,   220,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    33,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,   119,    -1,   124,    -1,    10,    -1,
     128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,
     155,    -1,   157,    -1,   159,   160,   161,   162,    -1,    -1,
      -1,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,    -1,    -1,    -1,    -1,    21,    22,   183,    -1,
      -1,    -1,    -1,    -1,    -1,   193,    -1,    -1,    -1,   197,
      -1,   199,   200,    -1,    -1,   200,   201,   202,    -1,   204,
      -1,    -1,   207,   208,    -1,    -1,    -1,   215,    -1,    -1,
     215,    -1,   217,   221,   219,   220,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,   141,
      -1,    -1,   144,   145,   146,   147,   148,   149,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     192,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    13,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,   214,   215,    -1,    25,    -1,   219,   220,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,   214,
     215,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    -1,    -1,   156,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,   173,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   219,
      -1,    33,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    41,    60,    61,
      -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,   156,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,   173,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,   124,    -1,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    -1,   219,    -1,   221,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   165,    -1,    -1,    19,    -1,    -1,    -1,    -1,   173,
      25,   193,    -1,    -1,    -1,   197,    31,   199,   200,    -1,
      -1,   185,    -1,    -1,    -1,    -1,    41,   191,    -1,    -1,
      -1,    -1,    -1,   215,    49,    -1,    -1,    -1,    -1,   221,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,   219,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,
      -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,    64,    -1,    -1,    -1,    -1,    -1,   173,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   219,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,   173,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,    -1,   146,    -1,    -1,    -1,    -1,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,    -1,    -1,    -1,   219,   136,   137,
     138,   139,   140,   141,    21,    22,   144,   145,   146,   147,
     148,   149,   150,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,    -1,    -1,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,    -1,
      -1,    -1,    -1,    -1,   192,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,
      -1,   219,   220,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,    -1,    -1,    -1,   136,
     137,   138,   139,   140,   141,    -1,    -1,   144,   145,   146,
     147,   148,   149,   150,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,   192,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,    -1,   219,   220,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    21,
      22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    21,    22,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    21,    22,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,
     218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    21,
      22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    21,    22,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    21,    22,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,
     218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    21,
      22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    21,    22,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    21,    22,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,
     218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    21,
      22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    21,    22,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    21,    22,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,
     218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,   216,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    21,
      22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,   216,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   214,   215,   216,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    21,    22,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,   216,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,   216,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    21,    22,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    38,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
     216,    -1,    -1,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,    -1,   128,   129,   130,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,   144,
     145,   146,   147,   148,   149,   150,    -1,    -1,    -1,    -1,
      -1,    21,    22,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     163,    -1,   207,   208,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    21,    22,   116,   117,   118,   119,
     120,   214,   215,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,   214,   215,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,   142,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    21,    22,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,   142,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    21,    22,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,   214,   215,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    21,    22,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    21,    22,    -1,    -1,    -1,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,   214,   215,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    21,    22,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,   185,    -1,
     150,    21,    22,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,   214,   215,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    21,    22,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      21,    22,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,   214,   215,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,    -1,    -1,    -1,    19,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,   173,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    35,    98,    99,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,   129,   130,    -1,    -1,    71,    -1,
      73,    -1,    75,    76,    77,    78,    79,   173,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,   158,    -1,    98,    99,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,
      -1,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   223,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   102,   103,   165,   185,   204,   215,   224,   228,   237,
     239,   240,   245,   252,   276,   280,   316,   397,   404,   408,
     419,   465,   470,   475,    19,    20,   173,   268,   269,   270,
     166,   246,   247,   173,   174,   175,   204,   241,   242,   243,
     163,   183,   285,   405,   173,   219,   226,    57,    63,   400,
     400,   400,   143,   173,   302,    34,    63,   107,   136,   208,
     217,   272,   273,   274,   275,   302,   252,     5,     6,     8,
      36,   107,   416,    62,   395,   192,   191,   194,   191,   242,
      22,    57,   203,   214,   244,   400,   401,   403,   401,   395,
     476,   466,   471,   173,   143,   238,   274,   274,   274,   217,
     144,   145,   146,   191,   216,    57,    63,   281,   283,    57,
      63,   406,     5,     6,    57,    63,   417,    57,    63,   396,
      15,    16,   166,   171,   173,   176,   217,   230,   269,   166,
     247,   173,   241,   241,   173,   252,   164,   184,   286,   401,
     252,    57,    63,   225,   173,   173,   173,   173,   177,   236,
     218,   270,   274,   274,   274,   274,   284,   173,   407,   420,
     285,   398,   177,   178,   179,   229,    15,    16,   166,   171,
     173,   230,   266,   267,   244,   402,   252,   208,   227,   477,
     467,   472,   177,   218,    35,    71,    73,    75,    76,    77,
      78,    79,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    93,    94,    95,    98,    99,   100,   101,
     118,   119,   173,   279,   282,   194,   285,   106,   414,   415,
     393,   160,   220,   271,   366,   177,   178,   179,   191,   218,
     192,    66,   194,   429,   285,   285,    21,    22,    38,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   128,   129,   130,
     137,   138,   139,   140,   141,   144,   145,   146,   147,   148,
     149,   150,   193,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   207,   208,   214,   215,    35,    35,   217,
     277,   285,   287,    75,    79,    93,    94,    98,    99,   100,
     101,   424,   409,   173,   421,   165,   286,   394,   270,   269,
     220,   252,   152,   173,   391,   392,   266,    19,    25,    31,
      41,    49,    64,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   156,   219,   302,   423,
     425,   426,   430,   436,   464,    79,    94,    99,   101,   285,
     468,   473,   173,   173,   173,   216,    22,   173,   216,   155,
     218,   366,   376,   377,   378,   194,   278,   290,   285,   399,
     194,   413,   285,   418,   366,   216,   269,   217,    43,   191,
     194,   197,   390,   198,   198,   198,   217,   198,   198,   217,
     429,   198,   198,   198,   198,   198,   217,   302,    33,    60,
      61,   124,   128,   193,   197,   200,   215,   221,   435,   195,
     478,   381,   384,   173,   108,   109,   110,   111,   112,   113,
     114,   115,   137,   147,   148,   149,   217,     7,    50,   315,
     252,   191,   252,   218,   464,     1,     5,     6,     9,    10,
      11,    13,    15,    16,    17,    18,    19,    25,    26,    27,
      28,    29,    31,    38,    39,    42,    44,    45,    48,    51,
      52,    54,    55,    58,    59,    65,    68,    69,    80,   102,
     103,   104,   105,   118,   119,   133,   134,   135,   151,   152,
     153,   154,   155,   157,   159,   160,   161,   162,   166,   167,
     168,   169,   170,   171,   172,   174,   175,   176,   183,   200,
     201,   202,   207,   208,   215,   217,   219,   220,   235,   237,
     248,   249,   252,   253,   256,   257,   259,   261,   262,   263,
     264,   286,   288,   289,   291,   296,   301,   302,   303,   307,
     308,   309,   310,   311,   312,   313,   314,   316,   320,   321,
     328,   331,   334,   339,   342,   343,   345,   346,   347,   349,
     354,   357,   358,   365,   423,   480,   495,   506,   510,   523,
     526,   411,   173,   252,   410,   302,   372,   392,   216,    65,
     104,   174,   296,   358,   173,   173,   436,   127,   137,   192,
     389,   437,   442,   444,   358,   446,   440,   173,   198,   448,
     450,   452,   454,   456,   458,   460,   462,   358,   198,   217,
      33,   197,    33,   197,   215,   221,   216,   358,   215,   221,
     436,   428,   173,   191,   252,   379,   433,   464,   469,   173,
     382,   433,   474,   358,   152,   173,   386,   387,   422,   378,
     378,   378,   198,   198,   358,   260,   198,   304,   425,   480,
     217,   302,   198,     5,   102,   103,   198,   217,   127,   301,
     332,   343,   358,   287,   198,   217,    61,   358,   217,   358,
      19,   173,   198,   198,   217,   252,   198,   166,    58,   358,
     217,   287,   198,   217,   198,   198,   217,   198,   198,   127,
     301,   358,   358,   358,   220,   287,   334,   338,   338,   338,
     217,   217,   217,   217,   217,   217,    13,   436,    13,   436,
      13,   358,   505,   521,   198,   358,   198,   234,    13,   294,
     505,   522,   358,   358,   358,   358,   358,    13,    49,   292,
     332,   358,   158,   173,   332,   481,   483,   220,   252,   252,
     358,    10,    37,   334,   340,   173,   217,   252,   252,   252,
     252,   252,    66,   317,   276,   132,   252,    21,    22,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   136,   137,   138,   139,   140,   141,   144,
     145,   146,   147,   148,   149,   150,   192,   193,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   214,   215,
     340,   217,   399,   192,   252,   286,   215,   252,   276,   286,
     373,   198,   218,    43,   252,   389,   301,   358,   464,   464,
     434,   464,   218,   464,   464,   218,   431,   464,   277,   464,
     277,   464,   277,   379,   380,   382,   383,   218,   439,   292,
     216,   216,   358,   173,   252,   479,   194,   433,   286,   194,
     433,   286,   218,   217,    43,   127,   191,   192,   194,   197,
     385,   496,   498,   287,   422,   305,   217,   302,   198,   217,
     329,   198,   198,   198,   517,   332,   301,   332,   191,   108,
     109,   110,   111,   112,   113,   114,   115,   121,   122,   127,
     140,   141,   147,   148,   149,   192,    14,   436,   294,   358,
     358,   287,   173,   192,   322,   324,   358,   326,   194,   166,
     358,   519,   332,   502,   507,   332,   500,   436,   301,   358,
     220,   276,   358,   358,   358,   358,   358,   358,   422,    53,
     200,   215,   217,   358,   481,   484,   488,   504,   509,   422,
     217,   484,   509,   422,   142,   184,   185,   186,   489,   297,
     287,   299,   179,   180,   229,   422,   184,   191,   525,   422,
      13,   216,   191,   525,   217,   137,   385,   525,   191,   525,
     218,   152,   157,   198,   302,   348,   287,   258,   285,   341,
      70,   215,   218,   332,   483,   160,   217,   319,   392,     4,
     160,   337,   338,    19,   158,   173,   423,    19,   158,   173,
     423,   133,   134,   135,   288,   344,   358,   344,   358,   344,
     358,   344,   358,   344,   358,   344,   358,   344,   358,   344,
     358,   358,   358,   358,   344,   358,   344,   358,   358,   358,
     358,   173,   344,   358,   358,   158,   173,   358,   358,   358,
     423,   358,   358,   358,   344,   358,   344,   358,   358,   358,
     358,   344,   358,   344,   358,   344,   358,   358,   344,   358,
      22,   358,   358,   358,   358,   358,   358,   358,   358,   358,
     358,   358,   129,   130,   158,   173,   214,   215,   355,   423,
     358,   218,   332,   412,   358,   275,     8,   366,   371,   436,
     173,   301,   358,   252,   199,   199,   199,   433,   199,   199,
     173,   427,   199,   278,   199,   278,   199,   278,   199,   433,
     199,   433,   295,   464,   218,   216,   192,   252,   286,   464,
     464,   358,   173,   173,   464,   358,   436,   436,    20,   464,
      70,   332,   483,   494,   198,   358,   173,   358,   464,   511,
     513,   515,   436,   525,   358,   358,   358,   358,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   358,   358,   358,
     358,   358,   287,   199,   433,   218,   218,   192,   265,   436,
     436,   218,   436,   218,   436,   525,   436,   380,   525,   383,
     199,   337,   218,   218,   218,   218,   218,   218,    20,   338,
     215,   358,   218,   142,   191,   185,   488,   188,   189,   216,
     492,   191,   185,   188,   216,   491,    20,   218,   488,   184,
     187,   490,    20,   358,   184,   505,   295,   295,   358,    20,
     505,    20,   422,   358,   358,   358,   218,   158,   173,   217,
     217,   350,   352,    12,    23,    24,   250,   251,   358,   290,
     276,   173,   218,   483,   481,   191,   218,   218,   173,   318,
     318,   217,   127,   137,   173,   192,   197,   335,   336,   277,
     198,   217,   198,   217,   338,   338,   338,   217,   217,   216,
      19,   158,   173,   423,   194,   158,   173,   358,   217,   217,
     158,   173,   358,     1,   217,   216,   218,   286,   252,   216,
      57,    63,   369,    67,   370,   252,   199,   252,   438,   443,
     445,   464,   447,   441,   199,   252,   449,   199,   453,   199,
     457,   199,   461,   379,   463,   382,   199,   433,   358,   218,
      43,   385,   199,   199,   332,   199,   483,   218,   218,   218,
     173,   218,   185,   199,   218,   199,   436,   380,   383,   199,
     218,   217,   436,   464,   358,   199,   199,   199,   199,   218,
     199,   199,   218,   199,   337,   277,   217,   332,   484,   488,
     358,   481,   492,   216,   358,   504,   216,   332,   484,   184,
     187,   190,   493,   216,   332,   199,   199,   194,   232,   332,
     332,    20,   218,   217,   137,   385,   358,   358,   436,   277,
     287,   358,    12,   254,   286,   337,   218,   216,   215,   191,
     216,   218,   336,   173,   173,   217,   173,   173,   191,   216,
     278,   359,   358,   361,   358,   218,   332,   358,   198,   217,
     358,   217,   216,   358,   215,   218,   332,   217,   216,   356,
     218,   332,   252,    47,   370,    46,   106,   367,   337,   432,
     173,   451,   455,   459,   217,   464,   252,   173,   358,   497,
     499,   287,   306,   218,   199,   433,   173,   217,   330,   199,
     199,   199,   518,   294,   199,   323,   325,   327,   520,   503,
     508,   501,   217,   340,   278,   218,   332,   185,   488,   492,
     185,   488,   216,   185,   298,   300,   233,   181,   185,   185,
     332,   137,   385,   358,   358,   358,   218,   218,   199,   278,
     287,   255,   252,   277,   218,   481,   173,   216,   194,   390,
     218,   173,   335,   216,   142,   287,   333,   436,   218,   464,
     218,   218,   218,   363,   358,   358,   218,   481,   218,   358,
     218,    33,   368,   367,   369,   292,   217,   217,   358,   173,
     199,   358,   512,   514,   516,   217,   218,   217,   358,   358,
     358,   217,    70,   494,   217,   217,   218,   358,   333,   218,
     358,   492,   358,   493,   505,   358,   217,   293,   231,   505,
     358,   185,   358,   358,   218,   351,   199,   251,    26,   105,
     256,   308,   309,   310,   312,   358,   278,   216,   194,   390,
     436,   389,   218,   127,   358,   199,   199,   464,   218,   218,
     216,   218,   374,   368,   386,   387,   388,   218,   494,   494,
     218,   199,   217,   218,   217,   217,   217,   292,   294,   332,
     494,   494,   218,   185,   524,   524,   524,   292,   177,   524,
     524,   358,   137,   385,   348,   353,   127,   127,   358,   287,
     218,   436,   389,   389,   301,   358,   358,   360,   362,   199,
     218,   282,   375,   217,   481,   485,   486,   487,   487,   358,
     358,   494,   494,   481,   482,   218,   218,   525,   487,   482,
      53,   216,   184,   184,   191,   525,   184,   216,   524,   358,
     358,   348,   358,   389,   301,   358,   301,   358,   252,   364,
     252,   282,   481,   191,   525,   218,   218,   218,   218,   487,
     487,   218,   218,   218,   218,   358,   216,   216,   184,   218,
     216,   301,   358,   252,   252,   287,   218,   217,   218,   218,
     252,   481,   218
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   222,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   224,   225,   225,
     225,   226,   226,   227,   227,   228,   229,   229,   229,   229,
     230,   230,   231,   231,   232,   233,   232,   234,   234,   234,
     235,   236,   236,   238,   237,   239,   240,   241,   241,   241,
     242,   242,   242,   242,   243,   243,   244,   244,   245,   246,
     246,   247,   247,   248,   249,   249,   250,   250,   251,   251,
     251,   252,   252,   253,   253,   254,   255,   254,   256,   256,
     256,   256,   256,   257,   258,   257,   260,   259,   261,   262,
     263,   265,   264,   264,   266,   266,   266,   266,   266,   266,
     267,   267,   268,   268,   268,   269,   269,   269,   269,   269,
     269,   269,   269,   270,   270,   271,   271,   271,   272,   272,
     272,   272,   273,   273,   274,   274,   274,   274,   274,   274,
     274,   275,   275,   276,   276,   277,   277,   277,   278,   278,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   280,   281,   281,   281,
     282,   284,   283,   285,   285,   286,   286,   287,   287,   288,
     288,   288,   289,   289,   289,   289,   289,   289,   289,   289,
     289,   289,   289,   289,   289,   289,   289,   289,   289,   289,
     289,   289,   289,   290,   290,   290,   291,   292,   292,   293,
     293,   294,   294,   295,   295,   297,   298,   296,   299,   300,
     296,   301,   301,   301,   301,   301,   302,   302,   302,   303,
     303,   305,   306,   304,   304,   307,   307,   307,   307,   307,
     307,   308,   309,   310,   310,   310,   311,   311,   311,   312,
     312,   313,   313,   313,   314,   315,   315,   315,   316,   316,
     317,   317,   318,   318,   319,   319,   319,   319,   319,   319,
     319,   319,   320,   320,   322,   323,   321,   324,   325,   321,
     326,   327,   321,   329,   330,   328,   331,   331,   331,   331,
     331,   331,   332,   332,   333,   333,   333,   334,   334,   334,
     335,   335,   335,   335,   335,   336,   336,   337,   337,   337,
     338,   338,   339,   341,   340,   342,   342,   342,   342,   342,
     342,   342,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   344,   344,   344,   344,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   346,   346,   347,   347,   348,   348,   349,   350,
     351,   349,   352,   353,   349,   354,   354,   354,   354,   354,
     354,   354,   355,   356,   354,   357,   357,   357,   357,   357,
     357,   357,   358,   358,   358,   358,   358,   358,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   358,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   358,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   358,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   358,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   358,   358,   359,
     360,   358,   358,   358,   358,   361,   362,   358,   358,   358,
     363,   364,   358,   358,   358,   358,   358,   358,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   365,   365,   365,
     365,   365,   365,   365,   365,   365,   365,   365,   365,   365,
     365,   365,   365,   366,   366,   366,   367,   367,   367,   368,
     368,   369,   369,   369,   370,   370,   371,   372,   372,   373,
     372,   374,   372,   375,   372,   372,   376,   377,   377,   378,
     378,   378,   378,   378,   379,   379,   380,   380,   381,   381,
     381,   382,   383,   383,   384,   384,   384,   385,   385,   386,
     386,   386,   387,   387,   388,   388,   389,   389,   389,   390,
     390,   391,   391,   391,   391,   391,   392,   392,   392,   392,
     392,   393,   393,   394,   393,   395,   395,   396,   396,   396,
     397,   398,   397,   399,   399,   399,   399,   400,   400,   400,
     402,   401,   403,   403,   404,   405,   404,   406,   406,   406,
     407,   409,   410,   408,   411,   412,   408,   413,   413,   414,
     414,   415,   416,   416,   416,   416,   417,   417,   417,   418,
     418,   420,   421,   419,   422,   422,   422,   422,   422,   423,
     423,   423,   423,   423,   423,   423,   423,   423,   423,   423,
     423,   423,   423,   423,   423,   423,   423,   423,   423,   423,
     423,   423,   423,   423,   423,   423,   424,   424,   424,   424,
     424,   424,   424,   424,   425,   426,   426,   426,   427,   427,
     428,   428,   428,   428,   429,   429,   429,   429,   429,   431,
     432,   430,   433,   433,   434,   434,   435,   435,   436,   436,
     436,   436,   436,   436,   437,   438,   436,   436,   436,   439,
     436,   436,   436,   436,   436,   436,   436,   436,   436,   436,
     436,   436,   436,   440,   441,   436,   436,   442,   443,   436,
     444,   445,   436,   446,   447,   436,   436,   448,   449,   436,
     450,   451,   436,   436,   452,   453,   436,   454,   455,   436,
     436,   456,   457,   436,   458,   459,   436,   460,   461,   436,
     462,   463,   436,   464,   464,   464,   466,   467,   468,   469,
     465,   471,   472,   473,   474,   470,   476,   477,   478,   479,
     475,   480,   480,   480,   480,   480,   481,   481,   481,   481,
     481,   481,   481,   481,   482,   482,   483,   484,   484,   485,
     485,   486,   486,   487,   487,   488,   488,   489,   489,   490,
     490,   491,   491,   492,   492,   492,   493,   493,   493,   494,
     494,   495,   495,   495,   495,   495,   495,   496,   497,   495,
     498,   499,   495,   500,   501,   495,   502,   503,   495,   504,
     504,   504,   505,   505,   506,   507,   508,   506,   509,   509,
     510,   510,   510,   511,   512,   510,   513,   514,   510,   515,
     516,   510,   510,   517,   518,   510,   510,   519,   520,   510,
     521,   521,   522,   522,   523,   523,   523,   523,   523,   524,
     524,   525,   525,   526,   526,   526,   526,   526,   526
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     0,     1,
       1,     1,     1,     0,     2,     5,     1,     1,     2,     2,
       3,     2,     0,     2,     0,     0,     3,     0,     2,     5,
       3,     1,     2,     0,     4,     2,     2,     1,     1,     1,
       1,     2,     3,     3,     2,     4,     0,     1,     2,     1,
       3,     1,     3,     3,     3,     2,     1,     1,     0,     2,
       4,     1,     1,     1,     1,     0,     0,     3,     1,     1,
       1,     1,     1,     4,     0,     6,     0,     6,     2,     3,
       3,     0,     5,     5,     1,     1,     1,     1,     1,     1,
       1,     3,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     1,     5,     1,     3,     2,     3,     2,     1,     1,
       1,     1,     1,     4,     1,     2,     3,     3,     3,     3,
       2,     1,     3,     0,     3,     0,     2,     3,     0,     2,
       1,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     3,     3,     2,     2,     3,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     3,     2,     2,     2,     2,     2,     3,     3,     3,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     0,     1,     1,
       3,     0,     4,     1,     1,     1,     1,     3,     7,     2,
       2,     6,     1,     1,     1,     1,     2,     2,     1,     1,
       1,     1,     1,     1,     2,     2,     1,     1,     1,     1,
       2,     2,     2,     0,     2,     2,     3,     0,     2,     0,
       4,     0,     2,     1,     3,     0,     0,     7,     0,     0,
       7,     3,     2,     2,     2,     1,     1,     3,     2,     2,
       3,     0,     0,     5,     1,     2,     5,     5,     5,     6,
       2,     1,     1,     1,     2,     3,     2,     2,     3,     2,
       3,     2,     2,     3,     4,     1,     1,     0,     1,     1,
       1,     0,     1,     3,     9,     8,     8,     7,     8,     7,
       7,     6,     3,     3,     0,     0,     7,     0,     0,     7,
       0,     0,     7,     0,     0,     6,     5,     8,    10,     5,
       8,    10,     1,     3,     1,     2,     3,     1,     1,     2,
       2,     2,     2,     2,     4,     1,     3,     0,     4,     4,
       1,     6,     6,     0,     7,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     6,     8,     5,     6,     1,     4,     3,     0,
       0,     8,     0,     0,     9,     3,     4,     5,     6,     8,
       5,     6,     0,     0,     5,     3,     4,     4,     5,     4,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     2,     4,     3,     4,     5,     4,     5,     3,     4,
       1,     1,     2,     4,     4,     7,     8,     3,     5,     0,
       0,     8,     3,     3,     3,     0,     0,     8,     3,     4,
       0,     0,     9,     4,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     2,     4,     1,     4,     4,     4,
       4,     4,     1,     6,     7,     6,     6,     7,     7,     6,
       7,     6,     6,     0,     4,     1,     0,     1,     1,     0,
       1,     0,     1,     1,     0,     1,     5,     0,     2,     0,
       4,     0,     9,     0,    10,     5,     3,     3,     4,     1,
       1,     3,     3,     3,     1,     3,     1,     3,     0,     2,
       3,     3,     1,     3,     0,     2,     3,     1,     1,     1,
       2,     3,     3,     5,     1,     1,     1,     1,     1,     0,
       1,     1,     4,     3,     3,     5,     4,     6,     5,     5,
       4,     0,     2,     0,     4,     0,     1,     0,     1,     1,
       6,     0,     6,     0,     2,     3,     5,     0,     1,     1,
       0,     5,     2,     3,     4,     0,     4,     0,     1,     1,
       1,     0,     0,     9,     0,     0,    11,     0,     2,     0,
       1,     3,     1,     1,     2,     2,     0,     1,     1,     0,
       3,     0,     0,     7,     1,     4,     3,     3,     5,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     4,     1,     3,
       0,     2,     3,     5,     0,     2,     2,     2,     2,     0,
       0,     7,     1,     1,     1,     3,     3,     4,     1,     1,
       1,     1,     2,     3,     0,     0,     6,     4,     5,     0,
       9,     4,     2,     2,     3,     2,     3,     2,     2,     3,
       3,     3,     2,     0,     0,     6,     2,     0,     0,     6,
       0,     0,     6,     0,     0,     6,     1,     0,     0,     6,
       0,     0,     7,     1,     0,     0,     6,     0,     0,     7,
       1,     0,     0,     6,     0,     0,     7,     0,     0,     6,
       0,     0,     6,     1,     3,     3,     0,     0,     0,     0,
      10,     0,     0,     0,     0,    10,     0,     0,     0,     0,
      11,     1,     1,     1,     1,     1,     3,     3,     5,     5,
       6,     6,     8,     8,     0,     1,     2,     1,     3,     3,
       5,     1,     2,     1,     0,     0,     2,     2,     1,     2,
       1,     2,     1,     2,     1,     1,     2,     1,     1,     0,
       1,     5,     4,     6,     7,     5,     7,     0,     0,    10,
       0,     0,    10,     0,     0,    10,     0,     0,     7,     1,
       3,     3,     3,     1,     5,     0,     0,    10,     1,     3,
       3,     4,     4,     0,     0,    11,     0,     0,    11,     0,
       0,    10,     5,     0,     0,     9,     5,     0,     0,    10,
       1,     3,     1,     3,     3,     3,     4,     7,     9,     0,
       3,     0,     1,     9,    10,    10,    10,     9,    10
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

    case YYSYMBOL_optional_expr_map_tuple_list: /* optional_expr_map_tuple_list  */
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

    case YYSYMBOL_make_struct_dim: /* make_struct_dim  */
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

    case YYSYMBOL_make_tuple_call: /* make_tuple_call  */
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
  case 3: /* program: program module_declaration  */
                                   {
            if ( yyextra->das_has_type_declarations ) {
                das_yyerror(scanner,"module name has to be first declaration",tokAt(scanner,(yylsp[0])), CompilationError::syntax_error);
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

  case 17: /* top_level_reader_macro: expr_reader semicolon  */
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

  case 23: /* optional_not_required: %empty  */
        { (yyval.b) = false; }
    break;

  case 24: /* optional_not_required: '!' "inscope"  */
                        { (yyval.b) = true; }
    break;

  case 25: /* module_declaration: "module" module_name optional_shared optional_public_or_private_module optional_not_required  */
                                                                                                                                    {
        yyextra->g_Program->thisModuleName = *(yyvsp[-3].s);
        yyextra->g_Program->thisModule->isPublic = (yyvsp[-1].b);
        yyextra->g_Program->thisModule->isModule = true;
        yyextra->g_Program->thisModule->visibleEverywhere = (yyvsp[0].b);
        if ( yyextra->g_Program->thisModule->name.empty() ) {
            yyextra->g_Program->library.renameModule(yyextra->g_Program->thisModule.get(),*(yyvsp[-3].s));
        } else if ( yyextra->g_Program->thisModule->name != *(yyvsp[-3].s) ){
            das_yyerror(scanner,"this module already has a name " + yyextra->g_Program->thisModule->name,tokAt(scanner,(yylsp[-3])),
                CompilationError::module_already_has_a_name);
        }
        if ( !yyextra->g_Program->policies.ignore_shared_modules ) {
            yyextra->g_Program->promoteToBuiltin = (yyvsp[-2].b);
        }
        delete (yyvsp[-3].s);
    }
    break;

  case 26: /* character_sequence: STRING_CHARACTER  */
                                                                                  { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
    break;

  case 27: /* character_sequence: STRING_CHARACTER_ESC  */
                                                                                  { (yyval.s) = new string(); *(yyval.s) += "\\\\"; }
    break;

  case 28: /* character_sequence: character_sequence STRING_CHARACTER  */
                                                                                  { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
    break;

  case 29: /* character_sequence: character_sequence STRING_CHARACTER_ESC  */
                                                                                  { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += "\\\\"; }
    break;

  case 30: /* string_constant: "start of the string" character_sequence "end of the string"  */
                                                           { (yyval.s) = (yyvsp[-1].s); }
    break;

  case 31: /* string_constant: "start of the string" "end of the string"  */
                                                           { (yyval.s) = new string(); }
    break;

  case 32: /* format_string: %empty  */
        { (yyval.s) = new string(); }
    break;

  case 33: /* format_string: format_string STRING_CHARACTER  */
                                                 { (yyval.s) = (yyvsp[-1].s); (yyvsp[-1].s)->push_back((yyvsp[0].ch)); }
    break;

  case 34: /* optional_format_string: %empty  */
        { (yyval.s) = new string(""); }
    break;

  case 35: /* $@1: %empty  */
            { das_strfmt(scanner); }
    break;

  case 36: /* optional_format_string: ':' $@1 format_string  */
                                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 37: /* string_builder_body: %empty  */
        {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 38: /* string_builder_body: string_builder_body character_sequence  */
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

  case 39: /* string_builder_body: string_builder_body "{" expr optional_format_string "}"  */
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

  case 40: /* string_builder: "start of the string" string_builder_body "end of the string"  */
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

  case 41: /* reader_character_sequence: STRING_CHARACTER  */
                               {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
    break;

  case 42: /* reader_character_sequence: reader_character_sequence STRING_CHARACTER  */
                                                                {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
    break;

  case 43: /* $@2: %empty  */
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

  case 44: /* expr_reader: '%' name_in_namespace $@2 reader_character_sequence  */
                                     {
        yyextra->g_ReaderExpr->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = yyextra->g_ReaderExpr;
        int thisLine = 0;
        FileInfo * info = nullptr;
        if ( auto seqt = yyextra->g_ReaderMacro->suffix(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, thisLine, info, tokAt(scanner,(yylsp[0]))) ) {
            das_accept_sequence(scanner,seqt,strlen(seqt),thisLine,info);
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

  case 45: /* options_declaration: "options" annotation_argument_list  */
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
        for ( auto & opt : *(yyvsp[0].aaList) ) {
            if ( yyextra->g_Access->isOptionAllowed(opt.name, yyextra->g_Program->thisModule->fileName) ) {
                yyextra->g_Program->options.push_back(opt);
            } else {
                das_yyerror(scanner,"option " + opt.name + " is not allowed here",
                    tokAt(scanner,(yylsp[0])), CompilationError::invalid_option);
            }
        }
        delete (yyvsp[0].aaList);
    }
    break;

  case 47: /* keyword_or_name: "name"  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 48: /* keyword_or_name: "keyword"  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 49: /* keyword_or_name: "type function"  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 50: /* require_module_name: keyword_or_name  */
                              {
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 51: /* require_module_name: '%' require_module_name  */
                                     {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 52: /* require_module_name: require_module_name '.' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 53: /* require_module_name: require_module_name '/' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 54: /* require_module: require_module_name is_public_module  */
                                                         {
        ast_requireModule(scanner,(yyvsp[-1].s),nullptr,(yyvsp[0].b),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 55: /* require_module: require_module_name "as" "name" is_public_module  */
                                                                              {
        ast_requireModule(scanner,(yyvsp[-3].s),(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 56: /* is_public_module: %empty  */
                    { (yyval.b) = false; }
    break;

  case 57: /* is_public_module: "public"  */
                    { (yyval.b) = true; }
    break;

  case 61: /* expect_error: "integer constant"  */
                   {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
    break;

  case 62: /* expect_error: "integer constant" ':' "integer constant"  */
                                      {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
    break;

  case 63: /* expression_label: "label" "integer constant" ':'  */
                                          {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }
    break;

  case 64: /* expression_goto: "goto" "label" "integer constant"  */
                                                {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }
    break;

  case 65: /* expression_goto: "goto" expr  */
                               {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 66: /* elif_or_static_elif: "elif"  */
                          { (yyval.b) = false; }
    break;

  case 67: /* elif_or_static_elif: "static_elif"  */
                          { (yyval.b) = true; }
    break;

  case 68: /* expression_else: %empty  */
                                                           { (yyval.pExpression) = nullptr; }
    break;

  case 69: /* expression_else: "else" expression_block  */
                                                           { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 70: /* expression_else: elif_or_static_elif expr expression_block expression_else  */
                                                                                          {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),
            (yyvsp[-1].pExpression),(yyvsp[0].pExpression));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 71: /* semicolon: "end of line"  */
                       {}
    break;

  case 72: /* semicolon: "end of expression"  */
          {}
    break;

  case 73: /* if_or_static_if: "if"  */
                        { (yyval.b) = false; }
    break;

  case 74: /* if_or_static_if: "static_if"  */
                        { (yyval.b) = true; }
    break;

  case 75: /* expression_else_one_liner: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 76: /* $@3: %empty  */
                      { yyextra->das_need_oxford_comma = true; }
    break;

  case 77: /* expression_else_one_liner: "else" $@3 expression_if_one_liner  */
                                                                                                 {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 78: /* expression_if_one_liner: expr  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 79: /* expression_if_one_liner: expression_return_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 80: /* expression_if_one_liner: expression_yield_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 81: /* expression_if_one_liner: expression_break  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 82: /* expression_if_one_liner: expression_continue  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 83: /* expression_if_then_else: if_or_static_if expr expression_block expression_else  */
                                                                                      {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),
            (yyvsp[-1].pExpression),(yyvsp[0].pExpression));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 84: /* $@4: %empty  */
                                                     { yyextra->das_need_oxford_comma = true; }
    break;

  case 85: /* expression_if_then_else: expression_if_one_liner "if" $@4 expr expression_else_one_liner semicolon  */
                                                                                                                                                         {
        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-4])),(yyvsp[-2].pExpression),ast_wrapInBlock((yyvsp[-5].pExpression)),(yyvsp[-1].pExpression) ? ast_wrapInBlock((yyvsp[-1].pExpression)) : nullptr);
    }
    break;

  case 86: /* $@5: %empty  */
                     { yyextra->das_need_oxford_comma=false; }
    break;

  case 87: /* expression_for_loop: "for" $@5 variable_name_with_pos_list "in" expr_list expression_block  */
                                                                                                                                                 {
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-3].pNameWithPosList),(yyvsp[-1].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 88: /* expression_unsafe: "unsafe" expression_block  */
                                                 {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-1])));
        pUnsafe->body = (yyvsp[0].pExpression);
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 89: /* expression_while_loop: "while" expr expression_block  */
                                                               {
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-2])));
        pWhile->cond = (yyvsp[-1].pExpression);
        pWhile->body = (yyvsp[0].pExpression);
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 90: /* expression_with: "with" expr expression_block  */
                                                         {
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-2])));
        pWith->with = (yyvsp[-1].pExpression);
        pWith->body = (yyvsp[0].pExpression);
        (yyval.pExpression) = pWith;
    }
    break;

  case 91: /* $@6: %empty  */
                                        { yyextra->das_need_oxford_comma=true; }
    break;

  case 92: /* expression_with_alias: "assume" "name" '=' $@6 expr  */
                                                                                               {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-4])), *(yyvsp[-3].s), ExpressionPtr((yyvsp[0].pExpression)));
        delete (yyvsp[-3].s);
    }
    break;

  case 93: /* expression_with_alias: "assume" "type" "name" '=' type_declaration  */
                                                                        {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-4])), *(yyvsp[-2].s), TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
    break;

  case 94: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 95: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 96: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 97: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 98: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 99: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 100: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 101: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 102: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 103: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 104: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 105: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 106: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 107: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 108: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 109: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 110: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 111: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 112: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 113: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 114: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 115: /* metadata_argument_list: '@' annotation_argument  */
                                      {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 116: /* metadata_argument_list: metadata_argument_list '@' annotation_argument  */
                                                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 117: /* metadata_argument_list: metadata_argument_list semicolon  */
                                               {
        (yyval.aaList) = (yyvsp[-1].aaList);
    }
    break;

  case 118: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 119: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 120: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 121: /* annotation_declaration_name: "template"  */
                                    { (yyval.s) = new string("template"); }
    break;

  case 122: /* annotation_declaration_basic: annotation_declaration_name  */
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
            das_yyerror(scanner,"annotation " + *(yyvsp[0].s) + " is not allowed here",
                        tokAt(scanner,(yylsp[0])), CompilationError::invalid_annotation);
        }
        delete (yyvsp[0].s);
    }
    break;

  case 123: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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
            das_yyerror(scanner,"annotation " + *(yyvsp[-3].s) + " is not allowed here",
                        tokAt(scanner,(yylsp[-3])), CompilationError::invalid_annotation);
        }
        swap ( (yyval.fa)->arguments, *(yyvsp[-1].aaList) );
        delete (yyvsp[-1].aaList);
        delete (yyvsp[-3].s);
    }
    break;

  case 124: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 125: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation);
            delete (yyvsp[0].fa); (yyvsp[0].fa) = nullptr;
        }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 126: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
                                                                              {
        if ( !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation);
            delete (yyvsp[-2].fa); (yyvsp[-2].fa) = nullptr;
        }
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation);
            delete (yyvsp[-2].fa); (yyvsp[-2].fa) = nullptr;
        }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::And,(yyvsp[-2].fa),(yyvsp[0].fa));
    }
    break;

  case 127: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
                                                                            {
        if ( !(yyvsp[-2].fa)->annotation || !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation);
            delete (yyvsp[-2].fa); (yyvsp[-2].fa) = nullptr;
        }
        if ( !(yyvsp[0].fa)->annotation || !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation);
            delete (yyvsp[-2].fa); (yyvsp[-2].fa) = nullptr;
        }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Or,(yyvsp[-2].fa),(yyvsp[0].fa));
    }
    break;

  case 128: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
                                                                              {
        if ( !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation);
            delete (yyvsp[-2].fa); (yyvsp[-2].fa) = nullptr;
        }
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation);
            delete (yyvsp[-2].fa); (yyvsp[-2].fa) = nullptr;
        }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Xor,(yyvsp[-2].fa),(yyvsp[0].fa));
    }
    break;

  case 129: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 130: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 131: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 132: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 133: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 134: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 135: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 136: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 137: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 138: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 139: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 140: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 141: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 142: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 143: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 144: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 145: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 146: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 147: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 148: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 149: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 150: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 151: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 152: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 153: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 154: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 155: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 156: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 157: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 158: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 159: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 160: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 161: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 162: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 163: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 164: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 165: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 166: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 167: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 168: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 169: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 170: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 171: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 172: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 173: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 174: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 175: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 176: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 177: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 178: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 179: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 180: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 181: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 182: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 183: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 184: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 185: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 186: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 187: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 188: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 189: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 190: /* function_name: "operator" '.' "name" "+="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`+="); delete (yyvsp[-1].s); }
    break;

  case 191: /* function_name: "operator" '.' "name" "-="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`-="); delete (yyvsp[-1].s); }
    break;

  case 192: /* function_name: "operator" '.' "name" "*="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`*="); delete (yyvsp[-1].s); }
    break;

  case 193: /* function_name: "operator" '.' "name" "/="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`/="); delete (yyvsp[-1].s); }
    break;

  case 194: /* function_name: "operator" '.' "name" "%="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`%="); delete (yyvsp[-1].s); }
    break;

  case 195: /* function_name: "operator" '.' "name" "&="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&="); delete (yyvsp[-1].s); }
    break;

  case 196: /* function_name: "operator" '.' "name" "|="  */
                                          { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`|="); delete (yyvsp[-1].s); }
    break;

  case 197: /* function_name: "operator" '.' "name" "^="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^="); delete (yyvsp[-1].s); }
    break;

  case 198: /* function_name: "operator" '.' "name" "&&="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&&="); delete (yyvsp[-1].s); }
    break;

  case 199: /* function_name: "operator" '.' "name" "||="  */
                                            { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`||="); delete (yyvsp[-1].s); }
    break;

  case 200: /* function_name: "operator" '.' "name" "^^="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^^="); delete (yyvsp[-1].s); }
    break;

  case 201: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 202: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 203: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 204: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 205: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 206: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 207: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 208: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 209: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 210: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 211: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 212: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 213: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 214: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 215: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 216: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 217: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 218: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 219: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 220: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 221: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 222: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 223: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 224: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 225: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 226: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 227: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 228: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 229: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 230: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 231: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 232: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 233: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 234: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 235: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 236: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
                                                                                {
        (yyvsp[0].pFuncDecl)->atDecl = tokRangeAt(scanner,(yylsp[-1]),(yylsp[0]));
        assignDefaultArguments((yyvsp[0].pFuncDecl));
        runFunctionAnnotations(scanner, yyextra, (yyvsp[0].pFuncDecl), (yyvsp[-2].faList), tokAt(scanner,(yylsp[-2])));
        if ( (yyvsp[0].pFuncDecl)->isGeneric() ) {
            implAddGenericFunction(scanner,(yyvsp[0].pFuncDecl));
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

  case 237: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 238: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 239: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 240: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 241: /* $@7: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 242: /* function_declaration: optional_public_or_private_function $@7 function_declaration_header expression_block  */
                                                                {
        (yyvsp[-1].pFuncDecl)->body = (yyvsp[0].pExpression);
        (yyvsp[-1].pFuncDecl)->privateFunction = !(yyvsp[-3].b);
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
        }
    }
    break;

  case 247: /* expression_block: open_block expressions close_block  */
                                                                  {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 248: /* expression_block: open_block expressions close_block "finally" open_block expressions close_block  */
                                                                                                                        {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 249: /* expr_call_pipe: expr expr_full_block_assumed_piped  */
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

  case 250: /* expr_call_pipe: expression_keyword expr_full_block_assumed_piped  */
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

  case 251: /* expr_call_pipe: "generator" '<' type_declaration_no_options '>' optional_capture_list expr_full_block_assumed_piped  */
                                                                                                                                             {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-3].pTypeDecl),(yyvsp[-1].pCaptList),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 252: /* expression_any: semicolon  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 253: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 254: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 255: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 256: /* expression_any: expr_assign semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 257: /* expression_any: expression_delete semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 258: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 259: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 260: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 261: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 262: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 263: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 264: /* expression_any: expression_break semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 265: /* expression_any: expression_continue semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 266: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 267: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 268: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 269: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 270: /* expression_any: expression_label semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 271: /* expression_any: expression_goto semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 272: /* expression_any: "pass" semicolon  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 273: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 274: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back((yyvsp[0].pExpression));
        }
    }
    break;

  case 275: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 276: /* expr_keyword: "keyword" expr expression_block  */
                                                           {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s));
        pCall->arguments.push_back((yyvsp[-1].pExpression));
        auto resT = new TypeDecl(Type::autoinfer);
        auto blk = ast_makeBlock(scanner,0,nullptr,nullptr,nullptr,resT,(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])));
        pCall->arguments.push_back(blk);
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 277: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 278: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 279: /* optional_expr_list_in_braces: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 280: /* optional_expr_list_in_braces: '(' optional_expr_list optional_comma ')'  */
                                                             { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 281: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 282: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 283: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 284: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 285: /* $@8: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 286: /* $@9: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 287: /* expression_keyword: "keyword" '<' $@8 type_declaration_no_options_list '>' $@9 expr  */
                                                                                                                                                     {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 288: /* $@10: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 289: /* $@11: %empty  */
                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 290: /* expression_keyword: "type function" '<' $@10 type_declaration_no_options_list '>' $@11 optional_expr_list_in_braces  */
                                                                                                                                                                                   {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 291: /* expr_pipe: expr_assign " <|" expr_block  */
                                                        {
        Expression * pipeCall = (yyvsp[-2].pExpression)->tail();
        if ( pipeCall->rtti_isCallLikeExpr() ) {
            auto pCall = (ExprLooksLikeCall *) pipeCall;
            pCall->arguments.push_back((yyvsp[0].pExpression));
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        } else if ( pipeCall->rtti_isVar() ) {
            // a += b <| c
            auto pVar = (ExprVar *) pipeCall;
            auto pCall = yyextra->g_Program->makeCall(pVar->at,pVar->name);
            pCall->arguments.push_back((yyvsp[0].pExpression));
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

  case 292: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 293: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 294: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 295: /* expr_pipe: expr_call_pipe  */
                             {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 296: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 297: /* name_in_namespace: "name" "::" "name"  */
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

  case 298: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 299: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
    }
    break;

  case 300: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 301: /* $@12: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 302: /* $@13: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 303: /* new_type_declaration: '<' $@12 type_declaration '>' $@13  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 304: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 305: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pTypeDecl),false);
    }
    break;

  case 306: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 307: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 308: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),(yyvsp[-1].pExpression));
    }
    break;

  case 309: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),(yyvsp[-1].pExpression));
    }
    break;

  case 310: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 311: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 312: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 313: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 314: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 315: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 316: /* expression_return: expression_return_no_pipe semicolon  */
                                                    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 317: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 318: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 319: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 320: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 321: /* expression_yield: expression_yield_no_pipe semicolon  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 322: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 323: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 324: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 325: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 326: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 327: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 328: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 329: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 330: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 331: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 332: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 333: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 334: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-7].pNameList),tokAt(scanner,(yylsp[-7])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 335: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 336: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 337: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                           {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameList),tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 338: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr semicolon  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-6]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 339: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-5]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameList),tokAt(scanner,(yylsp[-5])),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 340: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr semicolon  */
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

  case 341: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                   {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameList),tokAt(scanner,(yylsp[-4])),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 342: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 343: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 344: /* $@14: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 345: /* $@15: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 346: /* expr_cast: "cast" '<' $@14 type_declaration_no_options '>' $@15 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
    }
    break;

  case 347: /* $@16: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 348: /* $@17: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 349: /* expr_cast: "upcast" '<' $@16 type_declaration_no_options '>' $@17 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 350: /* $@18: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 351: /* $@19: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 352: /* expr_cast: "reinterpret" '<' $@18 type_declaration_no_options '>' $@19 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 353: /* $@20: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 354: /* $@21: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 355: /* expr_type_decl: "type" '<' $@20 type_declaration '>' $@21  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 356: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
                                                                         {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-2].s),ptd->typeexpr);
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-2].s),(yyvsp[-1].pExpression));
            }
            delete (yyvsp[-2].s);
    }
    break;

  case 357: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
                                                                                                {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-5].s),ptd->typeexpr,*(yyvsp[-3].s));
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-5].s),(yyvsp[-1].pExpression),*(yyvsp[-3].s));
            }
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
    break;

  case 358: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" c_or_s "name" '>' expr ')'  */
                                                                                                                        {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-7].s),ptd->typeexpr,*(yyvsp[-5].s),*(yyvsp[-3].s));
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-7].s),(yyvsp[-1].pExpression),*(yyvsp[-5].s),*(yyvsp[-3].s));
            }
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
    break;

  case 359: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
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

  case 360: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
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

  case 361: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" "end of expression" "name" '>' '(' expr ')'  */
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

  case 362: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 363: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 364: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 365: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 366: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 367: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 368: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 369: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 370: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 371: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 372: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 373: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 374: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 375: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 376: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 377: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 378: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 379: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 380: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),(yyvsp[0].pExpression));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 381: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 382: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 383: /* $@22: %empty  */
                             {  yyextra->das_need_oxford_comma = false; }
    break;

  case 384: /* expr_full_block_assumed_piped: block_or_lambda $@22 optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 385: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 386: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 387: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 388: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 389: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 390: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 391: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 392: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 393: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 394: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 395: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 396: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 397: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 398: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 399: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 400: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 401: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 402: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 403: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 404: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 405: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 406: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 407: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 408: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 409: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 410: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 411: /* expr_assign_pipe_right: "@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 412: /* expr_assign_pipe_right: "@@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 413: /* expr_assign_pipe_right: "$ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 414: /* expr_assign_pipe_right: expr_call_pipe  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 415: /* expr_assign_pipe: expr '=' expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 416: /* expr_assign_pipe: expr "<-" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 417: /* expr_assign_pipe: expr "&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 418: /* expr_assign_pipe: expr "|=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 419: /* expr_assign_pipe: expr "^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 420: /* expr_assign_pipe: expr "&&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 421: /* expr_assign_pipe: expr "||=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 422: /* expr_assign_pipe: expr "^^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 423: /* expr_assign_pipe: expr "+=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 424: /* expr_assign_pipe: expr "-=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 425: /* expr_assign_pipe: expr "*=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 426: /* expr_assign_pipe: expr "/=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 427: /* expr_assign_pipe: expr "%=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 428: /* expr_assign_pipe: expr "<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 429: /* expr_assign_pipe: expr ">>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 430: /* expr_assign_pipe: expr "<<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 431: /* expr_assign_pipe: expr ">>>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 432: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 433: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 434: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 435: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 436: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 437: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 438: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 439: /* $@23: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 440: /* $@24: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 441: /* func_addr_expr: '@' '@' '<' $@23 type_declaration_no_options '>' $@24 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = (yyvsp[-3].pTypeDecl);
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 442: /* $@25: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 443: /* $@26: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 444: /* func_addr_expr: '@' '@' '<' $@25 optional_function_argument_list optional_function_type '>' $@26 func_addr_name  */
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

  case 445: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 446: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 447: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 448: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 449: /* expr_field: expr '.' "name" '(' '[' make_struct_fields ']' ')'  */
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

  case 450: /* expr_field: expr '.' basic_type_declaration '(' ')'  */
                                                                        {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), method_name);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 451: /* expr_field: expr '.' basic_type_declaration '(' expr_list ')'  */
                                                                                             {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), method_name);
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 452: /* $@27: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 453: /* $@28: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 454: /* expr_field: expr '.' $@27 error $@28  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), "");
        yyerrok;
    }
    break;

  case 455: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 456: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
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

  case 457: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
                                                               {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-3].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 458: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
                                                                                 {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-4])),*(yyvsp[-4].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-4].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 459: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 460: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 461: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 462: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 463: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 464: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 465: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 466: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 467: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 468: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 469: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 470: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 471: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 472: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",(yyvsp[0].pExpression)); }
    break;

  case 473: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",(yyvsp[0].pExpression)); }
    break;

  case 474: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",(yyvsp[0].pExpression)); }
    break;

  case 475: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",(yyvsp[0].pExpression)); }
    break;

  case 476: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 477: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 478: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 479: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 480: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 481: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 482: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 483: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 484: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 485: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 486: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 487: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 488: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 489: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 490: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 491: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 492: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 493: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 494: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 495: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 496: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 497: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back((yyvsp[-2].pExpression));
        itv->arguments.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = itv;
    }
    break;

  case 498: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", (yyvsp[0].pExpression)); }
    break;

  case 499: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", (yyvsp[0].pExpression)); }
    break;

  case 500: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", (yyvsp[-1].pExpression)); }
    break;

  case 501: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", (yyvsp[-1].pExpression)); }
    break;

  case 502: /* expr: '(' expr_list optional_comma ')'  */
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

  case 503: /* expr: '(' make_struct_single ')'  */
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

  case 504: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 505: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 506: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 507: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 508: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 509: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 510: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 511: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 512: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression)); }
    break;

  case 513: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 514: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 515: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 516: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 517: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 518: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
        }
    break;

  case 519: /* $@29: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 520: /* $@30: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 521: /* expr: expr "is" "type" '<' $@29 type_declaration_no_options '>' $@30  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 522: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),vdecl);
    }
    break;

  case 523: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 524: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 525: /* $@31: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 526: /* $@32: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 527: /* expr: expr "as" "type" '<' $@31 type_declaration '>' $@32  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 528: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 529: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 530: /* $@33: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 531: /* $@34: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 532: /* expr: expr '?' "as" "type" '<' $@33 type_declaration '>' $@34  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-8].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 533: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 534: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 535: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 536: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 537: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 538: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 539: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 540: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 541: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 542: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 543: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 544: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 545: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 546: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 547: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 548: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 549: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 550: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 551: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 552: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 553: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 554: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 555: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 556: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 557: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 558: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 559: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 560: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-6].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 561: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 562: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 563: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 564: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); /*this one is gone when BRABRA is disabled*/ }
    break;

  case 565: /* optional_field_annotation: metadata_argument_list  */
                                                        { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 566: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 567: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 568: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 569: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 570: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 571: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 572: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 573: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 574: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 575: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 576: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 577: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 578: /* struct_variable_declaration_list: struct_variable_declaration_list semicolon  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 579: /* $@35: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 580: /* struct_variable_declaration_list: struct_variable_declaration_list $@35 structure_variable_declaration semicolon  */
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

  case 581: /* $@36: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 582: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@36 function_declaration_header semicolon  */
                                                          {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 583: /* $@37: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 584: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@37 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 585: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' semicolon  */
                                                                                       {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 586: /* function_argument_declaration_no_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_no_type  */
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

  case 587: /* function_argument_declaration_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_type  */
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

  case 588: /* function_argument_declaration_type: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))));
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 589: /* function_argument_list: function_argument_declaration_no_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 590: /* function_argument_list: function_argument_declaration_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 591: /* function_argument_list: function_argument_declaration_no_type semicolon function_argument_list  */
                                                                                            { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 592: /* function_argument_list: function_argument_declaration_type semicolon function_argument_list  */
                                                                                            { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 593: /* function_argument_list: function_argument_declaration_type ',' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 594: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 595: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 596: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 597: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                          { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 598: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 599: /* tuple_alias_type_list: tuple_alias_type_list c_or_s  */
                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 600: /* tuple_alias_type_list: tuple_alias_type_list tuple_type c_or_s  */
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

  case 601: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 602: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 603: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 604: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 605: /* variant_alias_type_list: variant_alias_type_list c_or_s  */
                                           {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 606: /* variant_alias_type_list: variant_alias_type_list variant_type c_or_s  */
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

  case 607: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 608: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 609: /* variable_declaration_no_type: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 610: /* variable_declaration_no_type: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 611: /* variable_declaration_no_type: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 612: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 613: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 614: /* variable_declaration: variable_declaration_type  */
                                        {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 615: /* variable_declaration: variable_declaration_no_type  */
                                           {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 616: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 617: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 618: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 619: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 620: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 621: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 622: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 623: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 624: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 625: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 626: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options semicolon  */
                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 627: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 628: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 629: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr semicolon  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 630: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 631: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 632: /* global_variable_declaration_list: global_variable_declaration_list "end of line"  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 633: /* $@38: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 634: /* global_variable_declaration_list: global_variable_declaration_list $@38 optional_field_annotation let_variable_declaration  */
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

  case 635: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 636: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 637: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 638: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 639: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 640: /* global_let: kwd_let optional_shared optional_public_or_private_variable open_block global_variable_declaration_list close_block  */
                                                                                                                                                      {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 641: /* $@39: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 642: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@39 optional_field_annotation let_variable_declaration  */
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

  case 643: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 644: /* enum_list: enum_list semicolon  */
                                {
        (yyval.pEnum) = (yyvsp[-1].pEnum);
    }
    break;

  case 645: /* enum_list: enum_list "name" semicolon  */
                                           {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt(scanner,(yylsp[-1]))) ) {
            das_yyerror(scanner,"enumeration already declared " + *(yyvsp[-1].s), tokAt(scanner,(yylsp[-1])),
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

  case 646: /* enum_list: enum_list "name" '=' expr semicolon  */
                                                           {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-3]))) ) {
            das_yyerror(scanner,"enumeration value already declared " + *(yyvsp[-3].s), tokAt(scanner,(yylsp[-3])),
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

  case 647: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 648: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 649: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 650: /* $@40: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 651: /* single_alias: optional_public_or_private_alias "name" $@40 '=' type_declaration  */
                                  {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        (yyvsp[0].pTypeDecl)->isPrivateAlias = !(yyvsp[-4].b);
        if ( (yyvsp[0].pTypeDecl)->baseType == Type::alias ) {
            das_yyerror(scanner,"alias cannot be defined in terms of another alias "+*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])),
                CompilationError::invalid_type);
        }
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !yyextra->g_Program->addAlias((yyvsp[0].pTypeDecl)) ) {
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

  case 655: /* $@41: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 657: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 658: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 659: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 660: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 661: /* $@42: %empty  */
                                                                                                                       {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 662: /* $@43: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 663: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name open_block $@42 enum_list $@43 close_block  */
                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-2].pEnum),Type::tInt);
    }
    break;

  case 664: /* $@44: %empty  */
                                                                                                                                                            {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 665: /* $@45: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 666: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration open_block $@44 enum_list $@45 close_block  */
                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-2].pEnum),(yyvsp[-5].type));
    }
    break;

  case 667: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 668: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 669: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 670: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 671: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 672: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 673: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 674: /* class_or_struct: "template" "class"  */
                                 { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 675: /* class_or_struct: "template" "struct"  */
                                 { (yyval.i) = CorS_StructTemplate; }
    break;

  case 676: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 677: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 678: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 679: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 680: /* optional_struct_variable_declaration_list: open_block struct_variable_declaration_list close_block  */
                                                                      {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 681: /* $@46: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 682: /* $@47: %empty  */
                         {
        if ( (yyvsp[0].pStructure) ) {
            (yyvsp[0].pStructure)->isClass = (yyvsp[-3].i)==CorS_Class || (yyvsp[-3].i)==CorS_ClassTemplate;
            (yyvsp[0].pStructure)->isTemplate = (yyvsp[-3].i)==CorS_ClassTemplate || (yyvsp[-3].i)==CorS_StructTemplate;
            (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b);
        }
    }
    break;

  case 683: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@46 structure_name $@47 optional_struct_variable_declaration_list  */
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

  case 684: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 685: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 686: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 687: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 688: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 689: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 690: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 691: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 692: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 693: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 694: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 695: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 696: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 697: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 698: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 699: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 700: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 701: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 702: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 703: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 704: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 705: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 706: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 707: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 708: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 709: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 710: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 711: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 712: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 713: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 714: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 715: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 716: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 717: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 718: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 719: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 720: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 721: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 722: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 723: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 724: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 725: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 726: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 727: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 728: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 729: /* bitfield_bits: bitfield_bits semicolon "name"  */
                                                 {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 730: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<tuple<string,Expression *>>();
        (yyval.pNameExprList) = pSL;

    }
    break;

  case 731: /* bitfield_alias_bits: bitfield_alias_bits semicolon  */
                                            {
        (yyval.pNameExprList) = (yyvsp[-1].pNameExprList);
    }
    break;

  case 732: /* bitfield_alias_bits: bitfield_alias_bits "name" semicolon  */
                                                       {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pNameExprList) = (yyvsp[-2].pNameExprList);
        (yyval.pNameExprList)->emplace_back(*(yyvsp[-1].s),nullptr);
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntry((yyvsp[-1].s)->c_str(),atvname);
        }
        delete (yyvsp[-1].s);
    }
    break;

  case 733: /* bitfield_alias_bits: bitfield_alias_bits "name" '=' expr semicolon  */
                                                                       {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        (yyval.pNameExprList) = (yyvsp[-4].pNameExprList);
        (yyval.pNameExprList)->emplace_back(*(yyvsp[-3].s),(yyvsp[-1].pExpression));
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntry((yyvsp[-3].s)->c_str(),atvname);
        }
        delete (yyvsp[-3].s);
    }
    break;

  case 734: /* bitfield_basic_type_declaration: %empty  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 735: /* bitfield_basic_type_declaration: ':' "uint8"  */
                             { (yyval.type) = Type::tBitfield8; }
    break;

  case 736: /* bitfield_basic_type_declaration: ':' "uint16"  */
                             { (yyval.type) = Type::tBitfield16; }
    break;

  case 737: /* bitfield_basic_type_declaration: ':' "uint"  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 738: /* bitfield_basic_type_declaration: ':' "uint64"  */
                             { (yyval.type) = Type::tBitfield64; }
    break;

  case 739: /* $@48: %empty  */
                                                                     { yyextra->das_arrow_depth ++; }
    break;

  case 740: /* $@49: %empty  */
                                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 741: /* bitfield_type_declaration: "bitfield" bitfield_basic_type_declaration '<' $@48 bitfield_bits '>' $@49  */
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

  case 744: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 745: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 746: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 747: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 748: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 749: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 750: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 751: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 752: /* type_declaration_no_options: type_declaration_no_options dim_list  */
                                                                {
        if ( (yyvsp[-1].pTypeDecl)->baseType==Type::typeDecl ) {
            das_yyerror(scanner,"type declaration can`t be used as array base type",tokAt(scanner,(yylsp[-1])),
                CompilationError::invalid_type);
        } else if ( (yyvsp[-1].pTypeDecl)->baseType==Type::typeMacro ) {
            das_yyerror(scanner,"macro can`t be used as array base type",tokAt(scanner,(yylsp[-1])),
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

  case 753: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 754: /* $@50: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 755: /* $@51: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 756: /* type_declaration_no_options: "type" '<' $@50 type_declaration '>' $@51  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 757: /* type_declaration_no_options: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 758: /* type_declaration_no_options: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 759: /* $@52: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 760: /* type_declaration_no_options: '$' name_in_namespace '<' $@52 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 761: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 762: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 763: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 764: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 765: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 766: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 767: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 768: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 769: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 770: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 771: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 772: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 773: /* $@53: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 774: /* $@54: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 775: /* type_declaration_no_options: "smart_ptr" '<' $@53 type_declaration '>' $@54  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 776: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 777: /* $@55: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 778: /* $@56: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 779: /* type_declaration_no_options: "array" '<' $@55 type_declaration '>' $@56  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 780: /* $@57: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 781: /* $@58: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 782: /* type_declaration_no_options: "table" '<' $@57 table_type_pair '>' $@58  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].aTypePair).firstType;
        (yyval.pTypeDecl)->secondType = (yyvsp[-2].aTypePair).secondType;
    }
    break;

  case 783: /* $@59: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 784: /* $@60: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 785: /* type_declaration_no_options: "iterator" '<' $@59 type_declaration '>' $@60  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 786: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tVoid);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 787: /* $@61: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 788: /* $@62: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 789: /* type_declaration_no_options: "block" '<' $@61 type_declaration '>' $@62  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 790: /* $@63: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 791: /* $@64: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 792: /* type_declaration_no_options: "block" '<' $@63 optional_function_argument_list optional_function_type '>' $@64  */
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

  case 793: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tVoid);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 794: /* $@65: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 795: /* $@66: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 796: /* type_declaration_no_options: "function" '<' $@65 type_declaration '>' $@66  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 797: /* $@67: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 798: /* $@68: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 799: /* type_declaration_no_options: "function" '<' $@67 optional_function_argument_list optional_function_type '>' $@68  */
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

  case 800: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tVoid);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 801: /* $@69: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 802: /* $@70: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 803: /* type_declaration_no_options: "lambda" '<' $@69 type_declaration '>' $@70  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 804: /* $@71: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 805: /* $@72: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 806: /* type_declaration_no_options: "lambda" '<' $@71 optional_function_argument_list optional_function_type '>' $@72  */
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

  case 807: /* $@73: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 808: /* $@74: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 809: /* type_declaration_no_options: "tuple" '<' $@73 tuple_type_list '>' $@74  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 810: /* $@75: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 811: /* $@76: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 812: /* type_declaration_no_options: "variant" '<' $@75 variant_type_list '>' $@76  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 813: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 814: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 815: /* type_declaration: type_declaration '|' '#'  */
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

  case 816: /* $@77: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 817: /* $@78: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 818: /* $@79: %empty  */
                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 819: /* $@80: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 820: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@77 "name" $@78 open_block $@79 tuple_alias_type_list $@80 close_block  */
                  {
        auto vtype = make_smart<TypeDecl>(Type::tTuple);
        vtype->alias = *(yyvsp[-6].s);
        vtype->at = tokAt(scanner,(yylsp[-6]));
        vtype->isPrivateAlias = !(yyvsp[-8].b);
        varDeclToTypeDecl(scanner, vtype.get(), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        if ( !yyextra->g_Program->addAlias(vtype) ) {
            das_yyerror(scanner,"type alias is already defined "+*(yyvsp[-6].s),tokAt(scanner,(yylsp[-6])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTuple((yyvsp[-6].s)->c_str(),atvname);
        }
        delete (yyvsp[-6].s);
    }
    break;

  case 821: /* $@81: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 822: /* $@82: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 823: /* $@83: %empty  */
                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 824: /* $@84: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 825: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@81 "name" $@82 open_block $@83 variant_alias_type_list $@84 close_block  */
                  {
        auto vtype = make_smart<TypeDecl>(Type::tVariant);
        vtype->alias = *(yyvsp[-6].s);
        vtype->at = tokAt(scanner,(yylsp[-6]));
        vtype->isPrivateAlias = !(yyvsp[-8].b);
        varDeclToTypeDecl(scanner, vtype.get(), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        if ( !yyextra->g_Program->addAlias(vtype) ) {
            das_yyerror(scanner,"type alias is already defined "+*(yyvsp[-6].s),tokAt(scanner,(yylsp[-6])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariant((yyvsp[-6].s)->c_str(),atvname);
        }
        delete (yyvsp[-6].s);
    }
    break;

  case 826: /* $@85: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 827: /* $@86: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 828: /* $@87: %empty  */
                                                            {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 829: /* $@88: %empty  */
                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-5]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 830: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@85 "name" $@86 bitfield_basic_type_declaration open_block $@87 bitfield_alias_bits $@88 close_block  */
                  {
        auto btype = make_smart<TypeDecl>((yyvsp[-5].type));
        btype->alias = *(yyvsp[-7].s);
        btype->at = tokAt(scanner,(yylsp[-7]));
        btype->isPrivateAlias = !(yyvsp[-9].b);
        for ( auto & p : *(yyvsp[-2].pNameExprList) ) {
            if ( !get<1>(p) ) {
                btype->argNames.push_back(get<0>(p));
            }
        }
        auto maxBits = btype->maxBitfieldBits();
        if ( btype->argNames.size()>maxBits ) {
            das_yyerror(scanner,"only " + to_string(maxBits) + " different bits are allowed in a bitfield",tokAt(scanner,(yylsp[-7])),
                CompilationError::invalid_type);
        }
        for ( auto & p : *(yyvsp[-2].pNameExprList) ) {
            if ( get<1>(p) ) {
                ast_globalBitfieldConst ( scanner, btype, (yyvsp[-9].b), get<0>(p), get<1>(p) );
            }
        }
        if ( !yyextra->g_Program->addAlias(btype) ) {
            das_yyerror(scanner,"type alias is already defined "+*(yyvsp[-7].s),tokAt(scanner,(yylsp[-7])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-7]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfield((yyvsp[-7].s)->c_str(),atvname);
        }
        delete (yyvsp[-7].s);
        delete (yyvsp[-2].pNameExprList);
    }
    break;

  case 831: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 832: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 833: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 834: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 835: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 836: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 837: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 838: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 839: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 840: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 841: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 842: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 843: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 844: /* make_variant_dim: %empty  */
       {
        (yyval.pExpression) = ast_makeStructToMakeVariant(nullptr, LineInfo());
    }
    break;

  case 845: /* make_variant_dim: make_struct_fields  */
                              {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 846: /* make_struct_single: make_struct_fields optional_comma  */
                                               {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 847: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 848: /* make_struct_dim: make_struct_dim "end of expression" make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 849: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 850: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 851: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 852: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 853: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 854: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 855: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 856: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 869: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 870: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 871: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 872: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = (yyvsp[-2].pTypeDecl);
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 873: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = (yyvsp[-4].pTypeDecl);
        msd->useInitializer = true;
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = msd;
    }
    break;

  case 874: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-5].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 875: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back((yyvsp[-2].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 876: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-5].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),"to_array_move");
        tam->arguments.push_back((yyvsp[-2].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 877: /* $@89: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 878: /* $@90: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 879: /* make_struct_decl: "struct" '<' $@89 type_declaration_no_options '>' $@90 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 880: /* $@91: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 881: /* $@92: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 882: /* make_struct_decl: "class" '<' $@91 type_declaration_no_options '>' $@92 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                           {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 883: /* $@93: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 884: /* $@94: %empty  */
                                                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 885: /* make_struct_decl: "variant" '<' $@93 variant_type_list '>' $@94 '(' use_initializer make_variant_dim ')'  */
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

  case 886: /* $@95: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 887: /* $@96: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 888: /* make_struct_decl: "default" '<' $@95 type_declaration_no_options '>' $@96 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = (yyvsp[-3].pTypeDecl);
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 889: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 890: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 891: /* make_tuple: make_tuple ',' expr  */
                                      {
        ExprMakeTuple * mt;
        if ( (yyvsp[-2].pExpression)->rtti_isMakeTuple() ) {
            mt = static_cast<ExprMakeTuple *>((yyvsp[-2].pExpression));
        } else {
            mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-2])));
            mt->values.push_back((yyvsp[-2].pExpression));
        }
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 892: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 893: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 894: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 895: /* $@97: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 896: /* $@98: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 897: /* make_tuple_call: "tuple" '<' $@97 tuple_type_list '>' $@98 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 898: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mka;
    }
    break;

  case 899: /* make_dim: make_dim "end of expression" make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 900: /* make_dim_decl: '[' optional_expr_list ']'  */
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

  case 901: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-2].pTypeDecl);
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 902: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-2].pTypeDecl);
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 903: /* $@99: %empty  */
                                       { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 904: /* $@100: %empty  */
                                                                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 905: /* make_dim_decl: "array" "struct" '<' $@99 type_declaration_no_options '>' $@100 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 906: /* $@101: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 907: /* $@102: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 908: /* make_dim_decl: "array" "tuple" '<' $@101 tuple_type_list '>' $@102 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 909: /* $@103: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 910: /* $@104: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 911: /* make_dim_decl: "array" "variant" '<' $@103 variant_type_list '>' $@104 '(' make_variant_dim ')'  */
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

  case 912: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
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

  case 913: /* $@105: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 914: /* $@106: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 915: /* make_dim_decl: "array" '<' $@105 type_declaration_no_options '>' $@106 '(' optional_expr_list ')'  */
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

  case 916: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 917: /* $@107: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 918: /* $@108: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 919: /* make_dim_decl: "fixed_array" '<' $@107 type_declaration_no_options '>' $@108 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = (yyvsp[-6].pTypeDecl);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 920: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mka;
    }
    break;

  case 921: /* make_table: make_table "end of expression" make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 922: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 923: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 924: /* make_table_decl: "begin of code block" optional_expr_map_tuple_list "end of code block"  */
                                                              {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-2])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
            auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),"to_table_move");
            ttm->arguments.push_back(mka);
            (yyval.pExpression) = ttm;
        } else {
            auto mks = new ExprMakeStruct();
            mks->at = tokAt(scanner,(yylsp[-2]));
            mks->makeType = make_smart<TypeDecl>(Type::tTable);
            mks->makeType->firstType = make_smart<TypeDecl>(Type::autoinfer);
            mks->makeType->secondType = make_smart<TypeDecl>(Type::autoinfer);
            mks->useInitializer = true;
            mks->alwaysUseInitializer = true;
            (yyval.pExpression) = mks;
        }
    }
    break;

  case 925: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
                                                                          {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = mkt;
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-2]));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),"to_table_move");
        ttm->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = ttm;
    }
    break;

  case 926: /* make_table_decl: "table" '(' optional_expr_map_tuple_list ')'  */
                                                                       {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 927: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 928: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 929: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 930: /* array_comprehension_where: "end of expression" "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 931: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 932: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 933: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 934: /* array_comprehension: '[' "iterator" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                                 {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 935: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true,false);
    }
    break;

  case 936: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where "end of code block" ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),false,false);
    }
    break;

  case 937: /* array_comprehension: "begin of code block" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                              {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
    }
    break;

  case 938: /* array_comprehension: "{{" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block" "end of code block"  */
                                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true,true);
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
    if ( !yyextra->das_suppress_errors ) {
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
