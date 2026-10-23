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
  YYSYMBOL_DAS_TFLOAT16 = 102,             /* "float16"  */
  YYSYMBOL_DAS_THALF2 = 103,               /* "half2"  */
  YYSYMBOL_DAS_THALF3 = 104,               /* "half3"  */
  YYSYMBOL_DAS_THALF4 = 105,               /* "half4"  */
  YYSYMBOL_DAS_THALF8 = 106,               /* "half8"  */
  YYSYMBOL_DAS_TSHORT2 = 107,              /* "short2"  */
  YYSYMBOL_DAS_TSHORT3 = 108,              /* "short3"  */
  YYSYMBOL_DAS_TSHORT4 = 109,              /* "short4"  */
  YYSYMBOL_DAS_TSHORT8 = 110,              /* "short8"  */
  YYSYMBOL_DAS_TUSHORT2 = 111,             /* "ushort2"  */
  YYSYMBOL_DAS_TUSHORT3 = 112,             /* "ushort3"  */
  YYSYMBOL_DAS_TUSHORT4 = 113,             /* "ushort4"  */
  YYSYMBOL_DAS_TUSHORT8 = 114,             /* "ushort8"  */
  YYSYMBOL_DAS_TBYTE2 = 115,               /* "byte2"  */
  YYSYMBOL_DAS_TBYTE3 = 116,               /* "byte3"  */
  YYSYMBOL_DAS_TBYTE4 = 117,               /* "byte4"  */
  YYSYMBOL_DAS_TBYTE8 = 118,               /* "byte8"  */
  YYSYMBOL_DAS_TBYTE16 = 119,              /* "byte16"  */
  YYSYMBOL_DAS_TUBYTE2 = 120,              /* "ubyte2"  */
  YYSYMBOL_DAS_TUBYTE3 = 121,              /* "ubyte3"  */
  YYSYMBOL_DAS_TUBYTE4 = 122,              /* "ubyte4"  */
  YYSYMBOL_DAS_TUBYTE8 = 123,              /* "ubyte8"  */
  YYSYMBOL_DAS_TUBYTE16 = 124,             /* "ubyte16"  */
  YYSYMBOL_DAS_TTUPLE = 125,               /* "tuple"  */
  YYSYMBOL_DAS_TVARIANT = 126,             /* "variant"  */
  YYSYMBOL_DAS_GENERATOR = 127,            /* "generator"  */
  YYSYMBOL_DAS_YIELD = 128,                /* "yield"  */
  YYSYMBOL_DAS_SEALED = 129,               /* "sealed"  */
  YYSYMBOL_DAS_TEMPLATE = 130,             /* "template"  */
  YYSYMBOL_ADDEQU = 131,                   /* "+="  */
  YYSYMBOL_SUBEQU = 132,                   /* "-="  */
  YYSYMBOL_DIVEQU = 133,                   /* "/="  */
  YYSYMBOL_MULEQU = 134,                   /* "*="  */
  YYSYMBOL_MODEQU = 135,                   /* "%="  */
  YYSYMBOL_ANDEQU = 136,                   /* "&="  */
  YYSYMBOL_OREQU = 137,                    /* "|="  */
  YYSYMBOL_XOREQU = 138,                   /* "^="  */
  YYSYMBOL_SHL = 139,                      /* "<<"  */
  YYSYMBOL_SHR = 140,                      /* ">>"  */
  YYSYMBOL_ADDADD = 141,                   /* "++"  */
  YYSYMBOL_SUBSUB = 142,                   /* "--"  */
  YYSYMBOL_LEEQU = 143,                    /* "<="  */
  YYSYMBOL_SHLEQU = 144,                   /* "<<="  */
  YYSYMBOL_SHREQU = 145,                   /* ">>="  */
  YYSYMBOL_GREQU = 146,                    /* ">="  */
  YYSYMBOL_EQUEQU = 147,                   /* "=="  */
  YYSYMBOL_NOTEQU = 148,                   /* "!="  */
  YYSYMBOL_RARROW = 149,                   /* "->"  */
  YYSYMBOL_LARROW = 150,                   /* "<-"  */
  YYSYMBOL_QQ = 151,                       /* "??"  */
  YYSYMBOL_QDOT = 152,                     /* "?."  */
  YYSYMBOL_QBRA = 153,                     /* "?["  */
  YYSYMBOL_LPIPE = 154,                    /* "<|"  */
  YYSYMBOL_LBPIPE = 155,                   /* " <|"  */
  YYSYMBOL_LLPIPE = 156,                   /* "$ <|"  */
  YYSYMBOL_LAPIPE = 157,                   /* "@ <|"  */
  YYSYMBOL_LFPIPE = 158,                   /* "@@ <|"  */
  YYSYMBOL_RPIPE = 159,                    /* "|>"  */
  YYSYMBOL_CLONEEQU = 160,                 /* ":="  */
  YYSYMBOL_ROTL = 161,                     /* "<<<"  */
  YYSYMBOL_ROTR = 162,                     /* ">>>"  */
  YYSYMBOL_ROTLEQU = 163,                  /* "<<<="  */
  YYSYMBOL_ROTREQU = 164,                  /* ">>>="  */
  YYSYMBOL_MAPTO = 165,                    /* "=>"  */
  YYSYMBOL_COLCOL = 166,                   /* "::"  */
  YYSYMBOL_ANDAND = 167,                   /* "&&"  */
  YYSYMBOL_OROR = 168,                     /* "||"  */
  YYSYMBOL_XORXOR = 169,                   /* "^^"  */
  YYSYMBOL_ANDANDEQU = 170,                /* "&&="  */
  YYSYMBOL_OROREQU = 171,                  /* "||="  */
  YYSYMBOL_XORXOREQU = 172,                /* "^^="  */
  YYSYMBOL_DOTDOT = 173,                   /* ".."  */
  YYSYMBOL_MTAG_E = 174,                   /* "$$"  */
  YYSYMBOL_MTAG_I = 175,                   /* "$i"  */
  YYSYMBOL_MTAG_V = 176,                   /* "$v"  */
  YYSYMBOL_MTAG_B = 177,                   /* "$b"  */
  YYSYMBOL_MTAG_A = 178,                   /* "$a"  */
  YYSYMBOL_MTAG_T = 179,                   /* "$t"  */
  YYSYMBOL_MTAG_C = 180,                   /* "$c"  */
  YYSYMBOL_MTAG_F = 181,                   /* "$f"  */
  YYSYMBOL_MTAG_DOTDOTDOT = 182,           /* "..."  */
  YYSYMBOL_BRABRAB = 183,                  /* "[["  */
  YYSYMBOL_BRACBRB = 184,                  /* "[{"  */
  YYSYMBOL_CBRCBRB = 185,                  /* "{{"  */
  YYSYMBOL_OPEN_BRACE = 186,               /* "new scope"  */
  YYSYMBOL_CLOSE_BRACE = 187,              /* "close scope"  */
  YYSYMBOL_SEMICOLON = 188,                /* "end of line"  */
  YYSYMBOL_INTEGER = 189,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 190,             /* "long integer constant"  */
  YYSYMBOL_LONG_INTEGER_MIN = 191,         /* "int64 minimum magnitude (requires minus)"  */
  YYSYMBOL_UNSIGNED_INTEGER = 192,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 193,    /* "unsigned long integer constant"  */
  YYSYMBOL_UNSIGNED_INT8 = 194,            /* "unsigned int8 constant"  */
  YYSYMBOL_DAS_FLOAT = 195,                /* "floating point constant"  */
  YYSYMBOL_DAS_FLOAT16_CONST = 196,        /* "float16 constant"  */
  YYSYMBOL_DOUBLE = 197,                   /* "double constant"  */
  YYSYMBOL_NAME = 198,                     /* "name"  */
  YYSYMBOL_KEYWORD = 199,                  /* "keyword"  */
  YYSYMBOL_TYPE_FUNCTION = 200,            /* "type function"  */
  YYSYMBOL_BEGIN_STRING = 201,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 202,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 203,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 204,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 205,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 206,          /* "}"  */
  YYSYMBOL_END_OF_READ = 207,              /* "end of failed eader macro"  */
  YYSYMBOL_208_begin_of_code_block_ = 208, /* "begin of code block"  */
  YYSYMBOL_209_end_of_code_block_ = 209,   /* "end of code block"  */
  YYSYMBOL_210_end_of_expression_ = 210,   /* "end of expression"  */
  YYSYMBOL_SEMICOLON_CUR_CUR = 211,        /* ";}}"  */
  YYSYMBOL_SEMICOLON_CUR_SQR = 212,        /* ";}]"  */
  YYSYMBOL_SEMICOLON_SQR_SQR = 213,        /* ";]]"  */
  YYSYMBOL_COMMA_SQR_SQR = 214,            /* ",]]"  */
  YYSYMBOL_COMMA_CUR_SQR = 215,            /* ",}]"  */
  YYSYMBOL_216_ = 216,                     /* ','  */
  YYSYMBOL_217_ = 217,                     /* '='  */
  YYSYMBOL_218_ = 218,                     /* '?'  */
  YYSYMBOL_219_ = 219,                     /* ':'  */
  YYSYMBOL_220_ = 220,                     /* '|'  */
  YYSYMBOL_221_ = 221,                     /* '^'  */
  YYSYMBOL_222_ = 222,                     /* '&'  */
  YYSYMBOL_223_ = 223,                     /* '<'  */
  YYSYMBOL_224_ = 224,                     /* '>'  */
  YYSYMBOL_225_ = 225,                     /* '-'  */
  YYSYMBOL_226_ = 226,                     /* '+'  */
  YYSYMBOL_227_ = 227,                     /* '*'  */
  YYSYMBOL_228_ = 228,                     /* '/'  */
  YYSYMBOL_229_ = 229,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 230,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 231,               /* UNARY_PLUS  */
  YYSYMBOL_232_ = 232,                     /* '~'  */
  YYSYMBOL_233_ = 233,                     /* '!'  */
  YYSYMBOL_PRE_INC = 234,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 235,                  /* PRE_DEC  */
  YYSYMBOL_POST_INC = 236,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 237,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 238,                    /* DEREF  */
  YYSYMBOL_239_ = 239,                     /* '.'  */
  YYSYMBOL_240_ = 240,                     /* '['  */
  YYSYMBOL_241_ = 241,                     /* ']'  */
  YYSYMBOL_242_ = 242,                     /* '('  */
  YYSYMBOL_243_ = 243,                     /* ')'  */
  YYSYMBOL_244_ = 244,                     /* '$'  */
  YYSYMBOL_245_ = 245,                     /* '@'  */
  YYSYMBOL_246_ = 246,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 247,                 /* $accept  */
  YYSYMBOL_program = 248,                  /* program  */
  YYSYMBOL_top_level_reader_macro = 249,   /* top_level_reader_macro  */
  YYSYMBOL_optional_public_or_private_module = 250, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 251,              /* module_name  */
  YYSYMBOL_optional_not_required = 252,    /* optional_not_required  */
  YYSYMBOL_module_declaration = 253,       /* module_declaration  */
  YYSYMBOL_character_sequence = 254,       /* character_sequence  */
  YYSYMBOL_string_constant = 255,          /* string_constant  */
  YYSYMBOL_format_string = 256,            /* format_string  */
  YYSYMBOL_optional_format_string = 257,   /* optional_format_string  */
  YYSYMBOL_258_1 = 258,                    /* $@1  */
  YYSYMBOL_string_builder_body = 259,      /* string_builder_body  */
  YYSYMBOL_string_builder = 260,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 261, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 262,              /* expr_reader  */
  YYSYMBOL_263_2 = 263,                    /* $@2  */
  YYSYMBOL_options_declaration = 264,      /* options_declaration  */
  YYSYMBOL_require_declaration = 265,      /* require_declaration  */
  YYSYMBOL_keyword_or_name = 266,          /* keyword_or_name  */
  YYSYMBOL_require_module_name = 267,      /* require_module_name  */
  YYSYMBOL_require_module = 268,           /* require_module  */
  YYSYMBOL_is_public_module = 269,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 270,       /* expect_declaration  */
  YYSYMBOL_expect_list = 271,              /* expect_list  */
  YYSYMBOL_expect_error = 272,             /* expect_error  */
  YYSYMBOL_expression_label = 273,         /* expression_label  */
  YYSYMBOL_expression_goto = 274,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 275,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 276,          /* expression_else  */
  YYSYMBOL_semicolon = 277,                /* semicolon  */
  YYSYMBOL_if_or_static_if = 278,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 279, /* expression_else_one_liner  */
  YYSYMBOL_280_3 = 280,                    /* $@3  */
  YYSYMBOL_expression_if_one_liner = 281,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 282,  /* expression_if_then_else  */
  YYSYMBOL_283_4 = 283,                    /* $@4  */
  YYSYMBOL_expression_for_loop = 284,      /* expression_for_loop  */
  YYSYMBOL_285_5 = 285,                    /* $@5  */
  YYSYMBOL_expression_unsafe = 286,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 287,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 288,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 289,    /* expression_with_alias  */
  YYSYMBOL_290_6 = 290,                    /* $@6  */
  YYSYMBOL_291_7 = 291,                    /* $@7  */
  YYSYMBOL_annotation_argument_value = 292, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 293, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 294, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 295,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 296, /* annotation_argument_list  */
  YYSYMBOL_metadata_argument_list = 297,   /* metadata_argument_list  */
  YYSYMBOL_annotation_declaration_name = 298, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 299, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 300,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 301,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 302, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 303, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 304,   /* optional_function_type  */
  YYSYMBOL_function_name = 305,            /* function_name  */
  YYSYMBOL_optional_template = 306,        /* optional_template  */
  YYSYMBOL_global_function_declaration = 307, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 308, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 309, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 310,     /* function_declaration  */
  YYSYMBOL_311_8 = 311,                    /* $@8  */
  YYSYMBOL_open_block = 312,               /* open_block  */
  YYSYMBOL_close_block = 313,              /* close_block  */
  YYSYMBOL_expression_block = 314,         /* expression_block  */
  YYSYMBOL_expr_call_pipe = 315,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 316,           /* expression_any  */
  YYSYMBOL_expressions = 317,              /* expressions  */
  YYSYMBOL_expr_keyword = 318,             /* expr_keyword  */
  YYSYMBOL_optional_expr_list = 319,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_list_in_braces = 320, /* optional_expr_list_in_braces  */
  YYSYMBOL_optional_expr_map_tuple_list = 321, /* optional_expr_map_tuple_list  */
  YYSYMBOL_type_declaration_no_options_list = 322, /* type_declaration_no_options_list  */
  YYSYMBOL_expression_keyword = 323,       /* expression_keyword  */
  YYSYMBOL_324_9 = 324,                    /* $@9  */
  YYSYMBOL_325_10 = 325,                   /* $@10  */
  YYSYMBOL_326_11 = 326,                   /* $@11  */
  YYSYMBOL_327_12 = 327,                   /* $@12  */
  YYSYMBOL_expr_pipe = 328,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 329,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 330,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 331,     /* new_type_declaration  */
  YYSYMBOL_332_13 = 332,                   /* $@13  */
  YYSYMBOL_333_14 = 333,                   /* $@14  */
  YYSYMBOL_expr_new = 334,                 /* expr_new  */
  YYSYMBOL_expression_break = 335,         /* expression_break  */
  YYSYMBOL_expression_continue = 336,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 337, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 338,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 339, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 340,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 341,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 342,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 343,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 344,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 345,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 346, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 347,           /* expression_let  */
  YYSYMBOL_expr_cast = 348,                /* expr_cast  */
  YYSYMBOL_349_15 = 349,                   /* $@15  */
  YYSYMBOL_350_16 = 350,                   /* $@16  */
  YYSYMBOL_351_17 = 351,                   /* $@17  */
  YYSYMBOL_352_18 = 352,                   /* $@18  */
  YYSYMBOL_353_19 = 353,                   /* $@19  */
  YYSYMBOL_354_20 = 354,                   /* $@20  */
  YYSYMBOL_expr_type_decl = 355,           /* expr_type_decl  */
  YYSYMBOL_356_21 = 356,                   /* $@21  */
  YYSYMBOL_357_22 = 357,                   /* $@22  */
  YYSYMBOL_expr_type_info = 358,           /* expr_type_info  */
  YYSYMBOL_expr_list = 359,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 360,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 361,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 362,            /* capture_entry  */
  YYSYMBOL_capture_list = 363,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 364,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 365,               /* expr_block  */
  YYSYMBOL_expr_full_block = 366,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 367, /* expr_full_block_assumed_piped  */
  YYSYMBOL_368_23 = 368,                   /* $@23  */
  YYSYMBOL_expr_numeric_const = 369,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 370,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe_right = 371,   /* expr_assign_pipe_right  */
  YYSYMBOL_expr_assign_pipe = 372,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 373,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 374,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 375,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 376,           /* func_addr_expr  */
  YYSYMBOL_377_24 = 377,                   /* $@24  */
  YYSYMBOL_378_25 = 378,                   /* $@25  */
  YYSYMBOL_379_26 = 379,                   /* $@26  */
  YYSYMBOL_380_27 = 380,                   /* $@27  */
  YYSYMBOL_expr_field = 381,               /* expr_field  */
  YYSYMBOL_382_28 = 382,                   /* $@28  */
  YYSYMBOL_383_29 = 383,                   /* $@29  */
  YYSYMBOL_expr_call = 384,                /* expr_call  */
  YYSYMBOL_expr = 385,                     /* expr  */
  YYSYMBOL_386_30 = 386,                   /* $@30  */
  YYSYMBOL_387_31 = 387,                   /* $@31  */
  YYSYMBOL_388_32 = 388,                   /* $@32  */
  YYSYMBOL_389_33 = 389,                   /* $@33  */
  YYSYMBOL_390_34 = 390,                   /* $@34  */
  YYSYMBOL_391_35 = 391,                   /* $@35  */
  YYSYMBOL_expr_mtag = 392,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 393, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 394,        /* optional_override  */
  YYSYMBOL_optional_constant = 395,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 396, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 397, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 398, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 399, /* struct_variable_declaration_list  */
  YYSYMBOL_400_36 = 400,                   /* $@36  */
  YYSYMBOL_401_37 = 401,                   /* $@37  */
  YYSYMBOL_402_38 = 402,                   /* $@38  */
  YYSYMBOL_403_39 = 403,                   /* $@39  */
  YYSYMBOL_function_argument_declaration_no_type = 404, /* function_argument_declaration_no_type  */
  YYSYMBOL_function_argument_declaration_type = 405, /* function_argument_declaration_type  */
  YYSYMBOL_function_argument_list = 406,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 407,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 408,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 409,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 410,             /* variant_type  */
  YYSYMBOL_variant_type_list = 411,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 412,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 413,             /* copy_or_move  */
  YYSYMBOL_variable_declaration_no_type = 414, /* variable_declaration_no_type  */
  YYSYMBOL_variable_declaration_type = 415, /* variable_declaration_type  */
  YYSYMBOL_variable_declaration = 416,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 417,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 418,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 419, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 420, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 421, /* global_variable_declaration_list  */
  YYSYMBOL_422_40 = 422,                   /* $@40  */
  YYSYMBOL_optional_shared = 423,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 424, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 425,               /* global_let  */
  YYSYMBOL_426_41 = 426,                   /* $@41  */
  YYSYMBOL_enum_list = 427,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 428, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 429,             /* single_alias  */
  YYSYMBOL_430_42 = 430,                   /* $@42  */
  YYSYMBOL_alias_list = 431,               /* alias_list  */
  YYSYMBOL_alias_declaration = 432,        /* alias_declaration  */
  YYSYMBOL_433_43 = 433,                   /* $@43  */
  YYSYMBOL_optional_public_or_private_enum = 434, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 435,                /* enum_name  */
  YYSYMBOL_enum_declaration = 436,         /* enum_declaration  */
  YYSYMBOL_437_44 = 437,                   /* $@44  */
  YYSYMBOL_438_45 = 438,                   /* $@45  */
  YYSYMBOL_439_46 = 439,                   /* $@46  */
  YYSYMBOL_440_47 = 440,                   /* $@47  */
  YYSYMBOL_optional_structure_parent = 441, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 442,          /* optional_sealed  */
  YYSYMBOL_structure_name = 443,           /* structure_name  */
  YYSYMBOL_class_or_struct = 444,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 445, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 446, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 447,    /* structure_declaration  */
  YYSYMBOL_448_48 = 448,                   /* $@48  */
  YYSYMBOL_449_49 = 449,                   /* $@49  */
  YYSYMBOL_variable_name_with_pos_list = 450, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 451,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 452, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 453, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 454,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 455,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 456,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_basic_type_declaration = 457, /* bitfield_basic_type_declaration  */
  YYSYMBOL_bitfield_type_declaration = 458, /* bitfield_type_declaration  */
  YYSYMBOL_459_50 = 459,                   /* $@50  */
  YYSYMBOL_460_51 = 460,                   /* $@51  */
  YYSYMBOL_c_or_s = 461,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 462,          /* table_type_pair  */
  YYSYMBOL_dim_list = 463,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 464, /* type_declaration_no_options  */
  YYSYMBOL_465_52 = 465,                   /* $@52  */
  YYSYMBOL_466_53 = 466,                   /* $@53  */
  YYSYMBOL_467_54 = 467,                   /* $@54  */
  YYSYMBOL_468_55 = 468,                   /* $@55  */
  YYSYMBOL_469_56 = 469,                   /* $@56  */
  YYSYMBOL_470_57 = 470,                   /* $@57  */
  YYSYMBOL_471_58 = 471,                   /* $@58  */
  YYSYMBOL_472_59 = 472,                   /* $@59  */
  YYSYMBOL_473_60 = 473,                   /* $@60  */
  YYSYMBOL_474_61 = 474,                   /* $@61  */
  YYSYMBOL_475_62 = 475,                   /* $@62  */
  YYSYMBOL_476_63 = 476,                   /* $@63  */
  YYSYMBOL_477_64 = 477,                   /* $@64  */
  YYSYMBOL_478_65 = 478,                   /* $@65  */
  YYSYMBOL_479_66 = 479,                   /* $@66  */
  YYSYMBOL_480_67 = 480,                   /* $@67  */
  YYSYMBOL_481_68 = 481,                   /* $@68  */
  YYSYMBOL_482_69 = 482,                   /* $@69  */
  YYSYMBOL_483_70 = 483,                   /* $@70  */
  YYSYMBOL_484_71 = 484,                   /* $@71  */
  YYSYMBOL_485_72 = 485,                   /* $@72  */
  YYSYMBOL_486_73 = 486,                   /* $@73  */
  YYSYMBOL_487_74 = 487,                   /* $@74  */
  YYSYMBOL_488_75 = 488,                   /* $@75  */
  YYSYMBOL_489_76 = 489,                   /* $@76  */
  YYSYMBOL_490_77 = 490,                   /* $@77  */
  YYSYMBOL_491_78 = 491,                   /* $@78  */
  YYSYMBOL_492_79 = 492,                   /* $@79  */
  YYSYMBOL_type_declaration = 493,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 494,  /* tuple_alias_declaration  */
  YYSYMBOL_495_80 = 495,                   /* $@80  */
  YYSYMBOL_496_81 = 496,                   /* $@81  */
  YYSYMBOL_497_82 = 497,                   /* $@82  */
  YYSYMBOL_498_83 = 498,                   /* $@83  */
  YYSYMBOL_variant_alias_declaration = 499, /* variant_alias_declaration  */
  YYSYMBOL_500_84 = 500,                   /* $@84  */
  YYSYMBOL_501_85 = 501,                   /* $@85  */
  YYSYMBOL_502_86 = 502,                   /* $@86  */
  YYSYMBOL_503_87 = 503,                   /* $@87  */
  YYSYMBOL_bitfield_alias_declaration = 504, /* bitfield_alias_declaration  */
  YYSYMBOL_505_88 = 505,                   /* $@88  */
  YYSYMBOL_506_89 = 506,                   /* $@89  */
  YYSYMBOL_507_90 = 507,                   /* $@90  */
  YYSYMBOL_508_91 = 508,                   /* $@91  */
  YYSYMBOL_make_decl = 509,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 510,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 511,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 512,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim = 513,          /* make_struct_dim  */
  YYSYMBOL_make_struct_dim_list = 514,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 515,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 516, /* optional_make_struct_dim_decl  */
  YYSYMBOL_optional_block = 517,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_cur_cur = 518, /* optional_trailing_semicolon_cur_cur  */
  YYSYMBOL_optional_trailing_semicolon_cur_sqr = 519, /* optional_trailing_semicolon_cur_sqr  */
  YYSYMBOL_optional_trailing_semicolon_sqr_sqr = 520, /* optional_trailing_semicolon_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_sqr_sqr = 521, /* optional_trailing_delim_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_cur_sqr = 522, /* optional_trailing_delim_cur_sqr  */
  YYSYMBOL_use_initializer = 523,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 524,         /* make_struct_decl  */
  YYSYMBOL_525_92 = 525,                   /* $@92  */
  YYSYMBOL_526_93 = 526,                   /* $@93  */
  YYSYMBOL_527_94 = 527,                   /* $@94  */
  YYSYMBOL_528_95 = 528,                   /* $@95  */
  YYSYMBOL_529_96 = 529,                   /* $@96  */
  YYSYMBOL_530_97 = 530,                   /* $@97  */
  YYSYMBOL_531_98 = 531,                   /* $@98  */
  YYSYMBOL_532_99 = 532,                   /* $@99  */
  YYSYMBOL_make_tuple = 533,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 534,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 535,          /* make_tuple_call  */
  YYSYMBOL_536_100 = 536,                  /* $@100  */
  YYSYMBOL_537_101 = 537,                  /* $@101  */
  YYSYMBOL_make_dim = 538,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 539,            /* make_dim_decl  */
  YYSYMBOL_540_102 = 540,                  /* $@102  */
  YYSYMBOL_541_103 = 541,                  /* $@103  */
  YYSYMBOL_542_104 = 542,                  /* $@104  */
  YYSYMBOL_543_105 = 543,                  /* $@105  */
  YYSYMBOL_544_106 = 544,                  /* $@106  */
  YYSYMBOL_545_107 = 545,                  /* $@107  */
  YYSYMBOL_546_108 = 546,                  /* $@108  */
  YYSYMBOL_547_109 = 547,                  /* $@109  */
  YYSYMBOL_548_110 = 548,                  /* $@110  */
  YYSYMBOL_549_111 = 549,                  /* $@111  */
  YYSYMBOL_make_table = 550,               /* make_table  */
  YYSYMBOL_expr_map_tuple_list = 551,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 552,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 553, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 554,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 555       /* array_comprehension  */
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
#define YYLAST   16573

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  247
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  309
/* YYNRULES -- Number of rules.  */
#define YYNRULES  1027
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1860

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   474


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
       2,     2,     2,   233,     2,   246,   244,   229,   222,     2,
     242,   243,   227,   226,   216,   225,   239,   228,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   219,   210,
     223,   217,   224,   218,   245,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   240,     2,   241,   221,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   208,   220,   209,   232,     2,     2,     2,
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
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   211,   212,   213,   214,   215,   230,   231,
     234,   235,   236,   237,   238
};

