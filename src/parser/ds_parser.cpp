/* A Bison parser, made by GNU Bison 3.7.6.  */

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
#define YYBISON 30706

/* Bison version string.  */
#define YYBISON_VERSION "3.7.6"

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
#line 71 "ds_parser.ypp"

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

    void das_yyerror ( yyscan_t scanner, const string & error, const das::LineInfo & at, das::CompilationError cerr = das::CompilationError::unspecified );
    void das_checkName ( yyscan_t scanner, const string & name, const LineInfo &at );
    void das_yyerror ( DAS_YYLTYPE * lloc, yyscan_t scanner, const string & error );
    void das_yyfatalerror ( DAS_YYLTYPE * lloc, yyscan_t scanner, const string & error, CompilationError cerr );
    int yylex ( DAS_YYSTYPE *lvalp, DAS_YYLTYPE *llocp, yyscan_t scanner );
    void yybegin ( const char * str );

    void das_yybegin_reader ( yyscan_t yyscanner );
    void das_yyend_reader ( yyscan_t yyscanner );

    __forceinline string inThisModule ( const string & name ) { return "_::" + name; }

    #undef yyextra
    #define yyextra (*((das::DasParserState **)(scanner)))

#line 113 "ds_parser.cpp"

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
  YYSYMBOL_DAS_TBOOL = 64,                 /* "bool"  */
  YYSYMBOL_DAS_TVOID = 65,                 /* "void"  */
  YYSYMBOL_DAS_TSTRING = 66,               /* "string"  */
  YYSYMBOL_DAS_TAUTO = 67,                 /* "auto"  */
  YYSYMBOL_DAS_TINT = 68,                  /* "int"  */
  YYSYMBOL_DAS_TINT2 = 69,                 /* "int2"  */
  YYSYMBOL_DAS_TINT3 = 70,                 /* "int3"  */
  YYSYMBOL_DAS_TINT4 = 71,                 /* "int4"  */
  YYSYMBOL_DAS_TUINT = 72,                 /* "uint"  */
  YYSYMBOL_DAS_TBITFIELD = 73,             /* "bitfield"  */
  YYSYMBOL_DAS_TUINT2 = 74,                /* "uint2"  */
  YYSYMBOL_DAS_TUINT3 = 75,                /* "uint3"  */
  YYSYMBOL_DAS_TUINT4 = 76,                /* "uint4"  */
  YYSYMBOL_DAS_TFLOAT = 77,                /* "float"  */
  YYSYMBOL_DAS_TFLOAT2 = 78,               /* "float2"  */
  YYSYMBOL_DAS_TFLOAT3 = 79,               /* "float3"  */
  YYSYMBOL_DAS_TFLOAT4 = 80,               /* "float4"  */
  YYSYMBOL_DAS_TRANGE = 81,                /* "range"  */
  YYSYMBOL_DAS_TURANGE = 82,               /* "urange"  */
  YYSYMBOL_DAS_TBLOCK = 83,                /* "block"  */
  YYSYMBOL_DAS_TINT64 = 84,                /* "int64"  */
  YYSYMBOL_DAS_TUINT64 = 85,               /* "uint64"  */
  YYSYMBOL_DAS_TDOUBLE = 86,               /* "double"  */
  YYSYMBOL_DAS_TFUNCTION = 87,             /* "function"  */
  YYSYMBOL_DAS_TLAMBDA = 88,               /* "lambda"  */
  YYSYMBOL_DAS_TINT8 = 89,                 /* "int8"  */
  YYSYMBOL_DAS_TUINT8 = 90,                /* "uint8"  */
  YYSYMBOL_DAS_TINT16 = 91,                /* "int16"  */
  YYSYMBOL_DAS_TUINT16 = 92,               /* "uint16"  */
  YYSYMBOL_DAS_TTUPLE = 93,                /* "tuple"  */
  YYSYMBOL_DAS_TVARIANT = 94,              /* "variant"  */
  YYSYMBOL_DAS_GENERATOR = 95,             /* "generator"  */
  YYSYMBOL_DAS_YIELD = 96,                 /* "yield"  */
  YYSYMBOL_DAS_SEALED = 97,                /* "sealed"  */
  YYSYMBOL_ADDEQU = 98,                    /* "+="  */
  YYSYMBOL_SUBEQU = 99,                    /* "-="  */
  YYSYMBOL_DIVEQU = 100,                   /* "/="  */
  YYSYMBOL_MULEQU = 101,                   /* "*="  */
  YYSYMBOL_MODEQU = 102,                   /* "%="  */
  YYSYMBOL_ANDEQU = 103,                   /* "&="  */
  YYSYMBOL_OREQU = 104,                    /* "|="  */
  YYSYMBOL_XOREQU = 105,                   /* "^="  */
  YYSYMBOL_SHL = 106,                      /* "<<"  */
  YYSYMBOL_SHR = 107,                      /* ">>"  */
  YYSYMBOL_ADDADD = 108,                   /* "++"  */
  YYSYMBOL_SUBSUB = 109,                   /* "--"  */
  YYSYMBOL_LEEQU = 110,                    /* "<="  */
  YYSYMBOL_SHLEQU = 111,                   /* "<<="  */
  YYSYMBOL_SHREQU = 112,                   /* ">>="  */
  YYSYMBOL_GREQU = 113,                    /* ">="  */
  YYSYMBOL_EQUEQU = 114,                   /* "=="  */
  YYSYMBOL_NOTEQU = 115,                   /* "!="  */
  YYSYMBOL_RARROW = 116,                   /* "->"  */
  YYSYMBOL_LARROW = 117,                   /* "<-"  */
  YYSYMBOL_QQ = 118,                       /* "??"  */
  YYSYMBOL_QDOT = 119,                     /* "?."  */
  YYSYMBOL_QBRA = 120,                     /* "?["  */
  YYSYMBOL_LPIPE = 121,                    /* "<|"  */
  YYSYMBOL_LBPIPE = 122,                   /* " <|"  */
  YYSYMBOL_LAPIPE = 123,                   /* "@ <|"  */
  YYSYMBOL_LFPIPE = 124,                   /* "@@ <|"  */
  YYSYMBOL_RPIPE = 125,                    /* "|>"  */
  YYSYMBOL_CLONEEQU = 126,                 /* ":="  */
  YYSYMBOL_ROTL = 127,                     /* "<<<"  */
  YYSYMBOL_ROTR = 128,                     /* ">>>"  */
  YYSYMBOL_ROTLEQU = 129,                  /* "<<<="  */
  YYSYMBOL_ROTREQU = 130,                  /* ">>>="  */
  YYSYMBOL_MAPTO = 131,                    /* "=>"  */
  YYSYMBOL_COLCOL = 132,                   /* "::"  */
  YYSYMBOL_ANDAND = 133,                   /* "&&"  */
  YYSYMBOL_OROR = 134,                     /* "||"  */
  YYSYMBOL_XORXOR = 135,                   /* "^^"  */
  YYSYMBOL_ANDANDEQU = 136,                /* "&&="  */
  YYSYMBOL_OROREQU = 137,                  /* "||="  */
  YYSYMBOL_XORXOREQU = 138,                /* "^^="  */
  YYSYMBOL_BRABRAB = 139,                  /* "[["  */
  YYSYMBOL_BRACBRB = 140,                  /* "[{"  */
  YYSYMBOL_CBRCBRB = 141,                  /* "{{"  */
  YYSYMBOL_INTEGER = 142,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 143,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 144,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 145,    /* "unsigned long integer constant"  */
  YYSYMBOL_FLOAT = 146,                    /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 147,                   /* "double constant"  */
  YYSYMBOL_NAME = 148,                     /* "name"  */
  YYSYMBOL_BEGIN_STRING = 149,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 150,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 151,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 152,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 153,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 154,          /* "}"  */
  YYSYMBOL_END_OF_READ = 155,              /* "end of failed eader macro"  */
  YYSYMBOL_156_ = 156,                     /* ','  */
  YYSYMBOL_157_ = 157,                     /* '='  */
  YYSYMBOL_158_ = 158,                     /* '?'  */
  YYSYMBOL_159_ = 159,                     /* ':'  */
  YYSYMBOL_160_ = 160,                     /* '|'  */
  YYSYMBOL_161_ = 161,                     /* '^'  */
  YYSYMBOL_162_ = 162,                     /* '&'  */
  YYSYMBOL_163_ = 163,                     /* '<'  */
  YYSYMBOL_164_ = 164,                     /* '>'  */
  YYSYMBOL_165_ = 165,                     /* '-'  */
  YYSYMBOL_166_ = 166,                     /* '+'  */
  YYSYMBOL_167_ = 167,                     /* '*'  */
  YYSYMBOL_168_ = 168,                     /* '/'  */
  YYSYMBOL_169_ = 169,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 170,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 171,               /* UNARY_PLUS  */
  YYSYMBOL_172_ = 172,                     /* '~'  */
  YYSYMBOL_173_ = 173,                     /* '!'  */
  YYSYMBOL_PRE_INC = 174,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 175,                  /* PRE_DEC  */
  YYSYMBOL_POST_INC = 176,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 177,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 178,                    /* DEREF  */
  YYSYMBOL_179_ = 179,                     /* '.'  */
  YYSYMBOL_180_ = 180,                     /* '['  */
  YYSYMBOL_181_ = 181,                     /* ']'  */
  YYSYMBOL_182_ = 182,                     /* '('  */
  YYSYMBOL_183_ = 183,                     /* ')'  */
  YYSYMBOL_184_ = 184,                     /* '$'  */
  YYSYMBOL_185_ = 185,                     /* '@'  */
  YYSYMBOL_186_ = 186,                     /* ';'  */
  YYSYMBOL_187_ = 187,                     /* '{'  */
  YYSYMBOL_188_ = 188,                     /* '}'  */
  YYSYMBOL_189_ = 189,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 190,                 /* $accept  */
  YYSYMBOL_program = 191,                  /* program  */
  YYSYMBOL_optional_public_or_private_module = 192, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 193,              /* module_name  */
  YYSYMBOL_module_declaration = 194,       /* module_declaration  */
  YYSYMBOL_character_sequence = 195,       /* character_sequence  */
  YYSYMBOL_string_constant = 196,          /* string_constant  */
  YYSYMBOL_string_builder_body = 197,      /* string_builder_body  */
  YYSYMBOL_string_builder = 198,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 199, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 200,              /* expr_reader  */
  YYSYMBOL_201_1 = 201,                    /* $@1  */
  YYSYMBOL_options_declaration = 202,      /* options_declaration  */
  YYSYMBOL_require_declaration = 203,      /* require_declaration  */
  YYSYMBOL_require_module_name = 204,      /* require_module_name  */
  YYSYMBOL_require_module = 205,           /* require_module  */
  YYSYMBOL_is_public_module = 206,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 207,       /* expect_declaration  */
  YYSYMBOL_expect_list = 208,              /* expect_list  */
  YYSYMBOL_expect_error = 209,             /* expect_error  */
  YYSYMBOL_expression_label = 210,         /* expression_label  */
  YYSYMBOL_expression_goto = 211,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 212,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 213,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 214,          /* if_or_static_if  */
  YYSYMBOL_expression_if_then_else = 215,  /* expression_if_then_else  */
  YYSYMBOL_expression_for_loop = 216,      /* expression_for_loop  */
  YYSYMBOL_expression_unsafe = 217,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 218,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 219,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 220,    /* expression_with_alias  */
  YYSYMBOL_221_2 = 221,                    /* $@2  */
  YYSYMBOL_annotation_argument_value = 222, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 223, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 224, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 225,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 226, /* annotation_argument_list  */
  YYSYMBOL_annotation_declaration_name = 227, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration = 228,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 229,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 230, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 231, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 232,   /* optional_function_type  */
  YYSYMBOL_function_name = 233,            /* function_name  */
  YYSYMBOL_global_function_declaration = 234, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 235, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 236, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 237,     /* function_declaration  */
  YYSYMBOL_expression_block = 238,         /* expression_block  */
  YYSYMBOL_expression_any = 239,           /* expression_any  */
  YYSYMBOL_expressions = 240,              /* expressions  */
  YYSYMBOL_expr_pipe = 241,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 242,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 243,        /* expression_delete  */
  YYSYMBOL_expr_new = 244,                 /* expr_new  */
  YYSYMBOL_expression_break = 245,         /* expression_break  */
  YYSYMBOL_expression_continue = 246,      /* expression_continue  */
  YYSYMBOL_expression_return = 247,        /* expression_return  */
  YYSYMBOL_expression_yield = 248,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 249,     /* expression_try_catch  */
  YYSYMBOL_kwd_let = 250,                  /* kwd_let  */
  YYSYMBOL_expression_let = 251,           /* expression_let  */
  YYSYMBOL_expr_cast = 252,                /* expr_cast  */
  YYSYMBOL_253_3 = 253,                    /* $@3  */
  YYSYMBOL_254_4 = 254,                    /* $@4  */
  YYSYMBOL_255_5 = 255,                    /* $@5  */
  YYSYMBOL_256_6 = 256,                    /* $@6  */
  YYSYMBOL_257_7 = 257,                    /* $@7  */
  YYSYMBOL_258_8 = 258,                    /* $@8  */
  YYSYMBOL_expr_type_decl = 259,           /* expr_type_decl  */
  YYSYMBOL_260_9 = 260,                    /* $@9  */
  YYSYMBOL_261_10 = 261,                   /* $@10  */
  YYSYMBOL_expr_type_info = 262,           /* expr_type_info  */
  YYSYMBOL_expr_list = 263,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 264,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 265,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 266,            /* capture_entry  */
  YYSYMBOL_capture_list = 267,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 268,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 269,               /* expr_block  */
  YYSYMBOL_expr_numeric_const = 270,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 271,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe = 272,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 273,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 274,         /* expr_method_call  */
  YYSYMBOL_func_addr_expr = 275,           /* func_addr_expr  */
  YYSYMBOL_276_11 = 276,                   /* $@11  */
  YYSYMBOL_277_12 = 277,                   /* $@12  */
  YYSYMBOL_278_13 = 278,                   /* $@13  */
  YYSYMBOL_279_14 = 279,                   /* $@14  */
  YYSYMBOL_expr_field = 280,               /* expr_field  */
  YYSYMBOL_281_15 = 281,                   /* $@15  */
  YYSYMBOL_282_16 = 282,                   /* $@16  */
  YYSYMBOL_expr = 283,                     /* expr  */
  YYSYMBOL_284_17 = 284,                   /* $@17  */
  YYSYMBOL_285_18 = 285,                   /* $@18  */
  YYSYMBOL_optional_field_annotation = 286, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 287,        /* optional_override  */
  YYSYMBOL_optional_constant = 288,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 289, /* optional_public_or_private_member_variable  */
  YYSYMBOL_structure_variable_declaration = 290, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 291, /* struct_variable_declaration_list  */
  YYSYMBOL_292_19 = 292,                   /* $@19  */
  YYSYMBOL_function_argument_declaration = 293, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 294,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 295,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 296,          /* tuple_type_list  */
  YYSYMBOL_variant_type = 297,             /* variant_type  */
  YYSYMBOL_variant_type_list = 298,        /* variant_type_list  */
  YYSYMBOL_copy_or_move = 299,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 300,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 301,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 302,             /* optional_ref  */
  YYSYMBOL_let_variable_declaration = 303, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 304, /* global_variable_declaration_list  */
  YYSYMBOL_optional_shared = 305,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 306, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 307,               /* global_let  */
  YYSYMBOL_308_20 = 308,                   /* $@20  */
  YYSYMBOL_enum_list = 309,                /* enum_list  */
  YYSYMBOL_single_alias = 310,             /* single_alias  */
  YYSYMBOL_alias_list = 311,               /* alias_list  */
  YYSYMBOL_alias_declaration = 312,        /* alias_declaration  */
  YYSYMBOL_optional_public_or_private_enum = 313, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_declaration = 314,         /* enum_declaration  */
  YYSYMBOL_optional_structure_parent = 315, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 316,          /* optional_sealed  */
  YYSYMBOL_structure_name = 317,           /* structure_name  */
  YYSYMBOL_class_or_struct = 318,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 319, /* optional_public_or_private_structure  */
  YYSYMBOL_structure_declaration = 320,    /* structure_declaration  */
  YYSYMBOL_321_21 = 321,                   /* $@21  */
  YYSYMBOL_variable_name_with_pos_list = 322, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 323,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 324, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 325, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 326,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 327,            /* bitfield_bits  */
  YYSYMBOL_bitfield_type_declaration = 328, /* bitfield_type_declaration  */
  YYSYMBOL_329_22 = 329,                   /* $@22  */
  YYSYMBOL_330_23 = 330,                   /* $@23  */
  YYSYMBOL_type_declaration = 331,         /* type_declaration  */
  YYSYMBOL_332_24 = 332,                   /* $@24  */
  YYSYMBOL_333_25 = 333,                   /* $@25  */
  YYSYMBOL_334_26 = 334,                   /* $@26  */
  YYSYMBOL_335_27 = 335,                   /* $@27  */
  YYSYMBOL_336_28 = 336,                   /* $@28  */
  YYSYMBOL_337_29 = 337,                   /* $@29  */
  YYSYMBOL_338_30 = 338,                   /* $@30  */
  YYSYMBOL_339_31 = 339,                   /* $@31  */
  YYSYMBOL_340_32 = 340,                   /* $@32  */
  YYSYMBOL_341_33 = 341,                   /* $@33  */
  YYSYMBOL_342_34 = 342,                   /* $@34  */
  YYSYMBOL_343_35 = 343,                   /* $@35  */
  YYSYMBOL_344_36 = 344,                   /* $@36  */
  YYSYMBOL_345_37 = 345,                   /* $@37  */
  YYSYMBOL_346_38 = 346,                   /* $@38  */
  YYSYMBOL_347_39 = 347,                   /* $@39  */
  YYSYMBOL_348_40 = 348,                   /* $@40  */
  YYSYMBOL_349_41 = 349,                   /* $@41  */
  YYSYMBOL_350_42 = 350,                   /* $@42  */
  YYSYMBOL_351_43 = 351,                   /* $@43  */
  YYSYMBOL_352_44 = 352,                   /* $@44  */
  YYSYMBOL_353_45 = 353,                   /* $@45  */
  YYSYMBOL_354_46 = 354,                   /* $@46  */
  YYSYMBOL_355_47 = 355,                   /* $@47  */
  YYSYMBOL_variant_alias_declaration = 356, /* variant_alias_declaration  */
  YYSYMBOL_357_48 = 357,                   /* $@48  */
  YYSYMBOL_bitfield_alias_declaration = 358, /* bitfield_alias_declaration  */
  YYSYMBOL_359_49 = 359,                   /* $@49  */
  YYSYMBOL_make_decl = 360,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 361,       /* make_struct_fields  */
  YYSYMBOL_make_struct_dim = 362,          /* make_struct_dim  */
  YYSYMBOL_optional_block = 363,           /* optional_block  */
  YYSYMBOL_make_struct_decl = 364,         /* make_struct_decl  */
  YYSYMBOL_make_tuple = 365,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 366,           /* make_map_tuple  */
  YYSYMBOL_make_any_tuple = 367,           /* make_any_tuple  */
  YYSYMBOL_make_dim = 368,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 369,            /* make_dim_decl  */
  YYSYMBOL_make_table = 370,               /* make_table  */
  YYSYMBOL_make_table_decl = 371,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 372, /* array_comprehension_where  */
  YYSYMBOL_array_comprehension = 373       /* array_comprehension  */
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

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
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
#define YYLAST   7623

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  190
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  184
/* YYNRULES -- Number of rules.  */
#define YYNRULES  576
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1007

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   417


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
       2,     2,     2,   173,     2,   189,   184,   169,   162,     2,
     182,   183,   167,   166,   156,   165,   179,   168,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   159,   186,
     163,   157,   164,   158,   185,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   180,     2,   181,   161,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   187,   160,   188,   172,     2,     2,     2,
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
     155,   170,   171,   174,   175,   176,   177,   178
};

#if DAS_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   483,   483,   484,   485,   486,   487,   488,   489,   490,
     491,   492,   493,   494,   495,   499,   500,   501,   505,   506,
     510,   527,   528,   529,   530,   534,   538,   543,   552,   560,
     576,   581,   589,   589,   619,   640,   644,   647,   651,   657,
     666,   689,   714,   715,   719,   723,   724,   728,   731,   737,
     743,   746,   752,   753,   757,   758,   759,   768,   769,   773,
     782,   799,   807,   817,   826,   826,   833,   834,   835,   836,
     837,   838,   842,   847,   855,   856,   857,   861,   862,   863,
     864,   865,   866,   867,   868,   874,   888,   905,   906,   907,
     911,   919,   932,   936,   943,   944,   948,   949,   950,   954,
     957,   964,   968,   969,   970,   971,   972,   973,   974,   975,
     976,   977,   978,   979,   980,   981,   982,   983,   984,   985,
     986,   987,   988,   989,   990,   991,   992,   993,   994,   995,
     996,   997,   998,   999,  1000,  1001,  1002,  1003,  1004,  1005,
    1006,  1007,  1008,  1009,  1010,  1011,  1012,  1013,  1014,  1015,
    1019,  1041,  1042,  1043,  1047,  1086,  1094,  1098,  1109,  1110,
    1111,  1112,  1113,  1114,  1115,  1116,  1117,  1118,  1119,  1120,
    1121,  1122,  1123,  1124,  1125,  1126,  1127,  1128,  1132,  1137,
    1143,  1149,  1183,  1186,  1192,  1193,  1204,  1208,  1214,  1217,
    1220,  1224,  1230,  1234,  1238,  1241,  1244,  1249,  1252,  1260,
    1263,  1268,  1271,  1279,  1285,  1286,  1290,  1325,  1325,  1325,
    1328,  1328,  1328,  1333,  1333,  1333,  1341,  1341,  1341,  1347,
    1357,  1368,  1383,  1386,  1392,  1393,  1400,  1411,  1412,  1413,
    1417,  1418,  1419,  1420,  1424,  1429,  1437,  1438,  1442,  1447,
    1512,  1513,  1514,  1515,  1516,  1517,  1521,  1522,  1523,  1524,
    1525,  1526,  1527,  1528,  1529,  1530,  1531,  1532,  1533,  1534,
    1535,  1536,  1537,  1538,  1539,  1543,  1544,  1545,  1546,  1550,
    1561,  1566,  1576,  1580,  1580,  1580,  1587,  1587,  1587,  1601,
    1605,  1609,  1609,  1609,  1616,  1617,  1618,  1619,  1620,  1621,
    1622,  1623,  1624,  1625,  1626,  1627,  1628,  1629,  1630,  1631,
    1632,  1633,  1634,  1635,  1636,  1637,  1638,  1639,  1640,  1641,
    1642,  1643,  1644,  1645,  1646,  1647,  1648,  1649,  1650,  1651,
    1652,  1653,  1654,  1655,  1656,  1657,  1658,  1659,  1660,  1661,
    1662,  1666,  1670,  1673,  1676,  1677,  1678,  1679,  1688,  1697,
    1698,  1701,  1701,  1701,  1704,  1708,  1712,  1716,  1717,  1718,
    1719,  1720,  1721,  1722,  1723,  1739,  1755,  1810,  1818,  1819,
    1823,  1824,  1825,  1829,  1830,  1834,  1835,  1836,  1840,  1850,
    1853,  1858,  1857,  1903,  1969,  1978,  1983,  1991,  1992,  1996,
    1999,  2008,  2009,  2013,  2022,  2023,  2028,  2029,  2033,  2039,
    2045,  2048,  2052,  2058,  2067,  2068,  2069,  2073,  2074,  2078,
    2081,  2086,  2091,  2099,  2110,  2113,  2121,  2122,  2126,  2127,
    2128,  2132,  2165,  2165,  2204,  2207,  2216,  2229,  2241,  2242,
    2246,  2247,  2251,  2252,  2253,  2257,  2285,  2317,  2318,  2322,
    2323,  2327,  2371,  2372,  2376,  2377,  2378,  2383,  2382,  2479,
    2486,  2495,  2501,  2512,  2513,  2514,  2515,  2516,  2517,  2518,
    2519,  2520,  2521,  2522,  2523,  2524,  2525,  2526,  2527,  2528,
    2529,  2530,  2531,  2532,  2533,  2534,  2535,  2536,  2540,  2541,
    2542,  2543,  2544,  2545,  2549,  2560,  2564,  2574,  2581,  2590,
    2590,  2590,  2603,  2604,  2605,  2606,  2607,  2621,  2627,  2631,
    2635,  2640,  2645,  2650,  2655,  2659,  2663,  2668,  2672,  2677,
    2677,  2677,  2683,  2690,  2690,  2690,  2695,  2695,  2695,  2701,
    2701,  2701,  2706,  2710,  2710,  2710,  2715,  2715,  2715,  2724,
    2728,  2728,  2728,  2733,  2733,  2733,  2742,  2746,  2746,  2746,
    2751,  2751,  2751,  2760,  2760,  2760,  2766,  2766,  2766,  2775,
    2775,  2790,  2790,  2810,  2811,  2812,  2813,  2817,  2824,  2831,
    2837,  2846,  2851,  2858,  2859,  2863,  2869,  2876,  2884,  2891,
    2899,  2911,  2914,  2928,  2937,  2938,  2942,  2947,  2954,  2959,
    2969,  2974,  2981,  2993,  2994,  2998,  3021
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
  "\"private\"", "\"smart_ptr\"", "\"unsafe\"", "\"bool\"", "\"void\"",
  "\"string\"", "\"auto\"", "\"int\"", "\"int2\"", "\"int3\"", "\"int4\"",
  "\"uint\"", "\"bitfield\"", "\"uint2\"", "\"uint3\"", "\"uint4\"",
  "\"float\"", "\"float2\"", "\"float3\"", "\"float4\"", "\"range\"",
  "\"urange\"", "\"block\"", "\"int64\"", "\"uint64\"", "\"double\"",
  "\"function\"", "\"lambda\"", "\"int8\"", "\"uint8\"", "\"int16\"",
  "\"uint16\"", "\"tuple\"", "\"variant\"", "\"generator\"", "\"yield\"",
  "\"sealed\"", "\"+=\"", "\"-=\"", "\"/=\"", "\"*=\"", "\"%=\"", "\"&=\"",
  "\"|=\"", "\"^=\"", "\"<<\"", "\">>\"", "\"++\"", "\"--\"", "\"<=\"",
  "\"<<=\"", "\">>=\"", "\">=\"", "\"==\"", "\"!=\"", "\"->\"", "\"<-\"",
  "\"??\"", "\"?.\"", "\"?[\"", "\"<|\"", "\" <|\"", "\"@ <|\"",
  "\"@@ <|\"", "\"|>\"", "\":=\"", "\"<<<\"", "\">>>\"", "\"<<<=\"",
  "\">>>=\"", "\"=>\"", "\"::\"", "\"&&\"", "\"||\"", "\"^^\"", "\"&&=\"",
  "\"||=\"", "\"^^=\"", "\"[[\"", "\"[{\"", "\"{{\"",
  "\"integer constant\"", "\"long integer constant\"",
  "\"unsigned integer constant\"", "\"unsigned long integer constant\"",
  "\"floating point constant\"", "\"double constant\"", "\"name\"",
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
  "options_declaration", "require_declaration", "require_module_name",
  "require_module", "is_public_module", "expect_declaration",
  "expect_list", "expect_error", "expression_label", "expression_goto",
  "elif_or_static_elif", "expression_else", "if_or_static_if",
  "expression_if_then_else", "expression_for_loop", "expression_unsafe",
  "expression_while_loop", "expression_with", "expression_with_alias",
  "$@2", "annotation_argument_value", "annotation_argument_value_list",
  "annotation_argument_name", "annotation_argument",
  "annotation_argument_list", "annotation_declaration_name",
  "annotation_declaration", "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "global_function_declaration",
  "optional_public_or_private_function", "function_declaration_header",
  "function_declaration", "expression_block", "expression_any",
  "expressions", "expr_pipe", "name_in_namespace", "expression_delete",
  "expr_new", "expression_break", "expression_continue",
  "expression_return", "expression_yield", "expression_try_catch",
  "kwd_let", "expression_let", "expr_cast", "$@3", "$@4", "$@5", "$@6",
  "$@7", "$@8", "expr_type_decl", "$@9", "$@10", "expr_type_info",
  "expr_list", "block_or_simple_block", "block_or_lambda", "capture_entry",
  "capture_list", "optional_capture_list", "expr_block",
  "expr_numeric_const", "expr_assign", "expr_assign_pipe",
  "expr_named_call", "expr_method_call", "func_addr_expr", "$@11", "$@12",
  "$@13", "$@14", "expr_field", "$@15", "$@16", "expr", "$@17", "$@18",
  "optional_field_annotation", "optional_override", "optional_constant",
  "optional_public_or_private_member_variable",
  "structure_variable_declaration", "struct_variable_declaration_list",
  "$@19", "function_argument_declaration", "function_argument_list",
  "tuple_type", "tuple_type_list", "variant_type", "variant_type_list",
  "copy_or_move", "variable_declaration", "copy_or_move_or_clone",
  "optional_ref", "let_variable_declaration",
  "global_variable_declaration_list", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@20", "enum_list",
  "single_alias", "alias_list", "alias_declaration",
  "optional_public_or_private_enum", "enum_declaration",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "structure_declaration", "$@21", "variable_name_with_pos_list",
  "basic_type_declaration", "enum_basic_type_declaration",
  "structure_type_declaration", "auto_type_declaration", "bitfield_bits",
  "bitfield_type_declaration", "$@22", "$@23", "type_declaration", "$@24",
  "$@25", "$@26", "$@27", "$@28", "$@29", "$@30", "$@31", "$@32", "$@33",
  "$@34", "$@35", "$@36", "$@37", "$@38", "$@39", "$@40", "$@41", "$@42",
  "$@43", "$@44", "$@45", "$@46", "$@47", "variant_alias_declaration",
  "$@48", "bitfield_alias_declaration", "$@49", "make_decl",
  "make_struct_fields", "make_struct_dim", "optional_block",
  "make_struct_decl", "make_tuple", "make_map_tuple", "make_any_tuple",
  "make_dim", "make_dim_decl", "make_table", "make_table_decl",
  "array_comprehension_where", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,    44,    61,    63,    58,
     124,    94,    38,    60,    62,    45,    43,    42,    47,    37,
     411,   412,   126,    33,   413,   414,   415,   416,   417,    46,
      91,    93,    40,    41,    36,    64,    59,   123,   125,    35
};
#endif

