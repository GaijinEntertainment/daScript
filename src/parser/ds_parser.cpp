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
  YYSYMBOL_266_7 = 266,                    /* $@7  */
  YYSYMBOL_annotation_argument_value = 267, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 268, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 269, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 270,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 271, /* annotation_argument_list  */
  YYSYMBOL_metadata_argument_list = 272,   /* metadata_argument_list  */
  YYSYMBOL_annotation_declaration_name = 273, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 274, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 275,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 276,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 277, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 278, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 279,   /* optional_function_type  */
  YYSYMBOL_function_name = 280,            /* function_name  */
  YYSYMBOL_optional_template = 281,        /* optional_template  */
  YYSYMBOL_global_function_declaration = 282, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 283, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 284, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 285,     /* function_declaration  */
  YYSYMBOL_286_8 = 286,                    /* $@8  */
  YYSYMBOL_open_block = 287,               /* open_block  */
  YYSYMBOL_close_block = 288,              /* close_block  */
  YYSYMBOL_expression_block = 289,         /* expression_block  */
  YYSYMBOL_expr_call_pipe = 290,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 291,           /* expression_any  */
  YYSYMBOL_expressions = 292,              /* expressions  */
  YYSYMBOL_expr_keyword = 293,             /* expr_keyword  */
  YYSYMBOL_optional_expr_list = 294,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_list_in_braces = 295, /* optional_expr_list_in_braces  */
  YYSYMBOL_optional_expr_map_tuple_list = 296, /* optional_expr_map_tuple_list  */
  YYSYMBOL_type_declaration_no_options_list = 297, /* type_declaration_no_options_list  */
  YYSYMBOL_expression_keyword = 298,       /* expression_keyword  */
  YYSYMBOL_299_9 = 299,                    /* $@9  */
  YYSYMBOL_300_10 = 300,                   /* $@10  */
  YYSYMBOL_301_11 = 301,                   /* $@11  */
  YYSYMBOL_302_12 = 302,                   /* $@12  */
  YYSYMBOL_expr_pipe = 303,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 304,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 305,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 306,     /* new_type_declaration  */
  YYSYMBOL_307_13 = 307,                   /* $@13  */
  YYSYMBOL_308_14 = 308,                   /* $@14  */
  YYSYMBOL_expr_new = 309,                 /* expr_new  */
  YYSYMBOL_expression_break = 310,         /* expression_break  */
  YYSYMBOL_expression_continue = 311,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 312, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 313,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 314, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 315,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 316,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 317,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 318,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 319,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 320,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 321, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 322,           /* expression_let  */
  YYSYMBOL_expr_cast = 323,                /* expr_cast  */
  YYSYMBOL_324_15 = 324,                   /* $@15  */
  YYSYMBOL_325_16 = 325,                   /* $@16  */
  YYSYMBOL_326_17 = 326,                   /* $@17  */
  YYSYMBOL_327_18 = 327,                   /* $@18  */
  YYSYMBOL_328_19 = 328,                   /* $@19  */
  YYSYMBOL_329_20 = 329,                   /* $@20  */
  YYSYMBOL_expr_type_decl = 330,           /* expr_type_decl  */
  YYSYMBOL_331_21 = 331,                   /* $@21  */
  YYSYMBOL_332_22 = 332,                   /* $@22  */
  YYSYMBOL_expr_type_info = 333,           /* expr_type_info  */
  YYSYMBOL_expr_list = 334,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 335,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 336,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 337,            /* capture_entry  */
  YYSYMBOL_capture_list = 338,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 339,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 340,               /* expr_block  */
  YYSYMBOL_expr_full_block = 341,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 342, /* expr_full_block_assumed_piped  */
  YYSYMBOL_343_23 = 343,                   /* $@23  */
  YYSYMBOL_expr_numeric_const = 344,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 345,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe_right = 346,   /* expr_assign_pipe_right  */
  YYSYMBOL_expr_assign_pipe = 347,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 348,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 349,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 350,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 351,           /* func_addr_expr  */
  YYSYMBOL_352_24 = 352,                   /* $@24  */
  YYSYMBOL_353_25 = 353,                   /* $@25  */
  YYSYMBOL_354_26 = 354,                   /* $@26  */
  YYSYMBOL_355_27 = 355,                   /* $@27  */
  YYSYMBOL_expr_field = 356,               /* expr_field  */
  YYSYMBOL_357_28 = 357,                   /* $@28  */
  YYSYMBOL_358_29 = 358,                   /* $@29  */
  YYSYMBOL_expr_call = 359,                /* expr_call  */
  YYSYMBOL_expr = 360,                     /* expr  */
  YYSYMBOL_361_30 = 361,                   /* $@30  */
  YYSYMBOL_362_31 = 362,                   /* $@31  */
  YYSYMBOL_363_32 = 363,                   /* $@32  */
  YYSYMBOL_364_33 = 364,                   /* $@33  */
  YYSYMBOL_365_34 = 365,                   /* $@34  */
  YYSYMBOL_366_35 = 366,                   /* $@35  */
  YYSYMBOL_expr_mtag = 367,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 368, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 369,        /* optional_override  */
  YYSYMBOL_optional_constant = 370,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 371, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 372, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 373, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 374, /* struct_variable_declaration_list  */
  YYSYMBOL_375_36 = 375,                   /* $@36  */
  YYSYMBOL_376_37 = 376,                   /* $@37  */
  YYSYMBOL_377_38 = 377,                   /* $@38  */
  YYSYMBOL_378_39 = 378,                   /* $@39  */
  YYSYMBOL_function_argument_declaration_no_type = 379, /* function_argument_declaration_no_type  */
  YYSYMBOL_function_argument_declaration_type = 380, /* function_argument_declaration_type  */
  YYSYMBOL_function_argument_list = 381,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 382,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 383,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 384,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 385,             /* variant_type  */
  YYSYMBOL_variant_type_list = 386,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 387,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 388,             /* copy_or_move  */
  YYSYMBOL_variable_declaration_no_type = 389, /* variable_declaration_no_type  */
  YYSYMBOL_variable_declaration_type = 390, /* variable_declaration_type  */
  YYSYMBOL_variable_declaration = 391,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 392,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 393,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 394, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 395, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 396, /* global_variable_declaration_list  */
  YYSYMBOL_397_40 = 397,                   /* $@40  */
  YYSYMBOL_optional_shared = 398,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 399, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 400,               /* global_let  */
  YYSYMBOL_401_41 = 401,                   /* $@41  */
  YYSYMBOL_enum_list = 402,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 403, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 404,             /* single_alias  */
  YYSYMBOL_405_42 = 405,                   /* $@42  */
  YYSYMBOL_alias_list = 406,               /* alias_list  */
  YYSYMBOL_alias_declaration = 407,        /* alias_declaration  */
  YYSYMBOL_408_43 = 408,                   /* $@43  */
  YYSYMBOL_optional_public_or_private_enum = 409, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 410,                /* enum_name  */
  YYSYMBOL_enum_declaration = 411,         /* enum_declaration  */
  YYSYMBOL_412_44 = 412,                   /* $@44  */
  YYSYMBOL_413_45 = 413,                   /* $@45  */
  YYSYMBOL_414_46 = 414,                   /* $@46  */
  YYSYMBOL_415_47 = 415,                   /* $@47  */
  YYSYMBOL_optional_structure_parent = 416, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 417,          /* optional_sealed  */
  YYSYMBOL_structure_name = 418,           /* structure_name  */
  YYSYMBOL_class_or_struct = 419,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 420, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 421, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 422,    /* structure_declaration  */
  YYSYMBOL_423_48 = 423,                   /* $@48  */
  YYSYMBOL_424_49 = 424,                   /* $@49  */
  YYSYMBOL_variable_name_with_pos_list = 425, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 426,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 427, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 428, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 429,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 430,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 431,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_basic_type_declaration = 432, /* bitfield_basic_type_declaration  */
  YYSYMBOL_bitfield_type_declaration = 433, /* bitfield_type_declaration  */
  YYSYMBOL_434_50 = 434,                   /* $@50  */
  YYSYMBOL_435_51 = 435,                   /* $@51  */
  YYSYMBOL_c_or_s = 436,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 437,          /* table_type_pair  */
  YYSYMBOL_dim_list = 438,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 439, /* type_declaration_no_options  */
  YYSYMBOL_440_52 = 440,                   /* $@52  */
  YYSYMBOL_441_53 = 441,                   /* $@53  */
  YYSYMBOL_442_54 = 442,                   /* $@54  */
  YYSYMBOL_443_55 = 443,                   /* $@55  */
  YYSYMBOL_444_56 = 444,                   /* $@56  */
  YYSYMBOL_445_57 = 445,                   /* $@57  */
  YYSYMBOL_446_58 = 446,                   /* $@58  */
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
  YYSYMBOL_type_declaration = 467,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 468,  /* tuple_alias_declaration  */
  YYSYMBOL_469_79 = 469,                   /* $@79  */
  YYSYMBOL_470_80 = 470,                   /* $@80  */
  YYSYMBOL_471_81 = 471,                   /* $@81  */
  YYSYMBOL_472_82 = 472,                   /* $@82  */
  YYSYMBOL_variant_alias_declaration = 473, /* variant_alias_declaration  */
  YYSYMBOL_474_83 = 474,                   /* $@83  */
  YYSYMBOL_475_84 = 475,                   /* $@84  */
  YYSYMBOL_476_85 = 476,                   /* $@85  */
  YYSYMBOL_477_86 = 477,                   /* $@86  */
  YYSYMBOL_bitfield_alias_declaration = 478, /* bitfield_alias_declaration  */
  YYSYMBOL_479_87 = 479,                   /* $@87  */
  YYSYMBOL_480_88 = 480,                   /* $@88  */
  YYSYMBOL_481_89 = 481,                   /* $@89  */
  YYSYMBOL_482_90 = 482,                   /* $@90  */
  YYSYMBOL_make_decl = 483,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 484,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 485,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 486,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim = 487,          /* make_struct_dim  */
  YYSYMBOL_make_struct_dim_list = 488,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 489,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 490, /* optional_make_struct_dim_decl  */
  YYSYMBOL_optional_block = 491,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_cur_cur = 492, /* optional_trailing_semicolon_cur_cur  */
  YYSYMBOL_optional_trailing_semicolon_cur_sqr = 493, /* optional_trailing_semicolon_cur_sqr  */
  YYSYMBOL_optional_trailing_semicolon_sqr_sqr = 494, /* optional_trailing_semicolon_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_sqr_sqr = 495, /* optional_trailing_delim_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_cur_sqr = 496, /* optional_trailing_delim_cur_sqr  */
  YYSYMBOL_use_initializer = 497,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 498,         /* make_struct_decl  */
  YYSYMBOL_499_91 = 499,                   /* $@91  */
  YYSYMBOL_500_92 = 500,                   /* $@92  */
  YYSYMBOL_501_93 = 501,                   /* $@93  */
  YYSYMBOL_502_94 = 502,                   /* $@94  */
  YYSYMBOL_503_95 = 503,                   /* $@95  */
  YYSYMBOL_504_96 = 504,                   /* $@96  */
  YYSYMBOL_505_97 = 505,                   /* $@97  */
  YYSYMBOL_506_98 = 506,                   /* $@98  */
  YYSYMBOL_make_tuple = 507,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 508,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 509,          /* make_tuple_call  */
  YYSYMBOL_510_99 = 510,                   /* $@99  */
  YYSYMBOL_511_100 = 511,                  /* $@100  */
  YYSYMBOL_make_dim = 512,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 513,            /* make_dim_decl  */
  YYSYMBOL_514_101 = 514,                  /* $@101  */
  YYSYMBOL_515_102 = 515,                  /* $@102  */
  YYSYMBOL_516_103 = 516,                  /* $@103  */
  YYSYMBOL_517_104 = 517,                  /* $@104  */
  YYSYMBOL_518_105 = 518,                  /* $@105  */
  YYSYMBOL_519_106 = 519,                  /* $@106  */
  YYSYMBOL_520_107 = 520,                  /* $@107  */
  YYSYMBOL_521_108 = 521,                  /* $@108  */
  YYSYMBOL_522_109 = 522,                  /* $@109  */
  YYSYMBOL_523_110 = 523,                  /* $@110  */
  YYSYMBOL_make_table = 524,               /* make_table  */
  YYSYMBOL_expr_map_tuple_list = 525,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 526,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 527, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 528,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 529       /* array_comprehension  */
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
#define YYLAST   15104

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  222
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  308
/* YYNRULES -- Number of rules.  */
#define YYNRULES  960
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1779

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
       0,   567,   567,   568,   573,   574,   575,   576,   577,   578,
     579,   580,   581,   582,   583,   584,   585,   589,   595,   596,
     597,   601,   602,   606,   607,   611,   630,   631,   632,   633,
     637,   638,   642,   643,   647,   648,   648,   652,   657,   666,
     681,   697,   702,   710,   710,   749,   775,   779,   780,   781,
     785,   788,   792,   798,   807,   810,   816,   817,   821,   825,
     826,   830,   833,   839,   845,   848,   854,   855,   859,   860,
     861,   870,   871,   875,   876,   880,   881,   881,   887,   888,
     889,   890,   891,   895,   901,   901,   907,   907,   913,   921,
     931,   940,   940,   944,   944,   950,   951,   952,   953,   954,
     955,   956,   960,   965,   973,   974,   975,   979,   980,   981,
     982,   983,   984,   985,   986,   987,   993,   996,  1002,  1005,
    1008,  1014,  1015,  1016,  1017,  1021,  1038,  1060,  1063,  1073,
    1088,  1103,  1118,  1121,  1128,  1132,  1139,  1140,  1144,  1145,
    1146,  1150,  1153,  1160,  1164,  1165,  1166,  1167,  1168,  1169,
    1170,  1171,  1172,  1173,  1174,  1175,  1176,  1177,  1178,  1179,
    1180,  1181,  1182,  1183,  1184,  1185,  1186,  1187,  1188,  1189,
    1190,  1191,  1192,  1193,  1194,  1195,  1196,  1197,  1198,  1199,
    1200,  1201,  1202,  1203,  1204,  1205,  1206,  1207,  1208,  1209,
    1210,  1211,  1212,  1213,  1214,  1215,  1216,  1217,  1218,  1219,
    1220,  1221,  1222,  1223,  1224,  1225,  1226,  1227,  1228,  1229,
    1230,  1231,  1232,  1233,  1234,  1235,  1236,  1237,  1238,  1239,
    1240,  1241,  1242,  1243,  1244,  1245,  1246,  1247,  1248,  1249,
    1250,  1251,  1252,  1253,  1254,  1255,  1256,  1257,  1258,  1259,
    1260,  1261,  1262,  1263,  1264,  1265,  1266,  1267,  1268,  1269,
    1270,  1271,  1275,  1276,  1280,  1299,  1300,  1301,  1305,  1311,
    1311,  1329,  1330,  1333,  1334,  1337,  1341,  1352,  1361,  1370,
    1376,  1377,  1378,  1379,  1380,  1381,  1382,  1383,  1384,  1385,
    1386,  1387,  1388,  1389,  1390,  1391,  1392,  1393,  1394,  1395,
    1396,  1400,  1405,  1411,  1417,  1428,  1429,  1433,  1434,  1438,
    1439,  1443,  1447,  1454,  1454,  1454,  1460,  1460,  1460,  1469,
    1503,  1506,  1509,  1512,  1518,  1519,  1530,  1534,  1537,  1545,
    1545,  1545,  1548,  1554,  1557,  1561,  1565,  1572,  1579,  1585,
    1589,  1593,  1596,  1599,  1607,  1610,  1613,  1621,  1624,  1632,
    1635,  1638,  1646,  1652,  1653,  1654,  1658,  1659,  1663,  1664,
    1668,  1673,  1681,  1687,  1693,  1699,  1705,  1714,  1723,  1732,
    1744,  1747,  1753,  1753,  1753,  1756,  1756,  1756,  1761,  1761,
    1761,  1769,  1769,  1769,  1775,  1785,  1796,  1809,  1819,  1830,
    1845,  1848,  1854,  1855,  1862,  1873,  1874,  1875,  1879,  1880,
    1881,  1882,  1883,  1887,  1892,  1900,  1901,  1902,  1906,  1911,
    1918,  1925,  1925,  1934,  1935,  1936,  1937,  1938,  1939,  1940,
    1944,  1945,  1946,  1947,  1948,  1949,  1950,  1951,  1952,  1953,
    1954,  1955,  1956,  1957,  1958,  1959,  1960,  1961,  1962,  1966,
    1967,  1968,  1969,  1974,  1975,  1976,  1977,  1978,  1979,  1980,
    1981,  1982,  1983,  1984,  1985,  1986,  1987,  1988,  1989,  1990,
    1995,  2002,  2014,  2019,  2029,  2033,  2040,  2043,  2043,  2043,
    2048,  2048,  2048,  2061,  2065,  2069,  2074,  2081,  2090,  2095,
    2102,  2102,  2102,  2109,  2113,  2122,  2130,  2138,  2142,  2145,
    2151,  2152,  2153,  2154,  2155,  2156,  2157,  2158,  2159,  2160,
    2161,  2162,  2163,  2164,  2165,  2166,  2167,  2168,  2169,  2170,
    2171,  2172,  2173,  2174,  2175,  2176,  2177,  2178,  2179,  2180,
    2181,  2182,  2183,  2184,  2185,  2186,  2192,  2193,  2194,  2195,
    2196,  2209,  2218,  2219,  2220,  2221,  2222,  2223,  2224,  2225,
    2226,  2227,  2228,  2229,  2232,  2235,  2236,  2239,  2239,  2239,
    2242,  2247,  2251,  2255,  2255,  2255,  2260,  2263,  2267,  2267,
    2267,  2272,  2275,  2276,  2277,  2278,  2279,  2280,  2281,  2282,
    2283,  2285,  2289,  2290,  2295,  2299,  2300,  2301,  2302,  2303,
    2304,  2305,  2309,  2313,  2317,  2321,  2325,  2329,  2333,  2337,
    2341,  2348,  2349,  2350,  2354,  2355,  2356,  2360,  2361,  2365,
    2366,  2367,  2371,  2372,  2376,  2387,  2390,  2393,  2393,  2397,
    2397,  2416,  2415,  2431,  2430,  2444,  2453,  2465,  2474,  2484,
    2485,  2486,  2487,  2488,  2492,  2495,  2504,  2505,  2509,  2512,
    2515,  2530,  2539,  2540,  2544,  2547,  2550,  2563,  2564,  2568,
    2574,  2580,  2589,  2592,  2599,  2602,  2608,  2609,  2610,  2614,
    2615,  2619,  2626,  2631,  2640,  2646,  2657,  2660,  2665,  2670,
    2678,  2689,  2692,  2695,  2695,  2715,  2716,  2720,  2721,  2722,
    2726,  2729,  2729,  2748,  2751,  2754,  2769,  2788,  2789,  2790,
    2795,  2795,  2821,  2822,  2826,  2827,  2827,  2831,  2832,  2833,
    2837,  2847,  2852,  2847,  2864,  2869,  2864,  2884,  2885,  2889,
    2890,  2894,  2900,  2901,  2902,  2903,  2907,  2908,  2909,  2913,
    2916,  2922,  2927,  2922,  2947,  2954,  2959,  2968,  2974,  2985,
    2986,  2987,  2988,  2989,  2990,  2991,  2992,  2993,  2994,  2995,
    2996,  2997,  2998,  2999,  3000,  3001,  3002,  3003,  3004,  3005,
    3006,  3007,  3008,  3009,  3010,  3011,  3015,  3016,  3017,  3018,
    3019,  3020,  3021,  3022,  3026,  3037,  3041,  3048,  3060,  3067,
    3073,  3082,  3087,  3090,  3100,  3113,  3114,  3115,  3116,  3117,
    3121,  3125,  3125,  3125,  3139,  3140,  3144,  3148,  3155,  3159,
    3166,  3167,  3168,  3169,  3170,  3185,  3191,  3191,  3191,  3195,
    3200,  3207,  3207,  3214,  3218,  3222,  3227,  3232,  3237,  3242,
    3246,  3250,  3255,  3259,  3263,  3268,  3268,  3268,  3274,  3281,
    3281,  3281,  3286,  3286,  3286,  3292,  3292,  3292,  3297,  3302,
    3302,  3302,  3307,  3307,  3307,  3316,  3321,  3321,  3321,  3326,
    3326,  3326,  3335,  3340,  3340,  3340,  3345,  3345,  3345,  3354,
    3354,  3354,  3360,  3360,  3360,  3369,  3372,  3383,  3399,  3399,
    3404,  3409,  3399,  3434,  3434,  3439,  3445,  3434,  3470,  3470,
    3475,  3480,  3470,  3520,  3521,  3522,  3523,  3524,  3528,  3535,
    3542,  3548,  3554,  3561,  3568,  3574,  3583,  3586,  3592,  3600,
    3605,  3612,  3617,  3624,  3629,  3635,  3636,  3640,  3641,  3646,
    3647,  3651,  3652,  3656,  3657,  3661,  3662,  3663,  3667,  3668,
    3669,  3673,  3674,  3678,  3684,  3691,  3699,  3706,  3714,  3723,
    3723,  3723,  3731,  3731,  3731,  3738,  3738,  3738,  3749,  3749,
    3749,  3760,  3763,  3769,  3783,  3789,  3795,  3801,  3801,  3801,
    3815,  3820,  3827,  3846,  3851,  3858,  3858,  3858,  3868,  3868,
    3868,  3882,  3882,  3882,  3896,  3905,  3905,  3905,  3925,  3932,
    3932,  3932,  3942,  3947,  3954,  3957,  3963,  3982,  3991,  3999,
    4019,  4044,  4045,  4049,  4050,  4055,  4058,  4061,  4064,  4067,
    4070
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
  "expression_with_alias", "$@6", "$@7", "annotation_argument_value",
  "annotation_argument_value_list", "annotation_argument_name",
  "annotation_argument", "annotation_argument_list",
  "metadata_argument_list", "annotation_declaration_name",
  "annotation_declaration_basic", "annotation_declaration",
  "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "optional_template", "global_function_declaration",
  "optional_public_or_private_function", "function_declaration_header",
  "function_declaration", "$@8", "open_block", "close_block",
  "expression_block", "expr_call_pipe", "expression_any", "expressions",
  "expr_keyword", "optional_expr_list", "optional_expr_list_in_braces",
  "optional_expr_map_tuple_list", "type_declaration_no_options_list",
  "expression_keyword", "$@9", "$@10", "$@11", "$@12", "expr_pipe",
  "name_in_namespace", "expression_delete", "new_type_declaration", "$@13",
  "$@14", "expr_new", "expression_break", "expression_continue",
  "expression_return_no_pipe", "expression_return",
  "expression_yield_no_pipe", "expression_yield", "expression_try_catch",
  "kwd_let_var_or_nothing", "kwd_let", "optional_in_scope",
  "tuple_expansion", "tuple_expansion_variable_declaration",
  "expression_let", "expr_cast", "$@15", "$@16", "$@17", "$@18", "$@19",
  "$@20", "expr_type_decl", "$@21", "$@22", "expr_type_info", "expr_list",
  "block_or_simple_block", "block_or_lambda", "capture_entry",
  "capture_list", "optional_capture_list", "expr_block", "expr_full_block",
  "expr_full_block_assumed_piped", "$@23", "expr_numeric_const",
  "expr_assign", "expr_assign_pipe_right", "expr_assign_pipe",
  "expr_named_call", "expr_method_call", "func_addr_name",
  "func_addr_expr", "$@24", "$@25", "$@26", "$@27", "expr_field", "$@28",
  "$@29", "expr_call", "expr", "$@30", "$@31", "$@32", "$@33", "$@34",
  "$@35", "expr_mtag", "optional_field_annotation", "optional_override",
  "optional_constant", "optional_public_or_private_member_variable",
  "optional_static_member_variable", "structure_variable_declaration",
  "struct_variable_declaration_list", "$@36", "$@37", "$@38", "$@39",
  "function_argument_declaration_no_type",
  "function_argument_declaration_type", "function_argument_list",
  "tuple_type", "tuple_type_list", "tuple_alias_type_list", "variant_type",
  "variant_type_list", "variant_alias_type_list", "copy_or_move",
  "variable_declaration_no_type", "variable_declaration_type",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list", "let_variable_declaration",
  "global_variable_declaration_list", "$@40", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@41", "enum_list",
  "optional_public_or_private_alias", "single_alias", "$@42", "alias_list",
  "alias_declaration", "$@43", "optional_public_or_private_enum",
  "enum_name", "enum_declaration", "$@44", "$@45", "$@46", "$@47",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "optional_struct_variable_declaration_list", "structure_declaration",
  "$@48", "$@49", "variable_name_with_pos_list", "basic_type_declaration",
  "enum_basic_type_declaration", "structure_type_declaration",
  "auto_type_declaration", "bitfield_bits", "bitfield_alias_bits",
  "bitfield_basic_type_declaration", "bitfield_type_declaration", "$@50",
  "$@51", "c_or_s", "table_type_pair", "dim_list",
  "type_declaration_no_options", "$@52", "$@53", "$@54", "$@55", "$@56",
  "$@57", "$@58", "$@59", "$@60", "$@61", "$@62", "$@63", "$@64", "$@65",
  "$@66", "$@67", "$@68", "$@69", "$@70", "$@71", "$@72", "$@73", "$@74",
  "$@75", "$@76", "$@77", "$@78", "type_declaration",
  "tuple_alias_declaration", "$@79", "$@80", "$@81", "$@82",
  "variant_alias_declaration", "$@83", "$@84", "$@85", "$@86",
  "bitfield_alias_declaration", "$@87", "$@88", "$@89", "$@90",
  "make_decl", "make_struct_fields", "make_variant_dim",
  "make_struct_single", "make_struct_dim", "make_struct_dim_list",
  "make_struct_dim_decl", "optional_make_struct_dim_decl",
  "optional_block", "optional_trailing_semicolon_cur_cur",
  "optional_trailing_semicolon_cur_sqr",
  "optional_trailing_semicolon_sqr_sqr", "optional_trailing_delim_sqr_sqr",
  "optional_trailing_delim_cur_sqr", "use_initializer", "make_struct_decl",
  "$@91", "$@92", "$@93", "$@94", "$@95", "$@96", "$@97", "$@98",
  "make_tuple", "make_map_tuple", "make_tuple_call", "$@99", "$@100",
  "make_dim", "make_dim_decl", "$@101", "$@102", "$@103", "$@104", "$@105",
  "$@106", "$@107", "$@108", "$@109", "$@110", "make_table",
  "expr_map_tuple_list", "make_table_decl", "array_comprehension_where",
  "optional_comma", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1553)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-827)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1553,   751, -1553, -1553,    41,     6,   220,   -41, -1553,    -6,
     382,   382,   382, -1553, -1553,   -12,   198, -1553, -1553,   475,
   -1553, -1553, -1553, -1553,   441, -1553,    97, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553,     3, -1553,    39,
      66,   134, -1553, -1553, -1553, -1553,   220, -1553,    54, -1553,
   -1553, -1553,   382,   382, -1553, -1553,    97, -1553, -1553, -1553,
   -1553, -1553,   258,   213, -1553, -1553, -1553, -1553,   198,   198,
     198,   196, -1553,   826,   175, -1553, -1553,   331,   385,   421,
     715,   732, -1553,   771,    58,    41,   399,     6,   363,   408,
   -1553,   803,   803, -1553,   497,   475,    18,   475,   789,   524,
     557,   570, -1553,   606,   561, -1553, -1553,   -32,    41,   198,
     198,   198,   198, -1553, -1553, -1553, -1553,   806, -1553, -1553,
     665, -1553, -1553, -1553, -1553, -1553,   -41, -1553, -1553, -1553,
   -1553, -1553,   810,    77,   636, -1553, -1553, -1553, -1553,   836,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553,   475, -1553, -1553,
   -1553,   694, -1553, -1553, -1553, -1553, -1553,   728, -1553,   105,
   -1553,   642,   760,   826, -1553, -1553, -1553, -1553, -1553,   240,
     816, -1553,   -31, -1553, -1553, -1553,   814, -1553, -1553, -1553,
   -1553, -1553,   706, -1553, -1553,   179,   762, -1553,   746, -1553,
     890, -1553,   773,   -41,   -41, -1553, -1553, 14931,   719, -1553,
   -1553,   792, -1553,   504,    41,    41,   -68,   486, -1553, -1553,
   -1553,   807,    77, -1553, -1553, 10607, -1553,   780,   -41, -1553,
   -1553, 13865, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553,   949,   965, -1553,
     794,   -41, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
     -41, -1553,   815,   -41, -1553, -1553,   -31,   225, -1553,    41,
   -1553,   799,   977,   302, -1553, -1553, -1553,   825,   829,   830,
     812,   832,   835, -1553, -1553, -1553,   817, -1553, -1553, -1553,
   -1553, -1553,    85, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553,   837, -1553, -1553, -1553,   838,   850,
   -1553, -1553, -1553, -1553,   851,   852,   841,   -12, -1553, -1553,
   -1553, -1553, -1553,   961,   856, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553,   859,   879, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553,   880,   808, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,  1032, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553,   888,   848, -1553, -1553,   200,   871, -1553, -1553, -1553,
     563,   -12, -1553, -1553, -1553,   486,   854, -1553,  9741,   893,
     895, 10607, -1553,   -11, -1553, -1553, -1553,  9741, -1553, -1553,
     898,   874,   515,   527,   615, -1553, -1553,  9741,   507, -1553,
   -1553, -1553,    20, -1553, -1553, -1553,    21,  6029, -1553,   858,
   10355, -1553, 10458,   387, -1553, -1553, -1553, -1553,   907,   930,
    1892,   864, -1553,   120,   475,   465,   865, 10607, -1553,  2722,
   -1553,   222, -1553,   514, -1553,    62, -1553, -1553,   886,   896,
   -1553, -1553,   944,   -64,   897,    46, -1553,   565,   876,   900,
     901,   883,   903,   891,   567,   909, -1553,   589,   912,   913,
    9741,  9741,   899,   902,   904,   905,   908,   914, -1553,  2090,
   10252,  6237, -1553, -1553, -1553, -1553, -1553, -1553, -1553,   915,
     917, -1553,  6445,  9741,  9741,  9741,  9741,  9741,  5205,  6651,
   -1553,   868, -1553, -1553, -1553,    -2, -1553, -1553, -1553, -1553,
     918, -1553, -1553, -1553, -1553, -1553, -1553, -1553, 11061, -1553,
     919, -1553, -1553, -1553, -1553, -1553, -1553, -1553,  1069,  1036,
   -1553, -1553, -1553,  3961, 10607, 10607, 10607, 11099, 10607, 10607,
     906,   921, 10607,   794, 10607,   794, 10607,   794, 10710,   945,
   11210, -1553,  9741, -1553, -1553, -1553, -1553, -1553,   922, -1553,
   -1553, 13421,  9741, -1553,   961,   584,   -43, -1553, -1553,   525,
   -1553,   856,   514,   943,   525, -1553,   514, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553,  9741, -1553, -1553,   529,     5,     5,     5,
   -1553,   856, -1553,  9741, -1553, -1553, -1553,  2260, -1553,   -41,
    6857, -1553,  9741,   966, -1553,   475,   974,  7063,   -42,   946,
    3549,   202,   202,   202,  7269,   475,   475, -1553,  9741,  1133,
   -1553, -1553, -1553, -1553, -1553, -1553,  1108, -1553, -1553, -1553,
     369, -1553,   475,   475,   475,   475, -1553,   475, -1553, -1553,
    1080, -1553,   183, -1553, 10060,   563,  9741, -1553, -1553, -1553,
     198, -1553,  1139, -1553,   -31, -1553, -1553, -1553,   931, -1553,
   -1553,   -12,   624, -1553,   951,   954,   955, -1553,  9741, 10607,
    9741,  9741, -1553, -1553,  9741, -1553,  9741, -1553,  9741, -1553,
   -1553,  9741, -1553, 10607,   731,   731,  9741,  9741,  9741,  9741,
    9741,  9741,   529,  2928,   529,  3135,   529, 14093, -1553,   813,
   -1553, -1553,   782,   529,   978, -1553,   975,   731,   731,   -52,
     731,   731,   529,  1157,   956,   983, 14390,   960,   -19,   983,
     987,   962,   -18, -1553,  4167,    48, 14708, 14807,  9741,  9741,
   -1553, -1553,  9741,  9741,  9741,  9741,  1011,  9741,   160,  9741,
    9741,  9741,  9741,  9741,  9741,  9741,  9741,  9741,  7475,  9741,
    9741,  9741,  9741,  9741,  9741,  9741,  9741,  9741,  9741, 14869,
    9741, -1553,  7681,  1012, -1553,  3961, -1553,  1055, 10794,   -76,
     185,   990,   463, -1553,   292,   532, -1553, -1553,  1018,   708,
     871,   722,   871,   735,   871, -1553,   330, -1553,   372, -1553,
   10607,  1002, -1553, -1553, 13459,   338, -1553,   514, 10607, -1553,
   -1553, 10607, -1553, -1553, 11245,   979,  1155, -1553, -1553,   181,
   -1553, -1553, -1553, 13907,   529,  3961, -1553,  1008, 10915,  1189,
    9741, 14390,  1031, 13907,  1014, -1553,  1015,  1039, 14390, -1553,
   10607,  3961, -1553, 10915,   988, -1553,   918, -1553, -1553, -1553,
   13907, -1553, -1553, 13907, -1553,   -41, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553,   -70,   202, -1553,  4373,  4373,  4373,
    4373,  4373,  4373,  4373,  4373,  4373,  4373,  4373,  9741,  4373,
    4373,  4373,  4373,  4373,  4373, -1553,   514, 14000,  1034,   235,
     834,  1146,   475, 10607, 10607, 10607,  5411,  7887,  1041,  9741,
   10607, -1553, -1553, -1553, 10607,   983,  1159,   997, 11356, 10607,
   10607, 11394, 10607, 11505, 10607,   983, 10607, 10710,   983,   945,
     117, 11540, 11651, 11689, 11800, 11835, 11946,    25,   202,  3342,
    4581,  5617, 14130,  1025,     2,    33,  1026,   301,    29,  5823,
       2,   756,    60,  9741,  1038,  9741, -1553, -1553, 10607, 10607,
   -1553,  9741,   695,    65, -1553,  9741, -1553,    67,   529, -1553,
    9741, -1553,  9741, -1553,  9741, -1553,  9741,  1000,   542, -1553,
   -1553,  1009,  1010,   622, -1553, -1553,   -75,  4789, -1553,   261,
    1019,  1021,    -7,   794,  1033,  1024, -1553, -1553,  1044,  1028,
   -1553, -1553,   383,   383,  1190,  1190, 14639, 14639,  1029,   162,
    1035, -1553, 13570,   -58,   -58,   919,   383,   383, 14520, 10751,
   10107, 14483, 14838, 14223,  1361,  1721, 14613,  1190,  1190,   674,
     674,   162,   162,   162,   582,  9741,  1042,  1043,   617,  9741,
    1229,  1045, 13605, -1553,   289, -1553, -1553, 10794,  9741,  9741,
    9741,  9741,  9741,  9741,  9741,  9741,  9741,  9741,  9741,  9741,
    9741,  9741,  9741,  9741,  9741, -1553, -1553, -1553, -1553, 10607,
   -1553, -1553, -1553,   405, -1553,  1049, -1553,  1050, -1553,  1054,
   -1553, 10710, -1553,   945,   410,   856, -1553,  1037, -1553,  9741,
   -1553, -1553,   856,   856, -1553,  9741,  1059,  1070, 10607, -1553,
    9741, -1553,    75, -1553,  1008,  9741,   -41, 14390,  1064, -1553,
   -1553, -1553, -1553,   131, -1553, 10915, -1553,    48, -1553,   863,
    9741, -1553,   918,  1085,  1085, -1553, -1553, -1553,   202,   202,
     202, -1553, -1553, 10950, -1553, 10950, -1553, 10950, -1553, 10950,
   -1553, 10950, -1553, 10950, -1553, 10950, -1553, 10950, -1553, 10950,
   -1553, 10950, -1553, 10950, 14390, -1553, 10950, -1553, 10950, -1553,
   10950, -1553, 10950, -1553, 10950, -1553, 10950, -1553, -1553,  1071,
     475, -1553, -1553,   666, -1553,    91, -1553,   935,  1164,   752,
     675,   290,  1046,  1051,  1088, 11984,   432, 12095,   753, 10607,
   10710,   945,  1238,  1052,  1056, 10607, -1553, -1553,  1384,  1469,
   -1553,  1613, -1553,  1621,  1060,  1771,   414,  1061,   415,    48,
   -1553, -1553, -1553, -1553, -1553,  1057,  9741, -1553,  4997, 13421,
      38,  9741,   542,   675,    33, -1553, -1553,  1066, -1553,  9741,
    9741, -1553,  1068, -1553,  9741,   675,   681,  1074, -1553, -1553,
    9741, 14390, -1553, -1553,   417,   427, 14260,  9741, -1553,  9741,
      81, 14390, 12130, 14390, 14390, -1553,  1077,     1,  9741,  9741,
   10607,   794,     8, -1553,  1067,   306,  9947, -1553, -1553,    -7,
    1094,  1099,  1078,  1104,  1113, -1553,   343,   871, -1553,  9741,
   -1553,  9741,  8093,  9741, -1553,  1093,  1079, -1553, -1553,  9741,
    1083, -1553, 13716,  9741,  8299,  1084, -1553, 13754, -1553,  8505,
   -1553, -1553, -1553, 14390, 14390, 14390, 14390, 14390, 14390, 14390,
   14390, 14390, 14390, 14390, 14390, 14390, 14390, 14390, 14390, 14390,
   -1553, -1553, -1553,   856, -1553, -1553,  1129, -1553,  1130, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,  1087,
   10607, -1553, 14000, 12241, -1553,  1262,    92, 14390,  9741, -1553,
   10607,  9741,    48,   794,   -41, -1553, -1553,  9741, -1553,  2495,
    2722,    48, -1553,   416,   314, -1553, -1553, -1553, 10607, -1553,
    1278,    91, -1553, -1553,   834, -1553, -1553, -1553,  1095, -1553,
   -1553, -1553,   450, -1553,  1141,  1100, -1553, -1553,  1833,   492,
     508, -1553, -1553,  9741,  2005, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553,  1105,  8711,   707,     2,    33, 14390,
    1025, -1553, -1553, 14390,  1026, -1553,   709,     2,  1107, -1553,
   -1553, -1553, -1553,   710, -1553, -1553, -1553,  1144,   727,   729,
    9741,   137,  9741,  9741,  9741, 12279, 12390,  2028,   871, -1553,
    1109,  4789,   355, -1553, -1553,  1158, -1553, -1553,    -7,  1114,
     144, 10607, 12425, 10607, 12536, -1553,   373, 12574, -1553,  9741,
    1752,  9741, -1553, 12685,  4789, -1553,   419,  9741, -1553, -1553,
   -1553,   433, -1553, -1553, -1553, -1553, -1553, -1553, -1553,   856,
   -1553, -1553,  1160,  9741,   307,   856, 14390,   610,   871, -1553,
   13907, -1553,   475, -1553,   794,  1162,  1116,   602,   234, -1553,
   -1553,  1278,   529,  1119,  1120, -1553, -1553,  9741,  1166,  1142,
    9741, -1553, -1553, -1553, -1553,  1123,  1124,  1126,  9741,  9741,
    9741,  1131,  1277,  1132,  1136,  8917, -1553,   461,  9741,    33,
   -1553,  9741,   681, -1553,  9741,  9741,  1087, -1553, -1553,  9741,
    9741,   748,  9741,  9741, 12720, 14390, 14390, -1553, -1553, -1553,
    1161, -1553,   430, -1553,  1147, -1553, -1553,  9123, -1553, -1553,
    2377, -1553,   758, -1553, -1553, -1553, 10607, 12831, 12869, -1553,
     456, -1553, 12980, -1553, -1553, 14390, -1553, -1553,   144,   863,
    3755, -1553,   871, -1553,   686, 10607,   -11, -1553, 14931, -1553,
   -1553, -1553, -1553,  1277,  1277, 13015,  1169,  1137, 13126,  1153,
    1167,  1170,  9741, -1553,  9741,  1190,  1190,  1190,  9741, -1553,
   -1553,  1277,  1277, -1553, 13164, -1553, 14353, -1553, 14353, -1553,
    1187,  1190, -1553,  1198,  1187, 14353,  9741, 14390, 14390,   182,
     238, -1553,  1168, -1553,  9741, 14483, -1553, -1553,   759, -1553,
   -1553,  1171, -1553, -1553, -1553,  9329,  9535, -1553, -1553, -1553,
   -1553, -1553, 14390,   -41, 10607,   -11,   643,  3961,   475, 14931,
     109,   109, -1553,  9741,  9741, -1553,  1277,  1277,   675,  1177,
    1178,   983,   109,   675, -1553,  1323,  1172,  1193,  1194, -1553,
    1214,  1183, 14353,  9741,  9741, -1553,   238, -1553,  1752, -1553,
   -1553, -1553, -1553,  9741,  9741, 14390, -1553,   643,  3961,  3961,
   -1553, 10794, -1553,   -41,   675,  1025,  1210, -1553,  1184,  1185,
   13275, 13310,   109,   109,  1025,  1188, -1553, -1553,  1191,  1192,
    1196,  9741,  1199,  1200,  1223, -1553, -1553,  1202, 14390, 14390,
   -1553, -1553, 14390,  3961, -1553, 10794, -1553, 10794, -1553, -1553,
     462,  1195, -1553, -1553, -1553, -1553, -1553,  1201,  1203, -1553,
   -1553, -1553, -1553, 14390, -1553, -1553, -1553, -1553, -1553, 10794,
   -1553, -1553, -1553,   675, -1553, -1553, -1553,   464, -1553
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   136,     1,   346,     0,     0,     0,   675,   347,     0,
     667,   667,   667,    71,    72,     0,     0,    15,     3,     0,
      10,     9,     8,    16,     0,     7,   655,     6,    11,     5,
       4,    13,    12,    14,   105,   106,   104,   114,   116,    45,
      61,    58,    59,    47,    48,    49,     0,    50,    56,    46,
     262,   261,   667,   667,    22,    21,   655,   669,   668,   848,
     838,   843,     0,   314,    43,   122,   123,   124,     0,     0,
       0,   125,   127,   134,     0,   121,    17,   693,   692,   252,
     677,   696,   656,   657,     0,     0,     0,     0,    51,     0,
      57,     0,     0,    54,     0,     0,   667,     0,    18,     0,
       0,     0,   316,     0,     0,   133,   128,     0,     0,     0,
       0,     0,     0,   137,   695,   694,   253,   255,   679,   678,
       0,   698,   697,   701,   659,   658,   661,   112,   113,   110,
     111,   108,     0,     0,     0,   107,   117,    62,    60,    56,
      53,    52,   670,   672,   264,   263,   674,     0,   676,    19,
      20,    23,   849,   839,   844,   315,    41,    44,   132,     0,
     129,   130,   131,   135,   257,   256,   259,   254,   680,     0,
     689,   651,   581,    26,    27,    31,     0,   100,   101,    98,
      99,    96,     0,    95,   102,     0,     0,    55,     0,   673,
       0,    25,   755,     0,     0,    42,   126,     0,     0,   681,
     690,     0,   702,   653,     0,     0,   583,     0,    28,    29,
      30,     0,     0,   115,   109,     0,    24,     0,     0,   840,
     845,     0,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,     0,     0,   143,
     138,     0,   736,   739,   742,   743,   737,   740,   738,   741,
       0,   663,   687,   699,   652,   660,   581,     0,   118,     0,
     120,     0,   641,   639,   662,    97,   103,     0,     0,     0,
       0,     0,     0,   709,   729,   710,   745,   711,   715,   716,
     717,   718,   735,   722,   723,   724,   725,   726,   727,   728,
     730,   731,   732,   733,   808,   714,   721,   734,   815,   822,
     712,   719,   713,   720,     0,     0,     0,     0,   744,   770,
     773,   771,   772,   835,   671,   758,   759,   756,   757,   850,
     618,   624,   221,   222,   219,   146,   147,   149,   148,   150,
     151,   152,   153,   179,   180,   177,   178,   170,   181,   182,
     171,   168,   169,   220,   203,     0,   218,   183,   184,   185,
     186,   157,   158,   159,   154,   155,   156,   167,     0,   173,
     174,   172,   165,   166,   161,   160,   162,   163,   164,   145,
     144,   202,     0,   175,   176,   581,   141,   291,   260,   684,
     682,     0,   691,   595,   703,     0,     0,   119,     0,     0,
       0,     0,   640,     0,   776,   799,   802,     0,   805,   795,
       0,     0,   809,   816,   823,   829,   832,     0,   297,   785,
     790,   784,     0,   798,   794,   787,     0,     0,   789,   774,
       0,   751,   841,   846,   223,   224,   217,   201,   225,   204,
     187,     0,   139,   345,   609,   610,     0,     0,   258,     0,
     663,     0,   664,     0,   688,   599,   654,   582,     0,     0,
     486,   487,     0,     0,     0,     0,   480,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   735,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   570,     0,
       0,     0,   403,   405,   404,   406,   407,   408,   409,     0,
       0,    37,   299,     0,     0,     0,     0,     0,   295,     0,
     385,   386,   484,   483,   564,   481,   555,   554,   553,   552,
     136,   558,   482,   557,   556,   528,   488,   529,     0,   489,
       0,   485,   853,   857,   854,   855,   856,   643,   644,     0,
     637,   638,   636,     0,     0,     0,     0,     0,     0,     0,
       0,   761,     0,   138,     0,   138,     0,   138,     0,     0,
       0,   781,   295,   780,   792,   793,   786,   788,     0,   791,
     775,     0,     0,   837,   836,   851,   314,   764,   765,     0,
     619,   614,     0,     0,     0,   625,     0,   226,   206,   207,
     209,   208,   210,   211,   212,   213,   205,   214,   215,   216,
     190,   191,   193,   192,   194,   195,   196,   197,   188,   189,
     198,   199,   200,     0,   343,   344,     0,   581,   581,   581,
     140,   142,   293,     0,    73,    74,    86,   331,   329,     0,
       0,    93,     0,     0,   330,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   270,     0,     0,
     286,   281,   278,   277,   279,   280,   265,   313,   292,   272,
     564,   271,     0,    81,    82,    79,   284,    80,   285,   287,
     349,   276,     0,   273,   410,   685,     0,   665,   683,   597,
       0,   596,     0,   700,   581,   899,   902,   319,   323,   322,
     328,     0,     0,   371,     0,     0,     0,   935,     0,     0,
     299,     0,   362,   365,     0,   368,     0,   939,     0,   908,
     917,     0,   905,     0,   516,   517,     0,     0,     0,     0,
       0,     0,     0,   877,     0,     0,     0,   915,   942,     0,
     303,   306,     0,     0,     0,   944,   953,   493,   492,   530,
     491,   490,     0,     0,     0,   953,   380,     0,   314,   953,
     953,     0,   387,   562,     0,   395,     0,     0,     0,     0,
     518,   519,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   470,
       0,   642,     0,     0,   646,     0,   650,     0,   410,     0,
       0,     0,   766,   779,     0,     0,   746,   760,     0,     0,
     141,     0,   141,     0,   141,   616,     0,   622,     0,   747,
       0,   953,   783,   768,     0,     0,   752,     0,     0,   620,
     842,     0,   626,   847,     0,     0,   704,   606,   607,   629,
     611,   613,   612,     0,     0,     0,   335,   332,   380,     0,
       0,   317,     0,     0,     0,   290,     0,     0,    65,    88,
       0,     0,   340,   337,   386,   398,   136,   312,   310,   311,
       0,   288,   289,     0,    84,     0,   401,   268,   275,   282,
     283,   334,   339,   348,     0,     0,   274,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   267,     0,     0,     0,     0,
     589,   592,     0,     0,     0,     0,   891,     0,     0,     0,
       0,   925,   928,   931,     0,   953,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   953,     0,     0,   953,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   911,   869,   877,     0,   920,     0,     0,     0,
     877,     0,     0,     0,     0,     0,   880,   947,     0,     0,
      40,     0,    38,     0,   946,   954,   300,     0,     0,   922,
     954,   296,     0,   628,     0,   627,     0,     0,   954,   868,
     521,     0,     0,   457,   454,   456,     0,   295,   473,     0,
       0,     0,     0,   138,     0,     0,   541,   540,     0,     0,
     542,   546,   494,   495,   507,   508,   505,   506,     0,   535,
       0,   526,     0,   559,   560,   561,   496,   497,   512,   513,
     514,   515,     0,     0,   510,   511,   509,   503,   504,   499,
     498,   500,   501,   502,     0,     0,     0,   463,     0,     0,
       0,     0,     0,   478,     0,   645,   648,   410,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   649,   777,   800,   803,     0,
     806,   796,   748,     0,   810,     0,   817,     0,   824,     0,
     830,     0,   833,     0,     0,   301,   954,     0,   769,     0,
     753,   852,   615,   621,   608,     0,     0,     0,     0,   630,
       0,    89,     0,   336,   333,     0,     0,   318,     0,    90,
      91,    63,    64,     0,   341,   338,   387,   395,   294,    68,
       0,   291,   136,     0,     0,   361,   360,   309,     0,     0,
       0,   432,   441,   420,   442,   421,   444,   423,   443,   422,
     445,   424,   435,   414,   436,   415,   437,   416,   446,   425,
     447,   426,   434,   412,   413,   448,   427,   449,   428,   438,
     417,   439,   418,   440,   419,   433,   411,   686,   666,     0,
     137,   590,   591,   592,   593,   584,   600,     0,     0,     0,
     892,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   948,   531,     0,     0,
     532,     0,   563,     0,     0,     0,     0,     0,     0,   395,
     565,   566,   567,   568,   569,     0,     0,   878,     0,   380,
     877,     0,     0,     0,     0,   886,   887,     0,   894,     0,
       0,   884,     0,   923,     0,     0,     0,     0,   882,   924,
       0,   914,   879,   943,     0,     0,    34,     0,   945,     0,
       0,   381,     0,   859,   858,   520,     0,     0,     0,     0,
       0,   138,     0,   474,     0,     0,     0,   477,   475,     0,
       0,     0,     0,     0,     0,   393,     0,   141,   537,     0,
     543,     0,     0,     0,   524,     0,     0,   547,   551,     0,
       0,   527,     0,     0,     0,     0,   464,     0,   471,     0,
     522,   479,   647,   420,   421,   423,   422,   424,   414,   415,
     416,   425,   426,   412,   427,   428,   417,   418,   419,   411,
     778,   801,   804,   767,   807,   797,     0,   762,     0,   811,
     813,   818,   820,   825,   827,   831,   617,   834,   623,   297,
       0,   298,     0,     0,   706,   707,   632,   631,     0,   342,
       0,     0,   395,   138,     0,    66,    67,     0,    83,    75,
       0,   395,   350,     0,     0,   431,   429,   430,     0,   605,
     587,   584,   585,   586,   589,   900,   903,   320,     0,   325,
     326,   324,     0,   374,     0,     0,   377,   372,     0,     0,
       0,   936,   934,   299,     0,   363,   366,   369,   940,   938,
     909,   918,   916,   906,     0,     0,     0,   877,     0,   912,
     870,   893,   885,   913,   921,   883,     0,   877,     0,   889,
     890,   897,   881,     0,   304,   307,    35,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   141,   476,
       0,   295,     0,   390,   391,     0,   389,   388,     0,     0,
       0,     0,     0,     0,     0,   452,     0,     0,   548,     0,
     536,     0,   525,     0,   295,   465,     0,     0,   523,   472,
     468,     0,   750,   763,   749,   814,   821,   828,   782,   302,
     754,   705,     0,     0,     0,    94,    92,     0,   141,    69,
       0,    76,     0,   266,   138,     0,     0,   639,     0,   588,
     601,   587,     0,     0,     0,   321,   327,     0,     0,     0,
       0,   373,   926,   929,   932,     0,     0,     0,     0,     0,
       0,     0,   891,     0,     0,     0,   571,     0,     0,     0,
     895,     0,     0,   888,     0,     0,   297,    32,    39,     0,
       0,     0,     0,     0,     0,   861,   860,   455,   580,   458,
       0,   450,     0,   397,     0,   394,   396,     0,   382,   400,
       0,   579,     0,   577,   453,   574,     0,     0,     0,   573,
       0,   466,     0,   469,   708,   633,    87,   269,     0,    68,
       0,    85,   141,   351,   639,     0,     0,   598,     0,   603,
     635,   634,   594,   891,   891,     0,     0,     0,     0,     0,
       0,     0,   295,   949,   299,   364,   367,   370,     0,   892,
     910,   891,   891,   533,     0,   572,   951,   896,   951,   898,
     951,   305,   308,    36,   951,   951,     0,   863,   862,     0,
       0,   461,     0,   392,     0,   383,   538,   544,     0,   578,
     576,     0,   575,   399,    70,   331,     0,    77,    81,    82,
      79,    80,    78,     0,     0,     0,     0,     0,     0,     0,
     876,   876,   375,     0,     0,   378,   891,   891,   866,     0,
       0,   953,   876,   866,   534,     0,     0,     0,     0,    33,
       0,     0,   951,     0,     0,   459,     0,   451,   384,   539,
     545,   549,   467,     0,     0,   337,   402,     0,     0,     0,
     359,   410,   602,     0,     0,   873,   953,   875,     0,     0,
       0,     0,   876,   876,   867,     0,   937,   950,     0,     0,
       0,     0,     0,     0,     0,   959,   955,     0,   865,   864,
     462,   550,   338,     0,   357,   410,   355,   410,   358,   604,
       0,   954,   874,   901,   904,   376,   379,     0,     0,   933,
     941,   919,   907,   952,   957,   958,   960,   956,   353,   410,
     356,   354,   871,     0,   927,   930,   352,     0,   872
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1553, -1553, -1553, -1553, -1553, -1553, -1553,   676,  1338, -1553,
   -1553, -1553, -1553, -1553, -1553,  1422, -1553, -1553, -1553,   923,
    1378, -1553,  1286, -1553, -1553,  1339, -1553, -1553, -1553,  -161,
      -1, -1553, -1553, -1553,  -158, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553,  1217, -1553, -1553,   -29,   -46,
   -1553, -1553, -1553,   457,   754,  -431,  -521,  -781, -1553, -1553,
   -1553, -1553, -1529, -1553, -1553,    10,  -184,  -250,  -433, -1553,
     305, -1553,  -557, -1307,  -691,    50,  -428, -1553, -1553, -1553,
   -1553,  -541,    -5, -1553, -1553, -1553, -1553, -1553,  -157,  -151,
    -149, -1553,  -148, -1553, -1553, -1553,  1442, -1553,   312, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553,   -69,  -141,   449,   -10,   170, -1086,  -584, -1553,
    -646, -1553, -1553,  -442,   644, -1553, -1553, -1553, -1552, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,  1013, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553,  -134,    69,   -60,    68,
     271, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,   384,
    -402,  -897, -1553,  -406,  -895, -1553,  -814,   -57,   -55, -1553,
    -536, -1440, -1553,  -360, -1553, -1553,  1404, -1553, -1553, -1553,
    1016,   996,   246, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553,  -676,  -195, -1553,   999, -1553, -1553, -1553,
    1173, -1553, -1553, -1553,  -410, -1553, -1553,  -168, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553,  -209, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553,  1005,  -697,  -219,  -707,  -710, -1553, -1553, -1313,  -917,
   -1553, -1553, -1553, -1195,   -74, -1221, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553,   230,  -487, -1553, -1553, -1553,
     747, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1198,  -737, -1553
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    17,   151,    56,   191,    18,   176,   183,  1633,
    1437,  1547,   732,   512,   157,   513,   104,    20,    21,    47,
      48,    49,    93,    22,    41,    42,   645,   646,  1367,  1368,
     578,   648,  1502,  1590,   649,   650,  1130,   651,   844,   652,
     653,   654,   655,  1361,   852,   184,   185,    37,    38,    39,
     206,    71,    72,    73,    74,    24,   386,   448,   250,   117,
      25,   166,   251,   167,   197,   387,   146,   865,  1141,   658,
     449,   659,   744,   563,   734,  1094,   514,   968,  1545,   969,
    1546,   661,   515,   662,   688,   915,  1515,   516,   663,   664,
     665,   666,   667,   668,   669,   616,   670,   884,  1373,  1135,
     671,   517,   929,  1528,   930,  1529,   932,  1530,   518,   920,
    1521,   519,   745,  1569,   520,  1285,  1286,  1003,   867,   521,
     905,  1132,   522,   797,  1142,   673,   523,   524,   995,   525,
    1270,  1640,  1271,  1696,   526,  1050,  1479,   527,   746,  1461,
    1699,  1463,  1700,  1576,  1741,   529,   443,  1384,  1510,  1183,
    1185,   912,   455,   908,   684,  1598,  1669,   444,   445,   446,
     815,   816,   432,   817,   818,   433,   986,   837,   838,  1602,
     543,   403,   273,   274,   203,   266,    83,   126,    27,   172,
     390,    94,    95,   188,    96,    28,    53,   120,   169,    29,
     261,   453,   450,   906,   392,   201,   202,    81,   123,   394,
      30,   170,   263,   839,   530,   260,   320,   321,  1083,   575,
     218,   322,   808,  1483,  1091,   801,   429,   323,   544,  1330,
     820,   549,  1335,   545,  1331,   546,  1332,   548,  1334,   552,
    1339,   553,  1485,   554,  1341,   555,  1486,   556,  1343,   557,
    1487,   558,  1345,   559,  1347,   581,    31,   100,   193,   330,
     582,    32,   101,   194,   331,   586,    33,    99,   192,   431,
     827,   531,   750,  1725,   751,   954,  1716,  1717,  1718,   955,
     967,  1249,  1243,  1238,  1431,  1193,   532,   913,  1513,   914,
    1514,   939,  1534,   936,  1532,   956,   735,   533,   937,  1533,
     957,   534,  1199,  1609,  1200,  1610,  1201,  1611,   924,  1525,
     934,  1531,   729,   736,   535,  1686,   976,   536
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      23,   388,   796,   795,   728,   821,   324,   672,   981,   927,
      64,    75,   987,   989,   877,   960,   657,    52,    76,   265,
     319,   660,   580,   585,   682,  1110,   953,   584,   953,  1085,
     579,  1087,   810,  1089,   812,   456,   814,  1234,   207,  1421,
    1216,  1363,  1488,  1246,  1218,  1226,   947,  1000,   958,  1244,
     962,   694,  1001,   564,   566,   948,   136,   973,   868,   869,
      34,    35,   159,    75,    75,    75,   977,  1596,   766,  1668,
    -136,   768,   769,   127,   128,    57,    89,   768,   769,    62,
    1250,    58,   271,   747,  1097,  1257,   846,  1259,  1695,   755,
    1133,   948,   177,   178,   143,  1358,   148,    13,  1272,   862,
     103,  1440,   679,   272,    75,    75,    75,    75,   983,    63,
     657,    90,   109,   110,   111,   660,   540,    14,   984,   430,
    1280,    50,    50,  1076,   103,    62,   541,   614,   983,   204,
    1281,    62,   395,  1414,   991,   983,   171,  1382,  1443,   992,
    1713,    51,    51,  1273,  1740,   984,   189,  1134,   695,   696,
     419,   828,   269,   691,  1665,    63,   789,   790,   267,    82,
     441,    63,   789,   790,   419,   204,  1282,    54,  1112,   829,
     615,   753,    40,   985,   832,   706,   268,   420,   421,   199,
     993,   542,   144,   756,   757,  1283,   158,  1233,  1203,   205,
    1284,   420,   421,   985,   657,    84,   747,  1383,  1214,   660,
     985,  1217,   145,   219,   220,   270,   319,   657,  1002,  1192,
     318,  1272,   660,    55,    36,   754,  1107,   565,   567,   983,
    1107,  1235,  1236,  1540,   129,   205,   144,    13,   329,   130,
      85,   131,    65,   539,   132,   319,   568,   319,   621,  1632,
     397,   422,   569,   179,   697,   423,   145,    14,   180,  1237,
     181,  1107,   319,   132,  1056,   422,  1107,    91,  1107,   423,
      86,    66,   574,   698,   983,   656,  1107,   747,    92,   678,
     389,   683,  1107,   393,  1552,   133,  1497,   680,   134,   217,
     760,   761,  1272,  -755,   985,  1504,  1567,   430,   766,  1274,
     767,   768,   769,   770,   319,   319,    85,   182,   771,    97,
    1275,  1137,  1260,  1399,  1113,    67,  1400,    50,   983,   983,
     424,  1620,   418,  1418,   425,   885,  1219,   426,  1020,  1693,
    1124,   723,   725,   196,   424,    87,  1714,    51,   425,   985,
    1362,   426,   427,  1021,    68,   799,   800,   802,   428,   804,
     805,    62,   147,   809,  1627,   811,   427,   813,    13,   319,
     319,   319,   428,   319,   319,   441,   103,   319,  1719,   319,
     204,   319,   657,   319,  1227,    50,   112,   660,    14,  1729,
     212,    63,  1107,   985,   985,  1108,   789,   790,  1109,   849,
     430,    62,  1670,  1671,  1077,    51,   454,    13,   859,   452,
     991,   113,  1079,    43,    44,    45,   318,   213,   830,    13,
    1682,  1683,   833,    50,   756,   757,    69,    14,  1093,  1757,
    1758,    63,   657,   108,   676,    70,    85,   660,   442,    14,
     205,   510,   864,    51,    46,   318,   112,   318,   657,   430,
    1687,   102,  1688,   660,   198,  1127,  1690,  1691,   114,    57,
     749,   396,   318,   617,   619,    58,    77,    78,   647,    79,
     677,  1180,  1276,  1444,   681,  1722,  1723,   318,   692,   660,
     660,   660,   660,   660,   660,   660,   660,   660,   660,   660,
      50,   660,   660,   660,   660,   660,   660,    80,  1253,  1277,
    1115,  1115,  1287,  1388,   318,   318,  1240,   430,  1258,  1241,
      51,  1080,   115,   400,  1737,    13,   401,  1232,  1115,   402,
    1539,   760,   761,    13,   319,  1505,  1460,  1311,  1389,   766,
    1542,   767,   768,   769,   770,    14,  1205,  1242,   319,   771,
    1417,   577,  1450,    14,  1136,   105,   106,   107,   116,  1090,
    1099,   926,  1507,   953,  1458,  1427,  1420,    13,   794,   318,
     318,   318,  1493,   318,   318,   940,  1458,   318,   953,   318,
     911,   318,    13,   318,  1375,  1376,  1377,    14,   847,  1459,
     583,  1007,  1011,   577,  1115,   137,   160,   161,   162,   163,
      13,  1092,    14,  1563,   826,    13,  1025,    92,   577,    13,
      13,   139,    13,   784,   785,   786,   787,   788,   510,   864,
      14,  1574,    13,  1111,  1051,    14,  1336,   789,   790,    14,
      14,   577,    14,  1119,  1337,   577,   577,  1505,   577,  1349,
    1115,  1095,    14,  1411,  1413,    13,  1434,  1394,   577,  1102,
    1128,  1232,  1103,  1129,  1115,   319,  1435,  1553,    13,   925,
      13,  1395,  1506,   319,   855,    14,   319,  1581,   271,   935,
      13,   577,   938,  1101,   871,   872,  1642,  1232,    14,  1517,
      14,  1583,  1115,  1232,   577,  1232,   618,    13,   430,   272,
      14,   878,   879,   880,   881,   319,   882,  1560,   144,   264,
     142,   886,  1651,    13,   452,    75,   419,    14,   144,  1625,
    1772,   835,  1778,   577,  1350,   999,   917,  1348,   145,  1346,
      13,  1523,  1123,    14,   318,   756,   757,   152,   145,   577,
    1266,  1371,   836,   420,   421,   561,  1189,  1524,   318,  -812,
      14,  1198,  1526,  1380,  -812,  1267,   577,  1588,   319,   319,
     319,  -819,  1177,  1054,   562,   319,  -819,   430,    13,   319,
     153,  1081,  -812,  1184,   319,   319,   451,   319,   156,   319,
    1300,   319,   319,   154,  -819,  1187,  1188,   994,    14,    13,
    1448,     2,   756,   757,  1562,  1301,  1202,   825,     3,  1095,
    1095,  1208,  1209,   699,  1211,   707,  1213,   422,  1215,    14,
     540,   423,   118,   319,   319,  1305,  1114,  1580,   119,   155,
     541,     4,   700,     5,   708,     6,   109,   710,   111,   121,
    1306,     7,   760,   761,   252,   122,  1595,  1075,   253,   402,
     766,     8,   767,   768,   769,   770,   711,     9,  1093,  -826,
     771,  1663,   254,   255,  -826,   318,  -460,   256,   257,   258,
     259,  -460,   918,   318,  1100,  1694,   318,  1535,   124,   510,
     864,    10,  -826,   747,   125,   542,   424,  1298,   168,  -460,
     425,   919,  1498,   426,  1350,  1350,   149,  1191,  1272,   760,
     761,  1587,   150,    11,    12,   318,   186,   766,   427,   325,
     768,   769,   770,   164,   428,  1428,  1359,   771,  1429,   165,
    1333,  1430,   208,   209,   326,  1364,   786,   787,   788,   327,
    1664,   328,   749,   402,   319,  1131,  1365,  1366,   789,   790,
     749,  1181,  1538,    90,  1541,  1544,   319,  1182,  1115,  1356,
    1115,  1115,   190,   430,   109,   195,  1178,  1084,   318,   318,
     318,  1186,  1549,   319,  1550,   318,    13,   430,  1115,   318,
    1115,  1086,   200,  1680,   318,   318,   211,   318,   672,   318,
     430,   318,   318,  1636,  1088,   214,    14,   657,   215,  1115,
    1247,  1240,   660,  1248,  1728,   789,   790,   430,   430,   458,
     459,  1387,  1397,   430,   430,    15,   216,  1647,  1701,   173,
     174,   970,   971,   318,   318,   262,    16,   217,   419,   465,
     109,   110,   111,  1715,  1715,   467,    43,    44,    45,  1752,
     275,  1724,  1518,  1592,   383,  1715,  1724,   173,   174,   175,
    1093,   208,   209,   210,   419,   420,   421,   964,   965,   966,
     384,   840,   841,   842,   319,   319,    59,    60,    61,   391,
     319,   385,   474,   475,   140,   141,   398,  1750,  1254,  1255,
     399,   420,   421,   404,   437,  1715,  1715,   405,   406,   407,
     408,  1398,   434,   409,   410,   412,   413,  1404,   588,   589,
     590,   591,   592,   593,   594,   595,   477,   478,   414,   415,
     416,   430,   435,   436,   438,  1679,  1312,  1630,   417,   422,
    1667,   439,  1634,   423,   440,   447,   537,   596,   538,   419,
     457,   550,   551,   572,   318,   319,  1777,   597,   598,   599,
     587,   613,  1338,   620,   685,   422,   318,    62,   752,   423,
     866,   866,   866,   701,   686,   693,   420,   421,   702,   703,
     704,   705,  1447,   318,   489,   490,   491,   709,   706,   876,
     712,   713,   793,   730,  1499,   731,   716,    63,   583,   717,
     807,   718,   719,   876,   806,   720,  1710,   502,   424,  1708,
    1709,   721,   425,    16,  1385,   426,   792,   831,   822,   854,
     856,  1489,   687,   874,   860,   875,   883,   910,   916,   921,
     427,  1495,   922,   923,   424,   319,   428,  1416,   425,   508,
     422,   426,   974,   540,   423,   319,   975,  1744,  1746,  1508,
     978,  1743,   979,   541,   980,  1426,   427,   982,   988,  1379,
     990,  1433,   428,   319,  1018,  1055,  1503,   885,  1438,  1078,
    1439,  1082,   419,  1096,   318,   318,  1105,   419,  1106,  1115,
     318,    13,  1768,  1116,  1118,  1122,  1120,  1179,  1126,  1121,
    1568,   756,   757,  1184,  1196,  1206,  1232,  1239,  1265,   420,
     421,    14,  1252,  1466,   420,   421,  1268,  1269,   542,   424,
    1308,  1288,  1354,   425,   657,  1476,   426,  1278,  1279,   660,
    1481,  1289,  1290,  1355,  1586,  1291,  1292,   876,  1340,  1342,
    1589,   427,  1293,  1344,  1572,  1351,  1360,   428,  1372,  1303,
    1304,  1392,  1309,  1378,  1390,   318,   319,  1453,   319,  1391,
    1402,   419,  1454,  1403,  1415,   657,   657,  1456,  1409,  1412,
     660,   660,  1422,   422,  1425,  1449,  1457,   423,   422,  1494,
    1432,  1468,   423,  1570,  1442,  1455,  1469,   876,   420,   421,
    1471,  1477,  1482,  1484,   562,  1492,   758,   759,   760,   761,
     657,  1509,   876,  1516,  1519,   660,   766,  1520,   767,   768,
     769,   770,  1535,  1543,    13,  1548,   771,  1561,   772,   773,
    1566,  1564,  1594,  1584,   866,  1593,  1603,  1604,  1568,  1606,
    1612,  1607,  1613,  1614,    14,   318,  1537,  1619,  1618,  1621,
     577,  1490,   424,  1622,  1674,   318,   425,   424,  1204,   426,
    1641,   425,   422,  1386,   426,  1643,   423,  1648,  1673,   647,
    1676,  1551,  1685,   318,   427,  1689,  1731,  1733,  1734,   427,
     428,   319,   756,   757,  1677,   428,  1697,  1678,  1732,  1702,
     784,   785,   786,   787,   788,  1726,  1727,   866,  1735,  1736,
     319,  1751,  1753,  1754,   789,   790,  1759,  1766,   972,  1760,
    1761,   528,  1773,  1706,  1762,  1764,  1765,   419,  1767,  1774,
     547,  1775,   135,    19,    88,   187,   138,  1666,  1654,   276,
     560,   424,  1657,  1658,   909,   425,  1370,  1401,   426,  1659,
     571,  1660,  1661,    26,   420,   421,  1374,  1653,  1565,  1452,
    1511,  1599,  1512,   427,  1381,  1600,   318,  1601,   318,   428,
      98,   689,   674,  1749,  1730,   411,   675,   690,  1629,   319,
    1424,     0,   961,     0,     0,     0,     0,   758,   759,   760,
     761,   762,     0,     0,   763,   764,   765,   766,     0,   767,
     768,   769,   770,   714,   715,     0,  1707,   771,     0,   772,
     773,  1591,   419,     0,   727,     0,   876,  1597,   422,     0,
       0,     0,   423,     0,     0,   727,   737,   738,   739,   740,
     741,     0,     0,     0,     0,     0,     0,     0,     0,   420,
     421,     0,  1144,  1146,  1148,  1150,  1152,  1154,  1156,  1158,
    1160,  1162,     0,  1165,  1167,  1169,  1171,  1173,  1175,  1681,
       0,     0,     0,     0,     0,     0,   798,   780,   781,   782,
     783,   784,   785,   786,   787,   788,     0,     0,     0,     0,
       0,   318,     0,     0,   876,   789,   790,   424,     0,     0,
       0,   425,     0,  1405,   426,   824,   847,   866,   866,   866,
     318,     0,   876,   422,   876,     0,   876,   423,   876,   427,
     876,     0,   876,     0,   876,   428,   876,     0,   876,     0,
     876,     0,   876,     0,     0,   876,     0,   876,     0,   876,
       0,   876,     0,   876,     0,   876,   834,     0,     0,     0,
       0,     0,     0,     0,  1114,   994,   843,     0,     0,     0,
     848,     0,     0,   851,     0,   853,   419,     0,     0,     0,
     858,     0,     0,   863,   419,     0,     0,   870,     0,   318,
       0,   873,   424,     0,     0,     0,   425,  1712,  1406,   426,
       0,     0,     0,   420,   421,     0,     0,     0,     0,     0,
       0,   420,   421,     0,   427,     0,     0,     0,     0,   907,
     428,   994,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1748,     0,     0,   727,   928,     0,     0,   931,     0,   933,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   941,
     942,   943,   944,   945,   946,     0,   952,   422,   952,     0,
       0,   423,   756,   757,  1770,   422,  1771,     0,     0,   423,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1776,     0,
       0,  1012,  1013,   756,   757,  1014,  1015,  1016,  1017,     0,
    1019,     0,  1022,  1023,  1024,  1026,  1027,  1028,  1029,  1030,
    1031,  1033,  1034,  1035,  1036,  1037,  1038,  1039,  1040,  1041,
    1042,  1043,     0,  1052,   419,     0,   424,     0,  1057,     0,
     425,     0,  1407,   426,   424,     0,     0,     0,   425,     0,
    1408,   426,     0,     0,     0,     0,     0,     0,   427,     0,
       0,   420,   421,     0,   428,     0,   427,   758,   759,   760,
     761,   762,   428,     0,   763,   764,   765,   766,     0,   767,
     768,   769,   770,     0,     0,     0,     0,   771,   848,   772,
     773,     0,     0,  1117,     0,     0,   419,     0,   758,   759,
     760,   761,   762,     0,  1125,   763,   764,   765,   766,     0,
     767,   768,   769,   770,     0,     0,     0,     0,   771,     0,
     772,   773,     0,   420,   421,   422,   774,   775,   776,   423,
    1143,  1145,  1147,  1149,  1151,  1153,  1155,  1157,  1159,  1161,
    1163,  1164,  1166,  1168,  1170,  1172,  1174,  1176,   781,   782,
     783,   784,   785,   786,   787,   788,     0,     0,     0,     0,
    1195,     0,  1197,     0,     0,   789,   790,     0,     0,     0,
       0,     0,     0,     0,     0,   778,   876,   779,   780,   781,
     782,   783,   784,   785,   786,   787,   788,   422,     0,     0,
       0,   423,   737,  1229,   424,     0,   789,   790,   425,     0,
    1410,   426,     0,     0,     0,     0,  1251,     0,   727,     0,
       0,     0,     0,     0,  1256,     0,   427,     0,   727,     0,
       0,     0,   428,  1261,     0,  1262,     0,  1263,     0,  1264,
     600,   601,   602,   603,   604,   605,   606,   607,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   608,
       0,     0,     0,     0,     0,     0,   424,     0,     0,   609,
     425,     0,  1522,   426,     0,     0,     0,     0,   419,   610,
     611,   612,     0,     0,     0,     0,     0,     0,   427,     0,
       0,     0,     0,     0,   428,     0,     0,     0,  1302,     0,
       0,   419,  1307,     0,     0,   420,   421,     0,     0,     0,
       0,  1313,  1314,  1315,  1316,  1317,  1318,  1319,  1320,  1321,
    1322,  1323,  1324,  1325,  1326,  1327,  1328,  1329,   420,   421,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   722,     0,     0,     0,     0,     0,   277,
       0,     0,  1352,     0,     0,   278,     0,     0,  1353,     0,
       0,   279,     0,  1357,     0,     0,     0,     0,  1261,   422,
       0,   280,     0,   423,     0,     0,     0,     0,     0,   281,
       0,     0,     0,  1369,     0,     0,     0,     0,     0,     0,
       0,     0,   422,     0,   282,     0,   423,     0,     0,     0,
     876,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,   301,
     302,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,   313,   314,   315,   876,     0,   876,     0,   424,     0,
       0,     0,   425,     0,  1527,   426,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   876,     0,
     427,   424,     0,     0,     0,   425,   428,  1559,   426,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   427,  1419,     0,   316,     0,     0,   428,
       0,     0,  1423,   952,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,   458,   459,     0,     0,     0,
       0,     0,     0,     0,     0,   460,   461,   462,   463,   464,
       0,  1445,  1446,     0,     0,   465,     0,   466,     0,  1261,
       0,   467,     0,     0,     0,     0,     0,     0,     0,   468,
       0,     0,  1462,     0,  1464,   469,  1467,     0,   470,   317,
       0,   471,  1470,     0,     0,   472,  1473,     0,     0,     0,
       0,     0,     0,     0,     0,   473,     0,     0,   474,   475,
       0,   283,   284,   285,     0,   287,   288,   289,   290,   291,
     476,   293,   294,   295,   296,   297,   298,   299,   300,   301,
     302,   303,     0,   305,   306,   307,     0,     0,   310,   311,
     312,   313,   477,   478,   639,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1496,     0,     0,     0,   480,   481,
    1500,     0,     0,   674,     0,     0,     0,   845,     0,     0,
       0,     0,     0,   641,   642,   643,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
     419,   482,   483,   484,   485,   486,   727,   487,     0,   488,
     489,   490,   491,     0,     0,     0,   492,   493,   494,   495,
     496,   497,   498,    63,   499,   500,   501,   420,   421,     0,
       0,     0,     0,   502,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1554,  1555,  1556,     0,     0,
     503,   504,   505,     0,    15,     0,     0,   506,   507,     0,
       0,     0,     0,     0,     0,   508,     0,   509,     0,   510,
     511,     0,  1577,     0,  1578,     0,     0,     0,     0,     0,
    1582,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   422,     0,     0,     0,   423,  1585,  1501,     0,     0,
       0,     0,     0,     0,     0,     0,   756,   757,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1605,     0,     0,  1608,     0,     0,     0,     0,     0,     0,
       0,  1615,  1616,  1617,     0,     0,     0,     0,  1624,     0,
       0,  1626,     0,     0,  1628,     0,     0,   727,  1631,     0,
       0,     0,   727,  1635,     0,  1637,  1638,     0,     0,     0,
     424,     0,     0,     0,   425,     0,  1646,   426,     0,     0,
    1645,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   427,     0,     0,     0,     0,     0,   428,     0,
       0,     0,     0,  1662,     0,     0,     0,     0,     0,     0,
       0,   758,   759,   760,   761,   762,     0,     0,   763,   764,
     765,   766,     0,   767,   768,   769,   770,   727,     0,     0,
       0,   771,     0,   772,   773,     0,     0,     0,     0,   774,
     775,   776,     0,     0,     0,   777,     0,     0,     0,  1692,
       0,     0,     0,     0,     0,     0,     0,  1698,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1705,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1711,     0,     0,     0,     0,     0,  1720,  1721,   778,     0,
     779,   780,   781,   782,   783,   784,   785,   786,   787,   788,
       0,     0,     0,     0,     0,     0,  1738,  1739,     0,   789,
     790,     0,     0,     0,     0,     0,     0,  1742,     0,     0,
       0,  1745,  1747,   622,     0,     0,     0,   458,   459,     3,
       0,   623,   624,   625,     0,   626,     0,   460,   461,   462,
     463,   464,     0,     0,  1763,     0,     0,   465,   627,   466,
     628,   629,     0,   467,     0,     0,  1769,     0,     0,     0,
     630,   468,   631,     0,   632,     0,   633,   469,     0,     0,
     470,     0,     8,   471,   634,     0,   635,   472,     0,     0,
     636,   637,     0,     0,     0,     0,     0,   638,     0,     0,
     474,   475,     0,   283,   284,   285,     0,   287,   288,   289,
     290,   291,   476,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   303,     0,   305,   306,   307,     0,     0,
     310,   311,   312,   313,   477,   478,   639,   640,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     480,   481,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   641,   642,   643,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   482,   483,   484,   485,   486,     0,   487,
       0,   488,   489,   490,   491,     0,   144,    13,   492,   493,
     494,   495,   496,   497,   498,    63,   644,   500,   501,     0,
       0,     0,     0,     0,     0,   502,   145,    14,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   503,   504,   505,     0,    15,     0,     0,   506,
     507,     0,     0,   458,   459,     0,     0,   508,     0,   509,
       0,   510,   511,   460,   461,   462,   463,   464,     0,     0,
       0,     0,     0,   465,     0,   466,     0,     0,     0,   467,
       0,   419,     0,     0,     0,     0,     0,   468,     0,     0,
       0,     0,     0,   469,     0,     0,   470,     0,     0,   471,
       0,   948,     0,   472,     0,     0,     0,     0,   420,   421,
       0,     0,     0,   473,     0,     0,   474,   475,     0,   283,
     284,   285,     0,   287,   288,   289,   290,   291,   476,   293,
     294,   295,   296,   297,   298,   299,   300,   301,   302,   303,
       0,   305,   306,   307,     0,     0,   310,   311,   312,   313,
     477,   478,   479,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   480,   481,     0,     0,
       0,     0,   422,     0,     0,     0,   423,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   482,
     483,   484,   485,   486,     0,   487,   747,   488,   489,   490,
     491,     0,     0,     0,   492,   493,   494,   495,   496,   497,
     498,   748,   499,   500,   501,     0,     0,     0,     0,     0,
       0,   502,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,   425,     0,     0,   949,   504,
     505,     0,    15,     0,     0,   506,   507,     0,     0,     0,
     458,   459,     0,   950,     0,   951,     0,   510,   511,   428,
     460,   461,   462,   463,   464,     0,     0,     0,     0,     0,
     465,     0,   466,     0,     0,     0,   467,     0,   419,     0,
       0,     0,     0,     0,   468,     0,     0,     0,     0,     0,
     469,     0,     0,   470,     0,     0,   471,     0,     0,     0,
     472,     0,     0,     0,     0,   420,   421,     0,     0,     0,
     473,     0,     0,   474,   475,     0,   283,   284,   285,     0,
     287,   288,   289,   290,   291,   476,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,     0,   305,   306,
     307,     0,     0,   310,   311,   312,   313,   477,   478,   479,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   480,   481,     0,     0,     0,     0,   422,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   482,   483,   484,   485,
     486,     0,   487,   747,   488,   489,   490,   491,     0,     0,
       0,   492,   493,   494,   495,   496,   497,   498,   748,   499,
     500,   501,     0,     0,     0,     0,     0,     0,   502,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   424,     0,
       0,     0,   425,     0,     0,   949,   504,   505,     0,    15,
       0,     0,   506,   507,     0,     0,     0,   458,   459,     0,
     950,     0,   959,     0,   510,   511,   428,   460,   461,   462,
     463,   464,     0,     0,     0,     0,     0,   465,     0,   466,
       0,     0,     0,   467,     0,   566,     0,     0,     0,     0,
       0,   468,     0,     0,     0,     0,     0,   469,     0,     0,
     470,     0,     0,   471,     0,     0,     0,   472,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   473,     0,     0,
     474,   475,     0,   283,   284,   285,     0,   287,   288,   289,
     290,   291,   476,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   303,     0,   305,   306,   307,     0,     0,
     310,   311,   312,   313,   477,   478,   479,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     480,   481,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   482,   483,   484,   485,   486,     0,   487,
       0,   488,   489,   490,   491,     0,     0,     0,   492,   493,
     494,   495,   496,   497,   498,    63,   499,   500,   501,     0,
       0,     0,     0,     0,     0,   502,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   567,
       0,     0,   503,   504,   505,     0,    15,     0,     0,   506,
     507,     0,     0,     0,   458,   459,     0,  1228,     0,   509,
       0,   510,   511,   569,   460,   461,   462,   463,   464,     0,
       0,     0,     0,     0,   465,     0,   466,     0,     0,     0,
     467,     0,     0,     0,     0,     0,     0,     0,   468,     0,
       0,     0,     0,     0,   469,     0,     0,   470,     0,     0,
     471,     0,     0,     0,   472,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   473,     0,     0,   474,   475,     0,
     283,   284,   285,     0,   287,   288,   289,   290,   291,   476,
     293,   294,   295,   296,   297,   298,   299,   300,   301,   302,
     303,     0,   305,   306,   307,     0,     0,   310,   311,   312,
     313,   477,   478,   639,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   480,   481,     0,
       0,     0,     0,     0,     0,     0,   861,     0,     0,     0,
       0,     0,   641,   642,   643,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     482,   483,   484,   485,   486,     0,   487,     0,   488,   489,
     490,   491,     0,     0,     0,   492,   493,   494,   495,   496,
     497,   498,    63,   499,   500,   501,     0,     0,     0,     0,
       0,     0,   502,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   503,
     504,   505,     0,    15,     0,     0,   506,   507,     0,     0,
     458,   459,     0,     0,   508,     0,   509,     0,   510,   511,
     460,   461,   462,   463,   464,     0,     0,     0,     0,     0,
     465,  1655,   466,   628,     0,     0,   467,     0,     0,     0,
       0,     0,     0,     0,   468,     0,     0,     0,     0,     0,
     469,     0,     0,   470,     0,     0,   471,   634,     0,     0,
     472,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     473,     0,     0,   474,   475,     0,   283,   284,   285,     0,
     287,   288,   289,   290,   291,   476,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,     0,   305,   306,
     307,     0,     0,   310,   311,   312,   313,   477,   478,   479,
    1656,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   480,   481,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   482,   483,   484,   485,
     486,     0,   487,     0,   488,   489,   490,   491,     0,     0,
       0,   492,   493,   494,   495,   496,   497,   498,    63,   499,
     500,   501,     0,     0,     0,     0,     0,     0,   502,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   503,   504,   505,     0,    15,
       0,     0,   506,   507,     0,     0,   458,   459,     0,     0,
     508,     0,   509,     0,   510,   511,   460,   461,   462,   463,
     464,     0,     0,     0,     0,     0,   465,     0,   466,     0,
       0,     0,   467,     0,     0,     0,     0,     0,     0,     0,
     468,     0,     0,     0,     0,     0,   469,     0,     0,   470,
       0,     0,   471,     0,     0,     0,   472,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   473,     0,     0,   474,
     475,     0,   283,   284,   285,     0,   287,   288,   289,   290,
     291,   476,   293,   294,   295,   296,   297,   298,   299,   300,
     301,   302,   303,     0,   305,   306,   307,     0,     0,   310,
     311,   312,   313,   477,   478,   639,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   480,
     481,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   641,   642,   643,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   482,   483,   484,   485,   486,     0,   487,     0,
     488,   489,   490,   491,     0,     0,     0,   492,   493,   494,
     495,   496,   497,   498,    63,   499,   500,   501,     0,     0,
       0,     0,     0,     0,   502,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   503,   504,   505,     0,    15,     0,     0,   506,   507,
       0,     0,   458,   459,     0,     0,   508,     0,   509,     0,
     510,   511,   460,   461,   462,   463,   464,     0,     0,     0,
       0,     0,   465,     0,   466,     0,     0,     0,   467,     0,
       0,     0,     0,     0,     0,     0,   468,     0,     0,     0,
       0,     0,   469,     0,     0,   470,     0,     0,   471,     0,
       0,     0,   472,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   473,     0,     0,   474,   475,   996,   283,   284,
     285,     0,   287,   288,   289,   290,   291,   476,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,     0,
     305,   306,   307,     0,     0,   310,   311,   312,   313,   477,
     478,   479,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   480,   481,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   482,   483,
     484,   485,   486,     0,   487,   747,   488,   489,   490,   491,
       0,     0,     0,   492,   493,   494,   495,   496,   497,   498,
     748,   499,   500,   501,     0,     0,     0,     0,     0,     0,
     502,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   503,   504,   505,
       0,    15,     0,     0,   506,   507,     0,     0,   458,   459,
       0,     0,   997,     0,   509,   998,   510,   511,   460,   461,
     462,   463,   464,     0,     0,     0,     0,     0,   465,     0,
     466,     0,     0,     0,   467,     0,     0,     0,     0,     0,
       0,     0,   468,     0,     0,     0,     0,     0,   469,     0,
       0,   470,     0,     0,   471,     0,     0,     0,   472,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   473,     0,
       0,   474,   475,     0,   283,   284,   285,     0,   287,   288,
     289,   290,   291,   476,   293,   294,   295,   296,   297,   298,
     299,   300,   301,   302,   303,     0,   305,   306,   307,     0,
       0,   310,   311,   312,   313,   477,   478,   639,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   480,   481,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1138,  1139,  1140,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   482,   483,   484,   485,   486,     0,
     487,     0,   488,   489,   490,   491,     0,     0,     0,   492,
     493,   494,   495,   496,   497,   498,    63,   499,   500,   501,
       0,     0,     0,     0,     0,     0,   502,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   503,   504,   505,     0,    15,     0,     0,
     506,   507,     0,     0,     0,     0,   458,   459,   508,     0,
     509,     0,   510,   511,   742,     0,   460,   461,   462,   463,
     464,     0,     0,     0,     0,     0,   465,     0,   466,     0,
       0,     0,   467,     0,     0,     0,     0,     0,     0,     0,
     468,     0,     0,     0,     0,     0,   469,     0,     0,   470,
     743,     0,   471,     0,     0,     0,   472,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   473,     0,     0,   474,
     475,     0,   283,   284,   285,     0,   287,   288,   289,   290,
     291,   476,   293,   294,   295,   296,   297,   298,   299,   300,
     301,   302,   303,     0,   305,   306,   307,     0,     0,   310,
     311,   312,   313,   477,   478,   479,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   480,
     481,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   482,   483,   484,   485,   486,     0,   487,     0,
     488,   489,   490,   491,     0,     0,     0,   492,   493,   494,
     495,   496,   497,   498,    63,   499,   500,   501,     0,     0,
       0,     0,     0,     0,   502,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   503,   504,   505,     0,    15,     0,     0,   506,   507,
       0,     0,     0,     0,   458,   459,   508,   570,   509,     0,
     510,   511,   742,     0,   460,   461,   462,   463,   464,     0,
       0,     0,     0,     0,   465,     0,   466,     0,     0,     0,
     467,     0,     0,     0,     0,     0,     0,     0,   468,     0,
       0,     0,     0,     0,   469,     0,     0,   470,   743,     0,
     471,     0,     0,     0,   472,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   473,     0,     0,   474,   475,     0,
     283,   284,   285,     0,   287,   288,   289,   290,   291,   476,
     293,   294,   295,   296,   297,   298,   299,   300,   301,   302,
     303,     0,   305,   306,   307,     0,     0,   310,   311,   312,
     313,   477,   478,   479,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   480,   481,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     482,   483,   484,   485,   486,     0,   487,   747,   488,   489,
     490,   491,     0,     0,     0,   492,   493,   494,   495,   496,
     497,   498,   748,   499,   500,   501,     0,     0,     0,     0,
       0,     0,   502,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   503,
     504,   505,     0,    15,     0,     0,   506,   507,     0,     0,
       0,     0,   458,   459,   508,     0,   509,     0,   510,   511,
     742,     0,   460,   461,   462,   463,   464,     0,     0,     0,
       0,     0,   465,     0,   466,     0,     0,     0,   467,     0,
       0,     0,     0,     0,     0,     0,   468,     0,     0,     0,
       0,     0,   469,     0,     0,   470,   743,     0,   471,     0,
       0,     0,   472,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   473,     0,     0,   474,   475,     0,   283,   284,
     285,     0,   287,   288,   289,   290,   291,   476,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,     0,
     305,   306,   307,     0,     0,   310,   311,   312,   313,   477,
     478,   479,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   480,   481,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   482,   483,
     484,   485,   486,     0,   487,     0,   488,   489,   490,   491,
       0,     0,     0,   492,   493,   494,   495,   496,   497,   498,
      63,   499,   500,   501,     0,     0,     0,     0,     0,     0,
     502,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   503,   504,   505,
       0,    15,     0,     0,   506,   507,     0,     0,     0,     0,
     458,   459,   508,   822,   509,     0,   510,   511,   742,     0,
     460,   461,   462,   463,   464,     0,     0,     0,     0,     0,
     465,     0,   466,     0,     0,     0,   467,     0,     0,     0,
       0,     0,     0,     0,   468,     0,     0,     0,     0,     0,
     469,     0,     0,   470,   743,     0,   471,     0,     0,     0,
     472,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     473,     0,     0,   474,   475,     0,   283,   284,   285,     0,
     287,   288,   289,   290,   291,   476,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,     0,   305,   306,
     307,     0,     0,   310,   311,   312,   313,   477,   478,   479,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   480,   481,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   482,   483,   484,   485,
     486,     0,   487,     0,   488,   489,   490,   491,     0,     0,
       0,   492,   493,   494,   495,   496,   497,   498,    63,   499,
     500,   501,     0,     0,     0,     0,     0,     0,   502,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   503,   504,   505,     0,    15,
       0,     0,   506,   507,     0,     0,   458,   459,     0,     0,
     508,     0,   509,     0,   510,   511,   460,   461,   462,   463,
     464,     0,     0,     0,     0,     0,   465,     0,   466,     0,
       0,     0,   467,     0,     0,     0,     0,     0,     0,     0,
     468,     0,     0,     0,     0,     0,   469,     0,     0,   470,
       0,     0,   471,     0,     0,     0,   472,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   473,     0,     0,   474,
     475,  1190,   283,   284,   285,     0,   287,   288,   289,   290,
     291,   476,   293,   294,   295,   296,   297,   298,   299,   300,
     301,   302,   303,     0,   305,   306,   307,     0,     0,   310,
     311,   312,   313,   477,   478,   479,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   480,
     481,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   482,   483,   484,   485,   486,     0,   487,   747,
     488,   489,   490,   491,     0,     0,     0,   492,   493,   494,
     495,   496,   497,   498,   748,   499,   500,   501,     0,     0,
       0,     0,     0,     0,   502,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   503,   504,   505,     0,    15,     0,     0,   506,   507,
       0,     0,   458,   459,     0,     0,   508,     0,   509,     0,
     510,   511,   460,   461,   462,   463,   464,     0,     0,     0,
       0,     0,   465,     0,   466,     0,     0,     0,   467,     0,
       0,     0,     0,     0,     0,     0,   468,     0,     0,     0,
       0,     0,   469,     0,     0,   470,     0,     0,   471,     0,
       0,     0,   472,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   473,     0,     0,   474,   475,     0,   283,   284,
     285,     0,   287,   288,   289,   290,   291,   476,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,     0,
     305,   306,   307,     0,     0,   310,   311,   312,   313,   477,
     478,   479,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   480,   481,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   482,   483,
     484,   485,   486,     0,   487,   747,   488,   489,   490,   491,
       0,     0,     0,   492,   493,   494,   495,   496,   497,   498,
     748,   499,   500,   501,     0,     0,     0,     0,     0,     0,
     502,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   503,   504,   505,
       0,    15,     0,     0,   506,   507,     0,     0,   458,   459,
       0,     0,   508,     0,   509,  1230,   510,   511,   460,   461,
     462,   463,   464,     0,     0,     0,     0,     0,   465,     0,
     466,     0,     0,     0,   467,     0,     0,     0,     0,     0,
       0,     0,   468,     0,     0,     0,     0,     0,   469,     0,
       0,   470,     0,     0,   471,     0,     0,     0,   472,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   473,     0,
       0,   474,   475,     0,   283,   284,   285,     0,   287,   288,
     289,   290,   291,   476,   293,   294,   295,   296,   297,   298,
     299,   300,   301,   302,   303,     0,   305,   306,   307,     0,
       0,   310,   311,   312,   313,   477,   478,   479,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   480,   481,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   482,   483,   484,   485,   486,     0,
     487,   747,   488,   489,   490,   491,     0,     0,     0,   492,
     493,   494,   495,   496,   497,   498,   748,   499,   500,   501,
       0,     0,     0,     0,     0,     0,   502,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   503,   504,   505,     0,    15,     0,     0,
     506,   507,     0,     0,   458,   459,     0,     0,   508,     0,
     509,  1245,   510,   511,   460,   461,   462,   463,   464,     0,
       0,     0,     0,     0,   465,     0,   466,     0,     0,     0,
     467,     0,     0,     0,     0,     0,     0,     0,   468,     0,
       0,     0,     0,     0,   469,     0,     0,   470,     0,     0,
     471,     0,     0,     0,   472,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   473,     0,     0,   474,   475,     0,
     283,   284,   285,     0,   287,   288,   289,   290,   291,   476,
     293,   294,   295,   296,   297,   298,   299,   300,   301,   302,
     303,     0,   305,   306,   307,     0,     0,   310,   311,   312,
     313,   477,   478,   479,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   480,   481,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     482,   483,   484,   485,   486,     0,   487,     0,   488,   489,
     490,   491,     0,     0,     0,   492,   493,   494,   495,   496,
     497,   498,    63,   499,   500,   501,     0,     0,     0,     0,
       0,     0,   502,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   503,
     504,   505,     0,    15,     0,     0,   506,   507,     0,     0,
       0,     0,   458,   459,   508,   570,   509,     0,   510,   511,
     726,     0,   460,   461,   462,   463,   464,     0,     0,     0,
       0,     0,   465,     0,   466,     0,     0,     0,   467,     0,
       0,     0,     0,     0,     0,     0,   468,     0,     0,     0,
       0,     0,   469,     0,     0,   470,     0,     0,   471,     0,
       0,     0,   472,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   473,     0,     0,   474,   475,     0,   283,   284,
     285,     0,   287,   288,   289,   290,   291,   476,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,     0,
     305,   306,   307,     0,     0,   310,   311,   312,   313,   477,
     478,   479,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   480,   481,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   482,   483,
     484,   485,   486,     0,   487,     0,   488,   489,   490,   491,
       0,     0,     0,   492,   493,   494,   495,   496,   497,   498,
      63,   499,   500,   501,     0,     0,     0,     0,     0,     0,
     502,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   503,   504,   505,
       0,    15,     0,     0,   506,   507,     0,     0,     0,     0,
     458,   459,   508,     0,   509,     0,   510,   511,   733,     0,
     460,   461,   462,   463,   464,     0,     0,     0,     0,     0,
     465,     0,   466,     0,     0,     0,   467,     0,     0,     0,
       0,     0,     0,     0,   468,     0,     0,     0,     0,     0,
     469,     0,     0,   470,     0,     0,   471,     0,     0,     0,
     472,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     473,     0,     0,   474,   475,     0,   283,   284,   285,     0,
     287,   288,   289,   290,   291,   476,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,     0,   305,   306,
     307,     0,     0,   310,   311,   312,   313,   477,   478,   479,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   480,   481,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   482,   483,   484,   485,
     486,     0,   487,     0,   488,   489,   490,   491,     0,     0,
       0,   492,   493,   494,   495,   496,   497,   498,    63,   499,
     500,   501,     0,     0,     0,     0,     0,     0,   502,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   503,   504,   505,     0,    15,
       0,     0,   506,   507,     0,     0,   458,   459,     0,     0,
     508,     0,   509,     0,   510,   511,   460,   461,   462,   463,
     464,     0,     0,     0,     0,     0,   465,     0,   466,     0,
       0,     0,   467,     0,     0,     0,     0,     0,     0,     0,
     468,     0,     0,     0,     0,     0,   469,     0,     0,   470,
       0,     0,   471,     0,     0,     0,   472,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   473,     0,     0,   474,
     475,     0,   283,   284,   285,     0,   287,   288,   289,   290,
     291,   476,   293,   294,   295,   296,   297,   298,   299,   300,
     301,   302,   303,     0,   305,   306,   307,     0,     0,   310,
     311,   312,   313,   477,   478,   479,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   480,
     481,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   482,   483,   484,   485,   486,     0,   487,   747,
     488,   489,   490,   491,     0,     0,     0,   492,   493,   494,
     495,   496,   497,   498,   748,   499,   500,   501,     0,     0,
       0,     0,     0,     0,   502,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   503,   504,   505,     0,    15,     0,     0,   506,   507,
       0,     0,   458,   459,     0,     0,   508,     0,   509,     0,
     510,   511,   460,   461,   462,   463,   464,     0,     0,     0,
       0,     0,   465,     0,   466,     0,     0,     0,   467,     0,
       0,     0,     0,     0,     0,     0,   468,     0,     0,     0,
       0,     0,   469,     0,     0,   470,     0,     0,   471,     0,
       0,     0,   472,     0,     0,     0,     0,     0,   850,     0,
       0,     0,   473,     0,     0,   474,   475,     0,   283,   284,
     285,     0,   287,   288,   289,   290,   291,   476,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,     0,
     305,   306,   307,     0,     0,   310,   311,   312,   313,   477,
     478,   479,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   480,   481,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   482,   483,
     484,   485,   486,     0,   487,     0,   488,   489,   490,   491,
       0,     0,     0,   492,   493,   494,   495,   496,   497,   498,
      63,   499,   500,   501,     0,     0,     0,     0,     0,     0,
     502,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   503,   504,   505,
       0,    15,     0,     0,   506,   507,     0,     0,   458,   459,
       0,     0,   508,     0,   509,     0,   510,   511,   460,   461,
     462,   463,   464,     0,     0,     0,     0,     0,   465,     0,
     466,     0,     0,     0,   467,     0,     0,     0,     0,     0,
       0,     0,   468,     0,     0,     0,     0,     0,   469,     0,
       0,   470,     0,     0,   471,     0,     0,     0,   472,     0,
       0,   857,     0,     0,     0,     0,     0,     0,   473,     0,
       0,   474,   475,     0,   283,   284,   285,     0,   287,   288,
     289,   290,   291,   476,   293,   294,   295,   296,   297,   298,
     299,   300,   301,   302,   303,     0,   305,   306,   307,     0,
       0,   310,   311,   312,   313,   477,   478,   479,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   480,   481,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   482,   483,   484,   485,   486,     0,
     487,     0,   488,   489,   490,   491,     0,     0,     0,   492,
     493,   494,   495,   496,   497,   498,    63,   499,   500,   501,
       0,     0,     0,     0,     0,     0,   502,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   503,   504,   505,     0,    15,     0,     0,
     506,   507,     0,     0,   458,   459,     0,     0,   508,     0,
     509,     0,   510,   511,   460,   461,   462,   463,   464,     0,
       0,     0,     0,     0,   465,     0,   466,     0,     0,     0,
     467,     0,     0,     0,     0,     0,     0,     0,   468,     0,
       0,     0,     0,     0,   469,     0,     0,   470,     0,     0,
     471,     0,     0,     0,   472,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   473,     0,     0,   474,   475,     0,
     283,   284,   285,     0,   287,   288,   289,   290,   291,   476,
     293,   294,   295,   296,   297,   298,   299,   300,   301,   302,
     303,     0,   305,   306,   307,     0,     0,   310,   311,   312,
     313,   477,   478,   479,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   480,   481,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     482,   483,   484,   485,   486,     0,   487,     0,   488,   489,
     490,   491,     0,     0,     0,   492,   493,   494,   495,   496,
     497,   498,    63,   499,   500,   501,     0,     0,     0,     0,
       0,     0,   502,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   730,     0,   503,
     504,   505,     0,    15,     0,     0,   506,   507,     0,     0,
     458,   459,     0,     0,   508,     0,   509,     0,   510,   511,
     460,   461,   462,   463,   464,     0,     0,  1032,     0,     0,
     465,     0,   466,     0,     0,     0,   467,     0,     0,     0,
       0,     0,     0,     0,   468,     0,     0,     0,     0,     0,
     469,     0,     0,   470,     0,     0,   471,     0,     0,     0,
     472,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     473,     0,     0,   474,   475,     0,   283,   284,   285,     0,
     287,   288,   289,   290,   291,   476,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,     0,   305,   306,
     307,     0,     0,   310,   311,   312,   313,   477,   478,   479,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   480,   481,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   482,   483,   484,   485,
     486,     0,   487,     0,   488,   489,   490,   491,     0,     0,
       0,   492,   493,   494,   495,   496,   497,   498,    63,   499,
     500,   501,     0,     0,     0,     0,     0,     0,   502,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   503,   504,   505,     0,    15,
       0,     0,   506,   507,     0,     0,   458,   459,     0,     0,
     508,     0,   509,     0,   510,   511,   460,   461,   462,   463,
     464,     0,     0,     0,     0,     0,   465,     0,   466,     0,
       0,     0,   467,     0,     0,     0,     0,     0,     0,     0,
     468,     0,     0,     0,     0,     0,   469,     0,     0,   470,
       0,     0,   471,     0,     0,     0,   472,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   473,     0,     0,   474,
     475,     0,   283,   284,   285,     0,   287,   288,   289,   290,
     291,   476,   293,   294,   295,   296,   297,   298,   299,   300,
     301,   302,   303,     0,   305,   306,   307,     0,     0,   310,
     311,   312,   313,   477,   478,   479,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   480,
     481,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   482,   483,   484,   485,   486,     0,   487,     0,
     488,   489,   490,   491,     0,     0,     0,   492,   493,   494,
     495,   496,   497,   498,    63,   499,   500,   501,     0,     0,
       0,     0,     0,     0,   502,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   503,   504,   505,     0,    15,     0,     0,   506,   507,
       0,     0,   458,   459,     0,     0,   508,     0,   509,  1053,
     510,   511,   460,   461,   462,   463,   464,     0,     0,     0,
       0,     0,   465,     0,   466,     0,     0,     0,   467,     0,
       0,     0,     0,     0,     0,     0,   468,     0,     0,     0,
       0,     0,   469,     0,     0,   470,     0,     0,   471,     0,
       0,     0,   472,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   473,     0,     0,   474,   475,     0,   283,   284,
     285,     0,   287,   288,   289,   290,   291,   476,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,     0,
     305,   306,   307,     0,     0,   310,   311,   312,   313,   477,
     478,   479,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   480,   481,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   482,   483,
     484,   485,   486,     0,   487,     0,   488,   489,   490,   491,
       0,     0,     0,   492,   493,   494,   495,   496,   497,   498,
      63,   499,   500,   501,     0,     0,     0,     0,     0,     0,
     502,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1194,     0,   503,   504,   505,
       0,    15,     0,     0,   506,   507,     0,     0,   458,   459,
       0,     0,   508,     0,   509,     0,   510,   511,   460,   461,
     462,   463,   464,     0,     0,     0,     0,     0,   465,     0,
     466,     0,     0,     0,   467,     0,     0,     0,     0,     0,
       0,     0,   468,     0,     0,     0,     0,     0,   469,     0,
       0,   470,     0,     0,   471,     0,     0,     0,   472,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   473,     0,
       0,   474,   475,     0,   283,   284,   285,     0,   287,   288,
     289,   290,   291,   476,   293,   294,   295,   296,   297,   298,
     299,   300,   301,   302,   303,     0,   305,   306,   307,     0,
       0,   310,   311,   312,   313,   477,   478,   479,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   480,   481,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   482,   483,   484,   485,   486,     0,
     487,     0,   488,   489,   490,   491,     0,     0,     0,   492,
     493,   494,   495,   496,   497,   498,    63,   499,   500,   501,
       0,     0,     0,     0,     0,     0,   502,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   503,   504,   505,     0,    15,     0,     0,
     506,   507,     0,     0,   458,   459,     0,     0,   508,     0,
     509,  1465,   510,   511,   460,   461,   462,   463,   464,     0,
       0,     0,     0,     0,   465,     0,   466,     0,     0,     0,
     467,     0,     0,     0,     0,     0,     0,     0,   468,     0,
       0,     0,     0,     0,   469,     0,     0,   470,     0,     0,
     471,     0,     0,     0,   472,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   473,     0,     0,   474,   475,     0,
     283,   284,   285,     0,   287,   288,   289,   290,   291,   476,
     293,   294,   295,   296,   297,   298,   299,   300,   301,   302,
     303,     0,   305,   306,   307,     0,     0,   310,   311,   312,
     313,   477,   478,   479,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   480,   481,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     482,   483,   484,   485,   486,     0,   487,     0,   488,   489,
     490,   491,     0,     0,     0,   492,   493,   494,   495,   496,
     497,   498,    63,   499,   500,   501,     0,     0,     0,     0,
       0,     0,   502,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   503,
     504,   505,     0,    15,     0,     0,   506,   507,     0,     0,
     458,   459,     0,     0,  1474,     0,   509,  1475,   510,   511,
     460,   461,   462,   463,   464,     0,     0,     0,     0,     0,
     465,     0,   466,     0,     0,     0,   467,     0,     0,     0,
       0,     0,     0,     0,   468,     0,     0,     0,     0,     0,
     469,     0,     0,   470,     0,     0,   471,     0,     0,     0,
     472,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     473,     0,     0,   474,   475,     0,   283,   284,   285,     0,
     287,   288,   289,   290,   291,   476,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,     0,   305,   306,
     307,     0,     0,   310,   311,   312,   313,   477,   478,   479,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   480,   481,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   482,   483,   484,   485,
     486,     0,   487,     0,   488,   489,   490,   491,     0,     0,
       0,   492,   493,   494,   495,   496,   497,   498,    63,   499,
     500,   501,     0,     0,     0,     0,     0,     0,   502,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   503,   504,   505,     0,    15,
       0,     0,   506,   507,     0,     0,   458,   459,     0,     0,
     508,     0,   509,  1480,   510,   511,   460,   461,   462,   463,
     464,     0,     0,     0,     0,     0,   465,     0,   466,     0,
       0,     0,   467,     0,     0,     0,     0,     0,     0,     0,
     468,     0,     0,     0,     0,     0,   469,     0,     0,   470,
       0,     0,   471,     0,     0,     0,   472,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   473,     0,     0,   474,
     475,     0,   283,   284,   285,     0,   287,   288,   289,   290,
     291,   476,   293,   294,   295,   296,   297,   298,   299,   300,
     301,   302,   303,     0,   305,   306,   307,     0,     0,   310,
     311,   312,   313,   477,   478,   479,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   480,
     481,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   482,   483,   484,   485,   486,     0,   487,     0,
     488,   489,   490,   491,     0,     0,     0,   492,   493,   494,
     495,   496,   497,   498,    63,   499,   500,   501,     0,     0,
       0,     0,     0,     0,   502,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   503,   504,   505,     0,    15,     0,     0,   506,   507,
       0,     0,   458,   459,     0,     0,   508,     0,   509,  1536,
     510,   511,   460,   461,   462,   463,   464,     0,     0,     0,
       0,     0,   465,     0,   466,     0,     0,     0,   467,     0,
       0,     0,     0,     0,     0,     0,   468,     0,     0,     0,
       0,     0,   469,     0,     0,   470,     0,     0,   471,     0,
       0,     0,   472,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   473,     0,     0,   474,   475,     0,   283,   284,
     285,     0,   287,   288,   289,   290,   291,   476,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,     0,
     305,   306,   307,     0,     0,   310,   311,   312,   313,   477,
     478,   479,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   480,   481,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   482,   483,
     484,   485,   486,     0,   487,     0,   488,   489,   490,   491,
       0,     0,     0,   492,   493,   494,   495,   496,   497,   498,
      63,   499,   500,   501,     0,     0,     0,     0,     0,     0,
     502,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   503,   504,   505,
       0,    15,     0,     0,   506,   507,     0,     0,   458,   459,
       0,     0,   508,     0,   509,  1623,   510,   511,   460,   461,
     462,   463,   464,     0,     0,     0,     0,     0,   465,     0,
     466,     0,     0,     0,   467,     0,     0,     0,     0,     0,
       0,     0,   468,     0,     0,     0,     0,     0,   469,     0,
       0,   470,     0,     0,   471,     0,     0,     0,   472,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   473,     0,
       0,   474,   475,     0,   283,   284,   285,     0,   287,   288,
     289,   290,   291,   476,   293,   294,   295,   296,   297,   298,
     299,   300,   301,   302,   303,     0,   305,   306,   307,     0,
       0,   310,   311,   312,   313,   477,   478,   479,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   480,   481,     0,     0,     0,     0,     0,     0,     0,
    1644,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   482,   483,   484,   485,   486,     0,
     487,     0,   488,   489,   490,   491,     0,     0,     0,   492,
     493,   494,   495,   496,   497,   498,    63,   499,   500,   501,
       0,     0,     0,     0,     0,     0,   502,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   503,   504,   505,     0,    15,     0,     0,
     506,   507,     0,     0,   458,   459,     0,     0,   508,     0,
     509,     0,   510,   511,   460,   461,   462,   463,   464,     0,
       0,     0,     0,     0,   465,     0,   466,     0,     0,     0,
     467,     0,     0,     0,     0,     0,     0,     0,   468,     0,
       0,     0,     0,     0,   469,     0,     0,   470,     0,     0,
     471,     0,     0,     0,   472,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   473,     0,     0,   474,   475,     0,
     283,   284,   285,     0,   287,   288,   289,   290,   291,   476,
     293,   294,   295,   296,   297,   298,   299,   300,   301,   302,
     303,     0,   305,   306,   307,     0,     0,   310,   311,   312,
     313,   477,   478,   479,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   480,   481,     0,
       0,     0,     0,     0,     0,     0,  1703,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     482,   483,   484,   485,   486,     0,   487,     0,   488,   489,
     490,   491,     0,     0,     0,   492,   493,   494,   495,   496,
     497,   498,    63,   499,   500,   501,     0,     0,     0,     0,
       0,     0,   502,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   503,
     504,   505,     0,    15,     0,     0,   506,   507,     0,     0,
     458,   459,     0,     0,   508,     0,   509,     0,   510,   511,
     460,   461,   462,   463,   464,     0,     0,     0,     0,     0,
     465,     0,   466,     0,     0,     0,   467,     0,     0,     0,
       0,     0,     0,     0,   468,     0,     0,     0,     0,     0,
     469,     0,     0,   470,     0,     0,   471,     0,     0,     0,
     472,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     473,     0,     0,   474,   475,     0,   283,   284,   285,     0,
     287,   288,   289,   290,   291,   476,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,     0,   305,   306,
     307,     0,     0,   310,   311,   312,   313,   477,   478,   479,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   480,   481,     0,     0,     0,     0,     0,
       0,     0,  1704,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   482,   483,   484,   485,
     486,     0,   487,     0,   488,   489,   490,   491,     0,     0,
       0,   492,   493,   494,   495,   496,   497,   498,    63,   499,
     500,   501,     0,     0,     0,     0,     0,     0,   502,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   503,   504,   505,     0,    15,
       0,     0,   506,   507,     0,     0,   458,   459,     0,     0,
     508,     0,   509,     0,   510,   511,   460,   461,   462,   463,
     464,     0,     0,     0,     0,     0,   465,     0,   466,     0,
       0,     0,   467,     0,     0,     0,     0,     0,     0,     0,
     468,     0,     0,     0,     0,     0,   469,     0,     0,   470,
       0,     0,   471,     0,     0,     0,   472,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   473,     0,     0,   474,
     475,     0,   283,   284,   285,     0,   287,   288,   289,   290,
     291,   476,   293,   294,   295,   296,   297,   298,   299,   300,
     301,   302,   303,     0,   305,   306,   307,     0,     0,   310,
     311,   312,   313,   477,   478,   479,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   480,
     481,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   482,   483,   484,   485,   486,     0,   487,     0,
     488,   489,   490,   491,     0,     0,     0,   492,   493,   494,
     495,   496,   497,   498,    63,   499,   500,   501,     0,     0,
       0,     0,     0,     0,   502,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   503,   504,   505,     0,    15,     0,     0,   506,   507,
       0,     0,   458,   459,     0,     0,   508,     0,   509,     0,
     510,   511,   460,   461,   462,   463,   464,     0,     0,     0,
       0,     0,   465,     0,   466,     0,     0,     0,   467,     0,
       0,     0,     0,     0,     0,     0,   468,     0,     0,     0,
       0,     0,   469,     0,     0,   470,     0,     0,   471,     0,
       0,     0,   472,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   473,     0,     0,   474,   475,     0,   283,   284,
     285,     0,   287,   288,   289,   290,   291,   476,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,     0,
     305,   306,   307,     0,     0,   310,   311,   312,   313,   477,
     478,   479,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   480,   481,     0,     0,     0,
     -78,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   756,   757,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   482,   483,
     484,   485,   486,     0,   487,     0,   488,   489,   490,   491,
       0,     0,     0,   492,   493,   494,   495,   496,   497,   498,
      63,   499,   500,   501,     0,     0,     0,     0,   756,   757,
     502,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   503,   504,   505,
       0,    15,     0,     0,   506,   507,     0,     0,     0,     0,
       0,     0,  1451,     0,   509,     0,   510,   511,   887,   888,
     889,   890,   891,   892,   893,   894,   758,   759,   760,   761,
     762,   895,   896,   763,   764,   765,   766,   897,   767,   768,
     769,   770,     0,     0,     0,     0,   771,   898,   772,   773,
     899,   900,     0,     0,   774,   775,   776,   901,   902,   903,
     777,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   758,   759,   760,   761,   762,     0,     0,
     763,   764,   765,   766,     0,   767,   768,   769,   770,     0,
       0,     0,     0,   771,     0,   772,   773,     0,     0,     0,
       0,   774,   904,   778,     0,   779,   780,   781,   782,   783,
     784,   785,   786,   787,   788,   724,     0,     0,     0,     0,
       0,   277,     0,     0,   789,   790,     0,   278,     0,   510,
     864,     0,     0,   279,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   280,     0,     0,     0,     0,     0,     0,
       0,   281,   779,   780,   781,   782,   783,   784,   785,   786,
     787,   788,     0,     0,     0,     0,   282,     0,     0,     0,
       0,   789,   790,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,   313,   314,   315,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   277,     0,     0,     0,     0,     0,
     278,     0,     0,     0,     0,     0,   279,     0,     0,     0,
       0,     0,     0,     0,     0,    62,   280,     0,     0,     0,
       0,     0,     0,     0,   281,     0,     0,     0,   316,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   282,
       0,     0,     0,     0,     0,    63,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   317,     0,     0,     0,     0,     0,   277,     0,     0,
       0,     0,     0,   278,     0,     0,     0,     0,     0,   279,
       0,     0,     0,     0,     0,     0,     0,     0,    62,   280,
       0,     0,     0,     0,     0,     0,     0,   281,     0,     0,
       0,   316,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   282,     0,     0,     0,     0,     0,    63,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295,   296,   297,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,   313,
     314,   315,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   317,     0,   573,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   316,     0,     0,     0,     0,     0,
       0,     0,     0,    13,     0,     0,   277,     0,     0,     0,
       0,   576,   278,     0,     0,     0,     0,     0,   279,     0,
       0,     0,     0,    14,     0,     0,     0,     0,   280,   577,
       0,     0,     0,     0,     0,     0,   281,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   282,     0,     0,     0,     0,     0,   317,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   277,
       0,     0,     0,     0,     0,   278,     0,     0,     0,     0,
       0,   279,     0,     0,     0,     0,     0,     0,     0,     0,
      62,   280,     0,     0,     0,     0,     0,     0,     0,   281,
       0,     0,     0,   316,     0,     0,     0,     0,     0,     0,
       0,     0,   756,   757,   282,     0,     0,     0,     0,     0,
      63,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,   301,
     302,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,   313,   314,   315,     0,   756,   757,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   317,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   316,   758,   759,   760,
     761,   762,     0,     0,   763,   764,   765,   766,     0,   767,
     768,   769,   770,   576,     0,     0,     0,   771,     0,   772,
     773,     0,     0,     0,     0,   774,     0,   776,     0,     0,
       0,     0,  1058,  1059,  1060,  1061,  1062,  1063,  1064,  1065,
     758,   759,   760,   761,   762,  1066,  1067,   763,   764,   765,
     766,  1068,   767,   768,   769,   770,     0,     0,     0,   317,
     771,   898,   772,   773,  1069,  1070,   756,   757,   774,   775,
     776,  1071,  1072,  1073,   777,     0,   779,   780,   781,   782,
     783,   784,   785,   786,   787,   788,     0,     0,     0,    13,
       0,     0,     0,     0,     0,   789,   790,     0,     0,     0,
       0,   756,   757,     0,     0,     0,     0,     0,     0,    14,
       0,     0,     0,     0,     0,     0,  1074,   778,     0,   779,
     780,   781,   782,   783,   784,   785,   786,   787,   788,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   789,   790,
       0,     0,     0,   510,   864,     0,     0,     0,     0,     0,
       0,     0,     0,  1058,  1059,  1060,  1061,  1062,  1063,  1064,
    1065,   758,   759,   760,   761,   762,  1066,  1067,   763,   764,
     765,   766,  1068,   767,   768,   769,   770,  -410,     0,     0,
       0,   771,   898,   772,   773,  1069,  1070,     0,     0,   774,
     775,   776,  1071,  1072,  1073,   777,   758,   759,   760,   761,
     762,     0,     0,   763,   764,   765,   766,     0,   767,   768,
     769,   770,   756,   757,     0,     0,   771,     0,   772,   773,
       0,     0,     0,     0,   774,   775,   776,     0,     0,     0,
     777,     0,     0,     0,     0,     0,     0,  1074,   778,     0,
     779,   780,   781,   782,   783,   784,   785,   786,   787,   788,
     756,   757,     0,     0,     0,     0,     0,     0,     0,   789,
     790,     0,     0,     0,   510,   864,     0,     0,     0,     0,
       0,     0,     0,   778,     0,   779,   780,   781,   782,   783,
     784,   785,   786,   787,   788,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   789,   790,     0,     0,     0,   510,
     864,     0,     0,     0,     0,     0,     0,   758,   759,   760,
     761,   762,     0,     0,   763,   764,   765,   766,     0,   767,
     768,   769,   770,     0,     0,     0,     0,   771,     0,   772,
     773,     0,     0,     0,     0,   774,   775,   776,     0,     0,
       0,   777,     0,     0,     0,   758,   759,   760,   761,   762,
       0,     0,   763,   764,   765,   766,     0,   767,   768,   769,
     770,   756,   757,     0,     0,   771,     0,   772,   773,     0,
       0,     0,     0,   774,   775,   776,     0,     0,     0,   777,
       0,     0,     0,     0,   778,     0,   779,   780,   781,   782,
     783,   784,   785,   786,   787,   788,   756,   757,     0,     0,
       0,     0,     0,     0,     0,   789,   790,     0,     0,   791,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   778,     0,   779,   780,   781,   782,   783,   784,
     785,   786,   787,   788,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   789,   790,     0,     0,   803,     0,     0,
       0,     0,     0,     0,     0,     0,   758,   759,   760,   761,
     762,     0,     0,   763,   764,   765,   766,     0,   767,   768,
     769,   770,     0,     0,     0,     0,   771,     0,   772,   773,
       0,     0,     0,     0,   774,   775,   776,     0,     0,     0,
     777,   758,   759,   760,   761,   762,     0,     0,   763,   764,
     765,   766,     0,   767,   768,   769,   770,   756,   757,     0,
       0,   771,     0,   772,   773,     0,     0,     0,     0,   774,
     775,   776,     0,     0,     0,   777,     0,     0,     0,     0,
       0,     0,     0,   778,     0,   779,   780,   781,   782,   783,
     784,   785,   786,   787,   788,   756,   757,     0,     0,     0,
       0,     0,     0,     0,   789,   790,     0,     0,   819,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   778,     0,
     779,   780,   781,   782,   783,   784,   785,   786,   787,   788,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   789,
     790,     0,     0,  1104,     0,     0,     0,     0,     0,     0,
       0,     0,   758,   759,   760,   761,   762,     0,     0,   763,
     764,   765,   766,     0,   767,   768,   769,   770,     0,     0,
       0,     0,   771,     0,   772,   773,     0,     0,     0,     0,
     774,   775,   776,     0,     0,     0,   777,     0,     0,     0,
     758,   759,   760,   761,   762,     0,     0,   763,   764,   765,
     766,     0,   767,   768,   769,   770,   756,   757,     0,     0,
     771,     0,   772,   773,     0,     0,     0,     0,   774,   775,
     776,     0,     0,     0,   777,     0,     0,     0,     0,   778,
       0,   779,   780,   781,   782,   783,   784,   785,   786,   787,
     788,   756,   757,     0,     0,     0,     0,     0,     0,     0,
     789,   790,     0,     0,  1207,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   778,     0,   779,
     780,   781,   782,   783,   784,   785,   786,   787,   788,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   789,   790,
       0,     0,  1210,     0,     0,     0,     0,     0,     0,     0,
       0,   758,   759,   760,   761,   762,     0,     0,   763,   764,
     765,   766,     0,   767,   768,   769,   770,     0,     0,     0,
       0,   771,     0,   772,   773,     0,     0,     0,     0,   774,
     775,   776,     0,     0,     0,   777,   758,   759,   760,   761,
     762,     0,     0,   763,   764,   765,   766,     0,   767,   768,
     769,   770,   756,   757,     0,     0,   771,     0,   772,   773,
       0,     0,     0,     0,   774,   775,   776,     0,     0,     0,
     777,     0,     0,     0,     0,     0,     0,     0,   778,     0,
     779,   780,   781,   782,   783,   784,   785,   786,   787,   788,
     756,   757,     0,     0,     0,     0,     0,     0,     0,   789,
     790,     0,     0,  1212,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   778,     0,   779,   780,   781,   782,   783,
     784,   785,   786,   787,   788,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   789,   790,     0,     0,  1220,     0,
       0,     0,     0,     0,     0,     0,     0,   758,   759,   760,
     761,   762,     0,     0,   763,   764,   765,   766,     0,   767,
     768,   769,   770,     0,     0,     0,     0,   771,     0,   772,
     773,     0,     0,     0,     0,   774,   775,   776,     0,     0,
       0,   777,     0,     0,     0,   758,   759,   760,   761,   762,
       0,     0,   763,   764,   765,   766,     0,   767,   768,   769,
     770,   756,   757,     0,     0,   771,     0,   772,   773,     0,
       0,     0,     0,   774,   775,   776,     0,     0,     0,   777,
       0,     0,     0,     0,   778,     0,   779,   780,   781,   782,
     783,   784,   785,   786,   787,   788,   756,   757,     0,     0,
       0,     0,     0,     0,     0,   789,   790,     0,     0,  1221,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   778,     0,   779,   780,   781,   782,   783,   784,
     785,   786,   787,   788,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   789,   790,     0,     0,  1222,     0,     0,
       0,     0,     0,     0,     0,     0,   758,   759,   760,   761,
     762,     0,     0,   763,   764,   765,   766,     0,   767,   768,
     769,   770,     0,     0,     0,     0,   771,     0,   772,   773,
       0,     0,     0,     0,   774,   775,   776,     0,     0,     0,
     777,   758,   759,   760,   761,   762,     0,     0,   763,   764,
     765,   766,     0,   767,   768,   769,   770,   756,   757,     0,
       0,   771,     0,   772,   773,     0,     0,     0,     0,   774,
     775,   776,     0,     0,     0,   777,     0,     0,     0,     0,
       0,     0,     0,   778,     0,   779,   780,   781,   782,   783,
     784,   785,   786,   787,   788,   756,   757,     0,     0,     0,
       0,     0,     0,     0,   789,   790,     0,     0,  1223,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   778,     0,
     779,   780,   781,   782,   783,   784,   785,   786,   787,   788,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   789,
     790,     0,     0,  1224,     0,     0,     0,     0,     0,     0,
       0,     0,   758,   759,   760,   761,   762,     0,     0,   763,
     764,   765,   766,     0,   767,   768,   769,   770,     0,     0,
       0,     0,   771,     0,   772,   773,     0,     0,     0,     0,
     774,   775,   776,     0,     0,     0,   777,     0,     0,     0,
     758,   759,   760,   761,   762,     0,     0,   763,   764,   765,
     766,     0,   767,   768,   769,   770,   756,   757,     0,     0,
     771,     0,   772,   773,     0,     0,     0,     0,   774,   775,
     776,     0,     0,     0,   777,     0,     0,     0,     0,   778,
       0,   779,   780,   781,   782,   783,   784,   785,   786,   787,
     788,   756,   757,     0,     0,     0,     0,     0,     0,     0,
     789,   790,     0,     0,  1225,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   778,     0,   779,
     780,   781,   782,   783,   784,   785,   786,   787,   788,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   789,   790,
       0,     0,  1393,     0,     0,     0,     0,     0,     0,     0,
       0,   758,   759,   760,   761,   762,     0,     0,   763,   764,
     765,   766,     0,   767,   768,   769,   770,     0,     0,     0,
       0,   771,     0,   772,   773,     0,     0,     0,     0,   774,
     775,   776,     0,     0,     0,   777,   758,   759,   760,   761,
     762,     0,     0,   763,   764,   765,   766,     0,   767,   768,
     769,   770,   756,   757,     0,     0,   771,     0,   772,   773,
       0,     0,     0,     0,   774,   775,   776,     0,     0,     0,
     777,     0,     0,     0,     0,     0,     0,     0,   778,     0,
     779,   780,   781,   782,   783,   784,   785,   786,   787,   788,
     756,   757,     0,     0,     0,     0,     0,     0,     0,   789,
     790,     0,     0,  1396,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   778,     0,   779,   780,   781,   782,   783,
     784,   785,   786,   787,   788,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   789,   790,     0,     0,  1441,     0,
       0,     0,     0,     0,     0,     0,     0,   758,   759,   760,
     761,   762,     0,     0,   763,   764,   765,   766,     0,   767,
     768,   769,   770,     0,     0,     0,     0,   771,     0,   772,
     773,     0,     0,     0,     0,   774,   775,   776,     0,     0,
       0,   777,     0,     0,     0,   758,   759,   760,   761,   762,
       0,     0,   763,   764,   765,   766,     0,   767,   768,   769,
     770,   756,   757,     0,     0,   771,     0,   772,   773,     0,
       0,     0,     0,   774,   775,   776,     0,     0,     0,   777,
       0,     0,     0,     0,   778,     0,   779,   780,   781,   782,
     783,   784,   785,   786,   787,   788,   756,   757,     0,     0,
       0,     0,     0,     0,     0,   789,   790,     0,     0,  1491,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   778,     0,   779,   780,   781,   782,   783,   784,
     785,   786,   787,   788,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   789,   790,     0,     0,  1557,     0,     0,
       0,     0,     0,     0,     0,     0,   758,   759,   760,   761,
     762,     0,     0,   763,   764,   765,   766,     0,   767,   768,
     769,   770,     0,     0,     0,     0,   771,     0,   772,   773,
       0,     0,     0,     0,   774,   775,   776,     0,     0,     0,
     777,   758,   759,   760,   761,   762,     0,     0,   763,   764,
     765,   766,     0,   767,   768,   769,   770,   756,   757,     0,
       0,   771,     0,   772,   773,     0,     0,     0,     0,   774,
     775,   776,     0,     0,     0,   777,     0,     0,     0,     0,
       0,     0,     0,   778,     0,   779,   780,   781,   782,   783,
     784,   785,   786,   787,   788,   756,   757,     0,     0,     0,
       0,     0,     0,     0,   789,   790,     0,     0,  1558,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   778,     0,
     779,   780,   781,   782,   783,   784,   785,   786,   787,   788,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   789,
     790,     0,     0,  1571,     0,     0,     0,     0,     0,     0,
       0,     0,   758,   759,   760,   761,   762,     0,     0,   763,
     764,   765,   766,     0,   767,   768,   769,   770,     0,     0,
       0,     0,   771,     0,   772,   773,     0,     0,     0,     0,
     774,   775,   776,     0,     0,     0,   777,     0,     0,     0,
     758,   759,   760,   761,   762,     0,     0,   763,   764,   765,
     766,     0,   767,   768,   769,   770,   756,   757,     0,     0,
     771,     0,   772,   773,     0,     0,     0,     0,   774,   775,
     776,     0,     0,     0,   777,     0,     0,     0,     0,   778,
       0,   779,   780,   781,   782,   783,   784,   785,   786,   787,
     788,   756,   757,     0,     0,     0,     0,     0,     0,     0,
     789,   790,     0,     0,  1573,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   778,     0,   779,
     780,   781,   782,   783,   784,   785,   786,   787,   788,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   789,   790,
       0,     0,  1575,     0,     0,     0,     0,     0,     0,     0,
       0,   758,   759,   760,   761,   762,     0,     0,   763,   764,
     765,   766,     0,   767,   768,   769,   770,     0,     0,     0,
       0,   771,     0,   772,   773,     0,     0,     0,     0,   774,
     775,   776,     0,     0,     0,   777,   758,   759,   760,   761,
     762,     0,     0,   763,   764,   765,   766,     0,   767,   768,
     769,   770,   756,   757,     0,     0,   771,     0,   772,   773,
       0,     0,     0,     0,   774,   775,   776,     0,     0,     0,
     777,     0,     0,     0,     0,     0,     0,     0,   778,     0,
     779,   780,   781,   782,   783,   784,   785,   786,   787,   788,
     756,   757,     0,     0,     0,     0,     0,     0,     0,   789,
     790,     0,     0,  1579,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   778,     0,   779,   780,   781,   782,   783,
     784,   785,   786,   787,   788,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   789,   790,     0,     0,  1639,     0,
       0,     0,     0,     0,     0,     0,     0,   758,   759,   760,
     761,   762,     0,     0,   763,   764,   765,   766,     0,   767,
     768,   769,   770,     0,     0,     0,     0,   771,     0,   772,
     773,     0,     0,     0,     0,   774,   775,   776,     0,     0,
       0,   777,     0,     0,     0,   758,   759,   760,   761,   762,
       0,     0,   763,   764,   765,   766,     0,   767,   768,   769,
     770,   756,   757,     0,     0,   771,     0,   772,   773,     0,
       0,     0,     0,   774,   775,   776,     0,     0,     0,   777,
       0,     0,     0,     0,   778,     0,   779,   780,   781,   782,
     783,   784,   785,   786,   787,   788,   756,   757,     0,     0,
       0,     0,     0,     0,     0,   789,   790,     0,     0,  1649,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   778,     0,   779,   780,   781,   782,   783,   784,
     785,   786,   787,   788,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   789,   790,     0,     0,  1650,     0,     0,
       0,     0,     0,     0,     0,     0,   758,   759,   760,   761,
     762,     0,     0,   763,   764,   765,   766,     0,   767,   768,
     769,   770,     0,     0,     0,     0,   771,     0,   772,   773,
       0,     0,     0,     0,   774,   775,   776,     0,     0,     0,
     777,   758,   759,   760,   761,   762,     0,     0,   763,   764,
     765,   766,     0,   767,   768,   769,   770,   756,   757,     0,
       0,   771,     0,   772,   773,     0,     0,     0,     0,   774,
     775,   776,     0,     0,     0,   777,     0,     0,     0,     0,
       0,     0,     0,   778,     0,   779,   780,   781,   782,   783,
     784,   785,   786,   787,   788,   756,   757,     0,     0,     0,
       0,     0,     0,     0,   789,   790,     0,     0,  1652,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   778,     0,
     779,   780,   781,   782,   783,   784,   785,   786,   787,   788,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   789,
     790,     0,     0,  1672,     0,     0,     0,     0,     0,     0,
       0,     0,   758,   759,   760,   761,   762,     0,     0,   763,
     764,   765,   766,     0,   767,   768,   769,   770,     0,     0,
       0,     0,   771,     0,   772,   773,     0,     0,     0,     0,
     774,   775,   776,     0,     0,     0,   777,     0,     0,     0,
     758,   759,   760,   761,   762,     0,     0,   763,   764,   765,
     766,     0,   767,   768,   769,   770,   756,   757,     0,     0,
     771,     0,   772,   773,     0,     0,     0,     0,   774,   775,
     776,     0,     0,     0,   777,     0,     0,     0,     0,   778,
       0,   779,   780,   781,   782,   783,   784,   785,   786,   787,
     788,   756,   757,     0,     0,     0,     0,     0,     0,     0,
     789,   790,     0,     0,  1675,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   778,     0,   779,
     780,   781,   782,   783,   784,   785,   786,   787,   788,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   789,   790,
       0,     0,  1684,     0,     0,     0,     0,     0,     0,     0,
       0,   758,   759,   760,   761,   762,     0,     0,   763,   764,
     765,   766,     0,   767,   768,   769,   770,     0,     0,     0,
       0,   771,     0,   772,   773,     0,     0,     0,     0,   774,
     775,   776,     0,     0,     0,   777,   758,   759,   760,   761,
     762,     0,     0,   763,   764,   765,   766,     0,   767,   768,
     769,   770,   756,   757,     0,     0,   771,     0,   772,   773,
       0,     0,     0,     0,   774,   775,   776,     0,     0,     0,
     777,     0,     0,     0,     0,     0,     0,     0,   778,     0,
     779,   780,   781,   782,   783,   784,   785,   786,   787,   788,
     756,   757,     0,     0,     0,     0,     0,     0,     0,   789,
     790,     0,     0,  1755,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   778,     0,   779,   780,   781,   782,   783,
     784,   785,   786,   787,   788,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   789,   790,     0,     0,  1756,     0,
       0,     0,     0,     0,     0,     0,     0,   758,   759,   760,
     761,   762,     0,     0,   763,   764,   765,   766,     0,   767,
     768,   769,   770,     0,     0,     0,     0,   771,     0,   772,
     773,     0,     0,     0,     0,   774,   775,   776,     0,     0,
       0,   777,     0,     0,     0,   758,   759,   760,   761,   762,
       0,     0,   763,   764,   765,   766,     0,   767,   768,   769,
     770,   756,   757,     0,     0,   771,     0,   772,   773,     0,
       0,     0,     0,   774,   775,   776,     0,     0,     0,   777,
       0,     0,     0,     0,   778,     0,   779,   780,   781,   782,
     783,   784,   785,   786,   787,   788,   756,   757,     0,     0,
       0,     0,     0,     0,     0,   789,   790,   823,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   778,     0,   779,   780,   781,   782,   783,   784,
     785,   786,   787,   788,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   789,   790,  1098,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   758,   759,   760,   761,
     762,     0,     0,   763,   764,   765,   766,     0,   767,   768,
     769,   770,     0,     0,     0,     0,   771,     0,   772,   773,
       0,     0,     0,     0,   774,   775,   776,     0,     0,     0,
     777,   758,   759,   760,   761,   762,     0,     0,   763,   764,
     765,   766,     0,   767,   768,   769,   770,   756,   757,     0,
       0,   771,     0,   772,   773,     0,     0,     0,     0,   774,
     775,   776,     0,     0,     0,   777,     0,     0,     0,     0,
       0,     0,     0,   778,     0,   779,   780,   781,   782,   783,
     784,   785,   786,   787,   788,   756,   757,     0,     0,     0,
       0,     0,     0,     0,   789,   790,  1294,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   778,     0,
     779,   780,   781,   782,   783,   784,   785,   786,   787,   788,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   789,
     790,  1310,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   758,   759,   760,   761,   762,     0,     0,   763,
     764,   765,   766,     0,   767,   768,   769,   770,     0,     0,
       0,     0,   771,     0,   772,   773,     0,     0,     0,     0,
     774,   775,   776,     0,     0,     0,   777,     0,     0,     0,
     758,   759,   760,   761,   762,     0,     0,   763,   764,   765,
     766,     0,   767,   768,   769,   770,   332,   333,     0,     0,
     771,     0,   772,   773,     0,     0,     0,     0,   774,   775,
     776,     0,     0,   334,   777,     0,     0,     0,     0,   778,
       0,   779,   780,   781,   782,   783,   784,   785,   786,   787,
     788,     0,     0,     0,     0,     0,     0,     0,   756,   757,
     789,   790,  1472,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   778,     0,   779,
     780,   781,   782,   783,   784,   785,   786,   787,   788,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   789,   790,
    1478,     0,     0,   335,   336,   337,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,     0,     0,   353,   354,   355,     0,     0,     0,     0,
       0,     0,   356,   357,   358,   359,   360,     0,     0,   361,
     362,   363,   364,   365,   366,   367,     0,     0,     0,     0,
       0,   756,   757,   758,   759,   760,   761,   762,     0,     0,
     763,   764,   765,   766,     0,   767,   768,   769,   770,     0,
       0,     0,     0,   771,     0,   772,   773,     0,     0,     0,
       0,   774,   775,   776,     0,     0,     0,   777,   368,     0,
     369,   370,   371,   372,   373,   374,   375,   376,   377,   378,
      50,     0,   379,   380,     0,     0,     0,     0,     0,   381,
     382,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     778,     0,   779,   780,   781,   782,   783,   784,   785,   786,
     787,   788,     0,     0,   756,   757,   758,   759,   760,   761,
     762,   789,   790,   763,   764,   765,   766,     0,   767,   768,
     769,   770,     0,     0,     0,     0,   771,     0,   772,   773,
       0,     0,     0,     0,   774,   775,   776,     0,     0,     0,
     777,   756,   757,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    13,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    14,     0,     0,     0,     0,
       0,     0,     0,   778,     0,   779,   780,   781,   782,   783,
     784,   785,   786,   787,   788,     0,     0,     0,     0,   758,
     759,   760,   761,   762,   789,   790,   763,   764,   765,   766,
       0,   767,   768,   769,   770,     0,     0,     0,     0,   771,
       0,   772,   773,     0,     0,   963,     0,   774,   775,   776,
       0,     0,     0,   777,   756,   757,   758,   759,   760,   761,
     762,     0,     0,   763,   764,   765,   766,     0,   767,   768,
     769,   770,     0,     0,     0,     0,   771,     0,   772,   773,
       0,     0,  1231,     0,   774,   775,   776,     0,     0,     0,
     777,   756,   757,     0,     0,     0,   778,     0,   779,   780,
     781,   782,   783,   784,   785,   786,   787,   788,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   789,   790,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   778,     0,   779,   780,   781,   782,   783,
     784,   785,   786,   787,   788,     0,     0,     0,     0,   758,
     759,   760,   761,   762,   789,   790,   763,   764,   765,   766,
       0,   767,   768,   769,   770,     0,     0,     0,     0,   771,
       0,   772,   773,     0,     0,     0,     0,   774,   775,   776,
       0,     0,     0,   777,   756,   757,   758,   759,   760,   761,
     762,     0,     0,   763,   764,   765,   766,     0,   767,   768,
     769,   770,     0,     0,     0,     0,   771,     0,   772,   773,
       0,     0,     0,     0,   774,   775,   776,     0,     0,     0,
     777,   756,   757,     0,     0,     0,   778,  1299,   779,   780,
     781,   782,   783,   784,   785,   786,   787,   788,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   789,   790,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   778,  1436,   779,   780,   781,   782,   783,
     784,   785,   786,   787,   788,     0,     0,     0,     0,   758,
     759,   760,   761,   762,   789,   790,   763,   764,   765,   766,
       0,   767,   768,   769,   770,     0,     0,     0,     0,   771,
       0,   772,   773,     0,     0,     0,     0,   774,   775,   776,
       0,     0,     0,   777,   756,   757,   758,   759,   760,   761,
     762,     0,     0,   763,   764,   765,   766,     0,   767,   768,
     769,   770,     0,     0,     0,     0,   771,     0,   772,   773,
       0,     0,     0,     0,   774,   775,   776,     0,  1685,     0,
     777,   756,   757,     0,     0,     0,   778,     0,   779,   780,
     781,   782,   783,   784,   785,   786,   787,   788,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   789,   790,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   778,     0,   779,   780,   781,   782,   783,
     784,   785,   786,   787,   788,     0,     0,     0,     0,   758,
     759,   760,   761,   762,   789,   790,   763,   764,   765,   766,
       0,   767,   768,   769,   770,     0,     0,     0,     0,   771,
       0,   772,   773,     0,     0,     0,     0,   774,   775,   776,
       0,     0,     0,  -827,   756,   757,   758,   759,   760,   761,
     762,     0,     0,   763,   764,   765,   766,     0,   767,   768,
     769,   770,     0,     0,     0,     0,   771,     0,   772,   773,
     756,   757,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   778,     0,   779,   780,
     781,   782,   783,   784,   785,   786,   787,   788,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   789,   790,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   779,   780,   781,   782,   783,
     784,   785,   786,   787,   788,     0,     0,  1004,     0,   758,
     759,   760,   761,   762,   789,   790,   763,   764,   765,   766,
       0,   767,   768,   769,   770,     0,     0,     0,     0,   771,
       0,   772,   773,     0,     0,   758,   759,   760,   761,   762,
       0,     0,   763,     0,     0,   766,     0,   767,   768,   769,
     770,     0,     0,     0,     0,   771,     0,   772,   773,   283,
     284,   285,     0,   287,   288,   289,   290,   291,   476,   293,
     294,   295,   296,   297,   298,   299,   300,   301,   302,   303,
       0,   305,   306,   307,     0,     0,   310,   311,   312,   313,
       0,   782,   783,   784,   785,   786,   787,   788,     0,     0,
       0,     0,     0,     0,     0,     0,  1008,   789,   790,     0,
       0,     0,     0,     0,     0,     0,     0,   782,   783,   784,
     785,   786,   787,   788,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   789,   790,     0,     0,  1295,     0,     0,
       0,     0,     0,     0,     0,     0,  1005,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   283,   284,
     285,  1006,   287,   288,   289,   290,   291,   476,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,     0,
     305,   306,   307,     0,     0,   310,   311,   312,   313,   283,
     284,   285,     0,   287,   288,   289,   290,   291,   476,   293,
     294,   295,   296,   297,   298,   299,   300,   301,   302,   303,
       0,   305,   306,   307,     0,     0,   310,   311,   312,   313,
     283,   284,   285,     0,   287,   288,   289,   290,   291,   476,
     293,   294,   295,   296,   297,   298,   299,   300,   301,   302,
     303,     0,   305,   306,   307,  1009,   221,   310,   311,   312,
     313,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1010,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1296,     0,  1044,  1045,
       0,     0,   222,     0,   223,     0,   224,   225,   226,   227,
     228,  1297,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,     0,   240,   241,   242,  1046,     0,   243,
     244,   245,   246,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1047,     0,     0,     0,     0,     0,     0,   247,
     248,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1048,  1049,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   249
};

