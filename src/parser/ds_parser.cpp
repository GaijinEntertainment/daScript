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
  YYSYMBOL_global_function_declaration = 281, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 282, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 283, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 284,     /* function_declaration  */
  YYSYMBOL_285_8 = 285,                    /* $@8  */
  YYSYMBOL_open_block = 286,               /* open_block  */
  YYSYMBOL_close_block = 287,              /* close_block  */
  YYSYMBOL_expression_block = 288,         /* expression_block  */
  YYSYMBOL_expr_call_pipe = 289,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 290,           /* expression_any  */
  YYSYMBOL_expressions = 291,              /* expressions  */
  YYSYMBOL_expr_keyword = 292,             /* expr_keyword  */
  YYSYMBOL_optional_expr_list = 293,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_list_in_braces = 294, /* optional_expr_list_in_braces  */
  YYSYMBOL_optional_expr_map_tuple_list = 295, /* optional_expr_map_tuple_list  */
  YYSYMBOL_type_declaration_no_options_list = 296, /* type_declaration_no_options_list  */
  YYSYMBOL_expression_keyword = 297,       /* expression_keyword  */
  YYSYMBOL_298_9 = 298,                    /* $@9  */
  YYSYMBOL_299_10 = 299,                   /* $@10  */
  YYSYMBOL_300_11 = 300,                   /* $@11  */
  YYSYMBOL_301_12 = 301,                   /* $@12  */
  YYSYMBOL_expr_pipe = 302,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 303,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 304,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 305,     /* new_type_declaration  */
  YYSYMBOL_306_13 = 306,                   /* $@13  */
  YYSYMBOL_307_14 = 307,                   /* $@14  */
  YYSYMBOL_expr_new = 308,                 /* expr_new  */
  YYSYMBOL_expression_break = 309,         /* expression_break  */
  YYSYMBOL_expression_continue = 310,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 311, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 312,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 313, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 314,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 315,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 316,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 317,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 318,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 319,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 320, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 321,           /* expression_let  */
  YYSYMBOL_expr_cast = 322,                /* expr_cast  */
  YYSYMBOL_323_15 = 323,                   /* $@15  */
  YYSYMBOL_324_16 = 324,                   /* $@16  */
  YYSYMBOL_325_17 = 325,                   /* $@17  */
  YYSYMBOL_326_18 = 326,                   /* $@18  */
  YYSYMBOL_327_19 = 327,                   /* $@19  */
  YYSYMBOL_328_20 = 328,                   /* $@20  */
  YYSYMBOL_expr_type_decl = 329,           /* expr_type_decl  */
  YYSYMBOL_330_21 = 330,                   /* $@21  */
  YYSYMBOL_331_22 = 331,                   /* $@22  */
  YYSYMBOL_expr_type_info = 332,           /* expr_type_info  */
  YYSYMBOL_expr_list = 333,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 334,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 335,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 336,            /* capture_entry  */
  YYSYMBOL_capture_list = 337,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 338,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 339,               /* expr_block  */
  YYSYMBOL_expr_full_block = 340,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 341, /* expr_full_block_assumed_piped  */
  YYSYMBOL_342_23 = 342,                   /* $@23  */
  YYSYMBOL_expr_numeric_const = 343,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 344,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe_right = 345,   /* expr_assign_pipe_right  */
  YYSYMBOL_expr_assign_pipe = 346,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 347,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 348,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 349,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 350,           /* func_addr_expr  */
  YYSYMBOL_351_24 = 351,                   /* $@24  */
  YYSYMBOL_352_25 = 352,                   /* $@25  */
  YYSYMBOL_353_26 = 353,                   /* $@26  */
  YYSYMBOL_354_27 = 354,                   /* $@27  */
  YYSYMBOL_expr_field = 355,               /* expr_field  */
  YYSYMBOL_356_28 = 356,                   /* $@28  */
  YYSYMBOL_357_29 = 357,                   /* $@29  */
  YYSYMBOL_expr_call = 358,                /* expr_call  */
  YYSYMBOL_expr = 359,                     /* expr  */
  YYSYMBOL_360_30 = 360,                   /* $@30  */
  YYSYMBOL_361_31 = 361,                   /* $@31  */
  YYSYMBOL_362_32 = 362,                   /* $@32  */
  YYSYMBOL_363_33 = 363,                   /* $@33  */
  YYSYMBOL_364_34 = 364,                   /* $@34  */
  YYSYMBOL_365_35 = 365,                   /* $@35  */
  YYSYMBOL_expr_mtag = 366,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 367, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 368,        /* optional_override  */
  YYSYMBOL_optional_constant = 369,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 370, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 371, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 372, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 373, /* struct_variable_declaration_list  */
  YYSYMBOL_374_36 = 374,                   /* $@36  */
  YYSYMBOL_375_37 = 375,                   /* $@37  */
  YYSYMBOL_376_38 = 376,                   /* $@38  */
  YYSYMBOL_377_39 = 377,                   /* $@39  */
  YYSYMBOL_function_argument_declaration_no_type = 378, /* function_argument_declaration_no_type  */
  YYSYMBOL_function_argument_declaration_type = 379, /* function_argument_declaration_type  */
  YYSYMBOL_function_argument_list = 380,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 381,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 382,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 383,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 384,             /* variant_type  */
  YYSYMBOL_variant_type_list = 385,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 386,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 387,             /* copy_or_move  */
  YYSYMBOL_variable_declaration_no_type = 388, /* variable_declaration_no_type  */
  YYSYMBOL_variable_declaration_type = 389, /* variable_declaration_type  */
  YYSYMBOL_variable_declaration = 390,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 391,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 392,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 393, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 394, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 395, /* global_variable_declaration_list  */
  YYSYMBOL_396_40 = 396,                   /* $@40  */
  YYSYMBOL_optional_shared = 397,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 398, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 399,               /* global_let  */
  YYSYMBOL_400_41 = 400,                   /* $@41  */
  YYSYMBOL_enum_list = 401,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 402, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 403,             /* single_alias  */
  YYSYMBOL_404_42 = 404,                   /* $@42  */
  YYSYMBOL_alias_list = 405,               /* alias_list  */
  YYSYMBOL_alias_declaration = 406,        /* alias_declaration  */
  YYSYMBOL_407_43 = 407,                   /* $@43  */
  YYSYMBOL_optional_public_or_private_enum = 408, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 409,                /* enum_name  */
  YYSYMBOL_enum_declaration = 410,         /* enum_declaration  */
  YYSYMBOL_411_44 = 411,                   /* $@44  */
  YYSYMBOL_412_45 = 412,                   /* $@45  */
  YYSYMBOL_413_46 = 413,                   /* $@46  */
  YYSYMBOL_414_47 = 414,                   /* $@47  */
  YYSYMBOL_optional_structure_parent = 415, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 416,          /* optional_sealed  */
  YYSYMBOL_structure_name = 417,           /* structure_name  */
  YYSYMBOL_class_or_struct = 418,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 419, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 420, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 421,    /* structure_declaration  */
  YYSYMBOL_422_48 = 422,                   /* $@48  */
  YYSYMBOL_423_49 = 423,                   /* $@49  */
  YYSYMBOL_variable_name_with_pos_list = 424, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 425,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 426, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 427, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 428,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 429,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 430,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_basic_type_declaration = 431, /* bitfield_basic_type_declaration  */
  YYSYMBOL_bitfield_type_declaration = 432, /* bitfield_type_declaration  */
  YYSYMBOL_433_50 = 433,                   /* $@50  */
  YYSYMBOL_434_51 = 434,                   /* $@51  */
  YYSYMBOL_c_or_s = 435,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 436,          /* table_type_pair  */
  YYSYMBOL_dim_list = 437,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 438, /* type_declaration_no_options  */
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
  YYSYMBOL_464_77 = 464,                   /* $@77  */
  YYSYMBOL_465_78 = 465,                   /* $@78  */
  YYSYMBOL_type_declaration = 466,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 467,  /* tuple_alias_declaration  */
  YYSYMBOL_468_79 = 468,                   /* $@79  */
  YYSYMBOL_469_80 = 469,                   /* $@80  */
  YYSYMBOL_470_81 = 470,                   /* $@81  */
  YYSYMBOL_471_82 = 471,                   /* $@82  */
  YYSYMBOL_variant_alias_declaration = 472, /* variant_alias_declaration  */
  YYSYMBOL_473_83 = 473,                   /* $@83  */
  YYSYMBOL_474_84 = 474,                   /* $@84  */
  YYSYMBOL_475_85 = 475,                   /* $@85  */
  YYSYMBOL_476_86 = 476,                   /* $@86  */
  YYSYMBOL_bitfield_alias_declaration = 477, /* bitfield_alias_declaration  */
  YYSYMBOL_478_87 = 478,                   /* $@87  */
  YYSYMBOL_479_88 = 479,                   /* $@88  */
  YYSYMBOL_480_89 = 480,                   /* $@89  */
  YYSYMBOL_481_90 = 481,                   /* $@90  */
  YYSYMBOL_make_decl = 482,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 483,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 484,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 485,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim = 486,          /* make_struct_dim  */
  YYSYMBOL_make_struct_dim_list = 487,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 488,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 489, /* optional_make_struct_dim_decl  */
  YYSYMBOL_optional_block = 490,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_cur_cur = 491, /* optional_trailing_semicolon_cur_cur  */
  YYSYMBOL_optional_trailing_semicolon_cur_sqr = 492, /* optional_trailing_semicolon_cur_sqr  */
  YYSYMBOL_optional_trailing_semicolon_sqr_sqr = 493, /* optional_trailing_semicolon_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_sqr_sqr = 494, /* optional_trailing_delim_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_cur_sqr = 495, /* optional_trailing_delim_cur_sqr  */
  YYSYMBOL_use_initializer = 496,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 497,         /* make_struct_decl  */
  YYSYMBOL_498_91 = 498,                   /* $@91  */
  YYSYMBOL_499_92 = 499,                   /* $@92  */
  YYSYMBOL_500_93 = 500,                   /* $@93  */
  YYSYMBOL_501_94 = 501,                   /* $@94  */
  YYSYMBOL_502_95 = 502,                   /* $@95  */
  YYSYMBOL_503_96 = 503,                   /* $@96  */
  YYSYMBOL_504_97 = 504,                   /* $@97  */
  YYSYMBOL_505_98 = 505,                   /* $@98  */
  YYSYMBOL_make_tuple = 506,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 507,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 508,          /* make_tuple_call  */
  YYSYMBOL_509_99 = 509,                   /* $@99  */
  YYSYMBOL_510_100 = 510,                  /* $@100  */
  YYSYMBOL_make_dim = 511,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 512,            /* make_dim_decl  */
  YYSYMBOL_513_101 = 513,                  /* $@101  */
  YYSYMBOL_514_102 = 514,                  /* $@102  */
  YYSYMBOL_515_103 = 515,                  /* $@103  */
  YYSYMBOL_516_104 = 516,                  /* $@104  */
  YYSYMBOL_517_105 = 517,                  /* $@105  */
  YYSYMBOL_518_106 = 518,                  /* $@106  */
  YYSYMBOL_519_107 = 519,                  /* $@107  */
  YYSYMBOL_520_108 = 520,                  /* $@108  */
  YYSYMBOL_521_109 = 521,                  /* $@109  */
  YYSYMBOL_522_110 = 522,                  /* $@110  */
  YYSYMBOL_make_table = 523,               /* make_table  */
  YYSYMBOL_expr_map_tuple_list = 524,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 525,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 526, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 527,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 528       /* array_comprehension  */
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
#define YYLAST   14918

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  222
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  307
/* YYNRULES -- Number of rules.  */
#define YYNRULES  941
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1756

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
       0,   566,   566,   567,   572,   573,   574,   575,   576,   577,
     578,   579,   580,   581,   582,   583,   584,   588,   594,   595,
     596,   600,   601,   605,   606,   610,   629,   630,   631,   632,
     636,   637,   641,   642,   646,   647,   647,   651,   656,   665,
     680,   696,   701,   709,   709,   748,   774,   778,   779,   780,
     784,   787,   791,   797,   806,   809,   815,   816,   820,   824,
     825,   829,   832,   838,   844,   847,   853,   854,   858,   859,
     860,   869,   870,   874,   875,   879,   880,   880,   886,   887,
     888,   889,   890,   894,   900,   900,   906,   906,   912,   920,
     930,   939,   939,   943,   943,   949,   950,   951,   952,   953,
     954,   958,   963,   971,   972,   973,   977,   978,   979,   980,
     981,   982,   983,   984,   990,   993,   999,  1002,  1005,  1011,
    1012,  1013,  1014,  1018,  1035,  1057,  1060,  1070,  1085,  1100,
    1115,  1118,  1125,  1129,  1136,  1137,  1141,  1142,  1143,  1147,
    1150,  1157,  1161,  1162,  1163,  1164,  1165,  1166,  1167,  1168,
    1169,  1170,  1171,  1172,  1173,  1174,  1175,  1176,  1177,  1178,
    1179,  1180,  1181,  1182,  1183,  1184,  1185,  1186,  1187,  1188,
    1189,  1190,  1191,  1192,  1193,  1194,  1195,  1196,  1197,  1198,
    1199,  1200,  1201,  1202,  1203,  1204,  1205,  1206,  1207,  1208,
    1209,  1210,  1211,  1212,  1213,  1214,  1215,  1216,  1217,  1218,
    1219,  1220,  1221,  1222,  1223,  1224,  1225,  1226,  1227,  1228,
    1229,  1230,  1231,  1232,  1233,  1234,  1235,  1236,  1237,  1238,
    1239,  1240,  1241,  1242,  1243,  1244,  1245,  1246,  1247,  1248,
    1249,  1250,  1251,  1252,  1253,  1254,  1255,  1260,  1278,  1279,
    1280,  1284,  1290,  1290,  1308,  1309,  1312,  1313,  1316,  1320,
    1331,  1340,  1349,  1355,  1356,  1357,  1358,  1359,  1360,  1361,
    1362,  1363,  1364,  1365,  1366,  1367,  1368,  1369,  1370,  1371,
    1372,  1373,  1374,  1375,  1379,  1384,  1390,  1396,  1407,  1408,
    1412,  1413,  1417,  1418,  1422,  1426,  1433,  1433,  1433,  1439,
    1439,  1439,  1448,  1482,  1485,  1488,  1491,  1497,  1498,  1509,
    1513,  1516,  1524,  1524,  1524,  1527,  1533,  1536,  1540,  1544,
    1551,  1558,  1564,  1568,  1572,  1575,  1578,  1586,  1589,  1592,
    1600,  1603,  1611,  1614,  1617,  1625,  1631,  1632,  1633,  1637,
    1638,  1642,  1643,  1647,  1652,  1660,  1666,  1672,  1678,  1684,
    1693,  1702,  1711,  1723,  1726,  1732,  1732,  1732,  1735,  1735,
    1735,  1740,  1740,  1740,  1748,  1748,  1748,  1754,  1764,  1775,
    1788,  1798,  1809,  1824,  1827,  1833,  1834,  1841,  1852,  1853,
    1854,  1858,  1859,  1860,  1861,  1862,  1866,  1871,  1879,  1880,
    1881,  1885,  1890,  1897,  1904,  1904,  1913,  1914,  1915,  1916,
    1917,  1918,  1919,  1923,  1924,  1925,  1926,  1927,  1928,  1929,
    1930,  1931,  1932,  1933,  1934,  1935,  1936,  1937,  1938,  1939,
    1940,  1941,  1945,  1946,  1947,  1948,  1953,  1954,  1955,  1956,
    1957,  1958,  1959,  1960,  1961,  1962,  1963,  1964,  1965,  1966,
    1967,  1968,  1969,  1974,  1981,  1993,  1998,  2008,  2012,  2019,
    2022,  2022,  2022,  2027,  2027,  2027,  2040,  2044,  2048,  2053,
    2060,  2069,  2074,  2081,  2081,  2081,  2088,  2092,  2101,  2109,
    2117,  2121,  2124,  2130,  2131,  2132,  2133,  2134,  2135,  2136,
    2137,  2138,  2139,  2140,  2141,  2142,  2143,  2144,  2145,  2146,
    2147,  2148,  2149,  2150,  2151,  2152,  2153,  2154,  2155,  2156,
    2157,  2158,  2159,  2160,  2161,  2162,  2163,  2164,  2165,  2171,
    2172,  2173,  2174,  2175,  2188,  2197,  2198,  2199,  2200,  2201,
    2202,  2203,  2204,  2205,  2206,  2207,  2208,  2211,  2214,  2215,
    2218,  2218,  2218,  2221,  2226,  2230,  2234,  2234,  2234,  2239,
    2242,  2246,  2246,  2246,  2251,  2254,  2255,  2256,  2257,  2258,
    2259,  2260,  2261,  2262,  2264,  2268,  2269,  2274,  2278,  2279,
    2280,  2281,  2282,  2283,  2284,  2288,  2292,  2296,  2300,  2304,
    2308,  2312,  2316,  2320,  2327,  2328,  2329,  2333,  2334,  2335,
    2339,  2340,  2344,  2345,  2346,  2350,  2351,  2355,  2366,  2369,
    2372,  2372,  2376,  2376,  2395,  2394,  2410,  2409,  2423,  2432,
    2444,  2453,  2463,  2464,  2465,  2466,  2467,  2471,  2474,  2483,
    2484,  2488,  2491,  2494,  2509,  2518,  2519,  2523,  2526,  2529,
    2542,  2543,  2547,  2553,  2559,  2568,  2571,  2578,  2581,  2587,
    2588,  2589,  2593,  2594,  2598,  2605,  2610,  2619,  2625,  2636,
    2639,  2644,  2649,  2657,  2668,  2671,  2674,  2674,  2694,  2695,
    2699,  2700,  2701,  2705,  2708,  2708,  2727,  2730,  2733,  2748,
    2767,  2768,  2769,  2774,  2774,  2800,  2801,  2805,  2806,  2806,
    2810,  2811,  2812,  2816,  2826,  2831,  2826,  2843,  2848,  2843,
    2863,  2864,  2868,  2869,  2873,  2879,  2880,  2881,  2882,  2886,
    2887,  2888,  2892,  2895,  2901,  2906,  2901,  2926,  2933,  2938,
    2947,  2953,  2964,  2965,  2966,  2967,  2968,  2969,  2970,  2971,
    2972,  2973,  2974,  2975,  2976,  2977,  2978,  2979,  2980,  2981,
    2982,  2983,  2984,  2985,  2986,  2987,  2988,  2989,  2990,  2994,
    2995,  2996,  2997,  2998,  2999,  3000,  3001,  3005,  3016,  3020,
    3027,  3039,  3046,  3055,  3060,  3063,  3073,  3086,  3087,  3088,
    3089,  3090,  3094,  3094,  3094,  3108,  3109,  3113,  3117,  3124,
    3128,  3135,  3136,  3137,  3138,  3139,  3154,  3160,  3160,  3160,
    3164,  3169,  3176,  3176,  3183,  3187,  3191,  3196,  3201,  3206,
    3211,  3215,  3219,  3224,  3228,  3232,  3237,  3237,  3237,  3243,
    3250,  3250,  3250,  3255,  3255,  3255,  3261,  3261,  3261,  3266,
    3271,  3271,  3271,  3276,  3276,  3276,  3285,  3290,  3290,  3290,
    3295,  3295,  3295,  3304,  3309,  3309,  3309,  3314,  3314,  3314,
    3323,  3323,  3323,  3329,  3329,  3329,  3338,  3341,  3352,  3368,
    3368,  3373,  3378,  3368,  3403,  3403,  3408,  3414,  3403,  3439,
    3439,  3444,  3449,  3439,  3489,  3490,  3491,  3492,  3493,  3497,
    3504,  3511,  3517,  3523,  3530,  3537,  3543,  3552,  3555,  3561,
    3569,  3574,  3581,  3586,  3593,  3598,  3604,  3605,  3609,  3610,
    3615,  3616,  3620,  3621,  3625,  3626,  3630,  3631,  3632,  3636,
    3637,  3638,  3642,  3643,  3647,  3653,  3660,  3668,  3675,  3683,
    3692,  3692,  3692,  3700,  3700,  3700,  3707,  3707,  3707,  3718,
    3718,  3718,  3729,  3732,  3738,  3752,  3758,  3764,  3770,  3770,
    3770,  3784,  3789,  3796,  3815,  3820,  3827,  3827,  3827,  3837,
    3837,  3837,  3851,  3851,  3851,  3865,  3874,  3874,  3874,  3894,
    3901,  3901,  3901,  3911,  3916,  3923,  3926,  3932,  3951,  3960,
    3968,  3988,  4013,  4014,  4018,  4019,  4024,  4027,  4030,  4033,
    4036,  4039
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
  "function_name", "global_function_declaration",
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