#define YYPACT_NINF (-724)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-531)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -724,    34,  -724,  -724,    27,   -56,   -88,   -53,  -724,   -86,
    -724,  -724,    22,  -724,  -724,  -724,  -724,  -724,   201,  -724,
      37,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
     -14,  -724,    90,   150,   171,  -724,  -724,   -88,    11,  -724,
      36,   204,  -724,  -724,  -724,    37,   219,   221,  -724,  -724,
     233,   259,   226,  -724,   -35,  -724,  -724,  -724,   322,   323,
     325,  -724,   335,    13,    27,   253,   -56,   224,   274,  -724,
     285,   295,  -724,  7388,   251,   -84,   360,   257,   258,  -724,
     298,    27,    22,  -724,  -724,  -724,     1,  -724,  -724,  -724,
     299,  -724,  -724,   353,  -724,  -724,   279,  -724,  -724,  -724,
    -724,  -724,   156,    76,  -724,  -724,  -724,  -724,   406,  -724,
    -724,   301,   304,   305,   313,  -724,  -724,  -724,   303,  -724,
    -724,  -724,  -724,  -724,   314,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,   320,  -724,  -724,  -724,   330,   331,
    -724,  -724,  -724,  -724,   332,   333,  -724,  -724,  -724,  -724,
    -724,  2517,  -724,  -724,   300,  -724,  -724,  -724,   349,   350,
    -724,    21,  -724,  7000,   465,   466,  -724,   319,   315,    78,
    -724,   357,  -724,  -724,   364,  -724,  -724,   289,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,   108,  -724,  -724,  -724,  -724,
    -724,   359,  -724,   183,   184,   190,  -724,  -724,  -724,  -724,
    -724,  -724,   476,  -724,  -724,     9,  2929,  -724,  -724,  -724,
     329,   365,  -724,   339,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,   328,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
     337,  -724,  -724,   -81,   367,  -724,  -724,   269,  -724,   370,
     324,   -83,    27,   368,  -724,  -724,  -724,    76,  -724,  7388,
    7388,  7388,  7388,   336,   349,  7388,   319,  7388,   319,  7388,
     319,  7475,   350,  -724,  -724,  -724,   351,  -724,  -724,  -724,
    7301,   361,   374,  -724,   369,   376,   379,   371,   383,   373,
    -724,   386,  4597,  4597,  7127,  7214,  4597,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  4597,  4597,  4597,   162,  4597,  4597,
    -724,  4597,  -724,   372,  -724,  -724,  -724,   -32,  -724,  -724,
    -724,  -724,   378,  -724,  -724,  -724,  -724,  -724,  -724,  5689,
     377,  -724,  -724,  -724,  -724,  -724,   -74,  7388,   -47,  -724,
    -724,  -724,     0,  -724,   252,  7388,  -724,  1245,  -724,  -724,
    -724,  -724,  -724,  -724,   380,    -1,   162,  -724,  -724,  -724,
     368,   137,   511,  -724,   220,  -724,   404,   416,  1698,  2012,
    -724,    75,  2170,   367,  2299,   367,  2357,   367,   142,  -724,
     112,  2517,   176,  -724,  2454,  -724,   162,  -724,  4597,  -724,
    -724,  4597,  -724,  4597,  7388,   213,   213,   368,  1596,   368,
    1756,  6255,  -724,   -28,   277,   213,   213,   -48,  -724,   213,
     213,   800,    99,  -724,  2358,   417,    33,   412,  4597,  4597,
    -724,  -724,  4597,  4597,  4597,  4597,   422,  4597,   423,  4597,
    4597,  4597,  4597,  4597,  4597,  4597,  4597,  3066,  4597,  4597,
    4597,  4597,  4597,  4597,  4597,  4597,  4597,  4597,   -40,  4597,
    -724,  3203,  -724,  -724,  2517,  -724,  -724,   368,  -724,   -63,
    -724,   364,  2517,  -724,  4597,  -724,  -724,   368,  2076,  -724,
     315,  4597,  4597,   427,  -724,   390,   435,  3363,    61,  2221,
     247,   247,  -724,   529,   393,   394,  4597,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,   396,   397,   399,  -724,  -724,
    -724,   368,  -724,   -80,  -724,  6187,  -724,    97,  -724,  -724,
      14,  -724,   405,   439,   440,  7388,  -724,   -39,  -724,  7388,
    -724,  -724,  -724,   441,  -724,   426,  -724,   428,  -724,   430,
    7388,  -724,  7475,  -724,   350,  3500,  3660,  7388,  5143,  7388,
    7388,  5221,  7388,  5299,  2596,     7,   247,   106,  1916,  3820,
    6255,   442,   -21,   410,   443,  -724,  -724,   111,    20,  3980,
     -21,    15,  4597,  4597,   407,  -724,  4597,   206,   447,  -724,
     191,  -724,   452,  -724,   122,  6473,   -42,   319,   438,  -724,
    -724,  1532,  1532,    17,    17,  6915,  6915,   421,    48,  -724,
    5765,   -67,   -67,  1532,  1532,  6677,  6541,  6609,   456,  6337,
     717,  6745,  6881,    17,    17,  1852,  1852,    48,    48,    48,
     458,  4597,  -724,   459,  4597,   607,  5841,  -724,   125,  -724,
    -724,  -724,  7388,  -724,  2518,  -724,   291,    24,  2518,  -724,
    -724,   487,   543,   597,  6473,   291,   454,  -724,   453,   472,
    6473,  -724,  2518,  -724,  1057,   431,  -724,  -724,   432,  -724,
    -724,   291,  -724,  -724,  -724,  -724,   247,  -724,  4597,  4597,
    4597,  4597,  4597,  4597,  4597,  4597,  4597,  4597,  2655,  4597,
    4597,  4597,  4597,  4597,  4597,  2792,     3,  4597,  -724,    22,
    -724,   608,     6,   436,  -724,  -724,   576,   355,  -724,  -724,
    -724,  2518,  -724,  2733,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  2517,  -724,  -724,  -724,  -724,   133,   470,
    5377,  2870,  -724,  3007,  3144,  -724,  3202,  -724,   417,  4597,
    -724,  4597,  4597,   -18,   475,   452,   444,   445,  4597,   446,
    4597,  4597,   452,   450,   484,  6473,  -724,  -724,  6405,  -724,
     478,  7388,   319,   109,   143,  4597,  -724,   483,   492,   518,
     519,  -724,   144,   367,  -724,  4140,  -724,  -724,  4597,  -724,
    5917,  -724,  5993,  -724,  -724,  -724,  2654,  -724,  6084,  -724,
    4597,  -724,  4687,  4597,  4597,  -724,   315,  -724,  -724,  -724,
    -724,  -724,  4789,  -724,  -724,  -724,   352,  -724,  6473,  6473,
    6473,  6473,  6473,  6473,  6473,  6473,  6473,  6473,  4597,  4597,
    6473,  6473,  6473,  6473,  6473,  6473,  6473,  4597,  4597,  6473,
    -724,   902,   170,   362,  -724,  -724,   362,  -724,   526,   476,
    -724,  2518,  -724,  4891,  -724,  -724,  -724,  -724,  -724,   182,
    -724,  -724,  -724,  -724,  -724,   500,   -34,  6473,  6473,   -21,
     502,   123,   442,   503,  -724,  6473,  -724,  -724,    73,   -21,
     504,  -724,  -724,  -724,  3299,   367,   505,  6473,  -724,  -724,
    -724,  -724,   -42,   508,   -95,  7388,  -724,   134,  6473,  -724,
    -724,  -724,   476,  4597,    86,  -724,  -724,  4597,  -724,  1433,
     315,  -724,  -724,  4597,  -724,  6813,  6473,  6813,  6473,  -724,
     449,  -724,  -724,    32,   368,  -724,  -724,  4993,  -724,  -724,
    4597,   538,  -724,  4597,  4597,  4597,  4300,  4597,   509,   512,
    4597,  4597,  -724,  4597,   506,  -724,  -724,   528,  -724,  -724,
    -724,  4460,  -724,  -724,  3441,  -724,  6473,   315,  6473,  -724,
    -724,   291,  -724,  -724,   663,  -724,  -724,  5455,   532,    17,
      17,    17,  -724,  5533,  5068,   516,  -724,  6473,  6473,  5068,
     517,   162,  -724,  4597,  6473,  -724,  -724,   352,     1,  -724,
       1,  -724,  4597,  -724,   648,   521,  -724,   525,  -724,  -724,
     162,  6473,  -724,  -724,   534,   315,  5611,  4597,   533,   537,
    -724,  -724,  -724,  -724,  6473,  -724,  -724
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,    94,     1,   204,     0,     0,     0,     0,   205,     0,
     541,   539,     0,    14,     3,    10,     9,     8,     0,     7,
     406,     6,    11,     5,     4,    12,    13,    75,    76,    74,
      83,    85,    34,    47,    44,    45,    36,     0,    42,    35,
       0,     0,   421,    19,    18,   406,     0,     0,    88,    89,
       0,   184,    90,    92,     0,    87,   433,   432,   151,   422,
     434,   407,   408,     0,     0,     0,     0,    37,     0,    43,
       0,     0,    40,     0,     0,     0,    15,     0,     0,   186,
       0,     0,     0,    95,   153,   152,     0,   150,   424,   423,
       0,   436,   435,   429,   410,   409,   412,    81,    82,    79,
      80,    78,     0,     0,    77,    86,    48,    46,    42,    39,
      38,     0,     0,     0,     0,   443,   463,   444,   475,   445,
     449,   450,   451,   452,   467,   456,   457,   458,   459,   460,
     461,   462,   464,   465,   512,   448,   455,   466,   519,   526,
     446,   453,   447,   454,     0,     0,   474,   482,   485,   483,
     484,   417,   418,   420,     0,    16,    17,    20,     0,     0,
     185,     0,    93,     0,     0,     0,   101,    96,     0,     0,
     430,     0,   437,   404,   358,    21,    22,     0,    70,    71,
      68,    69,    67,    66,    72,     0,    41,   503,   506,   509,
     499,     0,   479,   513,   520,   527,   533,   536,   490,   495,
     489,   502,     0,   498,   492,     0,     0,   494,   419,   477,
       0,     0,   384,     0,    91,   149,   104,   105,   107,   106,
     108,   109,   110,   111,   136,   137,   134,   135,   127,   138,
     139,   128,   125,   126,   147,     0,   148,   140,   141,   142,
     143,   115,   116,   117,   112,   113,   114,   130,   131,   129,
     123,   124,   119,   118,   120,   121,   122,   103,   102,   146,
       0,   132,   133,   358,    99,   178,   155,     0,   414,   427,
       0,   358,     0,     0,    23,    24,    25,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    96,     0,    96,     0,
      96,     0,     0,   497,   491,   493,     0,   496,   290,   291,
       0,     0,     0,   284,     0,     0,     0,     0,     0,     0,
     467,     0,     0,     0,     0,     0,     0,   240,   242,   241,
     243,   244,   245,    26,     0,     0,     0,     0,     0,     0,
     487,     0,   227,   228,   288,   287,   238,   285,   350,   349,
     348,   347,    94,   353,   286,   352,   351,   329,   292,     0,
       0,   289,   543,   544,   545,   546,     0,     0,     0,   145,
     144,    97,     0,   377,     0,     0,   154,     0,   468,   471,
     469,   472,   470,   473,     0,     0,     0,   431,   369,   411,
       0,     0,   439,   413,   397,    73,     0,     0,     0,     0,
     476,     0,     0,    99,     0,    99,     0,    99,   184,   381,
       0,   379,     0,   488,   188,   191,     0,   216,     0,   207,
     210,     0,   213,     0,     0,   318,   319,     0,   553,     0,
       0,     0,   570,     0,     0,   296,   295,   334,    32,   294,
     293,     0,   229,   356,     0,   236,     0,     0,     0,     0,
     320,   321,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   281,     0,
     486,     0,   478,   542,   383,   540,   385,     0,   375,   388,
      98,   358,   100,   180,     0,    57,    58,     0,     0,   192,
       0,     0,     0,     0,   193,     0,     0,     0,     0,     0,
       0,     0,   158,   156,     0,     0,     0,   173,   168,   165,
     164,   166,   167,   179,   159,     0,     0,     0,   171,   172,
     174,     0,   163,     0,   160,   246,   414,     0,   425,   428,
     358,   405,     0,     0,     0,     0,   398,     0,   504,     0,
     510,   500,   480,     0,   514,     0,   521,     0,   528,     0,
       0,   534,     0,   537,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   184,     0,     0,
     561,   551,   553,     0,   564,   565,   566,     0,     0,     0,
     553,     0,     0,     0,     0,    29,     0,    27,     0,   322,
     273,   272,     0,   330,     0,   222,     0,    96,     0,   344,
     345,   297,   298,   310,   311,   308,   309,     0,   339,   327,
       0,   354,   355,   299,   300,   315,   316,   317,     0,     0,
     313,   314,   312,   306,   307,   302,   301,   303,   304,   305,
       0,     0,   279,     0,     0,     0,     0,   332,     0,   376,
     387,   386,     0,   389,     0,   378,     0,     0,     0,   194,
     197,     0,   246,     0,   187,     0,     0,   177,     0,     0,
      51,    61,     0,   201,   246,   228,   182,   183,     0,   175,
     176,     0,   162,   169,   170,   206,     0,   161,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   415,     0,
     438,     0,   360,     0,   359,   440,   441,     0,   395,   396,
     394,     0,   505,     0,   511,   501,   481,   515,   517,   522,
     524,   529,   531,   380,   535,   382,   538,   189,     0,     0,
       0,     0,   335,     0,     0,   336,     0,   357,   236,     0,
     554,     0,     0,   553,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   563,   571,   572,     0,    30,
      33,     0,    96,     0,     0,     0,   331,     0,     0,     0,
       0,   234,     0,    99,   341,     0,   325,   346,     0,   328,
       0,   280,     0,   282,   323,   333,   390,   393,   392,    62,
       0,   198,   246,     0,     0,   195,     0,    63,    64,    49,
      50,   202,   246,   199,   229,   178,    54,   181,   256,   257,
     259,   258,   260,   250,   251,   252,   261,   262,     0,     0,
     248,   249,   263,   264,   253,   254,   255,     0,     0,   247,
     426,     0,     0,   365,   361,   362,   365,   370,     0,   394,
     399,     0,   403,   246,   507,   518,   525,   532,   190,     0,
     219,   217,   208,   211,   214,     0,     0,   548,   547,   553,
       0,     0,   552,     0,   556,   562,   568,   567,     0,   553,
       0,   569,    28,    31,     0,    99,     0,   223,   232,   233,
     231,   230,     0,     0,     0,     0,   270,     0,   340,   326,
     324,   283,   386,     0,     0,   196,   203,     0,   200,     0,
       0,    52,    53,     0,    59,   267,   268,   265,   266,   416,
      95,   366,   367,   360,     0,   442,   401,   246,   402,   508,
       0,     0,   218,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   555,     0,     0,   559,   274,     0,   269,   235,
     237,     0,   224,   239,     0,   271,   391,     0,    65,   157,
      55,     0,   374,   371,   363,   368,   400,     0,     0,   209,
     212,   215,   337,     0,   573,     0,   557,   550,   549,   573,
       0,     0,   277,     0,   225,   342,    60,    54,     0,   364,
       0,   220,     0,   338,     0,     0,   558,     0,   560,   275,
       0,   226,   343,    56,     0,     0,     0,     0,     0,     0,
     278,   372,   373,   221,   574,   575,   576
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -724,  -724,  -724,  -724,  -724,   306,   652,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,   687,  -724,   617,  -724,  -724,   661,
    -724,  -724,  -724,  -246,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,   457,  -724,  -724,   671,   -59,  -724,   654,    40,
    -323,  -278,  -384,  -724,  -724,  -724,  -723,  -724,  -167,  -724,
     -65,  -470,   -12,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
     740,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -455,  -724,  -724,  -140,  -724,     5,  -477,
    -724,  -363,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,   308,  -724,  -724,  -160,  -169,  -724,   -91,
    -724,  -724,  -724,   267,  -724,   202,  -724,  -338,   464,  -473,
    -472,    69,  -724,  -349,  -724,   733,  -724,  -724,  -724,   255,
      44,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -360,   -70,  -724,  -724,  -724,   495,  -724,  -724,
    -724,   -29,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
     482,  -545,  -413,  -555,  -724,  -724,  -301,    35,   363,  -724,
    -724,  -724,  -183,  -724
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   157,    45,    14,   177,   183,   424,   334,   760,
     335,   588,    15,    16,    38,    39,    72,    17,    34,    35,
     504,   505,   903,   904,   506,   507,   508,   509,   510,   511,
     512,   897,   184,   185,    30,    31,    32,    52,    53,    54,
      18,   264,   366,   167,    19,    86,   168,    87,   336,   513,
     367,   514,   337,   515,   338,   516,   517,   518,   519,   520,
     521,   522,   339,   559,   923,   560,   924,   562,   925,   340,
     557,   922,   341,   594,   943,   342,   771,   772,   597,   343,
     344,   651,   524,   345,   346,   347,   761,   971,   762,   990,
     348,   635,   891,   595,   885,   992,   362,   836,   980,   913,
     703,   530,   978,   363,   364,   399,   400,   212,   213,   742,
     478,   711,   537,   383,   271,    62,    96,    21,   174,   375,
      42,    75,    22,    90,    23,   377,   171,   172,    60,    93,
      24,   270,   384,   350,   374,   148,   149,   210,   150,   284,
     716,   401,   282,   715,   279,   712,   280,   919,   281,   714,
     285,   717,   286,   845,   287,   719,   288,   846,   289,   721,
     290,   847,   291,   724,   292,   726,    25,    47,    26,    46,
     351,   571,   572,   573,   352,   574,   575,   576,   577,   353,
     423,   354,   985,   355
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      55,   266,   479,   147,   523,   639,   644,   580,   393,   545,
     395,   547,   397,   549,   273,   422,   638,   746,   650,   435,
     476,   -94,   161,   666,   667,   753,   739,    97,    98,   663,
     566,   531,    68,   566,     2,   163,   941,   436,   437,   751,
       3,   294,   676,   790,   151,    27,    28,   764,   477,   446,
     834,   598,   448,   449,   640,    48,   272,   565,   272,   578,
      36,   146,    43,     4,    40,     5,    69,     6,   436,   437,
      55,   448,   449,     7,   472,   767,   834,   953,   708,   630,
     631,    37,     8,    49,   768,    74,    33,   709,     9,   740,
     178,   179,   265,   534,   641,    40,   642,    61,    44,   643,
     728,   211,   361,   835,   153,   379,   677,    10,   632,   164,
     165,   380,   468,   469,   473,   769,   433,   479,   710,   154,
     770,    82,   765,   438,   439,   440,   441,   647,    11,   835,
     763,   468,   469,   446,    41,   447,   448,   449,   450,   633,
     634,   475,   451,    63,   452,   453,    83,   527,   382,   166,
     434,   527,   927,   272,    50,    99,   440,   441,   583,   100,
     584,   101,   102,   534,   446,   745,   447,   448,   449,   450,
      51,   295,   675,   451,   787,    29,   534,    64,   791,    70,
     534,   599,   463,   464,   465,   466,   467,   528,   860,   296,
      71,   830,   801,    73,   699,   103,   468,   469,   297,   807,
     862,   750,   700,   754,   214,    56,    57,   701,    58,   147,
     147,   147,   147,   381,    12,   147,   476,   147,   180,   147,
      13,   147,   181,   640,   182,   102,   640,   468,   469,   765,
     147,    50,   741,   436,   437,   741,    59,   267,    80,   542,
     640,   842,   765,   413,   147,   147,    64,    51,   265,   930,
     386,   387,   388,   389,   697,   994,   392,   995,   394,   933,
     396,   543,   590,   641,   277,   268,   641,   146,   146,   146,
     146,   404,   947,   146,    80,   146,   551,   146,   765,   146,
     641,   765,   756,   698,   856,   418,   420,   147,   146,   765,
     765,   278,   749,    64,    50,   147,   868,   750,   552,   744,
     882,   550,   146,   146,   928,   766,   175,   176,   785,    65,
      51,   436,   437,   893,   934,   428,   848,   945,   532,   773,
     887,   440,   441,   653,   876,   883,    82,    66,   474,   446,
     859,   661,   448,   449,   450,   894,   482,   368,   451,   869,
     553,   369,  -516,  -523,   147,   146,   920,  -516,  -523,  -530,
    -276,   910,    40,   146,  -530,  -276,   274,   275,   370,   371,
     372,   373,   554,   900,   529,  -516,  -523,    77,   921,    78,
     702,   916,  -530,  -276,   901,   902,   534,    84,    88,   535,
      91,    79,   536,    85,    89,   564,    92,   198,   931,   884,
      94,    80,   468,   469,   556,   106,    95,   438,   439,   440,
     441,   442,   146,    71,   443,   444,   445,   446,    81,   447,
     448,   449,   450,   199,   200,   155,   451,   911,   452,   453,
     591,   156,   108,   912,   454,   455,   456,   175,   176,   585,
     586,   332,   665,   109,   265,   480,   198,   152,   481,   274,
     275,   276,   955,   110,   158,   159,   160,   169,   198,   457,
     170,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,    69,   199,   200,   187,   147,   173,   188,   189,   147,
     468,   469,   708,   201,   199,   200,   190,   192,   265,   789,
     147,   709,   147,   193,   875,   191,   208,   147,   797,   147,
     147,   937,   147,   194,   195,   196,   197,   209,   211,   261,
     262,   263,   265,   272,   806,   269,   707,   283,   293,   359,
     713,   378,   839,   203,   349,   356,   382,   204,   360,   390,
     205,   723,   201,   146,   357,   358,   365,   146,   731,   376,
     733,   734,   403,   736,   201,   206,   523,   407,   146,   409,
     146,   840,   410,   406,   207,   146,   412,   146,   146,   414,
     146,   408,   533,   411,   479,   413,   596,   432,    12,   471,
     600,   202,   203,   436,   437,   668,   204,   526,   538,   205,
     607,   609,   147,   202,   203,   656,   657,   658,   204,   669,
     670,   205,   672,   673,   206,   674,   704,   705,   706,   472,
     718,   747,   720,   207,   722,   757,   206,   759,   744,   748,
     763,   774,   539,   775,   777,   207,   779,   781,   783,   676,
     796,   798,   799,   786,   800,   833,   804,   838,   849,   805,
     415,   416,   837,   861,   421,   863,   864,   866,   873,   896,
     146,   878,   425,   426,   427,   952,   429,   430,   870,   431,
     879,   678,   679,   680,   681,   682,   683,   684,   685,   438,
     439,   440,   441,   442,   686,   687,   443,   444,   445,   446,
     793,   447,   448,   449,   450,   871,   880,   881,   451,   689,
     452,   453,   690,   691,   915,   525,   454,   455,   456,   692,
     693,   694,   926,   929,   932,   935,   958,    55,   938,   940,
     965,   147,   972,   966,   970,   979,   982,   986,   988,   997,
     794,   457,   998,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   999,  1005,   104,   558,   942,  1006,   561,
    1001,   563,   468,   469,    67,   186,   570,   107,   570,   795,
     587,   993,   874,   950,   385,   105,   162,   436,   437,   832,
     899,    20,   939,   855,   954,   914,   601,   602,   645,   146,
     603,   604,   605,   606,   725,   608,   402,   610,   611,   612,
     613,   614,   615,   616,   617,   619,   620,   621,   622,   623,
     624,   625,   626,   627,   628,   629,   841,   636,    76,   391,
     976,   696,   405,   581,   977,   867,   987,     0,     0,     0,
       0,     0,   646,     0,     0,     0,   652,     0,     0,   654,
     655,     0,     0,     0,     0,   660,     0,   664,     0,     0,
       0,     0,     0,     0,   671,   147,     0,     0,     0,     0,
     436,   437,     0,   438,   439,   440,   441,   442,  1002,     0,
     443,   444,   445,   446,     0,   447,   448,   449,   450,     0,
       0,     0,   451,     0,   452,   453,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   944,     0,     0,     0,
       0,     0,     0,     0,   730,     0,     0,     0,     0,     0,
       0,     0,     0,   146,     0,     0,   425,   431,   459,   460,
     461,   462,   463,   464,   465,   466,   467,   431,     0,     0,
     755,   421,     0,     0,   758,     0,   468,   469,     0,     0,
       0,     0,     0,     0,     0,     0,   438,   439,   440,   441,
     442,     0,     0,   443,   444,   445,   446,     0,   447,   448,
     449,   450,   436,   437,     0,   451,     0,   452,   453,     0,
       0,     0,     0,   454,   455,   456,     0,     0,     0,   780,
       0,     0,   782,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   788,     0,     0,     0,   792,     0,   457,   989,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     802,     0,     0,     0,     0,     0,     0,     0,  1000,   468,
     469,     0,     0,   589,     0,     0,   808,   809,   810,   811,
     812,   813,   814,   815,   816,   817,   820,   821,   822,   823,
     824,   825,   826,   829,     0,   831,     0,     0,   438,   439,
     440,   441,   442,     0,     0,   443,   444,   445,   446,   843,
     447,   448,   449,   450,     0,     0,     0,   451,     0,   452,
     453,     0,     0,     0,     0,   454,   455,   456,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   857,
     858,     0,     0,     0,     0,     0,   865,     0,   570,     0,
     457,     0,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   467,     0,   877,     0,     0,     0,   436,   437,     0,
       0,   468,   469,     0,     0,     0,   888,     0,   909,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   820,   829,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   905,   906,     0,     0,
       0,     0,     0,     0,     0,   907,   908,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   917,
       0,     0,     0,     0,     0,   678,   679,   680,   681,   682,
     683,   684,   685,   438,   439,   440,   441,   442,   686,   687,
     443,   444,   445,   446,   793,   447,   448,   449,   450,     0,
       0,     0,   451,   689,   452,   453,   690,   691,     0,     0,
     454,   455,   456,   692,   693,   694,     0,     0,     0,     0,
       0,   946,     0,     0,     0,   948,     0,   525,     0,     0,
       0,   951,     0,     0,   794,   457,     0,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,     0,   957,     0,
       0,   959,   960,   961,   963,   964,   468,   469,   967,   968,
       0,   969,     0,   803,     0,     0,   483,     0,     0,   974,
       0,     3,     0,   484,   485,   486,     0,   487,     0,   298,
     299,   300,   301,   302,     0,     0,     0,     0,     0,     0,
     488,   303,   489,   490,     0,     0,     0,     0,     0,     0,
       0,   991,   491,   304,     0,   492,     0,   493,   305,     0,
     996,   306,     0,     8,   307,   494,     0,   495,   308,     0,
       0,   496,   497,     0,     0,  1004,     0,     0,   498,   115,
     116,   117,     0,   119,   120,   121,   122,   123,   310,   125,
     126,   127,   128,   129,   130,   131,   132,   133,     0,   135,
     136,   137,     0,     0,   140,   141,   142,   143,     0,     0,
     311,   499,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   312,   313,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   500,   501,
       0,     0,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,   314,   315,   316,   317,   318,   319,
     320,   321,   322,    51,   323,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     324,   325,   326,     0,   327,     0,     0,   328,   329,     0,
       0,     0,     0,     0,     0,     0,     0,   331,     0,   332,
     333,   502,   265,   503,   483,     0,     0,     0,     0,     3,
       0,   484,   485,   486,     0,   487,     0,   298,   299,   300,
     301,   302,     0,     0,     0,     0,     0,     0,   488,   303,
     489,   490,     0,     0,     0,     0,     0,     0,     0,     0,
     491,   304,     0,   492,     0,   493,   305,     0,     0,   306,
       0,     8,   307,   494,     0,   495,   308,     0,     0,   496,
     497,     0,     0,     0,     0,     0,   498,   115,   116,   117,
       0,   119,   120,   121,   122,   123,   310,   125,   126,   127,
     128,   129,   130,   131,   132,   133,     0,   135,   136,   137,
       0,     0,   140,   141,   142,   143,     0,     0,   311,   499,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   312,   313,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,   437,     0,     0,   500,   501,     0,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,   314,   315,   316,   317,   318,   319,   320,   321,
     322,    51,   323,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   324,   325,
     326,     0,   327,     0,     0,   328,   329,     0,     0,     0,
     298,   299,   300,   301,   302,   331,     0,   332,   333,   502,
     265,   949,   303,     0,     0,     0,     0,     0,   198,     0,
       0,     0,     0,     0,   304,     0,     0,     0,     0,   305,
     440,   441,   306,     0,     0,   307,     0,   566,   446,   308,
     447,   448,   449,   450,   199,   200,     0,   451,     0,   309,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   310,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,     0,
       0,   311,     0,     0,     0,     0,     0,   463,   464,   465,
     466,   467,     0,     0,   312,   313,     0,     0,     0,     0,
       0,   468,   469,     0,   201,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,     0,
     198,     0,     0,     0,     0,   314,   315,   316,   317,   318,
     319,   320,   321,   322,   567,   323,     0,     0,     0,     0,
       0,     0,     0,   202,   203,     0,   199,   200,   204,     0,
       0,   568,   325,   326,     0,   327,     0,     0,   328,   329,
     298,   299,   300,   301,   302,     0,   206,     0,   569,     0,
     332,   333,   303,   265,     0,   207,     0,     0,   198,     0,
       0,     0,     0,     0,   304,     0,     0,     0,     0,   305,
       0,     0,   306,     0,     0,   307,     0,     0,     0,   308,
       0,     0,     0,     0,   199,   200,   201,     0,     0,   309,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   310,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,     0,
       0,   311,     0,     0,     0,   202,   203,     0,     0,     0,
     204,     0,   540,   205,   312,   313,     0,     0,     0,     0,
       0,     0,   436,   437,   201,     0,     0,     0,   206,     0,
       0,     0,     0,     0,     0,     0,     0,   207,    50,     0,
       0,     0,     0,     0,     0,   314,   315,   316,   317,   318,
     319,   320,   321,   322,   567,   323,     0,     0,     0,     0,
       0,     0,     0,   202,   203,     0,     0,     0,   204,     0,
       0,   568,   325,   326,     0,   327,     0,     0,   328,   329,
     298,   299,   300,   301,   302,     0,   206,     0,   579,     0,
     332,   333,   303,   265,     0,   207,     0,     0,   294,     0,
       0,     0,     0,     0,   304,     0,     0,     0,     0,   305,
     440,   441,   306,     0,     0,   307,     0,     0,   446,   308,
     447,   448,   449,   450,     0,     0,     0,   451,     0,   309,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   310,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,     0,
       0,   311,     0,     0,     0,     0,     0,     0,     0,   465,
     466,   467,     0,     0,   312,   313,     0,     0,     0,     0,
       0,   468,   469,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   198,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,   314,   315,   316,   317,   318,
     319,   320,   321,   322,    51,   323,     0,     0,     0,     0,
     199,   200,     0,     0,     0,     0,     0,     0,   295,     0,
       0,   324,   325,   326,     0,   327,     0,     0,   328,   329,
     298,   299,   300,   301,   302,     0,   296,     0,   331,     0,
     332,   333,   303,   265,     0,   297,     0,     0,     0,     0,
       0,     0,     0,     0,   304,     0,     0,     0,     0,   305,
       0,     0,   306,     0,     0,   307,     0,     0,     0,   308,
     201,     0,     0,     0,     0,     0,     0,     0,     0,   309,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   310,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,   202,
     203,   311,     0,     0,   204,     0,   541,   205,     0,     0,
       0,     0,     0,     0,   312,   313,     0,     0,     0,     0,
       0,     0,   206,   648,     0,     0,     0,     0,     0,   500,
     501,   207,   198,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,   314,   315,   316,   317,   318,
     319,   320,   321,   322,    51,   323,     0,     0,   199,   200,
       0,     0,     0,     0,     0,   298,   299,   300,   301,   302,
       0,   324,   325,   326,     0,   327,     0,   303,   328,   329,
       0,     0,     0,     0,     0,     0,     0,     0,   331,   304,
     332,   333,   649,   265,   305,     0,     0,   306,     0,     0,
     307,     0,     0,     0,   308,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   309,   115,   116,   117,   201,   119,
     120,   121,   122,   123,   310,   125,   126,   127,   128,   129,
     130,   131,   132,   133,     0,   135,   136,   137,     0,     0,
     140,   141,   142,   143,     0,     0,   311,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   202,   203,   312,
     313,   198,   204,     0,   544,   205,     0,     0,   662,     0,
       0,     0,     0,     0,   500,   501,     0,     0,     0,     0,
     206,     0,     0,    50,     0,     0,     0,   199,   200,   207,
     314,   315,   316,   317,   318,   319,   320,   321,   322,    51,
     323,     0,   298,   299,   300,   301,   302,     0,     0,     0,
       0,     0,     0,     0,   303,     0,   324,   325,   326,   198,
     327,     0,     0,   328,   329,     0,   304,     0,     0,     0,
       0,   305,     0,   331,   306,   332,   333,   307,   265,     0,
       0,   308,     0,     0,     0,   199,   200,   201,     0,     0,
       0,   309,   115,   116,   117,     0,   119,   120,   121,   122,
     123,   310,   125,   126,   127,   128,   129,   130,   131,   132,
     133,     0,   135,   136,   137,     0,     0,   140,   141,   142,
     143,     0,     0,   311,     0,     0,   202,   203,     0,     0,
       0,   204,     0,   546,   205,     0,   312,   313,     0,     0,
       0,     0,     0,     0,     0,   201,     0,     0,     0,   206,
       0,     0,     0,     0,     0,     0,   198,     0,   207,     0,
      50,     0,     0,     0,     0,     0,     0,   314,   315,   316,
     317,   318,   319,   320,   321,   322,    51,   323,     0,     0,
       0,     0,   199,   200,   202,   203,     0,     0,     0,   204,
       0,   548,   205,   324,   325,   326,     0,   327,     0,     0,
     328,   329,   298,   299,   300,   301,   302,   206,   592,     0,
     331,   593,   332,   333,   303,   265,   207,     0,     0,   198,
       0,     0,     0,     0,     0,     0,   304,     0,     0,     0,
       0,   305,     0,     0,   306,     0,     0,   307,     0,     0,
       0,   308,   201,     0,     0,   199,   200,     0,     0,     0,
       0,   309,   115,   116,   117,     0,   119,   120,   121,   122,
     123,   310,   125,   126,   127,   128,   129,   130,   131,   132,
     133,     0,   135,   136,   137,     0,     0,   140,   141,   142,
     143,   202,   203,   311,     0,     0,   204,     0,     0,   205,
       0,     0,     0,     0,     0,     0,   312,   313,   198,     0,
       0,     0,     0,     0,   206,   201,   555,     0,     0,     0,
       0,   500,   501,   207,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,   199,   200,     0,   314,   315,   316,
     317,   318,   319,   320,   321,   322,    51,   323,     0,   298,
     299,   300,   301,   302,   202,   203,     0,     0,     0,   204,
       0,   303,   205,   324,   325,   326,   198,   327,     0,     0,
     328,   329,     0,   304,     0,     0,     0,   206,   305,     0,
     331,   306,   332,   333,   307,   265,   207,     0,   308,     0,
       0,     0,   199,   200,   201,     0,     0,     0,   309,   115,
     116,   117,     0,   119,   120,   121,   122,   123,   310,   125,
     126,   127,   128,   129,   130,   131,   132,   133,     0,   135,
     136,   137,     0,     0,   140,   141,   142,   143,     0,     0,
     311,     0,     0,   202,   203,     0,     0,     0,   204,     0,
     738,   205,     0,   312,   313,   198,     0,     0,     0,     0,
       0,   640,   201,     0,     0,     0,   206,     0,   818,   819,
       0,     0,     0,     0,     0,   207,     0,    50,     0,     0,
       0,   199,   200,     0,   314,   315,   316,   317,   318,   319,
     320,   321,   322,    51,   323,     0,   298,   299,   300,   301,
     302,   892,   203,     0,     0,     0,   204,     0,   303,   205,
     324,   325,   326,     0,   327,     0,     0,   328,   329,     0,
     304,     0,     0,     0,   206,   305,     0,   331,   306,   332,
     333,   307,   265,   207,     0,   308,     0,     0,     0,     0,
       0,   201,     0,     0,     0,   309,   115,   116,   117,     0,
     119,   120,   121,   122,   123,   310,   125,   126,   127,   128,
     129,   130,   131,   132,   133,     0,   135,   136,   137,     0,
       0,   140,   141,   142,   143,     0,     0,   311,     0,     0,
     202,   203,     0,     0,     0,   204,     0,   844,   205,     0,
     312,   313,   198,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   206,     0,   827,   828,     0,     0,     0,
       0,     0,   207,     0,    50,     0,     0,     0,   199,   200,
       0,   314,   315,   316,   317,   318,   319,   320,   321,   322,
      51,   323,     0,   298,   299,   300,   301,   302,     0,     0,
       0,     0,     0,     0,     0,   303,     0,   324,   325,   326,
       0,   327,     0,     0,   328,   329,     0,   304,     0,     0,
       0,     0,   305,     0,   331,   306,   332,   333,   307,   265,
       0,     0,   308,     0,     0,     0,     0,     0,   201,     0,
       0,     0,   309,   115,   116,   117,     0,   119,   120,   121,
     122,   123,   310,   125,   126,   127,   128,   129,   130,   131,
     132,   133,     0,   135,   136,   137,     0,     0,   140,   141,
     142,   143,     0,     0,   311,     0,     0,   202,   203,     0,
       0,     0,   204,     0,   851,   205,     0,   312,   313,   198,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     206,     0,     0,     0,     0,     0,     0,     0,     0,   207,
       0,    50,     0,     0,     0,   199,   200,     0,   314,   315,
     316,   317,   318,   319,   320,   321,   322,    51,   323,     0,
     298,   299,   300,   301,   302,     0,     0,   618,     0,     0,
       0,     0,   303,     0,   324,   325,   326,     0,   327,     0,
       0,   328,   329,     0,   304,     0,     0,     0,     0,   305,
     330,   331,   306,   332,   333,   307,   265,     0,     0,   308,
       0,     0,     0,     0,     0,   201,     0,     0,     0,   309,
     115,   116,   117,     0,   119,   120,   121,   122,   123,   310,
     125,   126,   127,   128,   129,   130,   131,   132,   133,     0,
     135,   136,   137,     0,     0,   140,   141,   142,   143,     0,
       0,   311,     0,     0,   202,   203,     0,     0,     0,   204,
       0,   852,   205,     0,   312,   313,   198,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   206,     0,     0,
       0,     0,     0,     0,     0,     0,   207,     0,    50,     0,
       0,     0,   199,   200,     0,   314,   315,   316,   317,   318,
     319,   320,   321,   322,    51,   323,     0,   298,   299,   300,
     301,   302,     0,     0,     0,     0,     0,     0,     0,   303,
       0,   324,   325,   326,   198,   327,     0,     0,   328,   329,
       0,   304,     0,     0,     0,     0,   305,     0,   331,   306,
     332,   333,   307,   265,     0,     0,   308,     0,     0,     0,
     199,   200,   201,     0,     0,     0,   309,   115,   116,   117,
       0,   119,   120,   121,   122,   123,   310,   125,   126,   127,
     128,   129,   130,   131,   132,   133,     0,   135,   136,   137,
       0,     0,   140,   141,   142,   143,     0,     0,   311,     0,
       0,   202,   203,     0,     0,     0,   204,     0,   853,   205,
       0,   312,   313,     0,     0,     0,     0,     0,     0,     0,
     201,     0,     0,     0,   206,     0,     0,     0,     0,     0,
       0,   198,     0,   207,     0,    50,     0,     0,     0,     0,
       0,     0,   314,   315,   316,   317,   318,   319,   320,   321,
     322,    51,   323,     0,     0,     0,     0,   199,   200,   202,
     203,     0,     0,     0,   204,     0,   854,   205,   324,   325,
     326,     0,   327,     0,     0,   328,   329,   298,   299,   300,
     301,   302,   206,     0,     0,   331,   637,   332,   333,   303,
     265,   207,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   304,     0,     0,     0,     0,   305,     0,     0,   306,
       0,     0,   307,     0,     0,     0,   308,   201,     0,   659,
       0,     0,     0,     0,     0,     0,   309,   115,   116,   117,
       0,   119,   120,   121,   122,   123,   310,   125,   126,   127,
     128,   129,   130,   131,   132,   133,     0,   135,   136,   137,
       0,     0,   140,   141,   142,   143,   202,   203,   311,     0,
       0,   204,     0,   936,   205,     0,     0,     0,     0,     0,
       0,   312,   313,   198,     0,     0,     0,     0,     0,   206,
       0,     0,     0,     0,     0,     0,     0,     0,   207,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,   199,
     200,     0,   314,   315,   316,   317,   318,   319,   320,   321,
     322,    51,   323,     0,   298,   299,   300,   301,   302,     0,
       0,     0,     0,     0,     0,     0,   303,     0,   324,   325,
     326,     0,   327,     0,     0,   328,   329,     0,   304,     0,
       0,     0,     0,   305,     0,   331,   306,   332,   333,   307,
     265,     0,     0,   308,     0,     0,     0,     0,     0,   201,
       0,     0,     0,   309,   115,   116,   117,     0,   119,   120,
     121,   122,   123,   310,   125,   126,   127,   128,   129,   130,
     131,   132,   133,     0,   135,   136,   137,     0,     0,   140,
     141,   142,   143,     0,     0,   311,     0,     0,   202,   203,
       0,     0,     0,   204,     0,   975,   205,     0,   312,   313,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   206,     0,     0,     0,     0,     0,     0,     0,     0,
     207,     0,    50,     0,     0,     0,     0,     0,     0,   314,
     315,   316,   317,   318,   319,   320,   321,   322,    51,   323,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   324,   325,   326,     0,   327,
       0,     0,   328,   329,   298,   299,   300,   301,   302,     0,
       0,     0,   331,   727,   332,   333,   303,   265,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,     0,     0,   305,     0,     0,   306,     0,     0,   307,
       0,     0,     0,   308,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   309,   115,   116,   117,     0,   119,   120,
     121,   122,   123,   310,   125,   126,   127,   128,   129,   130,
     131,   132,   133,     0,   135,   136,   137,     0,     0,   140,
     141,   142,   143,     0,     0,   311,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   312,   313,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,   314,
     315,   316,   317,   318,   319,   320,   321,   322,    51,   323,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   729,     0,   324,   325,   326,     0,   327,
       0,     0,   328,   329,   298,   299,   300,   301,   302,     0,
       0,     0,   331,     0,   332,   333,   303,   265,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,     0,     0,   305,     0,     0,   306,     0,     0,   307,
       0,     0,     0,   308,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   309,   115,   116,   117,     0,   119,   120,
     121,   122,   123,   310,   125,   126,   127,   128,   129,   130,
     131,   132,   133,     0,   135,   136,   137,     0,     0,   140,
     141,   142,   143,     0,     0,   311,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   312,   313,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,   314,
     315,   316,   317,   318,   319,   320,   321,   322,    51,   323,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   324,   325,   326,     0,   327,
       0,     0,   328,   329,   298,   299,   300,   301,   302,     0,
       0,     0,   331,   743,   332,   333,   303,   265,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,     0,     0,   305,     0,     0,   306,     0,     0,   307,
       0,     0,     0,   308,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   309,   115,   116,   117,     0,   119,   120,
     121,   122,   123,   310,   125,   126,   127,   128,   129,   130,
     131,   132,   133,     0,   135,   136,   137,     0,     0,   140,
     141,   142,   143,     0,     0,   311,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   312,   313,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,   314,
     315,   316,   317,   318,   319,   320,   321,   322,    51,   323,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   324,   325,   326,     0,   327,
       0,     0,   328,   329,   298,   299,   300,   301,   302,     0,
       0,     0,   331,   752,   332,   333,   303,   265,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,     0,     0,   305,     0,     0,   306,     0,     0,   307,
       0,     0,     0,   308,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   309,   115,   116,   117,     0,   119,   120,
     121,   122,   123,   310,   125,   126,   127,   128,   129,   130,
     131,   132,   133,     0,   135,   136,   137,     0,     0,   140,
     141,   142,   143,     0,     0,   311,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   312,   313,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,   314,
     315,   316,   317,   318,   319,   320,   321,   322,    51,   323,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   324,   325,   326,     0,   327,
       0,     0,   328,   329,   298,   299,   300,   301,   302,     0,
       0,     0,   331,   886,   332,   333,   303,   265,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,     0,     0,   305,     0,     0,   306,     0,     0,   307,
       0,     0,     0,   308,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   309,   115,   116,   117,     0,   119,   120,
     121,   122,   123,   310,   125,   126,   127,   128,   129,   130,
     131,   132,   133,     0,   135,   136,   137,     0,     0,   140,
     141,   142,   143,     0,     0,   311,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   312,   313,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,   314,
     315,   316,   317,   318,   319,   320,   321,   322,    51,   323,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   324,   325,   326,     0,   327,
       0,     0,   328,   329,   298,   299,   300,   301,   302,     0,
       0,     0,   331,   962,   332,   333,   303,   265,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,     0,     0,   305,     0,     0,   306,     0,     0,   307,
       0,     0,     0,   308,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   309,   115,   116,   117,     0,   119,   120,
     121,   122,   123,   310,   125,   126,   127,   128,   129,   130,
     131,   132,   133,     0,   135,   136,   137,     0,     0,   140,
     141,   142,   143,     0,     0,   311,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   312,   313,
       0,     0,     0,     0,     0,     0,     0,   973,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,   314,
     315,   316,   317,   318,   319,   320,   321,   322,    51,   323,
       0,   298,   299,   300,   301,   302,     0,     0,     0,     0,
       0,     0,     0,   303,     0,   324,   325,   326,     0,   327,
       0,     0,   328,   329,     0,   304,     0,     0,     0,     0,
     305,     0,   331,   306,   332,   333,   307,   265,     0,     0,
     308,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     309,   115,   116,   117,     0,   119,   120,   121,   122,   123,
     310,   125,   126,   127,   128,   129,   130,   131,   132,   133,
       0,   135,   136,   137,     0,     0,   140,   141,   142,   143,
       0,     0,   311,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   312,   313,   436,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,   314,   315,   316,   317,
     318,   319,   320,   321,   322,    51,   323,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   324,   325,   326,     0,   327,     0,     0,   328,
     329,     0,     0,     0,     0,     0,     0,     0,     0,   331,
       0,   332,   333,     0,   265,   678,   679,   680,   681,   682,
     683,   684,   685,   438,   439,   440,   441,   442,   686,   687,
     443,   444,   445,   446,   793,   447,   448,   449,   450,   436,
     437,     0,   451,   689,   452,   453,   690,   691,     0,     0,
     454,   455,   456,   692,   693,   694,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   794,   457,     0,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   468,   469,     0,     0,
       0,     0,     0,   895,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   678,   679,   680,
     681,   682,   683,   684,   685,   438,   439,   440,   441,   442,
     686,   687,   443,   444,   445,   446,   793,   447,   448,   449,
     450,   436,   437,     0,   451,   689,   452,   453,   690,   691,
       0,     0,   454,   455,   456,   692,   693,   694,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   794,   457,     0,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   468,   469,
       0,     0,     0,     0,     0,   898,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   678,
     679,   680,   681,   682,   683,   684,   685,   438,   439,   440,
     441,   442,   686,   687,   443,   444,   445,   446,   793,   447,
     448,   449,   450,   436,   437,     0,   451,   689,   452,   453,
     690,   691,     0,     0,   454,   455,   456,   692,   693,   694,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   794,   457,
       0,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     468,   469,     0,     0,     0,     0,     0,   918,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   436,   437,
       0,   678,   679,   680,   681,   682,   683,   684,   685,   438,
     439,   440,   441,   442,   686,   687,   443,   444,   445,   446,
     793,   447,   448,   449,   450,     0,     0,     0,   451,   689,
     452,   453,   690,   691,     0,     0,   454,   455,   456,   692,
     693,   694,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     794,   457,     0,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   436,   437,     0,     0,     0,     0,     0,
       0,     0,   468,   469,   438,   439,   440,   441,   442,   956,
       0,   443,   444,   445,   446,     0,   447,   448,   449,   450,
       0,     0,     0,   451,     0,   452,   453,     0,     0,     0,
       0,   454,   455,   456,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   457,     0,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   467,     0,     0,
       0,   436,   437,     0,     0,     0,     0,   468,   469,   438,
     439,   440,   441,   442,   984,     0,   443,   444,   445,   446,
       0,   447,   448,   449,   450,     0,     0,     0,   451,     0,
     452,   453,     0,     0,     0,     0,   454,   455,   456,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   457,     0,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,     0,     0,     0,     0,     0,     0,   436,
     437,     0,   468,   469,     0,     0,   732,   438,   439,   440,
     441,   442,     0,     0,   443,   444,   445,   446,     0,   447,
     448,   449,   450,     0,     0,     0,   451,     0,   452,   453,
       0,     0,     0,     0,   454,   455,   456,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   457,
       0,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,     0,     0,     0,     0,     0,     0,   436,   437,     0,
     468,   469,     0,     0,   735,   438,   439,   440,   441,   442,
       0,     0,   443,   444,   445,   446,     0,   447,   448,   449,
     450,     0,     0,     0,   451,     0,   452,   453,     0,     0,
       0,     0,   454,   455,   456,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   457,     0,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,     0,
       0,     0,     0,     0,     0,   436,   437,     0,   468,   469,
       0,     0,   737,   438,   439,   440,   441,   442,     0,     0,
     443,   444,   445,   446,     0,   447,   448,   449,   450,     0,
       0,     0,   451,     0,   452,   453,     0,     0,     0,     0,
     454,   455,   456,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   457,     0,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,     0,     0,     0,
       0,     0,     0,   436,   437,     0,   468,   469,     0,     0,
     850,   438,   439,   440,   441,   442,     0,     0,   443,   444,
     445,   446,     0,   447,   448,   449,   450,     0,     0,     0,
     451,     0,   452,   453,     0,     0,     0,     0,   454,   455,
     456,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   457,     0,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,     0,     0,     0,     0,     0,
       0,   436,   437,     0,   468,   469,     0,     0,   981,   438,
     439,   440,   441,   442,     0,     0,   443,   444,   445,   446,
       0,   447,   448,   449,   450,     0,     0,     0,   451,     0,
     452,   453,     0,     0,     0,     0,   454,   455,   456,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   457,     0,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,     0,     0,     0,     0,     0,     0,   436,
     437,     0,   468,   469,     0,     0,   983,   438,   439,   440,
     441,   442,     0,     0,   443,   444,   445,   446,     0,   447,
     448,   449,   450,     0,     0,     0,   451,     0,   452,   453,
       0,     0,     0,     0,   454,   455,   456,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   457,
       0,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,     0,     0,     0,     0,   436,   437,     0,     0,     0,
     468,   469,     0,     0,  1003,   438,   439,   440,   441,   442,
       0,     0,   443,   444,   445,   446,     0,   447,   448,   449,
     450,     0,     0,     0,   451,     0,   452,   453,     0,     0,
       0,     0,   454,   455,   456,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   457,     0,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,     0,
       0,   436,   437,     0,     0,     0,     0,     0,   468,   469,
     470,   438,   439,   440,   441,   442,     0,     0,   443,   444,
     445,   446,     0,   447,   448,   449,   450,     0,     0,     0,
     451,     0,   452,   453,     0,     0,     0,     0,   454,   455,
     456,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   457,     0,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,     0,     0,   436,   437,     0,
       0,     0,     0,     0,   468,   469,   776,   438,   439,   440,
     441,   442,     0,     0,   443,   444,   445,   446,     0,   447,
     448,   449,   450,     0,     0,     0,   451,     0,   452,   453,
       0,     0,     0,     0,   454,   455,   456,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   457,
       0,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,     0,     0,   436,   437,     0,     0,     0,     0,     0,
     468,   469,   784,   438,   439,   440,   441,   442,     0,     0,
     443,   444,   445,   446,     0,   447,   448,   449,   450,     0,
       0,     0,   451,     0,   452,   453,     0,     0,     0,     0,
     454,   455,   456,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   457,     0,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   468,   469,   889,   438,
     439,   440,   441,   442,   436,   437,   443,   444,   445,   446,
       0,   447,   448,   449,   450,     0,     0,     0,   451,     0,
     452,   453,     0,     0,     0,     0,   454,   455,   456,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   457,     0,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   468,   469,   890,     0,     0,     0,     0,     0,
       0,     0,   678,   679,   680,   681,   682,   683,   684,   685,
     438,   439,   440,   441,   442,   686,   687,   443,   444,   445,
     446,   793,   447,   448,   449,   450,  -246,   436,   437,   451,
     689,   452,   453,   690,   691,     0,     0,   454,   455,   456,
     692,   693,   694,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   794,   457,     0,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   468,   469,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,   437,     0,     0,     0,
       0,     0,     0,     0,     0,   678,   679,   680,   681,   682,
     683,   684,   685,   438,   439,   440,   441,   442,   686,   687,
     443,   444,   445,   446,   688,   447,   448,   449,   450,     0,
       0,     0,   451,   689,   452,   453,   690,   691,     0,     0,
     454,   455,   456,   692,   693,   694,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   695,   457,     0,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,   436,   437,     0,
       0,   438,   439,   440,   441,   442,   468,   469,   443,   444,
     445,   446,     0,   447,   448,   449,   450,     0,     0,     0,
     451,     0,   452,   453,     0,     0,   582,     0,   454,   455,
     456,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   457,     0,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   436,   437,     0,     0,     0,
       0,     0,     0,     0,   468,   469,     0,     0,     0,     0,
       0,     0,     0,   438,   439,   440,   441,   442,     0,     0,
     443,   444,   445,   446,     0,   447,   448,   449,   450,     0,
       0,     0,   451,     0,   452,   453,     0,     0,     0,     0,
     454,   455,   456,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,   437,   457,   778,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,     0,     0,     0,
       0,   438,   439,   440,   441,   442,   468,   469,   443,   444,
     445,   446,     0,   447,   448,   449,   450,     0,     0,     0,
     451,     0,   452,   453,     0,     0,     0,     0,   454,   455,
     456,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   872,
       0,   436,   437,   457,     0,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,     0,     0,     0,     0,   438,
     439,   440,   441,   442,   468,   469,   443,   444,   445,   446,
       0,   447,   448,   449,   450,     0,     0,     0,   451,     0,
     452,   453,     0,     0,     0,     0,   454,   455,   456,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
     437,   457,     0,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,     0,     0,     0,     0,   438,   439,   440,
     441,   442,   468,   469,   443,   444,   445,   446,     0,   447,
     448,   449,   450,     0,     0,     0,   451,     0,   452,   453,
       0,     0,     0,     0,   454,     0,   456,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   436,   437,     0,
       0,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,     0,     0,     0,     0,   438,   439,   440,   441,   442,
     468,   469,   443,   444,   445,   446,     0,   447,   448,   449,
     450,     0,     0,     0,   451,     0,   452,   453,     0,     0,
       0,     0,   454,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,   437,     0,     0,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,     0,
       0,     0,     0,   438,   439,   440,   441,   442,   468,   469,
     443,   444,   445,   446,     0,   447,   448,   449,   450,     0,
       0,     0,   451,     0,   452,   453,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,   437,     0,     0,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,     0,     0,     0,
       0,   438,   439,   440,   441,   442,   468,   469,   443,   444,
     445,   446,     0,   447,   448,   449,   450,     0,     0,     0,
     451,     0,   452,   453,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,   437,     0,     0,     0,     0,   460,   461,   462,
     463,   464,   465,   466,   467,     0,     0,     0,     0,   438,
     439,     0,     0,   442,   468,   469,   443,   444,   445,   446,
       0,   447,   448,   449,   450,   436,   437,     0,   451,     0,
     452,   453,     0,     0,     0,     0,   454,   455,   456,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   457,     0,   458,   459,   460,   461,   462,     0,     0,
       0,   466,     0,     0,     0,     0,     0,   438,   439,   440,
     441,   442,   468,   469,   443,   444,   445,   446,     0,   447,
     448,   449,   450,     0,     0,     0,   451,     0,   452,   453,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   439,   440,   441,   442,     0,     0,   443,     0,
       0,   446,     0,   447,   448,   449,   450,   215,     0,     0,
     451,     0,   452,   453,   461,   462,   463,   464,   465,   466,
     467,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     468,   469,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   461,   462,
     463,   464,   465,   466,   467,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   468,   469,     0,     0,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,     0,     0,     0,   234,
     235,     0,     0,     0,     0,     0,   236,   237,   238,   239,
     240,     0,     0,   241,   242,   243,   244,   245,   246,   417,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   111,     0,     0,     0,     0,     0,   112,     0,     0,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
       0,     0,   257,   258,   113,     0,     0,     0,     0,   259,
     260,     0,     0,     0,     0,     0,     0,     0,     0,   114,
       0,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,     0,     0,     0,     0,   419,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   111,     0,
       0,     0,     0,     0,   112,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
       0,   113,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,   114,     0,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   111,     0,     0,     0,     0,
       0,   112,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,   113,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,   114,     0,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   111,     0,     0,     0,     0,     0,   112,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,     0,   113,     0,     0,     0,     0,
     314,   315,   316,     0,     0,     0,     0,     0,     0,    51,
     114,     0,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   111,
       0,     0,     0,     0,     0,   112,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,     0,   113,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    51,   114,     0,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   398
};