static const yytype_int16 yycheck[] =
{
       1,   251,   543,   539,   491,   562,   215,   449,   745,   700,
      15,    16,   749,   750,   660,   725,   449,     7,    19,   203,
     215,   449,   432,   433,   455,   839,   723,   433,   725,   810,
     432,   812,   553,   814,   555,   395,   557,   954,   172,  1234,
     937,  1127,  1349,   960,   939,    20,   722,   754,   724,    20,
     726,     5,     4,    33,    33,    53,    85,   733,   642,   643,
      19,    20,   108,    68,    69,    70,   742,  1507,   126,  1598,
       8,   129,   130,    15,    16,    57,    22,   129,   130,   143,
      20,    63,   152,   158,   821,    20,   627,    20,  1640,   520,
     160,    53,    15,    16,    95,    20,    97,   165,   173,   640,
     143,    20,    40,   173,   109,   110,   111,   112,   127,   173,
     543,    57,   144,   145,   146,   543,   127,   185,   137,   195,
     127,   163,   163,   199,   143,   143,   137,     7,   127,   160,
     137,   143,   266,  1219,   152,   127,   126,    46,   137,   157,
    1669,   183,   183,   218,  1696,   137,   147,   217,   102,   103,
      33,   194,   220,   217,  1594,   173,   214,   215,   204,    62,
     155,   173,   214,   215,    33,   160,   173,   173,   844,   579,
      50,   173,   166,   192,   584,   217,   205,    60,    61,   169,
     198,   192,   164,    21,    22,   192,   218,   185,   925,   220,
     197,    60,    61,   192,   627,   192,   158,   106,   935,   627,
     192,   938,   184,   193,   194,   206,   401,   640,   160,   916,
     215,   173,   640,   219,   173,   217,   191,   197,   197,   127,
     191,   188,   189,  1418,   166,   220,   164,   165,   218,   171,
     191,   173,    34,   401,   176,   430,   215,   432,   447,  1546,
     269,   124,   221,   166,   198,   128,   184,   185,   171,   216,
     173,   191,   447,   176,   795,   124,   191,   203,   191,   128,
     194,    63,   430,   217,   127,   449,   191,   158,   214,   453,
     260,   455,   191,   263,   137,   217,  1362,   215,   220,   194,
     118,   119,   173,   198,   192,  1371,   142,   195,   126,   996,
     128,   129,   130,   131,   489,   490,   191,   220,   136,    53,
     997,   885,   978,  1200,   845,   107,  1201,   163,   127,   127,
     193,  1532,   317,  1230,   197,   132,   199,   200,   158,   137,
     861,   489,   490,   218,   193,   191,   217,   183,   197,   192,
     199,   200,   215,   173,   136,   544,   545,   546,   221,   548,
     549,   143,    96,   552,  1539,   554,   215,   556,   165,   544,
     545,   546,   221,   548,   549,   155,   143,   552,  1671,   554,
     160,   556,   795,   558,   948,   163,   191,   795,   185,  1682,
     191,   173,   191,   192,   192,   194,   214,   215,   197,   629,
     195,   143,  1603,  1604,   199,   183,   391,   165,   638,   390,
     152,   216,   802,   173,   174,   175,   401,   218,   582,   165,
    1621,  1622,   586,   163,    21,    22,   208,   185,   818,  1722,
    1723,   173,   845,   217,   192,   217,   191,   845,   218,   185,
     220,   219,   220,   183,   204,   430,   191,   432,   861,   195,
    1628,   173,  1630,   861,   194,   866,  1634,  1635,   107,    57,
     509,   216,   447,   444,   445,    63,     5,     6,   449,     8,
     451,   216,   191,  1267,   455,  1676,  1677,   462,   463,   887,
     888,   889,   890,   891,   892,   893,   894,   895,   896,   897,
     163,   899,   900,   901,   902,   903,   904,    36,   965,   218,
     191,   191,  1003,  1190,   489,   490,   185,   195,   975,   188,
     183,   199,   107,   191,  1692,   165,   194,   191,   191,   197,
    1417,   118,   119,   165,   699,   191,  1287,   218,   218,   126,
    1427,   128,   129,   130,   131,   185,   926,   216,   713,   136,
    1230,   191,   216,   185,   884,    68,    69,    70,   107,   199,
     192,   699,   218,  1230,   191,  1245,  1233,   165,   539,   544,
     545,   546,  1356,   548,   549,   713,   191,   552,  1245,   554,
     684,   556,   165,   558,  1138,  1139,  1140,   185,   627,   216,
     173,   756,   757,   191,   191,   166,   109,   110,   111,   112,
     165,   199,   185,   218,   575,   165,   771,   214,   191,   165,
     165,   173,   165,   200,   201,   202,   203,   204,   219,   220,
     185,   218,   165,   843,   789,   185,   191,   214,   215,   185,
     185,   191,   185,   853,   199,   191,   191,   191,   191,   199,
     191,   820,   185,   199,   199,   165,   199,   185,   191,   828,
     870,   191,   831,   873,   191,   820,   199,  1441,   165,   698,
     165,   199,   216,   828,   635,   185,   831,   218,   152,   708,
     165,   191,   711,   827,   645,   646,   216,   191,   185,   199,
     185,   218,   191,   191,   191,   191,   191,   165,   195,   173,
     185,   662,   663,   664,   665,   860,   667,  1448,   164,   165,
     173,   672,   216,   165,   675,   680,    33,   185,   164,   218,
     218,   152,   218,   191,  1094,   754,   691,  1093,   184,  1091,
     165,   199,   860,   185,   699,    21,    22,   173,   184,   191,
     158,  1132,   173,    60,    61,   198,   915,   199,   713,   194,
     185,   920,  1403,    47,   199,   173,   191,  1498,   913,   914,
     915,   194,   906,   792,   217,   920,   199,   195,   165,   924,
     173,   199,   217,    67,   929,   930,   173,   932,   177,   934,
     158,   936,   937,   173,   217,   913,   914,   752,   185,   165,
    1271,     0,    21,    22,  1451,   173,   924,   173,     7,   968,
     969,   929,   930,   198,   932,   198,   934,   124,   936,   185,
     127,   128,    57,   968,   969,   158,   845,  1474,    63,   173,
     137,    30,   217,    32,   217,    34,   144,   198,   146,    57,
     173,    40,   118,   119,    75,    63,   194,   798,    79,   197,
     126,    50,   128,   129,   130,   131,   217,    56,  1218,   194,
     136,  1592,    93,    94,   199,   820,   194,    98,    99,   100,
     101,   199,   198,   828,   825,  1639,   831,   217,    57,   219,
     220,    80,   217,   158,    63,   192,   193,  1032,   173,   217,
     197,   217,  1363,   200,  1254,  1255,    57,   916,   173,   118,
     119,  1497,    63,   102,   103,   860,   220,   126,   215,    79,
     129,   130,   131,    57,   221,   184,  1116,   136,   187,    63,
    1079,   190,   177,   178,    94,    12,   202,   203,   204,    99,
     194,   101,   951,   197,  1079,   875,    23,    24,   214,   215,
     959,    57,   185,    57,   185,   185,  1091,    63,   191,  1108,
     191,   191,   208,   195,   144,   177,   907,   199,   913,   914,
     915,   912,   185,  1108,   185,   920,   165,   195,   191,   924,
     191,   199,   106,  1614,   929,   930,   220,   932,  1370,   934,
     195,   936,   937,   185,   199,   173,   185,  1370,   192,   191,
     184,   185,  1370,   187,  1681,   214,   215,   195,   195,     5,
       6,   199,   199,   195,   195,   204,    66,   199,   199,   177,
     178,   179,   180,   968,   969,   173,   215,   194,    33,    25,
     144,   145,   146,  1670,  1671,    31,   173,   174,   175,  1716,
     173,  1678,  1392,  1504,    35,  1682,  1683,   177,   178,   179,
    1400,   177,   178,   179,    33,    60,    61,   184,   185,   186,
      35,   617,   618,   619,  1199,  1200,    10,    11,    12,   194,
    1205,   217,    68,    69,    91,    92,   217,  1714,   968,   969,
      43,    60,    61,   198,   216,  1722,  1723,   198,   198,   217,
     198,  1199,   173,   198,   217,   198,   198,  1205,   108,   109,
     110,   111,   112,   113,   114,   115,   102,   103,   198,   198,
     198,   195,   173,   173,    22,  1612,  1057,  1544,   217,   124,
    1596,   173,  1549,   128,   216,   194,   173,   137,   173,    33,
     216,   173,   198,   215,  1079,  1270,  1773,   147,   148,   149,
     173,   217,  1083,   218,   198,   124,  1091,   143,   220,   128,
     641,   642,   643,   217,   198,   198,    60,    61,   198,   198,
     217,   198,  1270,  1108,   160,   161,   162,   198,   217,   660,
     198,   198,    43,   198,  1364,   198,   217,   173,   173,   217,
     199,   217,   217,   674,   218,   217,  1667,   183,   193,  1665,
    1666,   217,   197,   215,   199,   200,   217,   194,   216,   173,
     166,  1350,   198,    10,   198,    37,    66,     8,   217,   198,
     215,  1360,   198,   198,   193,  1350,   221,  1226,   197,   215,
     124,   200,   184,   127,   128,  1360,   191,  1708,  1709,  1378,
      13,  1707,   216,   137,   191,  1244,   215,   217,   191,  1180,
     218,  1250,   221,  1378,   173,   173,  1370,   132,  1257,   199,
    1259,   173,    33,   191,  1199,  1200,   217,    33,    43,   191,
    1205,   165,  1743,    14,   173,   166,   192,   173,   220,   194,
    1460,    21,    22,    67,   173,   218,   191,   191,   218,    60,
      61,   185,   184,  1292,    60,    61,   217,   217,   192,   193,
       1,   198,   173,   197,  1667,  1304,   200,   218,   217,  1667,
    1309,   217,   198,   173,  1494,   217,   217,   798,   199,   199,
    1500,   215,   217,   199,  1463,   218,   192,   221,   173,   217,
     217,   173,   217,   192,   218,  1270,  1461,   173,  1463,   218,
     218,    33,   173,   217,   217,  1708,  1709,   173,   218,   218,
    1708,  1709,   216,   124,   216,   218,   173,   128,   124,  1358,
     216,   198,   128,  1461,   217,   217,   217,   848,    60,    61,
     217,   217,   173,   173,   217,    43,   116,   117,   118,   119,
    1743,    33,   863,   218,   173,  1743,   126,   217,   128,   129,
     130,   131,   217,   216,   165,   181,   136,   218,   138,   139,
     216,   173,   216,   173,   885,   173,   217,   217,  1588,   173,
     217,   199,   218,   217,   185,  1350,  1415,    70,   217,   217,
     191,  1352,   193,   217,   217,  1360,   197,   193,   199,   200,
     199,   197,   124,   199,   200,   218,   128,  1576,   199,  1370,
     217,  1440,   185,  1378,   215,   177,    53,   184,   184,   215,
     221,  1576,    21,    22,   217,   221,   218,   217,   216,   218,
     200,   201,   202,   203,   204,   218,   218,   948,   184,   216,
    1595,   191,   218,   218,   214,   215,   218,   184,   732,   218,
     218,   398,   217,  1663,   218,   216,   216,    33,   216,   218,
     407,   218,    84,     1,    46,   139,    87,  1595,  1589,   212,
     417,   193,  1590,  1590,   680,   197,  1131,   199,   200,  1590,
     427,  1590,  1590,     1,    60,    61,  1134,  1588,  1458,  1279,
    1381,  1511,  1384,   215,  1183,  1512,  1461,  1512,  1463,   221,
      56,   462,   449,  1713,  1683,   292,   450,   462,  1542,  1664,
    1240,    -1,   725,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   480,   481,    -1,  1664,   136,    -1,   138,
     139,  1502,    33,    -1,   491,    -1,  1057,  1508,   124,    -1,
      -1,    -1,   128,    -1,    -1,   502,   503,   504,   505,   506,
     507,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,
      61,    -1,   888,   889,   890,   891,   892,   893,   894,   895,
     896,   897,    -1,   899,   900,   901,   902,   903,   904,  1618,
      -1,    -1,    -1,    -1,    -1,    -1,   543,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,
      -1,  1576,    -1,    -1,  1125,   214,   215,   193,    -1,    -1,
      -1,   197,    -1,   199,   200,   572,  1655,  1138,  1139,  1140,
    1595,    -1,  1143,   124,  1145,    -1,  1147,   128,  1149,   215,
    1151,    -1,  1153,    -1,  1155,   221,  1157,    -1,  1159,    -1,
    1161,    -1,  1163,    -1,    -1,  1166,    -1,  1168,    -1,  1170,
      -1,  1172,    -1,  1174,    -1,  1176,   613,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1703,  1640,   623,    -1,    -1,    -1,
     627,    -1,    -1,   630,    -1,   632,    33,    -1,    -1,    -1,
     637,    -1,    -1,   640,    33,    -1,    -1,   644,    -1,  1664,
      -1,   648,   193,    -1,    -1,    -1,   197,  1668,   199,   200,
      -1,    -1,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    60,    61,    -1,   215,    -1,    -1,    -1,    -1,   676,
     221,  1696,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1711,    -1,    -1,   700,   701,    -1,    -1,   704,    -1,   706,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   716,
     717,   718,   719,   720,   721,    -1,   723,   124,   725,    -1,
      -1,   128,    21,    22,  1745,   124,  1747,    -1,    -1,   128,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1769,    -1,
      -1,   758,   759,    21,    22,   762,   763,   764,   765,    -1,
     767,    -1,   769,   770,   771,   772,   773,   774,   775,   776,
     777,   778,   779,   780,   781,   782,   783,   784,   785,   786,
     787,   788,    -1,   790,    33,    -1,   193,    -1,   795,    -1,
     197,    -1,   199,   200,   193,    -1,    -1,    -1,   197,    -1,
     199,   200,    -1,    -1,    -1,    -1,    -1,    -1,   215,    -1,
      -1,    60,    61,    -1,   221,    -1,   215,   116,   117,   118,
     119,   120,   221,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,   845,   138,
     139,    -1,    -1,   850,    -1,    -1,    33,    -1,   116,   117,
     118,   119,   120,    -1,   861,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    60,    61,   124,   144,   145,   146,   128,
     887,   888,   889,   890,   891,   892,   893,   894,   895,   896,
     897,   898,   899,   900,   901,   902,   903,   904,   197,   198,
     199,   200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,
     917,    -1,   919,    -1,    -1,   214,   215,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,  1497,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   124,    -1,    -1,
      -1,   128,   949,   950,   193,    -1,   214,   215,   197,    -1,
     199,   200,    -1,    -1,    -1,    -1,   963,    -1,   965,    -1,
      -1,    -1,    -1,    -1,   971,    -1,   215,    -1,   975,    -1,
      -1,    -1,   221,   980,    -1,   982,    -1,   984,    -1,   986,
     108,   109,   110,   111,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,    -1,   137,
     197,    -1,   199,   200,    -1,    -1,    -1,    -1,    33,   147,
     148,   149,    -1,    -1,    -1,    -1,    -1,    -1,   215,    -1,
      -1,    -1,    -1,    -1,   221,    -1,    -1,    -1,  1045,    -1,
      -1,    33,  1049,    -1,    -1,    60,    61,    -1,    -1,    -1,
      -1,  1058,  1059,  1060,  1061,  1062,  1063,  1064,  1065,  1066,
    1067,  1068,  1069,  1070,  1071,  1072,  1073,  1074,    60,    61,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,  1099,    -1,    -1,    25,    -1,    -1,  1105,    -1,
      -1,    31,    -1,  1110,    -1,    -1,    -1,    -1,  1115,   124,
      -1,    41,    -1,   128,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    -1,    -1,  1130,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    64,    -1,   128,    -1,    -1,    -1,
    1711,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,  1745,    -1,  1747,    -1,   193,    -1,
      -1,    -1,   197,    -1,   199,   200,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1769,    -1,
     215,   193,    -1,    -1,    -1,   197,   221,   199,   200,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   215,  1231,    -1,   156,    -1,    -1,   221,
      -1,    -1,  1239,  1240,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   173,    -1,     5,     6,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    15,    16,    17,    18,    19,
      -1,  1268,  1269,    -1,    -1,    25,    -1,    27,    -1,  1276,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,  1289,    -1,  1291,    45,  1293,    -1,    48,   219,
      -1,    51,  1299,    -1,    -1,    55,  1303,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1361,    -1,    -1,    -1,   118,   119,
    1367,    -1,    -1,  1370,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      33,   151,   152,   153,   154,   155,  1403,   157,    -1,   159,
     160,   161,   162,    -1,    -1,    -1,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,    60,    61,    -1,
      -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1442,  1443,  1444,    -1,    -1,
     200,   201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,
      -1,    -1,    -1,    -1,    -1,   215,    -1,   217,    -1,   219,
     220,    -1,  1469,    -1,  1471,    -1,    -1,    -1,    -1,    -1,
    1477,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,   128,  1493,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1517,    -1,    -1,  1520,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1528,  1529,  1530,    -1,    -1,    -1,    -1,  1535,    -1,
      -1,  1538,    -1,    -1,  1541,    -1,    -1,  1544,  1545,    -1,
      -1,    -1,  1549,  1550,    -1,  1552,  1553,    -1,    -1,    -1,
     193,    -1,    -1,    -1,   197,    -1,   199,   200,    -1,    -1,
    1567,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,   221,    -1,
      -1,    -1,    -1,  1590,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,  1614,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,  1636,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1644,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1656,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1667,    -1,    -1,    -1,    -1,    -1,  1673,  1674,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      -1,    -1,    -1,    -1,    -1,    -1,  1693,  1694,    -1,   214,
     215,    -1,    -1,    -1,    -1,    -1,    -1,  1704,    -1,    -1,
      -1,  1708,  1709,     1,    -1,    -1,    -1,     5,     6,     7,
      -1,     9,    10,    11,    -1,    13,    -1,    15,    16,    17,
      18,    19,    -1,    -1,  1731,    -1,    -1,    25,    26,    27,
      28,    29,    -1,    31,    -1,    -1,  1743,    -1,    -1,    -1,
      38,    39,    40,    -1,    42,    -1,    44,    45,    -1,    -1,
      48,    -1,    50,    51,    52,    -1,    54,    55,    -1,    -1,
      58,    59,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,   157,
      -1,   159,   160,   161,   162,    -1,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,    -1,
      -1,    -1,    -1,    -1,    -1,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,
     208,    -1,    -1,     5,     6,    -1,    -1,   215,    -1,   217,
      -1,   219,   220,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    60,    61,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,    -1,    -1,    -1,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,    -1,    -1,   197,    -1,    -1,   200,   201,
     202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,    -1,
       5,     6,    -1,   215,    -1,   217,    -1,   219,   220,   221,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    60,    61,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,    -1,    -1,
      -1,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
      -1,    -1,   197,    -1,    -1,   200,   201,   202,    -1,   204,
      -1,    -1,   207,   208,    -1,    -1,    -1,     5,     6,    -1,
     215,    -1,   217,    -1,   219,   220,   221,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    33,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,   157,
      -1,   159,   160,   161,   162,    -1,    -1,    -1,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,
      -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,
     208,    -1,    -1,    -1,     5,     6,    -1,   215,    -1,   217,
      -1,   219,   220,   221,    15,    16,    17,    18,    19,    -1,
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
      25,    26,    27,    28,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    51,    52,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    65,    -1,    -1,    68,    69,    70,    71,    72,
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
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,
     157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,
     207,   208,    -1,    -1,    -1,    -1,     5,     6,   215,    -1,
     217,    -1,   219,   220,    13,    -1,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,     5,     6,   215,   216,   217,    -1,
     219,   220,    13,    -1,    15,    16,    17,    18,    19,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,    -1,    -1,    -1,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,
     201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,
      -1,    -1,     5,     6,   215,    -1,   217,    -1,   219,   220,
      13,    -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
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
      -1,   204,    -1,    -1,   207,   208,    -1,    -1,    -1,    -1,
       5,     6,   215,   216,   217,    -1,   219,   220,    13,    -1,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    -1,    -1,    -1,
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
     215,    -1,   217,    -1,   219,   220,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    70,    71,    72,    73,    -1,    75,    76,    77,    78,
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
     157,   158,   159,   160,   161,   162,    -1,    -1,    -1,   166,
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
      -1,    -1,     5,     6,   215,   216,   217,    -1,   219,   220,
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
      -1,   204,    -1,    -1,   207,   208,    -1,    -1,    -1,    -1,
       5,     6,   215,    -1,   217,    -1,   219,   220,    13,    -1,
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
      -1,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
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
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    61,    -1,
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
      27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,   200,
     201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,
       5,     6,    -1,    -1,   215,    -1,   217,    -1,   219,   220,
      15,    16,    17,    18,    19,    -1,    -1,    22,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,   198,    -1,   200,   201,   202,
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
      -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,
     155,    -1,   157,    -1,   159,   160,   161,   162,    -1,    -1,
      -1,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   200,   201,   202,    -1,   204,
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
      10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,   154,   155,    -1,   157,    -1,   159,   160,   161,   162,
      -1,    -1,    -1,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,    -1,    -1,    -1,    -1,    21,    22,
     183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,
      -1,   204,    -1,    -1,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,   215,    -1,   217,    -1,   219,   220,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
     140,   141,    -1,    -1,   144,   145,   146,   147,   148,   149,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   192,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    13,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,   214,   215,    -1,    25,    -1,   219,
     220,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,   214,   215,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,   156,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,   173,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   219,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,
      -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,   173,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   219,    -1,   221,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    19,    -1,    -1,    -1,
      -1,   173,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,   185,    -1,    -1,    -1,    -1,    41,   191,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,   219,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    22,    64,    -1,    -1,    -1,    -1,    -1,
     173,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,    -1,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   219,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,   173,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,    -1,   146,    -1,    -1,
      -1,    -1,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,    -1,    -1,    -1,   219,
     136,   137,   138,   139,   140,   141,    21,    22,   144,   145,
     146,   147,   148,   149,   150,    -1,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    -1,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,   185,
      -1,    -1,    -1,    -1,    -1,    -1,   192,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,    -1,   219,   220,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,    -1,    -1,
      -1,   136,   137,   138,   139,   140,   141,    -1,    -1,   144,
     145,   146,   147,   148,   149,   150,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    21,    22,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,   192,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,    -1,   219,   220,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,    -1,   219,
     220,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    21,    22,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    21,    22,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     214,   215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    21,    22,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    21,    22,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    21,    22,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     214,   215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    21,    22,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    21,    22,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    21,    22,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     214,   215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    21,    22,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    21,    22,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    21,    22,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     214,   215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    21,    22,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,   215,   216,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,   216,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    21,    22,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,   216,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    21,    22,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    38,   150,    -1,    -1,    -1,    -1,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,
     214,   215,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    19,    -1,   116,
     117,   118,   119,   120,   214,   215,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,    -1,    -1,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
      -1,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,   214,   215,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,
      73,   173,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,   158,    35,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,   129,   130,
      -1,    -1,    71,    -1,    73,    -1,    75,    76,    77,    78,
      79,   173,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,   158,    -1,    98,
      99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   173
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   223,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   102,   103,   165,   185,   204,   215,   224,   228,   237,
     239,   240,   245,   252,   277,   282,   318,   400,   407,   411,
     422,   468,   473,   478,    19,    20,   173,   269,   270,   271,
     166,   246,   247,   173,   174,   175,   204,   241,   242,   243,
     163,   183,   287,   408,   173,   219,   226,    57,    63,   403,
     403,   403,   143,   173,   304,    34,    63,   107,   136,   208,
     217,   273,   274,   275,   276,   304,   252,     5,     6,     8,
      36,   419,    62,   398,   192,   191,   194,   191,   242,    22,
      57,   203,   214,   244,   403,   404,   406,   404,   398,   479,
     469,   474,   173,   143,   238,   275,   275,   275,   217,   144,
     145,   146,   191,   216,   107,   107,   107,   281,    57,    63,
     409,    57,    63,   420,    57,    63,   399,    15,    16,   166,
     171,   173,   176,   217,   220,   230,   270,   166,   247,   173,
     241,   241,   173,   252,   164,   184,   288,   404,   252,    57,
      63,   225,   173,   173,   173,   173,   177,   236,   218,   271,
     275,   275,   275,   275,    57,    63,   283,   285,   173,   410,
     423,   287,   401,   177,   178,   179,   229,    15,    16,   166,
     171,   173,   220,   230,   267,   268,   220,   244,   405,   252,
     208,   227,   480,   470,   475,   177,   218,   286,   194,   287,
     106,   417,   418,   396,   160,   220,   272,   368,   177,   178,
     179,   220,   191,   218,   173,   192,    66,   194,   432,   287,
     287,    35,    71,    73,    75,    76,    77,    78,    79,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      93,    94,    95,    98,    99,   100,   101,   118,   119,   173,
     280,   284,    75,    79,    93,    94,    98,    99,   100,   101,
     427,   412,   173,   424,   165,   288,   397,   271,   270,   220,
     252,   152,   173,   394,   395,   173,   267,    19,    25,    31,
      41,    49,    64,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   156,   219,   304,   426,
     428,   429,   433,   439,   467,    79,    94,    99,   101,   287,
     471,   476,    21,    22,    38,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   128,   129,   130,   137,   138,   139,   140,
     141,   144,   145,   146,   147,   148,   149,   150,   193,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   207,
     208,   214,   215,    35,    35,   217,   278,   287,   289,   287,
     402,   194,   416,   287,   421,   368,   216,   270,   217,    43,
     191,   194,   197,   393,   198,   198,   198,   217,   198,   198,
     217,   432,   198,   198,   198,   198,   198,   217,   304,    33,
      60,    61,   124,   128,   193,   197,   200,   215,   221,   438,
     195,   481,   384,   387,   173,   173,   173,   216,    22,   173,
     216,   155,   218,   368,   379,   380,   381,   194,   279,   292,
     414,   173,   252,   413,   304,   374,   395,   216,     5,     6,
      15,    16,    17,    18,    19,    25,    27,    31,    39,    45,
      48,    51,    55,    65,    68,    69,    80,   102,   103,   104,
     118,   119,   151,   152,   153,   154,   155,   157,   159,   160,
     161,   162,   166,   167,   168,   169,   170,   171,   172,   174,
     175,   176,   183,   200,   201,   202,   207,   208,   215,   217,
     219,   220,   235,   237,   298,   304,   309,   323,   330,   333,
     336,   341,   344,   348,   349,   351,   356,   359,   360,   367,
     426,   483,   498,   509,   513,   526,   529,   173,   173,   439,
     127,   137,   192,   392,   440,   445,   447,   360,   449,   443,
     173,   198,   451,   453,   455,   457,   459,   461,   463,   465,
     360,   198,   217,   295,    33,   197,    33,   197,   215,   221,
     216,   360,   215,   221,   439,   431,   173,   191,   252,   382,
     436,   467,   472,   173,   385,   436,   477,   173,   108,   109,
     110,   111,   112,   113,   114,   115,   137,   147,   148,   149,
     108,   109,   110,   111,   112,   113,   114,   115,   127,   137,
     147,   148,   149,   217,     7,    50,   317,   252,   191,   252,
     218,   467,     1,     9,    10,    11,    13,    26,    28,    29,
      38,    40,    42,    44,    52,    54,    58,    59,    65,   104,
     105,   133,   134,   135,   174,   248,   249,   252,   253,   256,
     257,   259,   261,   262,   263,   264,   288,   290,   291,   293,
     298,   303,   305,   310,   311,   312,   313,   314,   315,   316,
     318,   322,   345,   347,   360,   402,   192,   252,   288,    40,
     215,   252,   277,   288,   376,   198,   198,   198,   306,   428,
     483,   217,   304,   198,     5,   102,   103,   198,   217,   198,
     217,   217,   198,   198,   217,   198,   217,   198,   217,   198,
     198,   217,   198,   198,   360,   360,   217,   217,   217,   217,
     217,   217,    13,   439,    13,   439,    13,   360,   508,   524,
     198,   198,   234,    13,   296,   508,   525,   360,   360,   360,
     360,   360,    13,    49,   294,   334,   360,   158,   173,   334,
     484,   486,   220,   173,   217,   277,    21,    22,   116,   117,
     118,   119,   120,   123,   124,   125,   126,   128,   129,   130,
     131,   136,   138,   139,   144,   145,   146,   150,   193,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   214,
     215,   218,   217,    43,   252,   392,   303,   345,   360,   467,
     467,   437,   467,   218,   467,   467,   218,   199,   434,   467,
     278,   467,   278,   467,   278,   382,   383,   385,   386,   218,
     442,   294,   216,   216,   360,   173,   252,   482,   194,   436,
     288,   194,   436,   288,   360,   152,   173,   389,   390,   425,
     381,   381,   381,   360,   260,   127,   303,   334,   360,   289,
      61,   360,   266,   360,   173,   252,   166,    58,   360,   289,
     198,   127,   303,   360,   220,   289,   336,   340,   340,   340,
     360,   252,   252,   360,    10,    37,   336,   342,   252,   252,
     252,   252,   252,    66,   319,   132,   252,   108,   109,   110,
     111,   112,   113,   114,   115,   121,   122,   127,   137,   140,
     141,   147,   148,   149,   192,   342,   415,   360,   375,   276,
       8,   368,   373,   499,   501,   307,   217,   304,   198,   217,
     331,   198,   198,   198,   520,   334,   439,   296,   360,   324,
     326,   360,   328,   360,   522,   334,   505,   510,   334,   503,
     439,   360,   360,   360,   360,   360,   360,   425,    53,   200,
     215,   217,   360,   484,   487,   491,   507,   512,   425,   217,
     487,   512,   425,   142,   184,   185,   186,   492,   299,   301,
     179,   180,   229,   425,   184,   191,   528,   425,    13,   216,
     191,   528,   217,   127,   137,   192,   388,   528,   191,   528,
     218,   152,   157,   198,   304,   350,    70,   215,   218,   334,
     486,     4,   160,   339,    19,   158,   173,   426,    19,   158,
     173,   426,   360,   360,   360,   360,   360,   360,   173,   360,
     158,   173,   360,   360,   360,   426,   360,   360,   360,   360,
     360,   360,    22,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   129,   130,   158,   173,   214,   215,
     357,   426,   360,   218,   334,   173,   303,   360,   108,   109,
     110,   111,   112,   113,   114,   115,   121,   122,   127,   140,
     141,   147,   148,   149,   192,   252,   199,   199,   199,   436,
     199,   199,   173,   430,   199,   279,   199,   279,   199,   279,
     199,   436,   199,   436,   297,   467,   191,   528,   216,   192,
     252,   288,   467,   467,   218,   217,    43,   191,   194,   197,
     388,   289,   425,   303,   334,   191,    14,   360,   173,   289,
     192,   194,   166,   439,   303,   360,   220,   277,   289,   289,
     258,   287,   343,   160,   217,   321,   395,   340,   133,   134,
     135,   290,   346,   360,   346,   360,   346,   360,   346,   360,
     346,   360,   346,   360,   346,   360,   346,   360,   346,   360,
     346,   360,   346,   360,   360,   346,   360,   346,   360,   346,
     360,   346,   360,   346,   360,   346,   360,   288,   252,   173,
     216,    57,    63,   371,    67,   372,   252,   439,   439,   467,
      70,   334,   486,   497,   198,   360,   173,   360,   467,   514,
     516,   518,   439,   528,   199,   436,   218,   218,   439,   439,
     218,   439,   218,   439,   528,   439,   383,   528,   386,   199,
     218,   218,   218,   218,   218,   218,    20,   340,   215,   360,
     218,   142,   191,   185,   491,   188,   189,   216,   495,   191,
     185,   188,   216,   494,    20,   218,   491,   184,   187,   493,
      20,   360,   184,   508,   297,   297,   360,    20,   508,    20,
     425,   360,   360,   360,   360,   218,   158,   173,   217,   217,
     352,   354,   173,   218,   486,   484,   191,   218,   218,   217,
     127,   137,   173,   192,   197,   337,   338,   278,   198,   217,
     198,   217,   217,   217,   216,    19,   158,   173,   426,   194,
     158,   173,   360,   217,   217,   158,   173,   360,     1,   217,
     216,   218,   252,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   360,   360,   360,   360,   360,
     441,   446,   448,   467,   450,   444,   191,   199,   252,   452,
     199,   456,   199,   460,   199,   464,   382,   466,   385,   199,
     436,   218,   360,   360,   173,   173,   467,   360,    20,   289,
     192,   265,   199,   339,    12,    23,    24,   250,   251,   360,
     292,   277,   173,   320,   320,   340,   340,   340,   192,   252,
      47,   372,    46,   106,   369,   199,   199,   199,   486,   218,
     218,   218,   173,   218,   185,   199,   218,   199,   439,   383,
     386,   199,   218,   217,   439,   199,   199,   199,   199,   218,
     199,   199,   218,   199,   339,   217,   334,   487,   491,   360,
     484,   495,   216,   360,   507,   216,   334,   487,   184,   187,
     190,   496,   216,   334,   199,   199,   194,   232,   334,   334,
      20,   218,   217,   137,   388,   360,   360,   439,   278,   218,
     216,   215,   338,   173,   173,   217,   173,   173,   191,   216,
     279,   361,   360,   363,   360,   218,   334,   360,   198,   217,
     360,   217,   216,   360,   215,   218,   334,   217,   216,   358,
     218,   334,   173,   435,   173,   454,   458,   462,   295,   467,
     252,   218,    43,   388,   334,   467,   360,   339,   278,   289,
     360,    12,   254,   288,   339,   191,   216,   218,   467,    33,
     370,   369,   371,   500,   502,   308,   218,   199,   436,   173,
     217,   332,   199,   199,   199,   521,   296,   199,   325,   327,
     329,   523,   506,   511,   504,   217,   218,   334,   185,   491,
     495,   185,   491,   216,   185,   300,   302,   233,   181,   185,
     185,   334,   137,   388,   360,   360,   360,   218,   218,   199,
     279,   218,   484,   218,   173,   337,   216,   142,   289,   335,
     439,   218,   467,   218,   218,   218,   365,   360,   360,   218,
     484,   218,   360,   218,   173,   360,   289,   342,   279,   289,
     255,   252,   278,   173,   216,   194,   393,   252,   377,   370,
     389,   390,   391,   217,   217,   360,   173,   199,   360,   515,
     517,   519,   217,   218,   217,   360,   360,   360,   217,    70,
     497,   217,   217,   218,   360,   218,   360,   495,   360,   496,
     508,   360,   295,   231,   508,   360,   185,   360,   360,   218,
     353,   199,   216,   218,   127,   360,   199,   199,   467,   218,
     218,   216,   218,   335,   251,    26,   105,   256,   310,   311,
     312,   314,   360,   279,   194,   393,   439,   392,   284,   378,
     497,   497,   218,   199,   217,   218,   217,   217,   217,   294,
     296,   334,   497,   497,   218,   185,   527,   527,   527,   177,
     527,   527,   360,   137,   388,   350,   355,   218,   360,   362,
     364,   199,   218,   127,   127,   360,   289,   439,   392,   392,
     303,   360,   252,   284,   217,   484,   488,   489,   490,   490,
     360,   360,   497,   497,   484,   485,   218,   218,   528,   490,
     485,    53,   216,   184,   184,   184,   216,   527,   360,   360,
     350,   366,   360,   392,   303,   360,   303,   360,   252,   289,
     484,   191,   528,   218,   218,   218,   218,   490,   490,   218,
     218,   218,   218,   360,   216,   216,   184,   216,   303,   360,
     252,   252,   218,   217,   218,   218,   252,   484,   218
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
     263,   265,   264,   266,   264,   267,   267,   267,   267,   267,
     267,   267,   268,   268,   269,   269,   269,   270,   270,   270,
     270,   270,   270,   270,   270,   270,   271,   271,   272,   272,
     272,   273,   273,   273,   273,   274,   274,   275,   275,   275,
     275,   275,   275,   275,   276,   276,   277,   277,   278,   278,
     278,   279,   279,   280,   280,   280,   280,   280,   280,   280,
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
     280,   280,   281,   281,   282,   283,   283,   283,   284,   286,
     285,   287,   287,   288,   288,   289,   289,   290,   290,   290,
     291,   291,   291,   291,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   291,   291,   291,   291,
     291,   292,   292,   292,   293,   294,   294,   295,   295,   296,
     296,   297,   297,   299,   300,   298,   301,   302,   298,   303,
     303,   303,   303,   303,   304,   304,   304,   305,   305,   307,
     308,   306,   306,   309,   309,   309,   309,   309,   309,   310,
     311,   312,   312,   312,   313,   313,   313,   314,   314,   315,
     315,   315,   316,   317,   317,   317,   318,   318,   319,   319,
     320,   320,   321,   321,   321,   321,   321,   321,   321,   321,
     322,   322,   324,   325,   323,   326,   327,   323,   328,   329,
     323,   331,   332,   330,   333,   333,   333,   333,   333,   333,
     334,   334,   335,   335,   335,   336,   336,   336,   337,   337,
     337,   337,   337,   338,   338,   339,   339,   339,   340,   340,
     341,   343,   342,   344,   344,   344,   344,   344,   344,   344,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   346,
     346,   346,   346,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     348,   348,   349,   349,   350,   350,   351,   352,   353,   351,
     354,   355,   351,   356,   356,   356,   356,   356,   356,   356,
     357,   358,   356,   359,   359,   359,   359,   359,   359,   359,
     360,   360,   360,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   360,   360,   361,   362,   360,
     360,   360,   360,   363,   364,   360,   360,   360,   365,   366,
     360,   360,   360,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   368,   368,   368,   369,   369,   369,   370,   370,   371,
     371,   371,   372,   372,   373,   374,   374,   375,   374,   376,
     374,   377,   374,   378,   374,   374,   379,   380,   380,   381,
     381,   381,   381,   381,   382,   382,   383,   383,   384,   384,
     384,   385,   386,   386,   387,   387,   387,   388,   388,   389,
     389,   389,   390,   390,   391,   391,   392,   392,   392,   393,
     393,   394,   394,   394,   394,   394,   395,   395,   395,   395,
     395,   396,   396,   397,   396,   398,   398,   399,   399,   399,
     400,   401,   400,   402,   402,   402,   402,   403,   403,   403,
     405,   404,   406,   406,   407,   408,   407,   409,   409,   409,
     410,   412,   413,   411,   414,   415,   411,   416,   416,   417,
     417,   418,   419,   419,   419,   419,   420,   420,   420,   421,
     421,   423,   424,   422,   425,   425,   425,   425,   425,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   427,   427,   427,   427,
     427,   427,   427,   427,   428,   429,   429,   429,   430,   430,
     430,   431,   431,   431,   431,   432,   432,   432,   432,   432,
     433,   434,   435,   433,   436,   436,   437,   437,   438,   438,
     439,   439,   439,   439,   439,   439,   440,   441,   439,   439,
     439,   442,   439,   439,   439,   439,   439,   439,   439,   439,
     439,   439,   439,   439,   439,   443,   444,   439,   439,   445,
     446,   439,   447,   448,   439,   449,   450,   439,   439,   451,
     452,   439,   453,   454,   439,   439,   455,   456,   439,   457,
     458,   439,   439,   459,   460,   439,   461,   462,   439,   463,
     464,   439,   465,   466,   439,   467,   467,   467,   469,   470,
     471,   472,   468,   474,   475,   476,   477,   473,   479,   480,
     481,   482,   478,   483,   483,   483,   483,   483,   484,   484,
     484,   484,   484,   484,   484,   484,   485,   485,   486,   487,
     487,   488,   488,   489,   489,   490,   490,   491,   491,   492,
     492,   493,   493,   494,   494,   495,   495,   495,   496,   496,
     496,   497,   497,   498,   498,   498,   498,   498,   498,   499,
     500,   498,   501,   502,   498,   503,   504,   498,   505,   506,
     498,   507,   507,   507,   508,   508,   509,   510,   511,   509,
     512,   512,   513,   513,   513,   514,   515,   513,   516,   517,
     513,   518,   519,   513,   513,   520,   521,   513,   513,   522,
     523,   513,   524,   524,   525,   525,   526,   526,   526,   526,
     526,   527,   527,   528,   528,   529,   529,   529,   529,   529,
     529
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
       3,     0,     5,     0,     5,     1,     1,     3,     1,     1,
       1,     1,     1,     3,     1,     1,     1,     3,     3,     5,
       3,     3,     3,     3,     1,     5,     1,     3,     2,     3,
       2,     1,     1,     1,     1,     1,     4,     1,     2,     3,
       3,     3,     3,     2,     1,     3,     0,     3,     0,     2,
       3,     0,     2,     1,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     3,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     3,     2,     2,     3,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     3,     2,     2,
       2,     2,     2,     3,     3,     3,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     1,     4,     0,     1,     1,     3,     0,
       4,     1,     1,     1,     1,     3,     7,     2,     2,     6,
       1,     1,     1,     1,     2,     2,     1,     1,     1,     1,
       1,     1,     2,     2,     1,     1,     1,     1,     2,     2,
       2,     0,     2,     2,     3,     0,     2,     0,     4,     0,
       2,     1,     3,     0,     0,     7,     0,     0,     7,     3,
       2,     2,     2,     1,     1,     3,     2,     2,     3,     0,
       0,     5,     1,     2,     5,     5,     5,     6,     2,     1,
       1,     1,     2,     3,     2,     2,     3,     2,     3,     2,
       2,     3,     4,     1,     1,     0,     1,     1,     1,     0,
       1,     3,     9,     8,     8,     7,     8,     7,     7,     6,
       3,     3,     0,     0,     7,     0,     0,     7,     0,     0,
       7,     0,     0,     6,     5,     8,    10,     5,     8,    10,
       1,     3,     1,     2,     3,     1,     1,     2,     2,     2,
       2,     2,     4,     1,     3,     0,     4,     4,     1,     6,
       6,     0,     7,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     2,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       6,     8,     5,     6,     1,     4,     3,     0,     0,     8,
       0,     0,     9,     3,     4,     5,     6,     8,     5,     6,
       0,     0,     5,     3,     4,     4,     5,     4,     3,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     2,
       4,     3,     4,     5,     4,     5,     3,     4,     1,     1,
       2,     4,     4,     7,     8,     3,     5,     0,     0,     8,
       3,     3,     3,     0,     0,     8,     3,     4,     0,     0,
       9,     4,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     2,     4,     1,     4,     4,     4,     4,     4,
       1,     6,     7,     6,     6,     7,     7,     6,     7,     6,
       6,     0,     4,     1,     0,     1,     1,     0,     1,     0,
       1,     1,     0,     1,     5,     0,     2,     0,     7,     0,
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
       3,     0,     2,     3,     5,     0,     2,     2,     2,     2,
       4,     0,     0,     7,     1,     1,     1,     3,     3,     4,
       1,     1,     1,     1,     2,     3,     0,     0,     6,     4,
       3,     0,     7,     4,     2,     2,     3,     2,     3,     2,
       2,     3,     3,     3,     2,     0,     0,     6,     2,     0,
       0,     6,     0,     0,     6,     0,     0,     6,     1,     0,
       0,     6,     0,     0,     7,     1,     0,     0,     6,     0,
       0,     7,     1,     0,     0,     6,     0,     0,     7,     0,
       0,     6,     0,     0,     6,     1,     3,     3,     0,     0,
       0,     0,    10,     0,     0,     0,     0,    10,     0,     0,
       0,     0,    11,     1,     1,     1,     1,     1,     3,     3,
       5,     5,     6,     6,     8,     8,     0,     1,     2,     1,
       3,     3,     5,     1,     2,     1,     0,     0,     2,     2,
       1,     2,     1,     2,     1,     2,     1,     1,     2,     1,
       1,     0,     1,     5,     4,     6,     7,     5,     7,     0,
       0,    10,     0,     0,    10,     0,     0,    10,     0,     0,
       7,     1,     3,     3,     3,     1,     5,     0,     0,    10,
       1,     3,     3,     4,     4,     0,     0,    11,     0,     0,
      11,     0,     0,    10,     5,     0,     0,     9,     5,     0,
       0,    10,     1,     3,     1,     3,     3,     3,     4,     7,
       9,     0,     3,     0,     1,     9,    10,    10,    10,     9,
      10
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

  case 93: /* $@7: %empty  */
                         { yyextra->das_force_oxford_comma=true;}
    break;

  case 94: /* expression_with_alias: "typedef" $@7 "name" '=' type_declaration  */
                                                                                                         {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-4])), *(yyvsp[-2].s), TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
    break;

  case 95: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 96: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 97: /* annotation_argument_value: '@' '@' "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 98: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 99: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 100: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 101: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 102: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 103: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 104: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 105: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 106: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 107: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 108: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 109: /* annotation_argument: annotation_argument_name '=' '@' '@' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[0].s); delete (yyvsp[-4].s); }
    break;

  case 110: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 111: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 112: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 113: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 114: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 115: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 116: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 117: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 118: /* metadata_argument_list: '@' annotation_argument  */
                                      {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 119: /* metadata_argument_list: metadata_argument_list '@' annotation_argument  */
                                                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 120: /* metadata_argument_list: metadata_argument_list semicolon  */
                                               {
        (yyval.aaList) = (yyvsp[-1].aaList);
    }
    break;

  case 121: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 122: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 123: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 124: /* annotation_declaration_name: "template"  */
                                    { (yyval.s) = new string("template"); }
    break;

  case 125: /* annotation_declaration_basic: annotation_declaration_name  */
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

  case 126: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 127: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 128: /* annotation_declaration: '!' annotation_declaration  */
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

  case 129: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 130: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 131: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 132: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 133: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 134: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 135: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 136: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 137: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 138: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 139: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 140: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 141: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 142: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 143: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 144: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 145: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 146: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 147: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 148: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 149: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 150: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 151: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 152: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 153: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 154: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 155: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 156: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 157: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 158: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 159: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 160: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 161: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 162: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 163: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 164: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 165: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 166: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 167: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 168: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 169: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 170: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 171: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 172: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 173: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 174: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 175: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 176: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 177: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 178: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 179: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 180: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 181: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 182: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 183: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 184: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 185: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 186: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 187: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 188: /* function_name: "operator" '[' ']' "<-"  */
                                    { (yyval.s) = new string("[]<-"); }
    break;

  case 189: /* function_name: "operator" '[' ']' ":="  */
                                      { (yyval.s) = new string("[]:="); }
    break;

  case 190: /* function_name: "operator" '[' ']' "+="  */
                                     { (yyval.s) = new string("[]+="); }
    break;

  case 191: /* function_name: "operator" '[' ']' "-="  */
                                     { (yyval.s) = new string("[]-="); }
    break;

  case 192: /* function_name: "operator" '[' ']' "*="  */
                                     { (yyval.s) = new string("[]*="); }
    break;

  case 193: /* function_name: "operator" '[' ']' "/="  */
                                     { (yyval.s) = new string("[]/="); }
    break;

  case 194: /* function_name: "operator" '[' ']' "%="  */
                                     { (yyval.s) = new string("[]%="); }
    break;

  case 195: /* function_name: "operator" '[' ']' "&="  */
                                     { (yyval.s) = new string("[]&="); }
    break;

  case 196: /* function_name: "operator" '[' ']' "|="  */
                                     { (yyval.s) = new string("[]|="); }
    break;

  case 197: /* function_name: "operator" '[' ']' "^="  */
                                     { (yyval.s) = new string("[]^="); }
    break;

  case 198: /* function_name: "operator" '[' ']' "&&="  */
                                        { (yyval.s) = new string("[]&&="); }
    break;

  case 199: /* function_name: "operator" '[' ']' "||="  */
                                        { (yyval.s) = new string("[]||="); }
    break;

  case 200: /* function_name: "operator" '[' ']' "^^="  */
                                        { (yyval.s) = new string("[]^^="); }
    break;

  case 201: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 202: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 203: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 204: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 205: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 206: /* function_name: "operator" '.' "name" "+="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`+="); delete (yyvsp[-1].s); }
    break;

  case 207: /* function_name: "operator" '.' "name" "-="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`-="); delete (yyvsp[-1].s); }
    break;

  case 208: /* function_name: "operator" '.' "name" "*="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`*="); delete (yyvsp[-1].s); }
    break;

  case 209: /* function_name: "operator" '.' "name" "/="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`/="); delete (yyvsp[-1].s); }
    break;

  case 210: /* function_name: "operator" '.' "name" "%="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`%="); delete (yyvsp[-1].s); }
    break;

  case 211: /* function_name: "operator" '.' "name" "&="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&="); delete (yyvsp[-1].s); }
    break;

  case 212: /* function_name: "operator" '.' "name" "|="  */
                                          { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`|="); delete (yyvsp[-1].s); }
    break;

  case 213: /* function_name: "operator" '.' "name" "^="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^="); delete (yyvsp[-1].s); }
    break;

  case 214: /* function_name: "operator" '.' "name" "&&="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&&="); delete (yyvsp[-1].s); }
    break;

  case 215: /* function_name: "operator" '.' "name" "||="  */
                                            { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`||="); delete (yyvsp[-1].s); }
    break;

  case 216: /* function_name: "operator" '.' "name" "^^="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^^="); delete (yyvsp[-1].s); }
    break;

  case 217: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 218: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 219: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 220: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 221: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 222: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 223: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 224: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 225: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 226: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 227: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 228: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 229: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 230: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 231: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 232: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 233: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 234: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 235: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 236: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 237: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 238: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 239: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 240: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 241: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 242: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 243: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 244: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 245: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 246: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 247: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 248: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 249: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 250: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 251: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 252: /* optional_template: %empty  */
                                        { (yyval.b) = false; }
    break;

  case 253: /* optional_template: "template"  */
                                        { (yyval.b) = true; }
    break;

  case 254: /* global_function_declaration: optional_annotation_list "def" optional_template function_declaration  */
                                                                                                              {
        (yyvsp[0].pFuncDecl)->atDecl = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
        (yyvsp[0].pFuncDecl)->isTemplate = (yyvsp[-1].b);
        assignDefaultArguments((yyvsp[0].pFuncDecl));
        runFunctionAnnotations(scanner, yyextra, (yyvsp[0].pFuncDecl), (yyvsp[-3].faList), tokAt(scanner,(yylsp[-3])));
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

  case 255: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 256: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 257: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 258: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 259: /* $@8: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 260: /* function_declaration: optional_public_or_private_function $@8 function_declaration_header expression_block  */
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

  case 265: /* expression_block: open_block expressions close_block  */
                                                                  {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 266: /* expression_block: open_block expressions close_block "finally" open_block expressions close_block  */
                                                                                                                        {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 267: /* expr_call_pipe: expr expr_full_block_assumed_piped  */
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

  case 268: /* expr_call_pipe: expression_keyword expr_full_block_assumed_piped  */
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

  case 269: /* expr_call_pipe: "generator" '<' type_declaration_no_options '>' optional_capture_list expr_full_block_assumed_piped  */
                                                                                                                                             {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-3].pTypeDecl),(yyvsp[-1].pCaptList),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 270: /* expression_any: semicolon  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 271: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 272: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 273: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 274: /* expression_any: expr_assign semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 275: /* expression_any: expression_delete semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 276: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 277: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 278: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 279: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 280: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 281: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 282: /* expression_any: expression_break semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 283: /* expression_any: expression_continue semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 284: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 285: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 286: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 287: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 288: /* expression_any: expression_label semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 289: /* expression_any: expression_goto semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 290: /* expression_any: "pass" semicolon  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 291: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 292: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back((yyvsp[0].pExpression));
        }
    }
    break;

  case 293: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 294: /* expr_keyword: "keyword" expr expression_block  */
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

  case 295: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 296: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 297: /* optional_expr_list_in_braces: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 298: /* optional_expr_list_in_braces: '(' optional_expr_list optional_comma ')'  */
                                                             { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 299: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 300: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 301: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 302: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 303: /* $@9: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 304: /* $@10: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 305: /* expression_keyword: "keyword" '<' $@9 type_declaration_no_options_list '>' $@10 expr  */
                                                                                                                                                     {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 306: /* $@11: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 307: /* $@12: %empty  */
                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 308: /* expression_keyword: "type function" '<' $@11 type_declaration_no_options_list '>' $@12 optional_expr_list_in_braces  */
                                                                                                                                                                                   {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 309: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 310: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 311: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 312: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 313: /* expr_pipe: expr_call_pipe  */
                             {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 314: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 315: /* name_in_namespace: "name" "::" "name"  */
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

  case 316: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 317: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
    }
    break;

  case 318: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 319: /* $@13: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 320: /* $@14: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 321: /* new_type_declaration: '<' $@13 type_declaration '>' $@14  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 322: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 323: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pTypeDecl),false);
    }
    break;

  case 324: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 325: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 326: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),(yyvsp[-1].pExpression));
    }
    break;

  case 327: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),(yyvsp[-1].pExpression));
    }
    break;

  case 328: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 329: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 330: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 331: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 332: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 333: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 334: /* expression_return: expression_return_no_pipe semicolon  */
                                                    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 335: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 336: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 337: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 338: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 339: /* expression_yield: expression_yield_no_pipe semicolon  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 340: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 341: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 342: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 343: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 344: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 345: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 346: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 347: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 348: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 349: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 350: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 351: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 352: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-7].pNameList),tokAt(scanner,(yylsp[-7])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 353: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 354: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 355: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                           {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameList),tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 356: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr semicolon  */
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

  case 357: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr_pipe  */
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

  case 358: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr semicolon  */
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

  case 359: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr_pipe  */
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

  case 360: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 361: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 362: /* $@15: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 363: /* $@16: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 364: /* expr_cast: "cast" '<' $@15 type_declaration_no_options '>' $@16 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
    }
    break;

  case 365: /* $@17: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 366: /* $@18: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 367: /* expr_cast: "upcast" '<' $@17 type_declaration_no_options '>' $@18 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 368: /* $@19: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 369: /* $@20: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 370: /* expr_cast: "reinterpret" '<' $@19 type_declaration_no_options '>' $@20 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 371: /* $@21: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 372: /* $@22: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 373: /* expr_type_decl: "type" '<' $@21 type_declaration '>' $@22  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 374: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 375: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 376: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" c_or_s "name" '>' expr ')'  */
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

  case 377: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
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

  case 378: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
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

  case 379: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" "end of expression" "name" '>' '(' expr ')'  */
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

  case 380: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 381: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 382: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 383: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 384: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 385: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 386: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 387: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 388: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 389: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 390: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 391: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 392: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 393: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 394: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 395: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 396: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 397: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 398: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),(yyvsp[0].pExpression));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 399: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 400: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 401: /* $@23: %empty  */
                             {  yyextra->das_need_oxford_comma = false; }
    break;

  case 402: /* expr_full_block_assumed_piped: block_or_lambda $@23 optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 403: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 404: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 405: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 406: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 407: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 408: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 409: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 410: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 411: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 412: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 413: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 414: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 415: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 416: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 417: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 418: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 419: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 420: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 421: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 422: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 423: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 424: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 425: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 426: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 427: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 428: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 429: /* expr_assign_pipe_right: "@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 430: /* expr_assign_pipe_right: "@@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 431: /* expr_assign_pipe_right: "$ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 432: /* expr_assign_pipe_right: expr_call_pipe  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 433: /* expr_assign_pipe: expr '=' expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 434: /* expr_assign_pipe: expr "<-" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 435: /* expr_assign_pipe: expr "&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 436: /* expr_assign_pipe: expr "|=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 437: /* expr_assign_pipe: expr "^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 438: /* expr_assign_pipe: expr "&&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 439: /* expr_assign_pipe: expr "||=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 440: /* expr_assign_pipe: expr "^^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 441: /* expr_assign_pipe: expr "+=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 442: /* expr_assign_pipe: expr "-=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 443: /* expr_assign_pipe: expr "*=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 444: /* expr_assign_pipe: expr "/=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 445: /* expr_assign_pipe: expr "%=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 446: /* expr_assign_pipe: expr "<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 447: /* expr_assign_pipe: expr ">>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 448: /* expr_assign_pipe: expr "<<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 449: /* expr_assign_pipe: expr ">>>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 450: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 451: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 452: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 453: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 454: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 455: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 456: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 457: /* $@24: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 458: /* $@25: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 459: /* func_addr_expr: '@' '@' '<' $@24 type_declaration_no_options '>' $@25 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = (yyvsp[-3].pTypeDecl);
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 460: /* $@26: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 461: /* $@27: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 462: /* func_addr_expr: '@' '@' '<' $@26 optional_function_argument_list optional_function_type '>' $@27 func_addr_name  */
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

  case 463: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 464: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 465: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 466: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 467: /* expr_field: expr '.' "name" '(' '[' make_struct_fields ']' ')'  */
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

  case 468: /* expr_field: expr '.' basic_type_declaration '(' ')'  */
                                                                        {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), method_name);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 469: /* expr_field: expr '.' basic_type_declaration '(' expr_list ')'  */
                                                                                             {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), method_name);
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 470: /* $@28: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 471: /* $@29: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 472: /* expr_field: expr '.' $@28 error $@29  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), "");
        yyerrok;
    }
    break;

  case 473: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 474: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
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

  case 475: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
                                                               {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-3].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 476: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
                                                                                 {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-4])),*(yyvsp[-4].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-4].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 477: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 478: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 479: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 480: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 481: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 482: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 483: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 484: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 485: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 486: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 487: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 488: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 489: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 490: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",(yyvsp[0].pExpression)); }
    break;

  case 491: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",(yyvsp[0].pExpression)); }
    break;

  case 492: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",(yyvsp[0].pExpression)); }
    break;

  case 493: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",(yyvsp[0].pExpression)); }
    break;

  case 494: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 495: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 496: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 497: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 498: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 499: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 500: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 501: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 502: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 503: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 504: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 505: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 506: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 507: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 508: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 509: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 510: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 511: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 512: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 513: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 514: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 515: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back((yyvsp[-2].pExpression));
        itv->arguments.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = itv;
    }
    break;

  case 516: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", (yyvsp[0].pExpression)); }
    break;

  case 517: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", (yyvsp[0].pExpression)); }
    break;

  case 518: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", (yyvsp[-1].pExpression)); }
    break;

  case 519: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", (yyvsp[-1].pExpression)); }
    break;

  case 520: /* expr: '(' expr_list optional_comma ')'  */
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

  case 521: /* expr: '(' make_struct_single ')'  */
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

  case 522: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 523: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 524: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 525: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 526: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 527: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 528: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 529: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 530: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression)); }
    break;

  case 531: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 532: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 533: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 534: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 535: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 536: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
        }
    break;

  case 537: /* $@30: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 538: /* $@31: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 539: /* expr: expr "is" "type" '<' $@30 type_declaration_no_options '>' $@31  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 540: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),vdecl);
    }
    break;

  case 541: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 542: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 543: /* $@32: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 544: /* $@33: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 545: /* expr: expr "as" "type" '<' $@32 type_declaration '>' $@33  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 546: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 547: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 548: /* $@34: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 549: /* $@35: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 550: /* expr: expr '?' "as" "type" '<' $@34 type_declaration '>' $@35  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-8].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 551: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 552: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 553: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 554: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 555: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 556: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 557: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 558: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 559: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 560: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 561: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 562: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 563: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 564: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 565: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 566: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 567: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 568: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 569: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 570: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 571: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 572: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 573: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 574: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 575: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 576: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 577: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 578: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-6].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 579: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 580: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 581: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 582: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); /*this one is gone when BRABRA is disabled*/ }
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

  case 596: /* struct_variable_declaration_list: struct_variable_declaration_list semicolon  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 597: /* $@36: %empty  */
                                                               { yyextra->das_force_oxford_comma=true;}
    break;

  case 598: /* struct_variable_declaration_list: struct_variable_declaration_list "typedef" $@36 "name" '=' type_declaration semicolon  */
                                                                                                                                                         {
        (yyval.pVarDeclList) = (yyvsp[-6].pVarDeclList);
        ast_structureAlias(scanner,(yyvsp[-3].s),(yyvsp[-1].pTypeDecl),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 599: /* $@37: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 600: /* struct_variable_declaration_list: struct_variable_declaration_list $@37 structure_variable_declaration semicolon  */
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

  case 601: /* $@38: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 602: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@38 function_declaration_header semicolon  */
                                                          {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 603: /* $@39: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 604: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@39 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 605: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' semicolon  */
                                                                                       {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 606: /* function_argument_declaration_no_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_no_type  */
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

  case 607: /* function_argument_declaration_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_type  */
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

  case 608: /* function_argument_declaration_type: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))));
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 609: /* function_argument_list: function_argument_declaration_no_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 610: /* function_argument_list: function_argument_declaration_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 611: /* function_argument_list: function_argument_declaration_no_type semicolon function_argument_list  */
                                                                                            { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 612: /* function_argument_list: function_argument_declaration_type semicolon function_argument_list  */
                                                                                            { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 613: /* function_argument_list: function_argument_declaration_type ',' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 614: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 615: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 616: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 617: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                          { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 618: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 619: /* tuple_alias_type_list: tuple_alias_type_list c_or_s  */
                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 620: /* tuple_alias_type_list: tuple_alias_type_list tuple_type c_or_s  */
                                                            {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
        /*
        if ( !yyextra->g_CommentReaders.empty() ) {
            for ( auto & crd : yyextra->g_CommentReaders ) {
                for ( const auto & nl : *($decl->pNameList) ) {
                    crd->afterVariantEntry(nl.name.c_str(), nl.at);
                }
            }
        }
        */
    }
    break;

  case 621: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 622: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 623: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 624: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 625: /* variant_alias_type_list: variant_alias_type_list c_or_s  */
                                           {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 626: /* variant_alias_type_list: variant_alias_type_list variant_type c_or_s  */
                                                                {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
        if ( !yyextra->g_CommentReaders.empty() ) {
            for ( auto & crd : yyextra->g_CommentReaders ) {
                for ( const auto & nl : *((yyvsp[-1].pVarDecl)->pNameList) ) {
                    crd->afterVariantEntry(nl.name.c_str(), nl.at);
                }
            }
        }
    }
    break;

  case 627: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 628: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 629: /* variable_declaration_no_type: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 630: /* variable_declaration_no_type: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 631: /* variable_declaration_no_type: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 632: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 633: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 634: /* variable_declaration: variable_declaration_type  */
                                        {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 635: /* variable_declaration: variable_declaration_no_type  */
                                           {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 636: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 637: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 638: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 639: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 640: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 641: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 642: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 643: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 644: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 645: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 646: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options semicolon  */
                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 647: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 648: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 649: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr semicolon  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 650: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 651: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 652: /* global_variable_declaration_list: global_variable_declaration_list "end of line"  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 653: /* $@40: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 654: /* global_variable_declaration_list: global_variable_declaration_list $@40 optional_field_annotation let_variable_declaration  */
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

  case 660: /* global_let: kwd_let optional_shared optional_public_or_private_variable open_block global_variable_declaration_list close_block  */
                                                                                                                                                      {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 661: /* $@41: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 662: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@41 optional_field_annotation let_variable_declaration  */
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

  case 663: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 664: /* enum_list: enum_list semicolon  */
                                {
        (yyval.pEnum) = (yyvsp[-1].pEnum);
    }
    break;

  case 665: /* enum_list: enum_list "name" semicolon  */
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

  case 666: /* enum_list: enum_list "name" '=' expr semicolon  */
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

  case 667: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 668: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 669: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 670: /* $@42: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 671: /* single_alias: optional_public_or_private_alias "name" $@42 '=' type_declaration  */
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

  case 675: /* $@43: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 677: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 678: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 679: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 680: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 681: /* $@44: %empty  */
                                                                                                                       {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 682: /* $@45: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 683: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name open_block $@44 enum_list $@45 close_block  */
                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-2].pEnum),Type::tInt);
    }
    break;

  case 684: /* $@46: %empty  */
                                                                                                                                                            {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 685: /* $@47: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 686: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration open_block $@46 enum_list $@47 close_block  */
                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-2].pEnum),(yyvsp[-5].type));
    }
    break;

  case 687: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 688: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 689: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 690: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 691: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 692: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 693: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 694: /* class_or_struct: "class" "template"  */
                                 { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 695: /* class_or_struct: "struct" "template"  */
                                 { (yyval.i) = CorS_StructTemplate; }
    break;

  case 696: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 697: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 698: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 699: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 700: /* optional_struct_variable_declaration_list: open_block struct_variable_declaration_list close_block  */
                                                                      {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 701: /* $@48: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 702: /* $@49: %empty  */
                         {
        if ( (yyvsp[0].pStructure) ) {
            (yyvsp[0].pStructure)->isClass = (yyvsp[-3].i)==CorS_Class || (yyvsp[-3].i)==CorS_ClassTemplate;
            (yyvsp[0].pStructure)->isTemplate = (yyvsp[-3].i)==CorS_ClassTemplate || (yyvsp[-3].i)==CorS_StructTemplate;
            (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b);
        }
    }
    break;

  case 703: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@48 structure_name $@49 optional_struct_variable_declaration_list  */
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

  case 704: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 705: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 706: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 707: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 708: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 709: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 710: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 711: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 712: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 713: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 714: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 715: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 716: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 717: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 718: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 719: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 720: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 721: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 722: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 723: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 724: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 725: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 726: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 727: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 728: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 729: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 730: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 731: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 732: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 733: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 734: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 735: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 736: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 737: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 738: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 739: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 740: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 741: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 742: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 743: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 744: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 745: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 746: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 747: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 748: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 749: /* bitfield_bits: bitfield_bits semicolon "name"  */
                                                 {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 750: /* bitfield_bits: bitfield_bits ',' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 751: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<tuple<string,Expression *>>();
        (yyval.pNameExprList) = pSL;

    }
    break;

  case 752: /* bitfield_alias_bits: bitfield_alias_bits semicolon  */
                                            {
        (yyval.pNameExprList) = (yyvsp[-1].pNameExprList);
    }
    break;

  case 753: /* bitfield_alias_bits: bitfield_alias_bits "name" semicolon  */
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

  case 754: /* bitfield_alias_bits: bitfield_alias_bits "name" '=' expr semicolon  */
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

  case 755: /* bitfield_basic_type_declaration: %empty  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 756: /* bitfield_basic_type_declaration: ':' "uint8"  */
                             { (yyval.type) = Type::tBitfield8; }
    break;

  case 757: /* bitfield_basic_type_declaration: ':' "uint16"  */
                             { (yyval.type) = Type::tBitfield16; }
    break;

  case 758: /* bitfield_basic_type_declaration: ':' "uint"  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 759: /* bitfield_basic_type_declaration: ':' "uint64"  */
                             { (yyval.type) = Type::tBitfield64; }
    break;

  case 760: /* bitfield_type_declaration: "bitfield" bitfield_basic_type_declaration '<' '>'  */
                                                                          {
            (yyval.pTypeDecl) = new TypeDecl((yyvsp[-2].type));
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-2]));
    }
    break;

  case 761: /* $@50: %empty  */
                                                                     { yyextra->das_arrow_depth ++; }
    break;

  case 762: /* $@51: %empty  */
                                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 763: /* bitfield_type_declaration: "bitfield" bitfield_basic_type_declaration '<' $@50 bitfield_bits '>' $@51  */
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

  case 766: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 767: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 768: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 769: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 770: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 771: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 772: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 773: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 774: /* type_declaration_no_options: type_declaration_no_options dim_list  */
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

  case 775: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 776: /* $@52: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 777: /* $@53: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 778: /* type_declaration_no_options: "type" '<' $@52 type_declaration '>' $@53  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 779: /* type_declaration_no_options: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 780: /* type_declaration_no_options: '$' name_in_namespace optional_expr_list_in_braces  */
                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-1]), (yylsp[0]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[0].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-1])), *(yyvsp[-1].s)));
        delete (yyvsp[-1].s);
    }
    break;

  case 781: /* $@54: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 782: /* type_declaration_no_options: '$' name_in_namespace '<' $@54 type_declaration_no_options_list '>' optional_expr_list_in_braces  */
                                                                                                                                                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-5]), (yylsp[0]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-2].pTypeDeclList),(yyvsp[0].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-5])), *(yyvsp[-5].s)));
        delete (yyvsp[-5].s);
    }
    break;

  case 783: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 784: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 785: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 786: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 787: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 788: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 789: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 790: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 791: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 792: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 793: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 794: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 795: /* $@55: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 796: /* $@56: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 797: /* type_declaration_no_options: "smart_ptr" '<' $@55 type_declaration '>' $@56  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 798: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 799: /* $@57: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 800: /* $@58: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 801: /* type_declaration_no_options: "array" '<' $@57 type_declaration '>' $@58  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 802: /* $@59: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 803: /* $@60: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 804: /* type_declaration_no_options: "table" '<' $@59 table_type_pair '>' $@60  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].aTypePair).firstType;
        (yyval.pTypeDecl)->secondType = (yyvsp[-2].aTypePair).secondType;
    }
    break;

  case 805: /* $@61: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 806: /* $@62: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 807: /* type_declaration_no_options: "iterator" '<' $@61 type_declaration '>' $@62  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 808: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tVoid);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 809: /* $@63: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 810: /* $@64: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 811: /* type_declaration_no_options: "block" '<' $@63 type_declaration '>' $@64  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 812: /* $@65: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 813: /* $@66: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 814: /* type_declaration_no_options: "block" '<' $@65 optional_function_argument_list optional_function_type '>' $@66  */
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

  case 815: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tVoid);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 816: /* $@67: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 817: /* $@68: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 818: /* type_declaration_no_options: "function" '<' $@67 type_declaration '>' $@68  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 819: /* $@69: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 820: /* $@70: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 821: /* type_declaration_no_options: "function" '<' $@69 optional_function_argument_list optional_function_type '>' $@70  */
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

  case 822: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tVoid);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 823: /* $@71: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 824: /* $@72: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 825: /* type_declaration_no_options: "lambda" '<' $@71 type_declaration '>' $@72  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 826: /* $@73: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 827: /* $@74: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 828: /* type_declaration_no_options: "lambda" '<' $@73 optional_function_argument_list optional_function_type '>' $@74  */
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

  case 829: /* $@75: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 830: /* $@76: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 831: /* type_declaration_no_options: "tuple" '<' $@75 tuple_type_list '>' $@76  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 832: /* $@77: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 833: /* $@78: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 834: /* type_declaration_no_options: "variant" '<' $@77 variant_type_list '>' $@78  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 835: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 836: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 837: /* type_declaration: type_declaration '|' '#'  */
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

  case 838: /* $@79: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 839: /* $@80: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 840: /* $@81: %empty  */
                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 841: /* $@82: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 842: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@79 "name" $@80 open_block $@81 tuple_alias_type_list $@82 close_block  */
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

  case 843: /* $@83: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 844: /* $@84: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 845: /* $@85: %empty  */
                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 846: /* $@86: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 847: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@83 "name" $@84 open_block $@85 variant_alias_type_list $@86 close_block  */
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

  case 848: /* $@87: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 849: /* $@88: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 850: /* $@89: %empty  */
                                                            {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 851: /* $@90: %empty  */
                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-5]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 852: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@87 "name" $@88 bitfield_basic_type_declaration open_block $@89 bitfield_alias_bits $@90 close_block  */
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

  case 853: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 854: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 855: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 856: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 857: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 858: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 859: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 860: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 861: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 862: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 863: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 864: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 865: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 866: /* make_variant_dim: %empty  */
       {
        (yyval.pExpression) = ast_makeStructToMakeVariant(nullptr, LineInfo());
    }
    break;

  case 867: /* make_variant_dim: make_struct_fields  */
                              {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 868: /* make_struct_single: make_struct_fields optional_comma  */
                                               {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 869: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 870: /* make_struct_dim: make_struct_dim "end of expression" make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 871: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 872: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 873: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 874: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 875: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 876: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 877: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 878: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 891: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 892: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 893: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 894: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = (yyvsp[-2].pTypeDecl);
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 895: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = (yyvsp[-4].pTypeDecl);
        msd->useInitializer = true;
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = msd;
    }
    break;

  case 896: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-5].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 897: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back((yyvsp[-2].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 898: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 899: /* $@91: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 900: /* $@92: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 901: /* make_struct_decl: "struct" '<' $@91 type_declaration_no_options '>' $@92 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 902: /* $@93: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 903: /* $@94: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 904: /* make_struct_decl: "class" '<' $@93 type_declaration_no_options '>' $@94 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                           {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 905: /* $@95: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 906: /* $@96: %empty  */
                                                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 907: /* make_struct_decl: "variant" '<' $@95 variant_type_list '>' $@96 '(' use_initializer make_variant_dim ')'  */
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

  case 908: /* $@97: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 909: /* $@98: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 910: /* make_struct_decl: "default" '<' $@97 type_declaration_no_options '>' $@98 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = (yyvsp[-3].pTypeDecl);
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 911: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 912: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 913: /* make_tuple: make_tuple ',' expr  */
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

  case 914: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 915: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 916: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 917: /* $@99: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 918: /* $@100: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 919: /* make_tuple_call: "tuple" '<' $@99 tuple_type_list '>' $@100 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 920: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mka;
    }
    break;

  case 921: /* make_dim: make_dim "end of expression" make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 922: /* make_dim_decl: '[' optional_expr_list ']'  */
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

  case 923: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-2].pTypeDecl);
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 924: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-2].pTypeDecl);
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 925: /* $@101: %empty  */
                                       { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 926: /* $@102: %empty  */
                                                                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 927: /* make_dim_decl: "array" "struct" '<' $@101 type_declaration_no_options '>' $@102 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 928: /* $@103: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 929: /* $@104: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 930: /* make_dim_decl: "array" "tuple" '<' $@103 tuple_type_list '>' $@104 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 931: /* $@105: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 932: /* $@106: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 933: /* make_dim_decl: "array" "variant" '<' $@105 variant_type_list '>' $@106 '(' make_variant_dim ')'  */
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

  case 934: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
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

  case 935: /* $@107: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 936: /* $@108: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 937: /* make_dim_decl: "array" '<' $@107 type_declaration_no_options '>' $@108 '(' optional_expr_list ')'  */
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

  case 938: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 939: /* $@109: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 940: /* $@110: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 941: /* make_dim_decl: "fixed_array" '<' $@109 type_declaration_no_options '>' $@110 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = (yyvsp[-6].pTypeDecl);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 942: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mka;
    }
    break;

  case 943: /* make_table: make_table "end of expression" make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 944: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 945: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 946: /* make_table_decl: "begin of code block" optional_expr_map_tuple_list "end of code block"  */
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

  case 947: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
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

  case 948: /* make_table_decl: "table" '(' optional_expr_map_tuple_list ')'  */
                                                                       {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 949: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 950: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 951: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 952: /* array_comprehension_where: "end of expression" "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 953: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 954: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 955: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 956: /* array_comprehension: '[' "iterator" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                                 {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 957: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true,false);
    }
    break;

  case 958: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where "end of code block" ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),false,false);
    }
    break;

  case 959: /* array_comprehension: "begin of code block" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                              {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
    }
    break;

  case 960: /* array_comprehension: "{{" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block" "end of code block"  */
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