#if DAS_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   594,   594,   595,   600,   601,   602,   603,   604,   605,
     606,   607,   608,   609,   610,   611,   612,   616,   622,   623,
     624,   628,   629,   633,   634,   638,   657,   658,   659,   660,
     664,   665,   669,   670,   674,   675,   675,   679,   684,   693,
     708,   724,   729,   737,   737,   782,   812,   816,   817,   818,
     822,   825,   829,   833,   837,   841,   847,   856,   859,   865,
     866,   870,   874,   875,   879,   882,   888,   894,   897,   903,
     904,   908,   909,   910,   919,   920,   924,   925,   929,   930,
     930,   936,   937,   938,   939,   940,   944,   950,   950,   956,
     956,   962,   970,   980,   989,   989,   993,   993,   999,  1000,
    1001,  1002,  1003,  1004,  1005,  1006,  1007,  1008,  1009,  1010,
    1011,  1015,  1020,  1028,  1029,  1030,  1034,  1035,  1036,  1037,
    1038,  1039,  1040,  1041,  1042,  1043,  1044,  1045,  1046,  1047,
    1048,  1054,  1057,  1063,  1066,  1069,  1075,  1076,  1077,  1078,
    1082,  1099,  1121,  1124,  1134,  1149,  1164,  1179,  1182,  1189,
    1193,  1200,  1201,  1205,  1206,  1207,  1211,  1215,  1219,  1226,
    1230,  1231,  1232,  1233,  1234,  1235,  1236,  1237,  1238,  1239,
    1240,  1241,  1242,  1243,  1244,  1245,  1246,  1247,  1248,  1249,
    1250,  1251,  1252,  1253,  1254,  1255,  1256,  1257,  1258,  1259,
    1260,  1261,  1262,  1263,  1264,  1265,  1266,  1267,  1268,  1269,
    1270,  1271,  1272,  1273,  1274,  1275,  1276,  1277,  1278,  1279,
    1280,  1281,  1282,  1283,  1284,  1285,  1286,  1287,  1288,  1289,
    1290,  1291,  1292,  1293,  1294,  1295,  1296,  1297,  1298,  1299,
    1300,  1301,  1302,  1303,  1304,  1305,  1306,  1307,  1308,  1309,
    1310,  1311,  1312,  1313,  1314,  1315,  1316,  1317,  1318,  1319,
    1320,  1321,  1322,  1323,  1324,  1325,  1326,  1327,  1328,  1329,
    1330,  1331,  1332,  1333,  1334,  1335,  1336,  1337,  1338,  1339,
    1340,  1341,  1342,  1343,  1344,  1345,  1346,  1347,  1348,  1349,
    1350,  1351,  1352,  1353,  1354,  1355,  1356,  1357,  1358,  1359,
    1360,  1364,  1365,  1369,  1388,  1389,  1390,  1394,  1400,  1400,
    1418,  1419,  1422,  1423,  1426,  1430,  1441,  1450,  1459,  1465,
    1466,  1467,  1468,  1469,  1470,  1471,  1472,  1473,  1474,  1475,
    1476,  1477,  1478,  1479,  1480,  1481,  1482,  1483,  1484,  1485,
    1489,  1494,  1500,  1506,  1517,  1518,  1522,  1523,  1527,  1528,
    1532,  1536,  1543,  1543,  1543,  1549,  1549,  1549,  1558,  1592,
    1595,  1598,  1601,  1607,  1608,  1619,  1623,  1626,  1634,  1634,
    1634,  1637,  1643,  1646,  1650,  1654,  1661,  1668,  1674,  1678,
    1682,  1685,  1688,  1696,  1699,  1702,  1710,  1713,  1721,  1724,
    1727,  1735,  1741,  1742,  1743,  1747,  1748,  1752,  1753,  1757,
    1762,  1770,  1776,  1782,  1788,  1794,  1802,  1810,  1818,  1829,
    1832,  1838,  1838,  1838,  1841,  1841,  1841,  1846,  1846,  1846,
    1854,  1854,  1854,  1860,  1870,  1881,  1894,  1904,  1915,  1930,
    1933,  1939,  1940,  1948,  1960,  1961,  1962,  1966,  1967,  1968,
    1969,  1970,  1974,  1979,  1987,  1988,  1989,  1993,  1998,  2005,
    2012,  2012,  2021,  2022,  2023,  2024,  2025,  2026,  2027,  2028,
    2029,  2033,  2034,  2035,  2036,  2037,  2038,  2039,  2040,  2041,
    2042,  2043,  2044,  2045,  2046,  2047,  2048,  2049,  2050,  2051,
    2055,  2056,  2057,  2058,  2063,  2064,  2065,  2066,  2067,  2068,
    2069,  2070,  2071,  2072,  2073,  2074,  2075,  2076,  2077,  2078,
    2079,  2084,  2090,  2101,  2107,  2118,  2122,  2129,  2132,  2132,
    2132,  2137,  2137,  2137,  2150,  2154,  2158,  2164,  2172,  2180,
    2186,  2194,  2194,  2194,  2201,  2205,  2214,  2222,  2230,  2234,
    2237,  2243,  2244,  2245,  2246,  2247,  2248,  2249,  2250,  2251,
    2252,  2253,  2254,  2255,  2256,  2257,  2258,  2259,  2260,  2261,
    2262,  2263,  2264,  2265,  2266,  2267,  2268,  2269,  2270,  2271,
    2272,  2273,  2274,  2275,  2276,  2277,  2278,  2284,  2285,  2286,
    2287,  2288,  2303,  2312,  2313,  2314,  2315,  2316,  2317,  2318,
    2319,  2320,  2321,  2322,  2323,  2326,  2329,  2330,  2333,  2333,
    2333,  2336,  2341,  2345,  2349,  2349,  2349,  2354,  2357,  2361,
    2361,  2361,  2366,  2369,  2370,  2371,  2372,  2373,  2374,  2375,
    2376,  2377,  2379,  2383,  2384,  2389,  2393,  2394,  2395,  2396,
    2397,  2398,  2399,  2403,  2407,  2411,  2415,  2419,  2423,  2427,
    2431,  2435,  2442,  2443,  2444,  2448,  2449,  2450,  2454,  2455,
    2459,  2460,  2461,  2465,  2466,  2470,  2481,  2484,  2487,  2487,
    2491,  2491,  2510,  2509,  2525,  2524,  2538,  2547,  2559,  2568,
    2578,  2579,  2580,  2581,  2582,  2586,  2589,  2598,  2599,  2603,
    2606,  2609,  2624,  2633,  2634,  2638,  2641,  2644,  2657,  2658,
    2662,  2667,  2672,  2680,  2683,  2690,  2693,  2699,  2700,  2701,
    2705,  2706,  2710,  2717,  2722,  2731,  2737,  2741,  2752,  2756,
    2762,  2768,  2776,  2787,  2790,  2793,  2793,  2813,  2814,  2818,
    2819,  2820,  2824,  2827,  2827,  2846,  2849,  2852,  2867,  2886,
    2887,  2888,  2893,  2893,  2919,  2920,  2924,  2925,  2925,  2929,
    2930,  2931,  2935,  2945,  2950,  2945,  2962,  2967,  2962,  2982,
    2983,  2987,  2988,  2992,  2998,  2999,  3000,  3001,  3005,  3006,
    3007,  3011,  3014,  3020,  3025,  3020,  3045,  3052,  3057,  3066,
    3072,  3076,  3087,  3088,  3089,  3090,  3091,  3092,  3093,  3094,
    3095,  3096,  3097,  3098,  3099,  3100,  3101,  3102,  3103,  3104,
    3105,  3106,  3107,  3108,  3109,  3110,  3111,  3112,  3113,  3114,
    3115,  3116,  3117,  3118,  3119,  3120,  3121,  3122,  3123,  3124,
    3125,  3126,  3127,  3128,  3129,  3130,  3131,  3132,  3133,  3134,
    3135,  3136,  3140,  3141,  3142,  3143,  3144,  3145,  3146,  3147,
    3151,  3162,  3166,  3173,  3185,  3192,  3198,  3207,  3212,  3215,
    3225,  3238,  3239,  3240,  3241,  3242,  3246,  3250,  3250,  3250,
    3264,  3265,  3269,  3274,  3281,  3284,  3290,  3291,  3292,  3293,
    3294,  3304,  3307,  3307,  3307,  3311,  3316,  3323,  3323,  3323,
    3330,  3334,  3338,  3343,  3348,  3353,  3358,  3362,  3366,  3371,
    3375,  3379,  3384,  3384,  3384,  3390,  3397,  3397,  3397,  3402,
    3402,  3402,  3408,  3408,  3408,  3413,  3419,  3419,  3419,  3424,
    3424,  3424,  3433,  3439,  3439,  3439,  3444,  3444,  3444,  3453,
    3459,  3459,  3459,  3464,  3464,  3464,  3473,  3473,  3473,  3479,
    3479,  3479,  3488,  3491,  3502,  3518,  3518,  3523,  3528,  3518,
    3553,  3553,  3558,  3564,  3553,  3589,  3589,  3594,  3599,  3589,
    3639,  3640,  3641,  3642,  3643,  3647,  3654,  3661,  3667,  3673,
    3680,  3687,  3693,  3702,  3705,  3711,  3719,  3724,  3731,  3736,
    3743,  3748,  3754,  3755,  3759,  3760,  3765,  3766,  3770,  3771,
    3775,  3776,  3780,  3781,  3782,  3786,  3787,  3788,  3792,  3793,
    3797,  3803,  3810,  3818,  3825,  3833,  3842,  3842,  3842,  3850,
    3850,  3850,  3857,  3857,  3857,  3867,  3867,  3867,  3878,  3881,
    3887,  3901,  3907,  3913,  3919,  3919,  3919,  3932,  3937,  3944,
    3963,  3968,  3975,  3975,  3975,  3985,  3985,  3985,  3998,  3998,
    3998,  4011,  4020,  4020,  4020,  4040,  4047,  4047,  4047,  4057,
    4062,  4069,  4072,  4078,  4097,  4108,  4116,  4136,  4161,  4162,
    4166,  4167,  4172,  4175,  4178,  4181,  4184,  4187
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
  "\"int8\"", "\"uint8\"", "\"int16\"", "\"uint16\"", "\"float16\"",
  "\"half2\"", "\"half3\"", "\"half4\"", "\"half8\"", "\"short2\"",
  "\"short3\"", "\"short4\"", "\"short8\"", "\"ushort2\"", "\"ushort3\"",
  "\"ushort4\"", "\"ushort8\"", "\"byte2\"", "\"byte3\"", "\"byte4\"",
  "\"byte8\"", "\"byte16\"", "\"ubyte2\"", "\"ubyte3\"", "\"ubyte4\"",
  "\"ubyte8\"", "\"ubyte16\"", "\"tuple\"", "\"variant\"", "\"generator\"",
  "\"yield\"", "\"sealed\"", "\"template\"", "\"+=\"", "\"-=\"", "\"/=\"",
  "\"*=\"", "\"%=\"", "\"&=\"", "\"|=\"", "\"^=\"", "\"<<\"", "\">>\"",
  "\"++\"", "\"--\"", "\"<=\"", "\"<<=\"", "\">>=\"", "\">=\"", "\"==\"",
  "\"!=\"", "\"->\"", "\"<-\"", "\"??\"", "\"?.\"", "\"?[\"", "\"<|\"",
  "\" <|\"", "\"$ <|\"", "\"@ <|\"", "\"@@ <|\"", "\"|>\"", "\":=\"",
  "\"<<<\"", "\">>>\"", "\"<<<=\"", "\">>>=\"", "\"=>\"", "\"::\"",
  "\"&&\"", "\"||\"", "\"^^\"", "\"&&=\"", "\"||=\"", "\"^^=\"", "\"..\"",
  "\"$$\"", "\"$i\"", "\"$v\"", "\"$b\"", "\"$a\"", "\"$t\"", "\"$c\"",
  "\"$f\"", "\"...\"", "\"[[\"", "\"[{\"", "\"{{\"", "\"new scope\"",
  "\"close scope\"", "\"end of line\"", "\"integer constant\"",
  "\"long integer constant\"",
  "\"int64 minimum magnitude (requires minus)\"",
  "\"unsigned integer constant\"", "\"unsigned long integer constant\"",
  "\"unsigned int8 constant\"", "\"floating point constant\"",
  "\"float16 constant\"", "\"double constant\"", "\"name\"", "\"keyword\"",
  "\"type function\"", "\"start of the string\"", "STRING_CHARACTER",
  "STRING_CHARACTER_ESC", "\"end of the string\"", "\"{\"", "\"}\"",
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
  "$@75", "$@76", "$@77", "$@78", "$@79", "type_declaration",
  "tuple_alias_declaration", "$@80", "$@81", "$@82", "$@83",
  "variant_alias_declaration", "$@84", "$@85", "$@86", "$@87",
  "bitfield_alias_declaration", "$@88", "$@89", "$@90", "$@91",
  "make_decl", "make_struct_fields", "make_variant_dim",
  "make_struct_single", "make_struct_dim", "make_struct_dim_list",
  "make_struct_dim_decl", "optional_make_struct_dim_decl",
  "optional_block", "optional_trailing_semicolon_cur_cur",
  "optional_trailing_semicolon_cur_sqr",
  "optional_trailing_semicolon_sqr_sqr", "optional_trailing_delim_sqr_sqr",
  "optional_trailing_delim_cur_sqr", "use_initializer", "make_struct_decl",
  "$@92", "$@93", "$@94", "$@95", "$@96", "$@97", "$@98", "$@99",
  "make_tuple", "make_map_tuple", "make_tuple_call", "$@100", "$@101",
  "make_dim", "make_dim_decl", "$@102", "$@103", "$@104", "$@105", "$@106",
  "$@107", "$@108", "$@109", "$@110", "$@111", "make_table",
  "expr_map_tuple_list", "make_table_decl", "array_comprehension_where",
  "optional_comma", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1599)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-894)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1599,    56, -1599, -1599,    22,   -82,   529,   579, -1599,   -35,
     171,   171,   171, -1599, -1599,   231,   480, -1599, -1599,   593,
   -1599, -1599, -1599, -1599,   736, -1599,    96, -1599, -1599, -1599,
   -1599, -1599, -1599, -1599, -1599, -1599, -1599,    -4, -1599,   114,
       2,   142, -1599,    85, -1599, -1599, -1599,   462,   175, -1599,
      54, -1599, -1599, -1599,   171,   171, -1599, -1599,    96, -1599,
   -1599, -1599, -1599, -1599,   170,   255, -1599, -1599, -1599, -1599,
     480,   480,   480,   276, -1599,   976,   -19, -1599, -1599,   326,
     406,   459,   179,   254, -1599,   392,   162,    22,   339,   -82,
     529,   529,   359,   529,   436, -1599,   951,   951, -1599,   444,
     593,    68,   593,   713,   523,   586,   588, -1599,   621,   424,
   -1599, -1599,   205,    22,   480,   480,   480,   480, -1599, -1599,
   -1599, -1599,   765, -1599, -1599,   655, -1599, -1599, -1599, -1599,
   -1599,   579, -1599, -1599, -1599, -1599, -1599, -1599, -1599,   968,
     751,   216,   441, -1599, -1599, -1599, -1599,   359,   359,   359,
     828, -1599, -1599, -1599, -1599, -1599, -1599, -1599,   593, -1599,
   -1599, -1599,   664, -1599, -1599, -1599, -1599, -1599,   730, -1599,
    -119, -1599,   610,   748,   976, -1599, -1599, -1599, -1599, -1599,
     180,   841, -1599,   -70, -1599, -1599, -1599,   973, -1599, -1599,
   -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599,   786,
     737, -1599, -1599,   132,   802, -1599,   785, -1599,   939, -1599,
     799,   579,   579, -1599, -1599, 16375,  1027, -1599, -1599,   814,
   -1599,   476,    22,    22,   -83,   371, -1599, -1599, -1599, -1599,
   -1599, -1599, -1599,   827,   216, -1599, -1599, 12001, -1599,   769,
     579, -1599, -1599, 15019, -1599, -1599, -1599, -1599, -1599, -1599,
   -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599,
   -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599,
   -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599,
   -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599,
   -1599, -1599,   991,   997, -1599,   794,   579, -1599, -1599, -1599,
   -1599, -1599, -1599, -1599, -1599,   579, -1599,   818,   579, -1599,
   -1599,   -70,    99, -1599,    22, -1599,   805,  1002,   613, -1599,
   -1599, -1599,   831,   835,   845,   822,   848,   853, -1599, -1599,
   -1599,   837, -1599, -1599, -1599, -1599, -1599,   685, -1599, -1599,
   -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599,   861,
   -1599, -1599, -1599,   865,   867, -1599, -1599, -1599, -1599, -1599,
   -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599,
   -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599,
   -1599, -1599,   869,   871,   844,   231, -1599, -1599, -1599, -1599,
   -1599,   113,   876, -1599, -1599, -1599, -1599, -1599, -1599, -1599,
     900,   906, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599,
   -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599,
   -1599, -1599,   909,   868, -1599, -1599, -1599, -1599, -1599, -1599,
   -1599, -1599, -1599, -1599, -1599, -1599,  1089, -1599, -1599, -1599,
   -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599,   915,
     874, -1599, -1599,   -38,   -65, -1599, -1599, -1599,   617,   231,
   -1599, -1599, -1599,   371,   878, -1599, 10882,   924,   525, 12001,
   -1599,   -52, -1599, -1599, -1599, 10882, -1599, -1599,   925,   918,
     -81,   -55,    -7, -1599, -1599, 10882,   159, -1599, -1599, -1599,
      21, -1599, -1599, -1599,    48,  6489, -1599,   892, 11651, -1599,
   11827,   443, -1599, -1599, -1599, -1599,   949,  1263,   746,   911,
   -1599,   217,   593,   248,   913, 12001, 12001, -1599,  2551, -1599,
     237, -1599,   601, -1599,    38, -1599, -1599,   932,   940, -1599,
   -1599,  1163,   -75,   942,    34, -1599,   228,   931,   944,   955,
     945,   959,   950,   461,   970, -1599,   589,   977,   979, 10882,
   10882,   961,   962,   963,   964,   965,   966, -1599, 11303, 11477,
    6722, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599,   986,
     987, -1599,  6955,  7186, 10882, 10882, 10882, 10882,  5565,  7417,
   -1599,   971, -1599, -1599, -1599,  -103, -1599, -1599, -1599, -1599,
     978, -1599, -1599, -1599, -1599, -1599, -1599, -1599,  2268, -1599,
     969, -1599, -1599, -1599, -1599, -1599, -1599, -1599,   980,  1172,
    1222, -1599, -1599, -1599,  4171, 12001, 12001, 12001,  2305, 12001,
   12001,   981,   999, 12001,   794, 12001,   794, 12001,   794, 12175,
    1023, 12216, -1599, 10882, -1599, -1599, -1599, -1599, -1599,   993,
   -1599, -1599, 14495, 10882, -1599,   113,   678,   102, -1599, -1599,
     319, -1599,   876,   601,  1009,   319, -1599,   601, -1599, -1599,
   -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599,
   -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599,
   -1599, -1599, -1599, -1599, 10882, -1599, -1599,   580,   -27,   -27,
     -27, -1599,   876,   876, -1599, 10882, -1599, -1599, -1599,  3478,
   -1599,   579,  7648, -1599, 10882,  1040, -1599,   593,  1050,  7879,
     -58,  1019,  3709,   227,   227,   227,  8110,   593,   593, -1599,
   10882,  1233, -1599, -1599, -1599, -1599, -1599, -1599,  1208, -1599,
   -1599, -1599,   629, -1599,   593,   593,   593,   593, -1599,   593,
   -1599, -1599,  1181, -1599,   140, -1599,  1869,   617, 10882, -1599,
   -1599, -1599,   480, -1599,  1240, -1599,   -70, -1599, -1599, -1599,
    1007, -1599, -1599,   231,   620, -1599,  1028,  1029,  1030, -1599,
   10882, 12001, 10882, 10882, -1599, -1599, 10882, -1599, 10882, -1599,
   10882, -1599, -1599, 10882, -1599, 12001,    97,    97, 10882, 10882,
   10882, 10882, 10882, 10882,   580,  2782,   580,  3014,   580, 15274,
   -1599,   975, -1599, -1599,   957,   580,  1041, -1599,  1043, -1599,
      97,    97,   -60,    97,    97,   580,  1244,  1020,  1046, 15582,
    1022,   139,  1046,  1049,  1024,   373, -1599,  4402,    71, 16037,
   16092, 10882, 10882, -1599, -1599, 10882, 10882, 10882, 10882,  1068,
   10882,   212, 10882, 10882, 10882, 10882, 10882, 10882, 10882, 10882,
   10882,  8341, 10882, 10882, 10882, 10882, 10882, 10882, 10882, 10882,
   10882, 10882, 16274, 10882, -1599,  8572, 10882,  1070, -1599,  4171,
   -1599,  1114,  2022,   715,   727,  1047,   398, -1599,   728,   793,
   -1599, -1599,  1072,   795,   -65,   800,   -65,   803,   -65, -1599,
     154, -1599,   363, -1599, 12001,  1056, -1599, -1599, 14601,   285,
   -1599,   601, 12001, -1599, -1599, 12001, -1599, -1599, 12329,  1031,
    1234, -1599, -1599,   -16, -1599, -1599, -1599, 15063,   580,  4171,
   -1599,  1060, 12176,  1264, 10882, 15582,  1083, 15063,  1067, -1599,
    1071,  1098, 15582, -1599, 12001,  4171, -1599, 12176,  1048, -1599,
     978, -1599, -1599, -1599, 15063, -1599, -1599, 15063, -1599,   579,
   -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599,   141,   227,
   -1599,  4633,  4633,  4633,  4633,  4633,  4633,  4633,  4633,  4633,
    4633,  4633, 10882,  4633,  4633,  4633,  4633,  4633,  4633, -1599,
     601, 15172,  1093,   351,   767,  1225,   593, 12001, 12001, 12001,
    5796,  8803,  1096, 10882, 12001, -1599, -1599, -1599, 12001,  1046,
    2077,  1053, 12365, 12001, 12001, 12471, 12001, 12507, 12001,  1046,
   12001, 12175,  1046,  1023,   220, 12613, 12649, 12755, 12791, 12897,
   12933,    24,   227,  3246,  4866,  6027, 15314,  1081,     5,   468,
    1082,   387,    25,  6258,     5,   783,    29, 10882,  1090, 10882,
   -1599, -1599, 12001, 12001, -1599, 10882,   753,    46, -1599, 10882,
   -1599,    53,   580, -1599, 10882, -1599, 10882, -1599, 10882, -1599,
   10882,  1057,   498, -1599, -1599,  1061,  1064,   103, -1599, -1599,
     -72,  5099, -1599,   278,  1059,  1066,   -28,   794,  1086,  1075,
   -1599, -1599,  1091,  1076, -1599, -1599,  1734,  1734, 11644, 11644,
     698,   698,  1077,   453,  1080, -1599, 14637,   -49,   -49,   969,
    1734,  1734, 15772, 15716, 15811, 15677, 16220, 15409, 15867, 11305,
   11470, 11644, 11644,   812,   812,   453,   453,   453,   674, 10882,
    1084,  1085,   714, 10882,  1312,  1088, 14743, -1599,   360, 13039,
   -1599, -1599,  2022, 10882, 10882, 10882, 10882, 10882, 10882, 10882,
   10882, 10882, 10882, 10882, 10882, 10882, 10882, 10882, 10882, 10882,
   -1599, -1599, -1599, -1599, 12001, -1599, -1599, -1599,   367, -1599,
    1099, -1599,  1100, -1599,  1101, -1599, 12175, -1599,  1023,   433,
     876, -1599,  1097, -1599, 10882, -1599, -1599,   876,   876, -1599,
   10882,  1130,   585, 12001, -1599, 10882, -1599,    63, -1599,  1060,
   10882,   579, 15582,  1115, -1599, -1599, -1599, -1599,   316, -1599,
   12176, -1599,    71, -1599,   887, 10882, -1599,   978,  1133,  1133,
   -1599, -1599, -1599,   227,   227,   227, -1599, -1599,  1619, -1599,
    1619, -1599,  1619, -1599,  1619, -1599,  1619, -1599,  1619, -1599,
    1619, -1599,  1619, -1599,  1619, -1599,  1619, -1599,  1619, 15582,
   -1599,  1619, -1599,  1619, -1599,  1619, -1599,  1619, -1599,  1619,
   -1599,  1619, -1599, -1599,  1125,   593, -1599, -1599,    88, -1599,
      39, -1599,   995,  1165,   811,   722,   395,  1106,  1107,  1146,
   13075,   298, 13181,   824, 12001, 12175,  1023,  2117,  1108,  1110,
   12001, -1599, -1599,  2697,  3624, -1599,  3855, -1599,  4086,  1111,
    4317,   442,  1113,   488,    71, -1599, -1599, -1599, -1599, -1599,
    1116, 10882, -1599,  5332, 14495,     4, 10882,   498,   722,   468,
   -1599, -1599,  1112, -1599, 10882, 10882, -1599,  1118, -1599, 10882,
     722,   721,  1119, -1599, -1599, 10882, 15582, -1599, -1599,   537,
     538, 15448, 10882, -1599, 10882,    67, 15582, 13217, 15582, 15582,
   -1599,  1120,   -13, 10882, 10882, 12001,   794,    16, -1599,  1123,
     458, 11113, -1599, -1599,   -28,  1159,  1169,  1121,  1170,  1176,
   -1599,   515,   -65, -1599, 10882, -1599, 10882,  9034, 10882, -1599,
    1147,  1134, -1599, -1599, 10882,  1135, -1599, 14779, 10882,  9265,
    1136, -1599, 14885, -1599,  9496, -1599, -1599, -1599, -1599, 15582,
   15582, 15582, 15582, 15582, 15582, 15582, 15582, 15582, 15582, 15582,
   15582, 15582, 15582, 15582, 15582, 15582, -1599, -1599, -1599,   876,
   -1599, -1599,  1183, -1599,  1186, -1599, -1599, -1599, -1599, -1599,
   -1599, -1599, -1599, -1599, -1599, -1599, 12001, -1599, 15172, 13323,
   -1599,  1137,  1342,   -76, 15582, 10882, -1599, 12001, 10882,    71,
     794,   579, -1599, -1599, 10882, -1599,  1343,  2551,    71, -1599,
     541,   408, -1599, -1599, -1599, 12001, -1599,  1355,    39, -1599,
   -1599,   767, -1599, -1599, -1599,  1149, -1599, -1599, -1599,   592,
   -1599,  1195,  1160, -1599, -1599,  4548,   676,   677, -1599, -1599,
   10882,  4630, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599,
   -1599,  1164,  9727,   659,     5,   468, 15582,  1081, -1599, -1599,
   15582,  1082, -1599,   703,     5,  1166, -1599, -1599, -1599, -1599,
     734, -1599, -1599, -1599,  1202,   758,   780, 10882,   137, 10882,
   10882, 10882, 13359, 13465,  4779,   -65, -1599,  1171,  5099,   428,
   -1599, -1599,  1211, -1599, -1599,   -28,  1174,   121, 12001, 13501,
   12001, 13607, -1599,   432, 13643, -1599, 10882,  1829, 10882, -1599,
   13749,  5099, -1599,   440, 10882, -1599, -1599, -1599,   452, -1599,
   -1599, -1599, -1599, -1599, -1599,  1175,   876, -1599, -1599, 10882,
    1214, 10882,   200,   876, 15582,   891,   -65, -1599, 15063, -1599,
     593, -1599,   794,  1215,  1178,   335,   332, -1599, -1599,  1355,
     580,  1179,  1180, -1599, -1599, 10882,  1218,  1196, 10882, -1599,
   -1599, -1599, -1599,  1182,  1187,  1189, 10882, 10882, 10882,  1194,
    1358,  1199,  1201,  9958, -1599,   502, 10882,   468, -1599, 10882,
     721, -1599, 10882, 10882,  1175, -1599, -1599, 10882, 10882,   781,
   10882, 10882, 13785, 15582, 15582, -1599, -1599, -1599,  1224, -1599,
     558, -1599,  1206, -1599, -1599, 10189, -1599, -1599,  4863, -1599,
     826, -1599, -1599, -1599, 12001, 13891, 13927, -1599,   559, -1599,
   14033, -1599, -1599, 14069, -1599, 15582, -1599, -1599,   121,   887,
    3940, -1599,   -65, -1599,   639, 12001,   -52, -1599, 16375, -1599,
   -1599, -1599, -1599,  1358,  1358, 14175,  1226,  1209, 14211,  1210,
    1212,  1216, 10882, -1599, 10882, 11644, 11644, 11644, 10882, -1599,
   -1599,  1358,  1358, -1599, 14317, -1599, 15543, -1599, 15543, -1599,
    1228, 11644, -1599,  1223,  1228, 15543, 10882, 15582, 15582,   152,
     214, -1599,  1221, -1599, 10882, 15677, -1599, -1599,   833, -1599,
   -1599,  1229, -1599, -1599, -1599, -1599, 10420, 10651, -1599, -1599,
   -1599, -1599, -1599, 15582,   579, 12001,   -52,   912,  4171,   593,
   16375,   198,   198, -1599, 10882, 10882, -1599,  1358,  1358,   722,
    1231,  1232,  1046,   198,   722, -1599,  1400,  1220,  1250,  1260,
   -1599,  1262,  1235, 15543, 10882, 10882, -1599,   214, -1599,  1829,
   -1599, -1599, -1599, -1599, 10882, 10882, 15582, -1599,   912,  4171,
    4171, -1599,  2022, -1599,   579,   722,  1081,  1271, -1599,  1236,
    1237, 14353, 14459,   198,   198,  1081,  1245, -1599, -1599,  1255,
    1256,  1257, 10882,  1252,  1265,  1292, -1599, -1599,  1266, 15582,
   15582, -1599, -1599, 15582,  4171, -1599,  2022, -1599,  2022, -1599,
   -1599,   516,  1261, -1599, -1599, -1599, -1599, -1599,  1270,  1272,
   -1599, -1599, -1599, -1599, 15582, -1599, -1599, -1599, -1599, -1599,
    2022, -1599, -1599, -1599,   722, -1599, -1599, -1599,   524, -1599
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   151,     1,   385,     0,     0,     0,   717,   386,     0,
     709,   709,   709,    74,    75,     0,     0,    15,     3,     0,
      10,     9,     8,    16,     0,     7,   697,     6,    11,     5,
       4,    13,    12,    14,   114,   115,   113,   129,   131,    45,
      64,    61,    62,     0,    47,    48,    49,     0,     0,    50,
      59,    46,   301,   300,   709,   709,    22,    21,   697,   711,
     710,   915,   905,   910,     0,   353,    43,   137,   138,   139,
       0,     0,     0,   140,   142,   149,     0,   136,    17,   735,
     734,   291,   719,   738,   698,   699,     0,     0,     0,     0,
       0,     0,    51,     0,     0,    60,     0,     0,    57,     0,
       0,   709,     0,    18,     0,     0,     0,   355,     0,     0,
     148,   143,     0,     0,     0,     0,     0,     0,   152,   737,
     736,   292,   294,   721,   720,     0,   740,   739,   743,   701,
     700,   703,   127,   128,   119,   120,   123,   125,   117,     0,
       0,     0,     0,   116,   132,    65,    63,    53,    54,    52,
      59,    56,    55,   712,   714,   303,   302,   716,     0,   718,
      19,    20,    23,   916,   906,   911,   354,    41,    44,   147,
       0,   144,   145,   146,   150,   296,   295,   298,   293,   722,
       0,   731,   693,   622,    26,    27,    31,     0,   124,   121,
     122,   126,   109,   110,   101,   102,   105,   107,    99,     0,
       0,    98,   111,     0,     0,    58,     0,   715,     0,    25,
     821,     0,     0,    42,   141,     0,     0,   723,   732,     0,
     744,   695,     0,     0,   624,     0,    28,    29,    30,   106,
     103,   104,   108,     0,     0,   130,   118,     0,    24,     0,
       0,   907,   912,     0,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,     0,     0,   159,   153,     0,   802,   805,   808,
     809,   803,   806,   804,   807,     0,   705,   729,   741,   694,
     702,   622,     0,   133,     0,   135,     0,   682,   680,   704,
     100,   112,     0,     0,     0,     0,     0,     0,   752,   795,
     753,   811,   754,   758,   759,   760,   761,   801,   765,   766,
     767,   768,   769,   770,   771,   796,   797,   798,   799,   875,
     757,   764,   800,   882,   889,   755,   762,   756,   763,   772,
     773,   774,   775,   776,   777,   778,   779,   780,   781,   782,
     783,   784,   785,   786,   787,   788,   789,   790,   791,   792,
     793,   794,     0,     0,     0,     0,   810,   836,   839,   837,
     838,   902,   713,   824,   825,   822,   823,   917,   659,   665,
     237,   238,   235,   162,   163,   165,   164,   166,   167,   168,
     169,   195,   196,   193,   194,   186,   197,   198,   187,   184,
     185,   236,   219,     0,   234,   199,   200,   201,   202,   173,
     174,   175,   170,   171,   172,   183,     0,   189,   190,   188,
     181,   182,   177,   176,   178,   179,   180,   161,   160,   218,
       0,   191,   192,   622,   156,   330,   299,   726,   724,     0,
     733,   636,   745,     0,     0,   134,     0,     0,     0,     0,
     681,     0,   842,   866,   869,     0,   872,   862,     0,     0,
     876,   883,   890,   896,   899,     0,   336,   852,   857,   851,
       0,   865,   861,   854,     0,     0,   856,   840,     0,   817,
     908,   913,   239,   240,   233,   217,   241,   220,   203,     0,
     154,   384,   650,   651,     0,     0,     0,   297,     0,   705,
       0,   706,     0,   730,   640,   696,   623,     0,     0,   527,
     528,     0,     0,     0,     0,   521,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   801,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   611,     0,     0,
       0,   442,   444,   443,   446,   447,   448,   449,   450,     0,
       0,    37,   338,     0,     0,     0,     0,     0,   334,     0,
     424,   425,   525,   524,   605,   522,   596,   595,   594,   593,
     151,   599,   523,   598,   597,   569,   529,   570,     0,   530,
       0,   526,   920,   924,   921,   922,   923,   684,     0,   685,
       0,   678,   679,   677,     0,     0,     0,     0,     0,     0,
       0,     0,   827,     0,   153,     0,   153,     0,   153,     0,
       0,     0,   847,   334,   846,   859,   860,   853,   855,     0,
     858,   841,     0,     0,   904,   903,   918,   353,   830,   831,
       0,   660,   655,     0,     0,     0,   666,     0,   242,   222,
     223,   225,   224,   226,   227,   228,   229,   221,   230,   231,
     232,   206,   207,   209,   208,   210,   211,   212,   213,   204,
     205,   214,   215,   216,     0,   382,   383,     0,   622,   622,
     622,   155,   158,   157,   332,     0,    76,    77,    89,   370,
     368,     0,     0,    96,     0,     0,   369,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   309,
       0,     0,   325,   320,   317,   316,   318,   319,   304,   352,
     331,   311,   605,   310,     0,    84,    85,    82,   323,    83,
     324,   326,   388,   315,     0,   312,   451,   727,     0,   707,
     725,   638,     0,   637,     0,   742,   622,   966,   969,   358,
     362,   361,   367,     0,     0,   410,     0,     0,     0,  1002,
       0,     0,   338,     0,   401,   404,     0,   407,     0,  1006,
       0,   975,   984,     0,   972,     0,   557,   558,     0,     0,
       0,     0,     0,     0,     0,   944,     0,     0,     0,   982,
    1009,     0,   342,   345,     0,     0,     0,  1011,  1020,   445,
     534,   533,   571,   532,   531,     0,     0,     0,  1020,   419,
       0,   353,  1020,  1020,     0,   426,   603,     0,   434,     0,
       0,     0,     0,   559,   560,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   511,     0,   683,     0,     0,     0,   688,     0,
     692,     0,   451,     0,     0,     0,   832,   845,     0,     0,
     812,   826,     0,     0,   156,     0,   156,     0,   156,   657,
       0,   663,     0,   813,     0,  1020,   850,   834,     0,     0,
     818,     0,     0,   661,   909,     0,   667,   914,     0,     0,
     746,   647,   648,   670,   652,   654,   653,     0,     0,     0,
     374,   371,   419,     0,     0,   356,     0,     0,     0,   329,
       0,     0,    68,    91,     0,     0,   379,   376,   425,   437,
     151,   351,   349,   350,     0,   327,   328,     0,    87,     0,
     440,   307,   314,   321,   322,   373,   378,   387,     0,     0,
     313,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   306,
       0,     0,     0,     0,   630,   633,     0,     0,     0,     0,
     958,     0,     0,     0,     0,   992,   995,   998,     0,  1020,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1020,
       0,     0,  1020,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   978,   936,   944,     0,
     987,     0,     0,     0,   944,     0,     0,     0,     0,     0,
     947,  1014,     0,     0,    40,     0,    38,     0,  1013,  1021,
     339,     0,     0,   989,  1021,   335,     0,   669,     0,   668,
       0,     0,  1021,   935,   562,     0,     0,   498,   495,   497,
       0,   334,   514,     0,     0,     0,     0,   153,     0,     0,
     582,   581,     0,     0,   583,   587,   535,   536,   548,   549,
     546,   547,     0,   576,     0,   567,     0,   600,   601,   602,
     537,   538,   553,   554,   555,   556,     0,     0,   551,   552,
     550,   544,   545,   540,   539,   541,   542,   543,     0,     0,
       0,   504,     0,     0,     0,     0,     0,   519,     0,     0,
     687,   690,   451,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     691,   843,   867,   870,     0,   873,   863,   814,     0,   877,
       0,   884,     0,   891,     0,   897,     0,   900,     0,     0,
     340,  1021,     0,   835,     0,   819,   919,   656,   662,   649,
       0,     0,     0,     0,   671,     0,    92,     0,   375,   372,
       0,     0,   357,     0,    93,    94,    66,    67,     0,   380,
     377,   426,   434,   333,    71,     0,   330,   151,     0,     0,
     400,   399,   348,     0,     0,     0,   473,   482,   461,   483,
     462,   485,   464,   484,   463,   486,   465,   476,   455,   477,
     456,   478,   457,   487,   466,   488,   467,   475,   453,   454,
     489,   468,   490,   469,   479,   458,   480,   459,   481,   460,
     474,   452,   728,   708,     0,   152,   631,   632,   633,   634,
     625,   641,     0,     0,     0,   959,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1015,   572,     0,     0,   573,     0,   604,     0,     0,
       0,     0,     0,     0,   434,   606,   607,   608,   609,   610,
       0,     0,   945,     0,   419,   944,     0,     0,     0,     0,
     953,   954,     0,   961,     0,     0,   951,     0,   990,     0,
       0,     0,     0,   949,   991,     0,   981,   946,  1010,     0,
       0,    34,     0,  1012,     0,     0,   420,     0,   926,   925,
     561,     0,     0,     0,     0,     0,   153,     0,   515,     0,
       0,     0,   518,   516,     0,     0,     0,     0,     0,     0,
     432,     0,   156,   578,     0,   584,     0,     0,     0,   565,
       0,     0,   588,   592,     0,     0,   568,     0,     0,     0,
       0,   505,     0,   512,     0,   563,   520,   686,   689,   461,
     462,   464,   463,   465,   455,   456,   457,   466,   467,   453,
     468,   469,   458,   459,   460,   452,   844,   868,   871,   833,
     874,   864,     0,   828,     0,   878,   880,   885,   887,   892,
     894,   898,   658,   901,   664,   848,     0,   337,     0,     0,
     748,     0,   749,   673,   672,     0,   381,     0,     0,   434,
     153,     0,    69,    70,     0,    86,    78,     0,   434,   389,
       0,     0,   472,   470,   471,     0,   646,   628,   625,   626,
     627,   630,   967,   970,   359,     0,   364,   365,   363,     0,
     413,     0,     0,   416,   411,     0,     0,     0,  1003,  1001,
     338,     0,   402,   405,   408,  1007,  1005,   976,   985,   983,
     973,     0,     0,     0,   944,     0,   979,   937,   960,   952,
     980,   988,   950,     0,   944,     0,   956,   957,   964,   948,
       0,   343,   346,    35,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   156,   517,     0,   334,     0,
     429,   430,     0,   428,   427,     0,     0,     0,     0,     0,
       0,     0,   493,     0,     0,   589,     0,   577,     0,   566,
       0,   334,   506,     0,     0,   564,   513,   509,     0,   816,
     829,   815,   881,   888,   895,   336,   341,   820,   747,     0,
       0,     0,     0,    97,    95,     0,   156,    72,     0,    79,
       0,   305,   153,     0,     0,   680,     0,   629,   642,   628,
       0,     0,     0,   360,   366,     0,     0,     0,     0,   412,
     993,   996,   999,     0,     0,     0,     0,     0,     0,     0,
     958,     0,     0,     0,   612,     0,     0,     0,   962,     0,
       0,   955,     0,     0,   336,    32,    39,     0,     0,     0,
       0,     0,     0,   928,   927,   496,   621,   499,     0,   491,
       0,   436,     0,   433,   435,     0,   421,   439,     0,   620,
       0,   618,   494,   615,     0,     0,     0,   614,     0,   507,
       0,   510,   849,     0,   751,   674,    90,   308,     0,    71,
       0,    88,   156,   390,   680,     0,     0,   639,     0,   644,
     676,   675,   635,   958,   958,     0,     0,     0,     0,     0,
       0,     0,   334,  1016,   338,   403,   406,   409,     0,   959,
     977,   958,   958,   574,     0,   613,  1018,   963,  1018,   965,
    1018,   344,   347,    36,  1018,  1018,     0,   930,   929,     0,
       0,   502,     0,   431,     0,   422,   579,   585,     0,   619,
     617,     0,   616,   750,   438,    73,   370,     0,    80,    84,
      85,    82,    83,    81,     0,     0,     0,     0,     0,     0,
       0,   943,   943,   414,     0,     0,   417,   958,   958,   933,
       0,     0,  1020,   943,   933,   575,     0,     0,     0,     0,
      33,     0,     0,  1018,     0,     0,   500,     0,   492,   423,
     580,   586,   590,   508,     0,     0,   376,   441,     0,     0,
       0,   398,   451,   643,     0,     0,   940,  1020,   942,     0,
       0,     0,     0,   943,   943,   934,     0,  1004,  1017,     0,
       0,     0,     0,     0,     0,     0,  1026,  1022,     0,   932,
     931,   503,   591,   377,     0,   396,   451,   394,   451,   397,
     645,     0,  1021,   941,   968,   971,   415,   418,     0,     0,
    1000,  1008,   986,   974,  1019,  1024,  1025,  1027,  1023,   392,
     451,   395,   393,   938,     0,   994,   997,   391,     0,   939
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1599, -1599, -1599, -1599, -1599, -1599, -1599,   704,  1428, -1599,
   -1599, -1599, -1599, -1599, -1599,  1517, -1599, -1599, -1599,   863,
     253, -1599,  1369, -1599, -1599,  1431, -1599, -1599, -1599,  -148,
      -1, -1599, -1599, -1599,  -147, -1599, -1599, -1599, -1599, -1599,
   -1599, -1599, -1599, -1599, -1599,  1288, -1599, -1599,   -40,   -34,
   -1599, -1599, -1599,   679,   772,  -513,  -573,  -852, -1599, -1599,
   -1599, -1599, -1598, -1599, -1599,    18,  -214,  -277,  -504, -1599,
     320, -1599,  -632, -1432,  -766,   -64,  -485, -1599, -1599, -1599,
   -1599,  -610,     0, -1599, -1599, -1599, -1599, -1599,  -145,  -143,
    -142, -1599,  -141, -1599, -1599, -1599,  1529, -1599,   322, -1599,
   -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599,
   -1599, -1599,  -318,  -136,  -133,     1,   181, -1164,  -688, -1599,
    -719, -1599, -1599,  -516,   735, -1599, -1599, -1599, -1457, -1599,
   -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599,   761, -1599,
   -1599, -1599, -1599, -1599, -1599, -1599,  -163,    83,   -47,    82,
     286, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599,   501,
    -477,  -983, -1599,  -480,  -978, -1599,  -896,   -45,   -44, -1599,
    -607, -1521, -1599,  -434, -1599, -1599,  1489, -1599, -1599, -1599,
    1036,  1185,   164, -1599, -1599, -1599, -1599, -1599, -1599, -1599,
   -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599,
   -1599, -1599, -1599,  -746,  -197, -1599,  1026, -1599, -1599, -1599,
    1238, -1599, -1599, -1599,  -439, -1599, -1599,  -438, -1599, -1599,
   -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599,
   -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599,
   -1599, -1599, -1599, -1599, -1599, -1599,  -225, -1599, -1599, -1599,
   -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599,
   -1599, -1599,  1042,  -730,  -205,  -797,  -775, -1599, -1599, -1219,
    -991, -1599, -1599, -1599, -1285,   -46, -1047, -1599, -1599, -1599,
   -1599, -1599, -1599, -1599, -1599, -1599,   245,  -552, -1599, -1599,
   -1599,   779, -1599, -1599, -1599, -1599, -1599, -1599, -1599, -1599,
   -1599, -1599, -1599, -1599, -1599, -1599, -1209,  -813, -1599
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    17,   162,    58,   209,    18,   187,   201,  1713,
    1514,  1625,   804,   582,   168,   583,   109,    20,    21,    49,
      50,    51,    98,    22,    41,    42,   717,   718,  1444,  1445,
     649,   720,  1580,  1670,   721,   722,  1205,   723,   918,   724,
     725,   726,   727,  1438,   926,   202,   203,    37,    38,    39,
     224,    73,    74,    75,    76,    24,   454,   517,   295,   122,
      25,   177,   296,   178,   215,   455,   157,   939,  1216,   730,
     518,   731,   817,   634,   806,  1169,   584,  1042,  1623,  1043,
    1624,   733,   585,   734,   760,   989,  1593,   586,   735,   736,
     737,   738,   739,   740,   741,   687,   742,   958,  1450,  1210,
     743,   587,  1003,  1606,  1004,  1607,  1006,  1608,   588,   994,
    1599,   589,   818,  1647,   590,  1360,  1361,  1077,   941,   591,
     979,  1207,   592,   871,  1217,   745,   593,   594,  1069,   595,
    1345,  1720,  1346,  1777,   596,  1124,  1556,   597,   819,  1538,
    1780,  1540,  1781,  1654,  1822,   599,   511,  1461,  1588,  1258,
    1260,   986,   524,   982,   756,  1678,  1750,   512,   513,   514,
     889,   890,   500,   891,   892,   501,  1060,   911,   912,  1682,
     614,   471,   318,   319,   221,   311,    85,   131,    27,   183,
     458,    99,   100,   206,   101,    28,    55,   125,   180,    29,
     306,   522,   519,   980,   460,   219,   220,    83,   128,   462,
      30,   181,   308,   913,   600,   305,   388,   389,  1158,   646,
     240,   390,   882,  1560,  1166,   875,   497,   391,   615,  1406,
     894,  1565,   620,  1411,   616,  1407,   617,  1408,   619,  1410,
     623,  1415,   624,  1562,   625,  1417,   626,  1563,   627,  1419,
     628,  1564,   629,  1421,   630,  1423,   652,    31,   105,   211,
     398,   653,    32,   106,   212,   399,   657,    33,   104,   210,
     499,   901,   601,   823,  1806,   824,  1028,  1797,  1798,  1799,
    1029,  1041,  1324,  1318,  1313,  1508,  1268,   602,   987,  1591,
     988,  1592,  1013,  1612,  1010,  1610,  1030,   807,   603,  1011,
    1611,  1031,   604,  1274,  1689,  1275,  1690,  1276,  1691,   998,
    1603,  1008,  1609,   801,   808,   605,  1767,  1050,   606
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      23,   895,   744,   869,   870,  1055,  1001,   310,   800,  1061,
    1063,   754,   392,   951,   729,    66,    77,  1185,    78,   456,
     225,   655,  1034,   650,  1498,    54,   942,   943,  1291,   525,
    1074,   610,  1160,   732,  1162,  1293,  1164,  1309,  1440,   766,
     387,    34,    35,  1321,  1301,  1319,  -151,   144,  1021,  1325,
    1032,   884,  1036,   886,   635,   888,     2,  1022,  1022,  1047,
     645,   651,   656,     3,  1676,  1027,  1332,  1027,  -879,  1051,
      77,    77,    77,  1334,  1057,  1075,    94,   828,   751,   170,
    1749,   637,  1172,  1435,   515,  1459,     4,  1517,     5,   920,
       6,    64,   841,   842,  -886,   826,     7,    87,   611,   154,
     839,   159,   936,   841,   842,    13,     8,    40,   612,   820,
     729,    95,     9,   222,    77,    77,    77,    77,   829,   830,
     795,   797,  1355,    65,   214,    59,  1347,    14,    52,   732,
    1491,    60,  1356,  1662,  1057,  1457,    10,  1057,  -879,   827,
     509,  1059,  -893,  -879,   498,   222,   487,  1520,   463,   182,
      53,   509,  1794,  1746,   516,  1259,   222,   207,    84,   767,
     768,  -879,   314,    56,  -886,   613,  1057,   763,  1460,  -886,
    1357,  1348,  1187,   488,   489,   223,  1058,   132,   133,   862,
     863,    11,    12,   313,   778,   820,  1278,  -886,   312,  1358,
     862,   863,  1712,  1267,  1359,   729,  1289,   117,   217,  1292,
    1182,  1059,  1347,  1183,  1059,   510,  1184,   223,   729,    57,
    1618,   903,  -893,    86,   732,  1308,   906,  -893,   223,   102,
      36,    88,   118,   315,   685,   155,    13,   732,    59,   241,
     242,   192,   193,  1059,    60,  -893,   123,   386,   833,   834,
    1182,  1182,   124,   636,    13,  1182,   839,   156,    14,   841,
     842,   843,  -501,   487,  1076,   155,   844,   769,   397,  1131,
     490,   822,  1182,  1776,   491,   158,    14,   686,   108,  1182,
     638,  1212,   387,  1349,   465,  1575,   770,   156,   752,  1182,
     488,   489,    96,  1182,  1582,    15,  1645,  1057,   639,  1057,
     692,   693,  1476,    97,   640,   959,    16,  1630,  1477,  1058,
      92,   387,  1057,   387,   728,   108,  1335,    52,   750,  1188,
     755,   126,  1774,    90,  1495,    87,   316,   127,   387,   387,
    1821,   902,  -501,   457,  1208,  1199,   461,  -501,    13,    53,
      87,   492,  1707,  1000,  1302,   493,   862,   863,   494,   317,
     464,  1350,    13,   147,   148,  -501,   149,  1014,   234,   487,
      14,   134,   135,   495,  1059,   136,  1059,   137,    89,   496,
     138,   387,   387,   139,    14,   729,    52,   490,   107,  1059,
     648,   491,   114,   115,   116,   235,   488,   489,  1165,   820,
      64,   921,   632,  1209,   732,   486,    52,   140,    53,  1065,
     873,   874,   876,  1094,   878,   879,  1347,    64,   883,   216,
     885,   633,   887,    93,   141,   194,   195,   142,    53,   196,
    1095,   197,    65,    52,   198,   729,  1190,   139,   387,   387,
     387,   108,   387,   387,   923,    13,   387,  1202,   387,    65,
     387,   729,   387,   933,   732,    53,    13,  1154,   492,   904,
    1795,   199,   493,   907,  1294,   494,  1521,    14,   169,   129,
     732,   771,   999,  1168,   748,   130,   119,   521,    14,   523,
     495,   200,  1009,   490,   689,  1012,   496,   491,  1465,   386,
     772,   580,   938,    13,   829,   830,   732,   732,   732,   732,
     732,   732,   732,   732,   732,   732,   732,  1328,   732,   732,
     732,   732,   732,   732,  1351,    14,  1198,  1333,   386,  1768,
     386,  1769,  1174,  1617,  1362,  1771,  1772,    13,  1471,  1073,
    1537,   688,   690,  1620,    67,   386,   386,   719,   113,   749,
      13,  1352,  1472,   753,  1211,  1452,  1453,  1454,   145,    14,
    1494,   386,   764,  1800,   492,   648,   120,  1571,   493,    64,
    1439,   494,    14,    68,  1810,  1504,   316,  1128,  1065,  1262,
    1263,    13,   498,  1066,  1675,    13,   495,   470,   386,   386,
    1277,  1280,   496,  1700,  1818,  1283,  1284,   117,  1286,   317,
    1288,    65,  1290,    14,   387,  1027,  1190,    14,  1497,   648,
     940,   940,   940,  1412,  1838,  1839,    13,  1167,   387,   121,
    1027,  1413,  1255,   985,   833,   834,  1067,  1315,    97,   950,
    1316,  1189,   839,  1386,   840,   841,   842,   843,    14,   868,
      69,  1190,   844,   950,   648,   386,   386,   386,   498,   386,
     386,    13,  1631,   386,  1583,   386,   167,   386,  1317,   386,
      13,    13,  1081,  1085,   150,    43,  1751,  1752,  1466,    70,
    1186,   654,   153,    14,  1535,   900,    64,  1099,  1190,   648,
    1194,  1585,    14,    14,  1763,  1764,  1190,  1425,   648,   648,
      44,    45,    46,   155,   309,  1125,  1488,  1203,  1190,  1170,
    1204,  1641,  1266,  1638,  1307,  1652,    13,  1177,    65,  1341,
    1178,  1310,  1311,  1659,   779,   156,   204,  1176,  1424,  1422,
      91,    47,   862,   863,  1448,  1661,  1342,   387,    14,  1527,
     608,    48,    43,   780,   648,   387,   929,   822,   387,  1312,
    1803,  1804,  1490,    71,  1604,   822,   945,   946,  1190,   829,
     830,   163,    72,   609,  1668,    13,    13,    44,    45,    46,
    1426,  1535,  1307,   952,   953,   954,   955,   387,   956,   950,
    1307,    79,    80,   960,    81,  1705,   521,    14,    14,   110,
     111,   112,    77,   648,   648,   909,  1536,  1583,    47,  1853,
    1431,  1511,  1512,   991,  1264,    52,  1252,  1859,    48,  1273,
     160,   386,    82,  1525,  1307,  1307,   161,   114,   910,   116,
      13,    13,  1584,  1432,   164,   386,   165,    53,   155,   950,
     387,   387,   387,   171,   172,   173,   174,   387,  1640,  1722,
    1731,   387,    14,    14,   950,    13,   387,   387,   648,   387,
     156,   387,   782,   387,   387,   520,  1595,  1170,  1170,   166,
    1744,  1658,   175,  1775,  1256,  1068,   940,    14,   176,   468,
    1257,   783,   469,   829,   830,   470,  1475,   831,   832,   833,
     834,   835,  1481,   992,   836,   387,   387,   839,   393,   840,
     841,   842,   843,   179,  1168,  1375,  1667,   844,  1745,   845,
     846,   470,   993,   394,    13,    13,    13,  1576,   395,  1616,
     396,  1150,  1376,   580,   938,  1190,   899,   671,   672,   673,
     674,   675,   676,   677,   678,    95,    14,    14,    14,   940,
    1426,  1426,   648,   648,   386,  1380,   679,   208,  1175,  1441,
    1601,  1602,   386,   820,   239,   386,   680,  1524,  -821,  1373,
    1442,  1443,  1381,  1619,  1436,   114,   681,   682,   683,  1190,
    1347,   855,   856,   857,   858,   859,   860,   861,  1761,  1409,
    1505,   744,   213,  1506,   386,   498,  1507,   862,   863,  1151,
     188,   189,   190,   729,  1622,   487,   191,   498,   498,  1809,
    1190,  1152,  1155,   833,   834,   226,   227,   387,  1433,   151,
     152,   839,   732,   840,   841,   842,   843,  1206,  1627,   387,
     218,   844,   488,   489,  1190,   229,   230,   231,  1329,  1330,
    1253,   232,   233,  1493,  1833,  1261,   387,   386,   386,   386,
    1628,  1716,  1322,  1315,   386,  1323,  1190,  1190,   386,   950,
     236,  1503,   237,   386,   386,   238,   386,  1510,   386,  1672,
     386,   386,   307,   498,  1515,   498,  1516,  1156,   239,  1159,
     498,  1796,  1796,   498,  1161,   320,   451,  1163,   487,  1805,
    1596,   498,   452,  1796,  1805,  1464,   453,   459,  1168,   859,
     860,   861,   386,   386,   498,   467,   498,   466,  1474,  1543,
    1727,   862,   863,   498,   472,   488,   489,  1782,   473,   490,
    1760,  1553,   611,   491,   475,  1831,  1558,   950,   474,  1748,
    1710,   476,   612,  1796,  1796,  1714,   477,   387,   387,   478,
     940,   940,   940,   387,   480,   950,   485,   950,   481,   950,
     482,   950,   483,   950,   484,   950,   498,   950,   502,   950,
    1648,   950,   297,   950,   503,   950,   298,   504,   950,   505,
     950,   506,   950,   507,   950,   508,   950,  1572,   950,   526,
     299,   300,   607,   621,  1858,   301,   302,   303,   304,   613,
     492,  1388,   643,  1613,   493,   580,   938,   494,  1791,  1789,
    1790,   622,   490,   114,   115,   116,   491,   658,   387,    44,
      45,    46,   495,   684,   386,   757,   691,  1414,   496,   184,
     185,  1044,  1045,   758,  1577,   765,   386,   774,   527,   528,
     184,   185,   186,   773,  1615,   226,   227,   228,   775,  1825,
    1827,  1824,   777,   386,  1038,  1039,  1040,   776,   534,   914,
     915,   916,   778,   781,   536,    61,    62,    63,   487,  1629,
     784,  1566,   785,   788,   789,   790,   791,   792,   793,   802,
     803,   865,  1573,   492,  1849,   867,   825,   493,    16,  1462,
     494,   654,   866,   881,   880,   488,   489,   598,   905,   387,
    1586,   543,   544,  1581,   896,   495,   618,  1747,   928,   930,
     387,   496,   934,   948,   729,   949,   631,   957,   984,   990,
    1048,   995,   996,   997,  1456,   487,   642,  1052,   387,  1049,
    1646,  1053,  1054,   732,  1056,  1062,  1092,  1064,  1130,   959,
    1157,  1153,  1171,  1180,   386,   386,  1190,  1181,  1191,   746,
     386,  1193,   488,   489,  1195,   729,   729,  1197,   546,   547,
    1196,  1254,  1259,  1201,  1271,  1666,  1281,  1307,  1314,  1327,
    1340,  1669,  1353,  1343,   732,   732,  1344,  1788,  1354,  1363,
     786,   787,   490,  1383,  1365,  1650,   491,  1364,  1366,  1367,
     729,   799,  1368,  1416,  1418,  1420,  1378,  1379,  1430,    64,
    1384,  1449,  1437,   799,   810,   811,   812,   813,   814,   732,
    1427,   387,  1455,   387,  1469,   386,   558,   559,   560,  1467,
    1468,  1479,  1480,  1499,  1486,  1579,  1489,  1530,  1492,  1502,
    1509,    65,  1519,  1532,   829,   830,  1526,  1531,  1533,   490,
    1545,   572,   611,   491,  1534,   872,  1546,  1548,  1554,  1569,
    1762,  1559,   612,   492,  1561,  1570,   759,   493,  1587,  1463,
     494,  1646,  1594,  1597,   659,   660,   661,   662,   663,   664,
     665,   666,  1598,   578,   898,   495,  1613,  1621,  1626,  1642,
      13,   496,  1664,  1673,  1639,  1644,  1686,   633,   921,  1674,
    1687,  1683,  1684,   667,  1692,  1770,   386,  1567,  1699,  1728,
    1693,  1694,    14,   668,   669,   670,  1698,   386,  1766,   613,
     492,  1701,   950,  1702,   493,   908,   719,   494,  1721,  1723,
    1754,  1755,  1757,  1812,  1758,   386,   917,   387,  1759,  1814,
     922,  1813,   495,   925,  1778,   927,  1189,  1787,   496,  1815,
     932,  1816,  1783,   937,  1807,  1808,  1817,   944,   387,  1834,
    1835,   947,   831,   832,   833,   834,   835,  1832,  1840,   836,
     837,   838,   839,  1845,   840,   841,   842,   843,  1841,  1842,
    1843,  1847,   844,  1854,   845,   846,  1846,  1848,  1046,   981,
     847,   848,   849,  1855,   143,  1856,   850,  1830,    19,   205,
     146,  1735,   321,  1738,   983,  1739,  1447,  1740,  1741,  1742,
      26,  1451,  1734,   799,  1002,  1529,  1643,  1005,   386,  1007,
     386,  1589,  1679,  1590,  1458,  1680,  1681,   103,   387,  1015,
    1016,  1017,  1018,  1019,  1020,   747,  1026,   761,  1026,  1811,
    1501,   851,     0,   852,   853,   854,   855,   856,   857,   858,
     859,   860,   861,   762,  1709,   479,  1035,     0,     0,  1671,
       0,     0,   862,   863,     0,  1677,     0,     0,     0,     0,
       0,     0,  1086,  1087,     0,     0,  1088,  1089,  1090,  1091,
       0,  1093,     0,  1096,  1097,  1098,  1100,  1101,  1102,  1103,
    1104,  1105,  1107,  1108,  1109,  1110,  1111,  1112,  1113,  1114,
    1115,  1116,  1117,     0,  1126,     0,     0,  1129,     0,     0,
    1132,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     829,   830,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   386,     0,     0,     0,     0,   950,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   386,     0,     0,     0,     0,
     922,     0,     0,     0,     0,  1192,     0,     0,     0,     0,
       0,     0,     0,   950,     0,   950,  1200,  1219,  1221,  1223,
    1225,  1227,  1229,  1231,  1233,  1235,  1237,     0,  1240,  1242,
    1244,  1246,  1248,  1250,     0,     0,     0,   950,     0,     0,
    1068,     0,  1218,  1220,  1222,  1224,  1226,  1228,  1230,  1232,
    1234,  1236,  1238,  1239,  1241,  1243,  1245,  1247,  1249,  1251,
       0,     0,     0,     0,     0,   386,     0,     0,  1793,     0,
       0,     0,  1270,     0,  1272,   829,   830,     0,   831,   832,
     833,   834,   835,     0,     0,   836,   837,   838,   839,     0,
     840,   841,   842,   843,     0,     0,     0,  1068,   844,     0,
     845,   846,     0,     0,   810,  1304,   847,   848,   849,     0,
       0,  1829,   850,     0,     0,     0,     0,     0,  1326,     0,
     799,     0,     0,     0,     0,     0,  1331,     0,     0,     0,
     799,     0,     0,     0,     0,  1336,     0,  1337,     0,  1338,
       0,  1339,     0,     0,     0,  1851,     0,  1852,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   851,     0,   852,
     853,   854,   855,   856,   857,   858,   859,   860,   861,  1857,
     829,   830,     0,     0,     0,     0,     0,     0,   862,   863,
       0,     0,     0,   580,   938,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   833,   834,     0,     0,   -81,
    1377,     0,     0,   839,  1382,   840,   841,   842,   843,     0,
     829,   830,     0,   844,  1389,  1390,  1391,  1392,  1393,  1394,
    1395,  1396,  1397,  1398,  1399,  1400,  1401,  1402,  1403,  1404,
    1405,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1428,     0,     0,     0,     0,
       0,  1429,     0,     0,     0,     0,  1434,     0,     0,     0,
       0,  1336,     0,     0,     0,     0,     0,     0,     0,   857,
     858,   859,   860,   861,     0,     0,  1446,     0,   831,   832,
     833,   834,   835,   862,   863,   836,   837,   838,   839,     0,
     840,   841,   842,   843,     0,     0,     0,     0,   844,     0,
     845,   846,     0,     0,     0,     0,   847,   848,   849,     0,
     961,   962,   963,   964,   965,   966,   967,   968,   831,   832,
     833,   834,   835,   969,   970,   836,   837,   838,   839,   971,
     840,   841,   842,   843,     0,     0,     0,     0,   844,   972,
     845,   846,   973,   974,     0,     0,   847,   848,   849,   975,
     976,   977,   850,   829,   830,     0,     0,   851,     0,   852,
     853,   854,   855,   856,   857,   858,   859,   860,   861,     0,
       0,     0,     0,     0,     0,     0,     0,  1496,   862,   863,
       0,     0,     0,     0,     0,  1500,  1026,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   978,   851,     0,   852,
     853,   854,   855,   856,   857,   858,   859,   860,   861,     0,
       0,     0,     0,     0,  1522,  1523,     0,     0,   862,   863,
     487,     0,  1336,   580,   938,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1539,     0,  1541,     0,  1544,
       0,     0,     0,     0,     0,  1547,     0,   488,   489,  1550,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     487,     0,     0,  1133,  1134,  1135,  1136,  1137,  1138,  1139,
    1140,   831,   832,   833,   834,   835,  1141,  1142,   836,   837,
     838,   839,  1143,   840,   841,   842,   843,   488,   489,     0,
       0,   844,   972,   845,   846,  1144,  1145,     0,     0,   847,
     848,   849,  1146,  1147,  1148,   850,     0,     0,     0,  1574,
       0,     0,     0,     0,     0,  1578,     0,     0,   746,     0,
      13,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   490,     0,     0,     0,   491,     0,
       0,     0,    14,     0,     0,     0,     0,     0,     0,  1149,
     851,   799,   852,   853,   854,   855,   856,   857,   858,   859,
     860,   861,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   862,   863,     0,   490,    13,   580,   938,   491,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1632,  1633,  1634,     0,     0,     0,     0,    14,     0,   829,
     830,     0,     0,   648,     0,   492,     0,     0,     0,   493,
       0,  1279,   494,     0,     0,     0,     0,  1655,     0,  1656,
       0,     0,     0,     0,     0,  1660,     0,   495,     0,     0,
       0,     0,     0,   496,     0,     0,   829,   830,     0,     0,
    1663,     0,  1665,     0,     0,   492,     0,     0,     0,   493,
       0,  1478,   494,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1685,   495,     0,  1688,
       0,     0,     0,   496,     0,     0,     0,  1695,  1696,  1697,
       0,     0,     0,     0,  1704,     0,     0,  1706,     0,     0,
    1708,     0,     0,   799,  1711,     0,     0,     0,   799,  1715,
       0,  1717,  1718,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1725,   831,   832,   833,
     834,   835,     0,     0,   836,   837,   838,   839,     0,   840,
     841,   842,   843,     0,     0,     0,     0,   844,     0,   845,
     846,  1743,     0,     0,     0,   847,   848,   849,     0,     0,
       0,   850,     0,     0,   831,   832,   833,   834,   835,     0,
       0,   836,   837,   838,   839,   799,   840,   841,   842,   843,
       0,     0,     0,     0,   844,     0,   845,   846,     0,     0,
       0,     0,   847,   848,   849,     0,     0,  1773,   850,     0,
       0,     0,     0,     0,     0,  1779,   851,     0,   852,   853,
     854,   855,   856,   857,   858,   859,   860,   861,  1786,     0,
       0,     0,     0,     0,     0,     0,     0,   862,   863,  1792,
       0,   864,     0,     0,     0,  1801,  1802,     0,     0,     0,
       0,     0,     0,   851,     0,   852,   853,   854,   855,   856,
     857,   858,   859,   860,   861,  1819,  1820,     0,     0,     0,
       0,     0,     0,     0,   862,   863,  1823,     0,   877,     0,
    1826,  1828,   694,     0,     0,     0,   527,   528,     3,     0,
     695,   696,   697,     0,   698,     0,   529,   530,   531,   532,
     533,     0,     0,  1844,     0,     0,   534,   699,   535,   700,
     701,     0,   536,     0,     0,  1850,     0,     0,     0,   702,
     537,   703,     0,   704,     0,   705,   538,     0,     0,   539,
       0,     8,   540,   706,     0,   707,   541,     0,     0,   708,
     709,     0,     0,     0,     0,     0,   710,     0,     0,   543,
     544,     0,   328,   329,   330,     0,   332,   333,   334,   335,
     336,   545,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,     0,   350,   351,   352,     0,     0,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   546,   547,   711,   712,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   549,   550,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   713,   714,   715,
       0,     0,     0,     0,     0,     0,     0,    64,     0,     0,
       0,     0,     0,     0,     0,   551,   552,   553,   554,   555,
     487,   556,     0,   557,   558,   559,   560,     0,   155,    13,
     561,   562,     0,   563,   564,   565,   566,   567,   568,    65,
     716,   570,   571,     0,     0,     0,     0,   488,   489,   572,
     156,    14,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   573,   574,   575,     0,
      15,     0,     0,   576,   577,     0,     0,   527,   528,     0,
       0,   578,     0,   579,     0,   580,   581,   529,   530,   531,
     532,   533,     0,     0,     0,     0,     0,   534,     0,   535,
       0,     0,     0,   536,     0,   487,     0,     0,     0,     0,
       0,   537,     0,     0,     0,     0,     0,   538,     0,     0,
     539,     0,     0,   540,     0,  1022,     0,   541,     0,     0,
       0,     0,   488,   489,   490,     0,     0,   542,   491,     0,
     543,   544,     0,   328,   329,   330,     0,   332,   333,   334,
     335,   336,   545,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,     0,   350,   351,   352,     0,     0,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   546,   547,   548,
       0,     0,     0,     0,     0,   492,     0,     0,     0,   493,
       0,  1482,   494,   549,   550,     0,     0,     0,     0,   490,
       0,     0,     0,   491,     0,     0,     0,   495,     0,     0,
       0,     0,     0,   496,     0,     0,     0,     0,    64,     0,
       0,     0,     0,     0,     0,     0,   551,   552,   553,   554,
     555,     0,   556,   820,   557,   558,   559,   560,     0,     0,
       0,   561,   562,     0,   563,   564,   565,   566,   567,   568,
     821,   569,   570,   571,     0,     0,     0,     0,     0,     0,
     572,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     492,     0,     0,     0,   493,     0,     0,  1023,   574,   575,
       0,    15,     0,     0,   576,   577,     0,     0,     0,   527,
     528,     0,  1024,     0,  1025,     0,   580,   581,   496,   529,
     530,   531,   532,   533,     0,     0,     0,     0,     0,   534,
       0,   535,     0,     0,     0,   536,     0,   487,     0,     0,
       0,     0,     0,   537,     0,     0,     0,     0,     0,   538,
       0,     0,   539,     0,     0,   540,     0,     0,     0,   541,
       0,     0,     0,     0,   488,   489,     0,     0,     0,   542,
       0,     0,   543,   544,     0,   328,   329,   330,     0,   332,
     333,   334,   335,   336,   545,   338,   339,   340,   341,   342,
     343,   344,   345,   346,   347,   348,     0,   350,   351,   352,
       0,     0,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   368,   369,   370,   371,   372,
     373,   374,   375,   376,   377,   378,   379,   380,   381,   546,
     547,   548,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   549,   550,     0,     0,     0,
       0,   490,     0,     0,     0,   491,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,     0,     0,     0,     0,     0,     0,   551,   552,
     553,   554,   555,     0,   556,   820,   557,   558,   559,   560,
       0,     0,     0,   561,   562,     0,   563,   564,   565,   566,
     567,   568,   821,   569,   570,   571,     0,     0,     0,     0,
       0,     0,   572,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   492,     0,     0,     0,   493,     0,     0,  1023,
     574,   575,     0,    15,     0,     0,   576,   577,     0,     0,
       0,   527,   528,     0,  1024,     0,  1033,     0,   580,   581,
     496,   529,   530,   531,   532,   533,     0,     0,     0,     0,
       0,   534,     0,   535,     0,     0,     0,   536,     0,   637,
       0,     0,     0,     0,     0,   537,     0,     0,     0,     0,
       0,   538,     0,     0,   539,     0,     0,   540,     0,     0,
       0,   541,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   542,     0,     0,   543,   544,     0,   328,   329,   330,
       0,   332,   333,   334,   335,   336,   545,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,     0,   350,
     351,   352,     0,     0,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   546,   547,   548,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   549,   550,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    64,     0,     0,     0,     0,     0,     0,     0,
     551,   552,   553,   554,   555,     0,   556,     0,   557,   558,
     559,   560,     0,     0,     0,   561,   562,   809,   563,   564,
     565,   566,   567,   568,    65,   569,   570,   571,     0,     0,
       0,     0,     0,     0,   572,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   638,     0,
       0,   573,   574,   575,     0,    15,     0,     0,   576,   577,
       0,     0,     0,   527,   528,     0,  1303,     0,   579,     0,
     580,   581,   640,   529,   530,   531,   532,   533,     0,     0,
       0,     0,     0,   534,     0,   535,     0,     0,     0,   536,
       0,     0,     0,     0,     0,     0,     0,   537,     0,     0,
       0,     0,     0,   538,     0,     0,   539,     0,     0,   540,
       0,     0,     0,   541,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   542,     0,     0,   543,   544,     0,   328,
     329,   330,     0,   332,   333,   334,   335,   336,   545,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   348,
       0,   350,   351,   352,     0,     0,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   374,   375,   376,   377,   378,
     379,   380,   381,   546,   547,   711,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   549,
     550,     0,     0,     0,     0,     0,     0,     0,   919,     0,
       0,     0,     0,     0,   713,   714,   715,     0,     0,     0,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,   551,   552,   553,   554,   555,   487,   556,     0,
     557,   558,   559,   560,     0,     0,     0,   561,   562,     0,
     563,   564,   565,   566,   567,   568,    65,   569,   570,   571,
       0,     0,     0,     0,   488,   489,   572,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   573,   574,   575,     0,    15,     0,     0,
     576,   577,     0,     0,   527,   528,     0,     0,   578,     0,
     579,     0,   580,   581,   529,   530,   531,   532,   533,     0,
       0,     0,     0,     0,   534,     0,   535,     0,     0,     0,
     536,     0,     0,     0,     0,     0,     0,     0,   537,     0,
       0,     0,     0,     0,   538,     0,     0,   539,     0,     0,
     540,     0,     0,     0,   541,     0,     0,     0,     0,     0,
       0,   490,     0,     0,   542,   491,     0,   543,   544,     0,
     328,   329,   330,     0,   332,   333,   334,   335,   336,   545,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,     0,   350,   351,   352,     0,     0,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,   365,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
     378,   379,   380,   381,   546,   547,   711,     0,     0,     0,
       0,     0,   492,     0,     0,     0,   493,     0,  1483,   494,
     549,   550,     0,     0,     0,     0,     0,     0,     0,   935,
       0,     0,     0,     0,   495,   713,   714,   715,     0,     0,
     496,     0,     0,     0,     0,    64,     0,     0,     0,     0,
       0,     0,     0,   551,   552,   553,   554,   555,   487,   556,
       0,   557,   558,   559,   560,     0,     0,     0,   561,   562,
       0,   563,   564,   565,   566,   567,   568,    65,   569,   570,
     571,     0,     0,     0,     0,   488,   489,   572,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   573,   574,   575,     0,    15,     0,
       0,   576,   577,     0,     0,   527,   528,     0,     0,   578,
       0,   579,     0,   580,   581,   529,   530,   531,   532,   533,
       0,     0,     0,     0,     0,   534,  1736,   535,   700,     0,
       0,   536,     0,     0,     0,     0,     0,     0,     0,   537,
       0,     0,     0,     0,     0,   538,     0,     0,   539,     0,
       0,   540,   706,     0,     0,   541,     0,     0,     0,     0,
       0,     0,   490,     0,     0,   542,   491,     0,   543,   544,
       0,   328,   329,   330,     0,   332,   333,   334,   335,   336,
     545,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,     0,   350,   351,   352,     0,     0,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   364,   365,   366,
     367,   368,   369,   370,   371,   372,   373,   374,   375,   376,
     377,   378,   379,   380,   381,   546,   547,   548,  1737,     0,
       0,     0,     0,   492,     0,     0,     0,   493,     0,  1484,
     494,   549,   550,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   495,     0,     0,     0,     0,
       0,   496,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,   551,   552,   553,   554,   555,   487,
     556,     0,   557,   558,   559,   560,     0,     0,     0,   561,
     562,     0,   563,   564,   565,   566,   567,   568,    65,   569,
     570,   571,     0,     0,     0,     0,   488,   489,   572,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   573,   574,   575,     0,    15,
       0,     0,   576,   577,     0,     0,   527,   528,     0,     0,
     578,     0,   579,     0,   580,   581,   529,   530,   531,   532,
     533,     0,     0,     0,     0,     0,   534,     0,   535,     0,
       0,     0,   536,     0,     0,     0,     0,     0,     0,     0,
     537,     0,     0,     0,     0,     0,   538,     0,     0,   539,
       0,     0,   540,     0,     0,     0,   541,     0,     0,     0,
       0,     0,     0,   490,     0,     0,   542,   491,     0,   543,
     544,     0,   328,   329,   330,     0,   332,   333,   334,   335,
     336,   545,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,     0,   350,   351,   352,     0,     0,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   546,   547,   711,     0,
       0,     0,     0,     0,   492,     0,     0,     0,   493,     0,
    1485,   494,   549,   550,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   495,   713,   714,   715,
       0,     0,   496,     0,     0,     0,     0,    64,     0,     0,
       0,     0,     0,     0,     0,   551,   552,   553,   554,   555,
     487,   556,     0,   557,   558,   559,   560,     0,     0,     0,
     561,   562,     0,   563,   564,   565,   566,   567,   568,    65,
     569,   570,   571,     0,     0,     0,     0,   488,   489,   572,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   573,   574,   575,     0,
      15,     0,     0,   576,   577,     0,     0,   527,   528,     0,
       0,   578,     0,   579,     0,   580,   581,   529,   530,   531,
     532,   533,     0,     0,     0,     0,     0,   534,     0,   535,
       0,     0,     0,   536,     0,     0,     0,     0,     0,     0,
       0,   537,     0,     0,     0,     0,     0,   538,     0,     0,
     539,     0,     0,   540,     0,     0,     0,   541,     0,     0,
       0,     0,     0,     0,   490,     0,     0,   542,   491,     0,
     543,   544,  1070,   328,   329,   330,     0,   332,   333,   334,
     335,   336,   545,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,     0,   350,   351,   352,     0,     0,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   546,   547,   548,
       0,     0,     0,     0,     0,   492,     0,     0,     0,   493,
       0,  1487,   494,   549,   550,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   495,     0,     0,
       0,     0,     0,   496,     0,     0,     0,     0,    64,     0,
       0,     0,     0,     0,     0,     0,   551,   552,   553,   554,
     555,   487,   556,   820,   557,   558,   559,   560,     0,     0,
       0,   561,   562,     0,   563,   564,   565,   566,   567,   568,
     821,   569,   570,   571,     0,     0,     0,     0,   488,   489,
     572,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   573,   574,   575,
       0,    15,     0,     0,   576,   577,     0,     0,   527,   528,
       0,     0,  1071,     0,   579,  1072,   580,   581,   529,   530,
     531,   532,   533,     0,     0,     0,     0,     0,   534,     0,
     535,     0,     0,   487,   536,     0,     0,     0,     0,     0,
       0,     0,   537,     0,     0,     0,     0,     0,   538,     0,
       0,   539,     0,     0,   540,     0,     0,     0,   541,     0,
     488,   489,     0,     0,     0,   490,     0,     0,   542,   491,
       0,   543,   544,     0,   328,   329,   330,     0,   332,   333,
     334,   335,   336,   545,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,     0,   350,   351,   352,     0,
       0,   355,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   377,   378,   379,   380,   381,   546,   547,
     711,     0,     0,     0,     0,     0,   492,     0,     0,     0,
     493,     0,  1600,   494,   549,   550,     0,   490,     0,     0,
       0,   491,     0,     0,     0,     0,     0,     0,   495,  1213,
    1214,  1215,     0,     0,   496,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,   551,   552,   553,
     554,   555,   487,   556,     0,   557,   558,   559,   560,     0,
       0,     0,   561,   562,     0,   563,   564,   565,   566,   567,
     568,    65,   569,   570,   571,     0,     0,     0,     0,   488,
     489,   572,     0,     0,     0,     0,     0,     0,   492,     0,
       0,     0,   493,     0,  1605,   494,     0,     0,   573,   574,
     575,     0,    15,     0,     0,   576,   577,     0,     0,     0,
     495,   527,   528,   578,     0,   579,   496,   580,   581,   815,
       0,   529,   530,   531,   532,   533,     0,     0,     0,     0,
       0,   534,     0,   535,     0,     0,   487,   536,     0,     0,
       0,     0,     0,     0,     0,   537,     0,     0,     0,     0,
       0,   538,     0,     0,   539,   816,     0,   540,     0,     0,
       0,   541,     0,   488,   489,     0,   490,     0,     0,     0,
     491,   542,     0,     0,   543,   544,     0,   328,   329,   330,
       0,   332,   333,   334,   335,   336,   545,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,     0,   350,
     351,   352,     0,     0,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   546,   547,   548,     0,     0,     0,   492,     0,     0,
       0,   493,     0,  1637,   494,     0,     0,   549,   550,     0,
     490,     0,     0,     0,   491,     0,     0,     0,     0,   495,
       0,     0,     0,     0,     0,   496,     0,     0,     0,     0,
       0,     0,    64,     0,     0,     0,     0,     0,     0,     0,
     551,   552,   553,   554,   555,     0,   556,     0,   557,   558,
     559,   560,     0,     0,     0,   561,   562,     0,   563,   564,
     565,   566,   567,   568,    65,   569,   570,   571,     0,     0,
       0,     0,     0,     0,   572,     0,     0,     0,     0,     0,
       0,   492,     0,     0,     0,   493,     0,  1726,   494,     0,
       0,   573,   574,   575,     0,    15,     0,     0,   576,   577,
       0,     0,     0,   495,   527,   528,   578,   641,   579,   496,
     580,   581,   815,     0,   529,   530,   531,   532,   533,     0,
       0,     0,     0,     0,   534,     0,   535,     0,     0,     0,
     536,     0,     0,     0,     0,     0,     0,     0,   537,     0,
       0,     0,     0,     0,   538,     0,     0,   539,   816,     0,
     540,     0,     0,     0,   541,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   542,     0,     0,   543,   544,     0,
     328,   329,   330,     0,   332,   333,   334,   335,   336,   545,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,     0,   350,   351,   352,     0,     0,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,   365,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
     378,   379,   380,   381,   546,   547,   548,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     549,   550,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,     0,     0,     0,
       0,     0,     0,   551,   552,   553,   554,   555,     0,   556,
     820,   557,   558,   559,   560,     0,     0,     0,   561,   562,
       0,   563,   564,   565,   566,   567,   568,   821,   569,   570,
     571,     0,     0,     0,     0,     0,     0,   572,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   573,   574,   575,     0,    15,     0,
       0,   576,   577,     0,     0,     0,     0,   527,   528,   578,
       0,   579,     0,   580,   581,   815,     0,   529,   530,   531,
     532,   533,     0,     0,     0,     0,     0,   534,     0,   535,
       0,     0,     0,   536,     0,     0,     0,     0,     0,     0,
       0,   537,     0,     0,     0,     0,     0,   538,     0,     0,
     539,   816,     0,   540,     0,     0,     0,   541,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   542,     0,     0,
     543,   544,     0,   328,   329,   330,     0,   332,   333,   334,
     335,   336,   545,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,     0,   350,   351,   352,     0,     0,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   546,   547,   548,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   549,   550,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
       0,     0,     0,     0,     0,     0,   551,   552,   553,   554,
     555,     0,   556,     0,   557,   558,   559,   560,     0,     0,
       0,   561,   562,     0,   563,   564,   565,   566,   567,   568,
      65,   569,   570,   571,     0,     0,     0,     0,     0,     0,
     572,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   573,   574,   575,
       0,    15,     0,     0,   576,   577,     0,     0,     0,     0,
     527,   528,   578,   896,   579,     0,   580,   581,   815,     0,
     529,   530,   531,   532,   533,     0,     0,     0,     0,     0,
     534,     0,   535,     0,     0,     0,   536,     0,     0,     0,
       0,     0,     0,     0,   537,     0,     0,     0,     0,     0,
     538,     0,     0,   539,   816,     0,   540,     0,     0,     0,
     541,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     542,     0,     0,   543,   544,     0,   328,   329,   330,     0,
     332,   333,   334,   335,   336,   545,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,     0,   350,   351,
     352,     0,     0,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     546,   547,   548,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   549,   550,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,   551,
     552,   553,   554,   555,     0,   556,     0,   557,   558,   559,
     560,     0,     0,     0,   561,   562,     0,   563,   564,   565,
     566,   567,   568,    65,   569,   570,   571,     0,     0,     0,
       0,     0,     0,   572,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     573,   574,   575,     0,    15,     0,     0,   576,   577,     0,
       0,   527,   528,     0,     0,   578,     0,   579,     0,   580,
     581,   529,   530,   531,   532,   533,     0,     0,     0,     0,
       0,   534,     0,   535,     0,     0,     0,   536,     0,     0,
       0,     0,     0,     0,     0,   537,     0,     0,     0,     0,
       0,   538,     0,     0,   539,     0,     0,   540,     0,     0,
       0,   541,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   542,     0,     0,   543,   544,  1265,   328,   329,   330,
       0,   332,   333,   334,   335,   336,   545,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,     0,   350,
     351,   352,     0,     0,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   546,   547,   548,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   549,   550,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    64,     0,     0,     0,     0,     0,     0,     0,
     551,   552,   553,   554,   555,     0,   556,   820,   557,   558,
     559,   560,     0,     0,     0,   561,   562,     0,   563,   564,
     565,   566,   567,   568,   821,   569,   570,   571,     0,     0,
       0,     0,     0,     0,   572,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   573,   574,   575,     0,    15,     0,     0,   576,   577,
       0,     0,   527,   528,     0,     0,   578,     0,   579,     0,
     580,   581,   529,   530,   531,   532,   533,     0,     0,     0,
       0,     0,   534,     0,   535,     0,     0,     0,   536,     0,
       0,     0,     0,     0,     0,     0,   537,     0,     0,     0,
       0,     0,   538,     0,     0,   539,     0,     0,   540,     0,
       0,     0,   541,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   542,     0,     0,   543,   544,     0,   328,   329,
     330,     0,   332,   333,   334,   335,   336,   545,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,     0,
     350,   351,   352,     0,     0,   355,   356,   357,   358,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   546,   547,   548,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   549,   550,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,     0,     0,     0,     0,     0,
       0,   551,   552,   553,   554,   555,     0,   556,   820,   557,
     558,   559,   560,     0,     0,     0,   561,   562,     0,   563,
     564,   565,   566,   567,   568,   821,   569,   570,   571,     0,
       0,     0,     0,     0,     0,   572,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   573,   574,   575,     0,    15,     0,     0,   576,
     577,     0,     0,   527,   528,     0,     0,   578,     0,   579,
    1305,   580,   581,   529,   530,   531,   532,   533,     0,     0,
       0,     0,     0,   534,     0,   535,     0,     0,     0,   536,
       0,     0,     0,     0,     0,     0,     0,   537,     0,     0,
       0,     0,     0,   538,     0,     0,   539,     0,     0,   540,
       0,     0,     0,   541,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   542,     0,     0,   543,   544,     0,   328,
     329,   330,     0,   332,   333,   334,   335,   336,   545,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   348,
       0,   350,   351,   352,     0,     0,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   374,   375,   376,   377,   378,
     379,   380,   381,   546,   547,   548,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   549,
     550,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,   551,   552,   553,   554,   555,     0,   556,   820,
     557,   558,   559,   560,     0,     0,     0,   561,   562,     0,
     563,   564,   565,   566,   567,   568,   821,   569,   570,   571,
       0,     0,     0,     0,     0,     0,   572,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   573,   574,   575,     0,    15,     0,     0,
     576,   577,     0,     0,   527,   528,     0,     0,   578,     0,
     579,  1320,   580,   581,   529,   530,   531,   532,   533,     0,
       0,     0,     0,     0,   534,     0,   535,     0,     0,     0,
     536,     0,     0,     0,     0,     0,     0,     0,   537,     0,
       0,     0,     0,     0,   538,     0,     0,   539,     0,     0,
     540,     0,     0,     0,   541,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   542,     0,     0,   543,   544,     0,
     328,   329,   330,     0,   332,   333,   334,   335,   336,   545,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,     0,   350,   351,   352,     0,     0,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,   365,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
     378,   379,   380,   381,   546,   547,   548,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     549,   550,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,     0,     0,     0,
       0,     0,     0,   551,   552,   553,   554,   555,     0,   556,
       0,   557,   558,   559,   560,     0,     0,     0,   561,   562,
       0,   563,   564,   565,   566,   567,   568,    65,   569,   570,
     571,     0,     0,     0,     0,     0,     0,   572,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   573,   574,   575,     0,    15,     0,
       0,   576,   577,     0,     0,     0,     0,   527,   528,   578,
     641,   579,     0,   580,   581,   798,     0,   529,   530,   531,
     532,   533,     0,     0,     0,     0,     0,   534,     0,   535,
       0,     0,     0,   536,     0,     0,     0,     0,     0,     0,
       0,   537,     0,     0,     0,     0,     0,   538,     0,     0,
     539,     0,     0,   540,     0,     0,     0,   541,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   542,     0,     0,
     543,   544,     0,   328,   329,   330,     0,   332,   333,   334,
     335,   336,   545,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,     0,   350,   351,   352,     0,     0,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   546,   547,   548,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   549,   550,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
       0,     0,     0,     0,     0,     0,   551,   552,   553,   554,
     555,     0,   556,     0,   557,   558,   559,   560,     0,     0,
       0,   561,   562,     0,   563,   564,   565,   566,   567,   568,
      65,   569,   570,   571,     0,     0,     0,     0,     0,     0,
     572,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   573,   574,   575,
       0,    15,     0,     0,   576,   577,     0,     0,     0,     0,
     527,   528,   578,     0,   579,     0,   580,   581,   805,     0,
     529,   530,   531,   532,   533,     0,     0,     0,     0,     0,
     534,     0,   535,     0,     0,     0,   536,     0,     0,     0,
       0,     0,     0,     0,   537,     0,     0,     0,     0,     0,
     538,     0,     0,   539,     0,     0,   540,     0,     0,     0,
     541,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     542,     0,     0,   543,   544,     0,   328,   329,   330,     0,
     332,   333,   334,   335,   336,   545,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,     0,   350,   351,
     352,     0,     0,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     546,   547,   548,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   549,   550,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,   551,
     552,   553,   554,   555,     0,   556,     0,   557,   558,   559,
     560,     0,     0,     0,   561,   562,     0,   563,   564,   565,
     566,   567,   568,    65,   569,   570,   571,     0,     0,     0,
       0,     0,     0,   572,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     573,   574,   575,     0,    15,     0,     0,   576,   577,     0,
       0,   527,   528,     0,     0,   578,     0,   579,     0,   580,
     581,   529,   530,   531,   532,   533,     0,     0,     0,     0,
       0,   534,     0,   535,     0,     0,     0,   536,     0,     0,
       0,     0,     0,     0,     0,   537,     0,     0,     0,     0,
       0,   538,     0,     0,   539,     0,     0,   540,     0,     0,
       0,   541,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   542,     0,     0,   543,   544,     0,   328,   329,   330,
       0,   332,   333,   334,   335,   336,   545,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,     0,   350,
     351,   352,     0,     0,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   546,   547,   548,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   549,   550,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    64,     0,     0,     0,     0,     0,     0,     0,
     551,   552,   553,   554,   555,     0,   556,     0,   557,   558,
     559,   560,     0,     0,     0,   561,   562,   809,   563,   564,
     565,   566,   567,   568,    65,   569,   570,   571,     0,     0,
       0,     0,     0,     0,   572,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   573,   574,   575,     0,    15,     0,     0,   576,   577,
       0,     0,   527,   528,     0,     0,   578,     0,   579,     0,
     580,   581,   529,   530,   531,   532,   533,     0,     0,     0,
       0,     0,   534,     0,   535,     0,     0,     0,   536,     0,
       0,     0,     0,     0,     0,     0,   537,     0,     0,     0,
       0,     0,   538,     0,     0,   539,     0,     0,   540,     0,
       0,     0,   541,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   542,     0,     0,   543,   544,     0,   328,   329,
     330,     0,   332,   333,   334,   335,   336,   545,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,     0,
     350,   351,   352,     0,     0,   355,   356,   357,   358,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   546,   547,   548,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   549,   550,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,     0,     0,     0,     0,     0,
       0,   551,   552,   553,   554,   555,     0,   556,   820,   557,
     558,   559,   560,     0,     0,     0,   561,   562,     0,   563,
     564,   565,   566,   567,   568,   821,   569,   570,   571,     0,
       0,     0,     0,     0,     0,   572,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   573,   574,   575,     0,    15,     0,     0,   576,
     577,     0,     0,   527,   528,     0,     0,   578,     0,   579,
       0,   580,   581,   529,   530,   531,   532,   533,     0,     0,
       0,     0,     0,   534,     0,   535,     0,     0,     0,   536,
       0,     0,     0,     0,     0,     0,     0,   537,     0,     0,
       0,     0,     0,   538,     0,     0,   539,     0,     0,   540,
       0,     0,     0,   541,     0,     0,     0,     0,     0,   924,
       0,     0,     0,   542,     0,     0,   543,   544,     0,   328,
     329,   330,     0,   332,   333,   334,   335,   336,   545,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   348,
       0,   350,   351,   352,     0,     0,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   374,   375,   376,   377,   378,
     379,   380,   381,   546,   547,   548,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   549,
     550,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,   551,   552,   553,   554,   555,     0,   556,     0,
     557,   558,   559,   560,     0,     0,     0,   561,   562,     0,
     563,   564,   565,   566,   567,   568,    65,   569,   570,   571,
       0,     0,     0,     0,     0,     0,   572,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   573,   574,   575,     0,    15,     0,     0,
     576,   577,     0,     0,   527,   528,     0,     0,   578,     0,
     579,     0,   580,   581,   529,   530,   531,   532,   533,     0,
       0,     0,     0,     0,   534,     0,   535,     0,     0,     0,
     536,     0,     0,     0,     0,     0,     0,     0,   537,     0,
       0,     0,     0,     0,   538,     0,     0,   539,     0,     0,
     540,     0,     0,     0,   541,     0,     0,   931,     0,     0,
       0,     0,     0,     0,   542,     0,     0,   543,   544,     0,
     328,   329,   330,     0,   332,   333,   334,   335,   336,   545,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,     0,   350,   351,   352,     0,     0,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,   365,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
     378,   379,   380,   381,   546,   547,   548,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     549,   550,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,     0,     0,     0,
       0,     0,     0,   551,   552,   553,   554,   555,     0,   556,
       0,   557,   558,   559,   560,     0,     0,     0,   561,   562,
       0,   563,   564,   565,   566,   567,   568,    65,   569,   570,
     571,     0,     0,     0,     0,     0,     0,   572,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   573,   574,   575,     0,    15,     0,
       0,   576,   577,     0,     0,   527,   528,     0,     0,   578,
       0,   579,     0,   580,   581,   529,   530,   531,   532,   533,
       0,     0,     0,     0,     0,   534,     0,   535,     0,     0,
       0,   536,     0,     0,     0,     0,     0,     0,     0,   537,
       0,     0,     0,     0,     0,   538,     0,     0,   539,     0,
       0,   540,     0,     0,     0,   541,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   542,     0,     0,   543,   544,
       0,   328,   329,   330,     0,   332,   333,   334,   335,   336,
     545,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,     0,   350,   351,   352,     0,     0,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   364,   365,   366,
     367,   368,   369,   370,   371,   372,   373,   374,   375,   376,
     377,   378,   379,   380,   381,   546,   547,   548,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   549,   550,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,   551,   552,   553,   554,   555,     0,
     556,     0,   557,   558,   559,   560,     0,     0,     0,   561,
     562,     0,   563,   564,   565,   566,   567,   568,    65,   569,
     570,   571,     0,     0,     0,     0,     0,     0,   572,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   802,     0,   573,   574,   575,     0,    15,
       0,     0,   576,   577,     0,     0,   527,   528,     0,     0,
     578,     0,   579,     0,   580,   581,   529,   530,   531,   532,
     533,     0,     0,  1106,     0,     0,   534,     0,   535,     0,
       0,     0,   536,     0,     0,     0,     0,     0,     0,     0,
     537,     0,     0,     0,     0,     0,   538,     0,     0,   539,
       0,     0,   540,     0,     0,     0,   541,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   542,     0,     0,   543,
     544,     0,   328,   329,   330,     0,   332,   333,   334,   335,
     336,   545,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,     0,   350,   351,   352,     0,     0,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   546,   547,   548,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   549,   550,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    64,     0,     0,
       0,     0,     0,     0,     0,   551,   552,   553,   554,   555,
       0,   556,     0,   557,   558,   559,   560,     0,     0,     0,
     561,   562,     0,   563,   564,   565,   566,   567,   568,    65,
     569,   570,   571,     0,     0,     0,     0,     0,     0,   572,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   573,   574,   575,     0,
      15,     0,     0,   576,   577,     0,     0,   527,   528,     0,
       0,   578,     0,   579,     0,   580,   581,   529,   530,   531,
     532,   533,     0,     0,     0,     0,     0,   534,     0,   535,
       0,     0,     0,   536,     0,     0,     0,     0,     0,     0,
       0,   537,     0,     0,     0,     0,     0,   538,     0,     0,
     539,     0,     0,   540,     0,     0,     0,   541,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   542,     0,     0,
     543,   544,     0,   328,   329,   330,     0,   332,   333,   334,
     335,   336,   545,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,     0,   350,   351,   352,     0,     0,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   546,   547,   548,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   549,   550,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
       0,     0,     0,     0,     0,     0,   551,   552,   553,   554,
     555,     0,   556,     0,   557,   558,   559,   560,     0,     0,
       0,   561,   562,     0,   563,   564,   565,   566,   567,   568,
      65,   569,   570,   571,     0,     0,     0,     0,     0,     0,
     572,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   573,   574,   575,
       0,    15,     0,     0,   576,   577,     0,     0,   527,   528,
       0,     0,   578,     0,   579,  1127,   580,   581,   529,   530,
     531,   532,   533,     0,     0,     0,     0,     0,   534,     0,
     535,     0,     0,     0,   536,     0,     0,     0,     0,     0,
       0,     0,   537,     0,     0,     0,     0,     0,   538,     0,
       0,   539,     0,     0,   540,     0,     0,     0,   541,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   542,     0,
       0,   543,   544,     0,   328,   329,   330,     0,   332,   333,
     334,   335,   336,   545,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,     0,   350,   351,   352,     0,
       0,   355,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   377,   378,   379,   380,   381,   546,   547,
     548,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   549,   550,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,   551,   552,   553,
     554,   555,     0,   556,     0,   557,   558,   559,   560,     0,
       0,     0,   561,   562,     0,   563,   564,   565,   566,   567,
     568,    65,   569,   570,   571,     0,     0,     0,     0,     0,
       0,   572,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1269,     0,   573,   574,
     575,     0,    15,     0,     0,   576,   577,     0,     0,   527,
     528,     0,     0,   578,     0,   579,     0,   580,   581,   529,
     530,   531,   532,   533,     0,     0,     0,     0,     0,   534,
       0,   535,     0,     0,     0,   536,     0,     0,     0,     0,
       0,     0,     0,   537,     0,     0,     0,     0,     0,   538,
       0,     0,   539,     0,     0,   540,     0,     0,     0,   541,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   542,
       0,     0,   543,   544,     0,   328,   329,   330,     0,   332,
     333,   334,   335,   336,   545,   338,   339,   340,   341,   342,
     343,   344,   345,   346,   347,   348,     0,   350,   351,   352,
       0,     0,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   368,   369,   370,   371,   372,
     373,   374,   375,   376,   377,   378,   379,   380,   381,   546,
     547,   548,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   549,   550,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,     0,     0,     0,     0,     0,     0,   551,   552,
     553,   554,   555,     0,   556,     0,   557,   558,   559,   560,
       0,     0,     0,   561,   562,     0,   563,   564,   565,   566,
     567,   568,    65,   569,   570,   571,     0,     0,     0,     0,
       0,     0,   572,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   573,
     574,   575,     0,    15,     0,     0,   576,   577,     0,     0,
     527,   528,     0,     0,   578,     0,   579,  1542,   580,   581,
     529,   530,   531,   532,   533,     0,     0,     0,     0,     0,
     534,     0,   535,     0,     0,     0,   536,     0,     0,     0,
       0,     0,     0,     0,   537,     0,     0,     0,     0,     0,
     538,     0,     0,   539,     0,     0,   540,     0,     0,     0,
     541,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     542,     0,     0,   543,   544,     0,   328,   329,   330,     0,
     332,   333,   334,   335,   336,   545,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,     0,   350,   351,
     352,     0,     0,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     546,   547,   548,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   549,   550,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,   551,
     552,   553,   554,   555,     0,   556,     0,   557,   558,   559,
     560,     0,     0,     0,   561,   562,     0,   563,   564,   565,
     566,   567,   568,    65,   569,   570,   571,     0,     0,     0,
       0,     0,     0,   572,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     573,   574,   575,     0,    15,     0,     0,   576,   577,     0,
       0,   527,   528,     0,     0,  1551,     0,   579,  1552,   580,
     581,   529,   530,   531,   532,   533,     0,     0,     0,     0,
       0,   534,     0,   535,     0,     0,     0,   536,     0,     0,
       0,     0,     0,     0,     0,   537,     0,     0,     0,     0,
       0,   538,     0,     0,   539,     0,     0,   540,     0,     0,
       0,   541,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   542,     0,     0,   543,   544,     0,   328,   329,   330,
       0,   332,   333,   334,   335,   336,   545,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,     0,   350,
     351,   352,     0,     0,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   546,   547,   548,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   549,   550,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    64,     0,     0,     0,     0,     0,     0,     0,
     551,   552,   553,   554,   555,     0,   556,     0,   557,   558,
     559,   560,     0,     0,     0,   561,   562,     0,   563,   564,
     565,   566,   567,   568,    65,   569,   570,   571,     0,     0,
       0,     0,     0,     0,   572,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   573,   574,   575,     0,    15,     0,     0,   576,   577,
       0,     0,   527,   528,     0,     0,   578,     0,   579,  1557,
     580,   581,   529,   530,   531,   532,   533,     0,     0,     0,
       0,     0,   534,     0,   535,     0,     0,     0,   536,     0,
       0,     0,     0,     0,     0,     0,   537,     0,     0,     0,
       0,     0,   538,     0,     0,   539,     0,     0,   540,     0,
       0,     0,   541,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   542,     0,     0,   543,   544,     0,   328,   329,
     330,     0,   332,   333,   334,   335,   336,   545,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,     0,
     350,   351,   352,     0,     0,   355,   356,   357,   358,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   546,   547,   548,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   549,   550,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,     0,     0,     0,     0,     0,
       0,   551,   552,   553,   554,   555,     0,   556,     0,   557,
     558,   559,   560,     0,     0,     0,   561,   562,     0,   563,
     564,   565,   566,   567,   568,    65,   569,   570,   571,     0,
       0,     0,     0,     0,     0,   572,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   573,   574,   575,     0,    15,     0,     0,   576,
     577,     0,     0,   527,   528,     0,     0,   578,     0,   579,
    1614,   580,   581,   529,   530,   531,   532,   533,     0,     0,
       0,     0,     0,   534,     0,   535,     0,     0,     0,   536,
       0,     0,     0,     0,     0,     0,     0,   537,     0,     0,
       0,     0,     0,   538,     0,     0,   539,     0,     0,   540,
       0,     0,     0,   541,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   542,     0,     0,   543,   544,     0,   328,
     329,   330,     0,   332,   333,   334,   335,   336,   545,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   348,
       0,   350,   351,   352,     0,     0,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   374,   375,   376,   377,   378,
     379,   380,   381,   546,   547,   548,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   549,
     550,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,   551,   552,   553,   554,   555,     0,   556,     0,
     557,   558,   559,   560,     0,     0,     0,   561,   562,     0,
     563,   564,   565,   566,   567,   568,    65,   569,   570,   571,
       0,     0,     0,     0,     0,     0,   572,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   573,   574,   575,     0,    15,     0,     0,
     576,   577,     0,     0,   527,   528,     0,     0,   578,     0,
     579,  1703,   580,   581,   529,   530,   531,   532,   533,     0,
       0,     0,     0,     0,   534,     0,   535,     0,     0,     0,
     536,     0,     0,     0,     0,     0,     0,     0,   537,     0,
       0,     0,     0,     0,   538,     0,     0,   539,     0,     0,
     540,     0,     0,     0,   541,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   542,     0,     0,   543,   544,     0,
     328,   329,   330,     0,   332,   333,   334,   335,   336,   545,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,     0,   350,   351,   352,     0,     0,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,   365,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
     378,   379,   380,   381,   546,   547,   548,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     549,   550,     0,     0,     0,     0,     0,     0,     0,  1724,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,     0,     0,     0,
       0,     0,     0,   551,   552,   553,   554,   555,     0,   556,
       0,   557,   558,   559,   560,     0,     0,     0,   561,   562,
       0,   563,   564,   565,   566,   567,   568,    65,   569,   570,
     571,     0,     0,     0,     0,     0,     0,   572,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   573,   574,   575,     0,    15,     0,
       0,   576,   577,     0,     0,   527,   528,     0,     0,   578,
       0,   579,     0,   580,   581,   529,   530,   531,   532,   533,
       0,     0,     0,     0,     0,   534,     0,   535,     0,     0,
       0,   536,     0,     0,     0,     0,     0,     0,     0,   537,
       0,     0,     0,     0,     0,   538,     0,     0,   539,     0,
       0,   540,     0,     0,     0,   541,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   542,     0,     0,   543,   544,
       0,   328,   329,   330,     0,   332,   333,   334,   335,   336,
     545,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,     0,   350,   351,   352,     0,     0,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   364,   365,   366,
     367,   368,   369,   370,   371,   372,   373,   374,   375,   376,
     377,   378,   379,   380,   381,   546,   547,   548,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   549,   550,     0,     0,     0,     0,     0,     0,     0,
    1784,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,   551,   552,   553,   554,   555,     0,
     556,     0,   557,   558,   559,   560,     0,     0,     0,   561,
     562,     0,   563,   564,   565,   566,   567,   568,    65,   569,
     570,   571,     0,     0,     0,     0,     0,     0,   572,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   573,   574,   575,     0,    15,
       0,     0,   576,   577,     0,     0,   527,   528,     0,     0,
     578,     0,   579,     0,   580,   581,   529,   530,   531,   532,
     533,     0,     0,     0,     0,     0,   534,     0,   535,     0,
       0,     0,   536,     0,     0,     0,     0,     0,     0,     0,
     537,     0,     0,     0,     0,     0,   538,     0,     0,   539,
       0,     0,   540,     0,     0,     0,   541,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   542,     0,     0,   543,
     544,     0,   328,   329,   330,     0,   332,   333,   334,   335,
     336,   545,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,     0,   350,   351,   352,     0,     0,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   546,   547,   548,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   549,   550,     0,     0,     0,     0,     0,     0,
       0,  1785,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    64,     0,     0,
       0,     0,     0,     0,     0,   551,   552,   553,   554,   555,
       0,   556,     0,   557,   558,   559,   560,     0,     0,     0,
     561,   562,     0,   563,   564,   565,   566,   567,   568,    65,
     569,   570,   571,     0,     0,     0,     0,     0,     0,   572,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   573,   574,   575,     0,
      15,     0,     0,   576,   577,     0,     0,   527,   528,     0,
       0,   578,     0,   579,     0,   580,   581,   529,   530,   531,
     532,   533,     0,     0,     0,     0,     0,   534,     0,   535,
       0,     0,     0,   536,     0,     0,     0,     0,     0,     0,
       0,   537,     0,     0,     0,     0,     0,   538,     0,     0,
     539,     0,     0,   540,     0,     0,     0,   541,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   542,     0,     0,
     543,   544,     0,   328,   329,   330,     0,   332,   333,   334,
     335,   336,   545,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,     0,   350,   351,   352,     0,     0,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   546,   547,   548,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   549,   550,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
       0,     0,     0,     0,     0,     0,   551,   552,   553,   554,
     555,     0,   556,     0,   557,   558,   559,   560,     0,     0,
       0,   561,   562,     0,   563,   564,   565,   566,   567,   568,
      65,   569,   570,   571,     0,     0,     0,     0,     0,     0,
     572,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   573,   574,   575,
       0,    15,     0,     0,   576,   577,     0,     0,   527,   528,
       0,     0,   578,     0,   579,     0,   580,   581,   529,   530,
     531,   532,   533,     0,     0,     0,     0,     0,   534,     0,
     535,     0,     0,     0,   536,     0,     0,     0,     0,     0,
       0,     0,   537,     0,     0,     0,     0,     0,   538,     0,
       0,   539,     0,     0,   540,     0,     0,     0,   541,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   542,     0,
       0,   543,   544,     0,   328,   329,   330,     0,   332,   333,
     334,   335,   336,   545,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,     0,   350,   351,   352,     0,
       0,   355,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   377,   378,   379,   380,   381,   546,   547,
     548,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   549,   550,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,   551,   552,   553,
     554,   555,     0,   556,     0,   557,   558,   559,   560,     0,
       0,     0,   561,   562,     0,   563,   564,   565,   566,   567,
     568,    65,   569,   570,   571,     0,   794,     0,     0,     0,
       0,   572,   322,     0,     0,     0,   829,   830,   323,     0,
       0,     0,     0,     0,   324,     0,     0,     0,   573,   574,
     575,     0,    15,     0,   325,   576,   577,     0,     0,     0,
       0,     0,   326,  1528,     0,   579,     0,   580,   581,     0,
       0,     0,     0,     0,     0,     0,     0,   327,     0,     0,
       0,     0,     0,     0,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   377,   378,   379,   380,   381,   382,   383,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   831,   832,   833,   834,   835,     0,
       0,   836,   837,   838,   839,     0,   840,   841,   842,   843,
       0,     0,     0,     0,   844,     0,   845,   846,     0,    64,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   384,     0,     0,     0,     0,     0,     0,     0,
     796,   829,   830,     0,     0,     0,   322,     0,     0,     0,
       0,    65,   323,     0,     0,     0,     0,     0,   324,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   325,     0,
       0,     0,     0,     0,     0,     0,   326,   854,   855,   856,
     857,   858,   859,   860,   861,     0,     0,     0,     0,     0,
       0,   327,     0,     0,   862,   863,     0,   385,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   382,   383,     0,     0,     0,     0,     0,   831,
     832,   833,   834,   835,     0,     0,   836,   837,   838,   839,
       0,   840,   841,   842,   843,     0,     0,     0,     0,   844,
       0,   845,   846,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   384,     0,     0,     0,
       0,     0,     0,     0,     0,   829,   830,     0,     0,     0,
     322,     0,     0,     0,     0,    65,   323,     0,     0,     0,
       0,     0,   324,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   325,   855,   856,   857,   858,   859,   860,   861,
     326,     0,     0,     0,     0,     0,     0,     0,     0,   862,
     863,     0,     0,     0,     0,   327,     0,     0,     0,     0,
       0,   385,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   382,   383,     0,     0,
       0,     0,     0,   831,   832,   833,   834,     0,     0,     0,
       0,     0,     0,   839,     0,   840,   841,   842,   843,     0,
       0,     0,     0,   844,     0,   845,   846,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    64,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     384,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   322,     0,     0,    65,
       0,     0,   323,     0,     0,     0,     0,     0,   324,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   325,   857,
     858,   859,   860,   861,     0,     0,   326,     0,     0,     0,
       0,     0,     0,   862,   863,     0,     0,     0,     0,     0,
       0,   327,     0,     0,     0,   385,     0,   644,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   382,   383,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   384,     0,     0,     0,
       0,     0,     0,     0,     0,    13,     0,     0,     0,     0,
     322,     0,     0,     0,     0,   647,   323,     0,     0,     0,
       0,     0,   324,     0,     0,     0,     0,    14,     0,     0,
       0,     0,   325,   648,     0,     0,     0,     0,     0,     0,
     326,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   327,     0,     0,     0,     0,
       0,   385,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   382,   383,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    64,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     384,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   322,     0,     0,   829,   830,    65,
     323,     0,     0,     0,     0,     0,   324,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   325,     0,     0,     0,
       0,     0,     0,     0,   326,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   829,   830,   327,
       0,     0,     0,     0,     0,   385,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,     0,     0,     0,     0,     0,  1133,  1134,  1135,
    1136,  1137,  1138,  1139,  1140,   831,   832,   833,   834,   835,
    1141,  1142,   836,   837,   838,   839,  1143,   840,   841,   842,
     843,  -451,     0,     0,     0,   844,   972,   845,   846,  1144,
    1145,    64,     0,   847,   848,   849,  1146,  1147,  1148,   850,
     829,   830,     0,     0,   384,   831,   832,   833,   834,   835,
       0,     0,   836,   837,   838,   839,     0,   840,   841,   842,
     843,     0,     0,   647,     0,   844,     0,   845,   846,     0,
       0,     0,     0,   847,   848,   849,   829,   830,     0,   850,
       0,     0,     0,  1149,   851,     0,   852,   853,   854,   855,
     856,   857,   858,   859,   860,   861,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   862,   863,     0,     0,   385,
     580,   938,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   851,     0,   852,   853,   854,   855,
     856,   857,   858,   859,   860,   861,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   862,   863,     0,     0,   893,
       0,     0,     0,     0,     0,     0,     0,     0,   831,   832,
     833,   834,   835,     0,     0,   836,   837,   838,   839,     0,
     840,   841,   842,   843,     0,     0,     0,     0,   844,     0,
     845,   846,   829,   830,     0,     0,   847,   848,   849,     0,
       0,     0,   850,     0,   831,   832,   833,   834,   835,     0,
       0,   836,   837,   838,   839,     0,   840,   841,   842,   843,
       0,     0,     0,     0,   844,     0,   845,   846,   829,   830,
       0,     0,   847,   848,   849,     0,     0,     0,   850,     0,
       0,     0,     0,     0,     0,     0,     0,   851,     0,   852,
     853,   854,   855,   856,   857,   858,   859,   860,   861,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   862,   863,
       0,     0,  1179,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   851,     0,   852,   853,   854,   855,   856,
     857,   858,   859,   860,   861,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   862,   863,     0,     0,  1282,     0,
     831,   832,   833,   834,   835,     0,     0,   836,   837,   838,
     839,     0,   840,   841,   842,   843,     0,     0,     0,     0,
     844,     0,   845,   846,   829,   830,     0,     0,   847,   848,
     849,     0,     0,     0,   850,     0,   831,   832,   833,   834,
     835,     0,     0,   836,   837,   838,   839,     0,   840,   841,
     842,   843,     0,     0,     0,     0,   844,     0,   845,   846,
     829,   830,     0,     0,   847,   848,   849,     0,     0,     0,
     850,     0,     0,     0,     0,     0,     0,     0,     0,   851,
       0,   852,   853,   854,   855,   856,   857,   858,   859,   860,
     861,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     862,   863,     0,     0,  1285,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   851,     0,   852,   853,   854,
     855,   856,   857,   858,   859,   860,   861,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   862,   863,     0,     0,
    1287,     0,   831,   832,   833,   834,   835,     0,     0,   836,
     837,   838,   839,     0,   840,   841,   842,   843,     0,     0,
       0,     0,   844,     0,   845,   846,   829,   830,     0,     0,
     847,   848,   849,     0,     0,     0,   850,     0,   831,   832,
     833,   834,   835,     0,     0,   836,   837,   838,   839,     0,
     840,   841,   842,   843,     0,     0,     0,     0,   844,     0,
     845,   846,   829,   830,     0,     0,   847,   848,   849,     0,
       0,     0,   850,     0,     0,     0,     0,     0,     0,     0,
       0,   851,     0,   852,   853,   854,   855,   856,   857,   858,
     859,   860,   861,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   862,   863,     0,     0,  1295,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   851,     0,   852,
     853,   854,   855,   856,   857,   858,   859,   860,   861,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   862,   863,
       0,     0,  1296,     0,   831,   832,   833,   834,   835,     0,
       0,   836,   837,   838,   839,     0,   840,   841,   842,   843,
       0,     0,     0,     0,   844,     0,   845,   846,   829,   830,
       0,     0,   847,   848,   849,     0,     0,     0,   850,     0,
     831,   832,   833,   834,   835,     0,     0,   836,   837,   838,
     839,     0,   840,   841,   842,   843,     0,     0,     0,     0,
     844,     0,   845,   846,   829,   830,     0,     0,   847,   848,
     849,     0,     0,     0,   850,     0,     0,     0,     0,     0,
       0,     0,     0,   851,     0,   852,   853,   854,   855,   856,
     857,   858,   859,   860,   861,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   862,   863,     0,     0,  1297,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   851,
       0,   852,   853,   854,   855,   856,   857,   858,   859,   860,
     861,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     862,   863,     0,     0,  1298,     0,   831,   832,   833,   834,
     835,     0,     0,   836,   837,   838,   839,     0,   840,   841,
     842,   843,     0,     0,     0,     0,   844,     0,   845,   846,
     829,   830,     0,     0,   847,   848,   849,     0,     0,     0,
     850,     0,   831,   832,   833,   834,   835,     0,     0,   836,
     837,   838,   839,     0,   840,   841,   842,   843,     0,     0,
       0,     0,   844,     0,   845,   846,   829,   830,     0,     0,
     847,   848,   849,     0,     0,     0,   850,     0,     0,     0,
       0,     0,     0,     0,     0,   851,     0,   852,   853,   854,
     855,   856,   857,   858,   859,   860,   861,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   862,   863,     0,     0,
    1299,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   851,     0,   852,   853,   854,   855,   856,   857,   858,
     859,   860,   861,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   862,   863,     0,     0,  1300,     0,   831,   832,
     833,   834,   835,     0,     0,   836,   837,   838,   839,     0,
     840,   841,   842,   843,     0,     0,     0,     0,   844,     0,
     845,   846,   829,   830,     0,     0,   847,   848,   849,     0,
       0,     0,   850,     0,   831,   832,   833,   834,   835,     0,
       0,   836,   837,   838,   839,     0,   840,   841,   842,   843,
       0,     0,     0,     0,   844,     0,   845,   846,   829,   830,
       0,     0,   847,   848,   849,     0,     0,     0,   850,     0,
       0,     0,     0,     0,     0,     0,     0,   851,     0,   852,
     853,   854,   855,   856,   857,   858,   859,   860,   861,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   862,   863,
       0,     0,  1387,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   851,     0,   852,   853,   854,   855,   856,
     857,   858,   859,   860,   861,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   862,   863,     0,     0,  1470,     0,
     831,   832,   833,   834,   835,     0,     0,   836,   837,   838,
     839,     0,   840,   841,   842,   843,     0,     0,     0,     0,
     844,     0,   845,   846,   829,   830,     0,     0,   847,   848,
     849,     0,     0,     0,   850,     0,   831,   832,   833,   834,
     835,     0,     0,   836,   837,   838,   839,     0,   840,   841,
     842,   843,     0,     0,     0,     0,   844,     0,   845,   846,
     829,   830,     0,     0,   847,   848,   849,     0,     0,     0,
     850,     0,     0,     0,     0,     0,     0,     0,     0,   851,
       0,   852,   853,   854,   855,   856,   857,   858,   859,   860,
     861,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     862,   863,     0,     0,  1473,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   851,     0,   852,   853,   854,
     855,   856,   857,   858,   859,   860,   861,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   862,   863,     0,     0,
    1518,     0,   831,   832,   833,   834,   835,     0,     0,   836,
     837,   838,   839,     0,   840,   841,   842,   843,     0,     0,
       0,     0,   844,     0,   845,   846,   829,   830,     0,     0,
     847,   848,   849,     0,     0,     0,   850,     0,   831,   832,
     833,   834,   835,     0,     0,   836,   837,   838,   839,     0,
     840,   841,   842,   843,     0,     0,     0,     0,   844,     0,
     845,   846,   829,   830,     0,     0,   847,   848,   849,     0,
       0,     0,   850,     0,     0,     0,     0,     0,     0,     0,
       0,   851,     0,   852,   853,   854,   855,   856,   857,   858,
     859,   860,   861,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   862,   863,     0,     0,  1568,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   851,     0,   852,
     853,   854,   855,   856,   857,   858,   859,   860,   861,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   862,   863,
       0,     0,  1635,     0,   831,   832,   833,   834,   835,     0,
       0,   836,   837,   838,   839,     0,   840,   841,   842,   843,
       0,     0,     0,     0,   844,     0,   845,   846,   829,   830,
       0,     0,   847,   848,   849,     0,     0,     0,   850,     0,
     831,   832,   833,   834,   835,     0,     0,   836,   837,   838,
     839,     0,   840,   841,   842,   843,     0,     0,     0,     0,
     844,     0,   845,   846,   829,   830,     0,     0,   847,   848,
     849,     0,     0,     0,   850,     0,     0,     0,     0,     0,
       0,     0,     0,   851,     0,   852,   853,   854,   855,   856,
     857,   858,   859,   860,   861,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   862,   863,     0,     0,  1636,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   851,
       0,   852,   853,   854,   855,   856,   857,   858,   859,   860,
     861,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     862,   863,     0,     0,  1649,     0,   831,   832,   833,   834,
     835,     0,     0,   836,   837,   838,   839,     0,   840,   841,
     842,   843,     0,     0,     0,     0,   844,     0,   845,   846,
     829,   830,     0,     0,   847,   848,   849,     0,     0,     0,
     850,     0,   831,   832,   833,   834,   835,     0,     0,   836,
     837,   838,   839,     0,   840,   841,   842,   843,     0,     0,
       0,     0,   844,     0,   845,   846,   829,   830,     0,     0,
     847,   848,   849,     0,     0,     0,   850,     0,     0,     0,
       0,     0,     0,     0,     0,   851,     0,   852,   853,   854,
     855,   856,   857,   858,   859,   860,   861,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   862,   863,     0,     0,
    1651,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   851,     0,   852,   853,   854,   855,   856,   857,   858,
     859,   860,   861,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   862,   863,     0,     0,  1653,     0,   831,   832,
     833,   834,   835,     0,     0,   836,   837,   838,   839,     0,
     840,   841,   842,   843,     0,     0,     0,     0,   844,     0,
     845,   846,   829,   830,     0,     0,   847,   848,   849,     0,
       0,     0,   850,     0,   831,   832,   833,   834,   835,     0,
       0,   836,   837,   838,   839,     0,   840,   841,   842,   843,
       0,     0,     0,     0,   844,     0,   845,   846,   829,   830,
       0,     0,   847,   848,   849,     0,     0,     0,   850,     0,
       0,     0,     0,     0,     0,     0,     0,   851,     0,   852,
     853,   854,   855,   856,   857,   858,   859,   860,   861,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   862,   863,
       0,     0,  1657,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   851,     0,   852,   853,   854,   855,   856,
     857,   858,   859,   860,   861,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   862,   863,     0,     0,  1719,     0,
     831,   832,   833,   834,   835,     0,     0,   836,   837,   838,
     839,     0,   840,   841,   842,   843,     0,     0,     0,     0,
     844,     0,   845,   846,   829,   830,     0,     0,   847,   848,
     849,     0,     0,     0,   850,     0,   831,   832,   833,   834,
     835,     0,     0,   836,   837,   838,   839,     0,   840,   841,
     842,   843,     0,     0,     0,     0,   844,     0,   845,   846,
     829,   830,     0,     0,   847,   848,   849,     0,     0,     0,
     850,     0,     0,     0,     0,     0,     0,     0,     0,   851,
       0,   852,   853,   854,   855,   856,   857,   858,   859,   860,
     861,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     862,   863,     0,     0,  1729,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   851,     0,   852,   853,   854,
     855,   856,   857,   858,   859,   860,   861,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   862,   863,     0,     0,
    1730,     0,   831,   832,   833,   834,   835,     0,     0,   836,
     837,   838,   839,     0,   840,   841,   842,   843,     0,     0,
       0,     0,   844,     0,   845,   846,   829,   830,     0,     0,
     847,   848,   849,     0,     0,     0,   850,     0,   831,   832,
     833,   834,   835,     0,     0,   836,   837,   838,   839,     0,
     840,   841,   842,   843,     0,     0,     0,     0,   844,     0,
     845,   846,   829,   830,     0,     0,   847,   848,   849,     0,
       0,     0,   850,     0,     0,     0,     0,     0,     0,     0,
       0,   851,     0,   852,   853,   854,   855,   856,   857,   858,
     859,   860,   861,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   862,   863,     0,     0,  1732,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   851,     0,   852,
     853,   854,   855,   856,   857,   858,   859,   860,   861,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   862,   863,
       0,     0,  1733,     0,   831,   832,   833,   834,   835,     0,
       0,   836,   837,   838,   839,     0,   840,   841,   842,   843,
       0,     0,     0,     0,   844,     0,   845,   846,   829,   830,
       0,     0,   847,   848,   849,     0,     0,     0,   850,     0,
     831,   832,   833,   834,   835,     0,     0,   836,   837,   838,
     839,     0,   840,   841,   842,   843,     0,     0,     0,     0,
     844,     0,   845,   846,   829,   830,     0,     0,   847,   848,
     849,     0,     0,     0,   850,     0,     0,     0,     0,     0,
       0,     0,     0,   851,     0,   852,   853,   854,   855,   856,
     857,   858,   859,   860,   861,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   862,   863,     0,     0,  1753,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   851,
       0,   852,   853,   854,   855,   856,   857,   858,   859,   860,
     861,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     862,   863,     0,     0,  1756,     0,   831,   832,   833,   834,
     835,     0,     0,   836,   837,   838,   839,     0,   840,   841,
     842,   843,     0,     0,     0,     0,   844,     0,   845,   846,
     829,   830,     0,     0,   847,   848,   849,     0,     0,     0,
     850,     0,   831,   832,   833,   834,   835,     0,     0,   836,
     837,   838,   839,     0,   840,   841,   842,   843,     0,     0,
       0,     0,   844,     0,   845,   846,   829,   830,     0,     0,
     847,   848,   849,     0,     0,     0,   850,     0,     0,     0,
       0,     0,     0,     0,     0,   851,     0,   852,   853,   854,
     855,   856,   857,   858,   859,   860,   861,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   862,   863,     0,     0,
    1765,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   851,     0,   852,   853,   854,   855,   856,   857,   858,
     859,   860,   861,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   862,   863,     0,     0,  1836,     0,   831,   832,
     833,   834,   835,     0,     0,   836,   837,   838,   839,     0,
     840,   841,   842,   843,     0,     0,     0,     0,   844,     0,
     845,   846,   829,   830,     0,     0,   847,   848,   849,     0,
       0,     0,   850,     0,   831,   832,   833,   834,   835,     0,
       0,   836,   837,   838,   839,     0,   840,   841,   842,   843,
       0,     0,     0,     0,   844,     0,   845,   846,   829,   830,
       0,     0,   847,   848,   849,     0,     0,     0,   850,     0,
       0,     0,     0,     0,     0,     0,     0,   851,     0,   852,
     853,   854,   855,   856,   857,   858,   859,   860,   861,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   862,   863,
       0,     0,  1837,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   851,     0,   852,   853,   854,   855,   856,
     857,   858,   859,   860,   861,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   862,   863,   897,     0,     0,     0,
     831,   832,   833,   834,   835,     0,     0,   836,   837,   838,
     839,     0,   840,   841,   842,   843,     0,     0,     0,     0,
     844,     0,   845,   846,   829,   830,     0,     0,   847,   848,
     849,     0,     0,     0,   850,     0,   831,   832,   833,   834,
     835,     0,     0,   836,   837,   838,   839,     0,   840,   841,
     842,   843,     0,     0,     0,     0,   844,     0,   845,   846,
     829,   830,     0,     0,   847,   848,   849,     0,     0,     0,
     850,     0,     0,     0,     0,     0,     0,     0,     0,   851,
       0,   852,   853,   854,   855,   856,   857,   858,   859,   860,
     861,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     862,   863,  1173,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   851,     0,   852,   853,   854,
     855,   856,   857,   858,   859,   860,   861,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   862,   863,  1369,     0,
       0,     0,   831,   832,   833,   834,   835,     0,     0,   836,
     837,   838,   839,     0,   840,   841,   842,   843,     0,     0,
       0,     0,   844,     0,   845,   846,   829,   830,     0,     0,
     847,   848,   849,     0,     0,     0,   850,     0,   831,   832,
     833,   834,   835,     0,     0,   836,   837,   838,   839,     0,
     840,   841,   842,   843,     0,     0,     0,     0,   844,     0,
     845,   846,     0,     0,     0,     0,   847,   848,   849,     0,
       0,     0,   850,     0,     0,     0,     0,     0,     0,     0,
       0,   851,     0,   852,   853,   854,   855,   856,   857,   858,
     859,   860,   861,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   862,   863,  1385,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   851,     0,   852,
     853,   854,   855,   856,   857,   858,   859,   860,   861,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   862,   863,
    1549,     0,     0,     0,   831,   832,   833,   834,   835,     0,
       0,   836,   837,   838,   839,     0,   840,   841,   842,   843,
     400,   401,     0,     0,   844,     0,   845,   846,     0,     0,
       0,     0,   847,   848,   849,     0,     0,   402,   850,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   829,   830,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   851,     0,   852,   853,   854,   855,   856,
     857,   858,   859,   860,   861,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   862,   863,  1555,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     403,   404,   405,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   415,   416,   417,   418,   419,   420,     0,     0,
     421,   422,   423,     0,     0,     0,     0,     0,     0,   424,
     425,   426,   427,   428,     0,     0,   429,   430,   431,   432,
     433,   434,   435,   829,   830,     0,     0,     0,     0,     0,
       0,     0,   831,   832,   833,   834,   835,     0,     0,   836,
     837,   838,   839,     0,   840,   841,   842,   843,     0,     0,
       0,     0,   844,     0,   845,   846,     0,     0,     0,     0,
     847,   848,   849,     0,     0,     0,   850,   436,     0,   437,
     438,   439,   440,   441,   442,   443,   444,   445,   446,    52,
       0,   447,   448,     0,     0,     0,     0,     0,   449,   450,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   851,     0,   852,   853,   854,   855,   856,   857,   858,
     859,   860,   861,     0,     0,   829,   830,     0,     0,     0,
       0,     0,   862,   863,     0,     0,     0,     0,     0,     0,
       0,   831,   832,   833,   834,   835,     0,     0,   836,   837,
     838,   839,     0,   840,   841,   842,   843,     0,     0,     0,
       0,   844,     0,   845,   846,   829,   830,     0,     0,   847,
     848,   849,     0,     0,     0,   850,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      13,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    14,     0,     0,     0,     0,     0,     0,     0,
     851,     0,   852,   853,   854,   855,   856,   857,   858,   859,
     860,   861,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   862,   863,   831,   832,   833,   834,   835,     0,     0,
     836,   837,   838,   839,     0,   840,   841,   842,   843,     0,
     829,   830,     0,   844,     0,   845,   846,     0,     0,  1037,
       0,   847,   848,   849,     0,     0,     0,   850,     0,     0,
       0,     0,     0,   831,   832,   833,   834,   835,     0,     0,
     836,   837,   838,   839,     0,   840,   841,   842,   843,   829,
     830,     0,     0,   844,     0,   845,   846,     0,     0,  1306,
       0,   847,   848,   849,     0,     0,     0,   850,     0,     0,
       0,     0,   851,     0,   852,   853,   854,   855,   856,   857,
     858,   859,   860,   861,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   862,   863,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   851,     0,   852,   853,   854,   855,   856,   857,
     858,   859,   860,   861,     0,     0,     0,     0,   831,   832,
     833,   834,   835,   862,   863,   836,   837,   838,   839,     0,
     840,   841,   842,   843,   829,   830,     0,     0,   844,     0,
     845,   846,     0,     0,     0,     0,   847,   848,   849,     0,
       0,     0,   850,     0,     0,     0,     0,   831,   832,   833,
     834,   835,     0,     0,   836,   837,   838,   839,     0,   840,
     841,   842,   843,   829,   830,     0,     0,   844,     0,   845,
     846,     0,     0,     0,     0,   847,   848,   849,     0,     0,
       0,   850,     0,     0,     0,     0,     0,   851,  1374,   852,
     853,   854,   855,   856,   857,   858,   859,   860,   861,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   862,   863,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   851,  1513,   852,   853,
     854,   855,   856,   857,   858,   859,   860,   861,     0,     0,
       0,     0,   831,   832,   833,   834,   835,   862,   863,   836,
     837,   838,   839,     0,   840,   841,   842,   843,   829,   830,
       0,     0,   844,     0,   845,   846,     0,     0,     0,     0,
     847,   848,   849,     0,     0,     0,   850,     0,     0,     0,
       0,   831,   832,   833,   834,   835,     0,     0,   836,   837,
     838,   839,     0,   840,   841,   842,   843,   829,   830,     0,
       0,   844,     0,   845,   846,     0,     0,     0,     0,   847,
     848,   849,     0,  1766,     0,   850,     0,     0,     0,     0,
       0,   851,     0,   852,   853,   854,   855,   856,   857,   858,
     859,   860,   861,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   862,   863,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   829,   830,     0,     0,     0,     0,     0,
     851,     0,   852,   853,   854,   855,   856,   857,   858,   859,
     860,   861,     0,     0,     0,     0,   831,   832,   833,   834,
     835,   862,   863,   836,   837,   838,   839,     0,   840,   841,
     842,   843,   829,   830,     0,     0,   844,     0,   845,   846,
       0,     0,     0,     0,   847,   848,   849,     0,     0,     0,
    -894,     0,     0,     0,     0,   831,   832,   833,   834,   835,
       0,     0,   836,   837,   838,   839,     0,   840,   841,   842,
     843,     0,     0,     0,     0,   844,     0,   845,   846,     0,
       0,     0,     0,   847,     0,   849,     0,     0,   829,   830,
       0,     0,     0,     0,     0,   851,     0,   852,   853,   854,
     855,   856,   857,   858,   859,   860,   861,     0,     0,     0,
       0,   831,   832,   833,   834,   835,   862,   863,   836,   837,
     838,   839,     0,   840,   841,   842,   843,     0,     0,     0,
       0,   844,     0,   845,   846,     0,   852,   853,   854,   855,
     856,   857,   858,   859,   860,   861,     0,     0,     0,     0,
     831,   832,   833,   834,   835,   862,   863,   836,   837,   838,
     839,     0,   840,   841,   842,   843,     0,     0,     0,     0,
     844,     0,   845,   846,     0,     0,     0,     0,   847,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   852,   853,   854,   855,   856,   857,   858,   859,
     860,   861,     0,     0,     0,     0,   831,   832,   833,   834,
     835,   862,   863,   836,   837,   838,   839,     0,   840,   841,
     842,   843,     0,     0,     0,     0,   844,     0,   845,   846,
       0,   852,   853,   854,   855,   856,   857,   858,   859,   860,
     861,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     862,   863,     0,     0,     0,     0,  1078,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   853,   854,
     855,   856,   857,   858,   859,   860,   861,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   862,   863,   328,   329,
     330,  1082,   332,   333,   334,   335,   336,   545,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,     0,
     350,   351,   352,     0,     0,   355,   356,   357,   358,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   376,   377,   378,   379,
     380,   381,     0,   328,   329,   330,     0,   332,   333,   334,
     335,   336,   545,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,     0,   350,   351,   352,     0,     0,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,     0,  1079,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1080,     0,     0,     0,  1370,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1083,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1084,   328,   329,   330,     0,   332,   333,   334,   335,   336,
     545,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,     0,   350,   351,   352,     0,     0,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   364,   365,   366,
     367,   368,   369,   370,   371,   372,   373,   374,   375,   376,
     377,   378,   379,   380,   381,   328,   329,   330,     0,   332,
     333,   334,   335,   336,   545,   338,   339,   340,   341,   342,
     343,   344,   345,   346,   347,   348,     0,   350,   351,   352,
       0,     0,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   368,   369,   370,   371,   372,
     373,   374,   375,   376,   377,   378,   379,   380,   381,     0,
       0,  1371,     0,     0,     0,     0,     0,     0,     0,     0,
     243,     0,     0,     0,     0,     0,     0,     0,  1372,     0,
       0,     0,     0,     0,     0,     0,  1118,  1119,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   244,     0,   245,     0,
     246,   247,   248,   249,   250,  1120,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,     0,   262,   263,
     264,     0,  1121,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1122,  1123,     0,   292,   293,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   294
};