static const yytype_int16 yycheck[] =
{
      12,   168,   362,    73,   367,   477,   479,   420,   286,   393,
     288,   395,   290,   397,   174,   316,   471,   572,   488,   342,
     358,     7,    81,   500,   501,   580,    19,    14,    15,   499,
      51,   380,    21,    51,     0,    34,   131,    20,    21,    19,
       6,    32,   122,    19,    73,    18,    19,   592,    48,   116,
      44,    18,   119,   120,   117,    33,   139,   417,   139,   419,
     148,    73,   148,    29,   148,    31,    55,    33,    20,    21,
      82,   119,   120,    39,   148,   117,    44,    45,   117,   119,
     120,   169,    48,    61,   126,    41,   142,   126,    54,   566,
      14,    15,   187,   156,   157,   148,   159,    60,   184,   162,
     555,   148,   183,    97,   188,   188,   186,    73,   148,   108,
     109,   271,   179,   180,   188,   157,   148,   477,   157,    75,
     162,   156,   156,   106,   107,   108,   109,   487,    94,    97,
     148,   179,   180,   116,   187,   118,   119,   120,   121,   179,
     180,   188,   125,   157,   127,   128,   181,   148,   148,   148,
     182,   148,   186,   139,   132,   142,   108,   109,   186,   146,
     188,   148,   149,   156,   116,   186,   118,   119,   120,   121,
     148,   162,   521,   125,   644,   148,   156,   156,   648,   168,
     156,   148,   165,   166,   167,   168,   169,   188,   743,   180,
     179,   188,   662,   157,   180,   182,   179,   180,   189,   676,
     745,   186,   188,   188,   183,     4,     5,   530,     7,   279,
     280,   281,   282,   272,   180,   285,   554,   287,   142,   289,
     186,   291,   146,   117,   148,   149,   117,   179,   180,   156,
     300,   132,   126,    20,    21,   126,    35,   159,   132,   164,
     117,   711,   156,   182,   314,   315,   156,   148,   187,   126,
     279,   280,   281,   282,   157,   978,   285,   980,   287,   186,
     289,   186,   163,   157,   156,   187,   157,   279,   280,   281,
     282,   300,   186,   285,   132,   287,   164,   289,   156,   291,
     157,   156,   583,   186,   739,   314,   315,   357,   300,   156,
     156,   183,   181,   156,   132,   365,   751,   186,   186,   156,
     156,   159,   314,   315,   859,   183,   150,   151,   183,   159,
     148,    20,    21,   786,   869,   327,   183,   183,   181,   597,
     775,   108,   109,   490,   181,   181,   156,   156,   357,   116,
     743,   498,   119,   120,   121,   790,   365,    68,   125,   752,
     164,    72,   159,   159,   414,   357,   164,   164,   164,   159,
     159,   181,   148,   365,   164,   164,   150,   151,    89,    90,
      91,    92,   186,    11,   376,   182,   182,   148,   186,   148,
     530,   841,   182,   182,    22,    23,   156,    55,    55,   159,
      55,   148,   162,    61,    61,   414,    61,    32,   861,   773,
      55,   132,   179,   180,   406,   142,    61,   106,   107,   108,
     109,   110,   414,   179,   113,   114,   115,   116,   182,   118,
     119,   120,   121,    58,    59,    55,   125,    55,   127,   128,
     432,    61,   148,    61,   133,   134,   135,   150,   151,   152,
     153,   184,   185,   148,   187,   183,    32,   186,   186,   150,
     151,   152,   914,   148,   187,   187,   148,   148,    32,   158,
      97,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,    55,    58,    59,   163,   535,   187,   163,   163,   539,
     179,   180,   117,   118,    58,    59,   163,   163,   187,   646,
     550,   126,   552,   163,   762,   182,   186,   557,   655,   559,
     560,   875,   562,   163,   163,   163,   163,   148,   148,    34,
      34,   182,   187,   139,   671,   148,   535,   148,    32,   181,
     539,   187,   157,   158,   206,   186,   148,   162,   181,   183,
     165,   550,   118,   535,   159,   186,   159,   539,   557,   159,
     559,   560,   181,   562,   118,   180,   899,   163,   550,   163,
     552,   186,   163,   182,   189,   557,   163,   559,   560,   163,
     562,   182,    41,   182,   914,   182,   139,   185,   180,   182,
     148,   157,   158,    20,    21,    36,   162,   187,   164,   165,
     148,   148,   642,   157,   158,   148,   186,   142,   162,   186,
     186,   165,   186,   186,   180,   186,   181,   148,   148,   148,
     164,   181,   164,   189,   164,   188,   180,   150,   156,   156,
     148,   163,   186,   182,   148,   189,   148,   148,     1,   122,
      13,   157,   159,   642,   142,     7,   185,    41,   148,   187,
     312,   313,   186,   148,   316,   181,   181,   181,   150,   796,
     642,   148,   324,   325,   326,   186,   328,   329,   188,   331,
     148,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   181,   148,   148,   125,   126,
     127,   128,   129,   130,   148,   367,   133,   134,   135,   136,
     137,   138,   182,   181,   181,   181,   148,   699,   183,   181,
     181,   761,   164,   181,   188,    32,   164,   181,   181,    51,
     157,   158,   181,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   188,   181,    63,   408,   884,   181,   411,
     186,   413,   179,   180,    37,   108,   418,    66,   420,   186,
     424,   977,   761,   900,   277,    64,    82,    20,    21,   699,
     805,     1,   882,   738,   913,   836,   438,   439,   481,   761,
     442,   443,   444,   445,   552,   447,   292,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,   707,   469,    45,   284,
     947,   526,   300,   420,   951,   750,   969,    -1,    -1,    -1,
      -1,    -1,   484,    -1,    -1,    -1,   488,    -1,    -1,   491,
     492,    -1,    -1,    -1,    -1,   497,    -1,   499,    -1,    -1,
      -1,    -1,    -1,    -1,   506,   885,    -1,    -1,    -1,    -1,
      20,    21,    -1,   106,   107,   108,   109,   110,   995,    -1,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   885,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   556,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   885,    -1,    -1,   568,   569,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   579,    -1,    -1,
     582,   583,    -1,    -1,   586,    -1,   179,   180,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,
     110,    -1,    -1,   113,   114,   115,   116,    -1,   118,   119,
     120,   121,    20,    21,    -1,   125,    -1,   127,   128,    -1,
      -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,   631,
      -1,    -1,   634,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   644,    -1,    -1,    -1,   648,    -1,   158,   971,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     662,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   990,   179,
     180,    -1,    -1,   183,    -1,    -1,   678,   679,   680,   681,
     682,   683,   684,   685,   686,   687,   688,   689,   690,   691,
     692,   693,   694,   695,    -1,   697,    -1,    -1,   106,   107,
     108,   109,   110,    -1,    -1,   113,   114,   115,   116,   711,
     118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,
     128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   741,
     742,    -1,    -1,    -1,    -1,    -1,   748,    -1,   750,    -1,
     158,    -1,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,    -1,   765,    -1,    -1,    -1,    20,    21,    -1,
      -1,   179,   180,    -1,    -1,    -1,   778,    -1,   186,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   793,   794,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   818,   819,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   827,   828,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   841,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,    -1,
      -1,    -1,   125,   126,   127,   128,   129,   130,    -1,    -1,
     133,   134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,
      -1,   893,    -1,    -1,    -1,   897,    -1,   899,    -1,    -1,
      -1,   903,    -1,    -1,   157,   158,    -1,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,    -1,   920,    -1,
      -1,   923,   924,   925,   926,   927,   179,   180,   930,   931,
      -1,   933,    -1,   186,    -1,    -1,     1,    -1,    -1,   941,
      -1,     6,    -1,     8,     9,    10,    -1,    12,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   973,    37,    38,    -1,    40,    -1,    42,    43,    -1,
     982,    46,    -1,    48,    49,    50,    -1,    52,    53,    -1,
      -1,    56,    57,    -1,    -1,   997,    -1,    -1,    63,    64,
      65,    66,    -1,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    -1,    84,
      85,    86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,
      95,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     165,   166,   167,    -1,   169,    -1,    -1,   172,   173,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,    -1,   184,
     185,   186,   187,   188,     1,    -1,    -1,    -1,    -1,     6,
      -1,     8,     9,    10,    -1,    12,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    38,    -1,    40,    -1,    42,    43,    -1,    -1,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    -1,    56,
      57,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    -1,   123,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,   166,
     167,    -1,   169,    -1,    -1,   172,   173,    -1,    -1,    -1,
      14,    15,    16,    17,    18,   182,    -1,   184,   185,   186,
     187,   188,    26,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
     108,   109,    46,    -1,    -1,    49,    -1,    51,   116,    53,
     118,   119,   120,   121,    58,    59,    -1,   125,    -1,    63,
      64,    65,    66,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    -1,    89,    90,    91,    92,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,   165,   166,   167,
     168,   169,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,
      -1,   179,   180,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,
      32,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,   158,    -1,    58,    59,   162,    -1,
      -1,   165,   166,   167,    -1,   169,    -1,    -1,   172,   173,
      14,    15,    16,    17,    18,    -1,   180,    -1,   182,    -1,
     184,   185,    26,   187,    -1,   189,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    58,    59,   118,    -1,    -1,    63,
      64,    65,    66,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    -1,    89,    90,    91,    92,    -1,
      -1,    95,    -1,    -1,    -1,   157,   158,    -1,    -1,    -1,
     162,    -1,   164,   165,   108,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,   118,    -1,    -1,    -1,   180,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,   132,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,   158,    -1,    -1,    -1,   162,    -1,
      -1,   165,   166,   167,    -1,   169,    -1,    -1,   172,   173,
      14,    15,    16,    17,    18,    -1,   180,    -1,   182,    -1,
     184,   185,    26,   187,    -1,   189,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
     108,   109,    46,    -1,    -1,    49,    -1,    -1,   116,    53,
     118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,    63,
      64,    65,    66,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    -1,    89,    90,    91,    92,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
     168,   169,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,
      -1,   179,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,    -1,    -1,    -1,    -1,
      58,    59,    -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,
      -1,   165,   166,   167,    -1,   169,    -1,    -1,   172,   173,
      14,    15,    16,    17,    18,    -1,   180,    -1,   182,    -1,
     184,   185,    26,   187,    -1,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    -1,    -1,    -1,    53,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      64,    65,    66,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    -1,    89,    90,    91,    92,   157,
     158,    95,    -1,    -1,   162,    -1,   164,   165,    -1,    -1,
      -1,    -1,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,
      -1,    -1,   180,   117,    -1,    -1,    -1,    -1,    -1,   123,
     124,   189,    32,    -1,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,    -1,    -1,    58,    59,
      -1,    -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,   165,   166,   167,    -1,   169,    -1,    26,   172,   173,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,    38,
     184,   185,   186,   187,    43,    -1,    -1,    46,    -1,    -1,
      49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    65,    66,   118,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    -1,
      89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,   158,   108,
     109,    32,   162,    -1,   164,   165,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,   123,   124,    -1,    -1,    -1,    -1,
     180,    -1,    -1,   132,    -1,    -1,    -1,    58,    59,   189,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    -1,   165,   166,   167,    32,
     169,    -1,    -1,   172,   173,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,   182,    46,   184,   185,    49,   187,    -1,
      -1,    53,    -1,    -1,    -1,    58,    59,   118,    -1,    -1,
      -1,    63,    64,    65,    66,    -1,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    -1,    84,    85,    86,    -1,    -1,    89,    90,    91,
      92,    -1,    -1,    95,    -1,    -1,   157,   158,    -1,    -1,
      -1,   162,    -1,   164,   165,    -1,   108,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,   180,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,   189,    -1,
     132,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,    -1,    -1,
      -1,    -1,    58,    59,   157,   158,    -1,    -1,    -1,   162,
      -1,   164,   165,   165,   166,   167,    -1,   169,    -1,    -1,
     172,   173,    14,    15,    16,    17,    18,   180,   180,    -1,
     182,   183,   184,   185,    26,   187,   189,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    -1,    -1,
      -1,    53,   118,    -1,    -1,    58,    59,    -1,    -1,    -1,
      -1,    63,    64,    65,    66,    -1,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    -1,    84,    85,    86,    -1,    -1,    89,    90,    91,
      92,   157,   158,    95,    -1,    -1,   162,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,   108,   109,    32,    -1,
      -1,    -1,    -1,    -1,   180,   118,   182,    -1,    -1,    -1,
      -1,   123,   124,   189,    -1,    -1,    -1,    -1,    -1,    -1,
     132,    -1,    -1,    -1,    58,    59,    -1,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,    -1,    14,
      15,    16,    17,    18,   157,   158,    -1,    -1,    -1,   162,
      -1,    26,   165,   165,   166,   167,    32,   169,    -1,    -1,
     172,   173,    -1,    38,    -1,    -1,    -1,   180,    43,    -1,
     182,    46,   184,   185,    49,   187,   189,    -1,    53,    -1,
      -1,    -1,    58,    59,   118,    -1,    -1,    -1,    63,    64,
      65,    66,    -1,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    -1,    84,
      85,    86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,
      95,    -1,    -1,   157,   158,    -1,    -1,    -1,   162,    -1,
     164,   165,    -1,   108,   109,    32,    -1,    -1,    -1,    -1,
      -1,   117,   118,    -1,    -1,    -1,   180,    -1,   123,   124,
      -1,    -1,    -1,    -1,    -1,   189,    -1,   132,    -1,    -1,
      -1,    58,    59,    -1,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    -1,    14,    15,    16,    17,
      18,   157,   158,    -1,    -1,    -1,   162,    -1,    26,   165,
     165,   166,   167,    -1,   169,    -1,    -1,   172,   173,    -1,
      38,    -1,    -1,    -1,   180,    43,    -1,   182,    46,   184,
     185,    49,   187,   189,    -1,    53,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    63,    64,    65,    66,    -1,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    -1,    84,    85,    86,    -1,
      -1,    89,    90,    91,    92,    -1,    -1,    95,    -1,    -1,
     157,   158,    -1,    -1,    -1,   162,    -1,   164,   165,    -1,
     108,   109,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    -1,   123,   124,    -1,    -1,    -1,
      -1,    -1,   189,    -1,   132,    -1,    -1,    -1,    58,    59,
      -1,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,   165,   166,   167,
      -1,   169,    -1,    -1,   172,   173,    -1,    38,    -1,    -1,
      -1,    -1,    43,    -1,   182,    46,   184,   185,    49,   187,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    63,    64,    65,    66,    -1,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    -1,    84,    85,    86,    -1,    -1,    89,    90,
      91,    92,    -1,    -1,    95,    -1,    -1,   157,   158,    -1,
      -1,    -1,   162,    -1,   164,   165,    -1,   108,   109,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,
      -1,   132,    -1,    -1,    -1,    58,    59,    -1,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    26,    -1,   165,   166,   167,    -1,   169,    -1,
      -1,   172,   173,    -1,    38,    -1,    -1,    -1,    -1,    43,
     181,   182,    46,   184,   185,    49,   187,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    63,
      64,    65,    66,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    -1,    89,    90,    91,    92,    -1,
      -1,    95,    -1,    -1,   157,   158,    -1,    -1,    -1,   162,
      -1,   164,   165,    -1,   108,   109,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,   132,    -1,
      -1,    -1,    58,    59,    -1,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,   165,   166,   167,    32,   169,    -1,    -1,   172,   173,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,   182,    46,
     184,   185,    49,   187,    -1,    -1,    53,    -1,    -1,    -1,
      58,    59,   118,    -1,    -1,    -1,    63,    64,    65,    66,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,    -1,
      -1,   157,   158,    -1,    -1,    -1,   162,    -1,   164,   165,
      -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,   189,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,    -1,    -1,    -1,    -1,    58,    59,   157,
     158,    -1,    -1,    -1,   162,    -1,   164,   165,   165,   166,
     167,    -1,   169,    -1,    -1,   172,   173,    14,    15,    16,
      17,    18,   180,    -1,    -1,   182,   183,   184,   185,    26,
     187,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    -1,    -1,    -1,    53,   118,    -1,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    -1,    89,    90,    91,    92,   157,   158,    95,    -1,
      -1,   162,    -1,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,   108,   109,    32,    -1,    -1,    -1,    -1,    -1,   180,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    58,
      59,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   165,   166,
     167,    -1,   169,    -1,    -1,   172,   173,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,   182,    46,   184,   185,    49,
     187,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    -1,    -1,   157,   158,
      -1,    -1,    -1,   162,    -1,   164,   165,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     189,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   165,   166,   167,    -1,   169,
      -1,    -1,   172,   173,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   182,   183,   184,   185,    26,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   163,    -1,   165,   166,   167,    -1,   169,
      -1,    -1,   172,   173,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   182,    -1,   184,   185,    26,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   165,   166,   167,    -1,   169,
      -1,    -1,   172,   173,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   182,   183,   184,   185,    26,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   165,   166,   167,    -1,   169,
      -1,    -1,   172,   173,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   182,   183,   184,   185,    26,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   165,   166,   167,    -1,   169,
      -1,    -1,   172,   173,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   182,   183,   184,   185,    26,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   165,   166,   167,    -1,   169,
      -1,    -1,   172,   173,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   182,   183,   184,   185,    26,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    -1,   165,   166,   167,    -1,   169,
      -1,    -1,   172,   173,    -1,    38,    -1,    -1,    -1,    -1,
      43,    -1,   182,    46,   184,   185,    49,   187,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    65,    66,    -1,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      -1,    84,    85,    86,    -1,    -1,    89,    90,    91,    92,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,   109,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   165,   166,   167,    -1,   169,    -1,    -1,   172,
     173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,
      -1,   184,   185,    -1,   187,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,    20,
      21,    -1,   125,   126,   127,   128,   129,   130,    -1,    -1,
     133,   134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,   158,    -1,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   179,   180,    -1,    -1,
      -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,    20,    21,    -1,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,   137,   138,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,   158,    -1,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   179,   180,
      -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,    20,    21,    -1,   125,   126,   127,   128,
     129,   130,    -1,    -1,   133,   134,   135,   136,   137,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,   158,
      -1,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,   180,    -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,    -1,    -1,    -1,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
     137,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,   158,    -1,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   179,   180,   106,   107,   108,   109,   110,   186,
      -1,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,
      -1,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,   179,   180,   106,
     107,   108,   109,   110,   186,    -1,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,   179,   180,    -1,    -1,   183,   106,   107,   108,
     109,   110,    -1,    -1,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
     179,   180,    -1,    -1,   183,   106,   107,   108,   109,   110,
      -1,    -1,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,   179,   180,
      -1,    -1,   183,   106,   107,   108,   109,   110,    -1,    -1,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,   179,   180,    -1,    -1,
     183,   106,   107,   108,   109,   110,    -1,    -1,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,   179,   180,    -1,    -1,   183,   106,
     107,   108,   109,   110,    -1,    -1,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,    -1,   179,   180,    -1,    -1,   183,   106,   107,   108,
     109,   110,    -1,    -1,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
     179,   180,    -1,    -1,   183,   106,   107,   108,   109,   110,
      -1,    -1,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,    -1,   179,   180,
     181,   106,   107,   108,   109,   110,    -1,    -1,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,   179,   180,   181,   106,   107,   108,
     109,   110,    -1,    -1,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,
     179,   180,   181,   106,   107,   108,   109,   110,    -1,    -1,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   179,   180,   181,   106,
     107,   108,   109,   110,    20,    21,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   179,   180,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,    20,    21,   125,
     126,   127,   128,   129,   130,    -1,    -1,   133,   134,   135,
     136,   137,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,   158,    -1,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   179,   180,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,    -1,
      -1,    -1,   125,   126,   127,   128,   129,   130,    -1,    -1,
     133,   134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,   158,    -1,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,    20,    21,    -1,
      -1,   106,   107,   108,   109,   110,   179,   180,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,   131,    -1,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   179,   180,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,   110,    -1,    -1,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,    -1,    -1,    -1,
      -1,   106,   107,   108,   109,   110,   179,   180,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,
      -1,    20,    21,   158,    -1,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,    -1,    -1,    -1,    -1,   106,
     107,   108,   109,   110,   179,   180,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,   158,    -1,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,   110,   179,   180,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,   133,    -1,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,    -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,
     179,   180,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,   127,   128,    -1,    -1,
      -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,   110,   179,   180,
     113,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      -1,    -1,   125,    -1,   127,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,    -1,    -1,    -1,
      -1,   106,   107,   108,   109,   110,   179,   180,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,    -1,
     125,    -1,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    -1,   162,   163,   164,
     165,   166,   167,   168,   169,    -1,    -1,    -1,    -1,   106,
     107,    -1,    -1,   110,   179,   180,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    20,    21,    -1,   125,    -1,
     127,   128,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,   160,   161,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,   110,   179,   180,   113,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,   128,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,   107,   108,   109,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,   118,   119,   120,   121,    37,    -1,    -1,
     125,    -1,   127,   128,   163,   164,   165,   166,   167,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   179,   180,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,    -1,    -1,    -1,   119,
     120,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
     130,    -1,    -1,   133,   134,   135,   136,   137,   138,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
      -1,    -1,   172,   173,    47,    -1,    -1,    -1,    -1,   179,
     180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      -1,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,
      -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,    62,    -1,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   148,    62,    -1,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,    -1,    47,    -1,    -1,    -1,    -1,
     139,   140,   141,    -1,    -1,    -1,    -1,    -1,    -1,   148,
      62,    -1,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   148,    62,    -1,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   191,     0,     6,    29,    31,    33,    39,    48,    54,
      73,    94,   180,   186,   194,   202,   203,   207,   230,   234,
     250,   307,   312,   314,   320,   356,   358,    18,    19,   148,
     224,   225,   226,   142,   208,   209,   148,   169,   204,   205,
     148,   187,   310,   148,   184,   193,   359,   357,    33,    61,
     132,   148,   227,   228,   229,   242,     4,     5,     7,    35,
     318,    60,   305,   157,   156,   159,   156,   204,    21,    55,
     168,   179,   206,   157,   310,   311,   305,   148,   148,   148,
     132,   182,   156,   181,    55,    61,   235,   237,    55,    61,
     313,    55,    61,   319,    55,    61,   306,    14,    15,   142,
     146,   148,   149,   182,   196,   225,   142,   209,   148,   148,
     148,    24,    30,    47,    62,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,   242,   323,   325,   326,
     328,   331,   186,   188,   310,    55,    61,   192,   187,   187,
     148,   226,   228,    34,   108,   109,   148,   233,   236,   148,
      97,   316,   317,   187,   308,   150,   151,   195,    14,    15,
     142,   146,   148,   196,   222,   223,   206,   163,   163,   163,
     163,   182,   163,   163,   163,   163,   163,   163,    32,    58,
      59,   118,   157,   158,   162,   165,   180,   189,   186,   148,
     327,   148,   297,   298,   183,    37,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   119,   120,   126,   127,   128,   129,
     130,   133,   134,   135,   136,   137,   138,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   172,   173,   179,
     180,    34,    34,   182,   231,   187,   238,   159,   187,   148,
     321,   304,   139,   286,   150,   151,   152,   156,   183,   334,
     336,   338,   332,   148,   329,   340,   342,   344,   346,   348,
     350,   352,   354,    32,    32,   162,   180,   189,    14,    15,
      16,    17,    18,    26,    38,    43,    46,    49,    53,    63,
      73,    95,   108,   109,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   149,   165,   166,   167,   169,   172,   173,
     181,   182,   184,   185,   198,   200,   238,   242,   244,   252,
     259,   262,   265,   269,   270,   273,   274,   275,   280,   283,
     323,   360,   364,   369,   371,   373,   186,   159,   186,   181,
     181,   183,   286,   293,   294,   159,   232,   240,    68,    72,
      89,    90,    91,    92,   324,   309,   159,   315,   187,   188,
     286,   226,   148,   303,   322,   222,   331,   331,   331,   331,
     183,   327,   331,   231,   331,   231,   331,   231,   148,   295,
     296,   331,   298,   181,   331,   360,   182,   163,   182,   163,
     163,   182,   163,   182,   163,   283,   283,    12,   331,    12,
     331,   283,   366,   370,   197,   283,   283,   283,   242,   283,
     283,   283,   185,   148,   182,   230,    20,    21,   106,   107,
     108,   109,   110,   113,   114,   115,   116,   118,   119,   120,
     121,   125,   127,   128,   133,   134,   135,   158,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   179,   180,
     181,   182,   148,   188,   331,   188,   297,    48,   300,   322,
     183,   186,   331,     1,     8,     9,    10,    12,    25,    27,
      28,    37,    40,    42,    50,    52,    56,    57,    63,    96,
     123,   124,   186,   188,   210,   211,   214,   215,   216,   217,
     218,   219,   220,   239,   241,   243,   245,   246,   247,   248,
     249,   250,   251,   271,   272,   283,   187,   148,   188,   242,
     291,   303,   181,    41,   156,   159,   162,   302,   164,   186,
     164,   164,   164,   186,   164,   232,   164,   232,   164,   232,
     159,   164,   186,   164,   186,   182,   242,   260,   283,   253,
     255,   283,   257,   283,   331,   322,    51,   148,   165,   182,
     283,   361,   362,   363,   365,   366,   367,   368,   322,   182,
     362,   368,   131,   186,   188,   152,   153,   195,   201,   183,
     163,   242,   180,   183,   263,   283,   139,   268,    18,   148,
     148,   283,   283,   283,   283,   283,   283,   148,   283,   148,
     283,   283,   283,   283,   283,   283,   283,   283,    21,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     119,   120,   148,   179,   180,   281,   283,   183,   263,   300,
     117,   157,   159,   162,   299,   293,   283,   322,   117,   186,
     241,   271,   283,   238,   283,   283,   148,   186,   142,    56,
     283,   238,   117,   241,   283,   185,   269,   269,    36,   186,
     186,   283,   186,   186,   186,   303,   122,   186,    98,    99,
     100,   101,   102,   103,   104,   105,   111,   112,   117,   126,
     129,   130,   136,   137,   138,   157,   309,   157,   186,   180,
     188,   230,   286,   290,   181,   148,   148,   331,   117,   126,
     157,   301,   335,   331,   339,   333,   330,   341,   164,   345,
     164,   349,   164,   331,   353,   295,   355,   183,   263,   163,
     283,   331,   183,   331,   331,   183,   331,   183,   164,    19,
     269,   126,   299,   183,   156,   186,   363,   181,   156,   181,
     186,    19,   183,   363,   188,   283,   366,   188,   283,   150,
     199,   276,   278,   148,   361,   156,   183,   117,   126,   157,
     162,   266,   267,   231,   163,   182,   181,   148,   159,   148,
     283,   148,   283,     1,   181,   183,   331,   241,   283,   238,
      19,   241,   283,   117,   157,   186,    13,   238,   157,   159,
     142,   241,   283,   186,   185,   187,   238,   269,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   123,   124,
     283,   283,   283,   283,   283,   283,   283,   123,   124,   283,
     188,   283,   229,     7,    44,    97,   287,   186,    41,   157,
     186,   301,   241,   283,   164,   343,   347,   351,   183,   148,
     183,   164,   164,   164,   164,   268,   263,   283,   283,   362,
     363,   148,   361,   181,   181,   283,   181,   367,   263,   362,
     188,   181,   154,   150,   331,   231,   181,   283,   148,   148,
     148,   148,   156,   181,   232,   284,   183,   263,   283,   181,
     181,   282,   157,   299,   263,   186,   238,   221,   186,   240,
      11,    22,    23,   212,   213,   283,   283,   283,   283,   186,
     181,    55,    61,   289,   289,   148,   241,   283,   186,   337,
     164,   186,   261,   254,   256,   258,   182,   186,   363,   181,
     126,   299,   181,   186,   363,   181,   164,   232,   183,   266,
     181,   131,   238,   264,   331,   183,   283,   186,   283,   188,
     238,   283,   186,    45,   287,   300,   186,   283,   148,   283,
     283,   283,   183,   283,   283,   181,   181,   283,   283,   283,
     188,   277,   164,   117,   283,   164,   238,   238,   292,    32,
     288,   183,   164,   183,   186,   372,   181,   372,   181,   242,
     279,   283,   285,   213,   236,   236,   283,    51,   181,   188,
     242,   186,   238,   183,   283,   181,   181
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int16 yyr1[] =
{
       0,   190,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   192,   192,   192,   193,   193,
     194,   195,   195,   195,   195,   196,   197,   197,   197,   198,
     199,   199,   201,   200,   202,   203,   204,   204,   204,   204,
     205,   205,   206,   206,   207,   208,   208,   209,   209,   210,
     211,   211,   212,   212,   213,   213,   213,   214,   214,   215,
     216,   217,   218,   219,   221,   220,   222,   222,   222,   222,
     222,   222,   223,   223,   224,   224,   224,   225,   225,   225,
     225,   225,   225,   225,   225,   226,   226,   227,   227,   227,
     228,   228,   229,   229,   230,   230,   231,   231,   231,   232,
     232,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     234,   235,   235,   235,   236,   237,   238,   238,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   240,   240,
     240,   241,   241,   241,   242,   242,   242,   243,   244,   244,
     244,   244,   245,   246,   247,   247,   247,   247,   247,   248,
     248,   248,   248,   249,   250,   250,   251,   253,   254,   252,
     255,   256,   252,   257,   258,   252,   260,   261,   259,   262,
     262,   262,   263,   263,   264,   264,   264,   265,   265,   265,
     266,   266,   266,   266,   267,   267,   268,   268,   269,   269,
     270,   270,   270,   270,   270,   270,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   272,   272,   272,   272,   273,
     274,   274,   275,   276,   277,   275,   278,   279,   275,   280,
     280,   281,   282,   280,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   284,   285,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   286,   286,
     287,   287,   287,   288,   288,   289,   289,   289,   290,   291,
     291,   292,   291,   291,   291,   293,   293,   294,   294,   295,
     295,   296,   296,   297,   298,   298,   299,   299,   300,   300,
     300,   300,   300,   300,   301,   301,   301,   302,   302,   303,
     303,   303,   303,   303,   304,   304,   305,   305,   306,   306,
     306,   307,   308,   307,   309,   309,   309,   310,   311,   311,
     312,   312,   313,   313,   313,   314,   314,   315,   315,   316,
     316,   317,   318,   318,   319,   319,   319,   321,   320,   322,
     322,   322,   322,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   324,   324,
     324,   324,   324,   324,   325,   326,   326,   327,   327,   329,
     330,   328,   331,   331,   331,   331,   331,   331,   331,   331,
     331,   331,   331,   331,   331,   331,   331,   331,   331,   332,
     333,   331,   331,   334,   335,   331,   336,   337,   331,   338,
     339,   331,   331,   340,   341,   331,   342,   343,   331,   331,
     344,   345,   331,   346,   347,   331,   331,   348,   349,   331,
     350,   351,   331,   352,   353,   331,   354,   355,   331,   357,
     356,   359,   358,   360,   360,   360,   360,   361,   361,   361,
     361,   362,   362,   363,   363,   364,   364,   364,   364,   364,
     364,   365,   365,   366,   367,   367,   368,   368,   369,   369,
     370,   370,   371,   372,   372,   373,   373
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     0,     1,     1,     1,     1,
       4,     1,     1,     2,     2,     3,     0,     2,     4,     3,
       1,     2,     0,     4,     2,     2,     1,     2,     3,     3,
       2,     4,     0,     1,     2,     1,     3,     1,     3,     3,
       3,     2,     1,     1,     0,     2,     4,     1,     1,     4,
       6,     2,     3,     3,     0,     5,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     1,     5,     1,     3,     1,     1,     1,
       1,     4,     1,     3,     0,     3,     0,     2,     3,     0,
       2,     1,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     3,     3,     2,     2,     2,     2,
       3,     0,     1,     1,     3,     3,     3,     7,     1,     1,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     1,     2,     2,     2,     0,     2,
       2,     3,     2,     2,     1,     3,     2,     2,     2,     4,
       5,     2,     1,     1,     2,     3,     4,     2,     3,     3,
       4,     2,     3,     4,     1,     1,     2,     0,     0,     7,
       0,     0,     7,     0,     0,     7,     0,     0,     6,     5,
       8,    10,     1,     3,     1,     2,     3,     1,     1,     2,
       2,     2,     2,     2,     1,     3,     0,     4,     1,     6,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     4,     4,     4,     4,     6,
       5,     6,     3,     0,     0,     8,     0,     0,     9,     3,
       4,     0,     0,     5,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     2,     3,     4,     5,     4,     5,     3,     4,     1,
       3,     4,     3,     4,     2,     4,     4,     7,     8,     3,
       5,     0,     0,     8,     3,     3,     4,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     2,     4,     0,     4,
       0,     1,     1,     0,     1,     0,     1,     1,     4,     0,
       3,     0,     8,     8,     5,     2,     3,     1,     3,     1,
       3,     1,     3,     3,     1,     3,     1,     1,     1,     2,
       3,     5,     3,     3,     1,     1,     1,     0,     1,     4,
       6,     5,     5,     4,     0,     3,     0,     1,     0,     1,
       1,     6,     0,     6,     0,     3,     5,     3,     2,     3,
       4,     2,     0,     1,     1,     7,     9,     0,     2,     0,
       1,     3,     1,     1,     0,     1,     1,     0,     8,     1,
       3,     3,     5,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     1,     3,     0,
       0,     6,     1,     1,     1,     1,     4,     3,     4,     2,
       2,     3,     2,     3,     2,     2,     3,     3,     2,     0,
       0,     6,     2,     0,     0,     6,     0,     0,     8,     0,
       0,     6,     1,     0,     0,     6,     0,     0,     7,     1,
       0,     0,     6,     0,     0,     7,     1,     0,     0,     6,
       0,     0,     7,     0,     0,     6,     0,     0,     6,     0,
       7,     0,     7,     1,     1,     1,     1,     3,     3,     5,
       5,     1,     3,     0,     2,     6,     5,     7,     8,     6,
       8,     1,     3,     3,     1,     1,     1,     3,     5,     5,
       1,     3,     4,     0,     3,    10,    10
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = DAS_YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


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


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YY_LOCATION_PRINT
#  if defined DAS_YYLTYPE_IS_TRIVIAL && DAS_YYLTYPE_IS_TRIVIAL

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

#   define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

#  else
#   define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#  endif
# endif /* !defined YY_LOCATION_PRINT */


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
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
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

  YY_LOCATION_PRINT (yyo, *yylocationp);
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
#line 146 "ds_parser.ypp"
            { delete ((*yyvaluep).s); }
#line 3702 "ds_parser.cpp"
        break;

    case YYSYMBOL_module_name: /* module_name  */
#line 146 "ds_parser.ypp"
            { delete ((*yyvaluep).s); }
#line 3708 "ds_parser.cpp"
        break;

    case YYSYMBOL_character_sequence: /* character_sequence  */
#line 146 "ds_parser.ypp"
            { delete ((*yyvaluep).s); }
#line 3714 "ds_parser.cpp"
        break;

    case YYSYMBOL_string_constant: /* string_constant  */
#line 146 "ds_parser.ypp"
            { delete ((*yyvaluep).s); }
#line 3720 "ds_parser.cpp"
        break;

    case YYSYMBOL_string_builder_body: /* string_builder_body  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3726 "ds_parser.cpp"
        break;

    case YYSYMBOL_string_builder: /* string_builder  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3732 "ds_parser.cpp"
        break;

    case YYSYMBOL_expr_reader: /* expr_reader  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3738 "ds_parser.cpp"
        break;

    case YYSYMBOL_require_module_name: /* require_module_name  */
#line 146 "ds_parser.ypp"
            { delete ((*yyvaluep).s); }
#line 3744 "ds_parser.cpp"
        break;

    case YYSYMBOL_expression_label: /* expression_label  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3750 "ds_parser.cpp"
        break;

    case YYSYMBOL_expression_goto: /* expression_goto  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3756 "ds_parser.cpp"
        break;

    case YYSYMBOL_expression_else: /* expression_else  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3762 "ds_parser.cpp"
        break;

    case YYSYMBOL_expression_if_then_else: /* expression_if_then_else  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3768 "ds_parser.cpp"
        break;

    case YYSYMBOL_expression_for_loop: /* expression_for_loop  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3774 "ds_parser.cpp"
        break;

    case YYSYMBOL_expression_unsafe: /* expression_unsafe  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3780 "ds_parser.cpp"
        break;

    case YYSYMBOL_expression_while_loop: /* expression_while_loop  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3786 "ds_parser.cpp"
        break;

    case YYSYMBOL_expression_with: /* expression_with  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3792 "ds_parser.cpp"
        break;

    case YYSYMBOL_expression_with_alias: /* expression_with_alias  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3798 "ds_parser.cpp"
        break;

    case YYSYMBOL_annotation_argument_value: /* annotation_argument_value  */
#line 153 "ds_parser.ypp"
            { delete ((*yyvaluep).aa); }
#line 3804 "ds_parser.cpp"
        break;

    case YYSYMBOL_annotation_argument_value_list: /* annotation_argument_value_list  */
#line 154 "ds_parser.ypp"
            { delete ((*yyvaluep).aaList); }
#line 3810 "ds_parser.cpp"
        break;

    case YYSYMBOL_annotation_argument_name: /* annotation_argument_name  */
#line 146 "ds_parser.ypp"
            { delete ((*yyvaluep).s); }
#line 3816 "ds_parser.cpp"
        break;

    case YYSYMBOL_annotation_argument: /* annotation_argument  */
#line 153 "ds_parser.ypp"
            { delete ((*yyvaluep).aa); }
#line 3822 "ds_parser.cpp"
        break;

    case YYSYMBOL_annotation_argument_list: /* annotation_argument_list  */
#line 154 "ds_parser.ypp"
            { delete ((*yyvaluep).aaList); }
#line 3828 "ds_parser.cpp"
        break;

    case YYSYMBOL_annotation_declaration_name: /* annotation_declaration_name  */
#line 146 "ds_parser.ypp"
            { delete ((*yyvaluep).s); }
#line 3834 "ds_parser.cpp"
        break;

    case YYSYMBOL_annotation_declaration: /* annotation_declaration  */
#line 155 "ds_parser.ypp"
            { delete ((*yyvaluep).fa); }
#line 3840 "ds_parser.cpp"
        break;

    case YYSYMBOL_annotation_list: /* annotation_list  */
#line 156 "ds_parser.ypp"
            { delete ((*yyvaluep).faList); }
#line 3846 "ds_parser.cpp"
        break;

    case YYSYMBOL_optional_annotation_list: /* optional_annotation_list  */
#line 156 "ds_parser.ypp"
            { delete ((*yyvaluep).faList); }
#line 3852 "ds_parser.cpp"
        break;

    case YYSYMBOL_optional_function_argument_list: /* optional_function_argument_list  */
#line 150 "ds_parser.ypp"
            { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 3858 "ds_parser.cpp"
        break;

    case YYSYMBOL_optional_function_type: /* optional_function_type  */
#line 151 "ds_parser.ypp"
            { delete ((*yyvaluep).pTypeDecl); }
#line 3864 "ds_parser.cpp"
        break;

    case YYSYMBOL_function_name: /* function_name  */
#line 146 "ds_parser.ypp"
            { delete ((*yyvaluep).s); }
#line 3870 "ds_parser.cpp"
        break;

    case YYSYMBOL_expression_block: /* expression_block  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3876 "ds_parser.cpp"
        break;

    case YYSYMBOL_expression_any: /* expression_any  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3882 "ds_parser.cpp"
        break;

    case YYSYMBOL_expressions: /* expressions  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3888 "ds_parser.cpp"
        break;

    case YYSYMBOL_expr_pipe: /* expr_pipe  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3894 "ds_parser.cpp"
        break;

    case YYSYMBOL_name_in_namespace: /* name_in_namespace  */
#line 146 "ds_parser.ypp"
            { delete ((*yyvaluep).s); }
#line 3900 "ds_parser.cpp"
        break;

    case YYSYMBOL_expression_delete: /* expression_delete  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3906 "ds_parser.cpp"
        break;

    case YYSYMBOL_expr_new: /* expr_new  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3912 "ds_parser.cpp"
        break;

    case YYSYMBOL_expression_break: /* expression_break  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3918 "ds_parser.cpp"
        break;

    case YYSYMBOL_expression_continue: /* expression_continue  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3924 "ds_parser.cpp"
        break;

    case YYSYMBOL_expression_return: /* expression_return  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3930 "ds_parser.cpp"
        break;

    case YYSYMBOL_expression_yield: /* expression_yield  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3936 "ds_parser.cpp"
        break;

    case YYSYMBOL_expression_try_catch: /* expression_try_catch  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3942 "ds_parser.cpp"
        break;

    case YYSYMBOL_expression_let: /* expression_let  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3948 "ds_parser.cpp"
        break;

    case YYSYMBOL_expr_cast: /* expr_cast  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3954 "ds_parser.cpp"
        break;

    case YYSYMBOL_expr_type_decl: /* expr_type_decl  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3960 "ds_parser.cpp"
        break;

    case YYSYMBOL_expr_type_info: /* expr_type_info  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3966 "ds_parser.cpp"
        break;

    case YYSYMBOL_expr_list: /* expr_list  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3972 "ds_parser.cpp"
        break;

    case YYSYMBOL_block_or_simple_block: /* block_or_simple_block  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 3978 "ds_parser.cpp"
        break;

    case YYSYMBOL_capture_entry: /* capture_entry  */
#line 159 "ds_parser.ypp"
            { delete ((*yyvaluep).pCapt); }
#line 3984 "ds_parser.cpp"
        break;

    case YYSYMBOL_capture_list: /* capture_list  */
#line 160 "ds_parser.ypp"
            { delete ((*yyvaluep).pCaptList); }
#line 3990 "ds_parser.cpp"
        break;

    case YYSYMBOL_optional_capture_list: /* optional_capture_list  */
#line 160 "ds_parser.ypp"
            { delete ((*yyvaluep).pCaptList); }
#line 3996 "ds_parser.cpp"
        break;

    case YYSYMBOL_expr_block: /* expr_block  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 4002 "ds_parser.cpp"
        break;

    case YYSYMBOL_expr_numeric_const: /* expr_numeric_const  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 4008 "ds_parser.cpp"
        break;

    case YYSYMBOL_expr_assign: /* expr_assign  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 4014 "ds_parser.cpp"
        break;

    case YYSYMBOL_expr_assign_pipe: /* expr_assign_pipe  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 4020 "ds_parser.cpp"
        break;

    case YYSYMBOL_expr_named_call: /* expr_named_call  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 4026 "ds_parser.cpp"
        break;

    case YYSYMBOL_expr_method_call: /* expr_method_call  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 4032 "ds_parser.cpp"
        break;

    case YYSYMBOL_func_addr_expr: /* func_addr_expr  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 4038 "ds_parser.cpp"
        break;

    case YYSYMBOL_expr_field: /* expr_field  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 4044 "ds_parser.cpp"
        break;

    case YYSYMBOL_expr: /* expr  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 4050 "ds_parser.cpp"
        break;

    case YYSYMBOL_optional_field_annotation: /* optional_field_annotation  */
#line 154 "ds_parser.ypp"
            { delete ((*yyvaluep).aaList); }
#line 4056 "ds_parser.cpp"
        break;

    case YYSYMBOL_structure_variable_declaration: /* structure_variable_declaration  */
#line 149 "ds_parser.ypp"
            { delete ((*yyvaluep).pVarDecl); }
#line 4062 "ds_parser.cpp"
        break;

    case YYSYMBOL_struct_variable_declaration_list: /* struct_variable_declaration_list  */
#line 150 "ds_parser.ypp"
            { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 4068 "ds_parser.cpp"
        break;

    case YYSYMBOL_function_argument_declaration: /* function_argument_declaration  */
#line 149 "ds_parser.ypp"
            { delete ((*yyvaluep).pVarDecl); }
#line 4074 "ds_parser.cpp"
        break;

    case YYSYMBOL_function_argument_list: /* function_argument_list  */
#line 150 "ds_parser.ypp"
            { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 4080 "ds_parser.cpp"
        break;

    case YYSYMBOL_tuple_type: /* tuple_type  */
#line 149 "ds_parser.ypp"
            { delete ((*yyvaluep).pVarDecl); }
#line 4086 "ds_parser.cpp"
        break;

    case YYSYMBOL_tuple_type_list: /* tuple_type_list  */
#line 150 "ds_parser.ypp"
            { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 4092 "ds_parser.cpp"
        break;

    case YYSYMBOL_variant_type: /* variant_type  */
#line 149 "ds_parser.ypp"
            { delete ((*yyvaluep).pVarDecl); }
#line 4098 "ds_parser.cpp"
        break;

    case YYSYMBOL_variant_type_list: /* variant_type_list  */
#line 150 "ds_parser.ypp"
            { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 4104 "ds_parser.cpp"
        break;

    case YYSYMBOL_variable_declaration: /* variable_declaration  */
#line 149 "ds_parser.ypp"
            { delete ((*yyvaluep).pVarDecl); }
#line 4110 "ds_parser.cpp"
        break;

    case YYSYMBOL_let_variable_declaration: /* let_variable_declaration  */
#line 149 "ds_parser.ypp"
            { delete ((*yyvaluep).pVarDecl); }
#line 4116 "ds_parser.cpp"
        break;

    case YYSYMBOL_global_variable_declaration_list: /* global_variable_declaration_list  */
#line 150 "ds_parser.ypp"
            { deleteVariableDeclarationList(((*yyvaluep).pVarDeclList)); }
#line 4122 "ds_parser.cpp"
        break;

    case YYSYMBOL_enum_list: /* enum_list  */
#line 158 "ds_parser.ypp"
            { delete ((*yyvaluep).pEnum); }
#line 4128 "ds_parser.cpp"
        break;

    case YYSYMBOL_optional_structure_parent: /* optional_structure_parent  */
#line 146 "ds_parser.ypp"
            { delete ((*yyvaluep).s); }
#line 4134 "ds_parser.cpp"
        break;

    case YYSYMBOL_variable_name_with_pos_list: /* variable_name_with_pos_list  */
#line 148 "ds_parser.ypp"
            { delete ((*yyvaluep).pNameWithPosList); }
#line 4140 "ds_parser.cpp"
        break;

    case YYSYMBOL_structure_type_declaration: /* structure_type_declaration  */
#line 151 "ds_parser.ypp"
            { delete ((*yyvaluep).pTypeDecl); }
#line 4146 "ds_parser.cpp"
        break;

    case YYSYMBOL_auto_type_declaration: /* auto_type_declaration  */
#line 151 "ds_parser.ypp"
            { delete ((*yyvaluep).pTypeDecl); }
#line 4152 "ds_parser.cpp"
        break;

    case YYSYMBOL_bitfield_bits: /* bitfield_bits  */
#line 147 "ds_parser.ypp"
            { delete ((*yyvaluep).pNameList); }
#line 4158 "ds_parser.cpp"
        break;

    case YYSYMBOL_bitfield_type_declaration: /* bitfield_type_declaration  */
#line 151 "ds_parser.ypp"
            { delete ((*yyvaluep).pTypeDecl); }
#line 4164 "ds_parser.cpp"
        break;

    case YYSYMBOL_type_declaration: /* type_declaration  */
#line 151 "ds_parser.ypp"
            { delete ((*yyvaluep).pTypeDecl); }
#line 4170 "ds_parser.cpp"
        break;

    case YYSYMBOL_make_decl: /* make_decl  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 4176 "ds_parser.cpp"
        break;

    case YYSYMBOL_make_struct_fields: /* make_struct_fields  */
#line 157 "ds_parser.ypp"
            { delete ((*yyvaluep).pMakeStruct); }
#line 4182 "ds_parser.cpp"
        break;

    case YYSYMBOL_make_struct_dim: /* make_struct_dim  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 4188 "ds_parser.cpp"
        break;

    case YYSYMBOL_optional_block: /* optional_block  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 4194 "ds_parser.cpp"
        break;

    case YYSYMBOL_make_struct_decl: /* make_struct_decl  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 4200 "ds_parser.cpp"
        break;

    case YYSYMBOL_make_tuple: /* make_tuple  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 4206 "ds_parser.cpp"
        break;

    case YYSYMBOL_make_map_tuple: /* make_map_tuple  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 4212 "ds_parser.cpp"
        break;

    case YYSYMBOL_make_any_tuple: /* make_any_tuple  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 4218 "ds_parser.cpp"
        break;

    case YYSYMBOL_make_dim: /* make_dim  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 4224 "ds_parser.cpp"
        break;

    case YYSYMBOL_make_dim_decl: /* make_dim_decl  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 4230 "ds_parser.cpp"
        break;

    case YYSYMBOL_make_table: /* make_table  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 4236 "ds_parser.cpp"
        break;

    case YYSYMBOL_make_table_decl: /* make_table_decl  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 4242 "ds_parser.cpp"
        break;

    case YYSYMBOL_array_comprehension_where: /* array_comprehension_where  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 4248 "ds_parser.cpp"
        break;

    case YYSYMBOL_array_comprehension: /* array_comprehension  */
#line 152 "ds_parser.ypp"
            { delete ((*yyvaluep).pExpression); }
#line 4254 "ds_parser.cpp"
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
    goto yyexhaustedlab;
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
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
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
#line 499 "ds_parser.ypp"
                        { (yyval.b) = yyextra->g_Program->policies.default_module_public; }
#line 4558 "ds_parser.cpp"
    break;

  case 16: /* optional_public_or_private_module: "public"  */
#line 500 "ds_parser.ypp"
                        { (yyval.b) = true; }
#line 4564 "ds_parser.cpp"
    break;

  case 17: /* optional_public_or_private_module: "private"  */
#line 501 "ds_parser.ypp"
                        { (yyval.b) = false; }
#line 4570 "ds_parser.cpp"
    break;

  case 18: /* module_name: '$'  */
#line 505 "ds_parser.ypp"
                    { (yyval.s) = new string("$"); }
#line 4576 "ds_parser.cpp"
    break;

  case 19: /* module_name: "name"  */
#line 506 "ds_parser.ypp"
                    { (yyval.s) = (yyvsp[0].s); }
#line 4582 "ds_parser.cpp"
    break;

  case 20: /* module_declaration: "module" module_name optional_shared optional_public_or_private_module  */
#line 510 "ds_parser.ypp"
                                                                                                      {
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
#line 4601 "ds_parser.cpp"
    break;

  case 21: /* character_sequence: STRING_CHARACTER  */
#line 527 "ds_parser.ypp"
                                                            { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
#line 4607 "ds_parser.cpp"
    break;

  case 22: /* character_sequence: STRING_CHARACTER_ESC  */
#line 528 "ds_parser.ypp"
                                                            { (yyval.s) = new string(); *(yyval.s) += "\\\\"; }
#line 4613 "ds_parser.cpp"
    break;

  case 23: /* character_sequence: character_sequence STRING_CHARACTER  */
#line 529 "ds_parser.ypp"
                                                            { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
#line 4619 "ds_parser.cpp"
    break;

  case 24: /* character_sequence: character_sequence STRING_CHARACTER_ESC  */
#line 530 "ds_parser.ypp"
                                                            { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += "\\\\"; }
#line 4625 "ds_parser.cpp"
    break;

  case 25: /* string_constant: "start of the string" character_sequence "end of the string"  */
#line 534 "ds_parser.ypp"
                                                           { (yyval.s) = (yyvsp[-1].s); }
#line 4631 "ds_parser.cpp"
    break;

  case 26: /* string_builder_body: %empty  */
#line 538 "ds_parser.ypp"
        {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 4641 "ds_parser.cpp"
    break;

  case 27: /* string_builder_body: string_builder_body character_sequence  */
#line 543 "ds_parser.ypp"
                                                           {
        bool err;
        auto esconst = unescapeString(*(yyvsp[0].s),&err);
        if ( err ) das_yyerror(scanner,"invalid escape sequence",tokAt(scanner,(yylsp[-1])), CompilationError::invalid_escape_sequence);
        auto sc = make_smart<ExprConstString>(tokAt(scanner,(yylsp[0])),esconst);
        delete (yyvsp[0].s);
        static_cast<ExprStringBuilder *>((yyvsp[-1].pExpression))->elements.push_back(sc);
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 4655 "ds_parser.cpp"
    break;

  case 28: /* string_builder_body: string_builder_body "{" expr "}"  */
#line 552 "ds_parser.ypp"
                                                                                {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 4665 "ds_parser.cpp"
    break;

  case 29: /* string_builder: "start of the string" string_builder_body "end of the string"  */
#line 560 "ds_parser.ypp"
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
#line 4683 "ds_parser.cpp"
    break;

  case 30: /* reader_character_sequence: STRING_CHARACTER  */
#line 576 "ds_parser.ypp"
                               {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
#line 4693 "ds_parser.cpp"
    break;

  case 31: /* reader_character_sequence: reader_character_sequence STRING_CHARACTER  */
#line 581 "ds_parser.ypp"
                                                                {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
#line 4703 "ds_parser.cpp"
    break;

  case 32: /* $@1: %empty  */
#line 589 "ds_parser.ypp"
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
#line 4730 "ds_parser.cpp"
    break;

  case 33: /* expr_reader: '%' name_in_namespace $@1 reader_character_sequence  */
#line 610 "ds_parser.ypp"
                                     {
        yyextra->g_ReaderExpr->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = yyextra->g_ReaderExpr;
        delete (yyvsp[-2].s);
        yyextra->g_ReaderMacro = nullptr;
        yyextra->g_ReaderExpr = nullptr;
    }
#line 4742 "ds_parser.cpp"
    break;

  case 34: /* options_declaration: "options" annotation_argument_list  */
#line 619 "ds_parser.ypp"
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
#line 4765 "ds_parser.cpp"
    break;

  case 36: /* require_module_name: "name"  */
#line 644 "ds_parser.ypp"
                   {
        (yyval.s) = (yyvsp[0].s);
    }
#line 4773 "ds_parser.cpp"
    break;

  case 37: /* require_module_name: '%' require_module_name  */
#line 647 "ds_parser.ypp"
                                     {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
#line 4782 "ds_parser.cpp"
    break;

  case 38: /* require_module_name: require_module_name '.' "name"  */
#line 651 "ds_parser.ypp"
                                                {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4793 "ds_parser.cpp"
    break;

  case 39: /* require_module_name: require_module_name '/' "name"  */
#line 657 "ds_parser.ypp"
                                                {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
#line 4804 "ds_parser.cpp"
    break;

  case 40: /* require_module: require_module_name is_public_module  */
#line 666 "ds_parser.ypp"
                                                         {
        auto info = yyextra->g_Access->getModuleInfo(*(yyvsp[-1].s), yyextra->g_FileAccessStack.back()->name);
        if ( auto mod = yyextra->g_Program->addModule(info.moduleName) ) {
            yyextra->g_Program->allRequireDecl.push_back(make_tuple(mod,*(yyvsp[-1].s),"",(yyvsp[0].b),tokAt(scanner,(yylsp[-1]))));
            yyextra->g_Program->thisModule->addDependency(mod, (yyvsp[0].b));
            if ( !info.importName.empty() ) {
                auto ita = yyextra->das_module_alias.find(info.importName);
                if ( ita !=yyextra->das_module_alias.end() ) {
                    if ( ita->second != info.moduleName ) {
                        das_yyerror(scanner,"module alias already used " + info.importName + " as " + ita->second,tokAt(scanner,(yylsp[-1])),
                            CompilationError::module_not_found);
                    }
                } else {
                    yyextra->das_module_alias[info.importName] = info.moduleName;
                }
            }
        } else {
            yyextra->g_Program->allRequireDecl.push_back(make_tuple((Module *)nullptr,*(yyvsp[-1].s),"",(yyvsp[0].b),tokAt(scanner,(yylsp[-1]))));
            das_yyerror(scanner,"required module not found " + *(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),
                CompilationError::module_not_found);
        }
        delete (yyvsp[-1].s);
    }
#line 4832 "ds_parser.cpp"
    break;

  case 41: /* require_module: require_module_name "as" "name" is_public_module  */
#line 689 "ds_parser.ypp"
                                                                              {
        auto info = yyextra->g_Access->getModuleInfo(*(yyvsp[-3].s), yyextra->g_FileAccessStack.back()->name);
        if ( auto mod = yyextra->g_Program->addModule(info.moduleName) ) {
            yyextra->g_Program->allRequireDecl.push_back(make_tuple(mod,*(yyvsp[-3].s),*(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3]))));
            yyextra->g_Program->thisModule->addDependency(mod, (yyvsp[0].b));
            auto ita = yyextra->das_module_alias.find(*(yyvsp[-1].s));
            if ( ita !=yyextra->das_module_alias.end() ) {
                if ( ita->second != info.moduleName ) {
                    das_yyerror(scanner,"module alias already used " + *(yyvsp[-1].s) + " as " + ita->second,tokAt(scanner,(yylsp[-3])),
                        CompilationError::module_not_found);
                }
            } else {
                yyextra->das_module_alias[*(yyvsp[-1].s)] = info.moduleName;
            }
        } else {
            yyextra->g_Program->allRequireDecl.push_back(make_tuple(mod,*(yyvsp[-3].s),*(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3]))));
            das_yyerror(scanner,"required module not found " + *(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])),
                CompilationError::module_not_found);
        }
        delete (yyvsp[-3].s);
        delete (yyvsp[-1].s);
    }
#line 4859 "ds_parser.cpp"
    break;

  case 42: /* is_public_module: %empty  */
#line 714 "ds_parser.ypp"
                    { (yyval.b) = false; }
#line 4865 "ds_parser.cpp"
    break;

  case 43: /* is_public_module: "public"  */
#line 715 "ds_parser.ypp"
                    { (yyval.b) = true; }
#line 4871 "ds_parser.cpp"
    break;

  case 47: /* expect_error: "integer constant"  */
#line 728 "ds_parser.ypp"
                   {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
#line 4879 "ds_parser.cpp"
    break;

  case 48: /* expect_error: "integer constant" ':' "integer constant"  */
#line 731 "ds_parser.ypp"
                                      {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
#line 4887 "ds_parser.cpp"
    break;

  case 49: /* expression_label: "label" "integer constant" ':'  */
#line 737 "ds_parser.ypp"
                                          {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }
#line 4895 "ds_parser.cpp"
    break;

  case 50: /* expression_goto: "goto" "label" "integer constant"  */
#line 743 "ds_parser.ypp"
                                                {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }
#line 4903 "ds_parser.cpp"
    break;

  case 51: /* expression_goto: "goto" expr  */
#line 746 "ds_parser.ypp"
                               {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 4911 "ds_parser.cpp"
    break;

  case 52: /* elif_or_static_elif: "elif"  */
#line 752 "ds_parser.ypp"
                          { (yyval.b) = false; }
#line 4917 "ds_parser.cpp"
    break;

  case 53: /* elif_or_static_elif: "static_elif"  */
#line 753 "ds_parser.ypp"
                          { (yyval.b) = true; }
#line 4923 "ds_parser.cpp"
    break;

  case 54: /* expression_else: %empty  */
#line 757 "ds_parser.ypp"
                                                           { (yyval.pExpression) = nullptr; }
#line 4929 "ds_parser.cpp"
    break;

  case 55: /* expression_else: "else" expression_block  */
#line 758 "ds_parser.ypp"
                                                           { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 4935 "ds_parser.cpp"
    break;

  case 56: /* expression_else: elif_or_static_elif expr expression_block expression_else  */
#line 759 "ds_parser.ypp"
                                                                                          {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4946 "ds_parser.cpp"
    break;

  case 57: /* if_or_static_if: "if"  */
#line 768 "ds_parser.ypp"
                        { (yyval.b) = false; }
#line 4952 "ds_parser.cpp"
    break;

  case 58: /* if_or_static_if: "static_if"  */
#line 769 "ds_parser.ypp"
                        { (yyval.b) = true; }
#line 4958 "ds_parser.cpp"
    break;

  case 59: /* expression_if_then_else: if_or_static_if expr expression_block expression_else  */
#line 773 "ds_parser.ypp"
                                                                                      {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
#line 4969 "ds_parser.cpp"
    break;

  case 60: /* expression_for_loop: "for" variable_name_with_pos_list "in" expr_list ';' expression_block  */
#line 782 "ds_parser.ypp"
                                                                                                           {
        auto pFor = new ExprFor(tokAt(scanner,(yylsp[-5])));
        pFor->visibility = tokAt(scanner,(yylsp[0]));
        for ( const auto & np : *(yyvsp[-4].pNameWithPosList) ) {
            pFor->iterators.push_back(np.name);
            pFor->iteratorsAka.push_back(np.aka);
            pFor->iteratorsAt.push_back(np.at);
        }
        delete (yyvsp[-4].pNameWithPosList);
        pFor->sources = sequenceToList((yyvsp[-2].pExpression));
        pFor->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pFor;
    }
#line 4988 "ds_parser.cpp"
    break;

  case 61: /* expression_unsafe: "unsafe" expression_block  */
#line 799 "ds_parser.ypp"
                                                 {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
#line 4998 "ds_parser.cpp"
    break;

  case 62: /* expression_while_loop: "while" expr expression_block  */
#line 807 "ds_parser.ypp"
                                                               {
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
#line 5010 "ds_parser.cpp"
    break;

  case 63: /* expression_with: "with" expr expression_block  */
#line 817 "ds_parser.ypp"
                                                         {
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
#line 5021 "ds_parser.cpp"
    break;

  case 64: /* $@2: %empty  */
#line 826 "ds_parser.ypp"
                                        { yyextra->das_need_oxford_comma=true; }
#line 5027 "ds_parser.cpp"
    break;

  case 65: /* expression_with_alias: "assume" "name" '=' $@2 expr  */
#line 826 "ds_parser.ypp"
                                                                                               {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-4])), *(yyvsp[-3].s), (yyvsp[0].pExpression) );
        delete (yyvsp[-3].s);
    }
#line 5036 "ds_parser.cpp"
    break;

  case 66: /* annotation_argument_value: string_constant  */
#line 833 "ds_parser.ypp"
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5042 "ds_parser.cpp"
    break;

  case 67: /* annotation_argument_value: "name"  */
#line 834 "ds_parser.ypp"
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 5048 "ds_parser.cpp"
    break;

  case 68: /* annotation_argument_value: "integer constant"  */
#line 835 "ds_parser.ypp"
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
#line 5054 "ds_parser.cpp"
    break;

  case 69: /* annotation_argument_value: "floating point constant"  */
#line 836 "ds_parser.ypp"
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
#line 5060 "ds_parser.cpp"
    break;

  case 70: /* annotation_argument_value: "true"  */
#line 837 "ds_parser.ypp"
                                 { (yyval.aa) = new AnnotationArgument("",true); }
#line 5066 "ds_parser.cpp"
    break;

  case 71: /* annotation_argument_value: "false"  */
#line 838 "ds_parser.ypp"
                                 { (yyval.aa) = new AnnotationArgument("",false); }
#line 5072 "ds_parser.cpp"
    break;

  case 72: /* annotation_argument_value_list: annotation_argument_value  */
#line 842 "ds_parser.ypp"
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
#line 5082 "ds_parser.cpp"
    break;

  case 73: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
#line 847 "ds_parser.ypp"
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
#line 5092 "ds_parser.cpp"
    break;

  case 74: /* annotation_argument_name: "name"  */
#line 855 "ds_parser.ypp"
                    { (yyval.s) = (yyvsp[0].s); }
#line 5098 "ds_parser.cpp"
    break;

  case 75: /* annotation_argument_name: "type"  */
#line 856 "ds_parser.ypp"
                    { (yyval.s) = new string("type"); }
#line 5104 "ds_parser.cpp"
    break;

  case 76: /* annotation_argument_name: "in"  */
#line 857 "ds_parser.ypp"
                    { (yyval.s) = new string("in"); }
#line 5110 "ds_parser.cpp"
    break;

  case 77: /* annotation_argument: annotation_argument_name '=' string_constant  */
#line 861 "ds_parser.ypp"
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 5116 "ds_parser.cpp"
    break;

  case 78: /* annotation_argument: annotation_argument_name '=' "name"  */
#line 862 "ds_parser.ypp"
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
#line 5122 "ds_parser.cpp"
    break;

  case 79: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
#line 863 "ds_parser.ypp"
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
#line 5128 "ds_parser.cpp"
    break;

  case 80: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
#line 864 "ds_parser.ypp"
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
#line 5134 "ds_parser.cpp"
    break;

  case 81: /* annotation_argument: annotation_argument_name '=' "true"  */
#line 865 "ds_parser.ypp"
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
#line 5140 "ds_parser.cpp"
    break;

  case 82: /* annotation_argument: annotation_argument_name '=' "false"  */
#line 866 "ds_parser.ypp"
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
#line 5146 "ds_parser.cpp"
    break;

  case 83: /* annotation_argument: annotation_argument_name  */
#line 867 "ds_parser.ypp"
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
#line 5152 "ds_parser.cpp"
    break;

  case 84: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
#line 868 "ds_parser.ypp"
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
#line 5160 "ds_parser.cpp"
    break;

  case 85: /* annotation_argument_list: annotation_argument  */
#line 874 "ds_parser.ypp"
                                  {
        (yyval.aaList) = new AnnotationArgumentList();
        if ( (yyvsp[0].aa)->type==Type::none ) {
            for ( auto & sarg : *((yyvsp[0].aa)->aList) ) {
                sarg.name = (yyvsp[0].aa)->name;
                sarg.at = (yyvsp[0].aa)->at;
                (yyval.aaList)->push_back(sarg);
            }
            delete (yyvsp[0].aa)->aList;
        } else {
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
        }
        delete (yyvsp[0].aa);
    }
#line 5179 "ds_parser.cpp"
    break;

  case 86: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
#line 888 "ds_parser.ypp"
                                                                    {
            (yyval.aaList) = (yyvsp[-2].aaList);
            if ( (yyvsp[0].aa)->type==Type::none ) {
                for ( auto & sarg : *((yyvsp[0].aa)->aList) ) {
                    sarg.name = (yyvsp[0].aa)->name;
                    sarg.at = (yyvsp[0].aa)->at;
                    (yyval.aaList)->push_back(sarg);
                }
                delete (yyvsp[0].aa)->aList;
            } else {
                (yyval.aaList)->push_back(*(yyvsp[0].aa));
            }
            delete (yyvsp[0].aa);
    }
#line 5198 "ds_parser.cpp"
    break;

  case 87: /* annotation_declaration_name: name_in_namespace  */
#line 905 "ds_parser.ypp"
                                    { (yyval.s) = (yyvsp[0].s); }
#line 5204 "ds_parser.cpp"
    break;

  case 88: /* annotation_declaration_name: "require"  */
#line 906 "ds_parser.ypp"
                                    { (yyval.s) = new string("require"); }
#line 5210 "ds_parser.cpp"
    break;

  case 89: /* annotation_declaration_name: "private"  */
#line 907 "ds_parser.ypp"
                                    { (yyval.s) = new string("private"); }
#line 5216 "ds_parser.cpp"
    break;

  case 90: /* annotation_declaration: annotation_declaration_name  */
#line 911 "ds_parser.ypp"
                                          {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[0]));
        if ( auto ann = findAnnotation(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0]))) ) {
            (yyval.fa)->annotation = ann;
        }
        delete (yyvsp[0].s);
    }
#line 5229 "ds_parser.cpp"
    break;

  case 91: /* annotation_declaration: annotation_declaration_name '(' annotation_argument_list ')'  */
#line 919 "ds_parser.ypp"
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
#line 5244 "ds_parser.cpp"
    break;

  case 92: /* annotation_list: annotation_declaration  */
#line 932 "ds_parser.ypp"
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 5253 "ds_parser.cpp"
    break;

  case 93: /* annotation_list: annotation_list ',' annotation_declaration  */
#line 936 "ds_parser.ypp"
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
#line 5262 "ds_parser.cpp"
    break;

  case 94: /* optional_annotation_list: %empty  */
#line 943 "ds_parser.ypp"
                                        { (yyval.faList) = nullptr; }
#line 5268 "ds_parser.cpp"
    break;

  case 95: /* optional_annotation_list: '[' annotation_list ']'  */
#line 944 "ds_parser.ypp"
                                        { (yyval.faList) = (yyvsp[-1].faList); }
#line 5274 "ds_parser.cpp"
    break;

  case 96: /* optional_function_argument_list: %empty  */
#line 948 "ds_parser.ypp"
                                                { (yyval.pVarDeclList) = nullptr; }
#line 5280 "ds_parser.cpp"
    break;

  case 97: /* optional_function_argument_list: '(' ')'  */
#line 949 "ds_parser.ypp"
                                                { (yyval.pVarDeclList) = nullptr; }
#line 5286 "ds_parser.cpp"
    break;

  case 98: /* optional_function_argument_list: '(' function_argument_list ')'  */
#line 950 "ds_parser.ypp"
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
#line 5292 "ds_parser.cpp"
    break;

  case 99: /* optional_function_type: %empty  */
#line 954 "ds_parser.ypp"
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
#line 5300 "ds_parser.cpp"
    break;

  case 100: /* optional_function_type: ':' type_declaration  */
#line 957 "ds_parser.ypp"
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
#line 5309 "ds_parser.cpp"
    break;

  case 101: /* function_name: "name"  */
#line 964 "ds_parser.ypp"
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
#line 5318 "ds_parser.cpp"
    break;

  case 102: /* function_name: "operator" '!'  */
#line 968 "ds_parser.ypp"
                             { (yyval.s) = new string("!"); }
#line 5324 "ds_parser.cpp"
    break;

  case 103: /* function_name: "operator" '~'  */
#line 969 "ds_parser.ypp"
                             { (yyval.s) = new string("~"); }
#line 5330 "ds_parser.cpp"
    break;

  case 104: /* function_name: "operator" "+="  */
#line 970 "ds_parser.ypp"
                             { (yyval.s) = new string("+="); }
#line 5336 "ds_parser.cpp"
    break;

  case 105: /* function_name: "operator" "-="  */
#line 971 "ds_parser.ypp"
                             { (yyval.s) = new string("-="); }
#line 5342 "ds_parser.cpp"
    break;

  case 106: /* function_name: "operator" "*="  */
#line 972 "ds_parser.ypp"
                             { (yyval.s) = new string("*="); }
#line 5348 "ds_parser.cpp"
    break;

  case 107: /* function_name: "operator" "/="  */
#line 973 "ds_parser.ypp"
                             { (yyval.s) = new string("/="); }
#line 5354 "ds_parser.cpp"
    break;

  case 108: /* function_name: "operator" "%="  */
#line 974 "ds_parser.ypp"
                             { (yyval.s) = new string("%="); }
#line 5360 "ds_parser.cpp"
    break;

  case 109: /* function_name: "operator" "&="  */
#line 975 "ds_parser.ypp"
                             { (yyval.s) = new string("&="); }
#line 5366 "ds_parser.cpp"
    break;

  case 110: /* function_name: "operator" "|="  */
#line 976 "ds_parser.ypp"
                             { (yyval.s) = new string("|="); }
#line 5372 "ds_parser.cpp"
    break;

  case 111: /* function_name: "operator" "^="  */
#line 977 "ds_parser.ypp"
                             { (yyval.s) = new string("^="); }
#line 5378 "ds_parser.cpp"
    break;

  case 112: /* function_name: "operator" "&&="  */
#line 978 "ds_parser.ypp"
                                { (yyval.s) = new string("&&="); }
#line 5384 "ds_parser.cpp"
    break;

  case 113: /* function_name: "operator" "||="  */
#line 979 "ds_parser.ypp"
                                { (yyval.s) = new string("||="); }
#line 5390 "ds_parser.cpp"
    break;

  case 114: /* function_name: "operator" "^^="  */
#line 980 "ds_parser.ypp"
                                { (yyval.s) = new string("^^="); }
#line 5396 "ds_parser.cpp"
    break;

  case 115: /* function_name: "operator" "&&"  */
#line 981 "ds_parser.ypp"
                             { (yyval.s) = new string("&&"); }
#line 5402 "ds_parser.cpp"
    break;

  case 116: /* function_name: "operator" "||"  */
#line 982 "ds_parser.ypp"
                             { (yyval.s) = new string("||"); }
#line 5408 "ds_parser.cpp"
    break;

  case 117: /* function_name: "operator" "^^"  */
#line 983 "ds_parser.ypp"
                             { (yyval.s) = new string("^^"); }
#line 5414 "ds_parser.cpp"
    break;

  case 118: /* function_name: "operator" '+'  */
#line 984 "ds_parser.ypp"
                             { (yyval.s) = new string("+"); }
#line 5420 "ds_parser.cpp"
    break;

  case 119: /* function_name: "operator" '-'  */
#line 985 "ds_parser.ypp"
                             { (yyval.s) = new string("-"); }
#line 5426 "ds_parser.cpp"
    break;

  case 120: /* function_name: "operator" '*'  */
#line 986 "ds_parser.ypp"
                             { (yyval.s) = new string("*"); }
#line 5432 "ds_parser.cpp"
    break;

  case 121: /* function_name: "operator" '/'  */
#line 987 "ds_parser.ypp"
                             { (yyval.s) = new string("/"); }
#line 5438 "ds_parser.cpp"
    break;

  case 122: /* function_name: "operator" '%'  */
#line 988 "ds_parser.ypp"
                             { (yyval.s) = new string("%"); }
#line 5444 "ds_parser.cpp"
    break;

  case 123: /* function_name: "operator" '<'  */
#line 989 "ds_parser.ypp"
                             { (yyval.s) = new string("<"); }
#line 5450 "ds_parser.cpp"
    break;

  case 124: /* function_name: "operator" '>'  */
#line 990 "ds_parser.ypp"
                             { (yyval.s) = new string(">"); }
#line 5456 "ds_parser.cpp"
    break;

  case 125: /* function_name: "operator" "=="  */
#line 991 "ds_parser.ypp"
                             { (yyval.s) = new string("=="); }
#line 5462 "ds_parser.cpp"
    break;

  case 126: /* function_name: "operator" "!="  */
#line 992 "ds_parser.ypp"
                             { (yyval.s) = new string("!="); }
#line 5468 "ds_parser.cpp"
    break;

  case 127: /* function_name: "operator" "<="  */
#line 993 "ds_parser.ypp"
                             { (yyval.s) = new string("<="); }
#line 5474 "ds_parser.cpp"
    break;

  case 128: /* function_name: "operator" ">="  */
#line 994 "ds_parser.ypp"
                             { (yyval.s) = new string(">="); }
#line 5480 "ds_parser.cpp"
    break;

  case 129: /* function_name: "operator" '&'  */
#line 995 "ds_parser.ypp"
                             { (yyval.s) = new string("&"); }
#line 5486 "ds_parser.cpp"
    break;

  case 130: /* function_name: "operator" '|'  */
#line 996 "ds_parser.ypp"
                             { (yyval.s) = new string("|"); }
#line 5492 "ds_parser.cpp"
    break;

  case 131: /* function_name: "operator" '^'  */
#line 997 "ds_parser.ypp"
                             { (yyval.s) = new string("^"); }
#line 5498 "ds_parser.cpp"
    break;

  case 132: /* function_name: "++" "operator"  */
#line 998 "ds_parser.ypp"
                             { (yyval.s) = new string("++"); }
#line 5504 "ds_parser.cpp"
    break;

  case 133: /* function_name: "--" "operator"  */
#line 999 "ds_parser.ypp"
                             { (yyval.s) = new string("--"); }
#line 5510 "ds_parser.cpp"
    break;

  case 134: /* function_name: "operator" "++"  */
#line 1000 "ds_parser.ypp"
                             { (yyval.s) = new string("+++"); }
#line 5516 "ds_parser.cpp"
    break;

  case 135: /* function_name: "operator" "--"  */
#line 1001 "ds_parser.ypp"
                             { (yyval.s) = new string("---"); }
#line 5522 "ds_parser.cpp"
    break;

  case 136: /* function_name: "operator" "<<"  */
#line 1002 "ds_parser.ypp"
                             { (yyval.s) = new string("<<"); }
#line 5528 "ds_parser.cpp"
    break;

  case 137: /* function_name: "operator" ">>"  */
#line 1003 "ds_parser.ypp"
                             { (yyval.s) = new string(">>"); }
#line 5534 "ds_parser.cpp"
    break;

  case 138: /* function_name: "operator" "<<="  */
#line 1004 "ds_parser.ypp"
                             { (yyval.s) = new string("<<="); }
#line 5540 "ds_parser.cpp"
    break;

  case 139: /* function_name: "operator" ">>="  */
#line 1005 "ds_parser.ypp"
                             { (yyval.s) = new string(">>="); }
#line 5546 "ds_parser.cpp"
    break;

  case 140: /* function_name: "operator" "<<<"  */
#line 1006 "ds_parser.ypp"
                             { (yyval.s) = new string("<<<"); }
#line 5552 "ds_parser.cpp"
    break;

  case 141: /* function_name: "operator" ">>>"  */
#line 1007 "ds_parser.ypp"
                             { (yyval.s) = new string(">>>"); }
#line 5558 "ds_parser.cpp"
    break;

  case 142: /* function_name: "operator" "<<<="  */
#line 1008 "ds_parser.ypp"
                             { (yyval.s) = new string("<<<="); }
#line 5564 "ds_parser.cpp"
    break;

  case 143: /* function_name: "operator" ">>>="  */
#line 1009 "ds_parser.ypp"
                             { (yyval.s) = new string(">>>="); }
#line 5570 "ds_parser.cpp"
    break;

  case 144: /* function_name: "operator" '[' ']'  */
#line 1010 "ds_parser.ypp"
                             { (yyval.s) = new string("[]"); }
#line 5576 "ds_parser.cpp"
    break;

  case 145: /* function_name: "operator" "?[" ']'  */
#line 1011 "ds_parser.ypp"
                                { (yyval.s) = new string("?[]"); }
#line 5582 "ds_parser.cpp"
    break;

  case 146: /* function_name: "operator" '.'  */
#line 1012 "ds_parser.ypp"
                             { (yyval.s) = new string("."); }
#line 5588 "ds_parser.cpp"
    break;

  case 147: /* function_name: "operator" "?."  */
#line 1013 "ds_parser.ypp"
                             { (yyval.s) = new string("?."); }
#line 5594 "ds_parser.cpp"
    break;

  case 148: /* function_name: "operator" ":="  */
#line 1014 "ds_parser.ypp"
                                { (yyval.s) = new string("clone"); }
#line 5600 "ds_parser.cpp"
    break;

  case 149: /* function_name: "operator" "delete"  */
#line 1015 "ds_parser.ypp"
                                { (yyval.s) = new string("finalize"); }
#line 5606 "ds_parser.cpp"
    break;

  case 150: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
#line 1019 "ds_parser.ypp"
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
#line 5630 "ds_parser.cpp"
    break;

  case 151: /* optional_public_or_private_function: %empty  */
#line 1041 "ds_parser.ypp"
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
#line 5636 "ds_parser.cpp"
    break;

  case 152: /* optional_public_or_private_function: "private"  */
#line 1042 "ds_parser.ypp"
                        { (yyval.b) = false; }
#line 5642 "ds_parser.cpp"
    break;

  case 153: /* optional_public_or_private_function: "public"  */
#line 1043 "ds_parser.ypp"
                        { (yyval.b) = true; }
#line 5648 "ds_parser.cpp"
    break;

  case 154: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
#line 1047 "ds_parser.ypp"
                                                                                                {
        auto pFunction = make_smart<Function>();
        pFunction->at = tokAt(scanner,(yylsp[-2]));
        pFunction->name = *(yyvsp[-2].s);
        pFunction->result = TypeDeclPtr((yyvsp[0].pTypeDecl));
        if ( (yyvsp[-1].pVarDeclList) ) {
            for ( auto pDecl : *(yyvsp[-1].pVarDeclList) ) {
                if ( pDecl->pTypeDecl ) {
                    for ( const auto & name_at : *pDecl->pNameList ) {
                        if ( !pFunction->findArgument(name_at.name) ) {
                            VariablePtr pVar = make_smart<Variable>();
                            pVar->name = name_at.name;
                            pVar->aka = name_at.aka;
                            pVar->at = name_at.at;
                            pVar->type = make_smart<TypeDecl>(*pDecl->pTypeDecl);
                            if ( pDecl->pInit ) {
                                pVar->init = ExpressionPtr(pDecl->pInit->clone());
                                pVar->init_via_move = pDecl->init_via_move;
                                pVar->init_via_clone = pDecl->init_via_clone;
                            }
                            if ( pDecl->annotation ) {
                                pVar->annotation = *pDecl->annotation;
                            }
                            pFunction->arguments.push_back(pVar);
                        } else {
                            das_yyerror(scanner,"function argument is already declared " + name_at.name,name_at.at,
                                CompilationError::argument_already_declared);
                        }
                    }
                }
            }
            deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
        }
        delete (yyvsp[-2].s);
        (yyval.pFuncDecl) = pFunction.orphan();
    }
#line 5689 "ds_parser.cpp"
    break;

  case 155: /* function_declaration: optional_public_or_private_function function_declaration_header expression_block  */
#line 1086 "ds_parser.ypp"
                                                                                                               {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyvsp[-1].pFuncDecl)->privateFunction = !(yyvsp[-2].b);
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
    }
#line 5699 "ds_parser.cpp"
    break;

  case 156: /* expression_block: '{' expressions '}'  */
#line 1094 "ds_parser.ypp"
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
#line 5708 "ds_parser.cpp"
    break;

  case 157: /* expression_block: '{' expressions '}' "finally" '{' expressions '}'  */
#line 1098 "ds_parser.ypp"
                                                                                          {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
#line 5721 "ds_parser.cpp"
    break;

  case 158: /* expression_any: ';'  */
#line 1109 "ds_parser.ypp"
                                            { (yyval.pExpression) = nullptr; }
#line 5727 "ds_parser.cpp"
    break;

  case 159: /* expression_any: expr_pipe  */
#line 1110 "ds_parser.ypp"
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5733 "ds_parser.cpp"
    break;

  case 160: /* expression_any: expr_assign_pipe  */
#line 1111 "ds_parser.ypp"
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5739 "ds_parser.cpp"
    break;

  case 161: /* expression_any: expr_assign ';'  */
#line 1112 "ds_parser.ypp"
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5745 "ds_parser.cpp"
    break;

  case 162: /* expression_any: expression_delete ';'  */
#line 1113 "ds_parser.ypp"
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5751 "ds_parser.cpp"
    break;

  case 163: /* expression_any: expression_let  */
#line 1114 "ds_parser.ypp"
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5757 "ds_parser.cpp"
    break;

  case 164: /* expression_any: expression_while_loop  */
#line 1115 "ds_parser.ypp"
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5763 "ds_parser.cpp"
    break;

  case 165: /* expression_any: expression_unsafe  */
#line 1116 "ds_parser.ypp"
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5769 "ds_parser.cpp"
    break;

  case 166: /* expression_any: expression_with  */
#line 1117 "ds_parser.ypp"
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5775 "ds_parser.cpp"
    break;

  case 167: /* expression_any: expression_with_alias  */
#line 1118 "ds_parser.ypp"
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5781 "ds_parser.cpp"
    break;

  case 168: /* expression_any: expression_for_loop  */
#line 1119 "ds_parser.ypp"
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5787 "ds_parser.cpp"
    break;

  case 169: /* expression_any: expression_break ';'  */
#line 1120 "ds_parser.ypp"
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5793 "ds_parser.cpp"
    break;

  case 170: /* expression_any: expression_continue ';'  */
#line 1121 "ds_parser.ypp"
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5799 "ds_parser.cpp"
    break;

  case 171: /* expression_any: expression_return  */
#line 1122 "ds_parser.ypp"
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5805 "ds_parser.cpp"
    break;

  case 172: /* expression_any: expression_yield  */
#line 1123 "ds_parser.ypp"
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5811 "ds_parser.cpp"
    break;

  case 173: /* expression_any: expression_if_then_else  */
#line 1124 "ds_parser.ypp"
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5817 "ds_parser.cpp"
    break;

  case 174: /* expression_any: expression_try_catch  */
#line 1125 "ds_parser.ypp"
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 5823 "ds_parser.cpp"
    break;

  case 175: /* expression_any: expression_label ';'  */
#line 1126 "ds_parser.ypp"
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5829 "ds_parser.cpp"
    break;

  case 176: /* expression_any: expression_goto ';'  */
#line 1127 "ds_parser.ypp"
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 5835 "ds_parser.cpp"
    break;

  case 177: /* expression_any: "pass" ';'  */
#line 1128 "ds_parser.ypp"
                                            { (yyval.pExpression) = nullptr; }
#line 5841 "ds_parser.cpp"
    break;

  case 178: /* expressions: %empty  */
#line 1132 "ds_parser.ypp"
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
#line 5851 "ds_parser.cpp"
    break;

  case 179: /* expressions: expressions expression_any  */
#line 1137 "ds_parser.ypp"
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
#line 5862 "ds_parser.cpp"
    break;

  case 180: /* expressions: expressions error  */
#line 1143 "ds_parser.ypp"
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
#line 5870 "ds_parser.cpp"
    break;

  case 181: /* expr_pipe: expr_assign " <|" expr_block  */
#line 1149 "ds_parser.ypp"
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
#line 5909 "ds_parser.cpp"
    break;

  case 182: /* expr_pipe: "@ <|" expr_block  */
#line 1183 "ds_parser.ypp"
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5917 "ds_parser.cpp"
    break;

  case 183: /* expr_pipe: "@@ <|" expr_block  */
#line 1186 "ds_parser.ypp"
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 5925 "ds_parser.cpp"
    break;

  case 184: /* name_in_namespace: "name"  */
#line 1192 "ds_parser.ypp"
                                               { (yyval.s) = (yyvsp[0].s); }
#line 5931 "ds_parser.cpp"
    break;

  case 185: /* name_in_namespace: "name" "::" "name"  */
#line 1193 "ds_parser.ypp"
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
#line 5947 "ds_parser.cpp"
    break;

  case 186: /* name_in_namespace: "::" "name"  */
#line 1204 "ds_parser.ypp"
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
#line 5953 "ds_parser.cpp"
    break;

  case 187: /* expression_delete: "delete" expr  */
#line 1208 "ds_parser.ypp"
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5961 "ds_parser.cpp"
    break;

  case 188: /* expr_new: "new" type_declaration  */
#line 1214 "ds_parser.ypp"
                                                   {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
#line 5969 "ds_parser.cpp"
    break;

  case 189: /* expr_new: "new" type_declaration '(' ')'  */
#line 1217 "ds_parser.ypp"
                                                           {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-3])),TypeDeclPtr((yyvsp[-2].pTypeDecl)),true);
    }
#line 5977 "ds_parser.cpp"
    break;

  case 190: /* expr_new: "new" type_declaration '(' expr_list ')'  */
#line 1220 "ds_parser.ypp"
                                                                                {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
#line 5986 "ds_parser.cpp"
    break;

  case 191: /* expr_new: "new" make_decl  */
#line 1224 "ds_parser.ypp"
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 5994 "ds_parser.cpp"
    break;

  case 192: /* expression_break: "break"  */
#line 1230 "ds_parser.ypp"
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
#line 6000 "ds_parser.cpp"
    break;

  case 193: /* expression_continue: "continue"  */
#line 1234 "ds_parser.ypp"
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
#line 6006 "ds_parser.cpp"
    break;

  case 194: /* expression_return: "return" ';'  */
#line 1238 "ds_parser.ypp"
                            {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),nullptr);
    }
#line 6014 "ds_parser.cpp"
    break;

  case 195: /* expression_return: "return" expr ';'  */
#line 1241 "ds_parser.ypp"
                                          {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 6022 "ds_parser.cpp"
    break;

  case 196: /* expression_return: "return" "<-" expr ';'  */
#line 1244 "ds_parser.ypp"
                                                 {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 6032 "ds_parser.cpp"
    break;

  case 197: /* expression_return: "return" expr_pipe  */
#line 1249 "ds_parser.ypp"
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 6040 "ds_parser.cpp"
    break;

  case 198: /* expression_return: "return" "<-" expr_pipe  */
#line 1252 "ds_parser.ypp"
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 6050 "ds_parser.cpp"
    break;

  case 199: /* expression_yield: "yield" expr ';'  */
#line 1260 "ds_parser.ypp"
                                         {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
#line 6058 "ds_parser.cpp"
    break;

  case 200: /* expression_yield: "yield" "<-" expr ';'  */
#line 1263 "ds_parser.ypp"
                                                {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 6068 "ds_parser.cpp"
    break;

  case 201: /* expression_yield: "yield" expr_pipe  */
#line 1268 "ds_parser.ypp"
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 6076 "ds_parser.cpp"
    break;

  case 202: /* expression_yield: "yield" "<-" expr_pipe  */
#line 1271 "ds_parser.ypp"
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
#line 6086 "ds_parser.cpp"
    break;

  case 203: /* expression_try_catch: "try" expression_block "recover" expression_block  */
#line 1279 "ds_parser.ypp"
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 6094 "ds_parser.cpp"
    break;

  case 204: /* kwd_let: "let"  */
#line 1285 "ds_parser.ypp"
                 { (yyval.b) = true; }
#line 6100 "ds_parser.cpp"
    break;

  case 205: /* kwd_let: "var"  */
#line 1286 "ds_parser.ypp"
                 { (yyval.b) = false; }
#line 6106 "ds_parser.cpp"
    break;

  case 206: /* expression_let: kwd_let let_variable_declaration  */
#line 1290 "ds_parser.ypp"
                                               {
        auto pLet = new ExprLet();
        pLet->at = tokAt(scanner,(yylsp[-1]));
        pLet->atInit = tokAt(scanner,(yylsp[0]));
        if ( (yyvsp[0].pVarDecl)->pTypeDecl ) {
            for ( const auto & name_at : *(yyvsp[0].pVarDecl)->pNameList ) {
                if ( !pLet->find(name_at.name) ) {
                    VariablePtr pVar = make_smart<Variable>();
                    pVar->name = name_at.name;
                    pVar->aka = name_at.aka;
                    pVar->at = name_at.at;
                    pVar->type = make_smart<TypeDecl>(*(yyvsp[0].pVarDecl)->pTypeDecl);
                    if ( (yyvsp[0].pVarDecl)->pInit ) {
                        pVar->init = (yyvsp[0].pVarDecl)->pInit->clone();
                        pVar->init_via_move = (yyvsp[0].pVarDecl)->init_via_move;
                        pVar->init_via_clone = (yyvsp[0].pVarDecl)->init_via_clone;
                    }
                    if ( (yyvsp[-1].b) ) {
                        pVar->type->constant = true;
                    } else {
                        pVar->type->removeConstant = true;
                    }
                    pLet->variables.push_back(pVar);
                } else {
                    das_yyerror(scanner,"local variable is already declared " + name_at.name,name_at.at,
                        CompilationError::local_variable_already_declared);
                }
            }
        }
        delete (yyvsp[0].pVarDecl);
        (yyval.pExpression) = pLet;
    }
#line 6143 "ds_parser.cpp"
    break;

  case 207: /* $@3: %empty  */
#line 1325 "ds_parser.ypp"
                          { yyextra->das_arrow_depth ++; }
#line 6149 "ds_parser.cpp"
    break;

  case 208: /* $@4: %empty  */
#line 1325 "ds_parser.ypp"
                                                                                      { yyextra->das_arrow_depth --; }
#line 6155 "ds_parser.cpp"
    break;

  case 209: /* expr_cast: "cast" '<' $@3 type_declaration '>' $@4 expr  */
#line 1325 "ds_parser.ypp"
                                                                                                                                     {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
#line 6163 "ds_parser.cpp"
    break;

  case 210: /* $@5: %empty  */
#line 1328 "ds_parser.ypp"
                            { yyextra->das_arrow_depth ++; }
#line 6169 "ds_parser.cpp"
    break;

  case 211: /* $@6: %empty  */
#line 1328 "ds_parser.ypp"
                                                                                        { yyextra->das_arrow_depth --; }
#line 6175 "ds_parser.cpp"
    break;

  case 212: /* expr_cast: "upcast" '<' $@5 type_declaration '>' $@6 expr  */
#line 1328 "ds_parser.ypp"
                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
#line 6185 "ds_parser.cpp"
    break;

  case 213: /* $@7: %empty  */
#line 1333 "ds_parser.ypp"
                                 { yyextra->das_arrow_depth ++; }
#line 6191 "ds_parser.cpp"
    break;

  case 214: /* $@8: %empty  */
#line 1333 "ds_parser.ypp"
                                                                                             { yyextra->das_arrow_depth --; }
#line 6197 "ds_parser.cpp"
    break;

  case 215: /* expr_cast: "reinterpret" '<' $@7 type_declaration '>' $@8 expr  */
#line 1333 "ds_parser.ypp"
                                                                                                                                            {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
#line 6207 "ds_parser.cpp"
    break;

  case 216: /* $@9: %empty  */
#line 1341 "ds_parser.ypp"
                         { yyextra->das_arrow_depth ++; }
#line 6213 "ds_parser.cpp"
    break;

  case 217: /* $@10: %empty  */
#line 1341 "ds_parser.ypp"
                                                                                     { yyextra->das_arrow_depth --; }
#line 6219 "ds_parser.cpp"
    break;

  case 218: /* expr_type_decl: "type" '<' $@9 type_declaration '>' $@10  */
#line 1341 "ds_parser.ypp"
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 6227 "ds_parser.cpp"
    break;

  case 219: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
#line 1347 "ds_parser.ypp"
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
#line 6242 "ds_parser.cpp"
    break;

  case 220: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
#line 1357 "ds_parser.ypp"
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
#line 6258 "ds_parser.cpp"
    break;

  case 221: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" ';' "name" '>' expr ')'  */
#line 1368 "ds_parser.ypp"
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
#line 6275 "ds_parser.cpp"
    break;

  case 222: /* expr_list: expr  */
#line 1383 "ds_parser.ypp"
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 6283 "ds_parser.cpp"
    break;

  case 223: /* expr_list: expr_list ',' expr  */
#line 1386 "ds_parser.ypp"
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
#line 6291 "ds_parser.cpp"
    break;

  case 224: /* block_or_simple_block: expression_block  */
#line 1392 "ds_parser.ypp"
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6297 "ds_parser.cpp"
    break;

  case 225: /* block_or_simple_block: "=>" expr  */
#line 1393 "ds_parser.ypp"
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 6309 "ds_parser.cpp"
    break;

  case 226: /* block_or_simple_block: "=>" "<-" expr  */
#line 1400 "ds_parser.ypp"
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
#line 6322 "ds_parser.cpp"
    break;

  case 227: /* block_or_lambda: '$'  */
#line 1411 "ds_parser.ypp"
                { (yyval.i) = 0;   /* block */  }
#line 6328 "ds_parser.cpp"
    break;

  case 228: /* block_or_lambda: '@'  */
#line 1412 "ds_parser.ypp"
                { (yyval.i) = 1;   /* lambda */ }
#line 6334 "ds_parser.cpp"
    break;

  case 229: /* block_or_lambda: '@' '@'  */
#line 1413 "ds_parser.ypp"
                { (yyval.i) = 2;   /* local function */ }
#line 6340 "ds_parser.cpp"
    break;

  case 230: /* capture_entry: '&' "name"  */
#line 1417 "ds_parser.ypp"
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
#line 6346 "ds_parser.cpp"
    break;

  case 231: /* capture_entry: '=' "name"  */
#line 1418 "ds_parser.ypp"
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
#line 6352 "ds_parser.cpp"
    break;

  case 232: /* capture_entry: "<-" "name"  */
#line 1419 "ds_parser.ypp"
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
#line 6358 "ds_parser.cpp"
    break;

  case 233: /* capture_entry: ":=" "name"  */
#line 1420 "ds_parser.ypp"
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
#line 6364 "ds_parser.cpp"
    break;

  case 234: /* capture_list: capture_entry  */
#line 1424 "ds_parser.ypp"
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
#line 6374 "ds_parser.cpp"
    break;

  case 235: /* capture_list: capture_list ',' capture_entry  */
#line 1429 "ds_parser.ypp"
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
#line 6384 "ds_parser.cpp"
    break;

  case 236: /* optional_capture_list: %empty  */
#line 1437 "ds_parser.ypp"
        { (yyval.pCaptList) = nullptr; }
#line 6390 "ds_parser.cpp"
    break;

  case 237: /* optional_capture_list: "[[" capture_list ']' ']'  */
#line 1438 "ds_parser.ypp"
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
#line 6396 "ds_parser.cpp"
    break;

  case 238: /* expr_block: expression_block  */
#line 1442 "ds_parser.ypp"
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
#line 6406 "ds_parser.cpp"
    break;

  case 239: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
#line 1448 "ds_parser.ypp"
                                                                                            {
        auto mkb = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)), (yyvsp[-5].i)==1, (yyvsp[-5].i)==2);
        (yyval.pExpression) = mkb;
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        closure->returnType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
        if ( (yyvsp[-2].pVarDeclList) ) {
            for ( auto pDecl : *(yyvsp[-2].pVarDeclList) ) {
                if ( pDecl->pTypeDecl ) {
                    for ( const auto & name_at : *pDecl->pNameList ) {
                        if ( !closure->findArgument(name_at.name) ) {
                            VariablePtr pVar = make_smart<Variable>();
                            pVar->name = name_at.name;
                            pVar->aka = name_at.aka;
                            pVar->at = name_at.at;
                            pVar->type = make_smart<TypeDecl>(*pDecl->pTypeDecl);
                            if ( pDecl->pInit ) {
                                pVar->init = ExpressionPtr(pDecl->pInit->clone());
                                pVar->init_via_move = pDecl->init_via_move;
                                pVar->init_via_clone = pDecl->init_via_clone;
                            }
                            if ( pDecl->annotation ) {
                                pVar->annotation = *pDecl->annotation;
                            }
                            closure->arguments.push_back(pVar);
                        } else {
                            das_yyerror(scanner,"block argument is already declared " + name_at.name,
                                name_at.at,CompilationError::argument_already_declared);
                        }
                    }
                }
            }
            deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        }
        if ( (yyvsp[-3].pCaptList) ) {
            swap ( mkb->capture, *(yyvsp[-3].pCaptList) );
            delete (yyvsp[-3].pCaptList);
            if ( (yyvsp[-5].i) != 1 ) {   // if its not lambda, can't capture
                das_yyerror(scanner,"can only have capture section for the lambda",
                    mkb->at,CompilationError::invalid_capture);
            }
        }
        if ( (yyvsp[-4].faList) ) {
            for ( auto pA : *(yyvsp[-4].faList) ) {
                if ( pA->annotation ) {
                    if ( pA->annotation->rtti_isFunctionAnnotation() ) {
                        auto ann = static_pointer_cast<FunctionAnnotation>(pA->annotation);
                        string err;
                        if ( !ann->apply(closure, *yyextra->g_Program->thisModuleGroup, pA->arguments, err) ) {
                            das_yyerror(scanner,"macro [" +pA->annotation->name + "] failed to apply to the block\n" + err, tokAt(scanner,(yylsp[-4])),
                                CompilationError::invalid_annotation);
                        }
                    } else {
                        das_yyerror(scanner,"blocks are only allowed function macros", tokAt(scanner,(yylsp[-4])),
                            CompilationError::invalid_annotation);
                    }
                }
            }
            swap ( closure->annotations, *(yyvsp[-4].faList) );
            delete (yyvsp[-4].faList);
        }
    }
#line 6472 "ds_parser.cpp"
    break;

  case 240: /* expr_numeric_const: "integer constant"  */
#line 1512 "ds_parser.ypp"
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
#line 6478 "ds_parser.cpp"
    break;

  case 241: /* expr_numeric_const: "unsigned integer constant"  */
#line 1513 "ds_parser.ypp"
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
#line 6484 "ds_parser.cpp"
    break;

  case 242: /* expr_numeric_const: "long integer constant"  */
#line 1514 "ds_parser.ypp"
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
#line 6490 "ds_parser.cpp"
    break;

  case 243: /* expr_numeric_const: "unsigned long integer constant"  */
#line 1515 "ds_parser.ypp"
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
#line 6496 "ds_parser.cpp"
    break;

  case 244: /* expr_numeric_const: "floating point constant"  */
#line 1516 "ds_parser.ypp"
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
#line 6502 "ds_parser.cpp"
    break;

  case 245: /* expr_numeric_const: "double constant"  */
#line 1517 "ds_parser.ypp"
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
#line 6508 "ds_parser.cpp"
    break;

  case 246: /* expr_assign: expr  */
#line 1521 "ds_parser.ypp"
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6514 "ds_parser.cpp"
    break;

  case 247: /* expr_assign: expr '=' expr  */
#line 1522 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6520 "ds_parser.cpp"
    break;

  case 248: /* expr_assign: expr "<-" expr  */
#line 1523 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6526 "ds_parser.cpp"
    break;

  case 249: /* expr_assign: expr ":=" expr  */
#line 1524 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6532 "ds_parser.cpp"
    break;

  case 250: /* expr_assign: expr "&=" expr  */
#line 1525 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6538 "ds_parser.cpp"
    break;

  case 251: /* expr_assign: expr "|=" expr  */
#line 1526 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6544 "ds_parser.cpp"
    break;

  case 252: /* expr_assign: expr "^=" expr  */
#line 1527 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6550 "ds_parser.cpp"
    break;

  case 253: /* expr_assign: expr "&&=" expr  */
#line 1528 "ds_parser.ypp"
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6556 "ds_parser.cpp"
    break;

  case 254: /* expr_assign: expr "||=" expr  */
#line 1529 "ds_parser.ypp"
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6562 "ds_parser.cpp"
    break;

  case 255: /* expr_assign: expr "^^=" expr  */
#line 1530 "ds_parser.ypp"
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6568 "ds_parser.cpp"
    break;

  case 256: /* expr_assign: expr "+=" expr  */
#line 1531 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6574 "ds_parser.cpp"
    break;

  case 257: /* expr_assign: expr "-=" expr  */
#line 1532 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6580 "ds_parser.cpp"
    break;

  case 258: /* expr_assign: expr "*=" expr  */
#line 1533 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6586 "ds_parser.cpp"
    break;

  case 259: /* expr_assign: expr "/=" expr  */
#line 1534 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6592 "ds_parser.cpp"
    break;

  case 260: /* expr_assign: expr "%=" expr  */
#line 1535 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6598 "ds_parser.cpp"
    break;

  case 261: /* expr_assign: expr "<<=" expr  */
#line 1536 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6604 "ds_parser.cpp"
    break;

  case 262: /* expr_assign: expr ">>=" expr  */
#line 1537 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6610 "ds_parser.cpp"
    break;

  case 263: /* expr_assign: expr "<<<=" expr  */
#line 1538 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6616 "ds_parser.cpp"
    break;

  case 264: /* expr_assign: expr ">>>=" expr  */
#line 1539 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6622 "ds_parser.cpp"
    break;

  case 265: /* expr_assign_pipe: expr '=' "@ <|" expr  */
#line 1543 "ds_parser.ypp"
                                                    { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6628 "ds_parser.cpp"
    break;

  case 266: /* expr_assign_pipe: expr '=' "@@ <|" expr  */
#line 1544 "ds_parser.ypp"
                                                    { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 6634 "ds_parser.cpp"
    break;

  case 267: /* expr_assign_pipe: expr "<-" "@ <|" expr  */
#line 1545 "ds_parser.ypp"
                                                    { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6640 "ds_parser.cpp"
    break;

  case 268: /* expr_assign_pipe: expr "<-" "@@ <|" expr  */
#line 1546 "ds_parser.ypp"
                                                    { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6646 "ds_parser.cpp"
    break;

  case 269: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
#line 1550 "ds_parser.ypp"
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
#line 6658 "ds_parser.cpp"
    break;

  case 270: /* expr_method_call: expr "->" "name" '(' ')'  */
#line 1561 "ds_parser.ypp"
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6668 "ds_parser.cpp"
    break;

  case 271: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
#line 1566 "ds_parser.ypp"
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
#line 6680 "ds_parser.cpp"
    break;

  case 272: /* func_addr_expr: '@' '@' name_in_namespace  */
#line 1576 "ds_parser.ypp"
                                                 {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[-1])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6689 "ds_parser.cpp"
    break;

  case 273: /* $@11: %empty  */
#line 1580 "ds_parser.ypp"
                         { yyextra->das_arrow_depth ++; }
#line 6695 "ds_parser.cpp"
    break;

  case 274: /* $@12: %empty  */
#line 1580 "ds_parser.ypp"
                                                                                          { yyextra->das_arrow_depth --; }
#line 6701 "ds_parser.cpp"
    break;

  case 275: /* func_addr_expr: '@' '@' '<' $@11 type_declaration '>' $@12 name_in_namespace  */
#line 1580 "ds_parser.ypp"
                                                                                                                                                   {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-6])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = expr;
    }
#line 6713 "ds_parser.cpp"
    break;

  case 276: /* $@13: %empty  */
#line 1587 "ds_parser.ypp"
                         { yyextra->das_arrow_depth ++; }
#line 6719 "ds_parser.cpp"
    break;

  case 277: /* $@14: %empty  */
#line 1587 "ds_parser.ypp"
                                                                                                                                   { yyextra->das_arrow_depth --; }
#line 6725 "ds_parser.cpp"
    break;

  case 278: /* func_addr_expr: '@' '@' '<' $@13 optional_function_argument_list optional_function_type '>' $@14 name_in_namespace  */
#line 1587 "ds_parser.ypp"
                                                                                                                                                                                            {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-7])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        if ( (yyvsp[-4].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, expr->funcType.get(), (yyvsp[-4].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-4].pVarDeclList));
        }
        (yyval.pExpression) = expr;
    }
#line 6741 "ds_parser.cpp"
    break;

  case 279: /* expr_field: expr '.' "name"  */
#line 1601 "ds_parser.ypp"
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 6750 "ds_parser.cpp"
    break;

  case 280: /* expr_field: expr '.' '.' "name"  */
#line 1605 "ds_parser.ypp"
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
#line 6759 "ds_parser.cpp"
    break;

  case 281: /* $@15: %empty  */
#line 1609 "ds_parser.ypp"
                               { yyextra->das_supress_errors=true; }
#line 6765 "ds_parser.cpp"
    break;

  case 282: /* $@16: %empty  */
#line 1609 "ds_parser.ypp"
                                                                           { yyextra->das_supress_errors=false; }
#line 6771 "ds_parser.cpp"
    break;

  case 283: /* expr_field: expr '.' $@15 error $@16  */
#line 1609 "ds_parser.ypp"
                                                                                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
#line 6780 "ds_parser.cpp"
    break;

  case 284: /* expr: "null"  */
#line 1616 "ds_parser.ypp"
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
#line 6786 "ds_parser.cpp"
    break;

  case 285: /* expr: name_in_namespace  */
#line 1617 "ds_parser.ypp"
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 6792 "ds_parser.cpp"
    break;

  case 286: /* expr: expr_numeric_const  */
#line 1618 "ds_parser.ypp"
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6798 "ds_parser.cpp"
    break;

  case 287: /* expr: expr_reader  */
#line 1619 "ds_parser.ypp"
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6804 "ds_parser.cpp"
    break;

  case 288: /* expr: string_builder  */
#line 1620 "ds_parser.ypp"
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6810 "ds_parser.cpp"
    break;

  case 289: /* expr: make_decl  */
#line 1621 "ds_parser.ypp"
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6816 "ds_parser.cpp"
    break;

  case 290: /* expr: "true"  */
#line 1622 "ds_parser.ypp"
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
#line 6822 "ds_parser.cpp"
    break;

  case 291: /* expr: "false"  */
#line 1623 "ds_parser.ypp"
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
#line 6828 "ds_parser.cpp"
    break;

  case 292: /* expr: expr_field  */
#line 1624 "ds_parser.ypp"
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 6834 "ds_parser.cpp"
    break;

  case 293: /* expr: '!' expr  */
#line 1625 "ds_parser.ypp"
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6840 "ds_parser.cpp"
    break;

  case 294: /* expr: '~' expr  */
#line 1626 "ds_parser.ypp"
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6846 "ds_parser.cpp"
    break;

  case 295: /* expr: '+' expr  */
#line 1627 "ds_parser.ypp"
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6852 "ds_parser.cpp"
    break;

  case 296: /* expr: '-' expr  */
#line 1628 "ds_parser.ypp"
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
#line 6858 "ds_parser.cpp"
    break;

  case 297: /* expr: expr "<<" expr  */
#line 1629 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6864 "ds_parser.cpp"
    break;

  case 298: /* expr: expr ">>" expr  */
#line 1630 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6870 "ds_parser.cpp"
    break;

  case 299: /* expr: expr "<<<" expr  */
#line 1631 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6876 "ds_parser.cpp"
    break;

  case 300: /* expr: expr ">>>" expr  */
#line 1632 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6882 "ds_parser.cpp"
    break;

  case 301: /* expr: expr '+' expr  */
#line 1633 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6888 "ds_parser.cpp"
    break;

  case 302: /* expr: expr '-' expr  */
#line 1634 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6894 "ds_parser.cpp"
    break;

  case 303: /* expr: expr '*' expr  */
#line 1635 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6900 "ds_parser.cpp"
    break;

  case 304: /* expr: expr '/' expr  */
#line 1636 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6906 "ds_parser.cpp"
    break;

  case 305: /* expr: expr '%' expr  */
#line 1637 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6912 "ds_parser.cpp"
    break;

  case 306: /* expr: expr '<' expr  */
#line 1638 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6918 "ds_parser.cpp"
    break;

  case 307: /* expr: expr '>' expr  */
#line 1639 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6924 "ds_parser.cpp"
    break;

  case 308: /* expr: expr "==" expr  */
#line 1640 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6930 "ds_parser.cpp"
    break;

  case 309: /* expr: expr "!=" expr  */
#line 1641 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6936 "ds_parser.cpp"
    break;

  case 310: /* expr: expr "<=" expr  */
#line 1642 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6942 "ds_parser.cpp"
    break;

  case 311: /* expr: expr ">=" expr  */
#line 1643 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6948 "ds_parser.cpp"
    break;

  case 312: /* expr: expr '&' expr  */
#line 1644 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6954 "ds_parser.cpp"
    break;

  case 313: /* expr: expr '|' expr  */
#line 1645 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6960 "ds_parser.cpp"
    break;

  case 314: /* expr: expr '^' expr  */
#line 1646 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6966 "ds_parser.cpp"
    break;

  case 315: /* expr: expr "&&" expr  */
#line 1647 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6972 "ds_parser.cpp"
    break;

  case 316: /* expr: expr "||" expr  */
#line 1648 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6978 "ds_parser.cpp"
    break;

  case 317: /* expr: expr "^^" expr  */
#line 1649 "ds_parser.ypp"
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
#line 6984 "ds_parser.cpp"
    break;

  case 318: /* expr: "++" expr  */
#line 1650 "ds_parser.ypp"
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6990 "ds_parser.cpp"
    break;

  case 319: /* expr: "--" expr  */
#line 1651 "ds_parser.ypp"
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
#line 6996 "ds_parser.cpp"
    break;

  case 320: /* expr: expr "++"  */
#line 1652 "ds_parser.ypp"
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 7002 "ds_parser.cpp"
    break;

  case 321: /* expr: expr "--"  */
#line 1653 "ds_parser.ypp"
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
#line 7008 "ds_parser.cpp"
    break;

  case 322: /* expr: '(' expr ')'  */
#line 1654 "ds_parser.ypp"
                                                 { (yyval.pExpression) = (yyvsp[-1].pExpression); }
#line 7014 "ds_parser.cpp"
    break;

  case 323: /* expr: expr '[' expr ']'  */
#line 1655 "ds_parser.ypp"
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 7020 "ds_parser.cpp"
    break;

  case 324: /* expr: expr '.' '[' expr ']'  */
#line 1656 "ds_parser.ypp"
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
#line 7026 "ds_parser.cpp"
    break;

  case 325: /* expr: expr "?[" expr ']'  */
#line 1657 "ds_parser.ypp"
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
#line 7032 "ds_parser.cpp"
    break;

  case 326: /* expr: expr '.' "?[" expr ']'  */
#line 1658 "ds_parser.ypp"
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
#line 7038 "ds_parser.cpp"
    break;

  case 327: /* expr: expr "?." "name"  */
#line 1659 "ds_parser.ypp"
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
#line 7044 "ds_parser.cpp"
    break;

  case 328: /* expr: expr '.' "?." "name"  */
#line 1660 "ds_parser.ypp"
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
#line 7050 "ds_parser.cpp"
    break;

  case 329: /* expr: func_addr_expr  */
#line 1661 "ds_parser.ypp"
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7056 "ds_parser.cpp"
    break;

  case 330: /* expr: name_in_namespace '(' ')'  */
#line 1662 "ds_parser.ypp"
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
        }
#line 7065 "ds_parser.cpp"
    break;

  case 331: /* expr: name_in_namespace '(' expr_list ')'  */
#line 1666 "ds_parser.ypp"
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
        }
#line 7074 "ds_parser.cpp"
    break;

  case 332: /* expr: basic_type_declaration '(' ')'  */
#line 1670 "ds_parser.ypp"
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
#line 7082 "ds_parser.cpp"
    break;

  case 333: /* expr: basic_type_declaration '(' expr_list ')'  */
#line 1673 "ds_parser.ypp"
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
#line 7090 "ds_parser.cpp"
    break;

  case 334: /* expr: '*' expr  */
#line 1676 "ds_parser.ypp"
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
#line 7096 "ds_parser.cpp"
    break;

  case 335: /* expr: "deref" '(' expr ')'  */
#line 1677 "ds_parser.ypp"
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 7102 "ds_parser.cpp"
    break;

  case 336: /* expr: "addr" '(' expr ')'  */
#line 1678 "ds_parser.ypp"
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
#line 7108 "ds_parser.cpp"
    break;

  case 337: /* expr: "generator" '<' type_declaration '>' optional_capture_list '(' ')'  */
#line 1679 "ds_parser.ypp"
                                                                                                   {
        auto gen = new ExprMakeGenerator(tokAt(scanner,(yylsp[-6])));
        gen->iterType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        if ( (yyvsp[-2].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-2].pCaptList) );
            delete (yyvsp[-2].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 7122 "ds_parser.cpp"
    break;

  case 338: /* expr: "generator" '<' type_declaration '>' optional_capture_list '(' expr ')'  */
#line 1688 "ds_parser.ypp"
                                                                                                                 {
        auto gen = new ExprMakeGenerator(tokAt(scanner,(yylsp[-7])), ExpressionPtr((yyvsp[-1].pExpression)));
        gen->iterType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        if ( (yyvsp[-3].pCaptList) ) {
            swap ( gen->capture, *(yyvsp[-3].pCaptList) );
            delete (yyvsp[-3].pCaptList);
        }
        (yyval.pExpression) = gen;
    }
#line 7136 "ds_parser.cpp"
    break;

  case 339: /* expr: expr "??" expr  */
#line 1697 "ds_parser.ypp"
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
#line 7142 "ds_parser.cpp"
    break;

  case 340: /* expr: expr '?' expr ':' expr  */
#line 1698 "ds_parser.ypp"
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
#line 7150 "ds_parser.cpp"
    break;

  case 341: /* $@17: %empty  */
#line 1701 "ds_parser.ypp"
                                               { yyextra->das_arrow_depth ++; }
#line 7156 "ds_parser.cpp"
    break;

  case 342: /* $@18: %empty  */
#line 1701 "ds_parser.ypp"
                                                                                                           { yyextra->das_arrow_depth --; }
#line 7162 "ds_parser.cpp"
    break;

  case 343: /* expr: expr "is" "type" '<' $@17 type_declaration '>' $@18  */
#line 1701 "ds_parser.ypp"
                                                                                                                                            {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
#line 7170 "ds_parser.cpp"
    break;

  case 344: /* expr: expr "is" "name"  */
#line 1704 "ds_parser.ypp"
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 7179 "ds_parser.cpp"
    break;

  case 345: /* expr: expr "as" "name"  */
#line 1708 "ds_parser.ypp"
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 7188 "ds_parser.cpp"
    break;

  case 346: /* expr: expr '?' "as" "name"  */
#line 1712 "ds_parser.ypp"
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
#line 7197 "ds_parser.cpp"
    break;

  case 347: /* expr: expr_type_info  */
#line 1716 "ds_parser.ypp"
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7203 "ds_parser.cpp"
    break;

  case 348: /* expr: expr_type_decl  */
#line 1717 "ds_parser.ypp"
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7209 "ds_parser.cpp"
    break;

  case 349: /* expr: expr_cast  */
#line 1718 "ds_parser.ypp"
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7215 "ds_parser.cpp"
    break;

  case 350: /* expr: expr_new  */
#line 1719 "ds_parser.ypp"
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7221 "ds_parser.cpp"
    break;

  case 351: /* expr: expr_method_call  */
#line 1720 "ds_parser.ypp"
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7227 "ds_parser.cpp"
    break;

  case 352: /* expr: expr_named_call  */
#line 1721 "ds_parser.ypp"
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7233 "ds_parser.cpp"
    break;

  case 353: /* expr: expr_block  */
#line 1722 "ds_parser.ypp"
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 7239 "ds_parser.cpp"
    break;

  case 354: /* expr: expr "<|" expr  */
#line 1723 "ds_parser.ypp"
                                          {
        if ( (yyvsp[-2].pExpression)->rtti_isCallLikeExpr() ) {
            auto pCall = (ExprLooksLikeCall *) (yyvsp[-2].pExpression);
            pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        } else if ( (yyvsp[-2].pExpression)->rtti_isVar() ) {
            auto pVar = (ExprVar *) (yyvsp[-2].pExpression);
            auto pCall = yyextra->g_Program->makeCall(pVar->at,pVar->name);
            delete pVar;
            pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
            (yyval.pExpression) = pCall;
        } else {
            das_yyerror(scanner,"can only lpipe into a function call",tokAt(scanner,(yylsp[-1])),CompilationError::cant_pipe);
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        }
    }
#line 7260 "ds_parser.cpp"
    break;

  case 355: /* expr: expr "|>" expr  */
#line 1739 "ds_parser.ypp"
                                          {
        if ( (yyvsp[0].pExpression)->rtti_isCallLikeExpr() ) {
            auto pCall = (ExprLooksLikeCall *) (yyvsp[0].pExpression);
            pCall->arguments.insert(pCall->arguments.begin(),ExpressionPtr((yyvsp[-2].pExpression)));
            (yyval.pExpression) = (yyvsp[0].pExpression);
        } else if ( (yyvsp[0].pExpression)->rtti_isVar() ) {
            auto pVar = (ExprVar *) (yyvsp[0].pExpression);
            auto pCall = yyextra->g_Program->makeCall(pVar->at,pVar->name);
            delete pVar;
            pCall->arguments.insert(pCall->arguments.begin(),ExpressionPtr((yyvsp[-2].pExpression)));
            (yyval.pExpression) = pCall;
        } else {
            das_yyerror(scanner,"can only lpipe into a function call",tokAt(scanner,(yylsp[-1])),CompilationError::cant_pipe);
            (yyval.pExpression) = (yyvsp[0].pExpression);
        }
    }
#line 7281 "ds_parser.cpp"
    break;

  case 356: /* expr: name_in_namespace "name"  */
#line 1755 "ds_parser.ypp"
                                       {
        Enumeration * pEnum = nullptr;
        Expression * resConst = nullptr;
        auto enums = yyextra->g_Program->findEnum(*(yyvsp[-1].s));
        auto aliases = yyextra->g_Program->findAlias(*(yyvsp[-1].s));
        if ( enums.size()+aliases.size() > 1 ) {
            string candidates;
            if ( enums.size() ) candidates += yyextra->g_Program->describeCandidates(enums);
            if ( aliases.size() ) candidates += yyextra->g_Program->describeCandidates(aliases);
            das_yyerror(scanner,"too many options for the " + *(yyvsp[-1].s) + "\n" + candidates, tokAt(scanner,(yylsp[-1])),
                CompilationError::type_not_found);
        } else if ( enums.size()==0 && aliases.size()==0 ) {
            das_yyerror(scanner,"enumeration or bitfield not found " + *(yyvsp[-1].s), tokAt(scanner,(yylsp[-1])),
                CompilationError::type_not_found);
        } else if ( enums.size()==1 ) {
            pEnum = enums.back().get();
        } else if ( aliases.size()==1 ) {
            auto alias = aliases.back();
            if ( alias->isEnum() ) {
                pEnum = alias->enumType;
            } else if ( alias->isBitfield() ) {
                int bit = alias->findArgumentIndex(*(yyvsp[0].s));
                if ( bit!=-1 ) {
                    auto td = make_smart<TypeDecl>(*alias);
                    td->ref = false;
                    auto bitConst = new ExprConstBitfield(tokAt(scanner,(yylsp[0])), 1u << bit);
                    bitConst->bitfieldType = make_smart<TypeDecl>(*alias);
                    resConst = bitConst;
                } else {
                    das_yyerror(scanner,"enumeration or bitfield not found " + *(yyvsp[-1].s), tokAt(scanner,(yylsp[-1])),
                        CompilationError::bitfield_not_found);
                }
            } else {
                das_yyerror(scanner,"expecting enumeration or bitfield " + *(yyvsp[-1].s), tokAt(scanner,(yylsp[-1])),
                    CompilationError::syntax_error);
            }
        }
        if ( pEnum ) {
            auto ff = pEnum->find(*(yyvsp[0].s));
            if ( ff.second ) {
                auto td = make_smart<TypeDecl>(pEnum);
                resConst = new ExprConstEnumeration(tokAt(scanner,(yylsp[0])), *(yyvsp[0].s), td);
            } else {
                das_yyerror(scanner,"enumeraiton value not found " + *(yyvsp[-1].s) + " " + *(yyvsp[0].s), tokAt(scanner,(yylsp[0])),
                    CompilationError::enumeration_not_found);
            }
        }
        if ( resConst ) {
            (yyval.pExpression) = resConst;
        } else {
            (yyval.pExpression) = new ExprConstInt(0);   // dummy
        }
        delete (yyvsp[-1].s);
        delete (yyvsp[0].s);
    }
#line 7341 "ds_parser.cpp"
    break;

  case 357: /* expr: "unsafe" '(' expr ')'  */
#line 1810 "ds_parser.ypp"
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
#line 7351 "ds_parser.cpp"
    break;

  case 358: /* optional_field_annotation: %empty  */
#line 1818 "ds_parser.ypp"
                                                        { (yyval.aaList) = nullptr; }
#line 7357 "ds_parser.cpp"
    break;

  case 359: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
#line 1819 "ds_parser.ypp"
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); }
#line 7363 "ds_parser.cpp"
    break;

  case 360: /* optional_override: %empty  */
#line 1823 "ds_parser.ypp"
                      { (yyval.i) = OVERRIDE_NONE; }
#line 7369 "ds_parser.cpp"
    break;

  case 361: /* optional_override: "override"  */
#line 1824 "ds_parser.ypp"
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
#line 7375 "ds_parser.cpp"
    break;

  case 362: /* optional_override: "sealed"  */
#line 1825 "ds_parser.ypp"
                      { (yyval.i) = OVERRIDE_SEALED; }
#line 7381 "ds_parser.cpp"
    break;

  case 363: /* optional_constant: %empty  */
#line 1829 "ds_parser.ypp"
                        { (yyval.b) = false; }
#line 7387 "ds_parser.cpp"
    break;

  case 364: /* optional_constant: "const"  */
#line 1830 "ds_parser.ypp"
                        { (yyval.b) = true; }
#line 7393 "ds_parser.cpp"
    break;

  case 365: /* optional_public_or_private_member_variable: %empty  */
#line 1834 "ds_parser.ypp"
                        { (yyval.b) = false; }
#line 7399 "ds_parser.cpp"
    break;

  case 366: /* optional_public_or_private_member_variable: "public"  */
#line 1835 "ds_parser.ypp"
                        { (yyval.b) = false; }
#line 7405 "ds_parser.cpp"
    break;

  case 367: /* optional_public_or_private_member_variable: "private"  */
#line 1836 "ds_parser.ypp"
                        { (yyval.b) = true; }
#line 7411 "ds_parser.cpp"
    break;

  case 368: /* structure_variable_declaration: optional_field_annotation optional_override optional_public_or_private_member_variable variable_declaration  */
#line 1840 "ds_parser.ypp"
                                                                                                                                                 {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-3].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
#line 7423 "ds_parser.cpp"
    break;

  case 369: /* struct_variable_declaration_list: %empty  */
#line 1850 "ds_parser.ypp"
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7431 "ds_parser.cpp"
    break;

  case 370: /* struct_variable_declaration_list: struct_variable_declaration_list structure_variable_declaration ';'  */
#line 1853 "ds_parser.ypp"
                                                                                        {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        if ( (yyvsp[-1].pVarDecl) ) (yyvsp[-2].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
#line 7440 "ds_parser.cpp"
    break;

  case 371: /* $@19: %empty  */
#line 1858 "ds_parser.ypp"
                                                                                             { yyextra->das_force_oxford_comma=true; }
#line 7446 "ds_parser.cpp"
    break;

  case 372: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" $@19 function_declaration_header ';'  */
#line 1858 "ds_parser.ypp"
                                                                                                                                                                             {
        if ( !yyextra->g_thisStructure->isClass ) {
            das_yyerror(scanner,"structure can't have a member function",
                (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
        } else if ( (yyvsp[-1].pFuncDecl)->isGeneric() ) {
            das_yyerror(scanner,"generic function can't be a member of a class " + (yyvsp[-1].pFuncDecl)->getMangledName(),
                (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
        } else if ( (yyvsp[-1].pFuncDecl)->name==yyextra->g_thisStructure->name || (yyvsp[-1].pFuncDecl)->name=="finalize" ) {
            das_yyerror(scanner,"initializers and finalizers can't be abstract " + (yyvsp[-1].pFuncDecl)->getMangledName(),
                (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
        } else if ( (yyvsp[-6].faList)!=nullptr ) {
            das_yyerror(scanner,"abstract functions can't have annotations " + (yyvsp[-1].pFuncDecl)->getMangledName(),
                (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
            delete (yyvsp[-6].faList);
        } else if ( (yyvsp[-1].pFuncDecl)->result->baseType==Type::autoinfer ) {
            das_yyerror(scanner,"abstract functions must specify return type explicitly " + (yyvsp[-1].pFuncDecl)->getMangledName(),
                (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
        } else {
            auto varName = (yyvsp[-1].pFuncDecl)->name;
            (yyvsp[-1].pFuncDecl)->name = yyextra->g_thisStructure->name + "`" + (yyvsp[-1].pFuncDecl)->name;
            auto vars = new vector<VariableNameAndPosition>();
            vars->emplace_back(VariableNameAndPosition{varName,"",(yyvsp[-1].pFuncDecl)->at});
            TypeDecl * funcType = new TypeDecl(Type::tFunction);
            funcType->at = (yyvsp[-1].pFuncDecl)->at;
            swap ( funcType->firstType, (yyvsp[-1].pFuncDecl)->result );
            funcType->argTypes.reserve ( (yyvsp[-1].pFuncDecl)->arguments.size() );
            if ( yyextra->g_thisStructure->isClass ) {
                funcType->argTypes.push_back(make_smart<TypeDecl>(yyextra->g_thisStructure));
                funcType->argNames.push_back("self");
            }
            for ( auto & arg : (yyvsp[-1].pFuncDecl)->arguments ) {
                funcType->argTypes.push_back(arg->type);
                funcType->argNames.push_back(arg->name);
            }
            VariableDeclaration * decl = new VariableDeclaration(
                vars,
                funcType,
                nullptr
            );
            decl->isPrivate = (yyvsp[-4].b);
            (yyvsp[-7].pVarDeclList)->push_back(decl);
        }
        (yyvsp[-1].pFuncDecl)->delRef();
        (yyval.pVarDeclList) = (yyvsp[-7].pVarDeclList);
    }
#line 7496 "ds_parser.cpp"
    break;

  case 373: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_override optional_constant function_declaration_header expression_block  */
#line 1905 "ds_parser.ypp"
                                                                          {
        (yyvsp[-1].pFuncDecl)->atDecl = tokRangeAt(scanner,(yylsp[-5]),(yylsp[0]));
        (yyvsp[-1].pFuncDecl)->body = (yyvsp[0].pExpression);
        if ( !yyextra->g_thisStructure ) {
            das_yyerror(scanner,"internal error or invalid macro. member function is declared outside of a class",
                (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
        } else if ( !yyextra->g_thisStructure->isClass ) {
            das_yyerror(scanner,"structure can't have a member function",
                (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
        } else if ( (yyvsp[-1].pFuncDecl)->isGeneric() ) {
            das_yyerror(scanner,"generic function can't be a member of a class " + (yyvsp[-1].pFuncDecl)->getMangledName(),
                (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
        } else {
            (yyvsp[-1].pFuncDecl)->privateFunction = yyextra->g_thisStructure->privateStructure;
            if ( (yyvsp[-1].pFuncDecl)->name != yyextra->g_thisStructure->name && (yyvsp[-1].pFuncDecl)->name != "finalize") {
                auto varName = (yyvsp[-1].pFuncDecl)->name;
                (yyvsp[-1].pFuncDecl)->name = yyextra->g_thisStructure->name + "`" + (yyvsp[-1].pFuncDecl)->name;
                auto vars = new vector<VariableNameAndPosition>();
                vars->emplace_back(VariableNameAndPosition{varName,"",(yyvsp[-1].pFuncDecl)->at});
                Expression * finit = new ExprAddr((yyvsp[-1].pFuncDecl)->at, inThisModule((yyvsp[-1].pFuncDecl)->name));
                if ( (yyvsp[-3].i) == OVERRIDE_OVERRIDE ) {
                    finit = new ExprCast((yyvsp[-1].pFuncDecl)->at, finit, make_smart<TypeDecl>(Type::autoinfer));
                }
                VariableDeclaration * decl = new VariableDeclaration(
                    vars,
                    new TypeDecl(Type::autoinfer),
                    finit
                );
                decl->override = (yyvsp[-3].i) == OVERRIDE_OVERRIDE;
                decl->sealed = (yyvsp[-3].i) == OVERRIDE_SEALED;
                decl->isPrivate = (yyvsp[-4].b);
                (yyvsp[-7].pVarDeclList)->push_back(decl);
                modifyToClassMember((yyvsp[-1].pFuncDecl), yyextra->g_thisStructure, false, (yyvsp[-2].b));
            } else {
                if ( (yyvsp[-3].i) ) {
                    das_yyerror(scanner,"can't override an initializer or a finalizer " + (yyvsp[-1].pFuncDecl)->getMangledName(),
                        (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
                }
                if ( (yyvsp[-2].b) ) {
                    das_yyerror(scanner,"can't have a constant initializer or a finalizer " + (yyvsp[-1].pFuncDecl)->getMangledName(),
                        (yyvsp[-1].pFuncDecl)->at, CompilationError::invalid_member_function);
                }
                if ( (yyvsp[-1].pFuncDecl)->name!="finalize" ) {
                    auto ctr = makeClassConstructor(yyextra->g_thisStructure, (yyvsp[-1].pFuncDecl));
                    if ( !yyextra->g_Program->addFunction(ctr) ) {
                        das_yyerror(scanner,"intializer is already defined " + ctr->getMangledName(),
                            ctr->at, CompilationError::function_already_declared);
                    }
                    (yyvsp[-1].pFuncDecl)->name = yyextra->g_thisStructure->name + "`" + yyextra->g_thisStructure->name;
                    modifyToClassMember((yyvsp[-1].pFuncDecl), yyextra->g_thisStructure, false, false);
                } else {
                    modifyToClassMember((yyvsp[-1].pFuncDecl), yyextra->g_thisStructure, true, false);
                }
            }
            assignDefaultArguments((yyvsp[-1].pFuncDecl));
            runFunctionAnnotations(scanner, (yyvsp[-1].pFuncDecl), (yyvsp[-6].faList), tokAt(scanner,(yylsp[-6])));
            if ( !yyextra->g_Program->addFunction((yyvsp[-1].pFuncDecl)) ) {
                das_yyerror(scanner,"function is already defined " + (yyvsp[-1].pFuncDecl)->getMangledName(),
                    (yyvsp[-1].pFuncDecl)->at, CompilationError::function_already_declared);
            }
            (yyvsp[-1].pFuncDecl)->delRef();
        }
        (yyval.pVarDeclList) = (yyvsp[-7].pVarDeclList);
    }
#line 7565 "ds_parser.cpp"
    break;

  case 374: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' ';'  */
#line 1969 "ds_parser.ypp"
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
#line 7576 "ds_parser.cpp"
    break;

  case 375: /* function_argument_declaration: optional_field_annotation variable_declaration  */
#line 1978 "ds_parser.ypp"
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->constant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        }
#line 7586 "ds_parser.cpp"
    break;

  case 376: /* function_argument_declaration: optional_field_annotation "var" variable_declaration  */
#line 1983 "ds_parser.ypp"
                                                                           {
            (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
            (yyvsp[0].pVarDecl)->pTypeDecl->removeConstant = true;
            (yyvsp[0].pVarDecl)->annotation = (yyvsp[-2].aaList);
        }
#line 7596 "ds_parser.cpp"
    break;

  case 377: /* function_argument_list: function_argument_declaration  */
#line 1991 "ds_parser.ypp"
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7602 "ds_parser.cpp"
    break;

  case 378: /* function_argument_list: function_argument_list ';' function_argument_declaration  */
#line 1992 "ds_parser.ypp"
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7608 "ds_parser.cpp"
    break;

  case 379: /* tuple_type: type_declaration  */
#line 1996 "ds_parser.ypp"
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7616 "ds_parser.cpp"
    break;

  case 380: /* tuple_type: "name" ':' type_declaration  */
#line 1999 "ds_parser.ypp"
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7627 "ds_parser.cpp"
    break;

  case 381: /* tuple_type_list: tuple_type  */
#line 2008 "ds_parser.ypp"
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7633 "ds_parser.cpp"
    break;

  case 382: /* tuple_type_list: tuple_type_list ';' tuple_type  */
#line 2009 "ds_parser.ypp"
                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7639 "ds_parser.cpp"
    break;

  case 383: /* variant_type: "name" ':' type_declaration  */
#line 2013 "ds_parser.ypp"
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
#line 7650 "ds_parser.cpp"
    break;

  case 384: /* variant_type_list: variant_type  */
#line 2022 "ds_parser.ypp"
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7656 "ds_parser.cpp"
    break;

  case 385: /* variant_type_list: variant_type_list ';' variant_type  */
#line 2023 "ds_parser.ypp"
                                                         { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
#line 7662 "ds_parser.cpp"
    break;

  case 386: /* copy_or_move: '='  */
#line 2028 "ds_parser.ypp"
                    { (yyval.b) = false; }
#line 7668 "ds_parser.cpp"
    break;

  case 387: /* copy_or_move: "<-"  */
#line 2029 "ds_parser.ypp"
                    { (yyval.b) = true; }
#line 7674 "ds_parser.cpp"
    break;

  case 388: /* variable_declaration: variable_name_with_pos_list  */
#line 2033 "ds_parser.ypp"
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
#line 7685 "ds_parser.cpp"
    break;

  case 389: /* variable_declaration: variable_name_with_pos_list '&'  */
#line 2039 "ds_parser.ypp"
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
#line 7696 "ds_parser.cpp"
    break;

  case 390: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
#line 2045 "ds_parser.ypp"
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
#line 7704 "ds_parser.cpp"
    break;

  case 391: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
#line 2048 "ds_parser.ypp"
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7713 "ds_parser.cpp"
    break;

  case 392: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
#line 2052 "ds_parser.ypp"
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7724 "ds_parser.cpp"
    break;

  case 393: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
#line 2058 "ds_parser.ypp"
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
#line 7735 "ds_parser.cpp"
    break;

  case 394: /* copy_or_move_or_clone: '='  */
#line 2067 "ds_parser.ypp"
                    { (yyval.i) = CorM_COPY; }
#line 7741 "ds_parser.cpp"
    break;

  case 395: /* copy_or_move_or_clone: "<-"  */
#line 2068 "ds_parser.ypp"
                    { (yyval.i) = CorM_MOVE; }
#line 7747 "ds_parser.cpp"
    break;

  case 396: /* copy_or_move_or_clone: ":="  */
#line 2069 "ds_parser.ypp"
                    { (yyval.i) = CorM_CLONE; }
#line 7753 "ds_parser.cpp"
    break;

  case 397: /* optional_ref: %empty  */
#line 2073 "ds_parser.ypp"
            { (yyval.b) = false; }
#line 7759 "ds_parser.cpp"
    break;

  case 398: /* optional_ref: '&'  */
#line 2074 "ds_parser.ypp"
            { (yyval.b) = true; }
#line 7765 "ds_parser.cpp"
    break;

  case 399: /* let_variable_declaration: variable_name_with_pos_list ':' type_declaration ';'  */
#line 2078 "ds_parser.ypp"
                                                                             {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
#line 7773 "ds_parser.cpp"
    break;

  case 400: /* let_variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move_or_clone expr ';'  */
#line 2081 "ds_parser.ypp"
                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7783 "ds_parser.cpp"
    break;

  case 401: /* let_variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move_or_clone expr_pipe  */
#line 2086 "ds_parser.ypp"
                                                                                                                    {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7793 "ds_parser.cpp"
    break;

  case 402: /* let_variable_declaration: variable_name_with_pos_list optional_ref copy_or_move_or_clone expr ';'  */
#line 2091 "ds_parser.ypp"
                                                                                                      {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
#line 7806 "ds_parser.cpp"
    break;

  case 403: /* let_variable_declaration: variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
#line 2099 "ds_parser.ypp"
                                                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
#line 7819 "ds_parser.cpp"
    break;

  case 404: /* global_variable_declaration_list: %empty  */
#line 2110 "ds_parser.ypp"
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
#line 7827 "ds_parser.cpp"
    break;

  case 405: /* global_variable_declaration_list: global_variable_declaration_list optional_field_annotation let_variable_declaration  */
#line 2113 "ds_parser.ypp"
                                                                                                             {
        (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList);
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-1].aaList);
        (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl));
    }
#line 7837 "ds_parser.cpp"
    break;

  case 406: /* optional_shared: %empty  */
#line 2121 "ds_parser.ypp"
                     { (yyval.b) = false; }
#line 7843 "ds_parser.cpp"
    break;

  case 407: /* optional_shared: "shared"  */
#line 2122 "ds_parser.ypp"
                     { (yyval.b) = true; }
#line 7849 "ds_parser.cpp"
    break;

  case 408: /* optional_public_or_private_variable: %empty  */
#line 2126 "ds_parser.ypp"
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
#line 7855 "ds_parser.cpp"
    break;

  case 409: /* optional_public_or_private_variable: "private"  */
#line 2127 "ds_parser.ypp"
                     { (yyval.b) = false; }
#line 7861 "ds_parser.cpp"
    break;

  case 410: /* optional_public_or_private_variable: "public"  */
#line 2128 "ds_parser.ypp"
                     { (yyval.b) = true; }
#line 7867 "ds_parser.cpp"
    break;

  case 411: /* global_let: kwd_let optional_shared optional_public_or_private_variable '{' global_variable_declaration_list '}'  */
#line 2132 "ds_parser.ypp"
                                                                                                                                       {
        for ( auto pDecl : *(yyvsp[-1].pVarDeclList) ) {
            if ( pDecl->pTypeDecl ) {
                for ( const auto & name_at : *pDecl->pNameList ) {
                    VariablePtr pVar = make_smart<Variable>();
                    pVar->name = name_at.name;
                    pVar->aka = name_at.aka;
                    if ( !name_at.aka.empty() ) {
                        das_yyerror(scanner,"global variable " + name_at.name + " can't have an aka",name_at.at,
                            CompilationError::invalid_aka);
                    }
                    pVar->at = name_at.at;
                    pVar->type = make_smart<TypeDecl>(*pDecl->pTypeDecl);
                    if ( pDecl->pInit ) {
                        pVar->init = pDecl->pInit->clone();
                        pVar->init_via_move = pDecl->init_via_move;
                        pVar->init_via_clone = pDecl->init_via_clone;
                    }
                    if ( (yyvsp[-5].b) ) {
                        pVar->type->constant = true;
                    } else {
                        pVar->type->removeConstant = true;
                    }
                    pVar->global_shared = (yyvsp[-4].b);
                    pVar->private_variable = !(yyvsp[-3].b);
                    if ( !yyextra->g_Program->addVariable(pVar) )
                        das_yyerror(scanner,"global variable is already declared " + name_at.name,name_at.at,
                            CompilationError::global_variable_already_declared);
                }
            }
        }
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
#line 7905 "ds_parser.cpp"
    break;

  case 412: /* $@20: %empty  */
#line 2165 "ds_parser.ypp"
                                                                                        { yyextra->das_force_oxford_comma=true; }
#line 7911 "ds_parser.cpp"
    break;

  case 413: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@20 optional_field_annotation let_variable_declaration  */
#line 2165 "ds_parser.ypp"
                                                                                                                                                                                                {
        auto pDecl = (yyvsp[0].pVarDecl);
        if ( pDecl->pTypeDecl ) {
            for ( const auto & name_at : *pDecl->pNameList ) {
                VariablePtr pVar = make_smart<Variable>();
                pVar->name = name_at.name;
                pVar->aka = name_at.aka;
                if ( !name_at.aka.empty() ) {
                    das_yyerror(scanner,"global variable " + name_at.name + " can't have an aka",name_at.at,
                        CompilationError::invalid_aka);
                }
                pVar->at = name_at.at;
                pVar->type = make_smart<TypeDecl>(*pDecl->pTypeDecl);
                if ( pDecl->pInit ) {
                    pVar->init = pDecl->pInit->clone();
                    pVar->init_via_move = pDecl->init_via_move;
                    pVar->init_via_clone = pDecl->init_via_clone;
                }
                if ( (yyvsp[-5].b) ) {
                    pVar->type->constant = true;
                } else {
                    pVar->type->removeConstant = true;
                }
                pVar->global_shared = (yyvsp[-4].b);
                pVar->private_variable = !(yyvsp[-3].b);
                if ( (yyvsp[-1].aaList) ) {
                    pVar->annotation = move(*(yyvsp[-1].aaList));
                    delete (yyvsp[-1].aaList);
                }
                if ( !yyextra->g_Program->addVariable(pVar) )
                    das_yyerror(scanner,"global variable is already declared " + name_at.name,name_at.at,
                        CompilationError::global_variable_already_declared);
            }
        }
        delete pDecl;
    }
#line 7952 "ds_parser.cpp"
    break;

  case 414: /* enum_list: %empty  */
#line 2204 "ds_parser.ypp"
        {
        (yyval.pEnum) = new Enumeration();
    }
#line 7960 "ds_parser.cpp"
    break;

  case 415: /* enum_list: enum_list "name" ';'  */
#line 2207 "ds_parser.ypp"
                                     {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        if ( !(yyvsp[-2].pEnum)->add(*(yyvsp[-1].s),nullptr,tokAt(scanner,(yylsp[-1]))) ) {
            das_yyerror(scanner,"enumeration alread declared " + *(yyvsp[-1].s), tokAt(scanner,(yylsp[-1])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-1].s);
        (yyval.pEnum) = (yyvsp[-2].pEnum);
    }
#line 7974 "ds_parser.cpp"
    break;

  case 416: /* enum_list: enum_list "name" '=' expr ';'  */
#line 2216 "ds_parser.ypp"
                                                     {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt(scanner,(yylsp[-3]))) ) {
            das_yyerror(scanner,"enumeration value alread declared " + *(yyvsp[-3].s), tokAt(scanner,(yylsp[-3])),
                CompilationError::enumeration_value_already_declared);
        }
        delete (yyvsp[-3].s);
        (yyval.pEnum) = (yyvsp[-4].pEnum);
    }
#line 7988 "ds_parser.cpp"
    break;

  case 417: /* single_alias: "name" '=' type_declaration  */
#line 2229 "ds_parser.ypp"
                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-2].s);
        if ( !yyextra->g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
            das_yyerror(scanner,"type alias is already defined "+*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-2].s);
    }
#line 8002 "ds_parser.cpp"
    break;

  case 422: /* optional_public_or_private_enum: %empty  */
#line 2251 "ds_parser.ypp"
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
#line 8008 "ds_parser.cpp"
    break;

  case 423: /* optional_public_or_private_enum: "private"  */
#line 2252 "ds_parser.ypp"
                     { (yyval.b) = false; }
#line 8014 "ds_parser.cpp"
    break;

  case 424: /* optional_public_or_private_enum: "public"  */
#line 2253 "ds_parser.ypp"
                     { (yyval.b) = true; }
#line 8020 "ds_parser.cpp"
    break;

  case 425: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum "name" '{' enum_list '}'  */
#line 2257 "ds_parser.ypp"
                                                                                                                            {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        auto pEnum = EnumerationPtr((yyvsp[-1].pEnum));
        pEnum->at = tokAt(scanner,(yylsp[-3]));
        pEnum->name = *(yyvsp[-3].s);
        pEnum->isPrivate = !(yyvsp[-4].b);
        if ( (yyvsp[-6].faList) ) {
            for ( auto pA : *(yyvsp[-6].faList) ) {
                if ( pA->annotation ) {
                    if ( pA->annotation->rtti_isEnumerationAnnotation() ) {
                        auto ann = static_pointer_cast<EnumerationAnnotation>(pA->annotation);
                        string err;
                        if ( !ann->touch(pEnum, *yyextra->g_Program->thisModuleGroup, pA->arguments, err) ) {
                            das_yyerror(scanner,"macro [" +pA->annotation->name + "] failed to finalize the enumeration " + pEnum->name + "\n" + err, tokAt(scanner,(yylsp[-6])),
                                CompilationError::invalid_annotation);
                        }
                    }
                }
            }
            swap ( pEnum->annotations, *(yyvsp[-6].faList) );
            delete (yyvsp[-6].faList);
        }
        if ( !yyextra->g_Program->addEnumeration(pEnum) ) {
            das_yyerror(scanner,"enumeration is already defined "+*(yyvsp[-3].s),tokAt(scanner,(yylsp[-1])),
                CompilationError::enumeration_already_declared);
        }
        delete (yyvsp[-3].s);
    }
#line 8053 "ds_parser.cpp"
    break;

  case 426: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum "name" ':' enum_basic_type_declaration '{' enum_list '}'  */
#line 2285 "ds_parser.ypp"
                                                                                                                                                                 {
        das_checkName(scanner,*(yyvsp[-5].s),tokAt(scanner,(yylsp[-5])));
        auto pEnum = EnumerationPtr((yyvsp[-1].pEnum));
        pEnum->at = tokAt(scanner,(yylsp[-5]));
        pEnum->name = *(yyvsp[-5].s);
        pEnum->baseType = (yyvsp[-3].type);
        pEnum->isPrivate = !(yyvsp[-6].b);
        if ( (yyvsp[-8].faList) ) {
            for ( auto pA : *(yyvsp[-8].faList) ) {
                if ( pA->annotation ) {
                    if ( pA->annotation->rtti_isEnumerationAnnotation() ) {
                        auto ann = static_pointer_cast<EnumerationAnnotation>(pA->annotation);
                        string err;
                        if ( !ann->touch(pEnum, *yyextra->g_Program->thisModuleGroup, pA->arguments, err) ) {
                            das_yyerror(scanner,"macro [" +pA->annotation->name + "] failed to finalize the enumeration " +pEnum->name + "\n" + err, tokAt(scanner,(yylsp[-8])),
                                CompilationError::invalid_annotation);
                        }
                    }
                }
            }
            swap ( pEnum->annotations, *(yyvsp[-8].faList) );
            delete (yyvsp[-8].faList);
        }
        if ( !yyextra->g_Program->addEnumeration(pEnum) ) {
            das_yyerror(scanner,"enumeration is already defined "+*(yyvsp[-5].s),tokAt(scanner,(yylsp[-1])),
                CompilationError::enumeration_already_declared);
        }
        delete (yyvsp[-5].s);
    }
#line 8087 "ds_parser.cpp"
    break;

  case 427: /* optional_structure_parent: %empty  */
#line 2317 "ds_parser.ypp"
                                        { (yyval.s) = nullptr; }
#line 8093 "ds_parser.cpp"
    break;

  case 428: /* optional_structure_parent: ':' name_in_namespace  */
#line 2318 "ds_parser.ypp"
                                        { (yyval.s) = (yyvsp[0].s); }
#line 8099 "ds_parser.cpp"
    break;

  case 429: /* optional_sealed: %empty  */
#line 2322 "ds_parser.ypp"
                        { (yyval.b) = false; }
#line 8105 "ds_parser.cpp"
    break;

  case 430: /* optional_sealed: "sealed"  */
#line 2323 "ds_parser.ypp"
                        { (yyval.b) = true; }
#line 8111 "ds_parser.cpp"
    break;

  case 431: /* structure_name: optional_sealed "name" optional_structure_parent  */
#line 2327 "ds_parser.ypp"
                                                                           {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        StructurePtr pStruct;
        if ( (yyvsp[0].s) ) {
            auto structs = yyextra->g_Program->findStructure(*(yyvsp[0].s));
            if ( structs.size()==1 ) {
                pStruct = structs.back()->clone();
                pStruct->name = *(yyvsp[-1].s);
                pStruct->parent = structs.back().get();
                if ( pStruct->parent->sealed ) {
                    das_yyerror(scanner,"can't derive from a sealed class or structure "+*(yyvsp[0].s),tokAt(scanner,(yylsp[0])),
                        CompilationError::invalid_override);
                }
                pStruct->annotations.clear();
                pStruct->genCtor = false;
                pStruct->macroInterface = pStruct->parent && pStruct->parent->macroInterface;

            } else if ( structs.size()==0 ) {
                das_yyerror(scanner,"parent structure not found "+*(yyvsp[0].s),tokAt(scanner,(yylsp[0])),
                    CompilationError::structure_not_found);
            } else {
                string candidates = yyextra->g_Program->describeCandidates(structs);
                das_yyerror(scanner,"too many options for "+*(yyvsp[0].s) + "\n" + candidates,tokAt(scanner,(yylsp[0])),
                    CompilationError::structure_not_found);
            }
            delete (yyvsp[0].s);
        }
        if ( !pStruct ) {
            pStruct = make_smart<Structure>(*(yyvsp[-1].s));
        }
        pStruct->sealed = (yyvsp[-2].b);
        if ( !yyextra->g_Program->addStructure(pStruct) ) {
            das_yyerror(scanner,"structure is already defined "+*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),
                CompilationError::structure_already_declared);
            (yyval.pStructure) = nullptr;
        } else {
            (yyval.pStructure) = pStruct.get();
            yyextra->g_thisStructure = pStruct.get();
        }
        delete (yyvsp[-1].s);
    }
#line 8157 "ds_parser.cpp"
    break;

  case 432: /* class_or_struct: "class"  */
#line 2371 "ds_parser.ypp"
                    { (yyval.b) = true; }
#line 8163 "ds_parser.cpp"
    break;

  case 433: /* class_or_struct: "struct"  */
#line 2372 "ds_parser.ypp"
                    { (yyval.b) = false; }
#line 8169 "ds_parser.cpp"
    break;

  case 434: /* optional_public_or_private_structure: %empty  */
#line 2376 "ds_parser.ypp"
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
#line 8175 "ds_parser.cpp"
    break;

  case 435: /* optional_public_or_private_structure: "private"  */
#line 2377 "ds_parser.ypp"
                     { (yyval.b) = false; }
#line 8181 "ds_parser.cpp"
    break;

  case 436: /* optional_public_or_private_structure: "public"  */
#line 2378 "ds_parser.ypp"
                     { (yyval.b) = true; }
#line 8187 "ds_parser.cpp"
    break;

  case 437: /* $@21: %empty  */
#line 2383 "ds_parser.ypp"
        { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-2].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-1].b); } }
#line 8193 "ds_parser.cpp"
    break;

  case 438: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure structure_name $@21 '{' struct_variable_declaration_list '}'  */
#line 2383 "ds_parser.ypp"
                                                                                                                                  {
        if ( (yyvsp[-4].pStructure) ) {
            auto pStruct = (yyvsp[-4].pStructure);
            pStruct->at = tokAt(scanner,(yylsp[-4]));
            if ( pStruct->parent && pStruct->parent->isClass != pStruct->isClass ) {
                if ( pStruct->isClass ) {
                    das_yyerror(scanner,"class can only derive from a class", pStruct->at,
                        CompilationError::invalid_override);
                } else {
                    das_yyerror(scanner,"structure can only derive from a structure", pStruct->at,
                        CompilationError::invalid_override);
                }
            }
            if ( pStruct->isClass ) {
                makeClassRtti(pStruct);
                auto virtfin = makeClassFinalize(pStruct);
                if ( !yyextra->g_Program->addFunction(virtfin) ) {
                    das_yyerror(scanner,"built-in finalizer is already defined " + virtfin->getMangledName(),
                        virtfin->at, CompilationError::function_already_declared);
                }
            }
            for ( auto pDecl : *(yyvsp[-1].pVarDeclList) ) {
                for ( const auto & name_at : *pDecl->pNameList ) {
                    if ( !pStruct->isClass && pDecl->isPrivate ) {
                        das_yyerror(scanner,"only class member can be private "+name_at.name,name_at.at,
                            CompilationError::invalid_private);
                    }
                    auto oldFd = (Structure::FieldDeclaration *) pStruct->findField(name_at.name);
                    if ( !oldFd ) {
                        if ( pDecl->override ) {
                            das_yyerror(scanner,"structure field is not overriding anything "+name_at.name,name_at.at,
                                CompilationError::invalid_override);
                        } else {
                            auto td = make_smart<TypeDecl>(*pDecl->pTypeDecl);
                            auto init = pDecl->pInit ? ExpressionPtr(pDecl->pInit->clone()) : nullptr;
                            pStruct->fields.emplace_back(name_at.name, td, init,
                                pDecl->annotation ? *pDecl->annotation : AnnotationArgumentList(),
                                pDecl->init_via_move, name_at.at);
                            pStruct->fields.back().privateField = pDecl->isPrivate;
                            pStruct->fields.back().sealed = pDecl->sealed;
                        }
                    } else {
                        if ( pDecl->sealed || pDecl->override ) {
                            if ( oldFd->sealed ) {
                                das_yyerror(scanner,"structure field "+name_at.name+" is sealed",
                                    name_at.at, CompilationError::invalid_override);
                            }
                            auto init = pDecl->pInit ? ExpressionPtr(pDecl->pInit->clone()) : nullptr;
                            oldFd->init = init;
                            oldFd->parentType = oldFd->type->isAuto();
                            oldFd->privateField = pDecl->isPrivate;
                            oldFd->sealed = pDecl->sealed;
                        } else {
                            das_yyerror(scanner,"structure field is already declared "+name_at.name
                                +", use override to replace initial value instead",name_at.at,
                                    CompilationError::invalid_override);
                        }
                    }
                }
            }
            if ( (yyvsp[-7].faList) ) {
                for ( auto pA : *(yyvsp[-7].faList) ) {
                    if ( pA->annotation ) {
                        if ( pA->annotation->rtti_isStructureAnnotation() ) {
                            auto ann = static_pointer_cast<StructureAnnotation>(pA->annotation);
                            string err;
                            if ( !ann->touch(pStruct, *yyextra->g_Program->thisModuleGroup, pA->arguments, err) ) {
                                das_yyerror(scanner,"macro [" +pA->annotation->name + "] failed to apply to the structure " + pStruct->name + "\n" + err,
                                    tokAt(scanner,(yylsp[-6])), CompilationError::invalid_annotation);
                            }
                        } else if ( pA->annotation->rtti_isStructureTypeAnnotation() ) {
                            if ( (yyvsp[-7].faList)->size()!=1 ) {
                                das_yyerror(scanner,"structures are only allowed one structure macro", tokAt(scanner,(yylsp[-6])),
                                    CompilationError::invalid_annotation);
                            } else {
                                if ( !yyextra->g_Program->addStructureHandle(pStruct,
                                    static_pointer_cast<StructureTypeAnnotation>(pA->annotation), pA->arguments) ) {
                                        das_yyerror(scanner,"handled structure is already defined "+pStruct->name,tokAt(scanner,(yylsp[-6])),
                                        CompilationError::structure_already_declared);
                                } else {
                                    pStruct->module->removeStructure(pStruct);
                                }
                            }
                        }
                    }
                }
                swap ( pStruct->annotations, *(yyvsp[-7].faList) );
                delete (yyvsp[-7].faList);
            }
        }
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
        yyextra->g_thisStructure = nullptr;
    }
#line 8291 "ds_parser.cpp"
    break;

  case 439: /* variable_name_with_pos_list: "name"  */
#line 2479 "ds_parser.ypp"
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
#line 8303 "ds_parser.cpp"
    break;

  case 440: /* variable_name_with_pos_list: "name" "aka" "name"  */
#line 2486 "ds_parser.ypp"
                                         {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
#line 8317 "ds_parser.cpp"
    break;

  case 441: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
#line 2495 "ds_parser.ypp"
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
#line 8328 "ds_parser.cpp"
    break;

  case 442: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
#line 2501 "ds_parser.ypp"
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
#line 8341 "ds_parser.cpp"
    break;

  case 443: /* basic_type_declaration: "bool"  */
#line 2512 "ds_parser.ypp"
                        { (yyval.type) = Type::tBool; }
#line 8347 "ds_parser.cpp"
    break;

  case 444: /* basic_type_declaration: "string"  */
#line 2513 "ds_parser.ypp"
                        { (yyval.type) = Type::tString; }
#line 8353 "ds_parser.cpp"
    break;

  case 445: /* basic_type_declaration: "int"  */
#line 2514 "ds_parser.ypp"
                        { (yyval.type) = Type::tInt; }
#line 8359 "ds_parser.cpp"
    break;

  case 446: /* basic_type_declaration: "int8"  */
#line 2515 "ds_parser.ypp"
                        { (yyval.type) = Type::tInt8; }
#line 8365 "ds_parser.cpp"
    break;

  case 447: /* basic_type_declaration: "int16"  */
#line 2516 "ds_parser.ypp"
                        { (yyval.type) = Type::tInt16; }
#line 8371 "ds_parser.cpp"
    break;

  case 448: /* basic_type_declaration: "int64"  */
#line 2517 "ds_parser.ypp"
                        { (yyval.type) = Type::tInt64; }
#line 8377 "ds_parser.cpp"
    break;

  case 449: /* basic_type_declaration: "int2"  */
#line 2518 "ds_parser.ypp"
                        { (yyval.type) = Type::tInt2; }
#line 8383 "ds_parser.cpp"
    break;

  case 450: /* basic_type_declaration: "int3"  */
#line 2519 "ds_parser.ypp"
                        { (yyval.type) = Type::tInt3; }
#line 8389 "ds_parser.cpp"
    break;

  case 451: /* basic_type_declaration: "int4"  */
#line 2520 "ds_parser.ypp"
                        { (yyval.type) = Type::tInt4; }
#line 8395 "ds_parser.cpp"
    break;

  case 452: /* basic_type_declaration: "uint"  */
#line 2521 "ds_parser.ypp"
                        { (yyval.type) = Type::tUInt; }
#line 8401 "ds_parser.cpp"
    break;

  case 453: /* basic_type_declaration: "uint8"  */
#line 2522 "ds_parser.ypp"
                        { (yyval.type) = Type::tUInt8; }
#line 8407 "ds_parser.cpp"
    break;

  case 454: /* basic_type_declaration: "uint16"  */
#line 2523 "ds_parser.ypp"
                        { (yyval.type) = Type::tUInt16; }
#line 8413 "ds_parser.cpp"
    break;

  case 455: /* basic_type_declaration: "uint64"  */
#line 2524 "ds_parser.ypp"
                        { (yyval.type) = Type::tUInt64; }
#line 8419 "ds_parser.cpp"
    break;

  case 456: /* basic_type_declaration: "uint2"  */
#line 2525 "ds_parser.ypp"
                        { (yyval.type) = Type::tUInt2; }
#line 8425 "ds_parser.cpp"
    break;

  case 457: /* basic_type_declaration: "uint3"  */
#line 2526 "ds_parser.ypp"
                        { (yyval.type) = Type::tUInt3; }
#line 8431 "ds_parser.cpp"
    break;

  case 458: /* basic_type_declaration: "uint4"  */
#line 2527 "ds_parser.ypp"
                        { (yyval.type) = Type::tUInt4; }
#line 8437 "ds_parser.cpp"
    break;

  case 459: /* basic_type_declaration: "float"  */
#line 2528 "ds_parser.ypp"
                        { (yyval.type) = Type::tFloat; }
#line 8443 "ds_parser.cpp"
    break;

  case 460: /* basic_type_declaration: "float2"  */
#line 2529 "ds_parser.ypp"
                        { (yyval.type) = Type::tFloat2; }
#line 8449 "ds_parser.cpp"
    break;

  case 461: /* basic_type_declaration: "float3"  */
#line 2530 "ds_parser.ypp"
                        { (yyval.type) = Type::tFloat3; }
#line 8455 "ds_parser.cpp"
    break;

  case 462: /* basic_type_declaration: "float4"  */
#line 2531 "ds_parser.ypp"
                        { (yyval.type) = Type::tFloat4; }
#line 8461 "ds_parser.cpp"
    break;

  case 463: /* basic_type_declaration: "void"  */
#line 2532 "ds_parser.ypp"
                        { (yyval.type) = Type::tVoid; }
#line 8467 "ds_parser.cpp"
    break;

  case 464: /* basic_type_declaration: "range"  */
#line 2533 "ds_parser.ypp"
                        { (yyval.type) = Type::tRange; }
#line 8473 "ds_parser.cpp"
    break;

  case 465: /* basic_type_declaration: "urange"  */
#line 2534 "ds_parser.ypp"
                        { (yyval.type) = Type::tURange; }
#line 8479 "ds_parser.cpp"
    break;

  case 466: /* basic_type_declaration: "double"  */
#line 2535 "ds_parser.ypp"
                        { (yyval.type) = Type::tDouble; }
#line 8485 "ds_parser.cpp"
    break;

  case 467: /* basic_type_declaration: "bitfield"  */
#line 2536 "ds_parser.ypp"
                        { (yyval.type) = Type::tBitfield; }
#line 8491 "ds_parser.cpp"
    break;

  case 468: /* enum_basic_type_declaration: "int"  */
#line 2540 "ds_parser.ypp"
                        { (yyval.type) = Type::tInt; }
#line 8497 "ds_parser.cpp"
    break;

  case 469: /* enum_basic_type_declaration: "int8"  */
#line 2541 "ds_parser.ypp"
                        { (yyval.type) = Type::tInt8; }
#line 8503 "ds_parser.cpp"
    break;

  case 470: /* enum_basic_type_declaration: "int16"  */
#line 2542 "ds_parser.ypp"
                        { (yyval.type) = Type::tInt16; }
#line 8509 "ds_parser.cpp"
    break;

  case 471: /* enum_basic_type_declaration: "uint"  */
#line 2543 "ds_parser.ypp"
                        { (yyval.type) = Type::tUInt; }
#line 8515 "ds_parser.cpp"
    break;

  case 472: /* enum_basic_type_declaration: "uint8"  */
#line 2544 "ds_parser.ypp"
                        { (yyval.type) = Type::tUInt8; }
#line 8521 "ds_parser.cpp"
    break;

  case 473: /* enum_basic_type_declaration: "uint16"  */
#line 2545 "ds_parser.ypp"
                        { (yyval.type) = Type::tUInt16; }
#line 8527 "ds_parser.cpp"
    break;

  case 474: /* structure_type_declaration: name_in_namespace  */
#line 2549 "ds_parser.ypp"
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
#line 8540 "ds_parser.cpp"
    break;

  case 475: /* auto_type_declaration: "auto"  */
#line 2560 "ds_parser.ypp"
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
#line 8549 "ds_parser.cpp"
    break;

  case 476: /* auto_type_declaration: "auto" '(' "name" ')'  */
#line 2564 "ds_parser.ypp"
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
#line 8561 "ds_parser.cpp"
    break;

  case 477: /* bitfield_bits: "name"  */
#line 2574 "ds_parser.ypp"
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
#line 8573 "ds_parser.cpp"
    break;

  case 478: /* bitfield_bits: bitfield_bits ';' "name"  */
#line 2581 "ds_parser.ypp"
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
#line 8584 "ds_parser.cpp"
    break;

  case 479: /* $@22: %empty  */
#line 2590 "ds_parser.ypp"
                                     { yyextra->das_arrow_depth ++; }
#line 8590 "ds_parser.cpp"
    break;

  case 480: /* $@23: %empty  */
#line 2590 "ds_parser.ypp"
                                                                                            { yyextra->das_arrow_depth --; }
#line 8596 "ds_parser.cpp"
    break;

  case 481: /* bitfield_type_declaration: "bitfield" '<' $@22 bitfield_bits '>' $@23  */
#line 2590 "ds_parser.ypp"
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
#line 8611 "ds_parser.cpp"
    break;

  case 482: /* type_declaration: basic_type_declaration  */
#line 2603 "ds_parser.ypp"
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
#line 8617 "ds_parser.cpp"
    break;

  case 483: /* type_declaration: auto_type_declaration  */
#line 2604 "ds_parser.ypp"
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8623 "ds_parser.cpp"
    break;

  case 484: /* type_declaration: bitfield_type_declaration  */
#line 2605 "ds_parser.ypp"
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8629 "ds_parser.cpp"
    break;

  case 485: /* type_declaration: structure_type_declaration  */
#line 2606 "ds_parser.ypp"
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
#line 8635 "ds_parser.cpp"
    break;

  case 486: /* type_declaration: type_declaration '[' expr ']'  */
#line 2607 "ds_parser.ypp"
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
#line 8654 "ds_parser.cpp"
    break;

  case 487: /* type_declaration: type_declaration '[' ']'  */
#line 2621 "ds_parser.ypp"
                                           {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8665 "ds_parser.cpp"
    break;

  case 488: /* type_declaration: type_declaration '-' '[' ']'  */
#line 2627 "ds_parser.ypp"
                                               {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
#line 8674 "ds_parser.cpp"
    break;

  case 489: /* type_declaration: type_declaration "explicit"  */
#line 2631 "ds_parser.ypp"
                                                {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8683 "ds_parser.cpp"
    break;

  case 490: /* type_declaration: type_declaration "const"  */
#line 2635 "ds_parser.ypp"
                                             {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8693 "ds_parser.cpp"
    break;

  case 491: /* type_declaration: type_declaration '-' "const"  */
#line 2640 "ds_parser.ypp"
                                                 {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8703 "ds_parser.cpp"
    break;

  case 492: /* type_declaration: type_declaration '&'  */
#line 2645 "ds_parser.ypp"
                                       {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
#line 8713 "ds_parser.cpp"
    break;

  case 493: /* type_declaration: type_declaration '-' '&'  */
#line 2650 "ds_parser.ypp"
                                           {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8723 "ds_parser.cpp"
    break;

  case 494: /* type_declaration: type_declaration '#'  */
#line 2655 "ds_parser.ypp"
                                       {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
#line 8732 "ds_parser.cpp"
    break;

  case 495: /* type_declaration: type_declaration "implicit"  */
#line 2659 "ds_parser.ypp"
                                                {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
#line 8741 "ds_parser.cpp"
    break;

  case 496: /* type_declaration: type_declaration '-' '#'  */
#line 2663 "ds_parser.ypp"
                                           {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8751 "ds_parser.cpp"
    break;

  case 497: /* type_declaration: type_declaration '=' "const"  */
#line 2668 "ds_parser.ypp"
                                                 {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
#line 8760 "ds_parser.cpp"
    break;

  case 498: /* type_declaration: type_declaration '?'  */
#line 2672 "ds_parser.ypp"
                                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8770 "ds_parser.cpp"
    break;

  case 499: /* $@24: %empty  */
#line 2677 "ds_parser.ypp"
                               { yyextra->das_arrow_depth ++; }
#line 8776 "ds_parser.cpp"
    break;

  case 500: /* $@25: %empty  */
#line 2677 "ds_parser.ypp"
                                                                                               { yyextra->das_arrow_depth --; }
#line 8782 "ds_parser.cpp"
    break;

  case 501: /* type_declaration: "smart_ptr" '<' $@24 type_declaration '>' $@25  */
#line 2677 "ds_parser.ypp"
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8793 "ds_parser.cpp"
    break;

  case 502: /* type_declaration: type_declaration "??"  */
#line 2683 "ds_parser.ypp"
                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
#line 8805 "ds_parser.cpp"
    break;

  case 503: /* $@26: %empty  */
#line 2690 "ds_parser.ypp"
                           { yyextra->das_arrow_depth ++; }
#line 8811 "ds_parser.cpp"
    break;

  case 504: /* $@27: %empty  */
#line 2690 "ds_parser.ypp"
                                                                                           { yyextra->das_arrow_depth --; }
#line 8817 "ds_parser.cpp"
    break;

  case 505: /* type_declaration: "array" '<' $@26 type_declaration '>' $@27  */
#line 2690 "ds_parser.ypp"
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8827 "ds_parser.cpp"
    break;

  case 506: /* $@28: %empty  */
#line 2695 "ds_parser.ypp"
                           { yyextra->das_arrow_depth ++; }
#line 8833 "ds_parser.cpp"
    break;

  case 507: /* $@29: %empty  */
#line 2695 "ds_parser.ypp"
                                                                                                                                  { yyextra->das_arrow_depth --; }
#line 8839 "ds_parser.cpp"
    break;

  case 508: /* type_declaration: "table" '<' $@28 type_declaration ';' type_declaration '>' $@29  */
#line 2695 "ds_parser.ypp"
                                                                                                                                                                   {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-7]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8850 "ds_parser.cpp"
    break;

  case 509: /* $@30: %empty  */
#line 2701 "ds_parser.ypp"
                               { yyextra->das_arrow_depth ++; }
#line 8856 "ds_parser.cpp"
    break;

  case 510: /* $@31: %empty  */
#line 2701 "ds_parser.ypp"
                                                                                                 { yyextra->das_arrow_depth --; }
#line 8862 "ds_parser.cpp"
    break;

  case 511: /* type_declaration: "iterator" '<' $@30 type_declaration '>' $@31  */
#line 2701 "ds_parser.ypp"
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8872 "ds_parser.cpp"
    break;

  case 512: /* type_declaration: "block"  */
#line 2706 "ds_parser.ypp"
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
#line 8881 "ds_parser.cpp"
    break;

  case 513: /* $@32: %empty  */
#line 2710 "ds_parser.ypp"
                             { yyextra->das_arrow_depth ++; }
#line 8887 "ds_parser.cpp"
    break;

  case 514: /* $@33: %empty  */
#line 2710 "ds_parser.ypp"
                                                                                              { yyextra->das_arrow_depth --; }
#line 8893 "ds_parser.cpp"
    break;

  case 515: /* type_declaration: "block" '<' $@32 type_declaration '>' $@33  */
#line 2710 "ds_parser.ypp"
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8903 "ds_parser.cpp"
    break;

  case 516: /* $@34: %empty  */
#line 2715 "ds_parser.ypp"
                             { yyextra->das_arrow_depth ++; }
#line 8909 "ds_parser.cpp"
    break;

  case 517: /* $@35: %empty  */
#line 2715 "ds_parser.ypp"
                                                                                                                                       { yyextra->das_arrow_depth --; }
#line 8915 "ds_parser.cpp"
    break;

  case 518: /* type_declaration: "block" '<' $@34 optional_function_argument_list optional_function_type '>' $@35  */
#line 2715 "ds_parser.ypp"
                                                                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8929 "ds_parser.cpp"
    break;

  case 519: /* type_declaration: "function"  */
#line 2724 "ds_parser.ypp"
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
#line 8938 "ds_parser.cpp"
    break;

  case 520: /* $@36: %empty  */
#line 2728 "ds_parser.ypp"
                               { yyextra->das_arrow_depth ++; }
#line 8944 "ds_parser.cpp"
    break;

  case 521: /* $@37: %empty  */
#line 2728 "ds_parser.ypp"
                                                                                                { yyextra->das_arrow_depth --; }
#line 8950 "ds_parser.cpp"
    break;

  case 522: /* type_declaration: "function" '<' $@36 type_declaration '>' $@37  */
#line 2728 "ds_parser.ypp"
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 8960 "ds_parser.cpp"
    break;

  case 523: /* $@38: %empty  */
#line 2733 "ds_parser.ypp"
                               { yyextra->das_arrow_depth ++; }
#line 8966 "ds_parser.cpp"
    break;

  case 524: /* $@39: %empty  */
#line 2733 "ds_parser.ypp"
                                                                                                                                         { yyextra->das_arrow_depth --; }
#line 8972 "ds_parser.cpp"
    break;

  case 525: /* type_declaration: "function" '<' $@38 optional_function_argument_list optional_function_type '>' $@39  */
#line 2733 "ds_parser.ypp"
                                                                                                                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 8986 "ds_parser.cpp"
    break;

  case 526: /* type_declaration: "lambda"  */
#line 2742 "ds_parser.ypp"
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
#line 8995 "ds_parser.cpp"
    break;

  case 527: /* $@40: %empty  */
#line 2746 "ds_parser.ypp"
                             { yyextra->das_arrow_depth ++; }
#line 9001 "ds_parser.cpp"
    break;

  case 528: /* $@41: %empty  */
#line 2746 "ds_parser.ypp"
                                                                                              { yyextra->das_arrow_depth --; }
#line 9007 "ds_parser.cpp"
    break;

  case 529: /* type_declaration: "lambda" '<' $@40 type_declaration '>' $@41  */
#line 2746 "ds_parser.ypp"
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
#line 9017 "ds_parser.cpp"
    break;

  case 530: /* $@42: %empty  */
#line 2751 "ds_parser.ypp"
                             { yyextra->das_arrow_depth ++; }
#line 9023 "ds_parser.cpp"
    break;

  case 531: /* $@43: %empty  */
#line 2751 "ds_parser.ypp"
                                                                                                                                       { yyextra->das_arrow_depth --; }
#line 9029 "ds_parser.cpp"
    break;

  case 532: /* type_declaration: "lambda" '<' $@42 optional_function_argument_list optional_function_type '>' $@43  */
#line 2751 "ds_parser.ypp"
                                                                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
#line 9043 "ds_parser.cpp"
    break;

  case 533: /* $@44: %empty  */
#line 2760 "ds_parser.ypp"
                            { yyextra->das_arrow_depth ++; }
#line 9049 "ds_parser.cpp"
    break;

  case 534: /* $@45: %empty  */
#line 2760 "ds_parser.ypp"
                                                                                       { yyextra->das_arrow_depth --; }
#line 9055 "ds_parser.cpp"
    break;

  case 535: /* type_declaration: "tuple" '<' $@44 tuple_type_list '>' $@45  */
#line 2760 "ds_parser.ypp"
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 9066 "ds_parser.cpp"
    break;

  case 536: /* $@46: %empty  */
#line 2766 "ds_parser.ypp"
                              { yyextra->das_arrow_depth ++; }
#line 9072 "ds_parser.cpp"
    break;

  case 537: /* $@47: %empty  */
#line 2766 "ds_parser.ypp"
                                                                                           { yyextra->das_arrow_depth --; }
#line 9078 "ds_parser.cpp"
    break;

  case 538: /* type_declaration: "variant" '<' $@46 variant_type_list '>' $@47  */
#line 2766 "ds_parser.ypp"
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
#line 9089 "ds_parser.cpp"
    break;

  case 539: /* $@48: %empty  */
#line 2775 "ds_parser.ypp"
                     { yyextra->das_need_oxford_comma=false; }
#line 9095 "ds_parser.cpp"
    break;

  case 540: /* variant_alias_declaration: "variant" $@48 "name" '{' variant_type_list ';' '}'  */
#line 2775 "ds_parser.ypp"
                                                                                                               {
        auto vtype = make_smart<TypeDecl>(Type::tVariant);
        vtype->alias = *(yyvsp[-4].s);
        vtype->at = tokAt(scanner,(yylsp[-4]));
        varDeclToTypeDecl(scanner, vtype.get(), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        if ( !yyextra->g_Program->addAlias(vtype) ) {
            das_yyerror(scanner,"type alias is already defined "+*(yyvsp[-4].s),tokAt(scanner,(yylsp[-4])),
                CompilationError::type_alias_already_declared);
        }
        delete (yyvsp[-4].s);
    }
#line 9112 "ds_parser.cpp"
    break;

  case 541: /* $@49: %empty  */
#line 2790 "ds_parser.ypp"
                      { yyextra->das_need_oxford_comma=false; }
#line 9118 "ds_parser.cpp"
    break;

  case 542: /* bitfield_alias_declaration: "bitfield" $@49 "name" '{' bitfield_bits ';' '}'  */
#line 2790 "ds_parser.ypp"
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
#line 9139 "ds_parser.cpp"
    break;

  case 543: /* make_decl: make_struct_decl  */
#line 2810 "ds_parser.ypp"
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 9145 "ds_parser.cpp"
    break;

  case 544: /* make_decl: make_dim_decl  */
#line 2811 "ds_parser.ypp"
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 9151 "ds_parser.cpp"
    break;

  case 545: /* make_decl: make_table_decl  */
#line 2812 "ds_parser.ypp"
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 9157 "ds_parser.cpp"
    break;

  case 546: /* make_decl: array_comprehension  */
#line 2813 "ds_parser.ypp"
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 9163 "ds_parser.cpp"
    break;

  case 547: /* make_struct_fields: "name" copy_or_move expr  */
#line 2817 "ds_parser.ypp"
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 9175 "ds_parser.cpp"
    break;

  case 548: /* make_struct_fields: "name" ":=" expr  */
#line 2824 "ds_parser.ypp"
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
#line 9187 "ds_parser.cpp"
    break;

  case 549: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
#line 2831 "ds_parser.ypp"
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 9198 "ds_parser.cpp"
    break;

  case 550: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
#line 2837 "ds_parser.ypp"
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
#line 9209 "ds_parser.cpp"
    break;

  case 551: /* make_struct_dim: make_struct_fields  */
#line 2846 "ds_parser.ypp"
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
#line 9219 "ds_parser.cpp"
    break;

  case 552: /* make_struct_dim: make_struct_dim ';' make_struct_fields  */
#line 2851 "ds_parser.ypp"
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 9228 "ds_parser.cpp"
    break;

  case 553: /* optional_block: %empty  */
#line 2858 "ds_parser.ypp"
        { (yyval.pExpression) = nullptr; }
#line 9234 "ds_parser.cpp"
    break;

  case 554: /* optional_block: "where" expr_block  */
#line 2859 "ds_parser.ypp"
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 9240 "ds_parser.cpp"
    break;

  case 555: /* make_struct_decl: "[[" type_declaration make_struct_dim optional_block ']' ']'  */
#line 2863 "ds_parser.ypp"
                                                                                             {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 9251 "ds_parser.cpp"
    break;

  case 556: /* make_struct_decl: "[[" type_declaration optional_block ']' ']'  */
#line 2869 "ds_parser.ypp"
                                                                        {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = msd;
    }
#line 9263 "ds_parser.cpp"
    break;

  case 557: /* make_struct_decl: "[[" type_declaration '(' ')' optional_block ']' ']'  */
#line 2876 "ds_parser.ypp"
                                                                                {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-2].pExpression);
        msd->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = msd;
    }
#line 9276 "ds_parser.cpp"
    break;

  case 558: /* make_struct_decl: "[[" type_declaration '(' ')' make_struct_dim optional_block ']' ']'  */
#line 2884 "ds_parser.ypp"
                                                                                                     {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-7]));
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
#line 9288 "ds_parser.cpp"
    break;

  case 559: /* make_struct_decl: "[{" type_declaration make_struct_dim optional_block '}' ']'  */
#line 2891 "ds_parser.ypp"
                                                                                             {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-5]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 9301 "ds_parser.cpp"
    break;

  case 560: /* make_struct_decl: "[{" type_declaration '(' ')' make_struct_dim optional_block '}' ']'  */
#line 2899 "ds_parser.ypp"
                                                                                                     {
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-3].pExpression))->block = (yyvsp[-2].pExpression);
        (yyvsp[-3].pExpression)->at = tokAt(scanner,(yylsp[-7]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-7])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-3].pExpression)));
        (yyval.pExpression) = tam;
    }
#line 9315 "ds_parser.cpp"
    break;

  case 561: /* make_tuple: expr  */
#line 2911 "ds_parser.ypp"
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
#line 9323 "ds_parser.cpp"
    break;

  case 562: /* make_tuple: make_tuple ',' expr  */
#line 2914 "ds_parser.ypp"
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
#line 9339 "ds_parser.cpp"
    break;

  case 563: /* make_map_tuple: expr "=>" expr  */
#line 2928 "ds_parser.ypp"
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
#line 9350 "ds_parser.cpp"
    break;

  case 564: /* make_any_tuple: make_tuple  */
#line 2937 "ds_parser.ypp"
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 9356 "ds_parser.cpp"
    break;

  case 565: /* make_any_tuple: make_map_tuple  */
#line 2938 "ds_parser.ypp"
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 9362 "ds_parser.cpp"
    break;

  case 566: /* make_dim: make_any_tuple  */
#line 2942 "ds_parser.ypp"
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 9372 "ds_parser.cpp"
    break;

  case 567: /* make_dim: make_dim ';' make_any_tuple  */
#line 2947 "ds_parser.ypp"
                                              {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 9381 "ds_parser.cpp"
    break;

  case 568: /* make_dim_decl: "[[" type_declaration make_dim ']' ']'  */
#line 2954 "ds_parser.ypp"
                                                                  {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 9391 "ds_parser.cpp"
    break;

  case 569: /* make_dim_decl: "[{" type_declaration make_dim '}' ']'  */
#line 2959 "ds_parser.ypp"
                                                                  {
       ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
       (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
       auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
       tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
       (yyval.pExpression) = tam;
    }
#line 9403 "ds_parser.cpp"
    break;

  case 570: /* make_table: make_map_tuple  */
#line 2969 "ds_parser.ypp"
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
#line 9413 "ds_parser.cpp"
    break;

  case 571: /* make_table: make_table ';' make_map_tuple  */
#line 2974 "ds_parser.ypp"
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
#line 9422 "ds_parser.cpp"
    break;

  case 572: /* make_table_decl: "{{" make_table '}' '}'  */
#line 2981 "ds_parser.ypp"
                                              {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-2].pExpression))->makeType = mkt;
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = ttm;
    }
#line 9436 "ds_parser.cpp"
    break;

  case 573: /* array_comprehension_where: %empty  */
#line 2993 "ds_parser.ypp"
                                    { (yyval.pExpression) = nullptr; }
#line 9442 "ds_parser.cpp"
    break;

  case 574: /* array_comprehension_where: ';' "where" expr  */
#line 2994 "ds_parser.ypp"
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
#line 9448 "ds_parser.cpp"
    break;

  case 575: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where ']' ']'  */
#line 2998 "ds_parser.ypp"
                                                                                                                                                            {
        auto pFor = make_smart<ExprFor>(tokAt(scanner,(yylsp[-8])));
        pFor->visibility = tokRangeAt(scanner,(yylsp[-3]),(yylsp[0]));
        for ( const auto & np : *(yyvsp[-7].pNameWithPosList) ) {
            pFor->iterators.push_back(np.name);
            pFor->iteratorsAka.push_back(np.aka);
            if ( !np.aka.empty() ) {
                das_yyerror(scanner,"array comprehension can't have an aka",np.at,
                    CompilationError::invalid_aka);
            }
            pFor->iteratorsAt.push_back(np.at);
        }
        delete (yyvsp[-7].pNameWithPosList);
        pFor->sources = sequenceToList((yyvsp[-5].pExpression));
        auto pAC = new ExprArrayComprehension(tokAt(scanner,(yylsp[-8])));
        pAC->generatorSyntax = true;
        pAC->exprFor = pFor;
        pAC->subexpr = ExpressionPtr((yyvsp[-3].pExpression));
        if ( (yyvsp[-2].pExpression) ) {
            pAC->exprWhere = ExpressionPtr((yyvsp[-2].pExpression));
        }
        (yyval.pExpression) = pAC;
    }
#line 9476 "ds_parser.cpp"
    break;

  case 576: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list ';' expr array_comprehension_where '}' ']'  */
#line 3021 "ds_parser.ypp"
                                                                                                                                                            {
        auto pFor = make_smart<ExprFor>(tokAt(scanner,(yylsp[-8])));
        pFor->visibility = tokRangeAt(scanner,(yylsp[-3]),(yylsp[0]));
        for ( const auto & np : *(yyvsp[-7].pNameWithPosList) ) {
            pFor->iterators.push_back(np.name);
            pFor->iteratorsAka.push_back(np.aka);
            if ( !np.aka.empty() ) {
                das_yyerror(scanner,"array comprehension can't have an aka",np.at,
                    CompilationError::invalid_aka);
            }
            pFor->iteratorsAt.push_back(np.at);
        }
        delete (yyvsp[-7].pNameWithPosList);
        pFor->sources = sequenceToList((yyvsp[-5].pExpression));
        auto pAC = new ExprArrayComprehension(tokAt(scanner,(yylsp[-8])));
        pAC->generatorSyntax = false;
        pAC->exprFor = pFor;
        pAC->subexpr = ExpressionPtr((yyvsp[-3].pExpression));
        if ( (yyvsp[-2].pExpression) ) {
            pAC->exprWhere = ExpressionPtr((yyvsp[-2].pExpression));
        }
        (yyval.pExpression) = pAC;
    }
#line 9504 "ds_parser.cpp"
    break;


#line 9508 "ds_parser.cpp"

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
          goto yyexhaustedlab;
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
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if 1
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, scanner, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
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

#line 3046 "ds_parser.ypp"


void das_checkName ( yyscan_t scanner, const string & name, const LineInfo &at ) {
    if ( name.length()>=2 && name[0]=='_' && name[1]=='_' ) {
        yyextra->g_Program->error("names starting with __ are reserved, " + name,"","",at,CompilationError::invalid_name);
    }
}

void das_yyerror ( yyscan_t scanner, const string & error, const LineInfo & at, CompilationError cerr ) {
    yyextra->g_Program->error(error,"","",at,cerr);
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

vector<ExpressionPtr> sequenceToList ( Expression * arguments ) {
    vector<ExpressionPtr> argList;
    auto arg = arguments;
    if ( arg->rtti_isSequence() ) {
        while ( arg->rtti_isSequence() ) {
            auto pSeq = static_cast<ExprSequence *>(arg);
            DAS_ASSERT(!pSeq->right->rtti_isSequence());
            argList.push_back(pSeq->right);
            arg = pSeq->left.get();
        }
        argList.push_back(arg);
        reverse(argList.begin(),argList.end());
        delete arguments;
    } else {
        argList.push_back(ExpressionPtr(arg));
    }
    return argList;
}

ExprLooksLikeCall * parseFunctionArguments ( ExprLooksLikeCall * pCall, Expression * arguments ) {
    pCall->arguments = sequenceToList(arguments);
    return pCall;
}

void deleteVariableDeclarationList ( vector<VariableDeclaration *> * list ) {
    if ( !list ) return;
    for ( auto pD : *list )
        delete pD;
    delete list;
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

Annotation * findAnnotation ( yyscan_t scanner, const string & name, const LineInfo & at ) {
    auto ann = yyextra->g_Program->findAnnotation(name);
    if ( ann.size()==1 ) {
        return ann.back().get();
    } else if ( ann.size()==0 ) {
        das_yyerror(scanner,"annotation " + name + " not found", at, CompilationError::annotation_not_found );
        return nullptr;
    } else {
        string candidates = yyextra->g_Program->describeCandidates(ann);
        das_yyerror(scanner,"too many options for annotation " + name + "\n" + candidates, at, CompilationError::annotation_not_found );
        return nullptr;
    }
}

void varDeclToTypeDecl ( yyscan_t scanner, TypeDecl * pType, vector<VariableDeclaration*> * list, bool needNames ) {
    bool anyNames = false;
    for ( auto pDecl : *list ) {
        if ( pDecl->pTypeDecl ) {
            int count = pDecl->pNameList ? int(pDecl->pNameList->size()) : 1;
            for ( int ai=0; ai!=count; ++ai ) {
                auto pVarType = make_smart<TypeDecl>(*pDecl->pTypeDecl);
                if ( pDecl->pInit ) {
                    das_yyerror(scanner,"can't have default values in a type declaration",
                        (*pDecl->pNameList)[ai].at,CompilationError::cant_initialize);
                }
                pType->argTypes.push_back(pVarType);
                if ( needNames && pDecl->pNameList && !(*pDecl->pNameList)[ai].name.empty() ) {
                    if ( !(*pDecl->pNameList)[ai].aka.empty() ) {
                        das_yyerror(scanner,"type declaration can't have an aka", (*pDecl->pNameList)[ai].at,
                            CompilationError::invalid_aka);
                    }
                    anyNames = true;
                }
            }
        }
    }
    if ( anyNames ) {
        for ( auto pDecl : *list ) {
            if ( pDecl->pTypeDecl ) {
                if ( pDecl->pNameList ) {
                    for ( const auto & name : *pDecl->pNameList ) {
                        pType->argNames.push_back(name.name);
                    }
                } else {
                    pType->argNames.push_back(string());
                }
            }
        }
    }
}

void runFunctionAnnotations ( yyscan_t scanner, Function * func, AnnotationList * annL, const LineInfo & at ) {
    if ( annL ) {
        for ( const auto & pA : *annL ) {
            if ( pA->annotation ) {
                if ( pA->annotation->rtti_isFunctionAnnotation() ) {
                    auto ann = static_pointer_cast<FunctionAnnotation>(pA->annotation);
                    string err;
                    if ( !ann->apply(func, *yyextra->g_Program->thisModuleGroup, pA->arguments, err) ) {
                        das_yyerror(scanner,"macro [" +pA->annotation->name + "] failed to apply to a function " + func->name + "\n" + err, at,
                            CompilationError::invalid_annotation);
                    }
                } else {
                    das_yyerror(scanner,"functions are only allowed function macros",
                        at, CompilationError::invalid_annotation);
                }
            }
        }
        swap ( func->annotations, *annL );
        for ( const auto & pA : *annL ) {
            func->annotations.push_back(pA);
        }
        delete annL;
    }
}