#define YYPACT_NINF (-1552)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-808)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1552,   150, -1552, -1552,    40,   -30,    83,   351, -1552,   -59,
     750,   750,   750, -1552, -1552,   163,   216, -1552, -1552,   462,
   -1552, -1552, -1552, -1552,   302, -1552,   108, -1552, -1552, -1552,
   -1552, -1552, -1552, -1552, -1552, -1552, -1552,    76, -1552,   127,
     133,   165, -1552, -1552, -1552, -1552,    83, -1552,    28, -1552,
   -1552, -1552,   750,   750, -1552, -1552,   108, -1552, -1552, -1552,
   -1552, -1552,   212,   258, -1552, -1552, -1552, -1552,   216,   216,
     216,   218, -1552,   858,   282, -1552, -1552, -1552, -1552,   804,
     816,   627,   847, -1552,   859,    67,    40,   283,   -30,   275,
     354, -1552,   853,   853, -1552,   405,   462,   114,   462,   871,
     412,   463,   468, -1552,   473,   503, -1552, -1552,     9,    40,
     216,   216,   216,   216, -1552, -1552, -1552, -1552, -1552, -1552,
   -1552,   540, -1552, -1552, -1552, -1552, -1552, -1552, -1552,   351,
   -1552, -1552, -1552, -1552, -1552,   856,    38, -1552, -1552, -1552,
   -1552,   707, -1552, -1552, -1552, -1552, -1552, -1552, -1552,   462,
   -1552, -1552, -1552,   548, -1552, -1552, -1552, -1552, -1552,   646,
   -1552,   -49, -1552,   869,   687,   858, 14745, -1552,   190,   734,
   -1552,  -108, -1552, -1552, -1552,   867, -1552, -1552, -1552, -1552,
   -1552, -1552, -1552,     2, -1552,   659, -1552,   842, -1552,   725,
     351,   351, -1552, -1552, 13608, -1552, -1552, -1552, -1552, -1552,
   -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552,
   -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552,
     902,   905, -1552,   737,   351,   852, -1552, -1552,   751, -1552,
     301,    40,    40,   184,   443, -1552, -1552, -1552,    38, -1552,
   10534, -1552,   628,   351, -1552, -1552,   776,   805, -1552, -1552,
   -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552,
   -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552,   815,   759,
   -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552,
   -1552, -1552,   967, -1552, -1552, -1552, -1552, -1552, -1552, -1552,
   -1552, -1552, -1552, -1552, -1552,   819,   790, -1552, -1552,   -37,
     827, -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552,
   -1552,   351, -1552,   835,   351, -1552, -1552,  -108,   366, -1552,
      40, -1552,   813,   993,   329, -1552, -1552,   843,   844,   860,
     837,   864,   866, -1552, -1552, -1552,   848, -1552, -1552, -1552,
   -1552, -1552,    79, -1552, -1552, -1552, -1552, -1552, -1552, -1552,
   -1552, -1552, -1552, -1552,   874, -1552, -1552, -1552,   876,   877,
   -1552, -1552, -1552, -1552,   879,   880,   862,   163, -1552, -1552,
   -1552, -1552, -1552,   691,   885, -1552, -1552, -1552, -1552, -1552,
   -1552, -1552, -1552, -1552, -1552, -1552,   908,  1170, -1552,   900,
   -1552,    77,   462,   603,   901, 10534, -1552,  2655, -1552,   408,
     163, -1552, -1552, -1552,   443,   904, -1552,  9880,   909,   945,
   10534, -1552,   185, -1552, -1552, -1552,  9880, -1552, -1552,   949,
     930,   661,   670,   682, -1552, -1552,  9880,   -19, -1552, -1552,
   -1552,    39, -1552, -1552, -1552,    65,  6168, -1552,   914,  2196,
   -1552, 10385,   499, -1552, -1552, -1552, -1552, -1552, -1552, -1552,
   -1552, -1552, -1552, -1552, -1552, -1552,  9880, -1552, -1552,   527,
     -29,   -29,   -29, -1552,   885, -1552,   935,   938,  9880, -1552,
   -1552, -1552, -1552, -1552,    56,   -41,   939,    43,  3482, -1552,
   -1552,   351,     4,  6374,   923, -1552,  9880,   969,   948,   950,
     926, -1552,   462,   951,   984,  6580,   234,   555,   954, -1552,
     605,   955,   956,  3688,  9880,  9880,   200,   200,   200,   940,
     944,   946,   957,   958,   959, -1552,  1012,  2034,  6788, -1552,
   -1552, -1552, -1552, -1552, -1552, -1552,  6994,   964, -1552,  7202,
    9880,  9880,  9880,  9880,  9880,  5344,  7408, -1552,   936, -1552,
   -1552,   462,   462, -1552,  9880,  1155, -1552, -1552, -1552, -1552,
   -1552, -1552,  1136, -1552, -1552, -1552,   639, -1552,    -7,   462,
   -1552,   462,   462,   462, -1552,   462, -1552, -1552,  1112, -1552,
   -1552, -1552, -1552,   962, -1552, -1552,   238, -1552, -1552, -1552,
   -1552, -1552, -1552, 10199, -1552,   963, -1552, -1552, -1552, -1552,
   -1552, -1552, -1552,   437, -1552,   519, -1552,    60, -1552, -1552,
     965,   981,   983, -1552,  2188, -1552,  1140,  1106, -1552, -1552,
   -1552,  4100, 10534, 10534, 10534,  2390, 10534, 10534,   968, -1552,
   10534,   737, 10534,   737, 10534,   737, 10637,  1014, 10877, -1552,
    9880, -1552, -1552, -1552, -1552, -1552,   972, -1552, -1552, 13091,
    9880, -1552,   691,   575,   -42, -1552, -1552,   647, -1552,   885,
     519,   995,   647, -1552,   519, 10988,   974,  1150, -1552, -1552,
     154, -1552, -1552, -1552, -1552, -1552, 13650,   527, -1552,   978,
   -1552, -1552,   163,   607, -1552,  1005,  1011,  1013, -1552,  9880,
    4100, -1552,  1021,  1073, 10842,  1199, 10534,  9880,  9880, 14133,
    9880,  1043, 13650,  1026, -1552, -1552,  9880, -1552, -1552,  1025,
    1055, 14133,  9880, -1552, -1552,  9880, -1552, -1552,  9880, -1552,
   10534,  4100, -1552, 10842,   656,   656,  1003, -1552,   962, -1552,
   -1552, -1552,  9880,  9880,  9880,  9880,  9880,  9880,   527,  2861,
     527,  3068,   527, 13836, -1552,   865, -1552, 13650, -1552,   840,
     527,  1041, -1552,  1035,   656,   656,   -20,   656,   656,   527,
    1216,  1016,  1050, 14133,  1028,   194,  1050,  1051,  1029,   298,
   -1552, -1552, 13650, -1552,   351, -1552, -1552, -1552,  4306, -1552,
   -1552, -1552, -1552, -1552, -1552,    -9,    47,   200, -1552, 10281,
   14553,  4512,  4512,  4512,  4512,  4512,  4512,  4512,  4512,  9880,
    9880, -1552, -1552,  9880,  4512,  4512,  9880,  9880,  9880,  1075,
    4512,  9880,   214,  9880,  9880,  9880,  9880,  9880,  9880,  4512,
    4512,  9880,  9880,  9880,  4512,  4512,  4512,  9880,  4512,  7614,
    9880,  9880,  9880,  9880,  9880,  9880,  9880,  9880,  9880,  9880,
   14683,  9880, -1552,  7820,   408,  9880, -1552, -1552, -1552,   216,
   -1552,  1238, -1552,  -108, 10534, -1552,  1076, -1552,  4100, -1552,
   10721,   515,   566,  1052,   340, -1552,   594,   673, -1552,  1077,
     697,   827,   712,   827,   777,   827, -1552,   310, -1552,   419,
   -1552, 10534,  1061, -1552, -1552, 13202,   510, -1552,   519, 10534,
   -1552, -1552, 10534, -1552, -1552, -1552,  9880,  1080, -1552,  1085,
   -1552, 10534, -1552,  9880, 10534, 10534, -1552,    35, 10534,  5550,
    8026,  1086,  9880, 10534, -1552, -1552, -1552, 10534,  1050, -1552,
    1021,  9880,  9880,  9880,  9880,  9880,  9880,  9880,  9880,  9880,
    9880,  9880,  9880,  9880,  9880,  9880,  9880,  9880,  9880,   351,
     999,  1042, 14133, 11026,  1070, -1552, -1552, 10534, 10534, 11137,
   10534, -1552, -1552, 11172, 10534,  1050, 10534, 10637,  1050,  1014,
     526, -1552, 10842, -1552,    47, 11283, 11321, 11432, 11467, 11578,
   11616,    37,   200,  3275,  4720,  5756, 13873,  1074,    16,   333,
    1079,   314,    46,  5962,    16,   710,    55,  9880,  1091,  9880,
   -1552, -1552, 10534, -1552, 10534, -1552,  9880,   561,    72, -1552,
    9880, -1552,    73,   527, -1552,  9880, -1552,  9880,  9880,  9880,
    1071,   528, -1552, -1552,  1078,  1083,   686, -1552, -1552,   797,
    9880, -1552,   962,   -85,  4928, -1552,   122,  1072,  1115,  1115,
   -1552, -1552,  1092,   -24,   737, -1552,  1095,  1096, -1552, -1552,
    1098,  1099, -1552, -1552,   200,   200,   200, -1552, -1552,  1705,
   -1552,  1705, -1552,  1705, -1552,  1705, -1552,  1705, -1552,  1705,
   -1552,  1705, -1552,  1705,   530,   530,  1138, -1552,  1705, -1552,
    1705,  1138,  1950,  1950,  1103, -1552,  1705,   699,  1113, -1552,
   13237,   -52,   -52,   963, 14133,   530,   530, -1552,  1705, -1552,
    1705,  2225, 10678, 14356, -1552,  1705, -1552,  1705, -1552,  1705,
   14226, -1552,  1705, 14652, 13966, 10312, 14389, 14482,  1138,  1138,
     164,   164,   699,   699,   699,   531,  9880,  1114,  1117,   623,
    9880,  1293,  1118, 13348, -1552,   222,   519, 13743,  1124,   410,
     881,  1235,   462,  1007, -1552, -1552, 10721, -1552, -1552, -1552,
   -1552, 10534, -1552, -1552, -1552,   176, -1552,  1116, -1552,  1129,
   -1552,  1130, -1552, 10637, -1552,  1014,   429,   885, -1552,  1119,
   -1552,  9880, -1552, -1552,   885,   885, 11727, -1552,  1300,   -51,
   14133,  1111,  1177,  9880,   785,   679,   246,  1126,  1127,  1163,
   11762,   203, 11873,   788, 10534, 10637,  1014,  1339,  1128, 14133,
   14133, 14133, 14133, 14133, 14133, 14133, 14133, 14133, 14133, 14133,
   14133, 14133, 14133, 14133, 14133, 14133, 14133, -1552,  1133, 10534,
   -1552, -1552, 10534,  9880,  1351,  1498, -1552,  1509, -1552,  1597,
    1139,  1759,   440,  1142,   472,    47,   737, -1552, -1552, -1552,
   -1552, -1552,  1134,  9880, -1552,  5136, 13091,    14,  9880,   528,
     679,   333, -1552, -1552,  1131, -1552,  9880,  9880, -1552,  1146,
   -1552,  9880,   679,   657,  1147, -1552, -1552,  9880, 14133, -1552,
   -1552,   497,   551, 14003,  9880, -1552,  9880,    74, 11911, 14133,
   14133, -1552,  1141,   202,  9880,  9880, 10534,   737,   351, -1552,
   -1552,  9880, -1552,  1383,  2655,    47,   207, -1552,  1148,   422,
   10086, -1552, -1552, -1552,   439,   261,   -24,  1183,  1191,  1152,
    1192,  1194, -1552,   454,   827, -1552,  9880, -1552,  9880, -1552,
   -1552, -1552,  8232,  9880, -1552,  1173,  1156, -1552, -1552,  9880,
    1158, -1552, 13386,  9880,  8438,  1162, -1552, 13497, -1552,  8644,
   -1552, -1552, -1552, -1552,  1188,   462, -1552, -1552,   702, -1552,
      82, -1552,    47, -1552, -1552, -1552, -1552,   885, -1552, -1552,
   -1552,  1208, -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552,
   -1552, -1552,  1165, 10534, -1552, 13743, -1552,  1210,  9880, -1552,
   -1552,   324, -1552,  1167, -1552, -1552, -1552,   592, -1552,  1213,
    1172, -1552, -1552,  1976,   615,   651, -1552, -1552,  9880,  2442,
     885, 14133, -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552,
   -1552,   792,   827,  8850,   724,    16,   333, 14133,  1074, -1552,
   -1552, 14133,  1079, -1552,   771,    16,  1171, -1552, -1552, -1552,
   -1552,   778, -1552, -1552, -1552,  1209,   779,   782,  9880,   220,
    9880,  9880,  9880, 12022, 12057,  2584,   827, -1552, 13650, -1552,
     462, -1552,   737, -1552,  1175,  4928,  1218,  1178,   386,   292,
   -1552, -1552,  1223, -1552, -1552,   -24,  1181,   -43, 10534, 12168,
   10534, 12206, -1552,   306, 12317, -1552,  9880, 14263,  9880, -1552,
   12352,  4928, -1552,   347,  9880, -1552, -1552, -1552,   379, 10534,
   -1552,  1368,    82, -1552, -1552,   881,  1185, -1552, -1552, -1552,
   -1552, -1552, -1552,   885, -1552, -1552, 14133,  1189,  1190, -1552,
   -1552, -1552,  9880,  1236,  1214,  9880, -1552, -1552, -1552, -1552,
    1197,  1198,  1200,  9880,  9880,  9880,  1201,  1345,  1202,  1204,
    9056, -1552,   -43, -1552,   381,  9880,   333, -1552,  9880,   657,
   -1552,  9880,  9880,  1165, -1552, -1552,  9880,  9880,   783,  9880,
    9880, 12463, 14133, 14133, -1552, -1552, -1552,  1224,   797,  3894,
   -1552,   827, -1552,   477, -1552,   732, 10534,   185, -1552,  1206,
   -1552, -1552,  9262, -1552, -1552,  4509, -1552,   799, -1552, -1552,
   -1552, 10534, 12501, 12612, -1552,   483, -1552, 12647, -1552,   368,
   -1552, -1552,  1368,   527,  1345,  1345, 12758,  1227,  1205, 12796,
    1211,  1215,  1217,  9880, -1552,  9880,  1138,  1138,  1138,  9880,
   -1552, -1552,  1345,  1345, -1552, 12907, -1552, -1552, 14096, -1552,
   14096, -1552,  1242,  1138, -1552,  1254,  1242, 14096,  9880, 14133,
   14133,   223,   425, -1552, -1552,  9468,  9674, -1552, -1552, -1552,
   -1552, -1552, 14133,   351,  1219, 10534,   185,   793,  4100, -1552,
    9880, 14226, -1552, -1552,   802, -1552, -1552,  1221, -1552, -1552,
   14745, -1552, -1552, -1552, -1552,   141,   141, -1552,  9880,  9880,
   -1552,  1345,  1345,   679,  1226,  1228,  1050,   141,   679, -1552,
    1380,  1220,  1264,  1265, -1552,  1266,  1237, 14096,  9880,  9880,
   -1552,   425,  9880,  9880, 14133, -1552, -1552,   793,  4100,  4100,
   -1552, 10721, 14263, -1552, -1552, -1552, -1552,   462, 14745,   679,
    1074,  1260, -1552,  1234,  1240, 12942, 13053,   141,   141,  1074,
    1241, -1552, -1552,  1243,  1244,  1247,  9880,  1250,  1252,  1270,
   -1552, -1552,  1253, 14133, 14133, -1552, 14133,  4100, -1552, 10721,
   -1552, 10721, -1552, -1552, -1552,   351,   385,  1255, -1552, -1552,
   -1552, -1552, -1552,  1256,  1258, -1552, -1552, -1552, -1552, 14133,
   -1552, -1552, -1552, -1552, -1552, 10721, -1552, -1552, -1552, -1552,
     679, -1552, -1552, -1552,   433, -1552
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   134,     1,   329,     0,     0,     0,   658,   330,     0,
     650,   650,   650,    71,    72,     0,     0,    15,     3,     0,
      10,     9,     8,    16,     0,     7,   638,     6,    11,     5,
       4,    13,    12,    14,   104,   105,   103,   112,   114,    45,
      61,    58,    59,    47,    48,    49,     0,    50,    56,    46,
     245,   244,   650,   650,    22,    21,   638,   652,   651,   829,
     819,   824,     0,   297,    43,   120,   121,   122,     0,     0,
       0,   123,   125,   132,     0,   119,    17,   676,   675,   238,
     660,     0,   679,   639,   640,     0,     0,     0,     0,    51,
       0,    57,     0,     0,    54,     0,     0,   650,     0,    18,
       0,     0,     0,   299,     0,     0,   131,   126,     0,     0,
       0,     0,     0,     0,   135,   240,   239,   242,   237,   662,
     661,     0,   678,   677,   681,   680,   684,   642,   641,   644,
     110,   111,   108,   109,   107,     0,     0,   106,   115,    62,
      60,    56,    53,    52,   653,   655,   247,   246,   657,     0,
     659,    19,    20,    23,   830,   820,   825,   298,    41,    44,
     130,     0,   127,   128,   129,   133,     0,   663,     0,   672,
     634,   564,    26,    27,    31,     0,    99,   100,    97,    98,
      96,    95,   101,     0,    55,     0,   656,     0,    25,   737,
       0,     0,    42,   124,     0,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
       0,     0,   141,   136,     0,     0,   664,   673,     0,   685,
     636,     0,     0,   566,     0,    28,    29,    30,     0,   113,
       0,    24,     0,     0,   821,   826,   206,   207,   204,   144,
     145,   147,   146,   148,   149,   150,   151,   177,   178,   175,
     176,   168,   179,   180,   169,   166,   167,   205,   188,     0,
     203,   181,   182,   183,   184,   155,   156,   157,   152,   153,
     154,   165,     0,   171,   172,   170,   163,   164,   159,   158,
     160,   161,   162,   143,   142,   187,     0,   173,   174,   564,
     139,   274,   243,   719,   722,   725,   726,   720,   723,   721,
     724,     0,   646,   670,   682,   635,   643,   564,     0,   116,
       0,   118,     0,   624,   622,   645,   102,     0,     0,     0,
       0,     0,     0,   692,   712,   693,   728,   694,   698,   699,
     700,   701,   718,   705,   706,   707,   708,   709,   710,   711,
     713,   714,   715,   716,   789,   697,   704,   717,   796,   803,
     695,   702,   696,   703,     0,     0,     0,     0,   727,   751,
     754,   752,   753,   816,   654,   740,   741,   738,   739,   831,
     601,   607,   208,   209,   202,   186,   210,   189,   185,     0,
     137,   328,   592,   593,     0,     0,   241,     0,   667,   665,
       0,   674,   578,   686,     0,     0,   117,     0,     0,     0,
       0,   623,     0,   757,   780,   783,     0,   786,   776,     0,
       0,   790,   797,   804,   810,   813,     0,   280,   766,   771,
     765,     0,   779,   775,   768,     0,     0,   770,   755,     0,
     733,   822,   827,   211,   191,   192,   194,   193,   195,   196,
     197,   198,   190,   199,   200,   201,     0,   326,   327,     0,
     564,   564,   564,   138,   140,   276,     0,     0,     0,    73,
      74,    86,   469,   470,     0,     0,     0,     0,   314,   463,
     312,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,   313,     0,     0,     0,     0,     0,     0,     0,   718,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   553,     0,     0,     0,   386,
     388,   387,   389,   390,   391,   392,     0,     0,    37,   282,
       0,     0,     0,     0,     0,   278,     0,   368,   369,   467,
     466,     0,     0,   253,     0,     0,   269,   264,   261,   260,
     262,   263,   248,   296,   275,   255,   547,   254,   464,     0,
     538,    81,    82,    79,   267,    80,   268,   270,   332,   259,
     537,   536,   535,   134,   541,   465,     0,   256,   540,   539,
     511,   471,   512,   393,   472,     0,   468,   834,   838,   835,
     836,   837,   646,     0,   647,     0,   671,   582,   637,   565,
       0,     0,     0,   547,     0,   626,   627,     0,   620,   621,
     619,     0,     0,     0,     0,     0,     0,     0,     0,   742,
       0,   136,     0,   136,     0,   136,     0,     0,     0,   762,
     278,   761,   773,   774,   767,   769,     0,   772,   756,     0,
       0,   818,   817,   832,   297,   745,   746,     0,   602,   597,
       0,     0,     0,   608,     0,     0,     0,   687,   589,   590,
     612,   594,   596,   595,   880,   883,     0,     0,   302,   306,
     305,   311,     0,     0,   354,     0,     0,     0,   916,     0,
       0,   318,   315,     0,   363,     0,     0,   282,     0,   300,
       0,     0,     0,     0,   345,   348,     0,   273,   351,     0,
       0,    65,     0,    88,   920,     0,   889,   898,     0,   886,
       0,     0,   323,   320,   499,   500,   369,   381,   134,   295,
     293,   294,     0,     0,     0,     0,     0,     0,     0,   858,
       0,     0,     0,   896,   923,     0,   286,     0,   289,     0,
       0,     0,   925,   934,   476,   475,   513,   474,   473,     0,
       0,     0,   934,   363,     0,   297,   934,   934,     0,   370,
     271,   272,     0,    84,     0,   384,   251,   545,     0,   258,
     265,   266,   317,   322,   331,     0,   378,     0,   257,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   501,   502,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     453,     0,   250,     0,   668,     0,   648,   666,   580,     0,
     579,     0,   683,   564,     0,   625,     0,   629,     0,   633,
     393,     0,     0,     0,   747,   760,     0,     0,   729,     0,
       0,   139,     0,   139,     0,   139,   599,     0,   605,     0,
     730,     0,   934,   764,   749,     0,     0,   734,     0,     0,
     603,   823,     0,   609,   828,   591,     0,     0,   611,     0,
     610,     0,   613,     0,     0,     0,    89,     0,     0,   872,
       0,     0,     0,     0,   906,   909,   912,     0,   934,   319,
     316,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   301,     0,     0,    90,    91,     0,     0,     0,
       0,    63,    64,     0,     0,   934,     0,     0,   934,     0,
       0,   324,   321,   370,   378,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   892,   850,   858,     0,
     901,     0,     0,     0,   858,     0,     0,     0,     0,     0,
     861,   928,     0,   277,     0,    40,     0,    38,     0,   927,
     935,   283,     0,     0,   903,   935,   279,     0,     0,     0,
       0,   935,   849,   504,     0,     0,   440,   437,   439,    68,
       0,   274,   134,     0,   278,   456,     0,     0,     0,     0,
     344,   343,     0,     0,   136,   292,     0,     0,   524,   523,
       0,     0,   525,   529,     0,     0,     0,   415,   424,   403,
     425,   404,   427,   406,   426,   405,   428,   407,   418,   397,
     419,   398,   420,   399,   477,   478,   490,   429,   408,   430,
     409,   491,   488,   489,     0,   417,   395,   518,     0,   509,
       0,   542,   543,   544,   396,   479,   480,   431,   410,   432,
     411,   495,   496,   497,   421,   400,   422,   401,   423,   402,
     498,   416,   394,     0,     0,   493,   494,   492,   486,   487,
     482,   481,   483,   484,   485,     0,     0,     0,   446,     0,
       0,     0,     0,     0,   461,     0,     0,     0,     0,     0,
     572,   575,     0,     0,   628,   631,   393,   632,   758,   781,
     784,     0,   787,   777,   731,     0,   791,     0,   798,     0,
     805,     0,   811,     0,   814,     0,     0,   284,   935,     0,
     750,     0,   735,   833,   598,   604,     0,   689,   690,   615,
     614,     0,     0,     0,     0,   873,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   364,
     403,   404,   406,   405,   407,   397,   398,   399,   408,   409,
     395,   410,   411,   400,   401,   402,   394,   325,     0,     0,
     929,   514,     0,     0,     0,     0,   515,     0,   546,     0,
       0,     0,     0,     0,     0,   378,   136,   548,   549,   550,
     551,   552,     0,     0,   859,     0,   363,   858,     0,     0,
       0,     0,   867,   868,     0,   875,     0,     0,   865,     0,
     904,     0,     0,     0,     0,   863,   905,     0,   895,   860,
     924,     0,     0,    34,     0,   926,     0,     0,     0,   840,
     839,   503,     0,     0,     0,     0,     0,   136,     0,    66,
      67,     0,    83,    75,     0,   378,     0,   457,     0,     0,
       0,   460,   458,   333,     0,     0,     0,     0,     0,     0,
       0,     0,   376,     0,   139,   520,     0,   526,     0,   414,
     412,   413,     0,     0,   507,     0,     0,   530,   534,     0,
       0,   510,     0,     0,     0,     0,   447,     0,   454,     0,
     505,   462,   669,   649,     0,   135,   573,   574,   575,   576,
     567,   583,   378,   630,   759,   782,   785,   748,   788,   778,
     743,     0,   792,   794,   799,   801,   806,   808,   812,   600,
     815,   606,   280,     0,   281,     0,   688,     0,     0,   881,
     884,     0,   303,     0,   308,   309,   307,     0,   357,     0,
       0,   360,   355,     0,     0,     0,   917,   915,   282,     0,
      94,    92,   346,   349,   352,   921,   919,   890,   899,   897,
     887,     0,   139,     0,     0,   858,     0,   893,   851,   874,
     866,   894,   902,   864,     0,   858,     0,   870,   871,   878,
     862,     0,   287,   290,    35,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   139,    69,     0,    76,
       0,   249,   136,   459,     0,   278,     0,     0,   622,     0,
     373,   374,     0,   372,   371,     0,     0,     0,     0,     0,
       0,     0,   435,     0,     0,   531,     0,   519,     0,   508,
       0,   278,   448,     0,     0,   506,   455,   451,     0,     0,
     588,   570,   567,   568,   569,   572,     0,   744,   732,   795,
     802,   809,   763,   285,   736,   691,   616,     0,     0,    87,
     304,   310,     0,     0,     0,     0,   356,   907,   910,   913,
       0,     0,     0,     0,     0,     0,     0,   872,     0,     0,
       0,   252,     0,   554,     0,     0,     0,   876,     0,     0,
     869,     0,     0,   280,    32,    39,     0,     0,     0,     0,
       0,     0,   842,   841,   438,   563,   441,     0,    68,     0,
      85,   139,   433,     0,   334,   622,     0,     0,   380,     0,
     377,   379,     0,   365,   383,     0,   562,     0,   560,   436,
     557,     0,     0,     0,   556,     0,   449,     0,   452,     0,
     571,   584,   570,     0,   872,   872,     0,     0,     0,     0,
       0,     0,     0,   278,   930,   282,   347,   350,   353,     0,
     873,   891,   872,   872,   516,     0,   382,   555,   932,   877,
     932,   879,   932,   288,   291,    36,   932,   932,     0,   844,
     843,     0,     0,   444,    70,   314,     0,    77,    81,    82,
      79,    80,    78,     0,     0,     0,     0,     0,     0,   375,
       0,   366,   521,   527,     0,   561,   559,     0,   558,   581,
       0,   586,   618,   617,   577,   857,   857,   358,     0,     0,
     361,   872,   872,   847,     0,     0,   934,   857,   847,   517,
       0,     0,     0,     0,    33,     0,     0,   932,     0,     0,
     442,     0,     0,     0,   320,   385,   434,     0,     0,     0,
     342,   393,   367,   522,   528,   532,   450,     0,     0,     0,
     854,   934,   856,     0,     0,     0,     0,   857,   857,   848,
       0,   918,   931,     0,     0,     0,     0,     0,     0,     0,
     940,   936,     0,   846,   845,   445,   321,     0,   340,   393,
     338,   393,   341,   533,   585,     0,     0,   935,   855,   882,
     885,   359,   362,     0,     0,   914,   922,   900,   888,   933,
     938,   939,   941,   937,   336,   393,   339,   337,   587,   852,
       0,   908,   911,   335,     0,   853
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1552, -1552, -1552, -1552, -1552, -1552, -1552,   721,  1386, -1552,
   -1552, -1552, -1552, -1552, -1552,  1472, -1552, -1552, -1552,   889,
    1431, -1552,  1346, -1552, -1552,  1392, -1552, -1552, -1552,   -50,
      -1, -1552, -1552, -1552,   -48, -1552, -1552, -1552, -1552, -1552,
   -1552, -1552, -1552, -1552, -1552,  1257, -1552, -1552,   -54,   -39,
   -1552, -1552, -1552,   478,   653,  -551,  -576,  -826, -1552, -1552,
   -1552, -1551, -1552, -1552,     6,  -150,  -220,  -392, -1552,   482,
   -1552,  -618, -1256,  -680,  -413,  -356, -1552, -1552, -1552, -1552,
    -477,    -5, -1552, -1552, -1552, -1552, -1552,   -45,   -35,   -23,
   -1552,   -21, -1552, -1552, -1552,  1496, -1552,   479, -1552, -1552,
   -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552,
   -1552,  -457,    -2,   934,    70,   237,  -911,  -480, -1552,  -548,
   -1552, -1552,  -391,  1763, -1552, -1552, -1552, -1541, -1552, -1552,
   -1552, -1552, -1552, -1552, -1552, -1552, -1552,   952, -1552, -1552,
   -1552, -1552, -1552, -1552, -1552,  -156,    48,   -47,    49,   198,
   -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552,   595,  -425,
    -903, -1552,  -428,  -907, -1552,  -640,   -38,   -36, -1552,  -604,
   -1380, -1552,  -375, -1552, -1552,  1474, -1552, -1552, -1552,   942,
    1059,   277, -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552,
   -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552,
   -1552, -1552,  -611,  -136, -1552,  1066, -1552, -1552, -1552,  1203,
   -1552, -1552, -1552,  -351, -1552, -1552,  -401, -1552, -1552, -1552,
   -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552,
   -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552,
   -1552, -1552, -1552, -1552,  -206, -1552, -1552, -1552, -1552, -1552,
   -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552,
    1069,  -698,  -112,  -743,  -714, -1552, -1552, -1355,  -938, -1552,
   -1552, -1552, -1191,    30,  -940, -1552, -1552, -1552, -1552, -1552,
   -1552, -1552, -1552, -1552,   315,  -516, -1552, -1552, -1552,   822,
   -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552, -1552,
   -1552, -1552, -1552, -1552, -1274,  -733, -1552
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    17,   153,    56,   188,    18,   175,   181,  1605,
    1415,  1524,   739,   539,   159,   540,   105,    20,    21,    47,
      48,    49,    94,    22,    41,    42,   541,   542,  1271,  1272,
     646,   544,  1430,  1539,   545,   546,  1010,   547,   667,   548,
     549,   550,   551,  1203,   691,   182,   183,    37,    38,    39,
     233,    71,    72,    73,    74,    24,   300,   396,   223,    25,
     117,   224,   118,   166,   301,   148,   717,  1037,   554,   397,
     555,   751,   631,   741,  1146,   603,   982,  1522,   984,  1523,
     557,   558,   559,   669,   898,  1490,   560,   561,   562,   563,
     564,   565,   566,   567,   459,   568,   775,  1284,  1020,   569,
     570,   937,  1503,   938,  1504,   940,  1505,   571,   903,  1496,
     572,   752,  1554,   573,  1292,  1293,  1024,   719,   574,   832,
    1012,   575,   683,  1038,   577,   578,   579,  1008,   580,  1266,
    1612,  1267,  1671,   581,  1111,  1466,   582,   753,  1448,  1683,
    1450,  1684,  1561,  1723,   584,   391,  1475,  1571,  1328,  1330,
    1122,   597,  1118,   843,  1640,  1688,   392,   393,   394,   866,
     867,   441,   868,   869,   442,   999,   658,   659,  1644,   611,
     412,   324,   325,   230,   317,    84,   129,    27,   171,   399,
      95,    96,   185,    97,    28,    53,   121,   168,    29,   312,
     595,   592,  1116,   401,   228,   229,    82,   126,   403,    30,
     169,   314,   660,   585,   311,   370,   371,  1135,   643,   243,
     372,   859,  1477,  1143,   853,   438,   373,   612,  1334,   871,
     617,  1339,   613,  1335,   614,  1336,   616,  1338,   620,  1342,
     621,  1479,   622,  1344,   623,  1480,   624,  1346,   625,  1481,
     626,  1348,   627,  1350,   649,    31,   101,   190,   380,   650,
      32,   102,   191,   381,   654,    33,   100,   189,   440,   878,
     586,   757,  1700,   758,   968,  1691,  1692,  1693,   969,   981,
    1246,  1240,  1235,  1409,  1168,   587,   894,  1487,   895,  1488,
     949,  1509,   946,  1507,   970,   742,   588,   947,  1508,   971,
     589,  1174,  1580,  1175,  1581,  1176,  1582,   907,  1500,   944,
    1506,   735,   743,   590,  1661,   991,   591
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      23,   681,   734,   848,   302,   553,   576,   931,   766,   607,
      64,    75,   872,    52,   652,   234,   647,   974,    76,   996,
     893,   682,   776,  1000,  1002,  1017,   712,   720,   721,   598,
    1231,   967,   138,   967,   374,  1137,  1243,  1139,   642,  1141,
    1399,   556,  1214,  1216,  1212,   861,   841,   863,   675,   865,
      90,  1022,   231,   176,   177,  1163,   897,  1223,  1547,    34,
      35,   466,   467,    75,    75,    75,  1241,   962,  -134,   962,
     161,  1670,   632,   754,   799,  1247,   888,   802,   803,   756,
     316,   477,   130,   131,   457,    91,   553,   482,  1276,  1687,
     648,   653,  1254,  1256,  1418,   145,  1482,   150,   634,  1552,
     838,   104,    62,  1287,   369,    75,    75,    75,    75,   802,
     803,   553,   232,  1288,    54,   729,   731,   961,   389,   972,
      50,   976,   556,   231,   497,   498,   389,   458,  1473,   988,
    1715,   231,    63,  1277,   849,   170,    40,  1725,   992,  1149,
      51,   890,    86,   322,   439,   676,   677,   556,   186,  1289,
       2,  1018,   879,   110,   111,   112,  1167,     3,   500,   501,
      55,   404,   830,   831,   323,  1626,   767,   954,  1290,   193,
      83,    57,   754,  1291,   226,  1178,   672,    58,   319,   629,
       4,   390,     5,   232,     6,   779,   780,  1276,  1474,   464,
       7,   232,   318,   238,   830,   831,   244,   245,   630,    62,
       8,  1230,   686,   909,   178,  1517,     9,  1023,  1019,   179,
     768,   180,  1210,    36,   135,  1213,   516,   517,   518,   553,
     239,   687,   908,   910,   146,    13,   889,   160,   889,    63,
      10,    92,   321,   132,   951,   368,   633,   889,   133,   529,
     134,   678,    93,   135,   147,    14,   889,   552,   945,   379,
      65,   948,    11,    12,   668,   556,    43,    44,    45,   369,
     679,   685,   635,   889,   889,   889,   406,  1604,    85,  1375,
    1278,   535,  1374,   242,   369,   839,   703,  -737,   146,    66,
     636,   888,   791,   792,   136,   930,   637,    46,   553,  1396,
     799,  1694,   801,   802,   803,   804,   880,  1025,   147,   754,
     805,   883,  1704,   369,  1391,   369,    62,    77,    78,   950,
      79,  1016,   608,  1280,  1276,    13,  1279,   398,    86,   553,
     402,   888,   609,    67,   556,  1599,  1662,    87,  1663,   888,
      98,   998,  1665,  1666,   888,    14,    63,   104,    80,  1421,
    1281,    13,  1733,  1734,   998,   889,   890,   888,   891,    13,
     888,   892,    68,    50,    15,   556,    88,  1529,  1689,    62,
    1668,    14,   427,    50,  1432,    16,   827,   828,   829,    14,
     777,  1125,  1068,    51,   149,  1340,  1115,   610,   830,   831,
     369,   369,  1257,    51,   225,   103,   890,  1069,  1369,    63,
     368,   460,   462,  1712,   890,   596,   543,    50,   594,   890,
    1021,   104,  1370,    13,   320,   368,   851,   852,   854,    81,
     856,   857,   890,   911,   860,   890,   862,    51,   864,   537,
     716,  1476,  1363,    14,    69,   556,   556,   556,   556,   556,
     556,   556,   556,    70,   368,   109,   368,   911,   556,   556,
    1321,    62,  1166,  1123,   556,   837,   896,   842,  1294,   139,
    1004,   702,  1436,   556,   556,  1005,   553,  1516,   556,   556,
     556,  1275,   556,  1250,  1364,   146,   315,  1519,  1447,   368,
     673,    63,   935,   113,  1255,    13,   369,   369,   369,  1438,
     369,   369,  1224,  1445,   369,   147,   369,    50,   369,    93,
     369,   697,   556,  1161,  1162,    14,  1006,   911,   114,  1237,
     881,   645,  1238,  1131,   884,    13,  1177,    51,   756,  1142,
    1548,   368,   368,  1395,    50,   911,   756,   983,  1145,  1358,
     409,  1232,  1233,   410,  1559,    14,   411,   141,  1405,   967,
    1239,   645,  1398,    13,    51,   439,  1204,  1205,   911,  1207,
     760,   761,  1009,  1209,   967,  1211,   106,   107,   108,  1234,
     369,   779,   780,    14,  1299,  1300,  1301,    86,   769,   428,
     770,   771,   772,   439,   773,  1566,  1512,  1591,    62,  1251,
     911,  1252,   911,    13,   369,   778,  1229,  1004,   144,  1199,
    1546,   593,   405,   411,    13,   154,   429,   430,   162,   163,
     164,   165,   836,    14,    13,   322,   840,  1568,    63,  1597,
    1537,   113,    13,  1749,    14,    13,   847,   368,   368,   368,
     645,   368,   368,  1229,    14,   368,   323,   368,  1144,   368,
     645,   368,    14,  1422,  1229,    14,  1325,    13,  1352,   835,
    1436,   645,   122,   123,  1645,  1646,   155,    13,  1434,  1388,
    1392,   156,   877,  1029,  1033,  1445,   157,    14,   791,   792,
     431,  1755,  1657,  1658,   432,  1437,   799,    14,   801,   802,
     803,   804,    13,   645,    13,  1147,   805,   900,  1229,  1073,
    1446,  1390,   651,  1154,  1229,    13,  1155,   779,   780,   656,
     158,   368,    14,   146,    14,  1159,  1262,  1121,   645,  1310,
     645,  1426,  1164,  1624,  1112,    14,  1412,  1173,  1501,  1637,
     657,  1263,  1151,   147,  1311,   368,  1361,   375,   369,  1197,
     439,  1697,  1698,   167,  1128,  1623,    13,  1351,  1349,   433,
     779,   780,   376,   434,   428,  1215,   435,   377,  1153,   378,
     825,   826,   827,   828,   829,   369,    14,  1543,   235,   236,
      13,   436,   645,   369,   830,   831,   369,   437,   876,  1471,
    1413,   429,   430,   704,  1007,   369,   187,    13,   369,   369,
      14,   439,   369,  1565,    91,  1129,  1394,   369,    13,  1329,
    1011,   369,   705,  1373,   791,   792,  1147,    14,  1147,  1530,
      13,  1315,   799,   645,  1404,   802,   803,   804,    14,   439,
    1411,  1492,   805,  1132,   461,  1353,  1316,  1416,  1379,  1417,
      14,   369,   369,   707,   369,   901,   645,    57,   369,  1268,
     369,   369,    13,    58,  1498,   431,    13,   791,   792,   432,
    1269,  1270,   708,   192,   902,   799,   428,   801,   802,   803,
     804,   110,    14,   594,    75,   805,    14,   754,   645,   368,
     227,  1406,   645,  1511,  1407,  1453,   369,  1408,   369,  1127,
    1499,   240,  1276,   429,   430,  -793,  1541,  1463,   537,   716,
    -793,   115,  1468,  1145,  -800,  1425,   368,   116,   439,  -800,
     830,   831,  1133,   119,   368,  1152,  -807,   368,  -793,   120,
    -443,  -807,   553,   576,   433,  -443,   368,  -800,   434,   368,
     368,   435,   439,   368,  1244,  1237,  1136,  1245,   368,  -807,
    1353,  1353,   368,  -443,   124,  1655,   436,   439,   241,  1515,
     125,  1138,   437,   830,   831,   911,   127,   431,   556,   242,
     608,   432,   128,  1703,   313,  1337,  1625,   303,   151,   411,
     609,   304,   368,   368,   152,   368,  1514,   297,  1326,   368,
     298,   368,   368,  1628,  1327,   305,   306,  1690,  1690,   382,
     307,   308,   309,   310,   299,  1699,  1518,  1308,  1728,  1690,
    1699,  1528,   911,  1521,  1526,  1654,  1322,  1527,  1608,   911,
     911,  1669,   439,   911,   911,   385,  1140,   368,   383,   368,
     439,   142,   143,   439,  1362,   610,   433,  1372,   384,   386,
     434,  1726,   387,   435,   439,   369,  1380,   439,  1633,  1690,
    1690,  1685,   110,   111,   112,  1602,   388,   369,   436,  1510,
    1606,   537,   716,   110,   437,   112,  1493,   172,   173,   985,
     986,   395,  1678,  1679,  1145,   728,    43,    44,    45,   400,
     407,   327,   428,   172,   173,   174,   408,   328,   369,   369,
     428,   413,   414,   329,   235,   236,   237,  1555,  1427,   978,
     979,   980,  1754,   330,   416,   661,   662,   663,   415,   429,
     430,   331,   417,   369,   418,   419,   369,   429,   430,    59,
      60,    61,   421,  1717,   422,   423,   332,   424,   425,   426,
     439,   443,   605,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,   361,   362,   363,   364,   365,  1323,   456,   606,   463,
     599,  1331,   618,   431,  1431,  1333,   368,   432,   619,   640,
     369,   431,  1656,   664,  1341,   432,   665,   674,   368,   428,
     690,  1489,   693,   696,   428,  1627,   694,  1483,   695,   698,
     699,  1680,   706,   709,   710,    62,   759,   722,   682,   779,
     780,   723,   738,   724,    13,   763,   429,   430,   366,   368,
     368,   429,   430,   764,   725,   726,   727,    16,   774,   844,
     833,   736,   702,   846,    14,    63,   858,   651,   873,   882,
     645,   886,   433,   887,   368,   899,   434,   368,  1198,   435,
     433,  1718,  1720,   904,   434,   777,  1332,   435,  1538,   905,
     428,   906,   911,   929,   436,   910,   934,   369,   936,   941,
     437,   942,   436,   953,  1677,   989,   990,  1553,   437,   993,
     431,   367,   994,   608,   432,   431,   553,   429,   430,   432,
    1744,   995,  1001,   609,  1557,   997,  1120,  1003,  1064,  1124,
    1134,  1130,  1148,  1157,   789,   790,   791,   792,  1158,  1171,
    1200,   368,  1202,  1569,   799,  1229,   801,   802,   803,   804,
    1236,    13,   556,   543,   805,  1249,   807,   808,   444,   445,
     446,   447,   448,   449,   450,   451,   553,   553,  1283,  1261,
    1282,    14,  1553,  1295,  1318,  1264,  1297,  1324,   610,   433,
    1265,   431,  1329,   434,   433,   432,   435,   452,   434,  1286,
    1359,   435,   369,  1296,   369,  1343,  1298,   453,   454,   455,
    1302,   436,   556,   556,  1470,   553,   436,   437,  1345,  1347,
    1303,  1313,   437,   369,  1314,  1319,  1367,  1354,   825,   826,
     827,   828,   829,  1357,  1365,  1366,  1377,  1400,   368,   583,
    1378,  1393,   830,   831,  1484,  1634,  1440,  1386,  1420,   604,
    1389,   556,  1403,  1410,  1441,  1443,  1433,  1444,   615,  1442,
     433,  1455,   428,  1456,   434,  1458,  1360,   435,   628,  1464,
    1469,  1478,   630,  1485,   428,  1491,  1494,  1520,   639,  1495,
    1525,  1544,   436,  1542,  1545,  1429,  1549,  1551,   437,   429,
     430,  1570,  1510,  1675,   779,   780,  1574,  1575,   655,  1577,
     369,   429,   430,  1578,  1583,  1590,  1584,  1585,  1589,  1592,
     666,  1593,  1649,  1613,  1629,   369,  1648,  1660,  1651,  1540,
     684,  1664,  1652,  1706,  1653,   689,  1707,  1676,   692,  1686,
     718,   718,   718,   368,  1701,   368,  1702,   701,  1708,  1709,
    1710,  1727,  1729,  1711,  1742,   713,   714,   715,  1730,  1735,
     987,  1736,  1737,   431,   368,  1738,  1740,   432,  1741,  1743,
     733,   137,  1750,    19,  1751,   431,  1752,    89,   737,   432,
     140,   733,   744,   745,   746,   747,   748,   184,  1614,   369,
     765,  1617,  1119,  1274,  1618,   326,   762,    26,  1285,   789,
     790,   791,   792,   793,  1619,  1748,   796,   797,   798,   799,
    1596,   801,   802,   803,   804,  1550,  1620,   765,  1621,   805,
    1572,   807,   808,  1439,  1573,  1641,  1472,   811,   812,   813,
      99,   428,   433,   817,   834,  1642,   434,  1643,  1376,   435,
     670,   368,   428,   671,   433,   420,  1705,     0,   434,  1601,
    1382,   435,  1402,   975,   436,     0,   368,     0,   429,   430,
     437,     0,     0,   850,     0,     0,   436,     0,  1639,   429,
     430,     0,   437,     0,     0,     0,   819,     0,   820,   821,
     822,   823,   824,   825,   826,   827,   828,   829,     0,     0,
       0,     0,   875,     0,     0,     0,     0,   830,   831,     0,
       0,     0,     0,     0,     0,     0,     0,  1007,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   765,     0,
     368,     0,   431,     0,     0,     0,   432,     0,     0,     0,
     428,     0,   684,   431,     0,     0,     0,   432,     0,   733,
     932,     0,   933,     0,     0,     0,     0,   765,   939,     0,
       0,     0,     0,     0,   943,     0,     0,   429,   430,     0,
       0,     0,     0,   952,     0,     0,  1007,     0,     0,     0,
       0,     0,     0,     0,   955,   956,   957,   958,   959,   960,
    1722,   966,     0,   966,     0,     0,  1724,     0,     0,     0,
       0,   433,     0,     0,     0,   434,     0,  1383,   435,     0,
       0,     0,   433,     0,     0,     0,   434,     0,  1384,   435,
       0,   718,     0,   436,     0,     0,     0,     0,  1746,   437,
    1747,   431,     0,     0,   436,   432,   779,   780,     0,     0,
     437,     0,     0,  1039,  1041,  1043,  1045,  1047,  1049,  1051,
    1053,  1054,  1055,     0,  1753,  1056,  1058,  1060,  1061,  1062,
    1063,     0,  1066,  1067,     0,  1070,  1071,  1072,  1074,  1075,
    1076,  1078,  1080,  1081,  1082,  1083,  1085,  1087,  1089,  1090,
    1092,  1094,  1095,  1096,  1097,  1098,  1099,  1100,  1101,  1102,
    1103,  1104,     0,  1113,   765,     0,     0,  1117,     0,     0,
     433,     0,   428,     0,   434,     0,  1385,   435,     0,     0,
    1126,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,   437,   429,
     430,   789,   790,   791,   792,   793,     0,     0,   796,   797,
     798,   799,     0,   801,   802,   803,   804,     0,  1156,     0,
       0,   805,     0,   807,   808,  1160,     0,     0,     0,   811,
     812,   813,  1170,     0,  1172,   817,     0,     0,     0,     0,
       0,     0,     0,  1179,  1180,  1181,  1182,  1183,  1184,  1185,
    1186,  1187,  1188,  1189,  1190,  1191,  1192,  1193,  1194,  1195,
    1196,     0,     0,   431,     0,     0,   765,   432,     0,     0,
       0,     0,     0,     0,     0,     0,   718,     0,   819,     0,
     820,   821,   822,   823,   824,   825,   826,   827,   828,   829,
       0,     0,     0,     0,     0,   744,  1226,     0,     0,   830,
     831,     0,     0,     0,   537,   716,     0,     0,     0,  1248,
       0,   733,     0,     0,     0,     0,     0,     0,  1253,     0,
       0,     0,   733,     0,     0,     0,     0,  1179,     0,  1258,
    1259,  1260,   433,     0,     0,     0,   434,     0,  1387,   435,
       0,     0,  1273,     0,     0,     0,     0,     0,   718,   718,
     718,   779,   780,   765,   436,   765,     0,   765,     0,   765,
     437,   765,     0,   765,     0,   765,     0,   765,     0,     0,
       0,     0,   765,     0,   765,     0,     0,     0,     0,     0,
     765,     0,     0,     0,     0,     0,     0,     0,     0,   428,
       0,     0,   765,     0,   765,     0,     0,     0,     0,   765,
       0,   765,     0,   765,     0,     0,   765,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   429,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   730,     0,     0,
       0,     0,     0,   327,     0,     0,     0,     0,  1312,   328,
     765,     0,  1317,     0,     0,   329,   789,   790,   791,   792,
     793,     0,     0,   796,     0,   330,   799,     0,   801,   802,
     803,   804,     0,   331,     0,     0,   805,     0,   807,   808,
       0,     0,     0,     0,     0,     0,     0,     0,   332,     0,
     431,     0,     0,  1355,   432,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,   365,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   823,   824,
     825,   826,   827,   828,   829,  1381,     0,     0,     0,     0,
       0,     0,     0,     0,   830,   831,     0,     0,     0,   433,
       0,     0,     0,   434,     0,  1497,   435,    62,     0,     0,
    1397,     0,     0,     0,     0,     0,     0,     0,  1401,   966,
     366,   436,     0,     0,     0,     0,     0,   437,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,   779,
     780,     0,     0,     0,     0,   327,  1423,  1424,     0,     0,
       0,   328,     0,  1428,     0,     0,   583,   329,     0,     0,
       0,     0,  1179,     0,     0,     0,     0,   330,     0,     0,
       0,     0,     0,     0,     0,   331,   779,   780,  1449,     0,
    1451,     0,     0,   367,     0,  1454,     0,     0,     0,     0,
     332,  1457,     0,     0,     0,  1460,     0,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
       0,     0,     0,     0,   789,   790,   791,   792,   793,     0,
    1486,   796,   797,   798,   799,     0,   801,   802,   803,   804,
       0,     0,     0,     0,   805,   765,   807,   808,     0,     0,
     733,     0,   811,   812,   813,     0,     0,     0,   817,    62,
       0,   789,   790,   791,   792,   793,     0,     0,   796,   797,
     798,   799,   366,   801,   802,   803,   804,     0,     0,     0,
       0,   805,     0,   807,   808,     0,     0,     0,     0,    63,
       0,     0,  1531,  1532,  1533,     0,     0,     0,     0,     0,
       0,   819,     0,   820,   821,   822,   823,   824,   825,   826,
     827,   828,   829,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   830,   831,     0,     0,   845,     0,  1562,     0,
    1563,   779,   780,     0,     0,   367,  1567,   641,     0,     0,
     820,   821,   822,   823,   824,   825,   826,   827,   828,   829,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   830,
     831,     0,     0,     0,  1576,     0,     0,  1579,     0,     0,
       0,     0,     0,     0,     0,  1586,  1587,  1588,     0,     0,
       0,     0,  1595,     0,     0,     0,     0,  1598,     0,     0,
    1600,     0,     0,   733,  1603,   428,     0,     0,   733,  1607,
       0,  1609,  1610,     0,     0,     0,     0,     0,     0,     0,
       0,  1622,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   429,   430,  1631,     0,   789,   790,   791,   792,
     793,     0,     0,   796,   797,   798,   799,     0,   801,   802,
     803,   804,     0,     0,     0,     0,   805,     0,   807,   808,
       0,     0,     0,     0,   811,   812,   813,   733,     0,     0,
     817,     0,     0,     0,     0,  1040,  1042,  1044,  1046,  1048,
    1050,  1052,     0,     0,     0,     0,     0,  1057,  1059,     0,
    1667,     0,     0,  1065,     0,     0,   431,     0,  1674,     0,
     432,     0,  1077,  1079,     0,     0,     0,  1084,  1086,  1088,
    1681,  1091,  1682,   819,     0,   820,   821,   822,   823,   824,
     825,   826,   827,   828,   829,     0,     0,     0,     0,     0,
    1695,  1696,     0,     0,   830,   831,     0,     0,   855,     0,
       0,     0,     0,     0,     0,   765,     0,   428,     0,     0,
    1713,  1714,     0,     0,     0,  1716,     0,     0,     0,     0,
    1719,  1721,     0,     0,     0,   433,     0,     0,     0,   434,
       0,  1502,   435,     0,   429,   430,     0,     0,     0,     0,
       0,     0,     0,   765,     0,   765,   465,   436,  1739,     0,
     466,   467,     3,   437,   468,   469,   470,     0,   471,  1745,
     472,   473,   474,   475,   476,     0,     0,     0,     0,   765,
     477,   478,   479,   480,   481,     0,   482,     0,     0,     0,
       0,     0,     0,   483,   484,   485,     0,   486,     0,   487,
     488,     0,     0,   489,     0,     8,   490,   491,   431,   492,
     493,     0,   432,   494,   495,     0,     0,     0,     0,     0,
     496,     0,     0,   497,   498,     0,   333,   334,   335,     0,
     337,   338,   339,   340,   341,   499,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,     0,   355,   356,
     357,     0,     0,   360,   361,   362,   363,   500,   501,   502,
     503,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   504,   505,     0,     0,   433,     0,     0,
       0,   434,     0,  1536,   435,     0,     0,     0,   506,   507,
     508,     0,     0,     0,     0,     0,     0,     0,    62,   436,
       0,     0,     0,     0,     0,   437,   509,   510,   511,   512,
     513,     0,   514,     0,   515,   516,   517,   518,     0,   146,
      13,   519,   520,   521,   522,   523,   524,   525,    63,   526,
     527,   528,     0,     0,     0,     0,     0,     0,   529,   147,
      14,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   530,   531,   532,     0,    15,
       0,     0,   533,   534,     0,     0,   466,   467,     0,     0,
     535,     0,   536,     0,   537,   538,   472,   473,   474,   475,
     476,     0,     0,     0,     0,     0,   477,     0,   479,     0,
       0,     0,   482,     0,   428,     0,     0,     0,     0,     0,
     484,     0,     0,     0,     0,     0,   488,     0,     0,   489,
       0,     0,   490,     0,   962,     0,   493,     0,     0,     0,
       0,   429,   430,     0,     0,     0,   600,     0,     0,   497,
     498,     0,   333,   334,   335,     0,   337,   338,   339,   340,
     341,   499,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,     0,   355,   356,   357,     0,     0,   360,
     361,   362,   363,   500,   501,   601,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   504,
     505,     0,     0,     0,     0,   431,     0,     0,     0,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   509,   510,   511,   512,   513,     0,   514,   754,
     515,   516,   517,   518,     0,     0,     0,   519,   520,   521,
     522,   523,   524,   525,   755,   602,   527,   528,     0,     0,
       0,     0,     0,     0,   529,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,     0,     0,     0,   434,     0,
       0,   963,   531,   532,     0,    15,     0,     0,   533,   534,
       0,     0,     0,   466,   467,     0,   964,     0,   965,     0,
     537,   538,   437,   472,   473,   474,   475,   476,     0,     0,
       0,     0,     0,   477,     0,   479,     0,     0,     0,   482,
       0,   428,     0,     0,     0,     0,     0,   484,     0,     0,
       0,     0,     0,   488,     0,     0,   489,     0,     0,   490,
       0,     0,     0,   493,     0,     0,     0,     0,   429,   430,
       0,     0,     0,   600,     0,     0,   497,   498,     0,   333,
     334,   335,     0,   337,   338,   339,   340,   341,   499,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
       0,   355,   356,   357,     0,     0,   360,   361,   362,   363,
     500,   501,   601,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   504,   505,     0,     0,
       0,     0,   431,     0,     0,     0,   432,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   509,
     510,   511,   512,   513,     0,   514,   754,   515,   516,   517,
     518,     0,     0,     0,   519,   520,   521,   522,   523,   524,
     525,   755,   602,   527,   528,     0,     0,     0,     0,     0,
       0,   529,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,     0,     0,     0,   434,     0,     0,   963,   531,
     532,     0,    15,     0,     0,   533,   534,     0,     0,     0,
     466,   467,     0,   964,     0,   973,     0,   537,   538,   437,
     472,   473,   474,   475,   476,     0,     0,     0,     0,     0,
     477,     0,   479,     0,     0,     0,   482,     0,   634,     0,
       0,     0,     0,     0,   484,     0,     0,     0,     0,     0,
     488,     0,     0,   489,     0,     0,   490,     0,     0,     0,
     493,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     600,     0,     0,   497,   498,     0,   333,   334,   335,     0,
     337,   338,   339,   340,   341,   499,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,     0,   355,   356,
     357,     0,     0,   360,   361,   362,   363,   500,   501,   601,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   504,   505,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   509,   510,   511,   512,
     513,     0,   514,     0,   515,   516,   517,   518,     0,     0,
       0,   519,   520,   521,   522,   523,   524,   525,    63,   602,
     527,   528,     0,     0,     0,     0,     0,     0,   529,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   635,     0,     0,   530,   531,   532,     0,    15,
       0,     0,   533,   534,     0,     0,     0,   466,   467,     0,
    1225,     0,   536,     0,   537,   538,   637,   472,   473,   474,
     475,   476,     0,     0,     0,     0,     0,   477,     0,   479,
       0,     0,     0,   482,     0,     0,     0,     0,     0,     0,
       0,   484,     0,     0,     0,     0,     0,   488,     0,     0,
     489,     0,     0,   490,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   600,     0,     0,
     497,   498,     0,   333,   334,   335,     0,   337,   338,   339,
     340,   341,   499,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,     0,   355,   356,   357,     0,     0,
     360,   361,   362,   363,   500,   501,   502,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     504,   505,     0,     0,     0,     0,     0,     0,     0,   680,
       0,     0,     0,     0,     0,   506,   507,   508,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   509,   510,   511,   512,   513,     0,   514,
       0,   515,   516,   517,   518,     0,     0,     0,   519,   520,
     521,   522,   523,   524,   525,    63,   602,   527,   528,     0,
       0,     0,     0,     0,     0,   529,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   530,   531,   532,     0,    15,     0,     0,   533,
     534,     0,     0,   466,   467,     0,     0,   535,     0,   536,
       0,   537,   538,   472,   473,   474,   475,   476,     0,     0,
       0,     0,     0,   477,     0,   479,     0,     0,     0,   482,
       0,     0,     0,     0,     0,     0,     0,   484,     0,     0,
       0,     0,     0,   488,     0,     0,   489,     0,     0,   490,
       0,     0,     0,   493,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   600,     0,     0,   497,   498,     0,   333,
     334,   335,     0,   337,   338,   339,   340,   341,   499,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
       0,   355,   356,   357,     0,     0,   360,   361,   362,   363,
     500,   501,   502,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   504,   505,     0,     0,
       0,     0,     0,     0,     0,   711,     0,     0,     0,     0,
       0,   506,   507,   508,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   509,
     510,   511,   512,   513,     0,   514,     0,   515,   516,   517,
     518,     0,     0,     0,   519,   520,   521,   522,   523,   524,
     525,    63,   602,   527,   528,     0,     0,     0,     0,     0,
       0,   529,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   530,   531,
     532,     0,    15,     0,     0,   533,   534,     0,     0,   466,
     467,     0,     0,   535,     0,   536,     0,   537,   538,   472,
     473,   474,   475,   476,     0,     0,     0,     0,     0,   477,
    1615,   479,   480,     0,     0,   482,     0,     0,     0,     0,
       0,     0,     0,   484,     0,     0,     0,     0,     0,   488,
       0,     0,   489,     0,     0,   490,   491,     0,     0,   493,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   600,
       0,     0,   497,   498,     0,   333,   334,   335,     0,   337,
     338,   339,   340,   341,   499,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,     0,   355,   356,   357,
       0,     0,   360,   361,   362,   363,   500,   501,   601,  1616,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   504,   505,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   509,   510,   511,   512,   513,
       0,   514,     0,   515,   516,   517,   518,     0,     0,     0,
     519,   520,   521,   522,   523,   524,   525,    63,   602,   527,
     528,     0,     0,     0,     0,     0,     0,   529,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   530,   531,   532,     0,    15,     0,
       0,   533,   534,     0,     0,   466,   467,     0,     0,   535,
       0,   536,     0,   537,   538,   472,   473,   474,   475,   476,
       0,     0,     0,     0,     0,   477,     0,   479,     0,     0,
       0,   482,     0,     0,     0,     0,     0,     0,     0,   484,
       0,     0,     0,     0,     0,   488,     0,     0,   489,     0,
       0,   490,     0,     0,     0,   493,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   600,     0,     0,   497,   498,
       0,   333,   334,   335,     0,   337,   338,   339,   340,   341,
     499,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,   353,     0,   355,   356,   357,     0,     0,   360,   361,
     362,   363,   500,   501,   502,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   504,   505,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   506,   507,   508,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   509,   510,   511,   512,   513,     0,   514,     0,   515,
     516,   517,   518,     0,     0,     0,   519,   520,   521,   522,
     523,   524,   525,    63,   602,   527,   528,     0,     0,     0,
       0,     0,     0,   529,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     530,   531,   532,     0,    15,     0,     0,   533,   534,     0,
       0,   466,   467,     0,     0,   535,     0,   536,     0,   537,
     538,   472,   473,   474,   475,   476,     0,     0,     0,     0,
       0,   477,     0,   479,     0,     0,     0,   482,     0,     0,
       0,     0,     0,     0,     0,   484,     0,     0,     0,     0,
       0,   488,     0,     0,   489,     0,     0,   490,     0,     0,
       0,   493,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   600,     0,     0,   497,   498,  1013,   333,   334,   335,
       0,   337,   338,   339,   340,   341,   499,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,     0,   355,
     356,   357,     0,     0,   360,   361,   362,   363,   500,   501,
     601,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   504,   505,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   509,   510,   511,
     512,   513,     0,   514,   754,   515,   516,   517,   518,     0,
       0,     0,   519,   520,   521,   522,   523,   524,   525,   755,
     602,   527,   528,     0,     0,     0,     0,     0,     0,   529,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   530,   531,   532,     0,
      15,     0,     0,   533,   534,     0,     0,   466,   467,     0,
       0,  1014,     0,   536,  1015,   537,   538,   472,   473,   474,
     475,   476,     0,     0,     0,     0,     0,   477,     0,   479,
       0,     0,   428,   482,     0,     0,     0,     0,     0,     0,
       0,   484,     0,     0,     0,     0,     0,   488,     0,     0,
     489,     0,     0,   490,     0,     0,     0,   493,     0,   429,
     430,     0,     0,     0,     0,     0,     0,   600,     0,     0,
     497,   498,     0,   333,   334,   335,     0,   337,   338,   339,
     340,   341,   499,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,     0,   355,   356,   357,     0,     0,
     360,   361,   362,   363,   500,   501,   502,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     504,   505,     0,   431,     0,     0,     0,   432,     0,     0,
       0,     0,     0,     0,     0,  1034,  1035,  1036,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   509,   510,   511,   512,   513,     0,   514,
       0,   515,   516,   517,   518,     0,     0,     0,   519,   520,
     521,   522,   523,   524,   525,    63,   602,   527,   528,     0,
       0,     0,     0,     0,     0,   529,     0,     0,     0,     0,
       0,     0,   433,     0,     0,     0,   434,     0,  1632,   435,
       0,     0,   530,   531,   532,     0,    15,     0,     0,   533,
     534,     0,     0,     0,   436,   466,   467,   535,     0,   536,
     437,   537,   538,   749,     0,   472,   473,   474,   475,   476,
       0,     0,     0,     0,     0,   477,     0,   479,     0,     0,
       0,   482,     0,     0,     0,     0,     0,     0,     0,   484,
       0,     0,     0,     0,     0,   488,     0,     0,   489,   750,
       0,   490,     0,     0,     0,   493,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   600,     0,     0,   497,   498,
       0,   333,   334,   335,     0,   337,   338,   339,   340,   341,
     499,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,   353,     0,   355,   356,   357,     0,     0,   360,   361,
     362,   363,   500,   501,   601,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   504,   505,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   509,   510,   511,   512,   513,     0,   514,     0,   515,
     516,   517,   518,     0,     0,     0,   519,   520,   521,   522,
     523,   524,   525,    63,   602,   527,   528,     0,     0,     0,
       0,     0,     0,   529,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     530,   531,   532,     0,    15,     0,     0,   533,   534,     0,
       0,     0,     0,   466,   467,   535,   638,   536,     0,   537,
     538,   749,     0,   472,   473,   474,   475,   476,     0,     0,
       0,     0,     0,   477,     0,   479,     0,     0,     0,   482,
       0,     0,     0,     0,     0,     0,     0,   484,     0,     0,
       0,     0,     0,   488,     0,     0,   489,   750,     0,   490,
       0,     0,     0,   493,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   600,     0,     0,   497,   498,     0,   333,
     334,   335,     0,   337,   338,   339,   340,   341,   499,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
       0,   355,   356,   357,     0,     0,   360,   361,   362,   363,
     500,   501,   601,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   504,   505,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   509,
     510,   511,   512,   513,     0,   514,   754,   515,   516,   517,
     518,     0,     0,     0,   519,   520,   521,   522,   523,   524,
     525,   755,   602,   527,   528,     0,     0,     0,     0,     0,
       0,   529,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   530,   531,
     532,     0,    15,     0,     0,   533,   534,     0,     0,     0,
       0,   466,   467,   535,     0,   536,     0,   537,   538,   749,
       0,   472,   473,   474,   475,   476,     0,     0,     0,     0,
       0,   477,     0,   479,     0,     0,     0,   482,     0,     0,
       0,     0,     0,     0,     0,   484,     0,     0,     0,     0,
       0,   488,     0,     0,   489,   750,     0,   490,     0,     0,
       0,   493,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   600,     0,     0,   497,   498,     0,   333,   334,   335,
       0,   337,   338,   339,   340,   341,   499,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,     0,   355,
     356,   357,     0,     0,   360,   361,   362,   363,   500,   501,
     601,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   504,   505,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   509,   510,   511,
     512,   513,     0,   514,     0,   515,   516,   517,   518,     0,
       0,     0,   519,   520,   521,   522,   523,   524,   525,    63,
     602,   527,   528,     0,     0,     0,     0,     0,     0,   529,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   530,   531,   532,     0,
      15,     0,     0,   533,   534,     0,     0,     0,     0,   466,
     467,   535,   873,   536,     0,   537,   538,   749,     0,   472,
     473,   474,   475,   476,     0,     0,     0,     0,     0,   477,
       0,   479,     0,     0,     0,   482,     0,     0,     0,     0,
       0,     0,     0,   484,     0,     0,     0,     0,     0,   488,
       0,     0,   489,   750,     0,   490,     0,     0,     0,   493,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   600,
       0,     0,   497,   498,     0,   333,   334,   335,     0,   337,
     338,   339,   340,   341,   499,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,     0,   355,   356,   357,
       0,     0,   360,   361,   362,   363,   500,   501,   601,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   504,   505,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   509,   510,   511,   512,   513,
       0,   514,     0,   515,   516,   517,   518,     0,     0,     0,
     519,   520,   521,   522,   523,   524,   525,    63,   602,   527,
     528,     0,     0,     0,     0,     0,     0,   529,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   530,   531,   532,     0,    15,     0,
       0,   533,   534,     0,     0,   466,   467,     0,     0,   535,
       0,   536,     0,   537,   538,   472,   473,   474,   475,   476,
       0,     0,     0,     0,     0,   477,     0,   479,     0,     0,
       0,   482,     0,     0,     0,     0,     0,     0,     0,   484,
       0,     0,     0,     0,     0,   488,     0,     0,   489,     0,
       0,   490,     0,     0,     0,   493,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   600,     0,     0,   497,   498,
    1165,   333,   334,   335,     0,   337,   338,   339,   340,   341,
     499,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,   353,     0,   355,   356,   357,     0,     0,   360,   361,
     362,   363,   500,   501,   601,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   504,   505,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   509,   510,   511,   512,   513,     0,   514,   754,   515,
     516,   517,   518,     0,     0,     0,   519,   520,   521,   522,
     523,   524,   525,   755,   602,   527,   528,     0,     0,     0,
       0,     0,     0,   529,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     530,   531,   532,     0,    15,     0,     0,   533,   534,     0,
       0,   466,   467,     0,     0,   535,     0,   536,     0,   537,
     538,   472,   473,   474,   475,   476,     0,     0,     0,     0,
       0,   477,     0,   479,     0,     0,     0,   482,     0,     0,
       0,     0,     0,     0,     0,   484,     0,     0,     0,     0,
       0,   488,     0,     0,   489,     0,     0,   490,     0,     0,
       0,   493,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   600,     0,     0,   497,   498,     0,   333,   334,   335,
       0,   337,   338,   339,   340,   341,   499,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,     0,   355,
     356,   357,     0,     0,   360,   361,   362,   363,   500,   501,
     601,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   504,   505,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   509,   510,   511,
     512,   513,     0,   514,   754,   515,   516,   517,   518,     0,
       0,     0,   519,   520,   521,   522,   523,   524,   525,   755,
     602,   527,   528,     0,     0,     0,     0,     0,     0,   529,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   530,   531,   532,     0,
      15,     0,     0,   533,   534,     0,     0,   466,   467,     0,
       0,   535,     0,   536,  1227,   537,   538,   472,   473,   474,
     475,   476,     0,     0,     0,     0,     0,   477,     0,   479,
       0,     0,     0,   482,     0,     0,     0,     0,     0,     0,
       0,   484,     0,     0,     0,     0,     0,   488,     0,     0,
     489,     0,     0,   490,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   600,     0,     0,
     497,   498,     0,   333,   334,   335,     0,   337,   338,   339,
     340,   341,   499,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,     0,   355,   356,   357,     0,     0,
     360,   361,   362,   363,   500,   501,   601,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     504,   505,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   509,   510,   511,   512,   513,     0,   514,
     754,   515,   516,   517,   518,     0,     0,     0,   519,   520,
     521,   522,   523,   524,   525,   755,   602,   527,   528,     0,
       0,     0,     0,     0,     0,   529,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   530,   531,   532,     0,    15,     0,     0,   533,
     534,     0,     0,   466,   467,     0,     0,   535,     0,   536,
    1242,   537,   538,   472,   473,   474,   475,   476,     0,     0,
       0,     0,     0,   477,     0,   479,     0,     0,     0,   482,
       0,     0,     0,     0,     0,     0,     0,   484,     0,     0,
       0,     0,     0,   488,     0,     0,   489,     0,     0,   490,
       0,     0,     0,   493,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   600,     0,     0,   497,   498,     0,   333,
     334,   335,     0,   337,   338,   339,   340,   341,   499,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
       0,   355,   356,   357,     0,     0,   360,   361,   362,   363,
     500,   501,   601,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   504,   505,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   509,
     510,   511,   512,   513,     0,   514,     0,   515,   516,   517,
     518,     0,     0,     0,   519,   520,   521,   522,   523,   524,
     525,    63,   602,   527,   528,     0,     0,     0,     0,     0,
       0,   529,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   530,   531,
     532,     0,    15,     0,     0,   533,   534,     0,     0,   466,
     467,     0,     0,   535,   638,   536,     0,   537,   538,   472,
     473,   474,   475,   476,     0,     0,     0,     0,     0,   477,
       0,   479,     0,     0,     0,   482,     0,     0,     0,     0,
       0,     0,     0,   484,     0,     0,     0,     0,     0,   488,
       0,     0,   489,     0,     0,   490,     0,     0,     0,   493,
       0,     0,     0,     0,     0,   688,     0,     0,     0,   600,
       0,     0,   497,   498,     0,   333,   334,   335,     0,   337,
     338,   339,   340,   341,   499,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,     0,   355,   356,   357,
       0,     0,   360,   361,   362,   363,   500,   501,   601,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   504,   505,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   509,   510,   511,   512,   513,
       0,   514,     0,   515,   516,   517,   518,     0,     0,     0,
     519,   520,   521,   522,   523,   524,   525,    63,   602,   527,
     528,     0,     0,     0,     0,     0,     0,   529,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   530,   531,   532,     0,    15,     0,
       0,   533,   534,     0,     0,   466,   467,     0,     0,   535,
       0,   536,     0,   537,   538,   472,   473,   474,   475,   476,
       0,     0,     0,     0,     0,   477,     0,   479,     0,     0,
       0,   482,     0,     0,     0,     0,     0,     0,     0,   484,
       0,     0,     0,     0,     0,   488,     0,     0,   489,     0,
       0,   490,     0,     0,     0,   493,     0,     0,   700,     0,
       0,     0,     0,     0,     0,   600,     0,     0,   497,   498,
       0,   333,   334,   335,     0,   337,   338,   339,   340,   341,
     499,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,   353,     0,   355,   356,   357,     0,     0,   360,   361,
     362,   363,   500,   501,   601,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   504,   505,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   509,   510,   511,   512,   513,     0,   514,     0,   515,
     516,   517,   518,     0,     0,     0,   519,   520,   521,   522,
     523,   524,   525,    63,   602,   527,   528,     0,     0,     0,
       0,     0,     0,   529,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     530,   531,   532,     0,    15,     0,     0,   533,   534,     0,
       0,     0,     0,   466,   467,   535,     0,   536,     0,   537,
     538,   732,     0,   472,   473,   474,   475,   476,     0,     0,
       0,     0,     0,   477,     0,   479,     0,     0,     0,   482,
       0,     0,     0,     0,     0,     0,     0,   484,     0,     0,
       0,     0,     0,   488,     0,     0,   489,     0,     0,   490,
       0,     0,     0,   493,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   600,     0,     0,   497,   498,     0,   333,
     334,   335,     0,   337,   338,   339,   340,   341,   499,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
       0,   355,   356,   357,     0,     0,   360,   361,   362,   363,
     500,   501,   601,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   504,   505,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   509,
     510,   511,   512,   513,     0,   514,     0,   515,   516,   517,
     518,     0,     0,     0,   519,   520,   521,   522,   523,   524,
     525,    63,   602,   527,   528,     0,     0,     0,     0,     0,
       0,   529,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   530,   531,
     532,     0,    15,     0,     0,   533,   534,     0,     0,   466,
     467,     0,     0,   535,     0,   536,     0,   537,   538,   472,
     473,   474,   475,   476,     0,     0,     0,     0,     0,   477,
       0,   479,     0,     0,     0,   482,     0,     0,     0,     0,
       0,     0,     0,   484,     0,     0,     0,     0,     0,   488,
       0,     0,   489,     0,     0,   490,     0,     0,     0,   493,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   600,
       0,     0,   497,   498,     0,   333,   334,   335,     0,   337,
     338,   339,   340,   341,   499,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,     0,   355,   356,   357,
       0,     0,   360,   361,   362,   363,   500,   501,   601,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   504,   505,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   509,   510,   511,   512,   513,
       0,   514,     0,   515,   516,   517,   518,     0,     0,     0,
     519,   520,   521,   522,   523,   524,   525,    63,   602,   527,
     528,     0,     0,     0,     0,     0,     0,   529,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   736,     0,   530,   531,   532,     0,    15,     0,
       0,   533,   534,     0,     0,     0,     0,   466,   467,   535,
       0,   536,     0,   537,   538,   740,     0,   472,   473,   474,
     475,   476,     0,     0,     0,     0,     0,   477,     0,   479,
       0,     0,     0,   482,     0,     0,     0,     0,     0,     0,
       0,   484,     0,     0,     0,     0,     0,   488,     0,     0,
     489,     0,     0,   490,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   600,     0,     0,
     497,   498,     0,   333,   334,   335,     0,   337,   338,   339,
     340,   341,   499,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,     0,   355,   356,   357,     0,     0,
     360,   361,   362,   363,   500,   501,   601,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     504,   505,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   509,   510,   511,   512,   513,     0,   514,
       0,   515,   516,   517,   518,     0,     0,     0,   519,   520,
     521,   522,   523,   524,   525,    63,   602,   527,   528,     0,
       0,     0,     0,     0,     0,   529,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   530,   531,   532,     0,    15,     0,     0,   533,
     534,     0,     0,   466,   467,     0,     0,   535,     0,   536,
       0,   537,   538,   472,   473,   474,   475,   476,     0,     0,
       0,     0,     0,   477,     0,   479,     0,     0,     0,   482,
       0,     0,     0,     0,     0,     0,     0,   484,     0,     0,
       0,     0,     0,   488,     0,     0,   489,     0,     0,   490,
       0,     0,     0,   493,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   600,     0,     0,   497,   498,     0,   333,
     334,   335,     0,   337,   338,   339,   340,   341,   499,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
       0,   355,   356,   357,     0,     0,   360,   361,   362,   363,
     500,   501,   601,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   504,   505,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   509,
     510,   511,   512,   513,     0,   514,   754,   515,   516,   517,
     518,     0,     0,     0,   519,   520,   521,   522,   523,   524,
     525,   755,   602,   527,   528,     0,     0,     0,     0,     0,
       0,   529,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   530,   531,
     532,     0,    15,     0,     0,   533,   534,     0,     0,   466,
     467,     0,     0,   535,     0,   536,     0,   537,   538,   472,
     473,   474,   475,   476,     0,     0,  1093,     0,     0,   477,
       0,   479,     0,     0,     0,   482,     0,     0,     0,     0,
       0,     0,     0,   484,     0,     0,     0,     0,     0,   488,
       0,     0,   489,     0,     0,   490,     0,     0,     0,   493,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   600,
       0,     0,   497,   498,     0,   333,   334,   335,     0,   337,
     338,   339,   340,   341,   499,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,     0,   355,   356,   357,
       0,     0,   360,   361,   362,   363,   500,   501,   601,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   504,   505,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   509,   510,   511,   512,   513,
       0,   514,     0,   515,   516,   517,   518,     0,     0,     0,
     519,   520,   521,   522,   523,   524,   525,    63,   602,   527,
     528,     0,     0,     0,     0,     0,     0,   529,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   530,   531,   532,     0,    15,     0,
       0,   533,   534,     0,     0,   466,   467,     0,     0,   535,
       0,   536,     0,   537,   538,   472,   473,   474,   475,   476,
       0,     0,     0,     0,     0,   477,     0,   479,     0,     0,
       0,   482,     0,     0,     0,     0,     0,     0,     0,   484,
       0,     0,     0,     0,     0,   488,     0,     0,   489,     0,
       0,   490,     0,     0,     0,   493,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   600,     0,     0,   497,   498,
       0,   333,   334,   335,     0,   337,   338,   339,   340,   341,
     499,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,   353,     0,   355,   356,   357,     0,     0,   360,   361,
     362,   363,   500,   501,   601,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   504,   505,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   509,   510,   511,   512,   513,     0,   514,     0,   515,
     516,   517,   518,     0,     0,     0,   519,   520,   521,   522,
     523,   524,   525,    63,   602,   527,   528,     0,     0,     0,
       0,     0,     0,   529,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     530,   531,   532,     0,    15,     0,     0,   533,   534,     0,
       0,   466,   467,     0,     0,   535,     0,   536,  1114,   537,
     538,   472,   473,   474,   475,   476,     0,     0,     0,     0,
       0,   477,     0,   479,     0,     0,     0,   482,     0,     0,
       0,     0,     0,     0,     0,   484,     0,     0,     0,     0,
       0,   488,     0,     0,   489,     0,     0,   490,     0,     0,
       0,   493,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   600,     0,     0,   497,   498,     0,   333,   334,   335,
       0,   337,   338,   339,   340,   341,   499,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,     0,   355,
     356,   357,     0,     0,   360,   361,   362,   363,   500,   501,
     601,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   504,   505,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   509,   510,   511,
     512,   513,     0,   514,     0,   515,   516,   517,   518,     0,
       0,     0,   519,   520,   521,   522,   523,   524,   525,    63,
     602,   527,   528,     0,     0,     0,     0,     0,     0,   529,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1169,     0,   530,   531,   532,     0,
      15,     0,     0,   533,   534,     0,     0,   466,   467,     0,
       0,   535,     0,   536,     0,   537,   538,   472,   473,   474,
     475,   476,     0,     0,     0,     0,     0,   477,     0,   479,
       0,     0,     0,   482,     0,     0,     0,     0,     0,     0,
       0,   484,     0,     0,     0,     0,     0,   488,     0,     0,
     489,     0,     0,   490,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   600,     0,     0,
     497,   498,     0,   333,   334,   335,     0,   337,   338,   339,
     340,   341,   499,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,     0,   355,   356,   357,     0,     0,
     360,   361,   362,   363,   500,   501,   601,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     504,   505,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   509,   510,   511,   512,   513,     0,   514,
       0,   515,   516,   517,   518,     0,     0,     0,   519,   520,
     521,   522,   523,   524,   525,    63,   602,   527,   528,     0,
       0,     0,     0,     0,     0,   529,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   530,   531,   532,     0,    15,     0,     0,   533,
     534,     0,     0,   466,   467,     0,     0,   535,     0,   536,
    1452,   537,   538,   472,   473,   474,   475,   476,     0,     0,
       0,     0,     0,   477,     0,   479,     0,     0,     0,   482,
       0,     0,     0,     0,     0,     0,     0,   484,     0,     0,
       0,     0,     0,   488,     0,     0,   489,     0,     0,   490,
       0,     0,     0,   493,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   600,     0,     0,   497,   498,     0,   333,
     334,   335,     0,   337,   338,   339,   340,   341,   499,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
       0,   355,   356,   357,     0,     0,   360,   361,   362,   363,
     500,   501,   601,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   504,   505,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   509,
     510,   511,   512,   513,     0,   514,     0,   515,   516,   517,
     518,     0,     0,     0,   519,   520,   521,   522,   523,   524,
     525,    63,   602,   527,   528,     0,     0,     0,     0,     0,
       0,   529,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   530,   531,
     532,     0,    15,     0,     0,   533,   534,     0,     0,   466,
     467,     0,     0,  1461,     0,   536,  1462,   537,   538,   472,
     473,   474,   475,   476,     0,     0,     0,     0,     0,   477,
       0,   479,     0,     0,     0,   482,     0,     0,     0,     0,
       0,     0,     0,   484,     0,     0,     0,     0,     0,   488,
       0,     0,   489,     0,     0,   490,     0,     0,     0,   493,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   600,
       0,     0,   497,   498,     0,   333,   334,   335,     0,   337,
     338,   339,   340,   341,   499,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,     0,   355,   356,   357,
       0,     0,   360,   361,   362,   363,   500,   501,   601,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   504,   505,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   509,   510,   511,   512,   513,
       0,   514,     0,   515,   516,   517,   518,     0,     0,     0,
     519,   520,   521,   522,   523,   524,   525,    63,   602,   527,
     528,     0,     0,     0,     0,     0,     0,   529,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   530,   531,   532,     0,    15,     0,
       0,   533,   534,     0,     0,   466,   467,     0,     0,   535,
       0,   536,  1467,   537,   538,   472,   473,   474,   475,   476,
       0,     0,     0,     0,     0,   477,     0,   479,     0,     0,
       0,   482,     0,     0,     0,     0,     0,     0,     0,   484,
       0,     0,     0,     0,     0,   488,     0,     0,   489,     0,
       0,   490,     0,     0,     0,   493,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   600,     0,     0,   497,   498,
       0,   333,   334,   335,     0,   337,   338,   339,   340,   341,
     499,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,   353,     0,   355,   356,   357,     0,     0,   360,   361,
     362,   363,   500,   501,   601,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   504,   505,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   509,   510,   511,   512,   513,     0,   514,     0,   515,
     516,   517,   518,     0,     0,     0,   519,   520,   521,   522,
     523,   524,   525,    63,   602,   527,   528,     0,     0,     0,
       0,     0,     0,   529,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     530,   531,   532,     0,    15,     0,     0,   533,   534,     0,
       0,   466,   467,     0,     0,   535,     0,   536,  1513,   537,
     538,   472,   473,   474,   475,   476,     0,     0,     0,     0,
       0,   477,     0,   479,     0,     0,     0,   482,     0,     0,
       0,     0,     0,     0,     0,   484,     0,     0,     0,     0,
       0,   488,     0,     0,   489,     0,     0,   490,     0,     0,
       0,   493,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   600,     0,     0,   497,   498,     0,   333,   334,   335,
       0,   337,   338,   339,   340,   341,   499,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,     0,   355,
     356,   357,     0,     0,   360,   361,   362,   363,   500,   501,
     601,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   504,   505,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   509,   510,   511,
     512,   513,     0,   514,     0,   515,   516,   517,   518,     0,
       0,     0,   519,   520,   521,   522,   523,   524,   525,    63,
     602,   527,   528,     0,     0,     0,     0,     0,     0,   529,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   530,   531,   532,     0,
      15,     0,     0,   533,   534,     0,     0,   466,   467,     0,
       0,   535,     0,   536,  1594,   537,   538,   472,   473,   474,
     475,   476,     0,     0,     0,     0,     0,   477,     0,   479,
       0,     0,     0,   482,     0,     0,     0,     0,     0,     0,
       0,   484,     0,     0,     0,     0,     0,   488,     0,     0,
     489,     0,     0,   490,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   600,     0,     0,
     497,   498,     0,   333,   334,   335,     0,   337,   338,   339,
     340,   341,   499,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,     0,   355,   356,   357,     0,     0,
     360,   361,   362,   363,   500,   501,   601,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     504,   505,     0,     0,     0,     0,     0,     0,     0,  1630,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   509,   510,   511,   512,   513,     0,   514,
       0,   515,   516,   517,   518,     0,     0,     0,   519,   520,
     521,   522,   523,   524,   525,    63,   602,   527,   528,     0,
       0,     0,     0,     0,     0,   529,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   530,   531,   532,     0,    15,     0,     0,   533,
     534,     0,     0,   466,   467,     0,     0,   535,     0,   536,
       0,   537,   538,   472,   473,   474,   475,   476,     0,     0,
       0,     0,     0,   477,     0,   479,     0,     0,     0,   482,
       0,     0,     0,     0,     0,     0,     0,   484,     0,     0,
       0,     0,     0,   488,     0,     0,   489,     0,     0,   490,
       0,     0,     0,   493,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   600,     0,     0,   497,   498,     0,   333,
     334,   335,     0,   337,   338,   339,   340,   341,   499,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
       0,   355,   356,   357,     0,     0,   360,   361,   362,   363,
     500,   501,   601,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   504,   505,     0,     0,
       0,     0,     0,     0,     0,  1672,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   509,
     510,   511,   512,   513,     0,   514,     0,   515,   516,   517,
     518,     0,     0,     0,   519,   520,   521,   522,   523,   524,
     525,    63,   602,   527,   528,     0,     0,     0,     0,     0,
       0,   529,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   530,   531,
     532,     0,    15,     0,     0,   533,   534,     0,     0,   466,
     467,     0,     0,   535,     0,   536,     0,   537,   538,   472,
     473,   474,   475,   476,     0,     0,     0,     0,     0,   477,
       0,   479,     0,     0,     0,   482,     0,     0,     0,     0,
       0,     0,     0,   484,     0,     0,     0,     0,     0,   488,
       0,     0,   489,     0,     0,   490,     0,     0,     0,   493,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   600,
       0,     0,   497,   498,     0,   333,   334,   335,     0,   337,
     338,   339,   340,   341,   499,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,     0,   355,   356,   357,
       0,     0,   360,   361,   362,   363,   500,   501,   601,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   504,   505,     0,     0,     0,     0,     0,     0,
       0,  1673,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   509,   510,   511,   512,   513,
       0,   514,     0,   515,   516,   517,   518,     0,     0,     0,
     519,   520,   521,   522,   523,   524,   525,    63,   602,   527,
     528,     0,     0,     0,     0,     0,     0,   529,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   530,   531,   532,     0,    15,     0,
       0,   533,   534,     0,     0,   466,   467,     0,     0,   535,
       0,   536,     0,   537,   538,   472,   473,   474,   475,   476,
       0,     0,     0,     0,     0,   477,     0,   479,     0,     0,
       0,   482,     0,     0,     0,     0,     0,     0,     0,   484,
       0,     0,     0,     0,     0,   488,     0,     0,   489,     0,
       0,   490,     0,     0,     0,   493,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   600,     0,     0,   497,   498,
       0,   333,   334,   335,     0,   337,   338,   339,   340,   341,
     499,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,   353,     0,   355,   356,   357,     0,     0,   360,   361,
     362,   363,   500,   501,   601,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   504,   505,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   509,   510,   511,   512,   513,     0,   514,     0,   515,
     516,   517,   518,     0,     0,     0,   519,   520,   521,   522,
     523,   524,   525,    63,   602,   527,   528,     0,     0,     0,
       0,     0,     0,   529,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     530,   531,   532,     0,    15,     0,     0,   533,   534,     0,
       0,   466,   467,     0,     0,   535,     0,   536,     0,   537,
     538,   472,   473,   474,   475,   476,     0,     0,     0,     0,
       0,   477,     0,   479,     0,     0,     0,   482,     0,     0,
       0,     0,     0,     0,     0,   484,     0,     0,     0,     0,
       0,   488,     0,     0,   489,     0,     0,   490,     0,     0,
       0,   493,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   600,     0,     0,   497,   498,     0,   333,   334,   335,
       0,   337,   338,   339,   340,   341,   499,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,     0,   355,
     356,   357,     0,     0,   360,   361,   362,   363,   500,   501,
     601,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   504,   505,     0,     0,     0,   -78,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     779,   780,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   509,   510,   511,
     512,   513,     0,   514,     0,   515,   516,   517,   518,     0,
       0,     0,   519,   520,   521,   522,   523,   524,   525,    63,
     602,   527,   528,     0,     0,     0,     0,     0,     0,   529,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   530,   531,   532,     0,
      15,     0,     0,   533,   534,     0,     0,     0,     0,     0,
    1026,  1435,     0,   536,     0,   537,   538,   781,   782,   783,
     784,   785,   786,   787,   788,   789,   790,   791,   792,   793,
     794,   795,   796,   797,   798,   799,   800,   801,   802,   803,
     804,     0,     0,   779,   780,   805,   806,   807,   808,   809,
     810,     0,     0,   811,   812,   813,   814,   815,   816,   817,
       0,     0,   333,   334,   335,     0,   337,   338,   339,   340,
     341,   499,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,     0,   355,   356,   357,     0,     0,   360,
     361,   362,   363,     0,     0,     0,     0,     0,     0,     0,
       0,   818,   819,     0,   820,   821,   822,   823,   824,   825,
     826,   827,   828,   829,   327,     0,     0,     0,     0,     0,
     328,     0,     0,   830,   831,     0,   329,     0,   537,   716,
       0,     0,     0,     0,     0,     0,   330,     0,   789,   790,
     791,   792,   793,     0,   331,   796,   797,   798,   799,  1027,
     801,   802,   803,   804,     0,     0,     0,     0,   805,   332,
     807,   808,     0,     0,  1028,     0,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   364,   365,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   821,   822,
     823,   824,   825,   826,   827,   828,   829,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   830,   831,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   366,     0,     0,     0,     0,     0,     0,     0,     0,
      13,     0,     0,   327,     0,     0,     0,     0,   644,   328,
       0,     0,     0,     0,     0,   329,     0,     0,     0,     0,
      14,     0,     0,     0,     0,   330,   645,     0,     0,     0,
       0,     0,     0,   331,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   332,     0,
       0,     0,     0,     0,   367,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,   365,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   327,     0,     0,     0,
       0,     0,   328,     0,     0,     0,     0,     0,   329,     0,
       0,     0,     0,     0,     0,     0,     0,    62,   330,     0,
       0,     0,     0,     0,     0,     0,   331,     0,     0,     0,
     366,     0,     0,     0,     0,     0,     0,     0,     0,   779,
     780,   332,     0,     0,     0,     0,     0,    63,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,     0,   779,   780,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   367,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   366,   789,   790,   791,   792,   793,     0,
       0,   796,   797,   798,   799,     0,   801,   802,   803,   804,
     644,     0,     0,     0,   805,     0,   807,   808,     0,     0,
       0,     0,   811,     0,   813,     0,     0,     0,     0,   912,
     913,   914,   915,   916,   917,   918,   919,   789,   790,   791,
     792,   793,   920,   921,   796,   797,   798,   799,   922,   801,
     802,   803,   804,     0,     0,     0,   367,   805,   806,   807,
     808,   923,   924,   779,   780,   811,   812,   813,   925,   926,
     927,   817,     0,   820,   821,   822,   823,   824,   825,   826,
     827,   828,   829,     0,     0,     0,    13,     0,     0,     0,
       0,     0,   830,   831,     0,     0,     0,     0,   779,   780,
       0,     0,     0,     0,     0,     0,    14,     0,     0,     0,
       0,     0,     0,   928,   819,     0,   820,   821,   822,   823,
     824,   825,   826,   827,   828,   829,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   830,   831,     0,     0,     0,
     537,   716,     0,     0,     0,     0,     0,     0,     0,     0,
     912,   913,   914,   915,   916,   917,   918,   919,   789,   790,
     791,   792,   793,   920,   921,   796,   797,   798,   799,   922,
     801,   802,   803,   804,  -393,     0,     0,     0,   805,   806,
     807,   808,   923,   924,     0,     0,   811,   812,   813,   925,
     926,   927,   817,   789,   790,   791,   792,   793,     0,     0,
     796,   797,   798,   799,     0,   801,   802,   803,   804,   779,
     780,     0,     0,   805,     0,   807,   808,     0,     0,     0,
       0,   811,   812,   813,     0,     0,     0,   817,     0,     0,
       0,     0,     0,     0,   928,   819,     0,   820,   821,   822,
     823,   824,   825,   826,   827,   828,   829,   779,   780,     0,
       0,     0,     0,     0,     0,     0,   830,   831,     0,     0,
       0,   537,   716,     0,     0,     0,     0,     0,     0,     0,
     819,     0,   820,   821,   822,   823,   824,   825,   826,   827,
     828,   829,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   830,   831,     0,     0,   870,     0,     0,     0,     0,
       0,     0,     0,     0,   789,   790,   791,   792,   793,     0,
       0,   796,   797,   798,   799,     0,   801,   802,   803,   804,
       0,     0,     0,     0,   805,     0,   807,   808,     0,     0,
       0,     0,   811,   812,   813,     0,     0,     0,   817,     0,
       0,     0,   789,   790,   791,   792,   793,     0,     0,   796,
     797,   798,   799,     0,   801,   802,   803,   804,   779,   780,
       0,     0,   805,     0,   807,   808,     0,     0,     0,     0,
     811,   812,   813,     0,     0,     0,   817,     0,     0,     0,
       0,   819,     0,   820,   821,   822,   823,   824,   825,   826,
     827,   828,   829,   779,   780,     0,     0,     0,     0,     0,
       0,     0,   830,   831,     0,     0,   885,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   819,
       0,   820,   821,   822,   823,   824,   825,   826,   827,   828,
     829,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     830,   831,     0,     0,  1201,     0,     0,     0,     0,     0,
       0,     0,     0,   789,   790,   791,   792,   793,     0,     0,
     796,   797,   798,   799,     0,   801,   802,   803,   804,     0,
       0,     0,     0,   805,     0,   807,   808,     0,     0,     0,
       0,   811,   812,   813,     0,     0,     0,   817,   789,   790,
     791,   792,   793,     0,     0,   796,   797,   798,   799,     0,
     801,   802,   803,   804,   779,   780,     0,     0,   805,     0,
     807,   808,     0,     0,     0,     0,   811,   812,   813,     0,
       0,     0,   817,     0,     0,     0,     0,     0,     0,     0,
     819,     0,   820,   821,   822,   823,   824,   825,   826,   827,
     828,   829,   779,   780,     0,     0,     0,     0,     0,     0,
       0,   830,   831,     0,     0,  1206,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   819,     0,   820,   821,   822,
     823,   824,   825,   826,   827,   828,   829,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   830,   831,     0,     0,
    1208,     0,     0,     0,     0,     0,     0,     0,     0,   789,
     790,   791,   792,   793,     0,     0,   796,   797,   798,   799,
       0,   801,   802,   803,   804,     0,     0,     0,     0,   805,
       0,   807,   808,     0,     0,     0,     0,   811,   812,   813,
       0,     0,     0,   817,     0,     0,     0,   789,   790,   791,
     792,   793,     0,     0,   796,   797,   798,   799,     0,   801,
     802,   803,   804,   779,   780,     0,     0,   805,     0,   807,
     808,     0,     0,     0,     0,   811,   812,   813,     0,     0,
       0,   817,     0,     0,     0,     0,   819,     0,   820,   821,
     822,   823,   824,   825,   826,   827,   828,   829,   779,   780,
       0,     0,     0,     0,     0,     0,     0,   830,   831,     0,
       0,  1217,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   819,     0,   820,   821,   822,   823,
     824,   825,   826,   827,   828,   829,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   830,   831,     0,     0,  1218,
       0,     0,     0,     0,     0,     0,     0,     0,   789,   790,
     791,   792,   793,     0,     0,   796,   797,   798,   799,     0,
     801,   802,   803,   804,     0,     0,     0,     0,   805,     0,
     807,   808,     0,     0,     0,     0,   811,   812,   813,     0,
       0,     0,   817,   789,   790,   791,   792,   793,     0,     0,
     796,   797,   798,   799,     0,   801,   802,   803,   804,   779,
     780,     0,     0,   805,     0,   807,   808,     0,     0,     0,
       0,   811,   812,   813,     0,     0,     0,   817,     0,     0,
       0,     0,     0,     0,     0,   819,     0,   820,   821,   822,
     823,   824,   825,   826,   827,   828,   829,   779,   780,     0,
       0,     0,     0,     0,     0,     0,   830,   831,     0,     0,
    1219,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     819,     0,   820,   821,   822,   823,   824,   825,   826,   827,
     828,   829,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   830,   831,     0,     0,  1220,     0,     0,     0,     0,
       0,     0,     0,     0,   789,   790,   791,   792,   793,     0,
       0,   796,   797,   798,   799,     0,   801,   802,   803,   804,
       0,     0,     0,     0,   805,     0,   807,   808,     0,     0,
       0,     0,   811,   812,   813,     0,     0,     0,   817,     0,
       0,     0,   789,   790,   791,   792,   793,     0,     0,   796,
     797,   798,   799,     0,   801,   802,   803,   804,   779,   780,
       0,     0,   805,     0,   807,   808,     0,     0,     0,     0,
     811,   812,   813,     0,     0,     0,   817,     0,     0,     0,
       0,   819,     0,   820,   821,   822,   823,   824,   825,   826,
     827,   828,   829,   779,   780,     0,     0,     0,     0,     0,
       0,     0,   830,   831,     0,     0,  1221,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   819,
       0,   820,   821,   822,   823,   824,   825,   826,   827,   828,
     829,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     830,   831,     0,     0,  1222,     0,     0,     0,     0,     0,
       0,     0,     0,   789,   790,   791,   792,   793,     0,     0,
     796,   797,   798,   799,     0,   801,   802,   803,   804,     0,
       0,     0,     0,   805,     0,   807,   808,     0,     0,     0,
       0,   811,   812,   813,     0,     0,     0,   817,   789,   790,
     791,   792,   793,     0,     0,   796,   797,   798,   799,     0,
     801,   802,   803,   804,   779,   780,     0,     0,   805,     0,
     807,   808,     0,     0,     0,     0,   811,   812,   813,     0,
       0,     0,   817,     0,     0,     0,     0,     0,     0,     0,
     819,     0,   820,   821,   822,   823,   824,   825,   826,   827,
     828,   829,   779,   780,     0,     0,     0,     0,     0,     0,
       0,   830,   831,     0,     0,  1356,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   819,     0,   820,   821,   822,
     823,   824,   825,   826,   827,   828,   829,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   830,   831,     0,     0,
    1368,     0,     0,     0,     0,     0,     0,     0,     0,   789,
     790,   791,   792,   793,     0,     0,   796,   797,   798,   799,
       0,   801,   802,   803,   804,     0,     0,     0,     0,   805,
       0,   807,   808,     0,     0,     0,     0,   811,   812,   813,
       0,     0,     0,   817,     0,     0,     0,   789,   790,   791,
     792,   793,     0,     0,   796,   797,   798,   799,     0,   801,
     802,   803,   804,   779,   780,     0,     0,   805,     0,   807,
     808,     0,     0,     0,     0,   811,   812,   813,     0,     0,
       0,   817,     0,     0,     0,     0,   819,     0,   820,   821,
     822,   823,   824,   825,   826,   827,   828,   829,   779,   780,
       0,     0,     0,     0,     0,     0,     0,   830,   831,     0,
       0,  1371,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   819,     0,   820,   821,   822,   823,
     824,   825,   826,   827,   828,   829,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   830,   831,     0,     0,  1419,
       0,     0,     0,     0,     0,     0,     0,     0,   789,   790,
     791,   792,   793,     0,     0,   796,   797,   798,   799,     0,
     801,   802,   803,   804,     0,     0,     0,     0,   805,     0,
     807,   808,     0,     0,     0,     0,   811,   812,   813,     0,
       0,     0,   817,   789,   790,   791,   792,   793,     0,     0,
     796,   797,   798,   799,     0,   801,   802,   803,   804,   779,
     780,     0,     0,   805,     0,   807,   808,     0,     0,     0,
       0,   811,   812,   813,     0,     0,     0,   817,     0,     0,
       0,     0,     0,     0,     0,   819,     0,   820,   821,   822,
     823,   824,   825,   826,   827,   828,   829,   779,   780,     0,
       0,     0,     0,     0,     0,     0,   830,   831,     0,     0,
    1534,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     819,     0,   820,   821,   822,   823,   824,   825,   826,   827,
     828,   829,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   830,   831,     0,     0,  1535,     0,     0,     0,     0,
       0,     0,     0,     0,   789,   790,   791,   792,   793,     0,
       0,   796,   797,   798,   799,     0,   801,   802,   803,   804,
       0,     0,     0,     0,   805,     0,   807,   808,     0,     0,
       0,     0,   811,   812,   813,     0,     0,     0,   817,     0,
       0,     0,   789,   790,   791,   792,   793,     0,     0,   796,
     797,   798,   799,     0,   801,   802,   803,   804,   779,   780,
       0,     0,   805,     0,   807,   808,     0,     0,     0,     0,
     811,   812,   813,     0,     0,     0,   817,     0,     0,     0,
       0,   819,     0,   820,   821,   822,   823,   824,   825,   826,
     827,   828,   829,   779,   780,     0,     0,     0,     0,     0,
       0,     0,   830,   831,     0,     0,  1556,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   819,
       0,   820,   821,   822,   823,   824,   825,   826,   827,   828,
     829,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     830,   831,     0,     0,  1558,     0,     0,     0,     0,     0,
       0,     0,     0,   789,   790,   791,   792,   793,     0,     0,
     796,   797,   798,   799,     0,   801,   802,   803,   804,     0,
       0,     0,     0,   805,     0,   807,   808,     0,     0,     0,
       0,   811,   812,   813,     0,     0,     0,   817,   789,   790,
     791,   792,   793,     0,     0,   796,   797,   798,   799,     0,
     801,   802,   803,   804,   779,   780,     0,     0,   805,     0,
     807,   808,     0,     0,     0,     0,   811,   812,   813,     0,
       0,     0,   817,     0,     0,     0,     0,     0,     0,     0,
     819,     0,   820,   821,   822,   823,   824,   825,   826,   827,
     828,   829,   779,   780,     0,     0,     0,     0,     0,     0,
       0,   830,   831,     0,     0,  1560,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   819,     0,   820,   821,   822,
     823,   824,   825,   826,   827,   828,   829,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   830,   831,     0,     0,
    1564,     0,     0,     0,     0,     0,     0,     0,     0,   789,
     790,   791,   792,   793,     0,     0,   796,   797,   798,   799,
       0,   801,   802,   803,   804,     0,     0,     0,     0,   805,
       0,   807,   808,     0,     0,     0,     0,   811,   812,   813,
       0,     0,     0,   817,     0,     0,     0,   789,   790,   791,
     792,   793,     0,     0,   796,   797,   798,   799,     0,   801,
     802,   803,   804,   779,   780,     0,     0,   805,     0,   807,
     808,     0,     0,     0,     0,   811,   812,   813,     0,     0,
       0,   817,     0,     0,     0,     0,   819,     0,   820,   821,
     822,   823,   824,   825,   826,   827,   828,   829,   779,   780,
       0,     0,     0,     0,     0,     0,     0,   830,   831,     0,
       0,  1611,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   819,     0,   820,   821,   822,   823,
     824,   825,   826,   827,   828,   829,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   830,   831,     0,     0,  1635,
       0,     0,     0,     0,     0,     0,     0,     0,   789,   790,
     791,   792,   793,     0,     0,   796,   797,   798,   799,     0,
     801,   802,   803,   804,     0,     0,     0,     0,   805,     0,
     807,   808,     0,     0,     0,     0,   811,   812,   813,     0,
       0,     0,   817,   789,   790,   791,   792,   793,     0,     0,
     796,   797,   798,   799,     0,   801,   802,   803,   804,   779,
     780,     0,     0,   805,     0,   807,   808,     0,     0,     0,
       0,   811,   812,   813,     0,     0,     0,   817,     0,     0,
       0,     0,     0,     0,     0,   819,     0,   820,   821,   822,
     823,   824,   825,   826,   827,   828,   829,   779,   780,     0,
       0,     0,     0,     0,     0,     0,   830,   831,     0,     0,
    1636,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     819,     0,   820,   821,   822,   823,   824,   825,   826,   827,
     828,   829,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   830,   831,     0,     0,  1638,     0,     0,     0,     0,
       0,     0,     0,     0,   789,   790,   791,   792,   793,     0,
       0,   796,   797,   798,   799,     0,   801,   802,   803,   804,
       0,     0,     0,     0,   805,     0,   807,   808,     0,     0,
       0,     0,   811,   812,   813,     0,     0,     0,   817,     0,
       0,     0,   789,   790,   791,   792,   793,     0,     0,   796,
     797,   798,   799,     0,   801,   802,   803,   804,   779,   780,
       0,     0,   805,     0,   807,   808,     0,     0,     0,     0,
     811,   812,   813,     0,     0,     0,   817,     0,     0,     0,
       0,   819,     0,   820,   821,   822,   823,   824,   825,   826,
     827,   828,   829,   779,   780,     0,     0,     0,     0,     0,
       0,     0,   830,   831,     0,     0,  1647,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   819,
       0,   820,   821,   822,   823,   824,   825,   826,   827,   828,
     829,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     830,   831,     0,     0,  1650,     0,     0,     0,     0,     0,
       0,     0,     0,   789,   790,   791,   792,   793,     0,     0,
     796,   797,   798,   799,     0,   801,   802,   803,   804,     0,
       0,     0,     0,   805,     0,   807,   808,     0,     0,     0,
       0,   811,   812,   813,     0,     0,     0,   817,   789,   790,
     791,   792,   793,     0,     0,   796,   797,   798,   799,     0,
     801,   802,   803,   804,   779,   780,     0,     0,   805,     0,
     807,   808,     0,     0,     0,     0,   811,   812,   813,     0,
       0,     0,   817,     0,     0,     0,     0,     0,     0,     0,
     819,     0,   820,   821,   822,   823,   824,   825,   826,   827,
     828,   829,   779,   780,     0,     0,     0,     0,     0,     0,
       0,   830,   831,     0,     0,  1659,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   819,     0,   820,   821,   822,
     823,   824,   825,   826,   827,   828,   829,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   830,   831,     0,     0,
    1731,     0,     0,     0,     0,     0,     0,     0,     0,   789,
     790,   791,   792,   793,     0,     0,   796,   797,   798,   799,
       0,   801,   802,   803,   804,     0,     0,     0,     0,   805,
       0,   807,   808,     0,     0,     0,     0,   811,   812,   813,
       0,     0,     0,   817,     0,     0,     0,   789,   790,   791,
     792,   793,     0,     0,   796,   797,   798,   799,     0,   801,
     802,   803,   804,   779,   780,     0,     0,   805,     0,   807,
     808,     0,     0,     0,     0,   811,   812,   813,     0,     0,
       0,   817,     0,     0,     0,     0,   819,     0,   820,   821,
     822,   823,   824,   825,   826,   827,   828,   829,   779,   780,
       0,     0,     0,     0,     0,     0,     0,   830,   831,     0,
       0,  1732,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   819,     0,   820,   821,   822,   823,
     824,   825,   826,   827,   828,   829,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   830,   831,   874,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   789,   790,
     791,   792,   793,     0,     0,   796,   797,   798,   799,     0,
     801,   802,   803,   804,     0,     0,     0,     0,   805,     0,
     807,   808,     0,     0,     0,     0,   811,   812,   813,     0,
       0,     0,   817,   789,   790,   791,   792,   793,     0,     0,
     796,   797,   798,   799,     0,   801,   802,   803,   804,   779,
     780,     0,     0,   805,     0,   807,   808,     0,     0,     0,
       0,   811,   812,   813,     0,     0,     0,   817,     0,     0,
       0,     0,     0,     0,     0,   819,     0,   820,   821,   822,
     823,   824,   825,   826,   827,   828,   829,   779,   780,     0,
       0,     0,     0,     0,     0,     0,   830,   831,  1150,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     819,     0,   820,   821,   822,   823,   824,   825,   826,   827,
     828,   829,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   830,   831,  1304,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   789,   790,   791,   792,   793,     0,
       0,   796,   797,   798,   799,     0,   801,   802,   803,   804,
       0,     0,     0,     0,   805,     0,   807,   808,     0,     0,
       0,     0,   811,   812,   813,     0,     0,     0,   817,     0,
       0,     0,   789,   790,   791,   792,   793,     0,     0,   796,
     797,   798,   799,     0,   801,   802,   803,   804,   779,   780,
       0,     0,   805,     0,   807,   808,     0,     0,     0,     0,
     811,   812,   813,     0,     0,     0,   817,     0,     0,     0,
       0,   819,     0,   820,   821,   822,   823,   824,   825,   826,
     827,   828,   829,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   830,   831,  1320,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   819,
       0,   820,   821,   822,   823,   824,   825,   826,   827,   828,
     829,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     830,   831,  1459,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   789,   790,   791,   792,   793,     0,     0,
     796,   797,   798,   799,     0,   801,   802,   803,   804,   246,
     247,     0,     0,   805,     0,   807,   808,     0,     0,     0,
       0,   811,   812,   813,     0,     0,   248,   817,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   779,   780,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     819,     0,   820,   821,   822,   823,   824,   825,   826,   827,
     828,   829,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   830,   831,  1465,     0,     0,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,     0,     0,   267,   268,   269,     0,
       0,     0,     0,     0,     0,   270,   271,   272,   273,   274,
       0,     0,   275,   276,   277,   278,   279,   280,   281,     0,
       0,     0,     0,     0,   779,   780,   789,   790,   791,   792,
     793,     0,     0,   796,   797,   798,   799,     0,   801,   802,
     803,   804,     0,     0,     0,     0,   805,     0,   807,   808,
       0,     0,     0,     0,   811,   812,   813,     0,     0,     0,
     817,   282,     0,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,    50,     0,   293,   294,     0,     0,     0,
       0,     0,   295,   296,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    51,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   819,     0,   820,   821,   822,   823,   824,
     825,   826,   827,   828,   829,     0,     0,   779,   780,   789,
     790,   791,   792,   793,   830,   831,   796,   797,   798,   799,
       0,   801,   802,   803,   804,     0,     0,     0,     0,   805,
       0,   807,   808,     0,     0,     0,     0,   811,   812,   813,
       0,     0,     0,   817,   779,   780,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    13,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    14,     0,
       0,     0,     0,     0,     0,     0,   819,     0,   820,   821,
     822,   823,   824,   825,   826,   827,   828,   829,     0,     0,
       0,     0,   789,   790,   791,   792,   793,   830,   831,   796,
     797,   798,   799,     0,   801,   802,   803,   804,     0,     0,
       0,     0,   805,     0,   807,   808,     0,     0,   977,     0,
     811,   812,   813,     0,     0,     0,   817,   779,   780,   789,
     790,   791,   792,   793,     0,     0,   796,   797,   798,   799,
       0,   801,   802,   803,   804,     0,     0,     0,     0,   805,
       0,   807,   808,     0,     0,  1228,     0,   811,   812,   813,
       0,     0,     0,   817,   779,   780,     0,     0,     0,   819,
       0,   820,   821,   822,   823,   824,   825,   826,   827,   828,
     829,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     830,   831,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   819,     0,   820,   821,
     822,   823,   824,   825,   826,   827,   828,   829,     0,     0,
       0,     0,   789,   790,   791,   792,   793,   830,   831,   796,
     797,   798,   799,     0,   801,   802,   803,   804,     0,     0,
       0,     0,   805,     0,   807,   808,     0,     0,     0,     0,
     811,   812,   813,     0,     0,     0,   817,   779,   780,   789,
     790,   791,   792,   793,     0,     0,   796,   797,   798,   799,
       0,   801,   802,   803,   804,     0,     0,     0,     0,   805,
       0,   807,   808,     0,     0,     0,     0,   811,   812,   813,
       0,     0,     0,   817,   779,   780,     0,     0,     0,   819,
    1309,   820,   821,   822,   823,   824,   825,   826,   827,   828,
     829,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     830,   831,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   819,  1414,   820,   821,
     822,   823,   824,   825,   826,   827,   828,   829,     0,     0,
       0,     0,   789,   790,   791,   792,   793,   830,   831,   796,
     797,   798,   799,     0,   801,   802,   803,   804,     0,     0,
       0,     0,   805,     0,   807,   808,     0,     0,     0,     0,
     811,   812,   813,     0,     0,     0,   817,   779,   780,   789,
     790,   791,   792,   793,     0,     0,   796,   797,   798,   799,
       0,   801,   802,   803,   804,     0,     0,     0,     0,   805,
       0,   807,   808,     0,     0,     0,     0,   811,   812,   813,
       0,  1660,     0,   817,   779,   780,     0,     0,     0,   819,
       0,   820,   821,   822,   823,   824,   825,   826,   827,   828,
     829,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     830,   831,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   819,     0,   820,   821,
     822,   823,   824,   825,   826,   827,   828,   829,     0,     0,
       0,     0,   789,   790,   791,   792,   793,   830,   831,   796,
     797,   798,   799,     0,   801,   802,   803,   804,     0,     0,
       0,     0,   805,     0,   807,   808,     0,     0,     0,     0,
     811,   812,   813,     0,     0,     0,  -808,   779,   780,   789,
     790,   791,   792,   793,     0,     0,   796,   797,   798,   799,
       0,   801,   802,   803,   804,     0,     0,     0,     0,   805,
       0,   807,   808,     0,     0,     0,     0,   811,   812,   813,
     779,   780,     0,     0,     0,     0,     0,     0,     0,   819,
       0,   820,   821,   822,   823,   824,   825,   826,   827,   828,
     829,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     830,   831,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   819,     0,   820,   821,
     822,   823,   824,   825,   826,   827,   828,   829,     0,     0,
       0,     0,   789,   790,   791,   792,   793,   830,   831,   796,
     797,   798,   799,     0,   801,   802,   803,   804,     0,     0,
       0,     0,   805,     0,   807,   808,     0,     0,     0,     0,
     811,     0,     0,   779,   780,   789,   790,   791,   792,   793,
       0,     0,   796,   797,   798,   799,     0,   801,   802,   803,
     804,     0,     0,     0,     0,   805,     0,   807,   808,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   820,   821,   822,   823,   824,   825,   826,   827,   828,
     829,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     830,   831,  1030,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   822,   823,   824,   825,
     826,   827,   828,   829,     0,     0,     0,     0,   789,   790,
     791,   792,   793,   830,   831,   796,   797,   798,   799,     0,
     801,   802,   803,   804,     0,     0,     0,     0,   805,     0,
     807,   808,     0,     0,   333,   334,   335,     0,   337,   338,
     339,   340,   341,   499,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,     0,   355,   356,   357,     0,
       0,   360,   361,   362,   363,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1305,     0,     0,     0,     0,     0,     0,     0,     0,
     823,   824,   825,   826,   827,   828,   829,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   830,   831,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1031,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   333,   334,   335,  1032,   337,   338,   339,
     340,   341,   499,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,     0,   355,   356,   357,     0,     0,
     360,   361,   362,   363,   333,   334,   335,     0,   337,   338,
     339,   340,   341,   499,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,     0,   355,   356,   357,     0,
     194,   360,   361,   362,   363,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1306,     0,  1105,  1106,     0,     0,   195,     0,   196,     0,
     197,   198,   199,   200,   201,  1307,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,     0,   213,   214,
     215,  1107,     0,   216,   217,   218,   219,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1108,     0,     0,     0,
       0,     0,     0,   220,   221,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1109,  1110,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   222
};