static const yytype_int16 yycheck[] =
{
       1,   633,   518,   610,   614,   818,   772,   221,   560,   822,
     823,   524,   237,   732,   518,    15,    16,   913,    19,   296,
     183,   501,   797,   500,  1309,     7,   714,   715,  1011,   463,
     827,   469,   884,   518,   886,  1013,   888,  1028,  1202,     5,
     237,    19,    20,  1034,    20,    20,     8,    87,   794,    20,
     796,   624,   798,   626,    33,   628,     0,    53,    53,   805,
     498,   500,   501,     7,  1585,   795,    20,   797,   149,   815,
      70,    71,    72,    20,   150,     4,    22,   590,    40,   113,
    1678,    33,   895,    20,   149,    46,    30,    20,    32,   699,
      34,   166,   152,   153,   149,   198,    40,   216,   150,   100,
     149,   102,   712,   152,   153,   188,    50,   189,   160,   181,
     614,    57,    56,   183,   114,   115,   116,   117,    21,    22,
     558,   559,   150,   198,   243,    57,   198,   210,   186,   614,
    1294,    63,   160,  1565,   150,    47,    80,   150,   219,   242,
     178,   217,   149,   224,   220,   183,    33,   160,   311,   131,
     208,   178,  1750,  1674,   219,    67,   183,   158,    62,   125,
     126,   242,   245,   198,   219,   217,   150,   242,   129,   224,
     198,   243,   918,    60,    61,   245,   160,    15,    16,   239,
     240,   125,   126,   223,   242,   181,   999,   242,   222,   217,
     239,   240,  1624,   990,   222,   699,  1009,   216,   180,  1012,
     216,   217,   198,   219,   217,   243,   222,   245,   712,   244,
    1495,   650,   219,   217,   699,   210,   655,   224,   245,    55,
     198,   219,   241,   224,     7,   187,   188,   712,    57,   211,
     212,    15,    16,   217,    63,   242,    57,   237,   141,   142,
     216,   216,    63,   222,   188,   216,   149,   209,   210,   152,
     153,   154,   149,    33,   183,   187,   159,   223,   240,   869,
     147,   579,   216,  1720,   151,   101,   210,    50,   166,   216,
     222,   959,   469,  1070,   314,  1439,   242,   209,   240,   216,
      60,    61,   228,   216,  1448,   229,   165,   150,   240,   150,
     515,   516,  1275,   239,   246,   155,   240,   160,  1276,   160,
      47,   498,   150,   500,   518,   166,  1052,   186,   522,   919,
     524,    57,   160,   228,  1305,   216,   175,    63,   515,   516,
    1777,   219,   219,   305,   183,   935,   308,   224,   188,   208,
     216,   218,  1617,   771,  1022,   222,   239,   240,   225,   198,
     241,  1071,   188,    90,    91,   242,    93,   785,   216,    33,
     210,   189,   190,   240,   217,   193,   217,   195,   216,   246,
     198,   558,   559,   201,   210,   869,   186,   147,   198,   217,
     216,   151,   167,   168,   169,   243,    60,    61,   224,   181,
     166,   699,   223,   242,   869,   385,   186,   225,   208,   175,
     615,   616,   617,   181,   619,   620,   198,   166,   623,   219,
     625,   242,   627,   228,   242,   189,   190,   245,   208,   193,
     198,   195,   198,   186,   198,   919,   216,   201,   615,   616,
     617,   166,   619,   620,   701,   188,   623,   940,   625,   198,
     627,   935,   629,   710,   919,   208,   188,   876,   218,   653,
     242,   225,   222,   657,   224,   225,  1342,   210,   243,    57,
     935,   223,   770,   892,   217,    63,   130,   458,   210,   459,
     240,   245,   780,   147,   216,   783,   246,   151,  1265,   469,
     242,   244,   245,   188,    21,    22,   961,   962,   963,   964,
     965,   966,   967,   968,   969,   970,   971,  1039,   973,   974,
     975,   976,   977,   978,   216,   210,   934,  1049,   498,  1708,
     500,  1710,   217,  1494,  1077,  1714,  1715,   188,   210,   827,
    1362,   512,   513,  1504,    34,   515,   516,   518,   242,   520,
     188,   243,   224,   524,   958,  1213,  1214,  1215,   189,   210,
    1305,   531,   532,  1752,   218,   216,   130,  1433,   222,   166,
     224,   225,   210,    63,  1763,  1320,   175,   865,   175,   987,
     988,   188,   220,   180,   219,   188,   240,   222,   558,   559,
     998,  1000,   246,  1610,  1773,  1003,  1004,   216,  1006,   198,
    1008,   198,  1010,   210,   771,  1305,   216,   210,  1308,   216,
     713,   714,   715,   216,  1803,  1804,   188,   224,   785,   130,
    1320,   224,   241,   756,   141,   142,   223,   210,   239,   732,
     213,   919,   149,   243,   151,   152,   153,   154,   210,   610,
     130,   216,   159,   746,   216,   615,   616,   617,   220,   619,
     620,   188,  1518,   623,   216,   625,   202,   627,   241,   629,
     188,   188,   829,   830,   198,   173,  1683,  1684,   243,   159,
     917,   198,   198,   210,   216,   646,   166,   844,   216,   216,
     927,   243,   210,   210,  1701,  1702,   216,   224,   216,   216,
     198,   199,   200,   187,   188,   862,   224,   944,   216,   894,
     947,   243,   990,  1525,   216,   243,   188,   902,   198,   181,
     905,   213,   214,   243,   223,   209,   245,   901,  1168,  1166,
     228,   229,   239,   240,  1207,   243,   198,   894,   210,   241,
     175,   239,   173,   242,   216,   902,   707,  1025,   905,   241,
    1757,  1758,   224,   233,  1480,  1033,   717,   718,   216,    21,
      22,   198,   242,   198,  1576,   188,   188,   198,   199,   200,
    1169,   216,   216,   734,   735,   736,   737,   934,   739,   872,
     216,     5,     6,   744,     8,   243,   747,   210,   210,    70,
      71,    72,   752,   216,   216,   175,   241,   216,   229,   243,
     175,   224,   224,   763,   989,   186,   980,   243,   239,   994,
      57,   771,    36,  1346,   216,   216,    63,   167,   198,   169,
     188,   188,   241,   198,   198,   785,   198,   208,   187,   922,
     987,   988,   989,   114,   115,   116,   117,   994,  1528,   241,
     241,   998,   210,   210,   937,   188,  1003,  1004,   216,  1006,
     209,  1008,   223,  1010,  1011,   198,   224,  1042,  1043,   198,
    1672,  1551,    57,  1719,    57,   825,   959,   210,    63,   216,
      63,   242,   219,    21,    22,   222,  1274,   139,   140,   141,
     142,   143,  1280,   223,   146,  1042,  1043,   149,    79,   151,
     152,   153,   154,   198,  1293,   181,  1575,   159,   219,   161,
     162,   222,   242,    94,   188,   188,   188,  1440,    99,   210,
     101,   872,   198,   244,   245,   216,   198,   131,   132,   133,
     134,   135,   136,   137,   138,    57,   210,   210,   210,  1022,
    1329,  1330,   216,   216,   894,   181,   150,   233,   899,    12,
     224,   224,   902,   181,   219,   905,   160,  1345,   223,  1106,
      23,    24,   198,   210,  1191,   167,   170,   171,   172,   216,
     198,   223,   224,   225,   226,   227,   228,   229,  1694,  1154,
     209,  1447,   202,   212,   934,   220,   215,   239,   240,   224,
     189,   190,   191,  1447,   210,    33,   195,   220,   220,  1762,
     216,   224,   224,   141,   142,   202,   203,  1154,  1183,    96,
      97,   149,  1447,   151,   152,   153,   154,   949,   210,  1166,
     129,   159,    60,    61,   216,   189,   190,   191,  1042,  1043,
     981,   195,   245,  1301,  1797,   986,  1183,   987,   988,   989,
     210,   210,   209,   210,   994,   212,   216,   216,   998,  1132,
     198,  1319,   217,  1003,  1004,    66,  1006,  1325,  1008,  1582,
    1010,  1011,   198,   220,  1332,   220,  1334,   224,   219,   224,
     220,  1751,  1752,   220,   224,   198,    35,   224,    33,  1759,
    1469,   220,    35,  1763,  1764,   224,   242,   219,  1477,   227,
     228,   229,  1042,  1043,   220,    43,   220,   242,   224,  1367,
     224,   239,   240,   220,   223,    60,    61,   224,   223,   147,
    1692,  1379,   150,   151,   242,  1795,  1384,  1200,   223,  1676,
    1622,   223,   160,  1803,  1804,  1627,   223,  1274,  1275,   242,
    1213,  1214,  1215,  1280,   223,  1218,   242,  1220,   223,  1222,
     223,  1224,   223,  1226,   223,  1228,   220,  1230,   198,  1232,
    1538,  1234,    75,  1236,   198,  1238,    79,   198,  1241,   241,
    1243,    22,  1245,   198,  1247,   241,  1249,  1435,  1251,   241,
      93,    94,   198,   198,  1854,    98,    99,   100,   101,   217,
     218,  1132,   240,   242,   222,   244,   245,   225,  1748,  1746,
    1747,   223,   147,   167,   168,   169,   151,   198,  1345,   198,
     199,   200,   240,   242,  1154,   223,   243,  1158,   246,   202,
     203,   204,   205,   223,  1441,   223,  1166,   223,     5,     6,
     202,   203,   204,   242,  1492,   202,   203,   204,   223,  1789,
    1790,  1788,   223,  1183,   209,   210,   211,   242,    25,   688,
     689,   690,   242,   223,    31,    10,    11,    12,    33,  1517,
     223,  1426,   223,   242,   242,   242,   242,   242,   242,   223,
     223,   242,  1437,   218,  1824,    43,   245,   222,   240,   224,
     225,   198,   242,   224,   243,    60,    61,   466,   219,  1426,
    1455,    68,    69,  1447,   241,   240,   475,  1675,   198,   189,
    1437,   246,   223,    10,  1748,    37,   485,    66,     8,   242,
     209,   223,   223,   223,  1255,    33,   495,    13,  1455,   216,
    1537,   241,   216,  1748,   242,   216,   198,   243,   198,   155,
     198,   224,   216,   242,  1274,  1275,   216,    43,    14,   518,
    1280,   198,    60,    61,   217,  1789,  1790,   189,   125,   126,
     219,   198,    67,   245,   198,  1572,   243,   216,   216,   209,
     243,  1578,   243,   242,  1789,  1790,   242,  1745,   242,   223,
     549,   550,   147,     1,   223,  1540,   151,   242,   242,   242,
    1824,   560,   242,   224,   224,   224,   242,   242,   198,   166,
     242,   198,   217,   572,   573,   574,   575,   576,   577,  1824,
     243,  1538,   217,  1540,   198,  1345,   183,   184,   185,   243,
     243,   243,   242,   241,   243,    12,   243,   198,   242,   241,
     241,   198,   242,   242,    21,    22,   243,   198,   198,   147,
     223,   208,   150,   151,   198,   614,   242,   242,   242,   242,
    1698,   198,   160,   218,   198,    43,   223,   222,    33,   224,
     225,  1668,   243,   198,   131,   132,   133,   134,   135,   136,
     137,   138,   242,   240,   643,   240,   242,   241,   206,   198,
     188,   246,   198,   198,   243,   241,   198,   242,  1736,   241,
     224,   242,   242,   160,   242,   202,  1426,  1428,    70,  1654,
     243,   242,   210,   170,   171,   172,   242,  1437,   210,   217,
     218,   242,  1575,   242,   222,   684,  1447,   225,   224,   243,
     224,   242,   242,    53,   242,  1455,   695,  1654,   242,   209,
     699,   241,   240,   702,   243,   704,  1784,  1744,   246,   209,
     709,   209,   243,   712,   243,   243,   241,   716,  1675,   243,
     243,   720,   139,   140,   141,   142,   143,   216,   243,   146,
     147,   148,   149,   241,   151,   152,   153,   154,   243,   243,
     243,   209,   159,   242,   161,   162,   241,   241,   804,   748,
     167,   168,   169,   243,    86,   243,   173,  1794,     1,   150,
      89,  1669,   234,  1670,   752,  1670,  1206,  1670,  1670,  1670,
       1,  1209,  1668,   772,   773,  1354,  1535,   776,  1538,   778,
    1540,  1458,  1589,  1461,  1258,  1590,  1590,    58,  1745,   788,
     789,   790,   791,   792,   793,   519,   795,   531,   797,  1764,
    1315,   218,    -1,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   531,  1620,   337,   797,    -1,    -1,  1580,
      -1,    -1,   239,   240,    -1,  1586,    -1,    -1,    -1,    -1,
      -1,    -1,   831,   832,    -1,    -1,   835,   836,   837,   838,
      -1,   840,    -1,   842,   843,   844,   845,   846,   847,   848,
     849,   850,   851,   852,   853,   854,   855,   856,   857,   858,
     859,   860,   861,    -1,   863,    -1,    -1,   866,    -1,    -1,
     869,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1654,    -1,    -1,    -1,    -1,  1792,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1675,    -1,    -1,    -1,    -1,
     919,    -1,    -1,    -1,    -1,   924,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1826,    -1,  1828,   935,   962,   963,   964,
     965,   966,   967,   968,   969,   970,   971,    -1,   973,   974,
     975,   976,   977,   978,    -1,    -1,    -1,  1850,    -1,    -1,
    1720,    -1,   961,   962,   963,   964,   965,   966,   967,   968,
     969,   970,   971,   972,   973,   974,   975,   976,   977,   978,
      -1,    -1,    -1,    -1,    -1,  1745,    -1,    -1,  1749,    -1,
      -1,    -1,   991,    -1,   993,    21,    22,    -1,   139,   140,
     141,   142,   143,    -1,    -1,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,    -1,    -1,    -1,  1777,   159,    -1,
     161,   162,    -1,    -1,  1023,  1024,   167,   168,   169,    -1,
      -1,  1792,   173,    -1,    -1,    -1,    -1,    -1,  1037,    -1,
    1039,    -1,    -1,    -1,    -1,    -1,  1045,    -1,    -1,    -1,
    1049,    -1,    -1,    -1,    -1,  1054,    -1,  1056,    -1,  1058,
      -1,  1060,    -1,    -1,    -1,  1826,    -1,  1828,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,  1850,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,   239,   240,
      -1,    -1,    -1,   244,   245,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,    -1,    10,
    1119,    -1,    -1,   149,  1123,   151,   152,   153,   154,    -1,
      21,    22,    -1,   159,  1133,  1134,  1135,  1136,  1137,  1138,
    1139,  1140,  1141,  1142,  1143,  1144,  1145,  1146,  1147,  1148,
    1149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1174,    -1,    -1,    -1,    -1,
      -1,  1180,    -1,    -1,    -1,    -1,  1185,    -1,    -1,    -1,
      -1,  1190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   225,
     226,   227,   228,   229,    -1,    -1,  1205,    -1,   139,   140,
     141,   142,   143,   239,   240,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
     161,   162,    -1,    -1,    -1,    -1,   167,   168,   169,    -1,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,   170,
     171,   172,   173,    21,    22,    -1,    -1,   218,    -1,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1306,   239,   240,
      -1,    -1,    -1,    -1,    -1,  1314,  1315,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   217,   218,    -1,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,    -1,
      -1,    -1,    -1,    -1,  1343,  1344,    -1,    -1,   239,   240,
      33,    -1,  1351,   244,   245,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1364,    -1,  1366,    -1,  1368,
      -1,    -1,    -1,    -1,    -1,  1374,    -1,    60,    61,  1378,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,    60,    61,    -1,
      -1,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,   170,   171,   172,   173,    -1,    -1,    -1,  1438,
      -1,    -1,    -1,    -1,    -1,  1444,    -1,    -1,  1447,    -1,
     188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   147,    -1,    -1,    -1,   151,    -1,
      -1,    -1,   210,    -1,    -1,    -1,    -1,    -1,    -1,   217,
     218,  1480,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   239,   240,    -1,   147,   188,   244,   245,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1519,  1520,  1521,    -1,    -1,    -1,    -1,   210,    -1,    21,
      22,    -1,    -1,   216,    -1,   218,    -1,    -1,    -1,   222,
      -1,   224,   225,    -1,    -1,    -1,    -1,  1546,    -1,  1548,
      -1,    -1,    -1,    -1,    -1,  1554,    -1,   240,    -1,    -1,
      -1,    -1,    -1,   246,    -1,    -1,    21,    22,    -1,    -1,
    1569,    -1,  1571,    -1,    -1,   218,    -1,    -1,    -1,   222,
      -1,   224,   225,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1595,   240,    -1,  1598,
      -1,    -1,    -1,   246,    -1,    -1,    -1,  1606,  1607,  1608,
      -1,    -1,    -1,    -1,  1613,    -1,    -1,  1616,    -1,    -1,
    1619,    -1,    -1,  1622,  1623,    -1,    -1,    -1,  1627,  1628,
      -1,  1630,  1631,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1645,   139,   140,   141,
     142,   143,    -1,    -1,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,
     162,  1670,    -1,    -1,    -1,   167,   168,   169,    -1,    -1,
      -1,   173,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,   146,   147,   148,   149,  1694,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,   159,    -1,   161,   162,    -1,    -1,
      -1,    -1,   167,   168,   169,    -1,    -1,  1716,   173,    -1,
      -1,    -1,    -1,    -1,    -1,  1724,   218,    -1,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,  1737,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,   240,  1748,
      -1,   243,    -1,    -1,    -1,  1754,  1755,    -1,    -1,    -1,
      -1,    -1,    -1,   218,    -1,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,  1774,  1775,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   239,   240,  1785,    -1,   243,    -1,
    1789,  1790,     1,    -1,    -1,    -1,     5,     6,     7,    -1,
       9,    10,    11,    -1,    13,    -1,    15,    16,    17,    18,
      19,    -1,    -1,  1812,    -1,    -1,    25,    26,    27,    28,
      29,    -1,    31,    -1,    -1,  1824,    -1,    -1,    -1,    38,
      39,    40,    -1,    42,    -1,    44,    45,    -1,    -1,    48,
      -1,    50,    51,    52,    -1,    54,    55,    -1,    -1,    58,
      59,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,   157,   158,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   174,   175,   176,   177,   178,
      33,   180,    -1,   182,   183,   184,   185,    -1,   187,   188,
     189,   190,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    60,    61,   208,
     209,   210,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   225,   226,   227,    -1,
     229,    -1,    -1,   232,   233,    -1,    -1,     5,     6,    -1,
      -1,   240,    -1,   242,    -1,   244,   245,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    60,    61,   147,    -1,    -1,    65,   151,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
      -1,    -1,    -1,    -1,    -1,   218,    -1,    -1,    -1,   222,
      -1,   224,   225,   141,   142,    -1,    -1,    -1,    -1,   147,
      -1,    -1,    -1,   151,    -1,    -1,    -1,   240,    -1,    -1,
      -1,    -1,    -1,   246,    -1,    -1,    -1,    -1,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,   175,   176,   177,
     178,    -1,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,   189,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
     208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     218,    -1,    -1,    -1,   222,    -1,    -1,   225,   226,   227,
      -1,   229,    -1,    -1,   232,   233,    -1,    -1,    -1,     5,
       6,    -1,   240,    -1,   242,    -1,   244,   245,   246,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    60,    61,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,    -1,    -1,
      -1,   147,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,   175,
     176,   177,   178,    -1,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,   189,   190,    -1,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,
      -1,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   218,    -1,    -1,    -1,   222,    -1,    -1,   225,
     226,   227,    -1,   229,    -1,    -1,   232,   233,    -1,    -1,
      -1,     5,     6,    -1,   240,    -1,   242,    -1,   244,   245,
     246,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,   175,   176,   177,   178,    -1,   180,    -1,   182,   183,
     184,   185,    -1,    -1,    -1,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
      -1,    -1,    -1,    -1,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   222,    -1,
      -1,   225,   226,   227,    -1,   229,    -1,    -1,   232,   233,
      -1,    -1,    -1,     5,     6,    -1,   240,    -1,   242,    -1,
     244,   245,   246,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,   156,   157,   158,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,   175,   176,   177,   178,    33,   180,    -1,
     182,   183,   184,   185,    -1,    -1,    -1,   189,   190,    -1,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
      -1,    -1,    -1,    -1,    60,    61,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   225,   226,   227,    -1,   229,    -1,    -1,
     232,   233,    -1,    -1,     5,     6,    -1,    -1,   240,    -1,
     242,    -1,   244,   245,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,   147,    -1,    -1,    65,   151,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,    -1,    -1,    -1,
      -1,    -1,   218,    -1,    -1,    -1,   222,    -1,   224,   225,
     141,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,   240,   156,   157,   158,    -1,    -1,
     246,    -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   174,   175,   176,   177,   178,    33,   180,
      -1,   182,   183,   184,   185,    -1,    -1,    -1,   189,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    60,    61,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   225,   226,   227,    -1,   229,    -1,
      -1,   232,   233,    -1,    -1,     5,     6,    -1,    -1,   240,
      -1,   242,    -1,   244,   245,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    52,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,   147,    -1,    -1,    65,   151,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,   218,    -1,    -1,    -1,   222,    -1,   224,
     225,   141,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   240,    -1,    -1,    -1,    -1,
      -1,   246,    -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,   175,   176,   177,   178,    33,
     180,    -1,   182,   183,   184,   185,    -1,    -1,    -1,   189,
     190,    -1,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,    -1,    -1,    -1,    -1,    60,    61,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   225,   226,   227,    -1,   229,
      -1,    -1,   232,   233,    -1,    -1,     5,     6,    -1,    -1,
     240,    -1,   242,    -1,   244,   245,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,   147,    -1,    -1,    65,   151,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,    -1,
      -1,    -1,    -1,    -1,   218,    -1,    -1,    -1,   222,    -1,
     224,   225,   141,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   240,   156,   157,   158,
      -1,    -1,   246,    -1,    -1,    -1,    -1,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   174,   175,   176,   177,   178,
      33,   180,    -1,   182,   183,   184,   185,    -1,    -1,    -1,
     189,   190,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    60,    61,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   225,   226,   227,    -1,
     229,    -1,    -1,   232,   233,    -1,    -1,     5,     6,    -1,
      -1,   240,    -1,   242,    -1,   244,   245,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,   147,    -1,    -1,    65,   151,    -1,
      68,    69,    70,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
      -1,    -1,    -1,    -1,    -1,   218,    -1,    -1,    -1,   222,
      -1,   224,   225,   141,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   240,    -1,    -1,
      -1,    -1,    -1,   246,    -1,    -1,    -1,    -1,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,   175,   176,   177,
     178,    33,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,   189,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    60,    61,
     208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   225,   226,   227,
      -1,   229,    -1,    -1,   232,   233,    -1,    -1,     5,     6,
      -1,    -1,   240,    -1,   242,   243,   244,   245,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    33,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      60,    61,    -1,    -1,    -1,   147,    -1,    -1,    65,   151,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,    -1,    -1,    -1,    -1,    -1,   218,    -1,    -1,    -1,
     222,    -1,   224,   225,   141,   142,    -1,   147,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,   240,   156,
     157,   158,    -1,    -1,   246,    -1,    -1,    -1,    -1,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,   175,   176,
     177,   178,    33,   180,    -1,   182,   183,   184,   185,    -1,
      -1,    -1,   189,   190,    -1,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,    60,
      61,   208,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,
      -1,    -1,   222,    -1,   224,   225,    -1,    -1,   225,   226,
     227,    -1,   229,    -1,    -1,   232,   233,    -1,    -1,    -1,
     240,     5,     6,   240,    -1,   242,   246,   244,   245,    13,
      -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    33,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,    -1,
      -1,    55,    -1,    60,    61,    -1,   147,    -1,    -1,    -1,
     151,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,    -1,    -1,    -1,   218,    -1,    -1,
      -1,   222,    -1,   224,   225,    -1,    -1,   141,   142,    -1,
     147,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,   240,
      -1,    -1,    -1,    -1,    -1,   246,    -1,    -1,    -1,    -1,
      -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,   175,   176,   177,   178,    -1,   180,    -1,   182,   183,
     184,   185,    -1,    -1,    -1,   189,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
      -1,    -1,    -1,    -1,   208,    -1,    -1,    -1,    -1,    -1,
      -1,   218,    -1,    -1,    -1,   222,    -1,   224,   225,    -1,
      -1,   225,   226,   227,    -1,   229,    -1,    -1,   232,   233,
      -1,    -1,    -1,   240,     5,     6,   240,   241,   242,   246,
     244,   245,    13,    -1,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   174,   175,   176,   177,   178,    -1,   180,
     181,   182,   183,   184,   185,    -1,    -1,    -1,   189,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   225,   226,   227,    -1,   229,    -1,
      -1,   232,   233,    -1,    -1,    -1,    -1,     5,     6,   240,
      -1,   242,    -1,   244,   245,    13,    -1,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,   175,   176,   177,
     178,    -1,   180,    -1,   182,   183,   184,   185,    -1,    -1,
      -1,   189,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
     208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   225,   226,   227,
      -1,   229,    -1,    -1,   232,   233,    -1,    -1,    -1,    -1,
       5,     6,   240,   241,   242,    -1,   244,   245,    13,    -1,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,
     175,   176,   177,   178,    -1,   180,    -1,   182,   183,   184,
     185,    -1,    -1,    -1,   189,   190,    -1,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,    -1,    -1,    -1,
      -1,    -1,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     225,   226,   227,    -1,   229,    -1,    -1,   232,   233,    -1,
      -1,     5,     6,    -1,    -1,   240,    -1,   242,    -1,   244,
     245,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    70,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,   175,   176,   177,   178,    -1,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,   189,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
      -1,    -1,    -1,    -1,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   225,   226,   227,    -1,   229,    -1,    -1,   232,   233,
      -1,    -1,     5,     6,    -1,    -1,   240,    -1,   242,    -1,
     244,   245,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   174,   175,   176,   177,   178,    -1,   180,   181,   182,
     183,   184,   185,    -1,    -1,    -1,   189,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   225,   226,   227,    -1,   229,    -1,    -1,   232,
     233,    -1,    -1,     5,     6,    -1,    -1,   240,    -1,   242,
     243,   244,   245,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,   175,   176,   177,   178,    -1,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,   189,   190,    -1,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
      -1,    -1,    -1,    -1,    -1,    -1,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   225,   226,   227,    -1,   229,    -1,    -1,
     232,   233,    -1,    -1,     5,     6,    -1,    -1,   240,    -1,
     242,   243,   244,   245,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   174,   175,   176,   177,   178,    -1,   180,
      -1,   182,   183,   184,   185,    -1,    -1,    -1,   189,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   225,   226,   227,    -1,   229,    -1,
      -1,   232,   233,    -1,    -1,    -1,    -1,     5,     6,   240,
     241,   242,    -1,   244,   245,    13,    -1,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,   175,   176,   177,
     178,    -1,   180,    -1,   182,   183,   184,   185,    -1,    -1,
      -1,   189,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
     208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   225,   226,   227,
      -1,   229,    -1,    -1,   232,   233,    -1,    -1,    -1,    -1,
       5,     6,   240,    -1,   242,    -1,   244,   245,    13,    -1,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,
     175,   176,   177,   178,    -1,   180,    -1,   182,   183,   184,
     185,    -1,    -1,    -1,   189,   190,    -1,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,    -1,    -1,    -1,
      -1,    -1,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     225,   226,   227,    -1,   229,    -1,    -1,   232,   233,    -1,
      -1,     5,     6,    -1,    -1,   240,    -1,   242,    -1,   244,
     245,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,   175,   176,   177,   178,    -1,   180,    -1,   182,   183,
     184,   185,    -1,    -1,    -1,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
      -1,    -1,    -1,    -1,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   225,   226,   227,    -1,   229,    -1,    -1,   232,   233,
      -1,    -1,     5,     6,    -1,    -1,   240,    -1,   242,    -1,
     244,   245,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   174,   175,   176,   177,   178,    -1,   180,   181,   182,
     183,   184,   185,    -1,    -1,    -1,   189,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   225,   226,   227,    -1,   229,    -1,    -1,   232,
     233,    -1,    -1,     5,     6,    -1,    -1,   240,    -1,   242,
      -1,   244,   245,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,   175,   176,   177,   178,    -1,   180,    -1,
     182,   183,   184,   185,    -1,    -1,    -1,   189,   190,    -1,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
      -1,    -1,    -1,    -1,    -1,    -1,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   225,   226,   227,    -1,   229,    -1,    -1,
     232,   233,    -1,    -1,     5,     6,    -1,    -1,   240,    -1,
     242,    -1,   244,   245,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    -1,    -1,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   174,   175,   176,   177,   178,    -1,   180,
      -1,   182,   183,   184,   185,    -1,    -1,    -1,   189,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   225,   226,   227,    -1,   229,    -1,
      -1,   232,   233,    -1,    -1,     5,     6,    -1,    -1,   240,
      -1,   242,    -1,   244,   245,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,   175,   176,   177,   178,    -1,
     180,    -1,   182,   183,   184,   185,    -1,    -1,    -1,   189,
     190,    -1,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,    -1,    -1,    -1,    -1,    -1,    -1,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   223,    -1,   225,   226,   227,    -1,   229,
      -1,    -1,   232,   233,    -1,    -1,     5,     6,    -1,    -1,
     240,    -1,   242,    -1,   244,   245,    15,    16,    17,    18,
      19,    -1,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   174,   175,   176,   177,   178,
      -1,   180,    -1,   182,   183,   184,   185,    -1,    -1,    -1,
     189,   190,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   225,   226,   227,    -1,
     229,    -1,    -1,   232,   233,    -1,    -1,     5,     6,    -1,
      -1,   240,    -1,   242,    -1,   244,   245,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,   175,   176,   177,
     178,    -1,   180,    -1,   182,   183,   184,   185,    -1,    -1,
      -1,   189,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
     208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   225,   226,   227,
      -1,   229,    -1,    -1,   232,   233,    -1,    -1,     5,     6,
      -1,    -1,   240,    -1,   242,   243,   244,   245,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,   175,   176,
     177,   178,    -1,   180,    -1,   182,   183,   184,   185,    -1,
      -1,    -1,   189,   190,    -1,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,
      -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   223,    -1,   225,   226,
     227,    -1,   229,    -1,    -1,   232,   233,    -1,    -1,     5,
       6,    -1,    -1,   240,    -1,   242,    -1,   244,   245,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,   175,
     176,   177,   178,    -1,   180,    -1,   182,   183,   184,   185,
      -1,    -1,    -1,   189,   190,    -1,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,
      -1,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   225,
     226,   227,    -1,   229,    -1,    -1,   232,   233,    -1,    -1,
       5,     6,    -1,    -1,   240,    -1,   242,   243,   244,   245,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,
     175,   176,   177,   178,    -1,   180,    -1,   182,   183,   184,
     185,    -1,    -1,    -1,   189,   190,    -1,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,    -1,    -1,    -1,
      -1,    -1,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     225,   226,   227,    -1,   229,    -1,    -1,   232,   233,    -1,
      -1,     5,     6,    -1,    -1,   240,    -1,   242,   243,   244,
     245,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,   175,   176,   177,   178,    -1,   180,    -1,   182,   183,
     184,   185,    -1,    -1,    -1,   189,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
      -1,    -1,    -1,    -1,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   225,   226,   227,    -1,   229,    -1,    -1,   232,   233,
      -1,    -1,     5,     6,    -1,    -1,   240,    -1,   242,   243,
     244,   245,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   174,   175,   176,   177,   178,    -1,   180,    -1,   182,
     183,   184,   185,    -1,    -1,    -1,   189,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   225,   226,   227,    -1,   229,    -1,    -1,   232,
     233,    -1,    -1,     5,     6,    -1,    -1,   240,    -1,   242,
     243,   244,   245,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,   175,   176,   177,   178,    -1,   180,    -1,
     182,   183,   184,   185,    -1,    -1,    -1,   189,   190,    -1,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
      -1,    -1,    -1,    -1,    -1,    -1,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   225,   226,   227,    -1,   229,    -1,    -1,
     232,   233,    -1,    -1,     5,     6,    -1,    -1,   240,    -1,
     242,   243,   244,   245,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   174,   175,   176,   177,   178,    -1,   180,
      -1,   182,   183,   184,   185,    -1,    -1,    -1,   189,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   225,   226,   227,    -1,   229,    -1,
      -1,   232,   233,    -1,    -1,     5,     6,    -1,    -1,   240,
      -1,   242,    -1,   244,   245,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,   175,   176,   177,   178,    -1,
     180,    -1,   182,   183,   184,   185,    -1,    -1,    -1,   189,
     190,    -1,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,    -1,    -1,    -1,    -1,    -1,    -1,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   225,   226,   227,    -1,   229,
      -1,    -1,   232,   233,    -1,    -1,     5,     6,    -1,    -1,
     240,    -1,   242,    -1,   244,   245,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   174,   175,   176,   177,   178,
      -1,   180,    -1,   182,   183,   184,   185,    -1,    -1,    -1,
     189,   190,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   225,   226,   227,    -1,
     229,    -1,    -1,   232,   233,    -1,    -1,     5,     6,    -1,
      -1,   240,    -1,   242,    -1,   244,   245,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   174,   175,   176,   177,
     178,    -1,   180,    -1,   182,   183,   184,   185,    -1,    -1,
      -1,   189,   190,    -1,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
     208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   225,   226,   227,
      -1,   229,    -1,    -1,   232,   233,    -1,    -1,     5,     6,
      -1,    -1,   240,    -1,   242,    -1,   244,   245,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,   175,   176,
     177,   178,    -1,   180,    -1,   182,   183,   184,   185,    -1,
      -1,    -1,   189,   190,    -1,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,    -1,    13,    -1,    -1,    -1,
      -1,   208,    19,    -1,    -1,    -1,    21,    22,    25,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,   225,   226,
     227,    -1,   229,    -1,    41,   232,   233,    -1,    -1,    -1,
      -1,    -1,    49,   240,    -1,   242,    -1,   244,   245,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,   159,    -1,   161,   162,    -1,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      13,    21,    22,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,   198,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,   222,   223,   224,
     225,   226,   227,   228,   229,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,   239,   240,    -1,   244,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,
      -1,   161,   162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,   198,    25,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    41,   223,   224,   225,   226,   227,   228,   229,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,
     240,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,   244,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,    -1,    -1,    -1,
      -1,    -1,    -1,   149,    -1,   151,   152,   153,   154,    -1,
      -1,    -1,    -1,   159,    -1,   161,   162,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,   198,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,   225,
     226,   227,   228,   229,    -1,    -1,    49,    -1,    -1,    -1,
      -1,    -1,    -1,   239,   240,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,   244,    -1,   246,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,   198,    25,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,   210,    -1,    -1,
      -1,    -1,    41,   216,    -1,    -1,    -1,    -1,    -1,    -1,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,   244,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    21,    22,   198,
      25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    64,
      -1,    -1,    -1,    -1,    -1,   244,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,    -1,    -1,    -1,    -1,    -1,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,    -1,    -1,    -1,   159,   160,   161,   162,   163,
     164,   166,    -1,   167,   168,   169,   170,   171,   172,   173,
      21,    22,    -1,    -1,   179,   139,   140,   141,   142,   143,
      -1,    -1,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,    -1,    -1,   198,    -1,   159,    -1,   161,   162,    -1,
      -1,    -1,    -1,   167,   168,   169,    21,    22,    -1,   173,
      -1,    -1,    -1,   217,   218,    -1,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   239,   240,    -1,    -1,   244,
     244,   245,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   218,    -1,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   239,   240,    -1,    -1,   243,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
     161,   162,    21,    22,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,   173,    -1,   139,   140,   141,   142,   143,    -1,
      -1,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,   159,    -1,   161,   162,    21,    22,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,   173,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,   240,
      -1,    -1,   243,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   218,    -1,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   239,   240,    -1,    -1,   243,    -1,
     139,   140,   141,   142,   143,    -1,    -1,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
     159,    -1,   161,   162,    21,    22,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,   173,    -1,   139,   140,   141,   142,
     143,    -1,    -1,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,
      21,    22,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,
      -1,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     239,   240,    -1,    -1,   243,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   218,    -1,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   239,   240,    -1,    -1,
     243,    -1,   139,   140,   141,   142,   143,    -1,    -1,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,   159,    -1,   161,   162,    21,    22,    -1,    -1,
     167,   168,   169,    -1,    -1,    -1,   173,    -1,   139,   140,
     141,   142,   143,    -1,    -1,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
     161,   162,    21,    22,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   218,    -1,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   239,   240,    -1,    -1,   243,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,   240,
      -1,    -1,   243,    -1,   139,   140,   141,   142,   143,    -1,
      -1,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,   159,    -1,   161,   162,    21,    22,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,   173,    -1,
     139,   140,   141,   142,   143,    -1,    -1,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
     159,    -1,   161,   162,    21,    22,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   218,    -1,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   239,   240,    -1,    -1,   243,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,
      -1,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     239,   240,    -1,    -1,   243,    -1,   139,   140,   141,   142,
     143,    -1,    -1,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,
      21,    22,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
     173,    -1,   139,   140,   141,   142,   143,    -1,    -1,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,   159,    -1,   161,   162,    21,    22,    -1,    -1,
     167,   168,   169,    -1,    -1,    -1,   173,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   218,    -1,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   239,   240,    -1,    -1,
     243,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   218,    -1,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   239,   240,    -1,    -1,   243,    -1,   139,   140,
     141,   142,   143,    -1,    -1,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
     161,   162,    21,    22,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,   173,    -1,   139,   140,   141,   142,   143,    -1,
      -1,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,   159,    -1,   161,   162,    21,    22,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,   173,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,   240,
      -1,    -1,   243,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   218,    -1,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   239,   240,    -1,    -1,   243,    -1,
     139,   140,   141,   142,   143,    -1,    -1,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
     159,    -1,   161,   162,    21,    22,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,   173,    -1,   139,   140,   141,   142,
     143,    -1,    -1,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,
      21,    22,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,
      -1,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     239,   240,    -1,    -1,   243,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   218,    -1,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   239,   240,    -1,    -1,
     243,    -1,   139,   140,   141,   142,   143,    -1,    -1,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,   159,    -1,   161,   162,    21,    22,    -1,    -1,
     167,   168,   169,    -1,    -1,    -1,   173,    -1,   139,   140,
     141,   142,   143,    -1,    -1,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
     161,   162,    21,    22,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   218,    -1,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   239,   240,    -1,    -1,   243,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,   240,
      -1,    -1,   243,    -1,   139,   140,   141,   142,   143,    -1,
      -1,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,   159,    -1,   161,   162,    21,    22,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,   173,    -1,
     139,   140,   141,   142,   143,    -1,    -1,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
     159,    -1,   161,   162,    21,    22,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   218,    -1,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   239,   240,    -1,    -1,   243,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,
      -1,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     239,   240,    -1,    -1,   243,    -1,   139,   140,   141,   142,
     143,    -1,    -1,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,
      21,    22,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
     173,    -1,   139,   140,   141,   142,   143,    -1,    -1,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,   159,    -1,   161,   162,    21,    22,    -1,    -1,
     167,   168,   169,    -1,    -1,    -1,   173,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   218,    -1,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   239,   240,    -1,    -1,
     243,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   218,    -1,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   239,   240,    -1,    -1,   243,    -1,   139,   140,
     141,   142,   143,    -1,    -1,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
     161,   162,    21,    22,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,   173,    -1,   139,   140,   141,   142,   143,    -1,
      -1,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,   159,    -1,   161,   162,    21,    22,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,   173,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,   240,
      -1,    -1,   243,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   218,    -1,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   239,   240,    -1,    -1,   243,    -1,
     139,   140,   141,   142,   143,    -1,    -1,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
     159,    -1,   161,   162,    21,    22,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,   173,    -1,   139,   140,   141,   142,
     143,    -1,    -1,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,
      21,    22,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,
      -1,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     239,   240,    -1,    -1,   243,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   218,    -1,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   239,   240,    -1,    -1,
     243,    -1,   139,   140,   141,   142,   143,    -1,    -1,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,   159,    -1,   161,   162,    21,    22,    -1,    -1,
     167,   168,   169,    -1,    -1,    -1,   173,    -1,   139,   140,
     141,   142,   143,    -1,    -1,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
     161,   162,    21,    22,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   218,    -1,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   239,   240,    -1,    -1,   243,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,   240,
      -1,    -1,   243,    -1,   139,   140,   141,   142,   143,    -1,
      -1,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,   159,    -1,   161,   162,    21,    22,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,   173,    -1,
     139,   140,   141,   142,   143,    -1,    -1,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
     159,    -1,   161,   162,    21,    22,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   218,    -1,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   239,   240,    -1,    -1,   243,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,
      -1,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     239,   240,    -1,    -1,   243,    -1,   139,   140,   141,   142,
     143,    -1,    -1,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,
      21,    22,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
     173,    -1,   139,   140,   141,   142,   143,    -1,    -1,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,   159,    -1,   161,   162,    21,    22,    -1,    -1,
     167,   168,   169,    -1,    -1,    -1,   173,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   218,    -1,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   239,   240,    -1,    -1,
     243,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   218,    -1,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   239,   240,    -1,    -1,   243,    -1,   139,   140,
     141,   142,   143,    -1,    -1,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
     161,   162,    21,    22,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,   173,    -1,   139,   140,   141,   142,   143,    -1,
      -1,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,   159,    -1,   161,   162,    21,    22,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,   173,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,   240,
      -1,    -1,   243,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   218,    -1,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   239,   240,   241,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
     159,    -1,   161,   162,    21,    22,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,   173,    -1,   139,   140,   141,   142,
     143,    -1,    -1,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,
      21,    22,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,
      -1,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     239,   240,   241,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   218,    -1,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   239,   240,   241,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,   159,    -1,   161,   162,    21,    22,    -1,    -1,
     167,   168,   169,    -1,    -1,    -1,   173,    -1,   139,   140,
     141,   142,   143,    -1,    -1,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
     161,   162,    -1,    -1,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   218,    -1,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   239,   240,   241,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,   240,
     241,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
      21,    22,    -1,    -1,   159,    -1,   161,   162,    -1,    -1,
      -1,    -1,   167,   168,   169,    -1,    -1,    38,   173,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   218,    -1,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   239,   240,   241,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,    -1,    -1,
     151,   152,   153,    -1,    -1,    -1,    -1,    -1,    -1,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,   170,
     171,   172,   173,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,   159,    -1,   161,   162,    -1,    -1,    -1,    -1,
     167,   168,   169,    -1,    -1,    -1,   173,   218,    -1,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   186,
      -1,   232,   233,    -1,    -1,    -1,    -1,    -1,   239,   240,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   218,    -1,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,    -1,    -1,    21,    22,    -1,    -1,    -1,
      -1,    -1,   239,   240,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,    -1,    -1,    -1,
      -1,   159,    -1,   161,   162,    21,    22,    -1,    -1,   167,
     168,   169,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   210,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     218,    -1,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   239,   240,   139,   140,   141,   142,   143,    -1,    -1,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,    -1,
      21,    22,    -1,   159,    -1,   161,   162,    -1,    -1,   165,
      -1,   167,   168,   169,    -1,    -1,    -1,   173,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,    21,
      22,    -1,    -1,   159,    -1,   161,   162,    -1,    -1,   165,
      -1,   167,   168,   169,    -1,    -1,    -1,   173,    -1,    -1,
      -1,    -1,   218,    -1,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   239,   240,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   218,    -1,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,   239,   240,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,    21,    22,    -1,    -1,   159,    -1,
     161,   162,    -1,    -1,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,   173,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,    21,    22,    -1,    -1,   159,    -1,   161,
     162,    -1,    -1,    -1,    -1,   167,   168,   169,    -1,    -1,
      -1,   173,    -1,    -1,    -1,    -1,    -1,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,   240,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,   239,   240,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,    21,    22,
      -1,    -1,   159,    -1,   161,   162,    -1,    -1,    -1,    -1,
     167,   168,   169,    -1,    -1,    -1,   173,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,    21,    22,    -1,
      -1,   159,    -1,   161,   162,    -1,    -1,    -1,    -1,   167,
     168,   169,    -1,   210,    -1,   173,    -1,    -1,    -1,    -1,
      -1,   218,    -1,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   239,   240,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
     218,    -1,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,   239,   240,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,    21,    22,    -1,    -1,   159,    -1,   161,   162,
      -1,    -1,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,    -1,
      -1,    -1,    -1,   167,    -1,   169,    -1,    -1,    21,    22,
      -1,    -1,    -1,    -1,    -1,   218,    -1,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,   239,   240,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,    -1,    -1,    -1,
      -1,   159,    -1,   161,   162,    -1,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,   239,   240,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
     159,    -1,   161,   162,    -1,    -1,    -1,    -1,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,   239,   240,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,
      -1,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     239,   240,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   221,   222,
     223,   224,   225,   226,   227,   228,   229,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   239,   240,    71,    72,
      73,    19,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,    -1,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   198,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     198,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,    -1,
      -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,   153,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    -1,    73,    -1,
      75,    76,    77,    78,    79,   181,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,   198,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   239,   240,    -1,   141,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   198
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   248,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   125,   126,   188,   210,   229,   240,   249,   253,   262,
     264,   265,   270,   277,   302,   307,   343,   425,   432,   436,
     447,   494,   499,   504,    19,    20,   198,   294,   295,   296,
     189,   271,   272,   173,   198,   199,   200,   229,   239,   266,
     267,   268,   186,   208,   312,   433,   198,   244,   251,    57,
      63,   428,   428,   428,   166,   198,   329,    34,    63,   130,
     159,   233,   242,   298,   299,   300,   301,   329,   277,     5,
       6,     8,    36,   444,    62,   423,   217,   216,   219,   216,
     228,   228,   267,   228,    22,    57,   228,   239,   269,   428,
     429,   431,   429,   423,   505,   495,   500,   198,   166,   263,
     300,   300,   300,   242,   167,   168,   169,   216,   241,   130,
     130,   130,   306,    57,    63,   434,    57,    63,   445,    57,
      63,   424,    15,    16,   189,   190,   193,   195,   198,   201,
     225,   242,   245,   255,   295,   189,   272,   267,   267,   267,
     198,   266,   266,   198,   277,   187,   209,   313,   429,   277,
      57,    63,   250,   198,   198,   198,   198,   202,   261,   243,
     296,   300,   300,   300,   300,    57,    63,   308,   310,   198,
     435,   448,   312,   426,   202,   203,   204,   254,   189,   190,
     191,   195,    15,    16,   189,   190,   193,   195,   198,   225,
     245,   255,   292,   293,   245,   269,   430,   277,   233,   252,
     506,   496,   501,   202,   243,   311,   219,   312,   129,   442,
     443,   421,   183,   245,   297,   393,   202,   203,   204,   189,
     190,   191,   195,   245,   216,   243,   198,   217,    66,   219,
     457,   312,   312,    35,    71,    73,    75,    76,    77,    78,
      79,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    93,    94,    95,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   141,   142,   198,   305,   309,    75,    79,    93,
      94,    98,    99,   100,   101,   452,   437,   198,   449,   188,
     313,   422,   296,   295,   245,   277,   175,   198,   419,   420,
     198,   292,    19,    25,    31,    41,    49,    64,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   179,   244,   329,   451,   453,   454,
     458,   464,   493,    79,    94,    99,   101,   312,   497,   502,
      21,    22,    38,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   151,   152,   153,   160,   161,   162,   163,   164,   167,
     168,   169,   170,   171,   172,   173,   218,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   232,   233,   239,
     240,    35,    35,   242,   303,   312,   314,   312,   427,   219,
     441,   312,   446,   393,   241,   295,   242,    43,   216,   219,
     222,   418,   223,   223,   223,   242,   223,   223,   242,   457,
     223,   223,   223,   223,   223,   242,   329,    33,    60,    61,
     147,   151,   218,   222,   225,   240,   246,   463,   220,   507,
     409,   412,   198,   198,   198,   241,    22,   198,   241,   178,
     243,   393,   404,   405,   406,   149,   219,   304,   317,   439,
     198,   277,   438,   329,   399,   420,   241,     5,     6,    15,
      16,    17,    18,    19,    25,    27,    31,    39,    45,    48,
      51,    55,    65,    68,    69,    80,   125,   126,   127,   141,
     142,   174,   175,   176,   177,   178,   180,   182,   183,   184,
     185,   189,   190,   192,   193,   194,   195,   196,   197,   199,
     200,   201,   208,   225,   226,   227,   232,   233,   240,   242,
     244,   245,   260,   262,   323,   329,   334,   348,   355,   358,
     361,   366,   369,   373,   374,   376,   381,   384,   385,   392,
     451,   509,   524,   535,   539,   552,   555,   198,   175,   198,
     464,   150,   160,   217,   417,   465,   471,   473,   385,   475,
     469,   198,   223,   477,   479,   481,   483,   485,   487,   489,
     491,   385,   223,   242,   320,    33,   222,    33,   222,   240,
     246,   241,   385,   240,   246,   464,   456,   198,   216,   277,
     407,   461,   493,   498,   198,   410,   461,   503,   198,   131,
     132,   133,   134,   135,   136,   137,   138,   160,   170,   171,
     172,   131,   132,   133,   134,   135,   136,   137,   138,   150,
     160,   170,   171,   172,   242,     7,    50,   342,   277,   216,
     277,   243,   493,   493,     1,     9,    10,    11,    13,    26,
      28,    29,    38,    40,    42,    44,    52,    54,    58,    59,
      65,   127,   128,   156,   157,   158,   199,   273,   274,   277,
     278,   281,   282,   284,   286,   287,   288,   289,   313,   315,
     316,   318,   323,   328,   330,   335,   336,   337,   338,   339,
     340,   341,   343,   347,   370,   372,   385,   427,   217,   277,
     313,    40,   240,   277,   302,   313,   401,   223,   223,   223,
     331,   453,   509,   242,   329,   223,     5,   125,   126,   223,
     242,   223,   242,   242,   223,   223,   242,   223,   242,   223,
     242,   223,   223,   242,   223,   223,   385,   385,   242,   242,
     242,   242,   242,   242,    13,   464,    13,   464,    13,   385,
     534,   550,   223,   223,   259,    13,   321,   534,   551,   191,
     385,   385,   385,   385,   385,    13,    49,   319,   359,   385,
     181,   198,   359,   510,   512,   245,   198,   242,   302,    21,
      22,   139,   140,   141,   142,   143,   146,   147,   148,   149,
     151,   152,   153,   154,   159,   161,   162,   167,   168,   169,
     173,   218,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   239,   240,   243,   242,   242,    43,   277,   417,
     328,   370,   385,   493,   493,   462,   493,   243,   493,   493,
     243,   224,   459,   493,   303,   493,   303,   493,   303,   407,
     408,   410,   411,   243,   467,   319,   241,   241,   385,   198,
     277,   508,   219,   461,   313,   219,   461,   313,   385,   175,
     198,   414,   415,   450,   406,   406,   406,   385,   285,   150,
     328,   359,   385,   314,    61,   385,   291,   385,   198,   277,
     189,    58,   385,   314,   223,   150,   328,   385,   245,   314,
     361,   365,   365,   365,   385,   277,   277,   385,    10,    37,
     361,   367,   277,   277,   277,   277,   277,    66,   344,   155,
     277,   131,   132,   133,   134,   135,   136,   137,   138,   144,
     145,   150,   160,   163,   164,   170,   171,   172,   217,   367,
     440,   385,   400,   301,     8,   393,   398,   525,   527,   332,
     242,   329,   223,   242,   356,   223,   223,   223,   546,   359,
     464,   321,   385,   349,   351,   385,   353,   385,   548,   359,
     531,   536,   359,   529,   464,   385,   385,   385,   385,   385,
     385,   450,    53,   225,   240,   242,   385,   510,   513,   517,
     533,   538,   450,   242,   513,   538,   450,   165,   209,   210,
     211,   518,   324,   326,   204,   205,   254,   450,   209,   216,
     554,   450,    13,   241,   216,   554,   242,   150,   160,   217,
     413,   554,   216,   554,   243,   175,   180,   223,   329,   375,
      70,   240,   243,   359,   512,     4,   183,   364,    19,   181,
     198,   451,    19,   181,   198,   451,   385,   385,   385,   385,
     385,   385,   198,   385,   181,   198,   385,   385,   385,   451,
     385,   385,   385,   385,   385,   385,    22,   385,   385,   385,
     385,   385,   385,   385,   385,   385,   385,   385,   152,   153,
     181,   198,   239,   240,   382,   451,   385,   243,   359,   385,
     198,   328,   385,   131,   132,   133,   134,   135,   136,   137,
     138,   144,   145,   150,   163,   164,   170,   171,   172,   217,
     277,   224,   224,   224,   461,   224,   224,   198,   455,   224,
     304,   224,   304,   224,   304,   224,   461,   224,   461,   322,
     493,   216,   554,   241,   217,   277,   313,   493,   493,   243,
     242,    43,   216,   219,   222,   413,   314,   450,   328,   359,
     216,    14,   385,   198,   314,   217,   219,   189,   464,   328,
     385,   245,   302,   314,   314,   283,   312,   368,   183,   242,
     346,   420,   365,   156,   157,   158,   315,   371,   385,   371,
     385,   371,   385,   371,   385,   371,   385,   371,   385,   371,
     385,   371,   385,   371,   385,   371,   385,   371,   385,   385,
     371,   385,   371,   385,   371,   385,   371,   385,   371,   385,
     371,   385,   313,   277,   198,   241,    57,    63,   396,    67,
     397,   277,   464,   464,   493,    70,   359,   512,   523,   223,
     385,   198,   385,   493,   540,   542,   544,   464,   554,   224,
     461,   243,   243,   464,   464,   243,   464,   243,   464,   554,
     464,   408,   554,   411,   224,   243,   243,   243,   243,   243,
     243,    20,   365,   240,   385,   243,   165,   216,   210,   517,
     213,   214,   241,   521,   216,   210,   213,   241,   520,    20,
     243,   517,   209,   212,   519,    20,   385,   209,   534,   322,
     322,   385,    20,   534,    20,   450,   385,   385,   385,   385,
     243,   181,   198,   242,   242,   377,   379,   198,   243,   512,
     510,   216,   243,   243,   242,   150,   160,   198,   217,   222,
     362,   363,   303,   223,   242,   223,   242,   242,   242,   241,
      19,   181,   198,   451,   219,   181,   198,   385,   242,   242,
     181,   198,   385,     1,   242,   241,   243,   243,   277,   385,
     385,   385,   385,   385,   385,   385,   385,   385,   385,   385,
     385,   385,   385,   385,   385,   385,   466,   472,   474,   493,
     476,   470,   216,   224,   277,   478,   224,   482,   224,   486,
     224,   490,   407,   492,   410,   224,   461,   243,   385,   385,
     198,   175,   198,   493,   385,    20,   314,   217,   290,   224,
     364,    12,    23,    24,   275,   276,   385,   317,   302,   198,
     345,   345,   365,   365,   365,   217,   277,    47,   397,    46,
     129,   394,   224,   224,   224,   512,   243,   243,   243,   198,
     243,   210,   224,   243,   224,   464,   408,   411,   224,   243,
     242,   464,   224,   224,   224,   224,   243,   224,   224,   243,
     224,   364,   242,   359,   513,   517,   385,   510,   521,   241,
     385,   533,   241,   359,   513,   209,   212,   215,   522,   241,
     359,   224,   224,   219,   257,   359,   359,    20,   243,   242,
     160,   413,   385,   385,   464,   303,   243,   241,   240,   363,
     198,   198,   242,   198,   198,   216,   241,   304,   386,   385,
     388,   385,   243,   359,   385,   223,   242,   385,   242,   241,
     385,   240,   243,   359,   242,   241,   383,   243,   359,   198,
     460,   198,   480,   484,   488,   468,   493,   277,   243,   242,
      43,   413,   359,   493,   385,   364,   303,   314,   385,    12,
     279,   313,   364,   216,   241,   243,   493,    33,   395,   394,
     396,   526,   528,   333,   243,   224,   461,   198,   242,   357,
     224,   224,   224,   547,   321,   224,   350,   352,   354,   549,
     532,   537,   530,   242,   243,   359,   210,   517,   521,   210,
     517,   241,   210,   325,   327,   258,   206,   210,   210,   359,
     160,   413,   385,   385,   385,   243,   243,   224,   304,   243,
     510,   243,   198,   362,   241,   165,   314,   360,   464,   243,
     493,   243,   243,   243,   390,   385,   385,   243,   510,   243,
     385,   243,   320,   385,   198,   385,   314,   367,   304,   314,
     280,   277,   303,   198,   241,   219,   418,   277,   402,   395,
     414,   415,   416,   242,   242,   385,   198,   224,   385,   541,
     543,   545,   242,   243,   242,   385,   385,   385,   242,    70,
     523,   242,   242,   243,   385,   243,   385,   521,   385,   522,
     534,   385,   320,   256,   534,   385,   210,   385,   385,   243,
     378,   224,   241,   243,   150,   385,   224,   224,   493,   243,
     243,   241,   243,   243,   360,   276,    26,   128,   281,   335,
     336,   337,   339,   385,   304,   219,   418,   464,   417,   309,
     403,   523,   523,   243,   224,   242,   243,   242,   242,   242,
     319,   321,   359,   523,   523,   243,   210,   553,   553,   553,
     202,   553,   553,   385,   160,   413,   375,   380,   243,   385,
     387,   389,   224,   243,   150,   150,   385,   314,   464,   417,
     417,   328,   385,   277,   309,   242,   510,   514,   515,   516,
     516,   385,   385,   523,   523,   510,   511,   243,   243,   554,
     516,   511,    53,   241,   209,   209,   209,   241,   553,   385,
     385,   375,   391,   385,   417,   328,   385,   328,   385,   277,
     314,   510,   216,   554,   243,   243,   243,   243,   516,   516,
     243,   243,   243,   243,   385,   241,   241,   209,   241,   328,
     385,   277,   277,   243,   242,   243,   243,   277,   510,   243
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   247,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   248,   248,   249,   250,   250,
     250,   251,   251,   252,   252,   253,   254,   254,   254,   254,
     255,   255,   256,   256,   257,   258,   257,   259,   259,   259,
     260,   261,   261,   263,   262,   264,   265,   266,   266,   266,
     267,   267,   267,   267,   267,   267,   267,   268,   268,   269,
     269,   270,   271,   271,   272,   272,   273,   274,   274,   275,
     275,   276,   276,   276,   277,   277,   278,   278,   279,   280,
     279,   281,   281,   281,   281,   281,   282,   283,   282,   285,
     284,   286,   287,   288,   290,   289,   291,   289,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   292,   292,   292,
     292,   293,   293,   294,   294,   294,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   296,   296,   297,   297,   297,   298,   298,   298,   298,
     299,   299,   300,   300,   300,   300,   300,   300,   300,   301,
     301,   302,   302,   303,   303,   303,   304,   304,   304,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   306,   306,   307,   308,   308,   308,   309,   311,   310,
     312,   312,   313,   313,   314,   314,   315,   315,   315,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     317,   317,   317,   318,   319,   319,   320,   320,   321,   321,
     322,   322,   324,   325,   323,   326,   327,   323,   328,   328,
     328,   328,   328,   329,   329,   329,   330,   330,   332,   333,
     331,   331,   334,   334,   334,   334,   334,   334,   335,   336,
     337,   337,   337,   338,   338,   338,   339,   339,   340,   340,
     340,   341,   342,   342,   342,   343,   343,   344,   344,   345,
     345,   346,   346,   346,   346,   346,   346,   346,   346,   347,
     347,   349,   350,   348,   351,   352,   348,   353,   354,   348,
     356,   357,   355,   358,   358,   358,   358,   358,   358,   359,
     359,   360,   360,   360,   361,   361,   361,   362,   362,   362,
     362,   362,   363,   363,   364,   364,   364,   365,   365,   366,
     368,   367,   369,   369,   369,   369,   369,   369,   369,   369,
     369,   370,   370,   370,   370,   370,   370,   370,   370,   370,
     370,   370,   370,   370,   370,   370,   370,   370,   370,   370,
     371,   371,   371,   371,   372,   372,   372,   372,   372,   372,
     372,   372,   372,   372,   372,   372,   372,   372,   372,   372,
     372,   373,   373,   374,   374,   375,   375,   376,   377,   378,
     376,   379,   380,   376,   381,   381,   381,   381,   381,   381,
     381,   382,   383,   381,   384,   384,   384,   384,   384,   384,
     384,   385,   385,   385,   385,   385,   385,   385,   385,   385,
     385,   385,   385,   385,   385,   385,   385,   385,   385,   385,
     385,   385,   385,   385,   385,   385,   385,   385,   385,   385,
     385,   385,   385,   385,   385,   385,   385,   385,   385,   385,
     385,   385,   385,   385,   385,   385,   385,   385,   385,   385,
     385,   385,   385,   385,   385,   385,   385,   385,   386,   387,
     385,   385,   385,   385,   388,   389,   385,   385,   385,   390,
     391,   385,   385,   385,   385,   385,   385,   385,   385,   385,
     385,   385,   385,   385,   385,   385,   392,   392,   392,   392,
     392,   392,   392,   392,   392,   392,   392,   392,   392,   392,
     392,   392,   393,   393,   393,   394,   394,   394,   395,   395,
     396,   396,   396,   397,   397,   398,   399,   399,   400,   399,
     401,   399,   402,   399,   403,   399,   399,   404,   405,   405,
     406,   406,   406,   406,   406,   407,   407,   408,   408,   409,
     409,   409,   410,   411,   411,   412,   412,   412,   413,   413,
     414,   414,   414,   415,   415,   416,   416,   417,   417,   417,
     418,   418,   419,   419,   419,   419,   419,   419,   420,   420,
     420,   420,   420,   421,   421,   422,   421,   423,   423,   424,
     424,   424,   425,   426,   425,   427,   427,   427,   427,   428,
     428,   428,   430,   429,   431,   431,   432,   433,   432,   434,
     434,   434,   435,   437,   438,   436,   439,   440,   436,   441,
     441,   442,   442,   443,   444,   444,   444,   444,   445,   445,
     445,   446,   446,   448,   449,   447,   450,   450,   450,   450,
     450,   450,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   452,   452,   452,   452,   452,   452,   452,   452,
     453,   454,   454,   454,   455,   455,   455,   456,   456,   456,
     456,   457,   457,   457,   457,   457,   458,   459,   460,   458,
     461,   461,   462,   462,   463,   463,   464,   464,   464,   464,
     464,   464,   465,   466,   464,   464,   464,   467,   468,   464,
     464,   464,   464,   464,   464,   464,   464,   464,   464,   464,
     464,   464,   469,   470,   464,   464,   471,   472,   464,   473,
     474,   464,   475,   476,   464,   464,   477,   478,   464,   479,
     480,   464,   464,   481,   482,   464,   483,   484,   464,   464,
     485,   486,   464,   487,   488,   464,   489,   490,   464,   491,
     492,   464,   493,   493,   493,   495,   496,   497,   498,   494,
     500,   501,   502,   503,   499,   505,   506,   507,   508,   504,
     509,   509,   509,   509,   509,   510,   510,   510,   510,   510,
     510,   510,   510,   511,   511,   512,   513,   513,   514,   514,
     515,   515,   516,   516,   517,   517,   518,   518,   519,   519,
     520,   520,   521,   521,   521,   522,   522,   522,   523,   523,
     524,   524,   524,   524,   524,   524,   525,   526,   524,   527,
     528,   524,   529,   530,   524,   531,   532,   524,   533,   533,
     533,   534,   534,   535,   536,   537,   535,   538,   538,   539,
     539,   539,   540,   541,   539,   542,   543,   539,   544,   545,
     539,   539,   546,   547,   539,   539,   548,   549,   539,   550,
     550,   551,   551,   552,   552,   552,   552,   552,   553,   553,
     554,   554,   555,   555,   555,   555,   555,   555
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     0,     1,
       1,     1,     1,     0,     2,     5,     1,     1,     2,     2,
       3,     2,     0,     2,     0,     0,     3,     0,     2,     5,
       3,     1,     2,     0,     4,     2,     2,     1,     1,     1,
       1,     2,     3,     3,     3,     3,     3,     2,     4,     0,
       1,     2,     1,     3,     1,     3,     3,     3,     2,     1,
       1,     0,     2,     4,     1,     1,     1,     1,     0,     0,
       3,     1,     1,     1,     1,     1,     4,     0,     6,     0,
       6,     2,     3,     3,     0,     5,     0,     5,     1,     1,
       3,     1,     1,     2,     2,     1,     2,     1,     2,     1,
       1,     1,     3,     1,     1,     1,     3,     3,     5,     3,
       3,     4,     4,     3,     4,     3,     4,     3,     3,     1,
       5,     1,     3,     2,     3,     2,     1,     1,     1,     1,
       1,     4,     1,     2,     3,     3,     3,     3,     2,     1,
       3,     0,     3,     0,     2,     3,     0,     2,     2,     1,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     3,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     3,     2,     2,
       3,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     3,     2,     2,     2,     2,     2,     3,
       3,     3,     4,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     1,     4,     0,     1,     1,     3,     0,     4,
       1,     1,     1,     1,     3,     7,     2,     2,     6,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     2,     2,     1,     1,     1,     1,     2,     2,     2,
       0,     2,     2,     3,     0,     2,     0,     4,     0,     2,
       1,     3,     0,     0,     7,     0,     0,     7,     3,     2,
       2,     2,     1,     1,     3,     2,     2,     3,     0,     0,
       5,     1,     2,     5,     5,     5,     6,     2,     1,     1,
       1,     2,     3,     2,     2,     3,     2,     3,     2,     2,
       3,     4,     1,     1,     0,     1,     1,     1,     0,     1,
       3,     9,     8,     8,     7,     8,     7,     7,     6,     3,
       3,     0,     0,     7,     0,     0,     7,     0,     0,     7,
       0,     0,     6,     5,     8,    10,     5,     8,    10,     1,
       3,     1,     2,     3,     1,     1,     2,     2,     2,     2,
       2,     4,     1,     3,     0,     4,     4,     1,     6,     6,
       0,     7,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     2,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     6,     8,     5,     6,     1,     4,     3,     0,     0,
       8,     0,     0,     9,     3,     4,     5,     6,     8,     5,
       6,     0,     0,     5,     3,     4,     4,     5,     4,     3,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       2,     4,     3,     4,     5,     4,     5,     3,     4,     1,
       1,     2,     4,     4,     7,     8,     3,     5,     0,     0,
       8,     3,     3,     3,     0,     0,     8,     3,     4,     0,
       0,     9,     4,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     2,     4,     1,     4,     4,     4,     4,
       4,     1,     6,     7,     6,     6,     7,     7,     6,     7,
       6,     6,     0,     4,     1,     0,     1,     1,     0,     1,
       0,     1,     1,     0,     1,     5,     0,     2,     0,     7,
       0,     4,     0,     9,     0,    10,     5,     3,     3,     4,
       1,     1,     3,     3,     3,     1,     3,     1,     3,     0,
       2,     3,     3,     1,     3,     0,     2,     3,     1,     1,
       1,     2,     3,     3,     5,     1,     1,     1,     1,     1,
       0,     1,     1,     4,     3,     3,     6,     5,     4,     6,
       5,     5,     4,     0,     2,     0,     4,     0,     1,     0,
       1,     1,     6,     0,     6,     0,     2,     3,     5,     0,
       1,     1,     0,     5,     2,     3,     4,     0,     4,     0,
       1,     1,     1,     0,     0,     9,     0,     0,    11,     0,
       2,     0,     1,     3,     1,     1,     2,     2,     0,     1,
       1,     0,     3,     0,     0,     7,     1,     4,     3,     3,
       6,     5,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     4,     1,     3,     3,     0,     2,     3,
       5,     0,     2,     2,     2,     2,     4,     0,     0,     7,
       1,     1,     1,     3,     3,     4,     1,     1,     1,     1,
       2,     3,     0,     0,     6,     4,     3,     0,     0,     8,
       4,     2,     2,     3,     2,     3,     2,     2,     3,     3,
       3,     2,     0,     0,     6,     2,     0,     0,     6,     0,
       0,     6,     0,     0,     6,     1,     0,     0,     6,     0,
       0,     7,     1,     0,     0,     6,     0,     0,     7,     1,
       0,     0,     6,     0,     0,     7,     0,     0,     6,     0,
       0,     6,     1,     3,     3,     0,     0,     0,     0,    10,
       0,     0,     0,     0,    10,     0,     0,     0,     0,    11,
       1,     1,     1,     1,     1,     3,     3,     5,     5,     6,
       6,     8,     8,     0,     1,     2,     1,     3,     3,     5,
       1,     2,     1,     0,     0,     2,     2,     1,     2,     1,
       2,     1,     2,     1,     1,     2,     1,     1,     0,     1,
       5,     4,     6,     7,     5,     7,     0,     0,    10,     0,
       0,    10,     0,     0,    10,     0,     0,     7,     1,     3,
       3,     3,     1,     5,     0,     0,    10,     1,     3,     3,
       4,     4,     0,     0,    11,     0,     0,    11,     0,     0,
      10,     5,     0,     0,     9,     5,     0,     0,    10,     1,
       3,     1,     3,     3,     3,     4,     7,     9,     0,     3,
       0,     1,     9,    10,    10,    10,     9,    10
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
            { /* gc_node; */ }
        break;

    case YYSYMBOL_string_builder: /* string_builder  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_reader: /* expr_reader  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_keyword_or_name: /* keyword_or_name  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_require_module_name: /* require_module_name  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_expression_label: /* expression_label  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_goto: /* expression_goto  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_else: /* expression_else  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_else_one_liner: /* expression_else_one_liner  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_if_one_liner: /* expression_if_one_liner  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_if_then_else: /* expression_if_then_else  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_for_loop: /* expression_for_loop  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_unsafe: /* expression_unsafe  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_while_loop: /* expression_while_loop  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_with: /* expression_with  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_with_alias: /* expression_with_alias  */
            { /* gc_node; */ }
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
            { /* gc owns AnnotationDeclaration */ }
        break;

    case YYSYMBOL_annotation_declaration: /* annotation_declaration  */
            { /* gc owns AnnotationDeclaration */ }
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
            { /* gc owns TypeDecl */ }
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
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_call_pipe: /* expr_call_pipe  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_any: /* expression_any  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expressions: /* expressions  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_keyword: /* expr_keyword  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_optional_expr_list: /* optional_expr_list  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_optional_expr_list_in_braces: /* optional_expr_list_in_braces  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_optional_expr_map_tuple_list: /* optional_expr_map_tuple_list  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_type_declaration_no_options_list: /* type_declaration_no_options_list  */
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_expression_keyword: /* expression_keyword  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_pipe: /* expr_pipe  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_name_in_namespace: /* name_in_namespace  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_expression_delete: /* expression_delete  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_new_type_declaration: /* new_type_declaration  */
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_expr_new: /* expr_new  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_break: /* expression_break  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_continue: /* expression_continue  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_return_no_pipe: /* expression_return_no_pipe  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_return: /* expression_return  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_yield_no_pipe: /* expression_yield_no_pipe  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_yield: /* expression_yield  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expression_try_catch: /* expression_try_catch  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_tuple_expansion: /* tuple_expansion  */
            { delete ((*yyvaluep).pNameList); }
        break;

    case YYSYMBOL_tuple_expansion_variable_declaration: /* tuple_expansion_variable_declaration  */
            { delete ((*yyvaluep).pVarDecl); }
        break;

    case YYSYMBOL_expression_let: /* expression_let  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_cast: /* expr_cast  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_type_decl: /* expr_type_decl  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_type_info: /* expr_type_info  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_list: /* expr_list  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_block_or_simple_block: /* block_or_simple_block  */
            { /* gc_node; */ }
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
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_full_block: /* expr_full_block  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_full_block_assumed_piped: /* expr_full_block_assumed_piped  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_numeric_const: /* expr_numeric_const  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_assign: /* expr_assign  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_assign_pipe_right: /* expr_assign_pipe_right  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_assign_pipe: /* expr_assign_pipe  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_named_call: /* expr_named_call  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_method_call: /* expr_method_call  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_func_addr_name: /* func_addr_name  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_func_addr_expr: /* func_addr_expr  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_field: /* expr_field  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_call: /* expr_call  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr: /* expr  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_mtag: /* expr_mtag  */
            { /* gc_node; */ }
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
            { /* gc owns Enumeration */ }
        break;

    case YYSYMBOL_enum_name: /* enum_name  */
            { /* $$->delRef(); // if enum rule returns, module already has the link */ }
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
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_auto_type_declaration: /* auto_type_declaration  */
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_bitfield_bits: /* bitfield_bits  */
            { delete ((*yyvaluep).pNameList); }
        break;

    case YYSYMBOL_bitfield_alias_bits: /* bitfield_alias_bits  */
            { deleteNameExprList(((*yyvaluep).pNameExprList)); }
        break;

    case YYSYMBOL_bitfield_type_declaration: /* bitfield_type_declaration  */
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_table_type_pair: /* table_type_pair  */
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_dim_list: /* dim_list  */
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_type_declaration_no_options: /* type_declaration_no_options  */
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_type_declaration: /* type_declaration  */
            { /* gc owns TypeDecl */ }
        break;

    case YYSYMBOL_make_decl: /* make_decl  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_struct_fields: /* make_struct_fields  */
            { /* gc owns MakeStruct */ }
        break;

    case YYSYMBOL_make_variant_dim: /* make_variant_dim  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_struct_single: /* make_struct_single  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_struct_dim: /* make_struct_dim  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_struct_dim_list: /* make_struct_dim_list  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_struct_dim_decl: /* make_struct_dim_decl  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_optional_make_struct_dim_decl: /* optional_make_struct_dim_decl  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_optional_block: /* optional_block  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_struct_decl: /* make_struct_decl  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_tuple: /* make_tuple  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_map_tuple: /* make_map_tuple  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_tuple_call: /* make_tuple_call  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_dim: /* make_dim  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_dim_decl: /* make_dim_decl  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_table: /* make_table  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_expr_map_tuple_list: /* expr_map_tuple_list  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_make_table_decl: /* make_table_decl  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_array_comprehension_where: /* array_comprehension_where  */
            { /* gc_node; */ }
        break;

    case YYSYMBOL_array_comprehension: /* array_comprehension  */
            { /* gc_node; */ }
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
                das_yyerror(scanner,"module name has to be first declaration",tokAt(scanner,(yylsp[0])), CompilationError::invalid_module);
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
        (void)(yyvsp[-1].pExpression); // gc_node — Expression, don't delete
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
                CompilationError::already_declared_module_name);
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
        if ( err ) das_yyerror(scanner,"invalid escape sequence",tokAt(scanner,(yylsp[-1])), CompilationError::invalid_escape);
        auto sc = new ExprConstString(tokAt(scanner,(yylsp[0])),esconst);
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
            call_fmt->arguments.push_back(new ExprConstString(tokAt(scanner,(yylsp[-1])),":" + *(yyvsp[-1].s)));
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
            // gc_node — don't delete $sb
        } else if ( strb->elements.size()==1 && strb->elements[0]->rtti_isStringConstant() ) {
            auto sconst = static_cast<ExprConstString*>(strb->elements[0]);
            (yyval.pExpression) = new ExprConstString(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),sconst->text);
            // gc_node — don't delete $sb
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
                CompilationError::lookup_macro);
            if ( yychar == '~' ) {
                yyextra->g_ReaderMacro = das_unknown_reader_macro();
                yyextra->g_ReaderExpr = new ExprReader(tokAt(scanner,(yylsp[-1])),yyextra->g_ReaderMacro);
                yyclearin ;
                das_yybegin_reader(scanner);
            }
        } else if ( macros.size()>1 ) {
            string options;
            for ( auto & x : macros ) {
                options += "\t" + x->module->name + "::" + x->name + "\n";
            }
            das_yyerror(scanner,"too many options for the reader macro " + *(yyvsp[0].s) +  "\n" + options, tokAt(scanner,(yylsp[0])),
                CompilationError::ambiguous_macro);
        } else if ( yychar != '~' ) {
            das_yyerror(scanner,"expecting ~ after the reader macro", tokAt(scanner,(yylsp[0])),
                CompilationError::invalid_macro);
        } else {
            yyextra->g_ReaderMacro = macros.back();
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
                if ( yyextra->g_Access->isOptionBlocked(opt.name, yyextra->g_Program->thisModule->fileName) ) {
                    // blocked: ok to write, silently ignored (not applied)
                } else {
                    yyextra->g_Program->options.push_back(opt);
                }
            } else {
                das_yyerror(scanner,"option " + opt.name + " is not allowed here",
                    tokAt(scanner,(yylsp[0])), CompilationError::invalid_options);
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

  case 52: /* require_module_name: '.' '/' require_module_name  */
                                         {
        *(yyvsp[0].s) = "./" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 53: /* require_module_name: ".." '/' require_module_name  */
                                            {
        *(yyvsp[0].s) = "../" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 54: /* require_module_name: '%' '/' require_module_name  */
                                         {
        *(yyvsp[0].s) = "%/" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 55: /* require_module_name: require_module_name '.' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 56: /* require_module_name: require_module_name '/' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 57: /* require_module: require_module_name is_public_module  */
                                                         {
        ast_requireModule(scanner,(yyvsp[-1].s),nullptr,(yyvsp[0].b),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 58: /* require_module: require_module_name "as" "name" is_public_module  */
                                                                              {
        ast_requireModule(scanner,(yyvsp[-3].s),(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 59: /* is_public_module: %empty  */
                    { (yyval.b) = false; }
    break;

  case 60: /* is_public_module: "public"  */
                    { (yyval.b) = true; }
    break;

  case 64: /* expect_error: "integer constant"  */
                   {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
    break;

  case 65: /* expect_error: "integer constant" ':' "integer constant"  */
                                      {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
    break;

  case 66: /* expression_label: "label" "integer constant" ':'  */
                                          {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }
    break;

  case 67: /* expression_goto: "goto" "label" "integer constant"  */
                                                {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }
    break;

  case 68: /* expression_goto: "goto" expr  */
                               {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 69: /* elif_or_static_elif: "elif"  */
                          { (yyval.b) = false; }
    break;

  case 70: /* elif_or_static_elif: "static_elif"  */
                          { (yyval.b) = true; }
    break;

  case 71: /* expression_else: %empty  */
                                                           { (yyval.pExpression) = nullptr; }
    break;

  case 72: /* expression_else: "else" expression_block  */
                                                           { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 73: /* expression_else: elif_or_static_elif expr expression_block expression_else  */
                                                                                          {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),
            (yyvsp[-1].pExpression),(yyvsp[0].pExpression));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 74: /* semicolon: "end of line"  */
                       {}
    break;

  case 75: /* semicolon: "end of expression"  */
          {}
    break;

  case 76: /* if_or_static_if: "if"  */
                        { (yyval.b) = false; }
    break;

  case 77: /* if_or_static_if: "static_if"  */
                        { (yyval.b) = true; }
    break;

  case 78: /* expression_else_one_liner: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 79: /* $@3: %empty  */
                      { yyextra->das_need_oxford_comma = true; }
    break;

  case 80: /* expression_else_one_liner: "else" $@3 expression_if_one_liner  */
                                                                                                 {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 81: /* expression_if_one_liner: expr  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 82: /* expression_if_one_liner: expression_return_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 83: /* expression_if_one_liner: expression_yield_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 84: /* expression_if_one_liner: expression_break  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 85: /* expression_if_one_liner: expression_continue  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 86: /* expression_if_then_else: if_or_static_if expr expression_block expression_else  */
                                                                                      {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),
            (yyvsp[-1].pExpression),(yyvsp[0].pExpression));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 87: /* $@4: %empty  */
                                                     { yyextra->das_need_oxford_comma = true; }
    break;

  case 88: /* expression_if_then_else: expression_if_one_liner "if" $@4 expr expression_else_one_liner semicolon  */
                                                                                                                                                         {
        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-4])),(yyvsp[-2].pExpression),ast_wrapInBlock((yyvsp[-5].pExpression)),(yyvsp[-1].pExpression) ? ast_wrapInBlock((yyvsp[-1].pExpression)) : nullptr);
    }
    break;

  case 89: /* $@5: %empty  */
                     { yyextra->das_need_oxford_comma=false; }
    break;

  case 90: /* expression_for_loop: "for" $@5 variable_name_with_pos_list "in" expr_list expression_block  */
                                                                                                                                                 {
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-3].pNameWithPosList),(yyvsp[-1].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 91: /* expression_unsafe: "unsafe" expression_block  */
                                                 {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-1])));
        pUnsafe->body = (yyvsp[0].pExpression);
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 92: /* expression_while_loop: "while" expr expression_block  */
                                                               {
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-2])));
        pWhile->cond = (yyvsp[-1].pExpression);
        pWhile->body = (yyvsp[0].pExpression);
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 93: /* expression_with: "with" expr expression_block  */
                                                         {
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-2])));
        pWith->with = (yyvsp[-1].pExpression);
        pWith->body = (yyvsp[0].pExpression);
        (yyval.pExpression) = pWith;
    }
    break;

  case 94: /* $@6: %empty  */
                                        { yyextra->das_need_oxford_comma=true; }
    break;

  case 95: /* expression_with_alias: "assume" "name" '=' $@6 expr  */
                                                                                               {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-4])), *(yyvsp[-3].s), ExpressionPtr((yyvsp[0].pExpression)));
        delete (yyvsp[-3].s);
    }
    break;

  case 96: /* $@7: %empty  */
                         { yyextra->das_force_oxford_comma=true;}
    break;

  case 97: /* expression_with_alias: "typedef" $@7 "name" '=' type_declaration  */
                                                                                                         {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-4])), *(yyvsp[-2].s), TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
    break;

  case 98: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 99: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 100: /* annotation_argument_value: '@' '@' "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 101: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 102: /* annotation_argument_value: "long integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",int64_t((yyvsp[0].i64))); }
    break;

  case 103: /* annotation_argument_value: '-' "long integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",int64_t(0ull - uint64_t((yyvsp[0].i64)))); }
    break;

  case 104: /* annotation_argument_value: '-' "int64 minimum magnitude (requires minus)"  */
                                 { (yyval.aa) = new AnnotationArgument("",int64_t(INT64_MIN)); }
    break;

  case 105: /* annotation_argument_value: "unsigned long integer constant"  */
                                     { (yyval.aa) = new AnnotationArgument("",uint64_t((yyvsp[0].ui64))); }
    break;

  case 106: /* annotation_argument_value: '-' "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",int32_t(0u - uint32_t((yyvsp[0].i)))); }
    break;

  case 107: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 108: /* annotation_argument_value: '-' "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",-float((yyvsp[0].fd))); }
    break;

  case 109: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 110: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 111: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 112: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 113: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 114: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 115: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 116: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 117: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 118: /* annotation_argument: annotation_argument_name '=' '@' '@' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[0].s); delete (yyvsp[-4].s); }
    break;

  case 119: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 120: /* annotation_argument: annotation_argument_name '=' "long integer constant"  */
                                                               { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),int64_t((yyvsp[0].i64)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 121: /* annotation_argument: annotation_argument_name '=' '-' "long integer constant"  */
                                                                   { (yyval.aa) = new AnnotationArgument(*(yyvsp[-3].s),int64_t(0ull - uint64_t((yyvsp[0].i64))),tokAt(scanner,(yylsp[-3]))); delete (yyvsp[-3].s); }
    break;

  case 122: /* annotation_argument: annotation_argument_name '=' '-' "int64 minimum magnitude (requires minus)"  */
                                                                { (yyval.aa) = new AnnotationArgument(*(yyvsp[-3].s),int64_t(INT64_MIN),tokAt(scanner,(yylsp[-3]))); delete (yyvsp[-3].s); }
    break;

  case 123: /* annotation_argument: annotation_argument_name '=' "unsigned long integer constant"  */
                                                                        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),uint64_t((yyvsp[0].ui64)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 124: /* annotation_argument: annotation_argument_name '=' '-' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-3].s),int32_t(0u - uint32_t((yyvsp[0].i))),tokAt(scanner,(yylsp[-3]))); delete (yyvsp[-3].s); }
    break;

  case 125: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 126: /* annotation_argument: annotation_argument_name '=' '-' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-3].s),-float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-3]))); delete (yyvsp[-3].s); }
    break;

  case 127: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 128: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 129: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 130: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 131: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 132: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 133: /* metadata_argument_list: '@' annotation_argument  */
                                      {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 134: /* metadata_argument_list: metadata_argument_list '@' annotation_argument  */
                                                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 135: /* metadata_argument_list: metadata_argument_list semicolon  */
                                               {
        (yyval.aaList) = (yyvsp[-1].aaList);
    }
    break;

  case 136: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 137: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 138: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 139: /* annotation_declaration_name: "template"  */
                                    { (yyval.s) = new string("template"); }
    break;

  case 140: /* annotation_declaration_basic: annotation_declaration_name  */
                                          {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[0]));
        if ( yyextra->g_Access->isAnnotationAllowed(*(yyvsp[0].s), yyextra->g_Program->thisModuleName) ) {
            if ( auto ann = findAnnotation(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0]))) ) {
                (yyval.fa)->annotation = ann;
            } else {
                (yyval.fa)->annotation = new Annotation(*(yyvsp[0].s));
                das2_yyerror(scanner,"annotation " + *(yyvsp[0].s) + " is not found",
                            tokAt(scanner,(yylsp[0])), CompilationError::lookup_annotation);
            }
        } else {
            das_yyerror(scanner,"annotation " + *(yyvsp[0].s) + " is not allowed here",
                        tokAt(scanner,(yylsp[0])), CompilationError::invalid_annotation);
        }
        delete (yyvsp[0].s);
    }
    break;

  case 141: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
                                                                                 {
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner,(yylsp[-3]));
        if ( yyextra->g_Access->isAnnotationAllowed(*(yyvsp[-3].s), yyextra->g_Program->thisModuleName) ) {
            if ( auto ann = findAnnotation(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3]))) ) {
                (yyval.fa)->annotation = ann;
            } else {
                (yyval.fa)->annotation = new Annotation(*(yyvsp[-3].s));
                das2_yyerror(scanner,"annotation " + *(yyvsp[-3].s) + " is not found",
                            tokAt(scanner,(yylsp[-3])), CompilationError::lookup_annotation);
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

  case 142: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 143: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation);
            (yyvsp[0].fa) = nullptr; // gc_node — don't delete AnnotationDeclaration
        }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 144: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
                                                                              {
        if ( !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation);
            (yyvsp[-2].fa) = nullptr; // gc_node — don't delete AnnotationDeclaration
        }
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation);
            (yyvsp[0].fa) = nullptr; // gc_node — don't delete AnnotationDeclaration
        }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::And,(yyvsp[-2].fa),(yyvsp[0].fa));
    }
    break;

  case 145: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
                                                                            {
        if ( !(yyvsp[-2].fa)->annotation || !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation);
            (yyvsp[-2].fa) = nullptr; // gc_node — don't delete AnnotationDeclaration
        }
        if ( !(yyvsp[0].fa)->annotation || !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation);
            (yyvsp[0].fa) = nullptr; // gc_node — don't delete AnnotationDeclaration
        }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Or,(yyvsp[-2].fa),(yyvsp[0].fa));
    }
    break;

  case 146: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
                                                                              {
        if ( !(yyvsp[-2].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[-2].fa)->annotation))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[-2])),
                CompilationError::invalid_annotation);
            (yyvsp[-2].fa) = nullptr; // gc_node — don't delete AnnotationDeclaration
        }
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation);
            (yyvsp[0].fa) = nullptr; // gc_node — don't delete AnnotationDeclaration
        }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Xor,(yyvsp[-2].fa),(yyvsp[0].fa));
    }
    break;

  case 147: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 148: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 149: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 150: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 151: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 152: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 153: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 154: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 155: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 156: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yyloc));
    }
    break;

  case 157: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 158: /* optional_function_type: "->" type_declaration  */
                                           {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 159: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 160: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 161: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 162: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 163: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 164: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 165: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 166: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 167: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 168: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 169: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 170: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 171: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 172: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 173: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 174: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 175: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 176: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 177: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 178: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 179: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 180: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 181: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 182: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 183: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 184: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 185: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 186: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 187: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 188: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 189: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 190: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 191: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 192: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 193: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 194: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 195: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 196: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 197: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 198: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 199: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 200: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 201: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 202: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 203: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 204: /* function_name: "operator" '[' ']' "<-"  */
                                    { (yyval.s) = new string("[]<-"); }
    break;

  case 205: /* function_name: "operator" '[' ']' ":="  */
                                      { (yyval.s) = new string("[]:="); }
    break;

  case 206: /* function_name: "operator" '[' ']' "+="  */
                                     { (yyval.s) = new string("[]+="); }
    break;

  case 207: /* function_name: "operator" '[' ']' "-="  */
                                     { (yyval.s) = new string("[]-="); }
    break;

  case 208: /* function_name: "operator" '[' ']' "*="  */
                                     { (yyval.s) = new string("[]*="); }
    break;

  case 209: /* function_name: "operator" '[' ']' "/="  */
                                     { (yyval.s) = new string("[]/="); }
    break;

  case 210: /* function_name: "operator" '[' ']' "%="  */
                                     { (yyval.s) = new string("[]%="); }
    break;

  case 211: /* function_name: "operator" '[' ']' "&="  */
                                     { (yyval.s) = new string("[]&="); }
    break;

  case 212: /* function_name: "operator" '[' ']' "|="  */
                                     { (yyval.s) = new string("[]|="); }
    break;

  case 213: /* function_name: "operator" '[' ']' "^="  */
                                     { (yyval.s) = new string("[]^="); }
    break;

  case 214: /* function_name: "operator" '[' ']' "&&="  */
                                        { (yyval.s) = new string("[]&&="); }
    break;

  case 215: /* function_name: "operator" '[' ']' "||="  */
                                        { (yyval.s) = new string("[]||="); }
    break;

  case 216: /* function_name: "operator" '[' ']' "^^="  */
                                        { (yyval.s) = new string("[]^^="); }
    break;

  case 217: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 218: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 219: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 220: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 221: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 222: /* function_name: "operator" '.' "name" "+="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`+="); delete (yyvsp[-1].s); }
    break;

  case 223: /* function_name: "operator" '.' "name" "-="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`-="); delete (yyvsp[-1].s); }
    break;

  case 224: /* function_name: "operator" '.' "name" "*="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`*="); delete (yyvsp[-1].s); }
    break;

  case 225: /* function_name: "operator" '.' "name" "/="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`/="); delete (yyvsp[-1].s); }
    break;

  case 226: /* function_name: "operator" '.' "name" "%="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`%="); delete (yyvsp[-1].s); }
    break;

  case 227: /* function_name: "operator" '.' "name" "&="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&="); delete (yyvsp[-1].s); }
    break;

  case 228: /* function_name: "operator" '.' "name" "|="  */
                                          { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`|="); delete (yyvsp[-1].s); }
    break;

  case 229: /* function_name: "operator" '.' "name" "^="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^="); delete (yyvsp[-1].s); }
    break;

  case 230: /* function_name: "operator" '.' "name" "&&="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&&="); delete (yyvsp[-1].s); }
    break;

  case 231: /* function_name: "operator" '.' "name" "||="  */
                                            { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`||="); delete (yyvsp[-1].s); }
    break;

  case 232: /* function_name: "operator" '.' "name" "^^="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^^="); delete (yyvsp[-1].s); }
    break;

  case 233: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 234: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 235: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 236: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 237: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 238: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 239: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 240: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 241: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 242: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 243: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 244: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 245: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 246: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 247: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 248: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 249: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 250: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 251: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 252: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 253: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 254: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 255: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 256: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 257: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 258: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 259: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 260: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 261: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 262: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 263: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 264: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 265: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 266: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 267: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 268: /* function_name: "float16"  */
                     { (yyval.s) = new string("float16"); }
    break;

  case 269: /* function_name: "half2"  */
                     { (yyval.s) = new string("half2"); }
    break;

  case 270: /* function_name: "half3"  */
                     { (yyval.s) = new string("half3"); }
    break;

  case 271: /* function_name: "half4"  */
                     { (yyval.s) = new string("half4"); }
    break;

  case 272: /* function_name: "half8"  */
                     { (yyval.s) = new string("half8"); }
    break;

  case 273: /* function_name: "short2"  */
                     { (yyval.s) = new string("short2"); }
    break;

  case 274: /* function_name: "short3"  */
                     { (yyval.s) = new string("short3"); }
    break;

  case 275: /* function_name: "short4"  */
                     { (yyval.s) = new string("short4"); }
    break;

  case 276: /* function_name: "short8"  */
                     { (yyval.s) = new string("short8"); }
    break;

  case 277: /* function_name: "ushort2"  */
                     { (yyval.s) = new string("ushort2"); }
    break;

  case 278: /* function_name: "ushort3"  */
                     { (yyval.s) = new string("ushort3"); }
    break;

  case 279: /* function_name: "ushort4"  */
                     { (yyval.s) = new string("ushort4"); }
    break;

  case 280: /* function_name: "ushort8"  */
                     { (yyval.s) = new string("ushort8"); }
    break;

  case 281: /* function_name: "byte2"  */
                     { (yyval.s) = new string("byte2"); }
    break;

  case 282: /* function_name: "byte3"  */
                     { (yyval.s) = new string("byte3"); }
    break;

  case 283: /* function_name: "byte4"  */
                     { (yyval.s) = new string("byte4"); }
    break;

  case 284: /* function_name: "byte8"  */
                     { (yyval.s) = new string("byte8"); }
    break;

  case 285: /* function_name: "byte16"  */
                     { (yyval.s) = new string("byte16"); }
    break;

  case 286: /* function_name: "ubyte2"  */
                     { (yyval.s) = new string("ubyte2"); }
    break;

  case 287: /* function_name: "ubyte3"  */
                     { (yyval.s) = new string("ubyte3"); }
    break;

  case 288: /* function_name: "ubyte4"  */
                     { (yyval.s) = new string("ubyte4"); }
    break;

  case 289: /* function_name: "ubyte8"  */
                     { (yyval.s) = new string("ubyte8"); }
    break;

  case 290: /* function_name: "ubyte16"  */
                     { (yyval.s) = new string("ubyte16"); }
    break;

  case 291: /* optional_template: %empty  */
                                        { (yyval.b) = false; }
    break;

  case 292: /* optional_template: "template"  */
                                        { (yyval.b) = true; }
    break;

  case 293: /* global_function_declaration: optional_annotation_list "def" optional_template function_declaration  */
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
                        CompilationError::already_declared_function);
            }
        }
        (yyvsp[0].pFuncDecl)->delRef();
    }
    break;

  case 294: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 295: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 296: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 297: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 298: /* $@8: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 299: /* function_declaration: optional_public_or_private_function $@8 function_declaration_header expression_block  */
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

  case 304: /* expression_block: open_block expressions close_block  */
                                                                  {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 305: /* expression_block: open_block expressions close_block "finally" open_block expressions close_block  */
                                                                                                                        {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        // gc_node — don't delete Expression
    }
    break;

  case 306: /* expr_call_pipe: expr expr_full_block_assumed_piped  */
                                                      {
        if ( (yyvsp[-1].pExpression)->rtti_isCallLikeExpr() ) {
            ((ExprLooksLikeCall *)(yyvsp[-1].pExpression))->arguments.push_back((yyvsp[0].pExpression));
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        } else {
            (yyval.pExpression) = (yyvsp[-1].pExpression);
            // gc_node — don't delete Expression
        }
    }
    break;

  case 307: /* expr_call_pipe: expression_keyword expr_full_block_assumed_piped  */
                                                                    {
        if ( (yyvsp[-1].pExpression)->rtti_isCallLikeExpr() ) {
            ((ExprLooksLikeCall *)(yyvsp[-1].pExpression))->arguments.push_back((yyvsp[0].pExpression));
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        } else {
            (yyval.pExpression) = (yyvsp[-1].pExpression);
            // gc_node — don't delete Expression
        }
    }
    break;

  case 308: /* expr_call_pipe: "generator" '<' type_declaration_no_options '>' optional_capture_list expr_full_block_assumed_piped  */
                                                                                                                                             {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-3].pTypeDecl),(yyvsp[-1].pCaptList),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 309: /* expression_any: semicolon  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 310: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 311: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 312: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 313: /* expression_any: expr_assign semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 314: /* expression_any: expression_delete semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 315: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 316: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 317: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 318: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 319: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 320: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 321: /* expression_any: expression_break semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 322: /* expression_any: expression_continue semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 323: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 324: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 325: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 326: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 327: /* expression_any: expression_label semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 328: /* expression_any: expression_goto semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 329: /* expression_any: "pass" semicolon  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 330: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 331: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back((yyvsp[0].pExpression));
        }
    }
    break;

  case 332: /* expressions: expressions error  */
                                 {
        (void)(yyvsp[-1].pExpression); /* gc_node — don't delete Expression */ (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 333: /* expr_keyword: "keyword" expr expression_block  */
                                                           {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s));
        pCall->arguments.push_back((yyvsp[-1].pExpression));
        auto resT = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        auto blk = ast_makeBlock(scanner,0,nullptr,nullptr,nullptr,resT,(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),LineInfo());
        pCall->arguments.push_back(blk);
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 334: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 335: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 336: /* optional_expr_list_in_braces: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 337: /* optional_expr_list_in_braces: '(' optional_expr_list optional_comma ')'  */
                                                             { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 338: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 339: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 340: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 341: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 342: /* $@9: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 343: /* $@10: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 344: /* expression_keyword: "keyword" '<' $@9 type_declaration_no_options_list '>' $@10 expr  */
                                                                                                                                                     {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 345: /* $@11: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 346: /* $@12: %empty  */
                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 347: /* expression_keyword: "type function" '<' $@11 type_declaration_no_options_list '>' $@12 optional_expr_list_in_braces  */
                                                                                                                                                                                   {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 348: /* expr_pipe: expr_assign " <|" expr_block  */
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
                    tokAt(scanner,(yylsp[-1])),CompilationError::cant_expression);
                delete (yyvsp[0].pExpression);
            } else {
                pMS->block = (yyvsp[0].pExpression);
            }
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        } else {
            das_yyerror(scanner,"can only pipe into function call or [[ make structure ]]",
                tokAt(scanner,(yylsp[-1])),CompilationError::cant_expression);
            delete (yyvsp[0].pExpression);
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        }
    }
    break;

  case 349: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 350: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 351: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 352: /* expr_pipe: expr_call_pipe  */
                             {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 353: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 354: /* name_in_namespace: "name" "::" "name"  */
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

  case 355: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 356: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
    }
    break;

  case 357: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 358: /* $@13: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 359: /* $@14: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 360: /* new_type_declaration: '<' $@13 type_declaration '>' $@14  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 361: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 362: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pTypeDecl),false);
    }
    break;

  case 363: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 364: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 365: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),(yyvsp[-1].pExpression));
    }
    break;

  case 366: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),(yyvsp[-1].pExpression));
    }
    break;

  case 367: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 368: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 369: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 370: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 371: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 372: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 373: /* expression_return: expression_return_no_pipe semicolon  */
                                                    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 374: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 375: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 376: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 377: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 378: /* expression_yield: expression_yield_no_pipe semicolon  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 379: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 380: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 381: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 382: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 383: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 384: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 385: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 386: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 387: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 388: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 389: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 390: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 391: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-7].pNameList),tokAt(scanner,(yylsp[-7])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 392: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 393: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 394: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                           {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameList),tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 395: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr semicolon  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-6])));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 396: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-5])));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameList),tokAt(scanner,(yylsp[-5])),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 397: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr semicolon  */
                                                                                                        {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-5])));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameList),tokAt(scanner,(yylsp[-5])),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 398: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                   {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-4])));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameList),tokAt(scanner,(yylsp[-4])),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 399: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 400: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 401: /* $@15: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 402: /* $@16: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 403: /* expr_cast: "cast" '<' $@15 type_declaration_no_options '>' $@16 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
    }
    break;

  case 404: /* $@17: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 405: /* $@18: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 406: /* expr_cast: "upcast" '<' $@17 type_declaration_no_options '>' $@18 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 407: /* $@19: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 408: /* $@20: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 409: /* expr_cast: "reinterpret" '<' $@19 type_declaration_no_options '>' $@20 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 410: /* $@21: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 411: /* $@22: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 412: /* expr_type_decl: "type" '<' $@21 type_declaration '>' $@22  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 413: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
                                                                         {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-2].s),ptd->typeexpr);
                // gc_node — don't delete Expression
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-2].s),(yyvsp[-1].pExpression));
            }
            delete (yyvsp[-2].s);
    }
    break;

  case 414: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
                                                                                                {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-5].s),ptd->typeexpr,*(yyvsp[-3].s));
                // gc_node — don't delete Expression
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-5].s),(yyvsp[-1].pExpression),*(yyvsp[-3].s));
            }
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
    break;

  case 415: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" c_or_s "name" '>' expr ')'  */
                                                                                                                        {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-7].s),ptd->typeexpr,*(yyvsp[-5].s),*(yyvsp[-3].s));
                // gc_node — don't delete Expression
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-7].s),(yyvsp[-1].pExpression),*(yyvsp[-5].s),*(yyvsp[-3].s));
            }
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
    break;

  case 416: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
                                                                          {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-3].s),ptd->typeexpr);
                // gc_node — don't delete Expression
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-3].s),(yyvsp[-1].pExpression));
            }
            delete (yyvsp[-3].s);
    }
    break;

  case 417: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
                                                                                                {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-6].s),ptd->typeexpr,*(yyvsp[-4].s));
                // gc_node — don't delete Expression
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-6].s),(yyvsp[-1].pExpression),*(yyvsp[-4].s));
            }
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
    break;

  case 418: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" "end of expression" "name" '>' '(' expr ')'  */
                                                                                                                     {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-8].s),ptd->typeexpr,*(yyvsp[-6].s),*(yyvsp[-4].s));
                // gc_node — don't delete Expression
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-8].s),(yyvsp[-1].pExpression),*(yyvsp[-6].s),*(yyvsp[-4].s));
            }
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
    break;

  case 419: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 420: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 421: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 422: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = new ExprReturn(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->generated = true;
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 423: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = new ExprReturn(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->generated = true;
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 424: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 425: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 426: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 427: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 428: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 429: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 430: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 431: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 432: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 433: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 434: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 435: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 436: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 437: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),(yyvsp[0].pExpression));
        closure->returnType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
    }
    break;

  case 438: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 439: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 440: /* $@23: %empty  */
                             {  yyextra->das_need_oxford_comma = false; }
    break;

  case 441: /* expr_full_block_assumed_piped: block_or_lambda $@23 optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 442: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstInt>(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 443: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstUInt>(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 444: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstInt64>(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 445: /* expr_numeric_const: '-' "int64 minimum magnitude (requires minus)"  */
                                             { (yyval.pExpression) = newConstLiteral<ExprConstInt64>(tokAt(scanner,(yylsp[0])),int64_t(INT64_MIN)); }
    break;

  case 446: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstUInt64>(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 447: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstUInt8>(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 448: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstFloat>(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 449: /* expr_numeric_const: "float16 constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstFloat16>(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 450: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstDouble>(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 451: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 452: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 453: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 454: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 455: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 456: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 457: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 458: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 459: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 460: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 461: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 462: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 463: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 464: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 465: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 466: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 467: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 468: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 469: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 470: /* expr_assign_pipe_right: "@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 471: /* expr_assign_pipe_right: "@@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 472: /* expr_assign_pipe_right: "$ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 473: /* expr_assign_pipe_right: expr_call_pipe  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 474: /* expr_assign_pipe: expr '=' expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 475: /* expr_assign_pipe: expr "<-" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 476: /* expr_assign_pipe: expr "&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 477: /* expr_assign_pipe: expr "|=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 478: /* expr_assign_pipe: expr "^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 479: /* expr_assign_pipe: expr "&&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 480: /* expr_assign_pipe: expr "||=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 481: /* expr_assign_pipe: expr "^^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 482: /* expr_assign_pipe: expr "+=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 483: /* expr_assign_pipe: expr "-=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 484: /* expr_assign_pipe: expr "*=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 485: /* expr_assign_pipe: expr "/=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 486: /* expr_assign_pipe: expr "%=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 487: /* expr_assign_pipe: expr "<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 488: /* expr_assign_pipe: expr ">>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 489: /* expr_assign_pipe: expr "<<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 490: /* expr_assign_pipe: expr ">>>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 491: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 492: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 493: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 494: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-3])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-5]),(yyloc));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 495: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 496: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 497: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 498: /* $@24: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 499: /* $@25: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 500: /* func_addr_expr: '@' '@' '<' $@24 type_declaration_no_options '>' $@25 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value));
        expr->funcType = (yyvsp[-3].pTypeDecl);
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 501: /* $@26: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 502: /* $@27: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 503: /* func_addr_expr: '@' '@' '<' $@26 optional_function_argument_list optional_function_type '>' $@27 func_addr_name  */
                                                                                                                                                                                     {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value));
        expr->funcType = new TypeDecl(Type::tFunction, expr->at);
        expr->funcType->firstType = (yyvsp[-3].pTypeDecl);
        if ( (yyvsp[-4].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, expr->funcType, (yyvsp[-4].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-4].pVarDeclList));
        }
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 504: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 505: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 506: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 507: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-3])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-5]),(yyloc));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 508: /* expr_field: expr '.' "name" '(' '[' make_struct_fields ']' ')'  */
                                                                                       {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->methodCall = true;
        nc->arguments = (yyvsp[-2].pMakeStruct);
        nc->nonNamedArguments.push_back((yyvsp[-7].pExpression));
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 509: /* expr_field: expr '.' basic_type_declaration '(' ')'  */
                                                                        {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), method_name);
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 510: /* expr_field: expr '.' basic_type_declaration '(' expr_list ')'  */
                                                                                             {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-3])), (yyvsp[-5].pExpression), method_name);
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-5]),(yyloc));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 511: /* $@28: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 512: /* $@29: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 513: /* expr_field: expr '.' $@28 error $@29  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), "");
        yyerrok;
    }
    break;

  case 514: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 515: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
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

  case 516: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
                                                               {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-3].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 517: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
                                                                                 {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-4])),*(yyvsp[-4].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-4].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 518: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 519: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 520: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 521: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 522: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 523: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 524: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 525: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 526: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 527: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 528: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 529: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 530: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 531: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",(yyvsp[0].pExpression)); }
    break;

  case 532: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",(yyvsp[0].pExpression)); }
    break;

  case 533: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",(yyvsp[0].pExpression)); }
    break;

  case 534: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",(yyvsp[0].pExpression)); }
    break;

  case 535: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 536: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 537: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 538: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 539: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 540: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 541: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 542: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 543: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 544: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 545: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 546: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 547: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 548: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 549: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 550: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 551: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 552: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 553: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 554: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 555: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 556: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back((yyvsp[-2].pExpression));
        itv->arguments.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = itv;
    }
    break;

  case 557: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", (yyvsp[0].pExpression)); }
    break;

  case 558: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", (yyvsp[0].pExpression)); }
    break;

  case 559: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", (yyvsp[-1].pExpression)); }
    break;

  case 560: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", (yyvsp[-1].pExpression)); }
    break;

  case 561: /* expr: '(' expr_list optional_comma ')'  */
                                                         {
            if ( (yyvsp[-2].pExpression)->rtti_isSequence() ) {
                auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-2])));
                mkt->values = sequenceToList((yyvsp[-2].pExpression));
                mkt->shorthandRecordNames = ast_tupleCollectShorthandNames(mkt->values);
                (yyval.pExpression) = mkt;
            } else if ( (yyvsp[-1].b) ) {
                auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-2])));
                mkt->values.push_back((yyvsp[-2].pExpression));
                mkt->shorthandRecordNames = ast_tupleCollectShorthandNames(mkt->values);
                (yyval.pExpression) = mkt;
            } else {
                (yyval.pExpression) = (yyvsp[-2].pExpression);
            }
        }
    break;

  case 562: /* expr: '(' make_struct_single ')'  */
                                      {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        for ( auto & arg : *(((ExprMakeStruct *)(yyvsp[-1].pExpression))->structs.back()) ) {
            mkt->values.push_back(arg->value);
            mkt->recordNames.push_back(arg->name);
        }
        // gc_node — don't delete Expression
        (yyval.pExpression) = mkt;
    }
    break;

  case 563: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 564: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 565: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 566: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 567: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 568: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 569: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 570: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 571: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression)); }
    break;

  case 572: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 573: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 574: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 575: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 576: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 577: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
        }
    break;

  case 578: /* $@30: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 579: /* $@31: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 580: /* expr: expr "is" "type" '<' $@30 type_declaration_no_options '>' $@31  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 581: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type), tokAt(scanner,(yyloc)));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),vdecl);
    }
    break;

  case 582: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 583: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 584: /* $@32: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 585: /* $@33: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 586: /* expr: expr "as" "type" '<' $@32 type_declaration '>' $@33  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 587: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 588: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 589: /* $@34: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 590: /* $@35: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 591: /* expr: expr '?' "as" "type" '<' $@34 type_declaration '>' $@35  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-8].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 592: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 593: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 594: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 595: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 596: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 597: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 598: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 599: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 600: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 601: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 602: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 603: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 604: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 605: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 606: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 607: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 608: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 609: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 610: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 611: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 612: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 613: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 614: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 615: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 616: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 617: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 618: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 619: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-6].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 620: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 621: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 622: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 623: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); /*this one is gone when BRABRA is disabled*/ }
    break;

  case 624: /* optional_field_annotation: metadata_argument_list  */
                                                        { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 625: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 626: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 627: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 628: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 629: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 630: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 631: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 632: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 633: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 634: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 635: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 636: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 637: /* struct_variable_declaration_list: struct_variable_declaration_list semicolon  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 638: /* $@36: %empty  */
                                                               { yyextra->das_force_oxford_comma=true;}
    break;

  case 639: /* struct_variable_declaration_list: struct_variable_declaration_list "typedef" $@36 "name" '=' type_declaration semicolon  */
                                                                                                                                                         {
        (yyval.pVarDeclList) = (yyvsp[-6].pVarDeclList);
        ast_structureAlias(scanner,(yyvsp[-3].s),(yyvsp[-1].pTypeDecl),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 640: /* $@37: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 641: /* struct_variable_declaration_list: struct_variable_declaration_list $@37 structure_variable_declaration semicolon  */
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

  case 642: /* $@38: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 643: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@38 function_declaration_header semicolon  */
                                                          {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 644: /* $@39: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 645: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@39 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 646: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' semicolon  */
                                                                                       {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::invalid_annotation);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 647: /* function_argument_declaration_no_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_no_type  */
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

  case 648: /* function_argument_declaration_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_type  */
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

  case 649: /* function_argument_declaration_type: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))));
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none, tokAt(scanner,(yyloc))), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 650: /* function_argument_list: function_argument_declaration_no_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 651: /* function_argument_list: function_argument_declaration_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 652: /* function_argument_list: function_argument_declaration_no_type semicolon function_argument_list  */
                                                                                            { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 653: /* function_argument_list: function_argument_declaration_type semicolon function_argument_list  */
                                                                                            { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 654: /* function_argument_list: function_argument_declaration_type ',' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 655: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 656: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 657: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 658: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                          { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 659: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 660: /* tuple_alias_type_list: tuple_alias_type_list c_or_s  */
                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 661: /* tuple_alias_type_list: tuple_alias_type_list tuple_type c_or_s  */
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

  case 662: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 663: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 664: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 665: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 666: /* variant_alias_type_list: variant_alias_type_list c_or_s  */
                                           {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 667: /* variant_alias_type_list: variant_alias_type_list variant_type c_or_s  */
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

  case 668: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 669: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 670: /* variable_declaration_no_type: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[0])));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 671: /* variable_declaration_no_type: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-1])));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 672: /* variable_declaration_no_type: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-2])));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 673: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 674: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 675: /* variable_declaration: variable_declaration_type  */
                                        {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 676: /* variable_declaration: variable_declaration_no_type  */
                                           {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 677: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 678: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 679: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 680: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 681: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 682: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 683: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 684: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 685: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 686: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "$i" '(' expr ')'  */
                                                                               {
        (yyvsp[-5].pNameWithPosList)->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = (yyvsp[-5].pNameWithPosList);
    }
    break;

  case 687: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 688: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options semicolon  */
                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[-1]));
    }
    break;

  case 689: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[-1]));
    }
    break;

  case 690: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 691: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr semicolon  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-4])));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[-1]));
    }
    break;

  case 692: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-3])));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 693: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 694: /* global_variable_declaration_list: global_variable_declaration_list "end of line"  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 695: /* $@40: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 696: /* global_variable_declaration_list: global_variable_declaration_list $@40 optional_field_annotation let_variable_declaration  */
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

  case 697: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 698: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 699: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 700: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 701: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 702: /* global_let: kwd_let optional_shared optional_public_or_private_variable open_block global_variable_declaration_list close_block  */
                                                                                                                                                      {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 703: /* $@41: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 704: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@41 optional_field_annotation let_variable_declaration  */
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

  case 705: /* enum_list: %empty  */
        {
        (yyval.pEnumList) = new Enumeration();
    }
    break;

  case 706: /* enum_list: enum_list semicolon  */
                                {
        (yyval.pEnumList) = (yyvsp[-1].pEnumList);
    }
    break;

  case 707: /* enum_list: enum_list "name" semicolon  */
                                           {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        if ( !(yyvsp[-2].pEnumList)->add(*(yyvsp[-1].s),nullptr,tokAt(scanner,(yylsp[-1]))) ) {
            das_yyerror(scanner,"enumeration already declared " + *(yyvsp[-1].s), tokAt(scanner,(yylsp[-1])),
                CompilationError::already_declared_enumerator);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tokName = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) {
                crd->afterEnumerationEntry((yyvsp[-1].s)->c_str(), tokName);
            }
        }
        delete (yyvsp[-1].s);
        (yyval.pEnumList) = (yyvsp[-2].pEnumList);
    }
    break;

  case 708: /* enum_list: enum_list "name" '=' expr semicolon  */
                                                           {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        if ( !(yyvsp[-4].pEnumList)->add(*(yyvsp[-3].s),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-3]))) ) {
            das_yyerror(scanner,"enumeration value already declared " + *(yyvsp[-3].s), tokAt(scanner,(yylsp[-3])),
                CompilationError::already_declared_enumerator);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tokName = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) {
                crd->afterEnumerationEntry((yyvsp[-3].s)->c_str(), tokName);
            }
        }
        delete (yyvsp[-3].s);
        (yyval.pEnumList) = (yyvsp[-4].pEnumList);
    }
    break;

  case 709: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 710: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 711: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 712: /* $@42: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 713: /* single_alias: optional_public_or_private_alias "name" $@42 '=' type_declaration  */
                                  {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        (yyvsp[0].pTypeDecl)->isPrivateAlias = !(yyvsp[-4].b);
        if ( (yyvsp[0].pTypeDecl)->baseType == Type::alias ) {
            das_yyerror(scanner,"alias cannot be defined in terms of another alias "+*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])),
                CompilationError::invalid_type_alias);
        }
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !yyextra->g_Program->addAlias((yyvsp[0].pTypeDecl)) ) {
            das_yyerror(scanner,"type alias is already defined "+*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])),
                CompilationError::already_declared_type_alias);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterAlias((yyvsp[-3].s)->c_str(),pubename);
        }
        delete (yyvsp[-3].s);
    }
    break;

  case 717: /* $@43: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 719: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 720: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 721: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 722: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 723: /* $@44: %empty  */
                                                                                                                       {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 724: /* $@45: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 725: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name open_block $@44 enum_list $@45 close_block  */
                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-2].pEnumList),Type::tInt);
    }
    break;

  case 726: /* $@46: %empty  */
                                                                                                                                                            {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 727: /* $@47: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 728: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration open_block $@46 enum_list $@47 close_block  */
                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-2].pEnumList),(yyvsp[-5].type));
    }
    break;

  case 729: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 730: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 731: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 732: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 733: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 734: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 735: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 736: /* class_or_struct: "class" "template"  */
                                 { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 737: /* class_or_struct: "struct" "template"  */
                                 { (yyval.i) = CorS_StructTemplate; }
    break;

  case 738: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 739: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 740: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 741: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 742: /* optional_struct_variable_declaration_list: open_block struct_variable_declaration_list close_block  */
                                                                      {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 743: /* $@48: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 744: /* $@49: %empty  */
                         {
        if ( (yyvsp[0].pStructure) ) {
            (yyvsp[0].pStructure)->isClass = (yyvsp[-3].i)==CorS_Class || (yyvsp[-3].i)==CorS_ClassTemplate;
            (yyvsp[0].pStructure)->isTemplate = (yyvsp[-3].i)==CorS_ClassTemplate || (yyvsp[-3].i)==CorS_StructTemplate;
            (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b);
        }
    }
    break;

  case 745: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@48 structure_name $@49 optional_struct_variable_declaration_list  */
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

  case 746: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 747: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 748: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 749: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 750: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "$i" '(' expr ')'  */
                                                                           {
        (yyvsp[-5].pNameWithPosList)->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = (yyvsp[-5].pNameWithPosList);
    }
    break;

  case 751: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 752: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 753: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 754: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 755: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 756: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 757: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 758: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 759: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 760: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 761: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 762: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 763: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 764: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 765: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 766: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 767: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 768: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 769: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 770: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 771: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 772: /* basic_type_declaration: "float16"  */
                        { (yyval.type) = Type::tFloat16; }
    break;

  case 773: /* basic_type_declaration: "half2"  */
                        { (yyval.type) = Type::tHalf2; }
    break;

  case 774: /* basic_type_declaration: "half3"  */
                        { (yyval.type) = Type::tHalf3; }
    break;

  case 775: /* basic_type_declaration: "half4"  */
                        { (yyval.type) = Type::tHalf4; }
    break;

  case 776: /* basic_type_declaration: "half8"  */
                        { (yyval.type) = Type::tHalf8; }
    break;

  case 777: /* basic_type_declaration: "short2"  */
                        { (yyval.type) = Type::tShort2; }
    break;

  case 778: /* basic_type_declaration: "short3"  */
                        { (yyval.type) = Type::tShort3; }
    break;

  case 779: /* basic_type_declaration: "short4"  */
                        { (yyval.type) = Type::tShort4; }
    break;

  case 780: /* basic_type_declaration: "short8"  */
                        { (yyval.type) = Type::tShort8; }
    break;

  case 781: /* basic_type_declaration: "ushort2"  */
                        { (yyval.type) = Type::tUShort2; }
    break;

  case 782: /* basic_type_declaration: "ushort3"  */
                        { (yyval.type) = Type::tUShort3; }
    break;

  case 783: /* basic_type_declaration: "ushort4"  */
                        { (yyval.type) = Type::tUShort4; }
    break;

  case 784: /* basic_type_declaration: "ushort8"  */
                        { (yyval.type) = Type::tUShort8; }
    break;

  case 785: /* basic_type_declaration: "byte2"  */
                        { (yyval.type) = Type::tByte2; }
    break;

  case 786: /* basic_type_declaration: "byte3"  */
                        { (yyval.type) = Type::tByte3; }
    break;

  case 787: /* basic_type_declaration: "byte4"  */
                        { (yyval.type) = Type::tByte4; }
    break;

  case 788: /* basic_type_declaration: "byte8"  */
                        { (yyval.type) = Type::tByte8; }
    break;

  case 789: /* basic_type_declaration: "byte16"  */
                        { (yyval.type) = Type::tByte16; }
    break;

  case 790: /* basic_type_declaration: "ubyte2"  */
                        { (yyval.type) = Type::tUByte2; }
    break;

  case 791: /* basic_type_declaration: "ubyte3"  */
                        { (yyval.type) = Type::tUByte3; }
    break;

  case 792: /* basic_type_declaration: "ubyte4"  */
                        { (yyval.type) = Type::tUByte4; }
    break;

  case 793: /* basic_type_declaration: "ubyte8"  */
                        { (yyval.type) = Type::tUByte8; }
    break;

  case 794: /* basic_type_declaration: "ubyte16"  */
                        { (yyval.type) = Type::tUByte16; }
    break;

  case 795: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 796: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 797: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 798: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 799: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 800: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 801: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 802: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 803: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 804: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 805: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 806: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 807: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 808: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 809: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 810: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 811: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 812: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 813: /* auto_type_declaration: "$t" '(' expr ')'  */
                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::alias, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = "``MACRO``TAG``";
        (yyval.pTypeDecl)->isTag = true;
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner, (yylsp[-1]));
        (yyval.pTypeDecl)->firstType->typeMacroExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 814: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 815: /* bitfield_bits: bitfield_bits semicolon "name"  */
                                                 {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 816: /* bitfield_bits: bitfield_bits ',' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 817: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<tuple<string,Expression *>>();
        (yyval.pNameExprList) = pSL;

    }
    break;

  case 818: /* bitfield_alias_bits: bitfield_alias_bits semicolon  */
                                            {
        (yyval.pNameExprList) = (yyvsp[-1].pNameExprList);
    }
    break;

  case 819: /* bitfield_alias_bits: bitfield_alias_bits "name" semicolon  */
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

  case 820: /* bitfield_alias_bits: bitfield_alias_bits "name" '=' expr semicolon  */
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

  case 821: /* bitfield_basic_type_declaration: %empty  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 822: /* bitfield_basic_type_declaration: ':' "uint8"  */
                             { (yyval.type) = Type::tBitfield8; }
    break;

  case 823: /* bitfield_basic_type_declaration: ':' "uint16"  */
                             { (yyval.type) = Type::tBitfield16; }
    break;

  case 824: /* bitfield_basic_type_declaration: ':' "uint"  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 825: /* bitfield_basic_type_declaration: ':' "uint64"  */
                             { (yyval.type) = Type::tBitfield64; }
    break;

  case 826: /* bitfield_type_declaration: "bitfield" bitfield_basic_type_declaration '<' '>'  */
                                                                          {
            (yyval.pTypeDecl) = new TypeDecl((yyvsp[-2].type), tokAt(scanner,(yyloc)));
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-2]));
    }
    break;

  case 827: /* $@50: %empty  */
                                                                     { yyextra->das_arrow_depth ++; }
    break;

  case 828: /* $@51: %empty  */
                                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 829: /* bitfield_type_declaration: "bitfield" bitfield_basic_type_declaration '<' $@50 bitfield_bits '>' $@51  */
                                                                                                                                                             {
            (yyval.pTypeDecl) = new TypeDecl((yyvsp[-5].type), tokAt(scanner,(yyloc)));
            (yyval.pTypeDecl)->argNames = *(yyvsp[-2].pNameList);
            auto maxBits = (yyval.pTypeDecl)->maxBitfieldBits();
            if ( (yyval.pTypeDecl)->argNames.size()>maxBits ) {
                das_yyerror(scanner,"only " + to_string(maxBits) + " different bits are allowed in a bitfield",tokAt(scanner,(yylsp[-5])),
                    CompilationError::exceeds_bitfield);
            }
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
            delete (yyvsp[-2].pNameList);
    }
    break;

  case 832: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
        (yyval.aTypePair).secondType->at = (yyval.aTypePair).firstType->at;
    }
    break;

  case 833: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 834: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = appendDimExpr(nullptr, (yyvsp[-1].pExpression), tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 835: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = appendDimExpr((yyvsp[-3].pTypeDecl), (yyvsp[-1].pExpression), tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 836: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type), tokAt(scanner,(yyloc))); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 837: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 838: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 839: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 840: /* type_declaration_no_options: type_declaration_no_options dim_list  */
                                                                {
        if ( (yyvsp[-1].pTypeDecl)->baseType==Type::typeDecl ) {
            das_yyerror(scanner,"type declaration can`t be used as array base type",tokAt(scanner,(yylsp[-1])),
                CompilationError::invalid_array_type);
        } else if ( (yyvsp[-1].pTypeDecl)->baseType==Type::typeMacro ) {
            das_yyerror(scanner,"macro can`t be used as array base type",tokAt(scanner,(yylsp[-1])),
                CompilationError::invalid_array_type);
        }
        (yyval.pTypeDecl) = attachDimChain((yyvsp[0].pTypeDecl), (yyvsp[-1].pTypeDecl));
    }
    break;

  case 841: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyval.pTypeDecl) = appendAutoDim((yyvsp[-2].pTypeDecl), tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 842: /* $@52: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 843: /* $@53: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 844: /* type_declaration_no_options: "type" '<' $@52 type_declaration '>' $@53  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 845: /* type_declaration_no_options: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->typeMacroExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 846: /* type_declaration_no_options: '$' name_in_namespace optional_expr_list_in_braces  */
                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-1]), (yylsp[0]));
        (yyval.pTypeDecl)->typeMacroExpr = sequenceToList((yyvsp[0].pExpression));
        (yyval.pTypeDecl)->typeMacroExpr.insert((yyval.pTypeDecl)->typeMacroExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-1])), *(yyvsp[-1].s)));
        delete (yyvsp[-1].s);
    }
    break;

  case 847: /* $@54: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 848: /* $@55: %empty  */
                                                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 849: /* type_declaration_no_options: '$' name_in_namespace '<' $@54 type_declaration_no_options_list '>' $@55 optional_expr_list_in_braces  */
                                                                                                                                                                                              {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-6]), (yylsp[0]));
        (yyval.pTypeDecl)->typeMacroExpr = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        (yyval.pTypeDecl)->typeMacroExpr.insert((yyval.pTypeDecl)->typeMacroExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-6])), *(yyvsp[-6].s)));
        delete (yyvsp[-6].s);
    }
    break;

  case 850: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 851: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 852: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 853: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 854: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 855: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 856: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 857: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 858: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 859: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 860: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 861: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 862: /* $@56: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 863: /* $@57: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 864: /* type_declaration_no_options: "smart_ptr" '<' $@56 type_declaration '>' $@57  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 865: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tPointer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 866: /* $@58: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 867: /* $@59: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 868: /* type_declaration_no_options: "array" '<' $@58 type_declaration '>' $@59  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 869: /* $@60: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 870: /* $@61: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 871: /* type_declaration_no_options: "table" '<' $@60 table_type_pair '>' $@61  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].aTypePair).firstType;
        (yyval.pTypeDecl)->secondType = (yyvsp[-2].aTypePair).secondType;
    }
    break;

  case 872: /* $@62: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 873: /* $@63: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 874: /* type_declaration_no_options: "iterator" '<' $@62 type_declaration '>' $@63  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 875: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = (yyval.pTypeDecl)->at;
    }
    break;

  case 876: /* $@64: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 877: /* $@65: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 878: /* type_declaration_no_options: "block" '<' $@64 type_declaration '>' $@65  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 879: /* $@66: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 880: /* $@67: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 881: /* type_declaration_no_options: "block" '<' $@66 optional_function_argument_list optional_function_type '>' $@67  */
                                                                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
    break;

  case 882: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = (yyval.pTypeDecl)->at;
    }
    break;

  case 883: /* $@68: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 884: /* $@69: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 885: /* type_declaration_no_options: "function" '<' $@68 type_declaration '>' $@69  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 886: /* $@70: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 887: /* $@71: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 888: /* type_declaration_no_options: "function" '<' $@70 optional_function_argument_list optional_function_type '>' $@71  */
                                                                                                                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
    break;

  case 889: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = (yyval.pTypeDecl)->at;
    }
    break;

  case 890: /* $@72: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 891: /* $@73: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 892: /* type_declaration_no_options: "lambda" '<' $@72 type_declaration '>' $@73  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 893: /* $@74: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 894: /* $@75: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 895: /* type_declaration_no_options: "lambda" '<' $@74 optional_function_argument_list optional_function_type '>' $@75  */
                                                                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
    break;

  case 896: /* $@76: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 897: /* $@77: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 898: /* type_declaration_no_options: "tuple" '<' $@76 tuple_type_list '>' $@77  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 899: /* $@78: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 900: /* $@79: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 901: /* type_declaration_no_options: "variant" '<' $@78 variant_type_list '>' $@79  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 902: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 903: /* type_declaration: type_declaration '|' type_declaration_no_options  */
                                                                     {
        if ( (yyvsp[-2].pTypeDecl)->baseType==Type::option ) {
            (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
            (yyval.pTypeDecl)->argTypes.push_back((yyvsp[0].pTypeDecl));
        } else {
            (yyval.pTypeDecl) = new TypeDecl(Type::option, tokAt(scanner,(yyloc)));
            (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
            (yyval.pTypeDecl)->argTypes.push_back((yyvsp[-2].pTypeDecl));
            (yyval.pTypeDecl)->argTypes.push_back((yyvsp[0].pTypeDecl));
        }
    }
    break;

  case 904: /* type_declaration: type_declaration '|' '#'  */
                                             {
        if ( (yyvsp[-2].pTypeDecl)->baseType==Type::option ) {
            (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
            (yyval.pTypeDecl)->argTypes.push_back(new TypeDecl(*(yyvsp[-2].pTypeDecl)->argTypes.back()));
            (yyvsp[-2].pTypeDecl)->argTypes.back()->temporary ^= true;
        } else {
            (yyval.pTypeDecl) = new TypeDecl(Type::option, tokAt(scanner,(yyloc)));
            (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
            (yyval.pTypeDecl)->argTypes.push_back((yyvsp[-2].pTypeDecl));
            (yyval.pTypeDecl)->argTypes.push_back(new TypeDecl(*(yyvsp[-2].pTypeDecl)));
            (yyval.pTypeDecl)->argTypes.back()->temporary ^= true;
        }
    }
    break;

  case 905: /* $@80: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 906: /* $@81: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 907: /* $@82: %empty  */
                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 908: /* $@83: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 909: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@80 "name" $@81 open_block $@82 tuple_alias_type_list $@83 close_block  */
                  {
        auto vtype = new TypeDecl(Type::tTuple, tokAt(scanner,(yyloc)));
        vtype->alias = *(yyvsp[-6].s);
        vtype->at = tokAt(scanner,(yylsp[-6]));
        vtype->isPrivateAlias = !(yyvsp[-8].b);
        varDeclToTypeDecl(scanner, vtype, (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        if ( !yyextra->g_Program->addAlias(vtype) ) {
            das_yyerror(scanner,"type alias is already defined "+*(yyvsp[-6].s),tokAt(scanner,(yylsp[-6])),
                CompilationError::already_declared_type_alias);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTuple((yyvsp[-6].s)->c_str(),atvname);
        }
        delete (yyvsp[-6].s);
    }
    break;

  case 910: /* $@84: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 911: /* $@85: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 912: /* $@86: %empty  */
                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 913: /* $@87: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 914: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@84 "name" $@85 open_block $@86 variant_alias_type_list $@87 close_block  */
                  {
        auto vtype = new TypeDecl(Type::tVariant, tokAt(scanner,(yyloc)));
        vtype->alias = *(yyvsp[-6].s);
        vtype->at = tokAt(scanner,(yylsp[-6]));
        vtype->isPrivateAlias = !(yyvsp[-8].b);
        varDeclToTypeDecl(scanner, vtype, (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
        if ( !yyextra->g_Program->addAlias(vtype) ) {
            das_yyerror(scanner,"type alias is already defined "+*(yyvsp[-6].s),tokAt(scanner,(yylsp[-6])),
                CompilationError::already_declared_type_alias);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-6]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariant((yyvsp[-6].s)->c_str(),atvname);
        }
        delete (yyvsp[-6].s);
    }
    break;

  case 915: /* $@88: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 916: /* $@89: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 917: /* $@90: %empty  */
                                                            {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 918: /* $@91: %empty  */
                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-5]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 919: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@88 "name" $@89 bitfield_basic_type_declaration open_block $@90 bitfield_alias_bits $@91 close_block  */
                  {
        auto btype = new TypeDecl((yyvsp[-5].type), tokAt(scanner,(yyloc)));
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
                CompilationError::exceeds_bitfield);
        }
        for ( auto & p : *(yyvsp[-2].pNameExprList) ) {
            if ( get<1>(p) ) {
                ast_globalBitfieldConst ( scanner, btype, (yyvsp[-9].b), get<0>(p), get<1>(p) );
            }
        }
        if ( !yyextra->g_Program->addAlias(btype) ) {
            das_yyerror(scanner,"type alias is already defined "+*(yyvsp[-7].s),tokAt(scanner,(yylsp[-7])),
                CompilationError::already_declared_type_alias);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-7]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfield((yyvsp[-7].s)->c_str(),atvname);
        }
        delete (yyvsp[-7].s);
        delete (yyvsp[-2].pNameExprList);
    }
    break;

  case 920: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 921: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 922: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 923: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 924: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 925: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 926: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 927: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 928: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 929: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 930: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 931: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 932: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 933: /* make_variant_dim: %empty  */
       {
        (yyval.pExpression) = ast_makeStructToMakeVariant(nullptr, LineInfo());
    }
    break;

  case 934: /* make_variant_dim: make_struct_fields  */
                              {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 935: /* make_struct_single: make_struct_fields optional_comma  */
                                               {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 936: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 937: /* make_struct_dim: make_struct_dim "end of expression" make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 938: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 939: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 940: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 941: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 942: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 943: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 944: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 945: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 958: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 959: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 960: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 961: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = (yyvsp[-2].pTypeDecl);
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 962: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = (yyvsp[-4].pTypeDecl);
        msd->useInitializer = true;
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = msd;
    }
    break;

  case 963: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-5].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 964: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back((yyvsp[-2].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 965: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 966: /* $@92: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 967: /* $@93: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 968: /* make_struct_decl: "struct" '<' $@92 type_declaration_no_options '>' $@93 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 969: /* $@94: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 970: /* $@95: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 971: /* make_struct_decl: "class" '<' $@94 type_declaration_no_options '>' $@95 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                           {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 972: /* $@96: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 973: /* $@97: %empty  */
                                                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 974: /* make_struct_decl: "variant" '<' $@96 variant_type_list '>' $@97 '(' use_initializer make_variant_dim ')'  */
                                                                                                                                                                                                                        {
        auto mkt = new TypeDecl(Type::tVariant, tokAt(scanner,(yylsp[-9])));
        varDeclToTypeDecl(scanner, mkt, (yyvsp[-6].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-6].pVarDeclList));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = mkt;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 975: /* $@98: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 976: /* $@99: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 977: /* make_struct_decl: "default" '<' $@98 type_declaration_no_options '>' $@99 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = (yyvsp[-3].pTypeDecl);
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 978: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 979: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 980: /* make_tuple: make_tuple ',' expr  */
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

  case 981: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 982: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 983: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pExpression) = mkt;
    }
    break;

  case 984: /* $@100: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 985: /* $@101: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 986: /* make_tuple_call: "tuple" '<' $@100 tuple_type_list '>' $@101 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                 {
        auto mkt = new TypeDecl(Type::tTuple, tokAt(scanner,(yylsp[-9])));
        varDeclToTypeDecl(scanner, mkt, (yyvsp[-6].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-6].pVarDeclList));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = mkt;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 987: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mka;
    }
    break;

  case 988: /* make_dim: make_dim "end of expression" make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 989: /* make_dim_decl: '[' optional_expr_list ']'  */
                                                    {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-2])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
            mka->gen2 = true;
            auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),"to_array_move");
            tam->arguments.push_back(mka);
            (yyval.pExpression) = tam;
        } else {
            auto mks = new ExprMakeStruct();
            mks->at = tokAt(scanner,(yylsp[-2]));
            mks->makeType = new TypeDecl(Type::tArray, mks->at);
            mks->makeType->firstType = new TypeDecl(Type::autoinfer, mks->at);
            mks->useInitializer = true;
            mks->alwaysUseInitializer = true;
            (yyval.pExpression) = mks;
        }
    }
    break;

  case 990: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-2].pTypeDecl);
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 991: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-2].pTypeDecl);
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 992: /* $@102: %empty  */
                                       { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 993: /* $@103: %empty  */
                                                                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 994: /* make_dim_decl: "array" "struct" '<' $@102 type_declaration_no_options '>' $@103 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 995: /* $@104: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 996: /* $@105: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 997: /* make_dim_decl: "array" "tuple" '<' $@104 tuple_type_list '>' $@105 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                     {
        auto mkt = new TypeDecl(Type::tTuple, tokAt(scanner,(yylsp[-10])));
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

  case 998: /* $@106: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 999: /* $@107: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 1000: /* make_dim_decl: "array" "variant" '<' $@106 variant_type_list '>' $@107 '(' make_variant_dim ')'  */
                                                                                                                                                                      {
        auto mkt = new TypeDecl(Type::tVariant, tokAt(scanner,(yylsp[-9])));
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

  case 1001: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
                                                                   {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        mka->gen2 = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 1002: /* $@108: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 1003: /* $@109: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 1004: /* make_dim_decl: "array" '<' $@108 type_declaration_no_options '>' $@109 '(' optional_expr_list ')'  */
                                                                                                                                                                        {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-8])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = (yyvsp[-5].pTypeDecl);
            mka->gen2 = true;
            auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-8])),"to_array_move");
            tam->arguments.push_back(mka);
            (yyval.pExpression) = tam;
        } else {
            auto msd = new ExprMakeStruct();
            msd->at = tokAt(scanner,(yylsp[-8]));
            msd->makeType = new TypeDecl(Type::tArray, msd->at);
            msd->makeType->firstType = (yyvsp[-5].pTypeDecl);
            msd->at = tokAt(scanner,(yylsp[-5]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 1005: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 1006: /* $@110: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 1007: /* $@111: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 1008: /* make_dim_decl: "fixed_array" '<' $@110 type_declaration_no_options '>' $@111 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = (yyvsp[-6].pTypeDecl);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 1009: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mka;
    }
    break;

  case 1010: /* make_table: make_table "end of expression" make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 1011: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 1012: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 1013: /* make_table_decl: "begin of code block" optional_expr_map_tuple_list "end of code block"  */
                                                              {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-2])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
            auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),"to_table_move");
            ttm->arguments.push_back(mka);
            (yyval.pExpression) = ttm;
        } else {
            auto mks = new ExprMakeStruct();
            mks->at = tokAt(scanner,(yylsp[-2]));
            mks->makeType = new TypeDecl(Type::tTable, tokAt(scanner,(yyloc)));
            mks->makeType->firstType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-2])));
            mks->makeType->secondType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[0])));
            mks->useInitializer = true;
            mks->alwaysUseInitializer = true;
            (yyval.pExpression) = mks;
        }
    }
    break;

  case 1014: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
                                                                          {
        auto mkt = new TypeDecl(Type::tFixedArray, tokAt(scanner,(yyloc)));
        mkt->fixedDim = TypeDecl::dimAuto;
        mkt->at = tokAt(scanner,(yylsp[-2]));
        mkt->firstType = new TypeDecl(Type::autoinfer, mkt->at);
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = mkt;
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-2]));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),"to_table_move");
        ttm->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = ttm;
    }
    break;

  case 1015: /* make_table_decl: "table" '(' optional_expr_map_tuple_list ')'  */
                                                                       {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 1016: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
                                                                                                                 {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-6])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = (yyvsp[-4].pTypeDecl);
            auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),"to_table_move");
            ttm->arguments.push_back(mka);
            (yyval.pExpression) = ttm;
        } else {
            auto msd = new ExprMakeStruct();
            msd->at = tokAt(scanner,(yylsp[-6]));
            msd->makeType = new TypeDecl(Type::tTable, msd->at);
            msd->makeType->firstType = (yyvsp[-4].pTypeDecl);
            msd->makeType->secondType = new TypeDecl(Type::tVoid, tokAt(scanner,(yylsp[-6])));
            msd->at = tokAt(scanner,(yylsp[-6]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 1017: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
                                                                                                                                                             {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-8])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = new TypeDecl(Type::tTuple, tokAt(scanner,(yyloc)));
            mka->makeType->argTypes.push_back((yyvsp[-6].pTypeDecl));
            mka->makeType->argTypes.push_back((yyvsp[-4].pTypeDecl));
            auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-8])),"to_table_move");
            ttm->arguments.push_back(mka);
            (yyval.pExpression) = ttm;
        } else {
            auto msd = new ExprMakeStruct();
            msd->at = tokAt(scanner,(yylsp[-8]));
            msd->makeType = new TypeDecl(Type::tTable, msd->at);
            msd->makeType->firstType = (yyvsp[-6].pTypeDecl);
            msd->makeType->secondType = (yyvsp[-4].pTypeDecl);
            msd->at = tokAt(scanner,(yylsp[-8]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 1018: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 1019: /* array_comprehension_where: "end of expression" "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 1020: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 1021: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 1022: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 1023: /* array_comprehension: '[' "iterator" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                                 {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 1024: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true,false);
    }
    break;

  case 1025: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where "end of code block" ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),false,false);
    }
    break;

  case 1026: /* array_comprehension: "begin of code block" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                              {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
    }
    break;

  case 1027: /* array_comprehension: "{{" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block" "end of code block"  */
                                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true,true);
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
                CompilationError::invalid_expression);
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