static const yytype_int16 yycheck[] =
{
       1,   478,   518,   607,   224,   397,   397,   687,   556,   410,
      15,    16,   630,     7,   442,   171,   441,   731,    19,   752,
     660,   478,   573,   756,   757,   768,   503,   507,   508,   404,
     968,   729,    86,   731,   240,   861,   974,   863,   439,   865,
    1231,   397,   949,   954,   947,   621,   597,   623,     5,   625,
      22,     4,   160,    15,    16,    20,   667,    20,  1438,    19,
      20,     5,     6,    68,    69,    70,    20,    53,     8,    53,
     109,  1612,    33,   158,   126,    20,   127,   129,   130,   536,
     230,    25,    15,    16,     7,    57,   478,    31,   173,  1640,
     441,   442,    20,    20,    20,    96,  1352,    98,    33,   142,
      40,   143,   143,   127,   240,   110,   111,   112,   113,   129,
     130,   503,   220,   137,   173,   516,   517,   728,   155,   730,
     163,   732,   478,   160,    68,    69,   155,    50,    46,   740,
    1671,   160,   173,   218,   611,   129,   166,  1688,   749,   872,
     183,   192,   191,   152,   195,   102,   103,   503,   149,   173,
       0,   160,   194,   144,   145,   146,   899,     7,   102,   103,
     219,   317,   214,   215,   173,  1545,   173,   718,   192,   218,
      62,    57,   158,   197,   168,   908,   217,    63,   232,   198,
      30,   218,    32,   220,    34,    21,    22,   173,   106,   395,
      40,   220,   231,   191,   214,   215,   190,   191,   217,   143,
      50,   185,   198,   680,   166,  1396,    56,   160,   217,   171,
     217,   173,   945,   173,   176,   948,   160,   161,   162,   611,
     218,   217,   679,   680,   164,   165,   191,   218,   191,   173,
      80,   203,   233,   166,   711,   240,   197,   191,   171,   183,
     173,   198,   214,   176,   184,   185,   191,   397,   705,   243,
      34,   708,   102,   103,   198,   611,   173,   174,   175,   395,
     217,   481,   197,   191,   191,   191,   320,  1523,   192,  1176,
    1013,   215,  1175,   194,   410,   215,   496,   198,   164,    63,
     215,   127,   118,   119,   217,   686,   221,   204,   680,  1227,
     126,  1646,   128,   129,   130,   131,   647,   777,   184,   158,
     136,   652,  1657,   439,  1215,   441,   143,     5,     6,   710,
       8,   768,   127,   191,   173,   165,  1014,   311,   191,   711,
     314,   127,   137,   107,   680,  1516,  1600,   194,  1602,   127,
      53,   137,  1606,  1607,   127,   185,   173,   143,    36,   137,
     218,   165,  1697,  1698,   137,   191,   192,   127,   194,   165,
     127,   197,   136,   163,   204,   711,   191,   137,   217,   143,
     137,   185,   367,   163,  1275,   215,   202,   203,   204,   185,
     132,   848,   158,   183,    97,   199,   833,   192,   214,   215,
     516,   517,   993,   183,   194,   173,   192,   173,   185,   173,
     395,   392,   393,  1667,   192,   400,   397,   163,   399,   192,
     775,   143,   199,   165,   220,   410,   612,   613,   614,   107,
     616,   617,   192,   191,   620,   192,   622,   183,   624,   219,
     220,  1332,  1165,   185,   208,   781,   782,   783,   784,   785,
     786,   787,   788,   217,   439,   217,   441,   191,   794,   795,
     218,   143,   899,   844,   800,   595,   666,   597,  1024,   166,
     152,   217,   191,   809,   810,   157,   848,  1395,   814,   815,
     816,  1012,   818,   979,   218,   164,   165,  1405,  1294,   474,
     475,   173,   692,   191,   990,   165,   612,   613,   614,   218,
     616,   617,   962,   191,   620,   184,   622,   163,   624,   214,
     626,   492,   848,   894,   895,   185,   198,   191,   216,   185,
     650,   191,   188,   854,   654,   165,   907,   183,   965,   199,
     218,   516,   517,  1227,   163,   191,   973,   737,   869,  1159,
     191,   188,   189,   194,   218,   185,   197,   173,  1242,  1227,
     216,   191,  1230,   165,   183,   195,   937,   938,   191,   940,
     541,   542,   762,   944,  1242,   946,    68,    69,    70,   216,
     686,    21,    22,   185,  1034,  1035,  1036,   191,   559,    33,
     561,   562,   563,   195,   565,   218,  1392,  1507,   143,   982,
     191,   984,   191,   165,   710,   576,   191,   152,   173,   930,
     194,   173,   216,   197,   165,   173,    60,    61,   110,   111,
     112,   113,   593,   185,   165,   152,   597,   218,   173,   218,
    1426,   191,   165,   218,   185,   165,   607,   612,   613,   614,
     191,   616,   617,   191,   185,   620,   173,   622,   199,   624,
     191,   626,   185,  1263,   191,   185,   216,   165,   199,   192,
     191,   191,     5,     6,  1574,  1575,   173,   165,   216,   199,
    1216,   173,   643,   779,   780,   191,   173,   185,   118,   119,
     124,   218,  1592,  1593,   128,   216,   126,   185,   128,   129,
     130,   131,   165,   191,   165,   871,   136,   672,   191,   805,
     216,   199,   173,   879,   191,   165,   882,    21,    22,   152,
     177,   686,   185,   164,   185,   891,   158,   843,   191,   158,
     191,  1267,   898,   216,   830,   185,   199,   903,  1378,   216,
     173,   173,   192,   184,   173,   710,  1163,    79,   844,   929,
     195,  1651,  1652,   173,   199,  1541,   165,  1145,  1143,   193,
      21,    22,    94,   197,    33,   199,   200,    99,   878,   101,
     200,   201,   202,   203,   204,   871,   185,  1435,   177,   178,
     165,   215,   191,   879,   214,   215,   882,   221,   173,    47,
     199,    60,    61,   198,   759,   891,   208,   165,   894,   895,
     185,   195,   898,  1461,    57,   199,  1223,   903,   165,    67,
     764,   907,   217,  1174,   118,   119,   982,   185,   984,  1419,
     165,   158,   126,   191,  1241,   129,   130,   131,   185,   195,
    1247,   199,   136,   199,   191,  1146,   173,  1254,  1199,  1256,
     185,   937,   938,   198,   940,   198,   191,    57,   944,    12,
     946,   947,   165,    63,   199,   124,   165,   118,   119,   128,
      23,    24,   217,   177,   217,   126,    33,   128,   129,   130,
     131,   144,   185,   834,   839,   136,   185,   158,   191,   844,
     106,   184,   191,  1391,   187,  1302,   982,   190,   984,   850,
     199,   192,   173,    60,    61,   194,  1432,  1314,   219,   220,
     199,    57,  1319,  1214,   194,  1266,   871,    63,   195,   199,
     214,   215,   199,    57,   879,   876,   194,   882,   217,    63,
     194,   199,  1274,  1274,   193,   199,   891,   217,   197,   894,
     895,   200,   195,   898,   184,   185,   199,   187,   903,   217,
    1251,  1252,   907,   217,    57,  1585,   215,   195,    66,   185,
      63,   199,   221,   214,   215,   191,    57,   124,  1274,   194,
     127,   128,    63,  1656,   173,  1131,   194,    75,    57,   197,
     137,    79,   937,   938,    63,   940,  1393,    35,    57,   944,
      35,   946,   947,  1547,    63,    93,    94,  1645,  1646,   173,
      98,    99,   100,   101,   217,  1653,   185,  1093,  1691,  1657,
    1658,  1418,   191,   185,   185,  1583,  1116,   185,   185,   191,
     191,  1611,   195,   191,   191,   216,   199,   982,   173,   984,
     195,    92,    93,   195,   199,   192,   193,   199,   173,    22,
     197,  1689,   173,   200,   195,  1131,  1202,   195,   199,  1697,
    1698,   199,   144,   145,   146,  1521,   216,  1143,   215,   217,
    1526,   219,   220,   144,   221,   146,  1367,   177,   178,   179,
     180,   194,  1626,  1627,  1375,    13,   173,   174,   175,   194,
     217,    19,    33,   177,   178,   179,    43,    25,  1174,  1175,
      33,   198,   198,    31,   177,   178,   179,  1448,  1268,   184,
     185,   186,  1750,    41,   217,   460,   461,   462,   198,    60,
      61,    49,   198,  1199,   198,   217,  1202,    60,    61,    10,
      11,    12,   198,  1677,   198,   198,    64,   198,   198,   217,
     195,   173,   173,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,  1117,   217,   173,   218,
     216,  1122,   173,   124,  1274,  1126,  1131,   128,   198,   215,
    1266,   124,  1589,   198,  1135,   128,   198,   198,  1143,    33,
     217,  1361,   173,   217,    33,  1546,   198,  1353,   198,   198,
     166,  1628,   198,   198,   198,   143,   220,   217,  1615,    21,
      22,   217,   198,   217,   165,    10,    60,    61,   156,  1174,
    1175,    60,    61,    37,   217,   217,   217,   215,    66,   198,
     217,   198,   217,    43,   185,   173,   218,   173,   216,   194,
     191,   217,   193,    43,  1199,   217,   197,  1202,   199,   200,
     193,  1678,  1679,   198,   197,   132,   199,   200,  1428,   198,
      33,   198,   191,    14,   215,  1672,   173,  1353,   192,   194,
     221,   166,   215,   220,  1625,   184,   191,  1447,   221,    13,
     124,   219,   216,   127,   128,   124,  1628,    60,    61,   128,
    1717,   191,   191,   137,  1450,   217,     8,   218,   173,   173,
     173,   199,   191,   173,   116,   117,   118,   119,   173,   173,
     218,  1266,   192,  1469,   126,   191,   128,   129,   130,   131,
     191,   165,  1628,  1274,   136,   184,   138,   139,   108,   109,
     110,   111,   112,   113,   114,   115,  1678,  1679,   173,   218,
     218,   185,  1512,   198,     1,   217,   198,   173,   192,   193,
     217,   124,    67,   197,   193,   128,   200,   137,   197,   217,
     199,   200,  1448,   217,  1450,   199,   217,   147,   148,   149,
     217,   215,  1678,  1679,  1325,  1717,   215,   221,   199,   199,
     217,   217,   221,  1469,   217,   217,   173,   218,   200,   201,
     202,   203,   204,    43,   218,   218,   218,   216,  1353,   397,
     217,   217,   214,   215,  1355,  1561,   173,   218,   217,   407,
     218,  1717,   216,   216,   173,   173,   218,   173,   416,   217,
     193,   198,    33,   217,   197,   217,   199,   200,   426,   217,
     192,   173,   217,   173,    33,   218,   173,   216,   436,   217,
     181,   173,   215,   218,   216,    12,   173,   216,   221,    60,
      61,    33,   217,  1623,    21,    22,   217,   217,   456,   173,
    1546,    60,    61,   199,   217,    70,   218,   217,   217,   217,
     468,   217,   217,   199,   218,  1561,   199,   185,   217,  1430,
     478,   177,   217,    53,   217,   483,   216,   218,   486,   218,
     506,   507,   508,  1448,   218,  1450,   218,   495,   184,   184,
     184,   191,   218,   216,   184,   503,   504,   505,   218,   218,
     739,   218,   218,   124,  1469,   218,   216,   128,   216,   216,
     518,    85,   217,     1,   218,   124,   218,    46,   526,   128,
      88,   529,   530,   531,   532,   533,   534,   141,  1538,  1625,
     556,  1539,   839,  1011,  1539,   238,   544,     1,  1019,   116,
     117,   118,   119,   120,  1539,  1725,   123,   124,   125,   126,
    1512,   128,   129,   130,   131,  1445,  1539,   583,  1539,   136,
    1472,   138,   139,  1286,  1475,  1572,  1328,   144,   145,   146,
      56,    33,   193,   150,   592,  1573,   197,  1573,   199,   200,
     474,  1546,    33,   474,   193,   342,  1658,    -1,   197,  1519,
     199,   200,  1237,   731,   215,    -1,  1561,    -1,    60,    61,
     221,    -1,    -1,   611,    -1,    -1,   215,    -1,  1569,    60,
      61,    -1,   221,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,   640,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1612,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   684,    -1,
    1625,    -1,   124,    -1,    -1,    -1,   128,    -1,    -1,    -1,
      33,    -1,   680,   124,    -1,    -1,    -1,   128,    -1,   687,
     688,    -1,   690,    -1,    -1,    -1,    -1,   713,   696,    -1,
      -1,    -1,    -1,    -1,   702,    -1,    -1,    60,    61,    -1,
      -1,    -1,    -1,   711,    -1,    -1,  1671,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   722,   723,   724,   725,   726,   727,
    1681,   729,    -1,   731,    -1,    -1,  1687,    -1,    -1,    -1,
      -1,   193,    -1,    -1,    -1,   197,    -1,   199,   200,    -1,
      -1,    -1,   193,    -1,    -1,    -1,   197,    -1,   199,   200,
      -1,   777,    -1,   215,    -1,    -1,    -1,    -1,  1719,   221,
    1721,   124,    -1,    -1,   215,   128,    21,    22,    -1,    -1,
     221,    -1,    -1,   781,   782,   783,   784,   785,   786,   787,
     788,   789,   790,    -1,  1745,   793,   794,   795,   796,   797,
     798,    -1,   800,   801,    -1,   803,   804,   805,   806,   807,
     808,   809,   810,   811,   812,   813,   814,   815,   816,   817,
     818,   819,   820,   821,   822,   823,   824,   825,   826,   827,
     828,   829,    -1,   831,   850,    -1,    -1,   835,    -1,    -1,
     193,    -1,    33,    -1,   197,    -1,   199,   200,    -1,    -1,
     848,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,   221,    60,
      61,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,   886,    -1,
      -1,   136,    -1,   138,   139,   893,    -1,    -1,    -1,   144,
     145,   146,   900,    -1,   902,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   911,   912,   913,   914,   915,   916,   917,
     918,   919,   920,   921,   922,   923,   924,   925,   926,   927,
     928,    -1,    -1,   124,    -1,    -1,   952,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   962,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      -1,    -1,    -1,    -1,    -1,   963,   964,    -1,    -1,   214,
     215,    -1,    -1,    -1,   219,   220,    -1,    -1,    -1,   977,
      -1,   979,    -1,    -1,    -1,    -1,    -1,    -1,   986,    -1,
      -1,    -1,   990,    -1,    -1,    -1,    -1,   995,    -1,   997,
     998,   999,   193,    -1,    -1,    -1,   197,    -1,   199,   200,
      -1,    -1,  1010,    -1,    -1,    -1,    -1,    -1,  1034,  1035,
    1036,    21,    22,  1039,   215,  1041,    -1,  1043,    -1,  1045,
     221,  1047,    -1,  1049,    -1,  1051,    -1,  1053,    -1,    -1,
      -1,    -1,  1058,    -1,  1060,    -1,    -1,    -1,    -1,    -1,
    1066,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,  1078,    -1,  1080,    -1,    -1,    -1,    -1,  1085,
      -1,  1087,    -1,  1089,    -1,    -1,  1092,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,  1106,    25,
    1126,    -1,  1110,    -1,    -1,    31,   116,   117,   118,   119,
     120,    -1,    -1,   123,    -1,    41,   126,    -1,   128,   129,
     130,   131,    -1,    49,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
     124,    -1,    -1,  1151,   128,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,   199,
     200,   201,   202,   203,   204,  1203,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,    -1,   193,
      -1,    -1,    -1,   197,    -1,   199,   200,   143,    -1,    -1,
    1228,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1236,  1237,
     156,   215,    -1,    -1,    -1,    -1,    -1,   221,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,    21,
      22,    -1,    -1,    -1,    -1,    19,  1264,  1265,    -1,    -1,
      -1,    25,    -1,  1271,    -1,    -1,  1274,    31,    -1,    -1,
      -1,    -1,  1280,    -1,    -1,    -1,    -1,    41,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    49,    21,    22,  1296,    -1,
    1298,    -1,    -1,   219,    -1,  1303,    -1,    -1,    -1,    -1,
      64,  1309,    -1,    -1,    -1,  1313,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
    1358,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,  1391,   138,   139,    -1,    -1,
    1378,    -1,   144,   145,   146,    -1,    -1,    -1,   150,   143,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,   156,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   173,
      -1,    -1,  1420,  1421,  1422,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,   218,    -1,  1456,    -1,
    1458,    21,    22,    -1,    -1,   219,  1464,   221,    -1,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,    -1,  1492,    -1,    -1,  1495,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1503,  1504,  1505,    -1,    -1,
      -1,    -1,  1510,    -1,    -1,    -1,    -1,  1515,    -1,    -1,
    1518,    -1,    -1,  1521,  1522,    33,    -1,    -1,  1526,  1527,
      -1,  1529,  1530,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1539,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    60,    61,  1552,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,  1585,    -1,    -1,
     150,    -1,    -1,    -1,    -1,   782,   783,   784,   785,   786,
     787,   788,    -1,    -1,    -1,    -1,    -1,   794,   795,    -1,
    1608,    -1,    -1,   800,    -1,    -1,   124,    -1,  1616,    -1,
     128,    -1,   809,   810,    -1,    -1,    -1,   814,   815,   816,
    1628,   818,  1630,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,
    1648,  1649,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,  1681,    -1,    33,    -1,    -1,
    1668,  1669,    -1,    -1,    -1,  1673,    -1,    -1,    -1,    -1,
    1678,  1679,    -1,    -1,    -1,   193,    -1,    -1,    -1,   197,
      -1,   199,   200,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1719,    -1,  1721,     1,   215,  1706,    -1,
       5,     6,     7,   221,     9,    10,    11,    -1,    13,  1717,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,  1745,
      25,    26,    27,    28,    29,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    39,    40,    -1,    42,    -1,    44,
      45,    -1,    -1,    48,    -1,    50,    51,    52,   124,    54,
      55,    -1,   128,    58,    59,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,   119,    -1,    -1,   193,    -1,    -1,
      -1,   197,    -1,   199,   200,    -1,    -1,    -1,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   215,
      -1,    -1,    -1,    -1,    -1,   221,   151,   152,   153,   154,
     155,    -1,   157,    -1,   159,   160,   161,   162,    -1,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,   184,
     185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   200,   201,   202,    -1,   204,
      -1,    -1,   207,   208,    -1,    -1,     5,     6,    -1,    -1,
     215,    -1,   217,    -1,   219,   220,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    31,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    51,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    60,    61,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
     119,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,   128,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,    -1,    -1,    -1,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,    -1,    -1,    -1,   197,    -1,
      -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,   208,
      -1,    -1,    -1,     5,     6,    -1,   215,    -1,   217,    -1,
     219,   220,   221,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    60,    61,
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
      -1,    -1,   197,    -1,    -1,   200,   201,   202,    -1,   204,
      -1,    -1,   207,   208,    -1,    -1,    -1,     5,     6,    -1,
     215,    -1,   217,    -1,   219,   220,   221,    15,    16,    17,
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
      -1,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,   157,
      -1,   159,   160,   161,   162,    -1,    -1,    -1,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,
     208,    -1,    -1,     5,     6,    -1,    -1,   215,    -1,   217,
      -1,   219,   220,    15,    16,    17,    18,    19,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
      -1,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,   153,   154,   155,    -1,   157,    -1,   159,   160,   161,
     162,    -1,    -1,    -1,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,
     202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,     5,
       6,    -1,    -1,   215,    -1,   217,    -1,   219,   220,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    52,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,
      -1,   157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   200,   201,   202,    -1,   204,    -1,
      -1,   207,   208,    -1,    -1,     5,     6,    -1,    -1,   215,
      -1,   217,    -1,   219,   220,    15,    16,    17,    18,    19,
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
      -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   152,   153,   154,   155,    -1,   157,    -1,   159,
     160,   161,   162,    -1,    -1,    -1,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,    -1,    -1,    -1,
      -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     200,   201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,
      -1,     5,     6,    -1,    -1,   215,    -1,   217,    -1,   219,
     220,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    70,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,    -1,
      -1,    -1,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,    -1,
     204,    -1,    -1,   207,   208,    -1,    -1,     5,     6,    -1,
      -1,   215,    -1,   217,   218,   219,   220,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    33,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    60,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,   119,    -1,   124,    -1,    -1,    -1,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,   157,
      -1,   159,   160,   161,   162,    -1,    -1,    -1,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,    -1,    -1,   197,    -1,   199,   200,
      -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,
     208,    -1,    -1,    -1,   215,     5,     6,   215,    -1,   217,
     221,   219,   220,    13,    -1,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   152,   153,   154,   155,    -1,   157,    -1,   159,
     160,   161,   162,    -1,    -1,    -1,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,    -1,    -1,    -1,
      -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     200,   201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,
      -1,    -1,    -1,     5,     6,   215,   216,   217,    -1,   219,
     220,    13,    -1,    15,    16,    17,    18,    19,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,    -1,    -1,    -1,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,
     202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,    -1,
      -1,     5,     6,   215,    -1,   217,    -1,   219,   220,    13,
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
      -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,    -1,   157,    -1,   159,   160,   161,   162,    -1,
      -1,    -1,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,    -1,
     204,    -1,    -1,   207,   208,    -1,    -1,    -1,    -1,     5,
       6,   215,   216,   217,    -1,   219,   220,    13,    -1,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    -1,    -1,    -1,    55,
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
      -1,    -1,    -1,    -1,   200,   201,   202,    -1,   204,    -1,
      -1,   207,   208,    -1,    -1,     5,     6,    -1,    -1,   215,
      -1,   217,    -1,   219,   220,    15,    16,    17,    18,    19,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,    -1,    -1,    -1,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,    -1,    -1,    -1,
      -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     200,   201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,
      -1,     5,     6,    -1,    -1,   215,    -1,   217,    -1,   219,
     220,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,    -1,
      -1,    -1,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,    -1,
     204,    -1,    -1,   207,   208,    -1,    -1,     5,     6,    -1,
      -1,   215,    -1,   217,   218,   219,   220,    15,    16,    17,
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
      -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,    -1,    -1,    -1,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,
     208,    -1,    -1,     5,     6,    -1,    -1,   215,    -1,   217,
     218,   219,   220,    15,    16,    17,    18,    19,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,   153,   154,   155,    -1,   157,    -1,   159,   160,   161,
     162,    -1,    -1,    -1,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,
     202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,     5,
       6,    -1,    -1,   215,   216,   217,    -1,   219,   220,    15,
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
      -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,
      -1,   157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   200,   201,   202,    -1,   204,    -1,
      -1,   207,   208,    -1,    -1,     5,     6,    -1,    -1,   215,
      -1,   217,    -1,   219,   220,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   152,   153,   154,   155,    -1,   157,    -1,   159,
     160,   161,   162,    -1,    -1,    -1,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,    -1,    -1,    -1,
      -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     200,   201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,
      -1,    -1,    -1,     5,     6,   215,    -1,   217,    -1,   219,
     220,    13,    -1,    15,    16,    17,    18,    19,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,   153,   154,   155,    -1,   157,    -1,   159,   160,   161,
     162,    -1,    -1,    -1,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,
     202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,     5,
       6,    -1,    -1,   215,    -1,   217,    -1,   219,   220,    15,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,
      -1,   157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   198,    -1,   200,   201,   202,    -1,   204,    -1,
      -1,   207,   208,    -1,    -1,    -1,    -1,     5,     6,   215,
      -1,   217,    -1,   219,   220,    13,    -1,    15,    16,    17,
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
      -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,   157,
      -1,   159,   160,   161,   162,    -1,    -1,    -1,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,
     208,    -1,    -1,     5,     6,    -1,    -1,   215,    -1,   217,
      -1,   219,   220,    15,    16,    17,    18,    19,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,    -1,    -1,    -1,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,
     202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,     5,
       6,    -1,    -1,   215,    -1,   217,    -1,   219,   220,    15,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,
      -1,   157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   200,   201,   202,    -1,   204,    -1,
      -1,   207,   208,    -1,    -1,     5,     6,    -1,    -1,   215,
      -1,   217,    -1,   219,   220,    15,    16,    17,    18,    19,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   152,   153,   154,   155,    -1,   157,    -1,   159,
     160,   161,   162,    -1,    -1,    -1,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,    -1,    -1,    -1,
      -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     200,   201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,
      -1,     5,     6,    -1,    -1,   215,    -1,   217,   218,   219,
     220,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,    -1,   157,    -1,   159,   160,   161,   162,    -1,
      -1,    -1,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   198,    -1,   200,   201,   202,    -1,
     204,    -1,    -1,   207,   208,    -1,    -1,     5,     6,    -1,
      -1,   215,    -1,   217,    -1,   219,   220,    15,    16,    17,
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
      -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,   157,
      -1,   159,   160,   161,   162,    -1,    -1,    -1,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,
     208,    -1,    -1,     5,     6,    -1,    -1,   215,    -1,   217,
     218,   219,   220,    15,    16,    17,    18,    19,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,   153,   154,   155,    -1,   157,    -1,   159,   160,   161,
     162,    -1,    -1,    -1,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,
     202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,     5,
       6,    -1,    -1,   215,    -1,   217,   218,   219,   220,    15,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,
      -1,   157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   200,   201,   202,    -1,   204,    -1,
      -1,   207,   208,    -1,    -1,     5,     6,    -1,    -1,   215,
      -1,   217,   218,   219,   220,    15,    16,    17,    18,    19,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   152,   153,   154,   155,    -1,   157,    -1,   159,
     160,   161,   162,    -1,    -1,    -1,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,    -1,    -1,    -1,
      -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     200,   201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,
      -1,     5,     6,    -1,    -1,   215,    -1,   217,   218,   219,
     220,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,    -1,   157,    -1,   159,   160,   161,   162,    -1,
      -1,    -1,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,    -1,
     204,    -1,    -1,   207,   208,    -1,    -1,     5,     6,    -1,
      -1,   215,    -1,   217,   218,   219,   220,    15,    16,    17,
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
      -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,   157,
      -1,   159,   160,   161,   162,    -1,    -1,    -1,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,
     208,    -1,    -1,     5,     6,    -1,    -1,   215,    -1,   217,
      -1,   219,   220,    15,    16,    17,    18,    19,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,   153,   154,   155,    -1,   157,    -1,   159,   160,   161,
     162,    -1,    -1,    -1,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,
     202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,     5,
       6,    -1,    -1,   215,    -1,   217,    -1,   219,   220,    15,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,
      -1,   157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   200,   201,   202,    -1,   204,    -1,
      -1,   207,   208,    -1,    -1,     5,     6,    -1,    -1,   215,
      -1,   217,    -1,   219,   220,    15,    16,    17,    18,    19,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   152,   153,   154,   155,    -1,   157,    -1,   159,
     160,   161,   162,    -1,    -1,    -1,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,    -1,    -1,    -1,
      -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     200,   201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,
      -1,     5,     6,    -1,    -1,   215,    -1,   217,    -1,   219,
     220,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,    -1,   157,    -1,   159,   160,   161,   162,    -1,
      -1,    -1,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,    -1,
     204,    -1,    -1,   207,   208,    -1,    -1,    -1,    -1,    -1,
      19,   215,    -1,   217,    -1,   219,   220,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,    -1,    -1,    21,    22,   136,   137,   138,   139,   140,
     141,    -1,    -1,   144,   145,   146,   147,   148,   149,   150,
      -1,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   192,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,   214,   215,    -1,    31,    -1,   219,   220,
      -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,   116,   117,
     118,   119,   120,    -1,    49,   123,   124,   125,   126,   158,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    64,
     138,   139,    -1,    -1,   173,    -1,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,   197,
     198,   199,   200,   201,   202,   203,   204,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   214,   215,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     165,    -1,    -1,    19,    -1,    -1,    -1,    -1,   173,    25,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
     185,    -1,    -1,    -1,    -1,    41,   191,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,   219,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,
     156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    64,    -1,    -1,    -1,    -1,    -1,   173,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   219,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   156,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
     173,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,    -1,   146,    -1,    -1,    -1,    -1,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,    -1,    -1,    -1,   219,   136,   137,   138,
     139,   140,   141,    21,    22,   144,   145,   146,   147,   148,
     149,   150,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,   165,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,    -1,    -1,
      -1,    -1,    -1,   192,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,    -1,
     219,   220,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,    -1,    -1,    -1,   136,   137,
     138,   139,   140,   141,    -1,    -1,   144,   145,   146,   147,
     148,   149,   150,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    21,
      22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,   192,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,
      -1,   219,   220,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    21,    22,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     214,   215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    21,    22,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,
     218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    21,    22,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    21,
      22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,
     218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    21,    22,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     214,   215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    21,    22,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,
     218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    21,    22,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    21,
      22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,
     218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    21,    22,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     214,   215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    21,    22,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,
     218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    21,    22,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    21,
      22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,
     218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    21,    22,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     214,   215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    21,    22,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,
     218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    21,    22,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,   215,   216,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    21,
      22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   214,   215,   216,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   214,   215,   216,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    21,    22,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,   216,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     214,   215,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    21,
      22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    38,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   214,   215,   216,    -1,    -1,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    -1,    -1,   128,   129,   130,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
      -1,    -1,   144,   145,   146,   147,   148,   149,   150,    -1,
      -1,    -1,    -1,    -1,    21,    22,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   163,    -1,   207,   208,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    21,    22,   116,
     117,   118,   119,   120,   214,   215,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,   214,   215,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,   142,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    21,    22,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,   142,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    21,    22,    -1,    -1,    -1,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     214,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,   214,   215,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    21,    22,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    21,    22,    -1,    -1,    -1,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     214,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,   214,   215,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    21,    22,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,   185,    -1,   150,    21,    22,    -1,    -1,    -1,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     214,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,   214,   215,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    21,    22,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     214,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,   214,   215,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,    -1,    -1,    21,    22,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     214,   215,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,   116,   117,
     118,   119,   120,   214,   215,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     198,   199,   200,   201,   202,   203,   204,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    73,   173,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      35,    98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,   129,   130,    -1,    -1,    71,    -1,    73,    -1,
      75,    76,    77,    78,    79,   173,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,   158,    -1,    98,    99,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,    -1,    -1,
      -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   223,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   102,   103,   165,   185,   204,   215,   224,   228,   237,
     239,   240,   245,   252,   277,   281,   317,   399,   406,   410,
     421,   467,   472,   477,    19,    20,   173,   269,   270,   271,
     166,   246,   247,   173,   174,   175,   204,   241,   242,   243,
     163,   183,   286,   407,   173,   219,   226,    57,    63,   402,
     402,   402,   143,   173,   303,    34,    63,   107,   136,   208,
     217,   273,   274,   275,   276,   303,   252,     5,     6,     8,
      36,   107,   418,    62,   397,   192,   191,   194,   191,   242,
      22,    57,   203,   214,   244,   402,   403,   405,   403,   397,
     478,   468,   473,   173,   143,   238,   275,   275,   275,   217,
     144,   145,   146,   191,   216,    57,    63,   282,   284,    57,
      63,   408,     5,     6,    57,    63,   419,    57,    63,   398,
      15,    16,   166,   171,   173,   176,   217,   230,   270,   166,
     247,   173,   241,   241,   173,   252,   164,   184,   287,   403,
     252,    57,    63,   225,   173,   173,   173,   173,   177,   236,
     218,   271,   275,   275,   275,   275,   285,   173,   409,   422,
     286,   400,   177,   178,   179,   229,    15,    16,   166,   171,
     173,   230,   267,   268,   244,   404,   252,   208,   227,   479,
     469,   474,   177,   218,    35,    71,    73,    75,    76,    77,
      78,    79,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    93,    94,    95,    98,    99,   100,   101,
     118,   119,   173,   280,   283,   194,   286,   106,   416,   417,
     395,   160,   220,   272,   367,   177,   178,   179,   191,   218,
     192,    66,   194,   431,   286,   286,    21,    22,    38,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   128,   129,   130,
     137,   138,   139,   140,   141,   144,   145,   146,   147,   148,
     149,   150,   193,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   207,   208,   214,   215,    35,    35,   217,
     278,   286,   288,    75,    79,    93,    94,    98,    99,   100,
     101,   426,   411,   173,   423,   165,   287,   396,   271,   270,
     220,   252,   152,   173,   393,   394,   267,    19,    25,    31,
      41,    49,    64,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   156,   219,   303,   425,
     427,   428,   432,   438,   466,    79,    94,    99,   101,   286,
     470,   475,   173,   173,   173,   216,    22,   173,   216,   155,
     218,   367,   378,   379,   380,   194,   279,   291,   286,   401,
     194,   415,   286,   420,   367,   216,   270,   217,    43,   191,
     194,   197,   392,   198,   198,   198,   217,   198,   198,   217,
     431,   198,   198,   198,   198,   198,   217,   303,    33,    60,
      61,   124,   128,   193,   197,   200,   215,   221,   437,   195,
     480,   383,   386,   173,   108,   109,   110,   111,   112,   113,
     114,   115,   137,   147,   148,   149,   217,     7,    50,   316,
     252,   191,   252,   218,   466,     1,     5,     6,     9,    10,
      11,    13,    15,    16,    17,    18,    19,    25,    26,    27,
      28,    29,    31,    38,    39,    40,    42,    44,    45,    48,
      51,    52,    54,    55,    58,    59,    65,    68,    69,    80,
     102,   103,   104,   105,   118,   119,   133,   134,   135,   151,
     152,   153,   154,   155,   157,   159,   160,   161,   162,   166,
     167,   168,   169,   170,   171,   172,   174,   175,   176,   183,
     200,   201,   202,   207,   208,   215,   217,   219,   220,   235,
     237,   248,   249,   252,   253,   256,   257,   259,   261,   262,
     263,   264,   287,   289,   290,   292,   297,   302,   303,   304,
     308,   309,   310,   311,   312,   313,   314,   315,   317,   321,
     322,   329,   332,   335,   340,   343,   344,   346,   347,   348,
     350,   355,   358,   359,   366,   425,   482,   497,   508,   512,
     525,   528,   413,   173,   252,   412,   303,   373,   394,   216,
      65,   104,   174,   297,   359,   173,   173,   438,   127,   137,
     192,   391,   439,   444,   446,   359,   448,   442,   173,   198,
     450,   452,   454,   456,   458,   460,   462,   464,   359,   198,
     217,   294,    33,   197,    33,   197,   215,   221,   216,   359,
     215,   221,   438,   430,   173,   191,   252,   381,   435,   466,
     471,   173,   384,   435,   476,   359,   152,   173,   388,   389,
     424,   380,   380,   380,   198,   198,   359,   260,   198,   305,
     427,   482,   217,   303,   198,     5,   102,   103,   198,   217,
     127,   302,   333,   344,   359,   288,   198,   217,    61,   359,
     217,   266,   359,   173,   198,   198,   217,   252,   198,   166,
      58,   359,   217,   288,   198,   217,   198,   198,   217,   198,
     198,   127,   302,   359,   359,   359,   220,   288,   335,   339,
     339,   339,   217,   217,   217,   217,   217,   217,    13,   438,
      13,   438,    13,   359,   507,   523,   198,   359,   198,   234,
      13,   295,   507,   524,   359,   359,   359,   359,   359,    13,
      49,   293,   333,   359,   158,   173,   333,   483,   485,   220,
     252,   252,   359,    10,    37,   335,   341,   173,   217,   252,
     252,   252,   252,   252,    66,   318,   277,   132,   252,    21,
      22,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   136,   137,   138,   139,   140,
     141,   144,   145,   146,   147,   148,   149,   150,   192,   193,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     214,   215,   341,   217,   401,   192,   252,   287,    40,   215,
     252,   277,   287,   375,   198,   218,    43,   252,   391,   302,
     359,   466,   466,   436,   466,   218,   466,   466,   218,   433,
     466,   278,   466,   278,   466,   278,   381,   382,   384,   385,
     218,   441,   293,   216,   216,   359,   173,   252,   481,   194,
     435,   287,   194,   435,   287,   218,   217,    43,   127,   191,
     192,   194,   197,   387,   498,   500,   288,   424,   306,   217,
     303,   198,   217,   330,   198,   198,   198,   519,   333,   302,
     333,   191,   108,   109,   110,   111,   112,   113,   114,   115,
     121,   122,   127,   140,   141,   147,   148,   149,   192,    14,
     438,   295,   359,   359,   173,   288,   192,   323,   325,   359,
     327,   194,   166,   359,   521,   333,   504,   509,   333,   502,
     438,   302,   359,   220,   277,   359,   359,   359,   359,   359,
     359,   424,    53,   200,   215,   217,   359,   483,   486,   490,
     506,   511,   424,   217,   486,   511,   424,   142,   184,   185,
     186,   491,   298,   288,   300,   179,   180,   229,   424,   184,
     191,   527,   424,    13,   216,   191,   527,   217,   137,   387,
     527,   191,   527,   218,   152,   157,   198,   303,   349,   288,
     258,   286,   342,    70,   215,   218,   333,   485,   160,   217,
     320,   394,     4,   160,   338,   339,    19,   158,   173,   425,
      19,   158,   173,   425,   133,   134,   135,   289,   345,   359,
     345,   359,   345,   359,   345,   359,   345,   359,   345,   359,
     345,   359,   345,   359,   359,   359,   359,   345,   359,   345,
     359,   359,   359,   359,   173,   345,   359,   359,   158,   173,
     359,   359,   359,   425,   359,   359,   359,   345,   359,   345,
     359,   359,   359,   359,   345,   359,   345,   359,   345,   359,
     359,   345,   359,    22,   359,   359,   359,   359,   359,   359,
     359,   359,   359,   359,   359,   129,   130,   158,   173,   214,
     215,   356,   425,   359,   218,   333,   414,   359,   374,   276,
       8,   367,   372,   438,   173,   302,   359,   252,   199,   199,
     199,   435,   199,   199,   173,   429,   199,   279,   199,   279,
     199,   279,   199,   435,   199,   435,   296,   466,   191,   527,
     216,   192,   252,   287,   466,   466,   359,   173,   173,   466,
     359,   438,   438,    20,   466,    70,   333,   485,   496,   198,
     359,   173,   359,   466,   513,   515,   517,   438,   527,   359,
     359,   359,   359,   359,   359,   359,   359,   359,   359,   359,
     359,   359,   359,   359,   359,   359,   359,   288,   199,   435,
     218,   218,   192,   265,   438,   438,   218,   438,   218,   438,
     527,   438,   382,   527,   385,   199,   338,   218,   218,   218,
     218,   218,   218,    20,   339,   215,   359,   218,   142,   191,
     185,   490,   188,   189,   216,   494,   191,   185,   188,   216,
     493,    20,   218,   490,   184,   187,   492,    20,   359,   184,
     507,   296,   296,   359,    20,   507,    20,   424,   359,   359,
     359,   218,   158,   173,   217,   217,   351,   353,    12,    23,
      24,   250,   251,   359,   291,   277,   173,   218,   485,   483,
     191,   218,   218,   173,   319,   319,   217,   127,   137,   173,
     192,   197,   336,   337,   278,   198,   217,   198,   217,   339,
     339,   339,   217,   217,   216,    19,   158,   173,   425,   194,
     158,   173,   359,   217,   217,   158,   173,   359,     1,   217,
     216,   218,   287,   252,   173,   216,    57,    63,   370,    67,
     371,   252,   199,   252,   440,   445,   447,   466,   449,   443,
     199,   252,   451,   199,   455,   199,   459,   199,   463,   381,
     465,   384,   199,   435,   218,   359,   218,    43,   387,   199,
     199,   333,   199,   485,   218,   218,   218,   173,   218,   185,
     199,   218,   199,   438,   382,   385,   199,   218,   217,   438,
     466,   359,   199,   199,   199,   199,   218,   199,   199,   218,
     199,   338,   278,   217,   333,   486,   490,   359,   483,   494,
     216,   359,   506,   216,   333,   486,   184,   187,   190,   495,
     216,   333,   199,   199,   194,   232,   333,   333,    20,   218,
     217,   137,   387,   359,   359,   438,   278,   288,   359,    12,
     254,   287,   338,   218,   216,   215,   191,   216,   218,   337,
     173,   173,   217,   173,   173,   191,   216,   279,   360,   359,
     362,   359,   218,   333,   359,   198,   217,   359,   217,   216,
     359,   215,   218,   333,   217,   216,   357,   218,   333,   192,
     252,    47,   371,    46,   106,   368,   338,   434,   173,   453,
     457,   461,   294,   466,   252,   173,   359,   499,   501,   288,
     307,   218,   199,   435,   173,   217,   331,   199,   199,   199,
     520,   295,   199,   324,   326,   328,   522,   505,   510,   503,
     217,   341,   279,   218,   333,   185,   490,   494,   185,   490,
     216,   185,   299,   301,   233,   181,   185,   185,   333,   137,
     387,   359,   359,   359,   218,   218,   199,   279,   288,   255,
     252,   278,   218,   483,   173,   216,   194,   392,   218,   173,
     336,   216,   142,   288,   334,   438,   218,   466,   218,   218,
     218,   364,   359,   359,   218,   483,   218,   359,   218,   466,
      33,   369,   368,   370,   217,   217,   359,   173,   199,   359,
     514,   516,   518,   217,   218,   217,   359,   359,   359,   217,
      70,   496,   217,   217,   218,   359,   334,   218,   359,   494,
     359,   495,   507,   359,   294,   231,   507,   359,   185,   359,
     359,   218,   352,   199,   251,    26,   105,   256,   309,   310,
     311,   313,   359,   279,   216,   194,   392,   438,   391,   218,
     127,   359,   199,   199,   466,   218,   218,   216,   218,   252,
     376,   369,   388,   389,   390,   496,   496,   218,   199,   217,
     218,   217,   217,   217,   293,   295,   333,   496,   496,   218,
     185,   526,   526,   526,   177,   526,   526,   359,   137,   387,
     349,   354,   127,   127,   359,   288,   218,   438,   391,   391,
     302,   359,   359,   361,   363,   199,   218,   283,   377,   217,
     483,   487,   488,   489,   489,   359,   359,   496,   496,   483,
     484,   218,   218,   527,   489,   484,    53,   216,   184,   184,
     184,   216,   526,   359,   359,   349,   359,   391,   302,   359,
     302,   359,   252,   365,   252,   283,   483,   191,   527,   218,
     218,   218,   218,   489,   489,   218,   218,   218,   218,   359,
     216,   216,   184,   216,   302,   359,   252,   252,   288,   218,
     217,   218,   218,   252,   483,   218
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
     267,   268,   268,   269,   269,   269,   270,   270,   270,   270,
     270,   270,   270,   270,   271,   271,   272,   272,   272,   273,
     273,   273,   273,   274,   274,   275,   275,   275,   275,   275,
     275,   275,   276,   276,   277,   277,   278,   278,   278,   279,
     279,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   281,   282,   282,
     282,   283,   285,   284,   286,   286,   287,   287,   288,   288,
     289,   289,   289,   290,   290,   290,   290,   290,   290,   290,
     290,   290,   290,   290,   290,   290,   290,   290,   290,   290,
     290,   290,   290,   290,   291,   291,   291,   292,   293,   293,
     294,   294,   295,   295,   296,   296,   298,   299,   297,   300,
     301,   297,   302,   302,   302,   302,   302,   303,   303,   303,
     304,   304,   306,   307,   305,   305,   308,   308,   308,   308,
     308,   308,   309,   310,   311,   311,   311,   312,   312,   312,
     313,   313,   314,   314,   314,   315,   316,   316,   316,   317,
     317,   318,   318,   319,   319,   320,   320,   320,   320,   320,
     320,   320,   320,   321,   321,   323,   324,   322,   325,   326,
     322,   327,   328,   322,   330,   331,   329,   332,   332,   332,
     332,   332,   332,   333,   333,   334,   334,   334,   335,   335,
     335,   336,   336,   336,   336,   336,   337,   337,   338,   338,
     338,   339,   339,   340,   342,   341,   343,   343,   343,   343,
     343,   343,   343,   344,   344,   344,   344,   344,   344,   344,
     344,   344,   344,   344,   344,   344,   344,   344,   344,   344,
     344,   344,   345,   345,   345,   345,   346,   346,   346,   346,
     346,   346,   346,   346,   346,   346,   346,   346,   346,   346,
     346,   346,   346,   347,   347,   348,   348,   349,   349,   350,
     351,   352,   350,   353,   354,   350,   355,   355,   355,   355,
     355,   355,   355,   356,   357,   355,   358,   358,   358,   358,
     358,   358,   358,   359,   359,   359,   359,   359,   359,   359,
     359,   359,   359,   359,   359,   359,   359,   359,   359,   359,
     359,   359,   359,   359,   359,   359,   359,   359,   359,   359,
     359,   359,   359,   359,   359,   359,   359,   359,   359,   359,
     359,   359,   359,   359,   359,   359,   359,   359,   359,   359,
     359,   359,   359,   359,   359,   359,   359,   359,   359,   359,
     360,   361,   359,   359,   359,   359,   362,   363,   359,   359,
     359,   364,   365,   359,   359,   359,   359,   359,   359,   359,
     359,   359,   359,   359,   359,   359,   359,   359,   366,   366,
     366,   366,   366,   366,   366,   366,   366,   366,   366,   366,
     366,   366,   366,   366,   367,   367,   367,   368,   368,   368,
     369,   369,   370,   370,   370,   371,   371,   372,   373,   373,
     374,   373,   375,   373,   376,   373,   377,   373,   373,   378,
     379,   379,   380,   380,   380,   380,   380,   381,   381,   382,
     382,   383,   383,   383,   384,   385,   385,   386,   386,   386,
     387,   387,   388,   388,   388,   389,   389,   390,   390,   391,
     391,   391,   392,   392,   393,   393,   393,   393,   393,   394,
     394,   394,   394,   394,   395,   395,   396,   395,   397,   397,
     398,   398,   398,   399,   400,   399,   401,   401,   401,   401,
     402,   402,   402,   404,   403,   405,   405,   406,   407,   406,
     408,   408,   408,   409,   411,   412,   410,   413,   414,   410,
     415,   415,   416,   416,   417,   418,   418,   418,   418,   419,
     419,   419,   420,   420,   422,   423,   421,   424,   424,   424,
     424,   424,   425,   425,   425,   425,   425,   425,   425,   425,
     425,   425,   425,   425,   425,   425,   425,   425,   425,   425,
     425,   425,   425,   425,   425,   425,   425,   425,   425,   426,
     426,   426,   426,   426,   426,   426,   426,   427,   428,   428,
     428,   429,   429,   430,   430,   430,   430,   431,   431,   431,
     431,   431,   433,   434,   432,   435,   435,   436,   436,   437,
     437,   438,   438,   438,   438,   438,   438,   439,   440,   438,
     438,   438,   441,   438,   438,   438,   438,   438,   438,   438,
     438,   438,   438,   438,   438,   438,   442,   443,   438,   438,
     444,   445,   438,   446,   447,   438,   448,   449,   438,   438,
     450,   451,   438,   452,   453,   438,   438,   454,   455,   438,
     456,   457,   438,   438,   458,   459,   438,   460,   461,   438,
     462,   463,   438,   464,   465,   438,   466,   466,   466,   468,
     469,   470,   471,   467,   473,   474,   475,   476,   472,   478,
     479,   480,   481,   477,   482,   482,   482,   482,   482,   483,
     483,   483,   483,   483,   483,   483,   483,   484,   484,   485,
     486,   486,   487,   487,   488,   488,   489,   489,   490,   490,
     491,   491,   492,   492,   493,   493,   494,   494,   494,   495,
     495,   495,   496,   496,   497,   497,   497,   497,   497,   497,
     498,   499,   497,   500,   501,   497,   502,   503,   497,   504,
     505,   497,   506,   506,   506,   507,   507,   508,   509,   510,
     508,   511,   511,   512,   512,   512,   513,   514,   512,   515,
     516,   512,   517,   518,   512,   512,   519,   520,   512,   512,
     521,   522,   512,   523,   523,   524,   524,   525,   525,   525,
     525,   525,   526,   526,   527,   527,   528,   528,   528,   528,
     528,   528
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
       3,     0,     5,     0,     5,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     1,     5,     1,     3,     2,     3,     2,     1,
       1,     1,     1,     1,     4,     1,     2,     3,     3,     3,
       3,     2,     1,     3,     0,     3,     0,     2,     3,     0,
       2,     1,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     3,     3,     2,     2,     3,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     3,     2,     2,     2,     2,     2,     3,     3,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     0,     1,
       1,     3,     0,     4,     1,     1,     1,     1,     3,     7,
       2,     2,     6,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     2,     2,     1,     1,     1,
       1,     2,     2,     2,     0,     2,     2,     3,     0,     2,
       0,     4,     0,     2,     1,     3,     0,     0,     7,     0,
       0,     7,     3,     2,     2,     2,     1,     1,     3,     2,
       2,     3,     0,     0,     5,     1,     2,     5,     5,     5,
       6,     2,     1,     1,     1,     2,     3,     2,     2,     3,
       2,     3,     2,     2,     3,     4,     1,     1,     0,     1,
       1,     1,     0,     1,     3,     9,     8,     8,     7,     8,
       7,     7,     6,     3,     3,     0,     0,     7,     0,     0,
       7,     0,     0,     7,     0,     0,     6,     5,     8,    10,
       5,     8,    10,     1,     3,     1,     2,     3,     1,     1,
       2,     2,     2,     2,     2,     4,     1,     3,     0,     4,
       4,     1,     6,     6,     0,     7,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     2,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     6,     8,     5,     6,     1,     4,     3,
       0,     0,     8,     0,     0,     9,     3,     4,     5,     6,
       8,     5,     6,     0,     0,     5,     3,     4,     4,     5,
       4,     3,     4,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     2,     2,     4,     3,     4,     5,     4,     5,     3,
       4,     1,     1,     2,     4,     4,     7,     8,     3,     5,
       0,     0,     8,     3,     3,     3,     0,     0,     8,     3,
       4,     0,     0,     9,     4,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     2,     4,     1,     4,     4,
       4,     4,     4,     1,     6,     7,     6,     6,     7,     7,
       6,     7,     6,     6,     0,     4,     1,     0,     1,     1,
       0,     1,     0,     1,     1,     0,     1,     5,     0,     2,
       0,     7,     0,     4,     0,     9,     0,    10,     5,     3,
       3,     4,     1,     1,     3,     3,     3,     1,     3,     1,
       3,     0,     2,     3,     3,     1,     3,     0,     2,     3,
       1,     1,     1,     2,     3,     3,     5,     1,     1,     1,
       1,     1,     0,     1,     1,     4,     3,     3,     5,     4,
       6,     5,     5,     4,     0,     2,     0,     4,     0,     1,
       0,     1,     1,     6,     0,     6,     0,     2,     3,     5,
       0,     1,     1,     0,     5,     2,     3,     4,     0,     4,
       0,     1,     1,     1,     0,     0,     9,     0,     0,    11,
       0,     2,     0,     1,     3,     1,     1,     2,     2,     0,
       1,     1,     0,     3,     0,     0,     7,     1,     4,     3,
       3,     5,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       4,     1,     3,     0,     2,     3,     5,     0,     2,     2,
       2,     2,     0,     0,     7,     1,     1,     1,     3,     3,
       4,     1,     1,     1,     1,     2,     3,     0,     0,     6,
       4,     3,     0,     7,     4,     2,     2,     3,     2,     3,
       2,     2,     3,     3,     3,     2,     0,     0,     6,     2,
       0,     0,     6,     0,     0,     6,     0,     0,     6,     1,
       0,     0,     6,     0,     0,     7,     1,     0,     0,     6,
       0,     0,     7,     1,     0,     0,     6,     0,     0,     7,
       0,     0,     6,     0,     0,     6,     1,     3,     3,     0,
       0,     0,     0,    10,     0,     0,     0,     0,    10,     0,
       0,     0,     0,    11,     1,     1,     1,     1,     1,     3,
       3,     5,     5,     6,     6,     8,     8,     0,     1,     2,
       1,     3,     3,     5,     1,     2,     1,     0,     0,     2,
       2,     1,     2,     1,     2,     1,     2,     1,     1,     2,
       1,     1,     0,     1,     5,     4,     6,     7,     5,     7,
       0,     0,    10,     0,     0,    10,     0,     0,    10,     0,
       0,     7,     1,     3,     3,     3,     1,     5,     0,     0,
      10,     1,     3,     3,     4,     4,     0,     0,    11,     0,
       0,    11,     0,     0,    10,     5,     0,     0,     9,     5,
       0,     0,    10,     1,     3,     1,     3,     3,     3,     4,
       7,     9,     0,     3,     0,     1,     9,    10,    10,    10,
       9,    10
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

  case 97: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 98: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 99: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 100: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 101: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 102: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 103: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 104: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 105: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 106: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 107: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 108: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 109: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 110: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 111: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 112: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 113: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 114: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 115: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 116: /* metadata_argument_list: '@' annotation_argument  */
                                      {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 117: /* metadata_argument_list: metadata_argument_list '@' annotation_argument  */
                                                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 118: /* metadata_argument_list: metadata_argument_list semicolon  */
                                               {
        (yyval.aaList) = (yyvsp[-1].aaList);
    }
    break;

  case 119: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 120: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 121: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 122: /* annotation_declaration_name: "template"  */
                                    { (yyval.s) = new string("template"); }
    break;

  case 123: /* annotation_declaration_basic: annotation_declaration_name  */
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

  case 124: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 125: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 126: /* annotation_declaration: '!' annotation_declaration  */
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

  case 127: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 128: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 129: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 130: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 131: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 132: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 133: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 134: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 135: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 136: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 137: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 138: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 139: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 140: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 141: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 142: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 143: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 144: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 145: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 146: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 147: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 148: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 149: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 150: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 151: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 152: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 153: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 154: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 155: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 156: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 157: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 158: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 159: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 160: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 161: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 162: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 163: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 164: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 165: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 166: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 167: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 168: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 169: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 170: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 171: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 172: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 173: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 174: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 175: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 176: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 177: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 178: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 179: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 180: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 181: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 182: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 183: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 184: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 185: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 186: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 187: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 188: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 189: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 190: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 191: /* function_name: "operator" '.' "name" "+="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`+="); delete (yyvsp[-1].s); }
    break;

  case 192: /* function_name: "operator" '.' "name" "-="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`-="); delete (yyvsp[-1].s); }
    break;

  case 193: /* function_name: "operator" '.' "name" "*="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`*="); delete (yyvsp[-1].s); }
    break;

  case 194: /* function_name: "operator" '.' "name" "/="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`/="); delete (yyvsp[-1].s); }
    break;

  case 195: /* function_name: "operator" '.' "name" "%="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`%="); delete (yyvsp[-1].s); }
    break;

  case 196: /* function_name: "operator" '.' "name" "&="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&="); delete (yyvsp[-1].s); }
    break;

  case 197: /* function_name: "operator" '.' "name" "|="  */
                                          { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`|="); delete (yyvsp[-1].s); }
    break;

  case 198: /* function_name: "operator" '.' "name" "^="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^="); delete (yyvsp[-1].s); }
    break;

  case 199: /* function_name: "operator" '.' "name" "&&="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&&="); delete (yyvsp[-1].s); }
    break;

  case 200: /* function_name: "operator" '.' "name" "||="  */
                                            { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`||="); delete (yyvsp[-1].s); }
    break;

  case 201: /* function_name: "operator" '.' "name" "^^="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^^="); delete (yyvsp[-1].s); }
    break;

  case 202: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 203: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 204: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 205: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 206: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 207: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 208: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 209: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 210: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 211: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 212: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 213: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 214: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 215: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 216: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 217: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 218: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 219: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 220: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 221: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 222: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 223: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 224: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 225: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 226: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 227: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 228: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 229: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 230: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 231: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 232: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 233: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 234: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 235: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 236: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 237: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 238: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 239: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 240: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 241: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 242: /* $@8: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 243: /* function_declaration: optional_public_or_private_function $@8 function_declaration_header expression_block  */
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

  case 248: /* expression_block: open_block expressions close_block  */
                                                                  {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 249: /* expression_block: open_block expressions close_block "finally" open_block expressions close_block  */
                                                                                                                        {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 250: /* expr_call_pipe: expr expr_full_block_assumed_piped  */
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

  case 251: /* expr_call_pipe: expression_keyword expr_full_block_assumed_piped  */
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

  case 252: /* expr_call_pipe: "generator" '<' type_declaration_no_options '>' optional_capture_list expr_full_block_assumed_piped  */
                                                                                                                                             {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-3].pTypeDecl),(yyvsp[-1].pCaptList),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 253: /* expression_any: semicolon  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 254: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 255: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 256: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 257: /* expression_any: expr_assign semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 258: /* expression_any: expression_delete semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 259: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 260: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 261: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 262: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 263: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 264: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 265: /* expression_any: expression_break semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 266: /* expression_any: expression_continue semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 267: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 268: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 269: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 270: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 271: /* expression_any: expression_label semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 272: /* expression_any: expression_goto semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 273: /* expression_any: "pass" semicolon  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 274: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 275: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back((yyvsp[0].pExpression));
        }
    }
    break;

  case 276: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 277: /* expr_keyword: "keyword" expr expression_block  */
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

  case 278: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 279: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 280: /* optional_expr_list_in_braces: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 281: /* optional_expr_list_in_braces: '(' optional_expr_list optional_comma ')'  */
                                                             { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 282: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 283: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 284: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 285: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 286: /* $@9: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 287: /* $@10: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 288: /* expression_keyword: "keyword" '<' $@9 type_declaration_no_options_list '>' $@10 expr  */
                                                                                                                                                     {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 289: /* $@11: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 290: /* $@12: %empty  */
                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 291: /* expression_keyword: "type function" '<' $@11 type_declaration_no_options_list '>' $@12 optional_expr_list_in_braces  */
                                                                                                                                                                                   {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 292: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 293: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 294: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 295: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 296: /* expr_pipe: expr_call_pipe  */
                             {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 297: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 298: /* name_in_namespace: "name" "::" "name"  */
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

  case 299: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 300: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
    }
    break;

  case 301: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 302: /* $@13: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 303: /* $@14: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 304: /* new_type_declaration: '<' $@13 type_declaration '>' $@14  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 305: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 306: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pTypeDecl),false);
    }
    break;

  case 307: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 308: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 309: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),(yyvsp[-1].pExpression));
    }
    break;

  case 310: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),(yyvsp[-1].pExpression));
    }
    break;

  case 311: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 312: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 313: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 314: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 315: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 316: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 317: /* expression_return: expression_return_no_pipe semicolon  */
                                                    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 318: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 319: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 320: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 321: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 322: /* expression_yield: expression_yield_no_pipe semicolon  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 323: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 324: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 325: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 326: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 327: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 328: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 329: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 330: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 331: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 332: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 333: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 334: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 335: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-7].pNameList),tokAt(scanner,(yylsp[-7])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 336: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 337: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 338: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                           {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameList),tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 339: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr semicolon  */
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

  case 340: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr_pipe  */
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

  case 341: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr semicolon  */
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

  case 342: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr_pipe  */
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

  case 343: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 344: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 345: /* $@15: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 346: /* $@16: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 347: /* expr_cast: "cast" '<' $@15 type_declaration_no_options '>' $@16 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
    }
    break;

  case 348: /* $@17: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 349: /* $@18: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 350: /* expr_cast: "upcast" '<' $@17 type_declaration_no_options '>' $@18 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 351: /* $@19: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 352: /* $@20: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 353: /* expr_cast: "reinterpret" '<' $@19 type_declaration_no_options '>' $@20 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 354: /* $@21: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 355: /* $@22: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 356: /* expr_type_decl: "type" '<' $@21 type_declaration '>' $@22  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 357: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 358: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 359: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" c_or_s "name" '>' expr ')'  */
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

  case 360: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
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

  case 361: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
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

  case 362: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" "end of expression" "name" '>' '(' expr ')'  */
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

  case 363: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 364: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 365: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 366: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 367: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 368: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 369: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 370: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 371: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 372: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 373: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 374: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 375: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 376: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 377: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 378: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 379: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 380: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 381: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),(yyvsp[0].pExpression));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 382: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 383: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 384: /* $@23: %empty  */
                             {  yyextra->das_need_oxford_comma = false; }
    break;

  case 385: /* expr_full_block_assumed_piped: block_or_lambda $@23 optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 386: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 387: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 388: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 389: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 390: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 391: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 392: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 393: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 394: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 395: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 396: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 397: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 398: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 399: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 400: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 401: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 402: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 403: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 404: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 405: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 406: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 407: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 408: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 409: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 410: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 411: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 412: /* expr_assign_pipe_right: "@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 413: /* expr_assign_pipe_right: "@@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 414: /* expr_assign_pipe_right: "$ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 415: /* expr_assign_pipe_right: expr_call_pipe  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 416: /* expr_assign_pipe: expr '=' expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 417: /* expr_assign_pipe: expr "<-" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 418: /* expr_assign_pipe: expr "&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 419: /* expr_assign_pipe: expr "|=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 420: /* expr_assign_pipe: expr "^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 421: /* expr_assign_pipe: expr "&&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 422: /* expr_assign_pipe: expr "||=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 423: /* expr_assign_pipe: expr "^^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 424: /* expr_assign_pipe: expr "+=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 425: /* expr_assign_pipe: expr "-=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 426: /* expr_assign_pipe: expr "*=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 427: /* expr_assign_pipe: expr "/=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 428: /* expr_assign_pipe: expr "%=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 429: /* expr_assign_pipe: expr "<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 430: /* expr_assign_pipe: expr ">>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 431: /* expr_assign_pipe: expr "<<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 432: /* expr_assign_pipe: expr ">>>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 433: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 434: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 435: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 436: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 437: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 438: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 439: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 440: /* $@24: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 441: /* $@25: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 442: /* func_addr_expr: '@' '@' '<' $@24 type_declaration_no_options '>' $@25 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = (yyvsp[-3].pTypeDecl);
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 443: /* $@26: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 444: /* $@27: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 445: /* func_addr_expr: '@' '@' '<' $@26 optional_function_argument_list optional_function_type '>' $@27 func_addr_name  */
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

  case 446: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 447: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 448: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 449: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 450: /* expr_field: expr '.' "name" '(' '[' make_struct_fields ']' ')'  */
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

  case 451: /* expr_field: expr '.' basic_type_declaration '(' ')'  */
                                                                        {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), method_name);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 452: /* expr_field: expr '.' basic_type_declaration '(' expr_list ')'  */
                                                                                             {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), method_name);
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 453: /* $@28: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 454: /* $@29: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 455: /* expr_field: expr '.' $@28 error $@29  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), "");
        yyerrok;
    }
    break;

  case 456: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 457: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
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

  case 458: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
                                                               {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-3].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 459: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
                                                                                 {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-4])),*(yyvsp[-4].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-4].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 460: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 461: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 462: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 463: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 464: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 465: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 466: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 467: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 468: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 469: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 470: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 471: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 472: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 473: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",(yyvsp[0].pExpression)); }
    break;

  case 474: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",(yyvsp[0].pExpression)); }
    break;

  case 475: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",(yyvsp[0].pExpression)); }
    break;

  case 476: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",(yyvsp[0].pExpression)); }
    break;

  case 477: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 478: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 479: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 480: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 481: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 482: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 483: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 484: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 485: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 486: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 487: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 488: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 489: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 490: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 491: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 492: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 493: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 494: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 495: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 496: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 497: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 498: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back((yyvsp[-2].pExpression));
        itv->arguments.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = itv;
    }
    break;

  case 499: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", (yyvsp[0].pExpression)); }
    break;

  case 500: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", (yyvsp[0].pExpression)); }
    break;

  case 501: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", (yyvsp[-1].pExpression)); }
    break;

  case 502: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", (yyvsp[-1].pExpression)); }
    break;

  case 503: /* expr: '(' expr_list optional_comma ')'  */
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

  case 504: /* expr: '(' make_struct_single ')'  */
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

  case 505: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 506: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 507: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 508: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 509: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 510: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 511: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 512: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 513: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression)); }
    break;

  case 514: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 515: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 516: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 517: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 518: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 519: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
        }
    break;

  case 520: /* $@30: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 521: /* $@31: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 522: /* expr: expr "is" "type" '<' $@30 type_declaration_no_options '>' $@31  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 523: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),vdecl);
    }
    break;

  case 524: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 525: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 526: /* $@32: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 527: /* $@33: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 528: /* expr: expr "as" "type" '<' $@32 type_declaration '>' $@33  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 529: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 530: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 531: /* $@34: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 532: /* $@35: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 533: /* expr: expr '?' "as" "type" '<' $@34 type_declaration '>' $@35  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-8].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 534: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 535: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 536: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 537: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 538: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 539: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 540: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 541: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 542: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 543: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 544: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 545: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 546: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 547: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 548: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 549: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 550: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 551: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 552: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 553: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 554: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 555: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 556: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 557: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 558: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 559: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 560: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 561: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-6].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 562: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 563: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 564: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 565: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); /*this one is gone when BRABRA is disabled*/ }
    break;

  case 566: /* optional_field_annotation: metadata_argument_list  */
                                                        { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 567: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 568: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 569: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 570: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 571: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 572: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 573: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 574: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 575: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 576: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 577: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 578: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 579: /* struct_variable_declaration_list: struct_variable_declaration_list semicolon  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 580: /* $@36: %empty  */
                                                               { yyextra->das_force_oxford_comma=true;}
    break;

  case 581: /* struct_variable_declaration_list: struct_variable_declaration_list "typedef" $@36 "name" '=' type_declaration semicolon  */
                                                                                                                                                         {
        (yyval.pVarDeclList) = (yyvsp[-6].pVarDeclList);
        ast_structureAlias(scanner,(yyvsp[-3].s),(yyvsp[-1].pTypeDecl),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 582: /* $@37: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 583: /* struct_variable_declaration_list: struct_variable_declaration_list $@37 structure_variable_declaration semicolon  */
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

  case 584: /* $@38: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 585: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@38 function_declaration_header semicolon  */
                                                          {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 586: /* $@39: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 587: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@39 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 588: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' semicolon  */
                                                                                       {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 589: /* function_argument_declaration_no_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_no_type  */
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

  case 590: /* function_argument_declaration_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_type  */
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

  case 591: /* function_argument_declaration_type: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))));
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 592: /* function_argument_list: function_argument_declaration_no_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 593: /* function_argument_list: function_argument_declaration_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 594: /* function_argument_list: function_argument_declaration_no_type semicolon function_argument_list  */
                                                                                            { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 595: /* function_argument_list: function_argument_declaration_type semicolon function_argument_list  */
                                                                                            { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 596: /* function_argument_list: function_argument_declaration_type ',' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 597: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 598: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 599: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 600: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                          { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 601: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 602: /* tuple_alias_type_list: tuple_alias_type_list c_or_s  */
                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 603: /* tuple_alias_type_list: tuple_alias_type_list tuple_type c_or_s  */
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

  case 604: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 605: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 606: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 607: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 608: /* variant_alias_type_list: variant_alias_type_list c_or_s  */
                                           {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 609: /* variant_alias_type_list: variant_alias_type_list variant_type c_or_s  */
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

  case 610: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 611: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 612: /* variable_declaration_no_type: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 613: /* variable_declaration_no_type: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 614: /* variable_declaration_no_type: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 615: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 616: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 617: /* variable_declaration: variable_declaration_type  */
                                        {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 618: /* variable_declaration: variable_declaration_no_type  */
                                           {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 619: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 620: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 621: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 622: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 623: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 624: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 625: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 626: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 627: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 628: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 629: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options semicolon  */
                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 630: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 631: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 632: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr semicolon  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 633: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 634: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 635: /* global_variable_declaration_list: global_variable_declaration_list "end of line"  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 636: /* $@40: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 637: /* global_variable_declaration_list: global_variable_declaration_list $@40 optional_field_annotation let_variable_declaration  */
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

  case 638: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 639: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 640: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 641: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 642: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 643: /* global_let: kwd_let optional_shared optional_public_or_private_variable open_block global_variable_declaration_list close_block  */
                                                                                                                                                      {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 644: /* $@41: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 645: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@41 optional_field_annotation let_variable_declaration  */
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

  case 646: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 647: /* enum_list: enum_list semicolon  */
                                {
        (yyval.pEnum) = (yyvsp[-1].pEnum);
    }
    break;

  case 648: /* enum_list: enum_list "name" semicolon  */
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

  case 649: /* enum_list: enum_list "name" '=' expr semicolon  */
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

  case 650: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 651: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 652: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 653: /* $@42: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 654: /* single_alias: optional_public_or_private_alias "name" $@42 '=' type_declaration  */
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

  case 658: /* $@43: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 660: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 661: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 662: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 663: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 664: /* $@44: %empty  */
                                                                                                                       {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
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

  case 666: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name open_block $@44 enum_list $@45 close_block  */
                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-2].pEnum),Type::tInt);
    }
    break;

  case 667: /* $@46: %empty  */
                                                                                                                                                            {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 668: /* $@47: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 669: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration open_block $@46 enum_list $@47 close_block  */
                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-2].pEnum),(yyvsp[-5].type));
    }
    break;

  case 670: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 671: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 672: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 673: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 674: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 675: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 676: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 677: /* class_or_struct: "template" "class"  */
                                 { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 678: /* class_or_struct: "template" "struct"  */
                                 { (yyval.i) = CorS_StructTemplate; }
    break;

  case 679: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 680: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 681: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 682: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 683: /* optional_struct_variable_declaration_list: open_block struct_variable_declaration_list close_block  */
                                                                      {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 684: /* $@48: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 685: /* $@49: %empty  */
                         {
        if ( (yyvsp[0].pStructure) ) {
            (yyvsp[0].pStructure)->isClass = (yyvsp[-3].i)==CorS_Class || (yyvsp[-3].i)==CorS_ClassTemplate;
            (yyvsp[0].pStructure)->isTemplate = (yyvsp[-3].i)==CorS_ClassTemplate || (yyvsp[-3].i)==CorS_StructTemplate;
            (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b);
        }
    }
    break;

  case 686: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@48 structure_name $@49 optional_struct_variable_declaration_list  */
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

  case 687: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 688: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 689: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 690: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 691: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 692: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 693: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 694: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 695: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 696: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 697: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 698: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 699: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 700: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 701: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 702: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 703: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 704: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 705: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 706: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 707: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 708: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 709: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 710: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 711: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 712: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 713: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 714: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 715: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 716: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 717: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 718: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 719: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 720: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 721: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 722: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 723: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 724: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 725: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 726: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 727: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 728: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 729: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 730: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 731: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 732: /* bitfield_bits: bitfield_bits semicolon "name"  */
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

  case 734: /* bitfield_alias_bits: bitfield_alias_bits semicolon  */
                                            {
        (yyval.pNameExprList) = (yyvsp[-1].pNameExprList);
    }
    break;

  case 735: /* bitfield_alias_bits: bitfield_alias_bits "name" semicolon  */
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

  case 736: /* bitfield_alias_bits: bitfield_alias_bits "name" '=' expr semicolon  */
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

  case 737: /* bitfield_basic_type_declaration: %empty  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 738: /* bitfield_basic_type_declaration: ':' "uint8"  */
                             { (yyval.type) = Type::tBitfield8; }
    break;

  case 739: /* bitfield_basic_type_declaration: ':' "uint16"  */
                             { (yyval.type) = Type::tBitfield16; }
    break;

  case 740: /* bitfield_basic_type_declaration: ':' "uint"  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 741: /* bitfield_basic_type_declaration: ':' "uint64"  */
                             { (yyval.type) = Type::tBitfield64; }
    break;

  case 742: /* $@50: %empty  */
                                                                     { yyextra->das_arrow_depth ++; }
    break;

  case 743: /* $@51: %empty  */
                                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 744: /* bitfield_type_declaration: "bitfield" bitfield_basic_type_declaration '<' $@50 bitfield_bits '>' $@51  */
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

  case 747: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 748: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 749: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 750: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 751: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 752: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 753: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 754: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 755: /* type_declaration_no_options: type_declaration_no_options dim_list  */
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

  case 756: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 757: /* $@52: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 758: /* $@53: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 759: /* type_declaration_no_options: "type" '<' $@52 type_declaration '>' $@53  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 760: /* type_declaration_no_options: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 761: /* type_declaration_no_options: '$' name_in_namespace optional_expr_list_in_braces  */
                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-1]), (yylsp[0]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[0].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-1])), *(yyvsp[-1].s)));
        delete (yyvsp[-1].s);
    }
    break;

  case 762: /* $@54: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 763: /* type_declaration_no_options: '$' name_in_namespace '<' $@54 type_declaration_no_options_list '>' optional_expr_list_in_braces  */
                                                                                                                                                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-5]), (yylsp[0]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-2].pTypeDeclList),(yyvsp[0].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-5])), *(yyvsp[-5].s)));
        delete (yyvsp[-5].s);
    }
    break;

  case 764: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 765: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 766: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 767: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 768: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 769: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 770: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 771: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 772: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 773: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 774: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 775: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 776: /* $@55: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 777: /* $@56: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 778: /* type_declaration_no_options: "smart_ptr" '<' $@55 type_declaration '>' $@56  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 779: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 780: /* $@57: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 781: /* $@58: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 782: /* type_declaration_no_options: "array" '<' $@57 type_declaration '>' $@58  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 783: /* $@59: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 784: /* $@60: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 785: /* type_declaration_no_options: "table" '<' $@59 table_type_pair '>' $@60  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].aTypePair).firstType;
        (yyval.pTypeDecl)->secondType = (yyvsp[-2].aTypePair).secondType;
    }
    break;

  case 786: /* $@61: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 787: /* $@62: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 788: /* type_declaration_no_options: "iterator" '<' $@61 type_declaration '>' $@62  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 789: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tVoid);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 790: /* $@63: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 791: /* $@64: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 792: /* type_declaration_no_options: "block" '<' $@63 type_declaration '>' $@64  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 793: /* $@65: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 794: /* $@66: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 795: /* type_declaration_no_options: "block" '<' $@65 optional_function_argument_list optional_function_type '>' $@66  */
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

  case 796: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tVoid);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 797: /* $@67: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 798: /* $@68: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 799: /* type_declaration_no_options: "function" '<' $@67 type_declaration '>' $@68  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 800: /* $@69: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 801: /* $@70: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 802: /* type_declaration_no_options: "function" '<' $@69 optional_function_argument_list optional_function_type '>' $@70  */
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

  case 803: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tVoid);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 804: /* $@71: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 805: /* $@72: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 806: /* type_declaration_no_options: "lambda" '<' $@71 type_declaration '>' $@72  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 807: /* $@73: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 808: /* $@74: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 809: /* type_declaration_no_options: "lambda" '<' $@73 optional_function_argument_list optional_function_type '>' $@74  */
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

  case 810: /* $@75: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 811: /* $@76: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 812: /* type_declaration_no_options: "tuple" '<' $@75 tuple_type_list '>' $@76  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 813: /* $@77: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 814: /* $@78: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 815: /* type_declaration_no_options: "variant" '<' $@77 variant_type_list '>' $@78  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 816: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 817: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 818: /* type_declaration: type_declaration '|' '#'  */
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

  case 819: /* $@79: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 820: /* $@80: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 821: /* $@81: %empty  */
                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 822: /* $@82: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 823: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@79 "name" $@80 open_block $@81 tuple_alias_type_list $@82 close_block  */
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

  case 824: /* $@83: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 825: /* $@84: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 826: /* $@85: %empty  */
                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 827: /* $@86: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 828: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@83 "name" $@84 open_block $@85 variant_alias_type_list $@86 close_block  */
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

  case 829: /* $@87: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 830: /* $@88: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 831: /* $@89: %empty  */
                                                            {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 832: /* $@90: %empty  */
                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-5]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 833: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@87 "name" $@88 bitfield_basic_type_declaration open_block $@89 bitfield_alias_bits $@90 close_block  */
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

  case 834: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 835: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 836: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 837: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 838: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 839: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 840: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 841: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 842: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 843: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 844: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 845: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 846: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 847: /* make_variant_dim: %empty  */
       {
        (yyval.pExpression) = ast_makeStructToMakeVariant(nullptr, LineInfo());
    }
    break;

  case 848: /* make_variant_dim: make_struct_fields  */
                              {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 849: /* make_struct_single: make_struct_fields optional_comma  */
                                               {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 850: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 851: /* make_struct_dim: make_struct_dim "end of expression" make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 852: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 853: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 854: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 855: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 856: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 857: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 858: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 859: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 872: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 873: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 874: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 875: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = (yyvsp[-2].pTypeDecl);
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 876: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = (yyvsp[-4].pTypeDecl);
        msd->useInitializer = true;
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = msd;
    }
    break;

  case 877: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-5].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 878: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back((yyvsp[-2].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 879: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 880: /* $@91: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 881: /* $@92: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 882: /* make_struct_decl: "struct" '<' $@91 type_declaration_no_options '>' $@92 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 883: /* $@93: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 884: /* $@94: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 885: /* make_struct_decl: "class" '<' $@93 type_declaration_no_options '>' $@94 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                           {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 886: /* $@95: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 887: /* $@96: %empty  */
                                                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 888: /* make_struct_decl: "variant" '<' $@95 variant_type_list '>' $@96 '(' use_initializer make_variant_dim ')'  */
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

  case 889: /* $@97: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 890: /* $@98: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 891: /* make_struct_decl: "default" '<' $@97 type_declaration_no_options '>' $@98 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = (yyvsp[-3].pTypeDecl);
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 892: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 893: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 894: /* make_tuple: make_tuple ',' expr  */
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

  case 895: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 896: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 897: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 898: /* $@99: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 899: /* $@100: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 900: /* make_tuple_call: "tuple" '<' $@99 tuple_type_list '>' $@100 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 901: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mka;
    }
    break;

  case 902: /* make_dim: make_dim "end of expression" make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 903: /* make_dim_decl: '[' optional_expr_list ']'  */
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

  case 904: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-2].pTypeDecl);
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 905: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-2].pTypeDecl);
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 906: /* $@101: %empty  */
                                       { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 907: /* $@102: %empty  */
                                                                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 908: /* make_dim_decl: "array" "struct" '<' $@101 type_declaration_no_options '>' $@102 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 909: /* $@103: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 910: /* $@104: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 911: /* make_dim_decl: "array" "tuple" '<' $@103 tuple_type_list '>' $@104 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 912: /* $@105: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 913: /* $@106: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 914: /* make_dim_decl: "array" "variant" '<' $@105 variant_type_list '>' $@106 '(' make_variant_dim ')'  */
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

  case 915: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
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

  case 916: /* $@107: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 917: /* $@108: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 918: /* make_dim_decl: "array" '<' $@107 type_declaration_no_options '>' $@108 '(' optional_expr_list ')'  */
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

  case 919: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 920: /* $@109: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 921: /* $@110: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 922: /* make_dim_decl: "fixed_array" '<' $@109 type_declaration_no_options '>' $@110 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = (yyvsp[-6].pTypeDecl);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 923: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mka;
    }
    break;

  case 924: /* make_table: make_table "end of expression" make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 925: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 926: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 927: /* make_table_decl: "begin of code block" optional_expr_map_tuple_list "end of code block"  */
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

  case 928: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
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

  case 929: /* make_table_decl: "table" '(' optional_expr_map_tuple_list ')'  */
                                                                       {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 930: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 931: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 932: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 933: /* array_comprehension_where: "end of expression" "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 934: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 935: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 936: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 937: /* array_comprehension: '[' "iterator" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                                 {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 938: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true,false);
    }
    break;

  case 939: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where "end of code block" ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),false,false);
    }
    break;

  case 940: /* array_comprehension: "begin of code block" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                              {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
    }
    break;

  case 941: /* array_comprehension: "{{" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block" "end of code block"  */
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
