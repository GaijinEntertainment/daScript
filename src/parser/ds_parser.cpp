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
#define YYLAST   16751

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  247
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  309
/* YYNRULES -- Number of rules.  */
#define YYNRULES  1031
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1864

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
    1011,  1012,  1013,  1017,  1022,  1030,  1031,  1032,  1036,  1037,
    1038,  1039,  1040,  1041,  1042,  1043,  1044,  1045,  1046,  1047,
    1048,  1049,  1050,  1051,  1052,  1058,  1061,  1067,  1070,  1073,
    1079,  1080,  1081,  1082,  1086,  1103,  1125,  1128,  1138,  1153,
    1168,  1183,  1186,  1193,  1197,  1204,  1205,  1209,  1210,  1211,
    1215,  1219,  1223,  1230,  1234,  1235,  1236,  1237,  1238,  1239,
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
    1360,  1361,  1362,  1363,  1364,  1368,  1369,  1373,  1392,  1393,
    1394,  1398,  1404,  1404,  1422,  1423,  1426,  1427,  1430,  1434,
    1445,  1454,  1463,  1469,  1470,  1471,  1472,  1473,  1474,  1475,
    1476,  1477,  1478,  1479,  1480,  1481,  1482,  1483,  1484,  1485,
    1486,  1487,  1488,  1489,  1493,  1498,  1504,  1510,  1521,  1522,
    1526,  1527,  1531,  1532,  1536,  1540,  1547,  1547,  1547,  1553,
    1553,  1553,  1562,  1596,  1599,  1602,  1605,  1611,  1612,  1623,
    1627,  1630,  1638,  1638,  1638,  1641,  1647,  1650,  1654,  1658,
    1665,  1672,  1678,  1682,  1686,  1689,  1692,  1700,  1703,  1706,
    1714,  1717,  1725,  1728,  1731,  1739,  1745,  1746,  1747,  1751,
    1752,  1756,  1757,  1761,  1766,  1774,  1780,  1786,  1792,  1798,
    1806,  1814,  1822,  1833,  1836,  1842,  1842,  1842,  1845,  1845,
    1845,  1850,  1850,  1850,  1858,  1858,  1858,  1864,  1874,  1885,
    1898,  1908,  1919,  1934,  1937,  1943,  1944,  1952,  1964,  1965,
    1966,  1970,  1971,  1972,  1973,  1974,  1978,  1983,  1991,  1992,
    1993,  1997,  2002,  2009,  2016,  2016,  2025,  2026,  2027,  2028,
    2029,  2030,  2031,  2032,  2033,  2037,  2038,  2039,  2040,  2041,
    2042,  2043,  2044,  2045,  2046,  2047,  2048,  2049,  2050,  2051,
    2052,  2053,  2054,  2055,  2059,  2060,  2061,  2062,  2067,  2068,
    2069,  2070,  2071,  2072,  2073,  2074,  2075,  2076,  2077,  2078,
    2079,  2080,  2081,  2082,  2083,  2088,  2094,  2105,  2111,  2122,
    2126,  2133,  2136,  2136,  2136,  2141,  2141,  2141,  2154,  2158,
    2162,  2168,  2176,  2184,  2190,  2198,  2198,  2198,  2205,  2209,
    2218,  2226,  2234,  2238,  2241,  2247,  2248,  2249,  2250,  2251,
    2252,  2253,  2254,  2255,  2256,  2257,  2258,  2259,  2260,  2261,
    2262,  2263,  2264,  2265,  2266,  2267,  2268,  2269,  2270,  2271,
    2272,  2273,  2274,  2275,  2276,  2277,  2278,  2279,  2280,  2281,
    2282,  2288,  2289,  2290,  2291,  2292,  2307,  2316,  2317,  2318,
    2319,  2320,  2321,  2322,  2323,  2324,  2325,  2326,  2327,  2330,
    2333,  2334,  2337,  2337,  2337,  2340,  2345,  2349,  2353,  2353,
    2353,  2358,  2361,  2365,  2365,  2365,  2370,  2373,  2374,  2375,
    2376,  2377,  2378,  2379,  2380,  2381,  2383,  2387,  2388,  2393,
    2397,  2398,  2399,  2400,  2401,  2402,  2403,  2407,  2411,  2415,
    2419,  2423,  2427,  2431,  2435,  2439,  2446,  2447,  2448,  2452,
    2453,  2454,  2458,  2459,  2463,  2464,  2465,  2469,  2470,  2474,
    2485,  2488,  2491,  2491,  2495,  2495,  2514,  2513,  2529,  2528,
    2542,  2551,  2563,  2572,  2582,  2583,  2584,  2585,  2586,  2590,
    2593,  2602,  2603,  2607,  2610,  2613,  2628,  2637,  2638,  2642,
    2645,  2648,  2661,  2662,  2666,  2671,  2676,  2684,  2687,  2694,
    2697,  2703,  2704,  2705,  2709,  2710,  2714,  2721,  2726,  2735,
    2741,  2745,  2756,  2760,  2766,  2772,  2780,  2791,  2794,  2797,
    2797,  2817,  2818,  2822,  2823,  2824,  2828,  2831,  2831,  2850,
    2853,  2856,  2871,  2890,  2891,  2892,  2897,  2897,  2923,  2924,
    2928,  2929,  2929,  2933,  2934,  2935,  2939,  2949,  2954,  2949,
    2966,  2971,  2966,  2986,  2987,  2991,  2992,  2996,  3002,  3003,
    3004,  3005,  3009,  3010,  3011,  3015,  3018,  3024,  3029,  3024,
    3049,  3056,  3061,  3070,  3076,  3080,  3091,  3092,  3093,  3094,
    3095,  3096,  3097,  3098,  3099,  3100,  3101,  3102,  3103,  3104,
    3105,  3106,  3107,  3108,  3109,  3110,  3111,  3112,  3113,  3114,
    3115,  3116,  3117,  3118,  3119,  3120,  3121,  3122,  3123,  3124,
    3125,  3126,  3127,  3128,  3129,  3130,  3131,  3132,  3133,  3134,
    3135,  3136,  3137,  3138,  3139,  3140,  3144,  3145,  3146,  3147,
    3148,  3149,  3150,  3151,  3155,  3166,  3170,  3177,  3189,  3196,
    3202,  3211,  3216,  3219,  3229,  3242,  3243,  3244,  3245,  3246,
    3250,  3254,  3254,  3254,  3268,  3269,  3273,  3278,  3285,  3288,
    3294,  3295,  3296,  3297,  3298,  3308,  3311,  3311,  3311,  3315,
    3320,  3327,  3327,  3327,  3334,  3338,  3342,  3347,  3352,  3357,
    3362,  3366,  3370,  3375,  3379,  3383,  3388,  3388,  3388,  3394,
    3401,  3401,  3401,  3406,  3406,  3406,  3412,  3412,  3412,  3417,
    3423,  3423,  3423,  3428,  3428,  3428,  3437,  3443,  3443,  3443,
    3448,  3448,  3448,  3457,  3463,  3463,  3463,  3468,  3468,  3468,
    3477,  3477,  3477,  3483,  3483,  3483,  3492,  3495,  3506,  3522,
    3522,  3527,  3532,  3522,  3557,  3557,  3562,  3568,  3557,  3593,
    3593,  3598,  3603,  3593,  3643,  3644,  3645,  3646,  3647,  3651,
    3658,  3665,  3671,  3677,  3684,  3691,  3697,  3706,  3709,  3715,
    3723,  3728,  3735,  3740,  3747,  3752,  3758,  3759,  3763,  3764,
    3769,  3770,  3774,  3775,  3779,  3780,  3784,  3785,  3786,  3790,
    3791,  3792,  3796,  3797,  3801,  3807,  3814,  3822,  3829,  3837,
    3846,  3846,  3846,  3854,  3854,  3854,  3861,  3861,  3861,  3871,
    3871,  3871,  3882,  3885,  3891,  3905,  3911,  3917,  3923,  3923,
    3923,  3936,  3941,  3948,  3967,  3972,  3979,  3979,  3979,  3989,
    3989,  3989,  4002,  4002,  4002,  4015,  4024,  4024,  4024,  4044,
    4051,  4051,  4051,  4061,  4066,  4073,  4076,  4082,  4101,  4112,
    4120,  4140,  4165,  4166,  4170,  4171,  4176,  4179,  4182,  4185,
    4188,  4191
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

#define YYPACT_NINF (-1601)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-898)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1601,   849, -1601, -1601,    39,    11,   278,   326, -1601,   -16,
     744,   744,   744, -1601, -1601,   -71,    28, -1601, -1601,   359,
   -1601, -1601, -1601, -1601,   328, -1601,   154, -1601, -1601, -1601,
   -1601, -1601, -1601, -1601, -1601, -1601, -1601,   -96, -1601,    34,
      77,   129, -1601,   132, -1601, -1601, -1601,   360,   143, -1601,
      24, -1601, -1601, -1601,   744,   744, -1601, -1601,   154, -1601,
   -1601, -1601, -1601, -1601,   176,   261, -1601, -1601, -1601, -1601,
      28,    28,    28,   201, -1601,   864,   110, -1601, -1601,   341,
     349,   351,   798,   817, -1601,   819,   510,    39,   375,    11,
     278,   278,   368,   278,   465, -1601,   820,   820, -1601,   480,
     359,    85,   359,   821,   487,   519,   561, -1601,   579,   559,
   -1601, -1601,   -79,    39,    28,    28,    28,    28, -1601, -1601,
   -1601, -1601,   851, -1601, -1601,   618, -1601, -1601, -1601, -1601,
   -1601,   326, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601,
   -1601,   846,   682,   641,   546, -1601, -1601, -1601, -1601,   368,
     368,   368,   771, -1601, -1601, -1601, -1601, -1601, -1601, -1601,
     359, -1601, -1601, -1601,   605, -1601, -1601, -1601, -1601, -1601,
     683, -1601,   -97, -1601,    42,   720,   864, -1601, -1601, -1601,
   -1601, -1601,   251,   774, -1601,  -100, -1601, -1601, -1601,   868,
   -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601,
   -1601, -1601, -1601,   701,   650, -1601, -1601,   114,   715, -1601,
     713, -1601,   852, -1601,   723,   326,   326, -1601, -1601, 16553,
     769, -1601, -1601,   722, -1601,   595,    39,    39,   151,   -78,
   -1601, -1601, -1601, -1601, -1601, -1601, -1601,   746,   641, -1601,
   -1601, 12019, -1601,   719,   326, -1601, -1601, 15197, -1601, -1601,
   -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601,
   -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601,
   -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601,
   -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601,
   -1601, -1601, -1601, -1601, -1601, -1601,   926,   927, -1601,   728,
     326, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601,   326,
   -1601,   753,   326, -1601, -1601,  -100,   259, -1601,    39, -1601,
     738,   940,   678, -1601, -1601, -1601,   762,   765,   778,   752,
     786,   788, -1601, -1601, -1601,   783, -1601, -1601, -1601, -1601,
   -1601,   216, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601,
   -1601, -1601, -1601,   790, -1601, -1601, -1601,   793,   805, -1601,
   -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601,
   -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601,
   -1601, -1601, -1601, -1601, -1601, -1601,   806,   815,   802,   -71,
   -1601, -1601, -1601, -1601, -1601,   799,   831, -1601, -1601, -1601,
   -1601, -1601, -1601, -1601,   854,   857, -1601, -1601, -1601, -1601,
   -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601,
   -1601, -1601, -1601, -1601, -1601, -1601,   860,   824, -1601, -1601,
   -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601,
    1038, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601,
   -1601, -1601, -1601,   875,   836, -1601, -1601,    -3,   -82, -1601,
   -1601, -1601,   586,   -71, -1601, -1601, -1601,   -78,   839, -1601,
   10900,   884,   231, 12019, -1601,   -66, -1601, -1601, -1601, 10900,
   -1601, -1601,   885,   856,   -85,   -47,   -38, -1601, -1601, 10900,
    -114, -1601, -1601, -1601,    17, -1601, -1601, -1601,    47,  6507,
   -1601,   845, 11669, -1601, 11845,   430, -1601, -1601, -1601, -1601,
     890,   985,  1253,   861, -1601,   194,   359,   427,   850, 12019,
   12019, -1601,  2569, -1601,   193, -1601,   472, -1601,    68, -1601,
   -1601,   872,   887, -1601, -1601,    81,   -37,   888,    44, -1601,
     307,   870,   891,   892,   871,   901,   886,   308,   903, -1601,
     471,   906,   907, 10900, 10900,   889,   894,   897,   900,   904,
     905, -1601, 11321, 11495,  6740, -1601, -1601, -1601, -1601, -1601,
   -1601, -1601, -1601,   909,   910, -1601,  6973,  7204, 10900, 10900,
   10900, 10900,  5583,  7435, -1601,   914, -1601, -1601, -1601,   -10,
   -1601, -1601, -1601, -1601,   908, -1601, -1601, -1601, -1601, -1601,
   -1601, -1601,  2303, -1601,   920, -1601, -1601, -1601, -1601, -1601,
   -1601, -1601,   921,  1091,   993, -1601, -1601, -1601,  4189, 12019,
   12019, 12019, 12363, 12019, 12019,   917,   925, 12019,   728, 12019,
     728, 12019,   728, 12193,   953, 12472, -1601, 10900, -1601, -1601,
   -1601, -1601, -1601,   911, -1601, -1601, 14744, 10900, -1601,   799,
     617,   158, -1601, -1601,   448, -1601,   831,   472,   947,   448,
   -1601,   472, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601,
   -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601,
   -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, 10900, -1601,
   -1601,   249,   135,   135,   135, -1601,   831,   831, -1601, 10900,
   -1601, -1601, -1601,  3496, -1601,   326,  7666, -1601, 10900,   966,
   -1601,   359,   978,  7897,   184,   946,  3727,   168,   168,   168,
    8128,   359,   359, -1601, 10900,  1158, -1601, -1601, -1601, -1601,
   -1601, -1601,  1134, -1601, -1601, -1601,   500, -1601,   359,   359,
     359,   359, -1601,   359, -1601, -1601,  1109, -1601,   245, -1601,
    1873,   586, 10900, -1601, -1601, -1601,    28, -1601,  1168, -1601,
    -100, -1601, -1601, -1601,   935, -1601, -1601,   -71,   504, -1601,
     955,   957,   967, -1601, 10900, 12019, 10900, 10900, -1601, -1601,
   10900, -1601, 10900, -1601, 10900, -1601, -1601, 10900, -1601, 12019,
     443,   443, 10900, 10900, 10900, 10900, 10900, 10900,   249,  2800,
     249,  3032,   249, 15452, -1601,   881, -1601, -1601,   838,   249,
     982, -1601,   976, -1601,   443,   443,    60,   443,   443,   249,
    1180,   954,   981, 15760,   960,   -19,   981,   988,   964,   363,
   -1601,  4420,    51, 16215, 16270, 10900, 10900, -1601, -1601, 10900,
   10900, 10900, 10900,  1011, 10900,   406, 10900, 10900, 10900, 10900,
   10900, 10900, 10900, 10900, 10900,  8359, 10900, 10900, 10900, 10900,
   10900, 10900, 10900, 10900, 10900, 10900, 16452, 10900, -1601,  8590,
   10900,  1014, -1601,  4189, -1601,  1061, 12194,   290,   582,   996,
     242, -1601,   699,   734, -1601, -1601,  1024,   740,   -82,   749,
     -82,   757,   -82, -1601,   -92, -1601,   198, -1601, 12019,  1008,
   -1601, -1601, 14779,   364, -1601,   472, 12019, -1601, -1601, 12019,
   -1601, -1601, 12507,   984,  1186, -1601, -1601,   199, -1601, -1601,
   -1601, 15241,   249,  4189, -1601,  1018, 12328,  1216, 10900, 15760,
    1037, 15241,  1019, -1601,  1022,  1049, 15760, -1601, 12019,  4189,
   -1601, 12328,   999, -1601,   908, -1601, -1601, -1601, 15241, -1601,
   -1601, 15241, -1601,   326, -1601, -1601, -1601, -1601, -1601, -1601,
   -1601, -1601,   152,   168, -1601,  4651,  4651,  4651,  4651,  4651,
    4651,  4651,  4651,  4651,  4651,  4651, 10900,  4651,  4651,  4651,
    4651,  4651,  4651, -1601,   472, 15350,  1048,   473,   853,  1178,
     359, 12019, 12019, 12019,  5814,  8821,  1050, 10900, 12019, -1601,
   -1601, -1601, 12019,   981,   107,  1004, 12614, 12019, 12019, 12649,
   12019, 12756, 12019,   981, 12019, 12193,   981,   953,   703, 12791,
   12898, 12933, 13040, 13075, 13182,    27,   168,  3264,  4884,  6045,
   15492,  1035,    21,   102,  1036,  -103,    33,  6276,    21,   794,
      37, 10900,  1044, 10900, -1601, -1601, 12019, 12019, -1601, 10900,
     527,    41, -1601, 10900, -1601,    55,   249, -1601, 10900, -1601,
   10900, -1601, 10900, -1601, 10900,  1012,   550, -1601, -1601,  1017,
    1020,   -27, -1601, -1601,   139,  5117, -1601,   229,  1013,  1021,
     219,   728,  1031,  1023, -1601, -1601,  1041,  1025, -1601, -1601,
    1047,  1047,  1835,  1835, 11488, 11488,  1026,   214,  1028, -1601,
   14886,   -49,   -49,   920,  1047,  1047, 15950, 15894, 15989, 15855,
   16398, 15587, 16045,  1623, 11314,  1835,  1835,   945,   945,   214,
     214,   214,   614, 10900,  1029,  1039,   616, 10900,  1265,  1040,
   14921, -1601,   387, 13217, -1601, -1601, 12194, 10900, 10900, 10900,
   10900, 10900, 10900, 10900, 10900, 10900, 10900, 10900, 10900, 10900,
   10900, 10900, 10900, 10900, -1601, -1601, -1601, -1601, 12019, -1601,
   -1601, -1601,   356, -1601,  1053, -1601,  1056, -1601,  1064, -1601,
   12193, -1601,   953,   390,   831, -1601,  1042, -1601, 10900, -1601,
   -1601,   831,   831, -1601, 10900,  1093,   418, 12019, -1601, 10900,
   -1601,    58, -1601,  1018, 10900,   326, 15760,  1072, -1601, -1601,
   -1601, -1601,  1003, -1601, 12328, -1601,    51, -1601,   828, 10900,
   -1601,   908,  1094,  1094, -1601, -1601, -1601,   168,   168,   168,
   -1601, -1601,  2037, -1601,  2037, -1601,  2037, -1601,  2037, -1601,
    2037, -1601,  2037, -1601,  2037, -1601,  2037, -1601,  2037, -1601,
    2037, -1601,  2037, 15760, -1601,  2037, -1601,  2037, -1601,  2037,
   -1601,  2037, -1601,  2037, -1601,  2037, -1601, -1601,  1076,   359,
   -1601, -1601,   413, -1601,    23, -1601,  1636,  1650,   758,   645,
     389,  1051,  1052,  1099, 13324,    35, 13359,   766, 12019, 12193,
     953,  1718,  1058,  1063, 12019, -1601, -1601,  1901,  2006, -1601,
    2050, -1601,  2255,  1066,  2260,   450,  1073,   451,    51, -1601,
   -1601, -1601, -1601, -1601,  1065, 10900, -1601,  5350, 14744,    -2,
   10900,   550,   645,   102, -1601, -1601,  1077, -1601, 10900, 10900,
   -1601,  1078, -1601, 10900,   645,   692,  1080, -1601, -1601, 10900,
   15760, -1601, -1601,   502,   544, 15626, 10900, -1601, 10900,    59,
   15760, 13466, 15760, 15760, -1601,  1075,    -7, 10900, 10900, 12019,
     728,     5, -1601,  1081,   493, 11131, -1601, -1601,   219,  1108,
    1125,  1084,  1129,  1131, -1601,   508,   -82, -1601, 10900, -1601,
   10900,  9052, 10900, -1601,  1107,  1089, -1601, -1601, 10900,  1090,
   -1601, 15028, 10900,  9283,  1092, -1601, 15063, -1601,  9514, -1601,
   -1601, -1601, -1601, 15760, 15760, 15760, 15760, 15760, 15760, 15760,
   15760, 15760, 15760, 15760, 15760, 15760, 15760, 15760, 15760, 15760,
   -1601, -1601, -1601,   831, -1601, -1601,  1137, -1601,  1138, -1601,
   -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601,
   12019, -1601, 15350, 13501, -1601,  1101,  1301,   -57, 15760, 10900,
   -1601, 12019, 10900,    51,   728,   326, -1601, -1601, 10900, -1601,
    1347,  2569,    51, -1601,   517,   426, -1601, -1601, -1601, 12019,
   -1601,  1313,    23, -1601, -1601,   853, -1601, -1601, -1601,  1104,
   -1601, -1601, -1601,   563, -1601,  1150,  1110, -1601, -1601,  2268,
     584,   593, -1601, -1601, 10900,  2484, -1601, -1601, -1601, -1601,
   -1601, -1601, -1601, -1601, -1601,  1111,  9745,   721,    21,   102,
   15760,  1035, -1601, -1601, 15760,  1036, -1601,   724,    21,  1113,
   -1601, -1601, -1601, -1601,   725, -1601, -1601, -1601,  1144,   729,
     737, 10900,    88, 10900, 10900, 10900, 13608, 13643,  3642,   -82,
   -1601,  1112,  5117,   434, -1601, -1601,  1159, -1601, -1601,   219,
    1115,   136, 12019, 13750, 12019, 13785, -1601,   449, 13892, -1601,
   10900,  2200, 10900, -1601, 13927,  5117, -1601,   463, 10900, -1601,
   -1601, -1601,   464, -1601, -1601, -1601, -1601, -1601, -1601,  1116,
     831, -1601, -1601, 10900,  1162, 10900,   382,   831, 15760,   403,
     -82, -1601, 15241, -1601,   359, -1601,   728,  1163,  1121,   329,
     336, -1601, -1601,  1313,   249,  1122,  1123, -1601, -1601, 10900,
    1165,  1127, 10900, -1601, -1601, -1601, -1601,  1124,  1130,  1132,
   10900, 10900, 10900,  1135,  1297,  1139,  1140,  9976, -1601,   479,
   10900,   102, -1601, 10900,   692, -1601, 10900, 10900,  1116, -1601,
   -1601, 10900, 10900,   747, 10900, 10900, 14034, 15760, 15760, -1601,
   -1601, -1601,  1148, -1601,   525, -1601,  1151, -1601, -1601, 10207,
   -1601, -1601,  3873, -1601,   775, -1601, -1601, -1601, 12019, 14069,
   14176, -1601,   537, -1601, 14211, -1601, -1601, 14318, -1601, 15760,
   -1601, -1601,   136,   828,  3958, -1601,   -82, -1601,   474, 12019,
     -66, -1601, 16553, -1601, -1601, -1601, -1601,  1297,  1297, 14353,
    1156,  1141, 14460,  1154,  1155,  1160, 10900, -1601, 10900,  1835,
    1835,  1835, 10900, -1601, -1601,  1297,  1297, -1601, 14495, -1601,
   15721, -1601, 15721, -1601,  1185,  1835, -1601,  1196,  1185, 15721,
   10900, 15760, 15760,   142,   347, -1601,  1157, -1601, 10900, 15855,
   -1601, -1601,   776, -1601, -1601,  1161, -1601, -1601, -1601, -1601,
   10438, 10669, -1601, -1601, -1601, -1601, -1601, 15760,   326, 12019,
     -66,  1153,  4189,   359, 16553,   -68,   -68, -1601, 10900, 10900,
   -1601,  1297,  1297,   645,  1164,  1166,   981,   -68,   645, -1601,
    1348,  1169,  1197,  1202, -1601,  1203,  1173, 15721, 10900, 10900,
   -1601,   347, -1601,  2200, -1601, -1601, -1601, -1601, 10900, 10900,
   15760, -1601,  1153,  4189,  4189, -1601, 12194, -1601,   326,   645,
    1035,  1189, -1601,  1172,  1174, 14602, 14637,   -68,   -68,  1035,
    1175, -1601, -1601,  1176,  1177,  1183, 10900,  1187,  1188,  1207,
   -1601, -1601,  1191, 15760, 15760, -1601, -1601, 15760,  4189, -1601,
   12194, -1601, 12194, -1601, -1601,   482,  1179, -1601, -1601, -1601,
   -1601, -1601,  1184,  1190, -1601, -1601, -1601, -1601, 15760, -1601,
   -1601, -1601, -1601, -1601, 12194, -1601, -1601, -1601,   645, -1601,
   -1601, -1601,   485, -1601
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   155,     1,   389,     0,     0,     0,   721,   390,     0,
     713,   713,   713,    74,    75,     0,     0,    15,     3,     0,
      10,     9,     8,    16,     0,     7,   701,     6,    11,     5,
       4,    13,    12,    14,   116,   117,   115,   133,   135,    45,
      64,    61,    62,     0,    47,    48,    49,     0,     0,    50,
      59,    46,   305,   304,   713,   713,    22,    21,   701,   715,
     714,   919,   909,   914,     0,   357,    43,   141,   142,   143,
       0,     0,     0,   144,   146,   153,     0,   140,    17,   739,
     738,   295,   723,   742,   702,   703,     0,     0,     0,     0,
       0,     0,    51,     0,     0,    60,     0,     0,    57,     0,
       0,   713,     0,    18,     0,     0,     0,   359,     0,     0,
     152,   147,     0,     0,     0,     0,     0,     0,   156,   741,
     740,   296,   298,   725,   724,     0,   744,   743,   747,   705,
     704,   707,   131,   132,   121,   122,   126,   125,   127,   129,
     119,     0,     0,     0,     0,   118,   136,    65,    63,    53,
      54,    52,    59,    56,    55,   716,   718,   307,   306,   720,
       0,   722,    19,    20,    23,   920,   910,   915,   358,    41,
      44,   151,     0,   148,   149,   150,   154,   300,   299,   302,
     297,   726,     0,   735,   697,   626,    26,    27,    31,     0,
     128,   123,   124,   130,   111,   112,   101,   102,   106,   105,
     107,   109,    99,     0,     0,    98,   113,     0,     0,    58,
       0,   719,     0,    25,   825,     0,     0,    42,   145,     0,
       0,   727,   736,     0,   748,   699,     0,     0,   628,     0,
      28,    29,    30,   108,   103,   104,   110,     0,     0,   134,
     120,     0,    24,     0,     0,   911,   916,     0,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,     0,     0,   163,   157,
       0,   806,   809,   812,   813,   807,   810,   808,   811,     0,
     709,   733,   745,   698,   706,   626,     0,   137,     0,   139,
       0,   686,   684,   708,   100,   114,     0,     0,     0,     0,
       0,     0,   756,   799,   757,   815,   758,   762,   763,   764,
     765,   805,   769,   770,   771,   772,   773,   774,   775,   800,
     801,   802,   803,   879,   761,   768,   804,   886,   893,   759,
     766,   760,   767,   776,   777,   778,   779,   780,   781,   782,
     783,   784,   785,   786,   787,   788,   789,   790,   791,   792,
     793,   794,   795,   796,   797,   798,     0,     0,     0,     0,
     814,   840,   843,   841,   842,   906,   717,   828,   829,   826,
     827,   921,   663,   669,   241,   242,   239,   166,   167,   169,
     168,   170,   171,   172,   173,   199,   200,   197,   198,   190,
     201,   202,   191,   188,   189,   240,   223,     0,   238,   203,
     204,   205,   206,   177,   178,   179,   174,   175,   176,   187,
       0,   193,   194,   192,   185,   186,   181,   180,   182,   183,
     184,   165,   164,   222,     0,   195,   196,   626,   160,   334,
     303,   730,   728,     0,   737,   640,   749,     0,     0,   138,
       0,     0,     0,     0,   685,     0,   846,   870,   873,     0,
     876,   866,     0,     0,   880,   887,   894,   900,   903,     0,
     340,   856,   861,   855,     0,   869,   865,   858,     0,     0,
     860,   844,     0,   821,   912,   917,   243,   244,   237,   221,
     245,   224,   207,     0,   158,   388,   654,   655,     0,     0,
       0,   301,     0,   709,     0,   710,     0,   734,   644,   700,
     627,     0,     0,   531,   532,     0,     0,     0,     0,   525,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   805,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   615,     0,     0,     0,   446,   448,   447,   450,   451,
     452,   453,   454,     0,     0,    37,   342,     0,     0,     0,
       0,     0,   338,     0,   428,   429,   529,   528,   609,   526,
     600,   599,   598,   597,   155,   603,   527,   602,   601,   573,
     533,   574,     0,   534,     0,   530,   924,   928,   925,   926,
     927,   688,     0,   689,     0,   682,   683,   681,     0,     0,
       0,     0,     0,     0,     0,     0,   831,     0,   157,     0,
     157,     0,   157,     0,     0,     0,   851,   338,   850,   863,
     864,   857,   859,     0,   862,   845,     0,     0,   908,   907,
     922,   357,   834,   835,     0,   664,   659,     0,     0,     0,
     670,     0,   246,   226,   227,   229,   228,   230,   231,   232,
     233,   225,   234,   235,   236,   210,   211,   213,   212,   214,
     215,   216,   217,   208,   209,   218,   219,   220,     0,   386,
     387,     0,   626,   626,   626,   159,   162,   161,   336,     0,
      76,    77,    89,   374,   372,     0,     0,    96,     0,     0,
     373,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   313,     0,     0,   329,   324,   321,   320,
     322,   323,   308,   356,   335,   315,   609,   314,     0,    84,
      85,    82,   327,    83,   328,   330,   392,   319,     0,   316,
     455,   731,     0,   711,   729,   642,     0,   641,     0,   746,
     626,   970,   973,   362,   366,   365,   371,     0,     0,   414,
       0,     0,     0,  1006,     0,     0,   342,     0,   405,   408,
       0,   411,     0,  1010,     0,   979,   988,     0,   976,     0,
     561,   562,     0,     0,     0,     0,     0,     0,     0,   948,
       0,     0,     0,   986,  1013,     0,   346,   349,     0,     0,
       0,  1015,  1024,   449,   538,   537,   575,   536,   535,     0,
       0,     0,  1024,   423,     0,   357,  1024,  1024,     0,   430,
     607,     0,   438,     0,     0,     0,     0,   563,   564,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   515,     0,   687,     0,
       0,     0,   692,     0,   696,     0,   455,     0,     0,     0,
     836,   849,     0,     0,   816,   830,     0,     0,   160,     0,
     160,     0,   160,   661,     0,   667,     0,   817,     0,  1024,
     854,   838,     0,     0,   822,     0,     0,   665,   913,     0,
     671,   918,     0,     0,   750,   651,   652,   674,   656,   658,
     657,     0,     0,     0,   378,   375,   423,     0,     0,   360,
       0,     0,     0,   333,     0,     0,    68,    91,     0,     0,
     383,   380,   429,   441,   155,   355,   353,   354,     0,   331,
     332,     0,    87,     0,   444,   311,   318,   325,   326,   377,
     382,   391,     0,     0,   317,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   310,     0,     0,     0,     0,   634,   637,
       0,     0,     0,     0,   962,     0,     0,     0,     0,   996,
     999,  1002,     0,  1024,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1024,     0,     0,  1024,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     982,   940,   948,     0,   991,     0,     0,     0,   948,     0,
       0,     0,     0,     0,   951,  1018,     0,     0,    40,     0,
      38,     0,  1017,  1025,   343,     0,     0,   993,  1025,   339,
       0,   673,     0,   672,     0,     0,  1025,   939,   566,     0,
       0,   502,   499,   501,     0,   338,   518,     0,     0,     0,
       0,   157,     0,     0,   586,   585,     0,     0,   587,   591,
     539,   540,   552,   553,   550,   551,     0,   580,     0,   571,
       0,   604,   605,   606,   541,   542,   557,   558,   559,   560,
       0,     0,   555,   556,   554,   548,   549,   544,   543,   545,
     546,   547,     0,     0,     0,   508,     0,     0,     0,     0,
       0,   523,     0,     0,   691,   694,   455,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   695,   847,   871,   874,     0,   877,
     867,   818,     0,   881,     0,   888,     0,   895,     0,   901,
       0,   904,     0,     0,   344,  1025,     0,   839,     0,   823,
     923,   660,   666,   653,     0,     0,     0,     0,   675,     0,
      92,     0,   379,   376,     0,     0,   361,     0,    93,    94,
      66,    67,     0,   384,   381,   430,   438,   337,    71,     0,
     334,   155,     0,     0,   404,   403,   352,     0,     0,     0,
     477,   486,   465,   487,   466,   489,   468,   488,   467,   490,
     469,   480,   459,   481,   460,   482,   461,   491,   470,   492,
     471,   479,   457,   458,   493,   472,   494,   473,   483,   462,
     484,   463,   485,   464,   478,   456,   732,   712,     0,   156,
     635,   636,   637,   638,   629,   645,     0,     0,     0,   963,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1019,   576,     0,     0,   577,
       0,   608,     0,     0,     0,     0,     0,     0,   438,   610,
     611,   612,   613,   614,     0,     0,   949,     0,   423,   948,
       0,     0,     0,     0,   957,   958,     0,   965,     0,     0,
     955,     0,   994,     0,     0,     0,     0,   953,   995,     0,
     985,   950,  1014,     0,     0,    34,     0,  1016,     0,     0,
     424,     0,   930,   929,   565,     0,     0,     0,     0,     0,
     157,     0,   519,     0,     0,     0,   522,   520,     0,     0,
       0,     0,     0,     0,   436,     0,   160,   582,     0,   588,
       0,     0,     0,   569,     0,     0,   592,   596,     0,     0,
     572,     0,     0,     0,     0,   509,     0,   516,     0,   567,
     524,   690,   693,   465,   466,   468,   467,   469,   459,   460,
     461,   470,   471,   457,   472,   473,   462,   463,   464,   456,
     848,   872,   875,   837,   878,   868,     0,   832,     0,   882,
     884,   889,   891,   896,   898,   902,   662,   905,   668,   852,
       0,   341,     0,     0,   752,     0,   753,   677,   676,     0,
     385,     0,     0,   438,   157,     0,    69,    70,     0,    86,
      78,     0,   438,   393,     0,     0,   476,   474,   475,     0,
     650,   632,   629,   630,   631,   634,   971,   974,   363,     0,
     368,   369,   367,     0,   417,     0,     0,   420,   415,     0,
       0,     0,  1007,  1005,   342,     0,   406,   409,   412,  1011,
    1009,   980,   989,   987,   977,     0,     0,     0,   948,     0,
     983,   941,   964,   956,   984,   992,   954,     0,   948,     0,
     960,   961,   968,   952,     0,   347,   350,    35,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   160,
     521,     0,   338,     0,   433,   434,     0,   432,   431,     0,
       0,     0,     0,     0,     0,     0,   497,     0,     0,   593,
       0,   581,     0,   570,     0,   338,   510,     0,     0,   568,
     517,   513,     0,   820,   833,   819,   885,   892,   899,   340,
     345,   824,   751,     0,     0,     0,     0,    97,    95,     0,
     160,    72,     0,    79,     0,   309,   157,     0,     0,   684,
       0,   633,   646,   632,     0,     0,     0,   364,   370,     0,
       0,     0,     0,   416,   997,  1000,  1003,     0,     0,     0,
       0,     0,     0,     0,   962,     0,     0,     0,   616,     0,
       0,     0,   966,     0,     0,   959,     0,     0,   340,    32,
      39,     0,     0,     0,     0,     0,     0,   932,   931,   500,
     625,   503,     0,   495,     0,   440,     0,   437,   439,     0,
     425,   443,     0,   624,     0,   622,   498,   619,     0,     0,
       0,   618,     0,   511,     0,   514,   853,     0,   755,   678,
      90,   312,     0,    71,     0,    88,   160,   394,   684,     0,
       0,   643,     0,   648,   680,   679,   639,   962,   962,     0,
       0,     0,     0,     0,     0,     0,   338,  1020,   342,   407,
     410,   413,     0,   963,   981,   962,   962,   578,     0,   617,
    1022,   967,  1022,   969,  1022,   348,   351,    36,  1022,  1022,
       0,   934,   933,     0,     0,   506,     0,   435,     0,   426,
     583,   589,     0,   623,   621,     0,   620,   754,   442,    73,
     374,     0,    80,    84,    85,    82,    83,    81,     0,     0,
       0,     0,     0,     0,     0,   947,   947,   418,     0,     0,
     421,   962,   962,   937,     0,     0,  1024,   947,   937,   579,
       0,     0,     0,     0,    33,     0,     0,  1022,     0,     0,
     504,     0,   496,   427,   584,   590,   594,   512,     0,     0,
     380,   445,     0,     0,     0,   402,   455,   647,     0,     0,
     944,  1024,   946,     0,     0,     0,     0,   947,   947,   938,
       0,  1008,  1021,     0,     0,     0,     0,     0,     0,     0,
    1030,  1026,     0,   936,   935,   507,   595,   381,     0,   400,
     455,   398,   455,   401,   649,     0,  1025,   945,   972,   975,
     419,   422,     0,     0,  1004,  1012,   990,   978,  1023,  1028,
    1029,  1031,  1027,   396,   455,   399,   397,   942,     0,   998,
    1001,   395,     0,   943
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1601, -1601, -1601, -1601, -1601, -1601, -1601,   626,  1349, -1601,
   -1601, -1601, -1601, -1601, -1601,  1436, -1601, -1601, -1601,   768,
     376, -1601,  1286, -1601, -1601,  1350, -1601, -1601, -1601,  -233,
      -1, -1601, -1601, -1601,  -232, -1601, -1601, -1601, -1601, -1601,
   -1601, -1601, -1601, -1601, -1601,  1205, -1601, -1601,   -42,   -53,
   -1601, -1601, -1601,   538,   688,  -496,  -607,  -857, -1601, -1601,
   -1601, -1601, -1600, -1601, -1601,     2,  -219,  -280,  -495, -1601,
     235, -1601,  -635, -1425,  -765,  -201,  -483, -1601, -1601, -1601,
   -1601,  -611,     0, -1601, -1601, -1601, -1601, -1601,  -228,  -227,
    -226, -1601,  -223, -1601, -1601, -1601,  1451, -1601,   240, -1601,
   -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601,
   -1601, -1601,  -412,  -218,   918,   -84,    98, -1162,  -678, -1601,
    -726, -1601, -1601,  -519,   949, -1601, -1601, -1601, -1562, -1601,
   -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601,   761, -1601,
   -1601, -1601, -1601, -1601, -1601, -1601,  -159,    -4,  -132,     1,
     200, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601,   408,
    -482, -1011, -1601,  -481,  -974, -1601,  -900,  -131,  -129, -1601,
    -613, -1512, -1601,  -430, -1601, -1601,  1412, -1601, -1601, -1601,
     948,  1096,   159, -1601, -1601, -1601, -1601, -1601, -1601, -1601,
   -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601,
   -1601, -1601, -1601,  -746,  -222, -1601,   937, -1601, -1601, -1601,
    1142, -1601, -1601, -1601,  -476, -1601, -1601,  -437, -1601, -1601,
   -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601,
   -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601,
   -1601, -1601, -1601, -1601, -1601, -1601,  -236, -1601, -1601, -1601,
   -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601,
   -1601, -1601,   938,  -733,  -293,  -801,  -767, -1601, -1601, -1173,
    -990, -1601, -1601, -1601, -1275,  -148, -1256, -1601, -1601, -1601,
   -1601, -1601, -1601, -1601, -1601, -1601,   160,  -550, -1601, -1601,
   -1601,   677, -1601, -1601, -1601, -1601, -1601, -1601, -1601, -1601,
   -1601, -1601, -1601, -1601, -1601, -1601, -1489,  -814, -1601
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    17,   164,    58,   213,    18,   189,   205,  1717,
    1518,  1629,   808,   586,   170,   587,   109,    20,    21,    49,
      50,    51,    98,    22,    41,    42,   721,   722,  1448,  1449,
     653,   724,  1584,  1674,   725,   726,  1209,   727,   922,   728,
     729,   730,   731,  1442,   930,   206,   207,    37,    38,    39,
     228,    73,    74,    75,    76,    24,   458,   521,   299,   122,
      25,   179,   300,   180,   219,   459,   159,   943,  1220,   734,
     522,   735,   821,   638,   810,  1173,   588,  1046,  1627,  1047,
    1628,   737,   589,   738,   764,   993,  1597,   590,   739,   740,
     741,   742,   743,   744,   745,   691,   746,   962,  1454,  1214,
     747,   591,  1007,  1610,  1008,  1611,  1010,  1612,   592,   998,
    1603,   593,   822,  1651,   594,  1364,  1365,  1081,   945,   595,
     983,  1211,   596,   875,  1221,   749,   597,   598,  1073,   599,
    1349,  1724,  1350,  1781,   600,  1128,  1560,   601,   823,  1542,
    1784,  1544,  1785,  1658,  1826,   603,   515,  1465,  1592,  1262,
    1264,   990,   528,   986,   760,  1682,  1754,   516,   517,   518,
     893,   894,   504,   895,   896,   505,  1064,   915,   916,  1686,
     618,   475,   322,   323,   225,   315,    85,   131,    27,   185,
     462,    99,   100,   210,   101,    28,    55,   125,   182,    29,
     310,   526,   523,   984,   464,   223,   224,    83,   128,   466,
      30,   183,   312,   917,   604,   309,   392,   393,  1162,   650,
     244,   394,   886,  1564,  1170,   879,   501,   395,   619,  1410,
     898,  1569,   624,  1415,   620,  1411,   621,  1412,   623,  1414,
     627,  1419,   628,  1566,   629,  1421,   630,  1567,   631,  1423,
     632,  1568,   633,  1425,   634,  1427,   656,    31,   105,   215,
     402,   657,    32,   106,   216,   403,   661,    33,   104,   214,
     503,   905,   605,   827,  1810,   828,  1032,  1801,  1802,  1803,
    1033,  1045,  1328,  1322,  1317,  1512,  1272,   606,   991,  1595,
     992,  1596,  1017,  1616,  1014,  1614,  1034,   811,   607,  1015,
    1615,  1035,   608,  1278,  1693,  1279,  1694,  1280,  1695,  1002,
    1607,  1012,  1613,   805,   812,   609,  1771,  1054,   610
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      23,   873,   899,   748,  1295,   396,   314,   874,  1059,    54,
     955,  1005,  1065,  1067,   804,    66,    77,  1189,    78,   391,
     460,   888,   654,   890,   659,   892,   229,   733,   655,   660,
    1078,  1164,   758,  1166,  1038,  1168,   614,   529,  1502,   736,
     946,   947,  1313,  1297,  1444,   146,    94,  1305,  1325,   770,
     639,  1026,  1025,  1323,  1036,  1079,  1040,  1329,    34,    35,
     172,  1336,    67,  1051,  -883,   649,  1031,   519,  1031,  1463,
      77,    77,    77,  1055,  1026,  1338,  -155,  1680,  1439,  1521,
     641,    95,  1753,   226,   615,  1176,   531,   532,   114,   115,
     116,    68,   924,  1061,   616,    64,    13,   320,   832,   156,
     843,   161,  -890,   845,   846,   940,   538,  1319,   755,   636,
    1320,  -897,   540,   824,    77,    77,    77,    77,    14,    87,
     321,    86,  -505,   733,   652,   799,   801,    65,   637,    64,
    1351,  1061,  1169,   184,  -883,   736,  1495,   520,  1321,  -883,
     491,  1062,    59,  1061,  1666,   227,   218,   108,    60,   547,
     548,   617,  1464,  1524,  1798,  1061,   467,  -883,    69,   211,
    1063,    65,  1780,   502,   171,  1062,  1750,   492,   493,   771,
     772,   826,  -890,   316,  1799,   513,  1191,  -890,   907,   824,
     226,  -897,    56,   910,   221,   317,  -897,    70,   830,  1282,
     866,   867,  -505,  1271,    64,  -890,  1351,  -505,  1063,  1293,
      40,   689,  1296,  1716,  -897,   767,   550,   551,   733,   114,
    1063,   116,   845,   846,   102,  -505,    84,   245,   246,  1825,
     736,   733,  1063,  1772,  1622,  1773,    65,   319,    57,  1775,
    1776,  1312,   831,   736,  1080,   833,   834,    36,  1061,   640,
     514,   390,   227,  1186,   690,  1475,   401,    64,  1634,  1186,
      87,   391,    96,  1186,   494,   157,    13,  1186,   495,  1476,
     160,    71,  1135,    97,   562,   563,   564,   773,  1480,   642,
      72,  1186,   157,  1353,  1186,  1186,   469,   158,    14,    65,
     391,  1579,   391,   696,   697,  1216,   774,   643,  1822,   576,
    1586,   925,  1061,   644,   158,    13,    88,   391,   391,   866,
     867,  1649,  1778,   732,   763,  1063,  1481,   754,   756,   759,
    1339,   461,  1192,   513,   465,  1314,  1315,    14,   226,  1499,
     824,   582,    52,   652,   108,   496,   117,   320,  1203,   497,
     238,  1283,   498,    79,    80,  1212,    81,  1351,  1004,    13,
     391,   391,  1354,  1316,    53,    89,  1711,   499,  1306,  1061,
     321,   118,  1018,   500,    52,   837,   838,   239,  1704,  1063,
      90,    14,  1003,   843,    82,   844,   845,   846,   847,  1359,
      52,    93,  1013,   848,   107,  1016,    53,   906,   733,  1360,
     227,    13,  1352,   877,   878,   880,    13,   882,   883,   490,
     736,   887,    53,   889,  1213,   891,   318,   391,   391,   391,
     963,   391,   391,    14,  1158,   391,   612,   391,    14,   391,
     752,   391,   584,   942,   652,  1186,  1063,  1361,  1187,  1077,
    1172,  1188,  1171,    92,   913,   927,   782,   108,   733,   613,
      13,  1755,  1756,    13,   937,   243,  1362,    52,   908,  -825,
     736,  1363,   911,   113,   733,  1355,  1525,   914,  1206,  1767,
    1768,    43,    14,   866,   867,    14,   736,  1132,   652,    53,
    1461,   525,   502,   527,   833,   834,   149,   150,  1469,   151,
     220,   119,  1356,   390,  1366,    87,    44,    45,    46,   120,
    1263,   121,   736,   736,   736,   736,   736,   736,   736,   736,
     736,   736,   736,  1332,   736,   736,   736,   736,   736,   736,
     468,  1202,   390,  1337,   390,  1807,  1808,    47,  1621,  1541,
     502,  1193,    52,    64,  1155,   692,   694,    48,  1624,   390,
     390,   723,  1069,   753,    13,   132,   133,   757,  1284,    64,
     775,   783,  1215,    43,    53,   390,   768,  1575,  1069,  1456,
    1457,  1458,  1498,  1070,    13,    65,    14,    13,  1679,   776,
     784,   474,    13,   391,  1266,  1267,   502,  1508,    44,    45,
      46,    65,   390,   390,   147,  1281,    14,   391,    52,    14,
    1287,  1288,  1416,  1290,    14,  1292,  1031,  1294,    13,  1501,
    1417,  1178,  1270,  1804,   837,   838,  1071,  1098,    91,    47,
      53,  1031,   843,  1435,  1814,   845,   846,   847,  1194,    48,
      14,   989,   848,  1194,  1099,  1194,   652,    97,   110,   111,
     112,  1085,  1089,   872,  1429,    13,  1436,   826,    13,   390,
     390,   390,  1635,   390,   390,   826,  1103,   390,   658,   390,
    1390,   390,  1470,   390,  1842,  1843,    13,    14,    13,    13,
      14,  1190,  1587,   693,  1129,  1617,   652,   584,   942,   904,
    1539,  1198,   173,   174,   175,   176,   194,   195,    14,   157,
      14,    14,  1174,   152,   652,  1194,   652,   652,  1207,  1589,
    1181,  1208,  1642,  1182,  1492,  1494,   391,  1645,   155,  1194,
    1194,   158,   866,   867,   391,   165,  1180,   391,  1426,   117,
      13,  1428,  1656,  1749,   786,  1194,   474,  1430,  1311,   134,
     135,  1311,   136,   137,   138,   139,  1663,  1665,   140,  1311,
     933,   141,    14,   787,  1259,  1452,   391,   166,   652,  1608,
     949,   950,  1709,  1672,  1539,  1857,  1515,   996,  1863,   230,
     231,  1345,    13,  1587,  1531,   142,   491,   956,   957,   958,
     959,  1311,   960,  1529,   584,   942,   997,   964,  1346,  1540,
     525,    13,   143,  1311,    14,   144,    77,  1268,  1588,   167,
     652,   169,  1277,   492,   493,  1256,  1726,   995,  1516,   391,
     391,   391,    13,    14,    13,   390,   391,   168,  1735,   652,
     391,    13,   157,   313,   524,   391,   391,  1599,   391,   390,
     391,   208,   391,   391,    14,  1379,    14,  1384,   397,  1644,
     652,    59,   502,    14,   158,    13,  1156,    60,  1605,   652,
    1174,  1174,  1380,   398,  1385,   903,   181,  1606,   399,  1748,
     400,  1172,  1662,  1779,   391,   391,   824,    14,    95,  1072,
     196,   197,   491,   198,   199,   200,   201,  1580,   212,   202,
    1445,  1479,   141,  1351,   301,  1333,  1334,  1485,   302,     2,
     494,  1446,  1447,  1671,   495,   123,     3,  1430,  1430,   492,
     493,   124,   303,   304,   153,   154,   203,   305,   306,   307,
     308,   190,   191,   192,   126,  1154,   129,   193,   162,     4,
     127,     5,   130,     6,   163,   217,   204,   114,  1377,     7,
     233,   234,   235,  1497,   472,   237,   236,   473,   390,     8,
     474,  1509,  1179,   222,  1510,     9,   390,  1511,   177,   390,
    1260,  1507,  1528,   240,   178,  1440,  1261,  1514,   242,   502,
     311,   496,  1413,  1159,  1519,   497,  1520,  1298,   498,    10,
     241,  1620,   748,  1765,  1623,  1626,   391,  1194,   390,  1631,
    1194,  1194,   243,   499,   324,  1194,   494,  1632,   391,   500,
     495,  1437,  1813,  1194,   502,  1210,   733,  1720,  1160,  1547,
     502,   455,   456,  1194,  1163,   391,   833,   834,   736,   502,
     457,  1557,   463,  1165,    11,    12,  1562,   502,   502,  1676,
     470,  1167,  1468,   471,  1257,   476,   502,  1837,   477,  1265,
    1478,   390,   390,   390,   479,   502,   502,  1600,   390,  1731,
    1786,   478,   390,  1326,  1319,  1172,  1327,   390,   390,   480,
     390,   481,   390,   484,   390,   390,   485,   496,    44,    45,
      46,   497,  1800,  1800,   498,   482,   491,  1576,   486,   487,
    1809,   114,   115,   116,  1800,  1809,   491,    13,   488,   499,
     186,   187,  1048,  1049,   489,   500,   390,   390,   186,   187,
     188,   502,   506,   492,   493,   507,   391,   391,   508,    14,
     510,  1764,   391,   492,   493,   509,  1835,  1752,   833,   834,
     230,   231,   232,   511,  1800,  1800,  1714,   512,    15,   626,
     530,  1718,   611,   625,  1619,   647,   837,   838,   662,    16,
    1042,  1043,  1044,   695,   843,   761,   844,   845,   846,   847,
     918,   919,   920,   688,   848,  1652,    61,    62,    63,  1633,
     762,   769,   777,   780,   778,   779,   663,   664,   665,   666,
     667,   668,   669,   670,   781,  1862,   785,   391,   782,   788,
     789,   792,   806,   807,   871,  1392,   793,  1793,  1794,   794,
     494,  1795,   795,   615,   495,   671,   796,   797,    16,   885,
     494,   658,   900,   616,   495,   672,   673,   674,   390,   829,
     884,  1418,   869,   870,   932,  1581,   909,   934,   952,   938,
     390,   953,   863,   864,   865,   961,   988,   994,   999,  1828,
    1000,    13,  1829,  1831,   866,   867,   491,   390,   837,   838,
    1001,  1052,  1053,  1056,  1570,  1057,   843,  1058,   844,   845,
     846,   847,  1060,    14,  1066,  1577,   848,  1068,   391,  1096,
     617,   496,  1134,   492,   493,   497,   963,  1853,   498,   391,
    1157,   496,  1161,  1590,  1175,   497,  1184,  1443,   498,  1185,
    1195,   602,  1585,   499,  1194,  1197,  1199,   391,  1201,   500,
     622,  1200,  1751,   499,  1205,  1263,  1258,  1285,  1275,   500,
     635,  1311,  1318,  1331,  1367,  1344,  1357,   733,  1460,  1347,
     646,  1650,  1348,  1358,  1369,  1368,  1387,  1370,  1371,   736,
    1372,  1382,   861,   862,   863,   864,   865,  1420,   390,   390,
    1422,  1383,  1388,   750,   390,  1431,   866,   867,  1424,  1441,
    1766,  1434,  1453,  1459,  1471,  1472,  1670,  1473,   733,   733,
     494,  1483,  1673,   615,   495,  1484,  1534,  1496,  1654,  1490,
     736,   736,  1792,   616,   790,   791,  1493,  1523,  1503,  1506,
     391,  1513,   391,  1535,  1530,   803,  1536,  1537,   925,  1538,
    1549,  1550,  1552,   733,  1558,  1563,  1565,   803,   814,   815,
     816,   817,   818,  1573,  1574,   736,  1591,  1598,  1601,   390,
    1630,  1691,  1602,  1617,  1625,  1643,  1648,  1646,   637,  1583,
    1668,  1677,  1678,  1690,  1687,  1688,  1696,  1703,   833,   834,
     617,   496,  1725,  1697,  1698,   497,  1193,  1702,   498,   876,
    1758,  1705,  1706,  1759,   675,   676,   677,   678,   679,   680,
     681,   682,  1650,   499,  1727,  1770,  1761,  1762,  1774,   500,
    1782,  1816,  1763,   683,  1787,  1836,  1818,  1811,   902,  1812,
    1817,  1819,  1820,   684,  1821,  1838,  1851,  1839,  1844,  1845,
    1846,  1858,  1732,   685,   686,   687,  1847,  1859,  1849,  1850,
     390,  1571,  1852,  1860,  1050,   145,   391,    19,   209,   148,
    1739,   390,  1742,   325,   987,  1451,  1743,  1744,  1745,   912,
     723,  1746,    26,  1455,  1738,  1647,  1533,   391,  1593,   390,
     921,  1683,  1462,  1684,   926,  1685,  1594,   929,  1791,   931,
     103,   751,   765,   766,   936,  1815,  1713,   941,  1039,  1505,
       0,   948,     0,   483,     0,   951,   835,   836,   837,   838,
     839,     0,     0,   840,   841,   842,   843,     0,   844,   845,
     846,   847,     0,     0,     0,     0,   848,     0,   849,   850,
       0,     0,     0,   985,   851,   852,   853,     0,  1834,     0,
     854,     0,     0,     0,     0,     0,     0,   391,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   803,  1006,     0,
       0,  1009,   390,  1011,   390,     0,     0,     0,     0,     0,
       0,     0,     0,  1019,  1020,  1021,  1022,  1023,  1024,     0,
    1030,     0,  1030,     0,     0,   855,     0,   856,   857,   858,
     859,   860,   861,   862,   863,   864,   865,     0,     0,     0,
       0,     0,     0,  1675,     0,     0,   866,   867,     0,  1681,
       0,     0,     0,     0,     0,     0,  1090,  1091,     0,     0,
    1092,  1093,  1094,  1095,     0,  1097,     0,  1100,  1101,  1102,
    1104,  1105,  1106,  1107,  1108,  1109,  1111,  1112,  1113,  1114,
    1115,  1116,  1117,  1118,  1119,  1120,  1121,     0,  1130,     0,
       0,  1133,     0,     0,  1136,   944,   944,   944,     0,     0,
       0,     0,     0,     0,   833,   834,     0,     0,     0,     0,
       0,     0,     0,     0,   954,     0,     0,     0,   390,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   954,   491,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   390,
       0,     0,     0,   491,   926,     0,     0,     0,     0,  1196,
       0,     0,     0,     0,     0,     0,   492,   493,     0,     0,
    1204,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     492,   493,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1072,     0,  1222,  1224,  1226,  1228,
    1230,  1232,  1234,  1236,  1238,  1240,  1242,  1243,  1245,  1247,
    1249,  1251,  1253,  1255,     0,     0,     0,     0,     0,   390,
       0,   491,  1797,     0,     0,     0,  1274,     0,  1276,     0,
       0,     0,   835,   836,   837,   838,   839,     0,     0,   840,
     841,   842,   843,     0,   844,   845,   846,   847,   492,   493,
       0,  1072,   848,   494,   849,   850,     0,   495,   814,  1308,
       0,     0,     0,     0,   954,  1833,     0,   494,     0,     0,
       0,   495,  1330,     0,   803,     0,     0,     0,     0,     0,
    1335,     0,     0,     0,   803,     0,     0,     0,     0,  1340,
       0,  1341,     0,  1342,     0,  1343,     0,     0,     0,  1855,
       0,  1856,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   954,   858,   859,   860,   861,   862,
     863,   864,   865,  1861,   496,     0,   833,   834,   497,   954,
    1466,   498,   866,   867,     0,   494,     0,     0,   496,   495,
       0,     0,   497,     0,  1467,   498,   499,     0,     0,     0,
       0,   944,   500,   -81,  1381,     0,     0,     0,  1386,     0,
     499,     0,     0,     0,   833,   834,   500,     0,  1393,  1394,
    1395,  1396,  1397,  1398,  1399,  1400,  1401,  1402,  1403,  1404,
    1405,  1406,  1407,  1408,  1409,  1223,  1225,  1227,  1229,  1231,
    1233,  1235,  1237,  1239,  1241,     0,  1244,  1246,  1248,  1250,
    1252,  1254,     0,     0,   491,     0,   496,     0,     0,  1432,
     497,     0,  1482,   498,   944,  1433,     0,     0,     0,     0,
    1438,     0,     0,     0,     0,  1340,     0,     0,   499,     0,
       0,   492,   493,     0,   500,     0,     0,     0,     0,     0,
    1450,     0,     0,     0,   835,   836,   837,   838,     0,     0,
       0,     0,     0,     0,   843,     0,   844,   845,   846,   847,
       0,     0,     0,     0,   848,     0,   849,   850,     0,     0,
       0,     0,     0,     0,   965,   966,   967,   968,   969,   970,
     971,   972,   835,   836,   837,   838,   839,   973,   974,   840,
     841,   842,   843,   975,   844,   845,   846,   847,     0,     0,
       0,     0,   848,   976,   849,   850,   977,   978,     0,   491,
     851,   852,   853,   979,   980,   981,   854,     0,   494,     0,
       0,     0,   495,     0,   954,     0,     0,     0,   833,   834,
     861,   862,   863,   864,   865,     0,   492,   493,     0,     0,
       0,  1500,     0,     0,   866,   867,     0,     0,     0,  1504,
    1030,     0,     0,   491,     0,     0,     0,     0,     0,     0,
     982,   855,     0,   856,   857,   858,   859,   860,   861,   862,
     863,   864,   865,     0,     0,     0,     0,     0,  1526,  1527,
     492,   493,   866,   867,     0,     0,  1340,   584,   942,   496,
       0,     0,   954,   497,     0,  1486,   498,     0,     0,  1543,
       0,  1545,     0,  1548,     0,   944,   944,   944,     0,  1551,
     954,   499,   954,  1554,   954,     0,   954,   500,   954,     0,
     954,     0,   954,   494,   954,     0,   954,   495,   954,     0,
     954,     0,     0,   954,     0,   954,     0,   954,     0,   954,
       0,   954,     0,   954,     0,     0,   835,   836,   837,   838,
     839,     0,     0,   840,   841,   842,   843,     0,   844,   845,
     846,   847,     0,     0,     0,     0,   848,   494,   849,   850,
       0,   495,     0,  1578,   851,   852,   853,     0,     0,  1582,
     854,     0,   750,     0,     0,     0,     0,     0,     0,     0,
       0,   833,   834,     0,   496,     0,     0,     0,   497,     0,
    1487,   498,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   803,   499,     0,     0,     0,
       0,     0,   500,     0,     0,   855,     0,   856,   857,   858,
     859,   860,   861,   862,   863,   864,   865,     0,   496,     0,
       0,     0,   497,     0,  1488,   498,   866,   867,     0,     0,
       0,   584,   942,     0,  1636,  1637,  1638,     0,   491,     0,
     499,     0,     0,   491,     0,     0,   500,     0,     0,     0,
       0,   491,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1659,     0,  1660,     0,   492,   493,     0,     0,  1664,
     492,   493,     0,     0,   833,   834,     0,     0,   492,   493,
       0,     0,     0,     0,  1667,     0,  1669,     0,     0,   835,
     836,   837,   838,   839,     0,     0,   840,   841,   842,   843,
       0,   844,   845,   846,   847,     0,     0,     0,     0,   848,
    1689,   849,   850,  1692,     0,     0,     0,   851,   852,   853,
       0,  1699,  1700,  1701,     0,     0,     0,     0,  1708,     0,
       0,  1710,     0,     0,  1712,     0,     0,   803,  1715,     0,
       0,     0,   803,  1719,     0,  1721,  1722,     0,     0,     0,
       0,     0,   494,     0,     0,     0,   495,   494,     0,     0,
    1729,   495,     0,     0,     0,   494,     0,     0,   855,   495,
     856,   857,   858,   859,   860,   861,   862,   863,   864,   865,
       0,     0,     0,     0,     0,  1747,     0,     0,     0,   866,
     867,     0,   835,   836,   837,   838,   839,     0,     0,   840,
     841,   842,   843,     0,   844,   845,   846,   847,     0,   803,
       0,     0,   848,     0,   849,   850,     0,     0,     0,     0,
     851,   852,   853,   496,     0,     0,   854,   497,   496,  1489,
     498,  1777,   497,     0,  1491,   498,   496,     0,     0,  1783,
     497,     0,  1604,   498,     0,   499,     0,   954,     0,     0,
     499,   500,  1790,     0,     0,     0,   500,     0,   499,     0,
       0,     0,     0,  1796,   500,     0,     0,   491,     0,  1805,
    1806,   855,     0,   856,   857,   858,   859,   860,   861,   862,
     863,   864,   865,     0,     0,     0,     0,     0,     0,  1823,
    1824,     0,   866,   867,   492,   493,   868,     0,     0,     0,
    1827,     0,     0,     0,  1830,  1832,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     698,     0,     0,     0,   531,   532,     3,  1848,   699,   700,
     701,     0,   702,     0,   533,   534,   535,   536,   537,  1854,
       0,     0,     0,     0,   538,   703,   539,   704,   705,     0,
     540,     0,     0,     0,     0,     0,     0,   706,   541,   707,
       0,   708,     0,   709,   542,     0,     0,   543,     0,     8,
     544,   710,     0,   711,   545,     0,     0,   712,   713,     0,
       0,   494,     0,     0,   714,   495,     0,   547,   548,     0,
     332,   333,   334,     0,   336,   337,   338,   339,   340,   549,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,     0,   354,   355,   356,     0,     0,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   550,   551,   715,   716,     0,     0,
       0,     0,   496,     0,     0,     0,   497,     0,  1609,   498,
     553,   554,     0,     0,   954,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   499,   717,   718,   719,     0,     0,
     500,     0,     0,     0,     0,    64,     0,     0,     0,     0,
       0,     0,     0,   555,   556,   557,   558,   559,   954,   560,
     954,   561,   562,   563,   564,     0,   157,    13,   565,   566,
       0,   567,   568,   569,   570,   571,   572,    65,   720,   574,
     575,     0,   954,     0,     0,     0,     0,   576,   158,    14,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   577,   578,   579,     0,    15,     0,
       0,   580,   581,     0,     0,   531,   532,     0,     0,   582,
       0,   583,     0,   584,   585,   533,   534,   535,   536,   537,
       0,     0,     0,     0,     0,   538,     0,   539,     0,     0,
       0,   540,     0,   491,     0,     0,     0,     0,     0,   541,
       0,     0,     0,     0,     0,   542,     0,     0,   543,     0,
       0,   544,     0,  1026,     0,   545,     0,     0,     0,     0,
     492,   493,     0,     0,     0,   546,     0,     0,   547,   548,
       0,   332,   333,   334,     0,   336,   337,   338,   339,   340,
     549,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,     0,   354,   355,   356,     0,     0,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   550,   551,   552,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   553,   554,     0,     0,     0,     0,   494,     0,     0,
       0,   495,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,   555,   556,   557,   558,   559,     0,
     560,   824,   561,   562,   563,   564,     0,     0,     0,   565,
     566,     0,   567,   568,   569,   570,   571,   572,   825,   573,
     574,   575,     0,     0,     0,     0,     0,     0,   576,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   496,     0,
       0,     0,   497,     0,     0,  1027,   578,   579,     0,    15,
       0,     0,   580,   581,     0,     0,     0,   531,   532,     0,
    1028,     0,  1029,     0,   584,   585,   500,   533,   534,   535,
     536,   537,     0,     0,     0,     0,     0,   538,     0,   539,
       0,     0,     0,   540,     0,   491,     0,     0,     0,     0,
       0,   541,     0,     0,     0,     0,     0,   542,     0,     0,
     543,     0,     0,   544,     0,     0,     0,   545,     0,     0,
       0,     0,   492,   493,     0,     0,     0,   546,     0,     0,
     547,   548,     0,   332,   333,   334,     0,   336,   337,   338,
     339,   340,   549,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,     0,   354,   355,   356,     0,     0,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   374,   375,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   550,   551,   552,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   553,   554,     0,     0,     0,     0,   494,
       0,     0,     0,   495,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
       0,     0,     0,     0,     0,     0,   555,   556,   557,   558,
     559,     0,   560,   824,   561,   562,   563,   564,     0,     0,
       0,   565,   566,     0,   567,   568,   569,   570,   571,   572,
     825,   573,   574,   575,     0,     0,     0,     0,     0,     0,
     576,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     496,     0,     0,     0,   497,     0,     0,  1027,   578,   579,
       0,    15,     0,     0,   580,   581,     0,     0,     0,   531,
     532,     0,  1028,     0,  1037,     0,   584,   585,   500,   533,
     534,   535,   536,   537,     0,     0,     0,     0,     0,   538,
       0,   539,     0,     0,     0,   540,     0,   641,     0,     0,
       0,     0,     0,   541,     0,     0,     0,     0,     0,   542,
       0,     0,   543,     0,     0,   544,     0,     0,     0,   545,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   546,
       0,     0,   547,   548,     0,   332,   333,   334,     0,   336,
     337,   338,   339,   340,   549,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,     0,   354,   355,   356,
       0,     0,   359,   360,   361,   362,   363,   364,   365,   366,
     367,   368,   369,   370,   371,   372,   373,   374,   375,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   550,
     551,   552,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   553,   554,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,     0,     0,     0,     0,     0,     0,   555,   556,
     557,   558,   559,     0,   560,     0,   561,   562,   563,   564,
       0,     0,     0,   565,   566,   813,   567,   568,   569,   570,
     571,   572,    65,   573,   574,   575,     0,     0,     0,     0,
       0,     0,   576,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   642,     0,     0,   577,
     578,   579,     0,    15,     0,     0,   580,   581,     0,     0,
       0,   531,   532,     0,  1307,     0,   583,     0,   584,   585,
     644,   533,   534,   535,   536,   537,     0,     0,     0,     0,
       0,   538,     0,   539,     0,     0,     0,   540,     0,     0,
       0,     0,     0,     0,     0,   541,     0,     0,     0,     0,
       0,   542,     0,     0,   543,     0,     0,   544,     0,     0,
       0,   545,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   546,     0,     0,   547,   548,     0,   332,   333,   334,
       0,   336,   337,   338,   339,   340,   549,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,     0,   354,
     355,   356,     0,     0,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   550,   551,   715,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   553,   554,     0,
       0,     0,     0,     0,     0,     0,   923,     0,     0,     0,
       0,     0,   717,   718,   719,     0,     0,     0,     0,     0,
       0,     0,    64,     0,     0,     0,     0,     0,     0,     0,
     555,   556,   557,   558,   559,   491,   560,     0,   561,   562,
     563,   564,     0,     0,     0,   565,   566,     0,   567,   568,
     569,   570,   571,   572,    65,   573,   574,   575,     0,     0,
       0,     0,   492,   493,   576,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   577,   578,   579,     0,    15,     0,     0,   580,   581,
       0,     0,   531,   532,     0,     0,   582,     0,   583,     0,
     584,   585,   533,   534,   535,   536,   537,     0,     0,     0,
       0,     0,   538,     0,   539,     0,     0,     0,   540,     0,
       0,     0,     0,     0,     0,     0,   541,     0,     0,     0,
       0,     0,   542,     0,     0,   543,     0,     0,   544,     0,
       0,     0,   545,     0,     0,     0,     0,     0,     0,   494,
       0,     0,   546,   495,     0,   547,   548,     0,   332,   333,
     334,     0,   336,   337,   338,   339,   340,   549,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,     0,
     354,   355,   356,     0,     0,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   550,   551,   715,     0,     0,     0,     0,     0,
     496,     0,     0,     0,   497,     0,  1641,   498,   553,   554,
       0,     0,     0,     0,     0,     0,     0,   939,     0,     0,
       0,     0,   499,   717,   718,   719,     0,     0,   500,     0,
       0,     0,     0,    64,     0,     0,     0,     0,     0,     0,
       0,   555,   556,   557,   558,   559,   491,   560,     0,   561,
     562,   563,   564,     0,     0,     0,   565,   566,     0,   567,
     568,   569,   570,   571,   572,    65,   573,   574,   575,     0,
       0,     0,     0,   492,   493,   576,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   577,   578,   579,     0,    15,     0,     0,   580,
     581,     0,     0,   531,   532,     0,     0,   582,     0,   583,
       0,   584,   585,   533,   534,   535,   536,   537,     0,     0,
       0,     0,     0,   538,  1740,   539,   704,     0,     0,   540,
       0,     0,     0,     0,     0,     0,     0,   541,     0,     0,
       0,     0,     0,   542,     0,     0,   543,     0,     0,   544,
     710,     0,     0,   545,     0,     0,     0,     0,     0,     0,
     494,     0,     0,   546,   495,     0,   547,   548,     0,   332,
     333,   334,     0,   336,   337,   338,   339,   340,   549,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
       0,   354,   355,   356,     0,     0,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   368,   369,   370,   371,   372,
     373,   374,   375,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   550,   551,   552,  1741,     0,     0,     0,
       0,   496,     0,     0,     0,   497,     0,  1730,   498,   553,
     554,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   499,     0,     0,     0,     0,     0,   500,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,   555,   556,   557,   558,   559,     0,   560,     0,
     561,   562,   563,   564,     0,     0,     0,   565,   566,     0,
     567,   568,   569,   570,   571,   572,    65,   573,   574,   575,
       0,     0,     0,     0,     0,     0,   576,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   577,   578,   579,     0,    15,     0,     0,
     580,   581,     0,     0,   531,   532,     0,     0,   582,     0,
     583,     0,   584,   585,   533,   534,   535,   536,   537,     0,
       0,     0,     0,     0,   538,     0,   539,     0,     0,     0,
     540,     0,     0,     0,     0,     0,     0,     0,   541,     0,
       0,     0,     0,     0,   542,     0,     0,   543,     0,     0,
     544,     0,     0,     0,   545,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   546,     0,     0,   547,   548,     0,
     332,   333,   334,     0,   336,   337,   338,   339,   340,   549,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,     0,   354,   355,   356,     0,     0,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   550,   551,   715,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     553,   554,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   717,   718,   719,     0,     0,
       0,     0,     0,     0,     0,    64,     0,     0,     0,     0,
       0,     0,     0,   555,   556,   557,   558,   559,     0,   560,
       0,   561,   562,   563,   564,     0,     0,     0,   565,   566,
       0,   567,   568,   569,   570,   571,   572,    65,   573,   574,
     575,     0,     0,     0,     0,     0,     0,   576,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   577,   578,   579,     0,    15,     0,
       0,   580,   581,     0,     0,   531,   532,     0,     0,   582,
       0,   583,     0,   584,   585,   533,   534,   535,   536,   537,
       0,     0,     0,     0,     0,   538,     0,   539,     0,     0,
       0,   540,     0,     0,     0,     0,     0,     0,     0,   541,
       0,     0,     0,     0,     0,   542,     0,     0,   543,     0,
       0,   544,     0,     0,     0,   545,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   546,     0,     0,   547,   548,
    1074,   332,   333,   334,     0,   336,   337,   338,   339,   340,
     549,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,     0,   354,   355,   356,     0,     0,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   550,   551,   552,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   553,   554,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,   555,   556,   557,   558,   559,     0,
     560,   824,   561,   562,   563,   564,     0,     0,     0,   565,
     566,     0,   567,   568,   569,   570,   571,   572,   825,   573,
     574,   575,     0,     0,     0,     0,     0,     0,   576,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   577,   578,   579,     0,    15,
       0,     0,   580,   581,     0,     0,   531,   532,     0,     0,
    1075,     0,   583,  1076,   584,   585,   533,   534,   535,   536,
     537,     0,     0,     0,     0,     0,   538,     0,   539,     0,
       0,     0,   540,     0,     0,     0,     0,     0,     0,     0,
     541,     0,     0,     0,     0,     0,   542,     0,     0,   543,
       0,     0,   544,     0,     0,     0,   545,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   546,     0,     0,   547,
     548,     0,   332,   333,   334,     0,   336,   337,   338,   339,
     340,   549,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,     0,   354,   355,   356,     0,     0,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   550,   551,   715,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   553,   554,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1217,  1218,  1219,
       0,     0,     0,     0,     0,     0,     0,    64,     0,     0,
       0,     0,     0,     0,     0,   555,   556,   557,   558,   559,
       0,   560,     0,   561,   562,   563,   564,     0,     0,     0,
     565,   566,     0,   567,   568,   569,   570,   571,   572,    65,
     573,   574,   575,     0,     0,     0,     0,     0,     0,   576,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   577,   578,   579,     0,
      15,     0,     0,   580,   581,     0,     0,     0,     0,   531,
     532,   582,     0,   583,     0,   584,   585,   819,     0,   533,
     534,   535,   536,   537,     0,     0,     0,     0,     0,   538,
       0,   539,     0,     0,     0,   540,     0,     0,     0,     0,
       0,     0,     0,   541,     0,     0,     0,     0,     0,   542,
       0,     0,   543,   820,     0,   544,     0,     0,     0,   545,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   546,
       0,     0,   547,   548,     0,   332,   333,   334,     0,   336,
     337,   338,   339,   340,   549,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,     0,   354,   355,   356,
       0,     0,   359,   360,   361,   362,   363,   364,   365,   366,
     367,   368,   369,   370,   371,   372,   373,   374,   375,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   550,
     551,   552,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   553,   554,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,     0,     0,     0,     0,     0,     0,   555,   556,
     557,   558,   559,     0,   560,     0,   561,   562,   563,   564,
       0,     0,     0,   565,   566,     0,   567,   568,   569,   570,
     571,   572,    65,   573,   574,   575,     0,     0,     0,     0,
       0,     0,   576,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   577,
     578,   579,     0,    15,     0,     0,   580,   581,     0,     0,
       0,     0,   531,   532,   582,   645,   583,     0,   584,   585,
     819,     0,   533,   534,   535,   536,   537,     0,     0,     0,
       0,     0,   538,     0,   539,     0,     0,     0,   540,     0,
       0,     0,     0,     0,     0,     0,   541,     0,     0,     0,
       0,     0,   542,     0,     0,   543,   820,     0,   544,     0,
       0,     0,   545,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   546,     0,     0,   547,   548,     0,   332,   333,
     334,     0,   336,   337,   338,   339,   340,   549,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,     0,
     354,   355,   356,     0,     0,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   550,   551,   552,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   553,   554,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,     0,     0,     0,     0,     0,
       0,   555,   556,   557,   558,   559,     0,   560,   824,   561,
     562,   563,   564,     0,     0,     0,   565,   566,     0,   567,
     568,   569,   570,   571,   572,   825,   573,   574,   575,     0,
       0,     0,     0,     0,     0,   576,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   577,   578,   579,     0,    15,     0,     0,   580,
     581,     0,     0,     0,     0,   531,   532,   582,     0,   583,
       0,   584,   585,   819,     0,   533,   534,   535,   536,   537,
       0,     0,     0,     0,     0,   538,     0,   539,     0,     0,
       0,   540,     0,     0,     0,     0,     0,     0,     0,   541,
       0,     0,     0,     0,     0,   542,     0,     0,   543,   820,
       0,   544,     0,     0,     0,   545,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   546,     0,     0,   547,   548,
       0,   332,   333,   334,     0,   336,   337,   338,   339,   340,
     549,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,     0,   354,   355,   356,     0,     0,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   550,   551,   552,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   553,   554,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,   555,   556,   557,   558,   559,     0,
     560,     0,   561,   562,   563,   564,     0,     0,     0,   565,
     566,     0,   567,   568,   569,   570,   571,   572,    65,   573,
     574,   575,     0,     0,     0,     0,     0,     0,   576,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   577,   578,   579,     0,    15,
       0,     0,   580,   581,     0,     0,     0,     0,   531,   532,
     582,   900,   583,     0,   584,   585,   819,     0,   533,   534,
     535,   536,   537,     0,     0,     0,     0,     0,   538,     0,
     539,     0,     0,     0,   540,     0,     0,     0,     0,     0,
       0,     0,   541,     0,     0,     0,     0,     0,   542,     0,
       0,   543,   820,     0,   544,     0,     0,     0,   545,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   546,     0,
       0,   547,   548,     0,   332,   333,   334,     0,   336,   337,
     338,   339,   340,   549,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,     0,   354,   355,   356,     0,
       0,   359,   360,   361,   362,   363,   364,   365,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   550,   551,
     552,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   553,   554,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,   555,   556,   557,
     558,   559,     0,   560,     0,   561,   562,   563,   564,     0,
       0,     0,   565,   566,     0,   567,   568,   569,   570,   571,
     572,    65,   573,   574,   575,     0,     0,     0,     0,     0,
       0,   576,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   577,   578,
     579,     0,    15,     0,     0,   580,   581,     0,     0,   531,
     532,     0,     0,   582,     0,   583,     0,   584,   585,   533,
     534,   535,   536,   537,     0,     0,     0,     0,     0,   538,
       0,   539,     0,     0,     0,   540,     0,     0,     0,     0,
       0,     0,     0,   541,     0,     0,     0,     0,     0,   542,
       0,     0,   543,     0,     0,   544,     0,     0,     0,   545,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   546,
       0,     0,   547,   548,  1269,   332,   333,   334,     0,   336,
     337,   338,   339,   340,   549,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,     0,   354,   355,   356,
       0,     0,   359,   360,   361,   362,   363,   364,   365,   366,
     367,   368,   369,   370,   371,   372,   373,   374,   375,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   550,
     551,   552,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   553,   554,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,     0,     0,     0,     0,     0,     0,   555,   556,
     557,   558,   559,     0,   560,   824,   561,   562,   563,   564,
       0,     0,     0,   565,   566,     0,   567,   568,   569,   570,
     571,   572,   825,   573,   574,   575,     0,     0,     0,     0,
       0,     0,   576,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   577,
     578,   579,     0,    15,     0,     0,   580,   581,     0,     0,
     531,   532,     0,     0,   582,     0,   583,     0,   584,   585,
     533,   534,   535,   536,   537,     0,     0,     0,     0,     0,
     538,     0,   539,     0,     0,     0,   540,     0,     0,     0,
       0,     0,     0,     0,   541,     0,     0,     0,     0,     0,
     542,     0,     0,   543,     0,     0,   544,     0,     0,     0,
     545,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     546,     0,     0,   547,   548,     0,   332,   333,   334,     0,
     336,   337,   338,   339,   340,   549,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,     0,   354,   355,
     356,     0,     0,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     550,   551,   552,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   553,   554,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,   555,
     556,   557,   558,   559,     0,   560,   824,   561,   562,   563,
     564,     0,     0,     0,   565,   566,     0,   567,   568,   569,
     570,   571,   572,   825,   573,   574,   575,     0,     0,     0,
       0,     0,     0,   576,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     577,   578,   579,     0,    15,     0,     0,   580,   581,     0,
       0,   531,   532,     0,     0,   582,     0,   583,  1309,   584,
     585,   533,   534,   535,   536,   537,     0,     0,     0,     0,
       0,   538,     0,   539,     0,     0,     0,   540,     0,     0,
       0,     0,     0,     0,     0,   541,     0,     0,     0,     0,
       0,   542,     0,     0,   543,     0,     0,   544,     0,     0,
       0,   545,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   546,     0,     0,   547,   548,     0,   332,   333,   334,
       0,   336,   337,   338,   339,   340,   549,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,     0,   354,
     355,   356,     0,     0,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   550,   551,   552,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   553,   554,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    64,     0,     0,     0,     0,     0,     0,     0,
     555,   556,   557,   558,   559,     0,   560,   824,   561,   562,
     563,   564,     0,     0,     0,   565,   566,     0,   567,   568,
     569,   570,   571,   572,   825,   573,   574,   575,     0,     0,
       0,     0,     0,     0,   576,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   577,   578,   579,     0,    15,     0,     0,   580,   581,
       0,     0,   531,   532,     0,     0,   582,     0,   583,  1324,
     584,   585,   533,   534,   535,   536,   537,     0,     0,     0,
       0,     0,   538,     0,   539,     0,     0,     0,   540,     0,
       0,     0,     0,     0,     0,     0,   541,     0,     0,     0,
       0,     0,   542,     0,     0,   543,     0,     0,   544,     0,
       0,     0,   545,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   546,     0,     0,   547,   548,     0,   332,   333,
     334,     0,   336,   337,   338,   339,   340,   549,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,     0,
     354,   355,   356,     0,     0,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   550,   551,   552,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   553,   554,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,     0,     0,     0,     0,     0,
       0,   555,   556,   557,   558,   559,     0,   560,     0,   561,
     562,   563,   564,     0,     0,     0,   565,   566,     0,   567,
     568,   569,   570,   571,   572,    65,   573,   574,   575,     0,
       0,     0,     0,     0,     0,   576,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   577,   578,   579,     0,    15,     0,     0,   580,
     581,     0,     0,     0,     0,   531,   532,   582,   645,   583,
       0,   584,   585,   802,     0,   533,   534,   535,   536,   537,
       0,     0,     0,     0,     0,   538,     0,   539,     0,     0,
       0,   540,     0,     0,     0,     0,     0,     0,     0,   541,
       0,     0,     0,     0,     0,   542,     0,     0,   543,     0,
       0,   544,     0,     0,     0,   545,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   546,     0,     0,   547,   548,
       0,   332,   333,   334,     0,   336,   337,   338,   339,   340,
     549,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,     0,   354,   355,   356,     0,     0,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   550,   551,   552,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   553,   554,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,   555,   556,   557,   558,   559,     0,
     560,     0,   561,   562,   563,   564,     0,     0,     0,   565,
     566,     0,   567,   568,   569,   570,   571,   572,    65,   573,
     574,   575,     0,     0,     0,     0,     0,     0,   576,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   577,   578,   579,     0,    15,
       0,     0,   580,   581,     0,     0,     0,     0,   531,   532,
     582,     0,   583,     0,   584,   585,   809,     0,   533,   534,
     535,   536,   537,     0,     0,     0,     0,     0,   538,     0,
     539,     0,     0,     0,   540,     0,     0,     0,     0,     0,
       0,     0,   541,     0,     0,     0,     0,     0,   542,     0,
       0,   543,     0,     0,   544,     0,     0,     0,   545,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   546,     0,
       0,   547,   548,     0,   332,   333,   334,     0,   336,   337,
     338,   339,   340,   549,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,     0,   354,   355,   356,     0,
       0,   359,   360,   361,   362,   363,   364,   365,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   550,   551,
     552,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   553,   554,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,   555,   556,   557,
     558,   559,     0,   560,     0,   561,   562,   563,   564,     0,
       0,     0,   565,   566,     0,   567,   568,   569,   570,   571,
     572,    65,   573,   574,   575,     0,     0,     0,     0,     0,
       0,   576,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   577,   578,
     579,     0,    15,     0,     0,   580,   581,     0,     0,   531,
     532,     0,     0,   582,     0,   583,     0,   584,   585,   533,
     534,   535,   536,   537,     0,     0,     0,     0,     0,   538,
       0,   539,     0,     0,     0,   540,     0,     0,     0,     0,
       0,     0,     0,   541,     0,     0,     0,     0,     0,   542,
       0,     0,   543,     0,     0,   544,     0,     0,     0,   545,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   546,
       0,     0,   547,   548,     0,   332,   333,   334,     0,   336,
     337,   338,   339,   340,   549,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,     0,   354,   355,   356,
       0,     0,   359,   360,   361,   362,   363,   364,   365,   366,
     367,   368,   369,   370,   371,   372,   373,   374,   375,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   550,
     551,   552,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   553,   554,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,     0,     0,     0,     0,     0,     0,   555,   556,
     557,   558,   559,     0,   560,     0,   561,   562,   563,   564,
       0,     0,     0,   565,   566,   813,   567,   568,   569,   570,
     571,   572,    65,   573,   574,   575,     0,     0,     0,     0,
       0,     0,   576,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   577,
     578,   579,     0,    15,     0,     0,   580,   581,     0,     0,
     531,   532,     0,     0,   582,     0,   583,     0,   584,   585,
     533,   534,   535,   536,   537,     0,     0,     0,     0,     0,
     538,     0,   539,     0,     0,     0,   540,     0,     0,     0,
       0,     0,     0,     0,   541,     0,     0,     0,     0,     0,
     542,     0,     0,   543,     0,     0,   544,     0,     0,     0,
     545,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     546,     0,     0,   547,   548,     0,   332,   333,   334,     0,
     336,   337,   338,   339,   340,   549,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,     0,   354,   355,
     356,     0,     0,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     550,   551,   552,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   553,   554,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,   555,
     556,   557,   558,   559,     0,   560,   824,   561,   562,   563,
     564,     0,     0,     0,   565,   566,     0,   567,   568,   569,
     570,   571,   572,   825,   573,   574,   575,     0,     0,     0,
       0,     0,     0,   576,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     577,   578,   579,     0,    15,     0,     0,   580,   581,     0,
       0,   531,   532,     0,     0,   582,     0,   583,     0,   584,
     585,   533,   534,   535,   536,   537,     0,     0,     0,     0,
       0,   538,     0,   539,     0,     0,     0,   540,     0,     0,
       0,     0,     0,     0,     0,   541,     0,     0,     0,     0,
       0,   542,     0,     0,   543,     0,     0,   544,     0,     0,
       0,   545,     0,     0,     0,     0,     0,   928,     0,     0,
       0,   546,     0,     0,   547,   548,     0,   332,   333,   334,
       0,   336,   337,   338,   339,   340,   549,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,     0,   354,
     355,   356,     0,     0,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   550,   551,   552,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   553,   554,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    64,     0,     0,     0,     0,     0,     0,     0,
     555,   556,   557,   558,   559,     0,   560,     0,   561,   562,
     563,   564,     0,     0,     0,   565,   566,     0,   567,   568,
     569,   570,   571,   572,    65,   573,   574,   575,     0,     0,
       0,     0,     0,     0,   576,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   577,   578,   579,     0,    15,     0,     0,   580,   581,
       0,     0,   531,   532,     0,     0,   582,     0,   583,     0,
     584,   585,   533,   534,   535,   536,   537,     0,     0,     0,
       0,     0,   538,     0,   539,     0,     0,     0,   540,     0,
       0,     0,     0,     0,     0,     0,   541,     0,     0,     0,
       0,     0,   542,     0,     0,   543,     0,     0,   544,     0,
       0,     0,   545,     0,     0,   935,     0,     0,     0,     0,
       0,     0,   546,     0,     0,   547,   548,     0,   332,   333,
     334,     0,   336,   337,   338,   339,   340,   549,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,     0,
     354,   355,   356,     0,     0,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   550,   551,   552,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   553,   554,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,     0,     0,     0,     0,     0,
       0,   555,   556,   557,   558,   559,     0,   560,     0,   561,
     562,   563,   564,     0,     0,     0,   565,   566,     0,   567,
     568,   569,   570,   571,   572,    65,   573,   574,   575,     0,
       0,     0,     0,     0,     0,   576,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   577,   578,   579,     0,    15,     0,     0,   580,
     581,     0,     0,   531,   532,     0,     0,   582,     0,   583,
       0,   584,   585,   533,   534,   535,   536,   537,     0,     0,
       0,     0,     0,   538,     0,   539,     0,     0,     0,   540,
       0,     0,     0,     0,     0,     0,     0,   541,     0,     0,
       0,     0,     0,   542,     0,     0,   543,     0,     0,   544,
       0,     0,     0,   545,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   546,     0,     0,   547,   548,     0,   332,
     333,   334,     0,   336,   337,   338,   339,   340,   549,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
       0,   354,   355,   356,     0,     0,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   368,   369,   370,   371,   372,
     373,   374,   375,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   550,   551,   552,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   553,
     554,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,   555,   556,   557,   558,   559,     0,   560,     0,
     561,   562,   563,   564,     0,     0,     0,   565,   566,     0,
     567,   568,   569,   570,   571,   572,    65,   573,   574,   575,
       0,     0,     0,     0,     0,     0,   576,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   806,     0,   577,   578,   579,     0,    15,     0,     0,
     580,   581,     0,     0,   531,   532,     0,     0,   582,     0,
     583,     0,   584,   585,   533,   534,   535,   536,   537,     0,
       0,  1110,     0,     0,   538,     0,   539,     0,     0,     0,
     540,     0,     0,     0,     0,     0,     0,     0,   541,     0,
       0,     0,     0,     0,   542,     0,     0,   543,     0,     0,
     544,     0,     0,     0,   545,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   546,     0,     0,   547,   548,     0,
     332,   333,   334,     0,   336,   337,   338,   339,   340,   549,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,     0,   354,   355,   356,     0,     0,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   550,   551,   552,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     553,   554,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,     0,     0,     0,
       0,     0,     0,   555,   556,   557,   558,   559,     0,   560,
       0,   561,   562,   563,   564,     0,     0,     0,   565,   566,
       0,   567,   568,   569,   570,   571,   572,    65,   573,   574,
     575,     0,     0,     0,     0,     0,     0,   576,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   577,   578,   579,     0,    15,     0,
       0,   580,   581,     0,     0,   531,   532,     0,     0,   582,
       0,   583,     0,   584,   585,   533,   534,   535,   536,   537,
       0,     0,     0,     0,     0,   538,     0,   539,     0,     0,
       0,   540,     0,     0,     0,     0,     0,     0,     0,   541,
       0,     0,     0,     0,     0,   542,     0,     0,   543,     0,
       0,   544,     0,     0,     0,   545,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   546,     0,     0,   547,   548,
       0,   332,   333,   334,     0,   336,   337,   338,   339,   340,
     549,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,     0,   354,   355,   356,     0,     0,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   550,   551,   552,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   553,   554,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,   555,   556,   557,   558,   559,     0,
     560,     0,   561,   562,   563,   564,     0,     0,     0,   565,
     566,     0,   567,   568,   569,   570,   571,   572,    65,   573,
     574,   575,     0,     0,     0,     0,     0,     0,   576,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   577,   578,   579,     0,    15,
       0,     0,   580,   581,     0,     0,   531,   532,     0,     0,
     582,     0,   583,  1131,   584,   585,   533,   534,   535,   536,
     537,     0,     0,     0,     0,     0,   538,     0,   539,     0,
       0,     0,   540,     0,     0,     0,     0,     0,     0,     0,
     541,     0,     0,     0,     0,     0,   542,     0,     0,   543,
       0,     0,   544,     0,     0,     0,   545,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   546,     0,     0,   547,
     548,     0,   332,   333,   334,     0,   336,   337,   338,   339,
     340,   549,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,     0,   354,   355,   356,     0,     0,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   550,   551,   552,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   553,   554,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    64,     0,     0,
       0,     0,     0,     0,     0,   555,   556,   557,   558,   559,
       0,   560,     0,   561,   562,   563,   564,     0,     0,     0,
     565,   566,     0,   567,   568,   569,   570,   571,   572,    65,
     573,   574,   575,     0,     0,     0,     0,     0,     0,   576,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1273,     0,   577,   578,   579,     0,
      15,     0,     0,   580,   581,     0,     0,   531,   532,     0,
       0,   582,     0,   583,     0,   584,   585,   533,   534,   535,
     536,   537,     0,     0,     0,     0,     0,   538,     0,   539,
       0,     0,     0,   540,     0,     0,     0,     0,     0,     0,
       0,   541,     0,     0,     0,     0,     0,   542,     0,     0,
     543,     0,     0,   544,     0,     0,     0,   545,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   546,     0,     0,
     547,   548,     0,   332,   333,   334,     0,   336,   337,   338,
     339,   340,   549,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,     0,   354,   355,   356,     0,     0,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   374,   375,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   550,   551,   552,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   553,   554,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
       0,     0,     0,     0,     0,     0,   555,   556,   557,   558,
     559,     0,   560,     0,   561,   562,   563,   564,     0,     0,
       0,   565,   566,     0,   567,   568,   569,   570,   571,   572,
      65,   573,   574,   575,     0,     0,     0,     0,     0,     0,
     576,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   577,   578,   579,
       0,    15,     0,     0,   580,   581,     0,     0,   531,   532,
       0,     0,   582,     0,   583,  1546,   584,   585,   533,   534,
     535,   536,   537,     0,     0,     0,     0,     0,   538,     0,
     539,     0,     0,     0,   540,     0,     0,     0,     0,     0,
       0,     0,   541,     0,     0,     0,     0,     0,   542,     0,
       0,   543,     0,     0,   544,     0,     0,     0,   545,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   546,     0,
       0,   547,   548,     0,   332,   333,   334,     0,   336,   337,
     338,   339,   340,   549,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,     0,   354,   355,   356,     0,
       0,   359,   360,   361,   362,   363,   364,   365,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   550,   551,
     552,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   553,   554,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,   555,   556,   557,
     558,   559,     0,   560,     0,   561,   562,   563,   564,     0,
       0,     0,   565,   566,     0,   567,   568,   569,   570,   571,
     572,    65,   573,   574,   575,     0,     0,     0,     0,     0,
       0,   576,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   577,   578,
     579,     0,    15,     0,     0,   580,   581,     0,     0,   531,
     532,     0,     0,  1555,     0,   583,  1556,   584,   585,   533,
     534,   535,   536,   537,     0,     0,     0,     0,     0,   538,
       0,   539,     0,     0,     0,   540,     0,     0,     0,     0,
       0,     0,     0,   541,     0,     0,     0,     0,     0,   542,
       0,     0,   543,     0,     0,   544,     0,     0,     0,   545,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   546,
       0,     0,   547,   548,     0,   332,   333,   334,     0,   336,
     337,   338,   339,   340,   549,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,     0,   354,   355,   356,
       0,     0,   359,   360,   361,   362,   363,   364,   365,   366,
     367,   368,   369,   370,   371,   372,   373,   374,   375,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   550,
     551,   552,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   553,   554,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,     0,     0,     0,     0,     0,     0,   555,   556,
     557,   558,   559,     0,   560,     0,   561,   562,   563,   564,
       0,     0,     0,   565,   566,     0,   567,   568,   569,   570,
     571,   572,    65,   573,   574,   575,     0,     0,     0,     0,
       0,     0,   576,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   577,
     578,   579,     0,    15,     0,     0,   580,   581,     0,     0,
     531,   532,     0,     0,   582,     0,   583,  1561,   584,   585,
     533,   534,   535,   536,   537,     0,     0,     0,     0,     0,
     538,     0,   539,     0,     0,     0,   540,     0,     0,     0,
       0,     0,     0,     0,   541,     0,     0,     0,     0,     0,
     542,     0,     0,   543,     0,     0,   544,     0,     0,     0,
     545,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     546,     0,     0,   547,   548,     0,   332,   333,   334,     0,
     336,   337,   338,   339,   340,   549,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,     0,   354,   355,
     356,     0,     0,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     550,   551,   552,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   553,   554,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,   555,
     556,   557,   558,   559,     0,   560,     0,   561,   562,   563,
     564,     0,     0,     0,   565,   566,     0,   567,   568,   569,
     570,   571,   572,    65,   573,   574,   575,     0,     0,     0,
       0,     0,     0,   576,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     577,   578,   579,     0,    15,     0,     0,   580,   581,     0,
       0,   531,   532,     0,     0,   582,     0,   583,  1618,   584,
     585,   533,   534,   535,   536,   537,     0,     0,     0,     0,
       0,   538,     0,   539,     0,     0,     0,   540,     0,     0,
       0,     0,     0,     0,     0,   541,     0,     0,     0,     0,
       0,   542,     0,     0,   543,     0,     0,   544,     0,     0,
       0,   545,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   546,     0,     0,   547,   548,     0,   332,   333,   334,
       0,   336,   337,   338,   339,   340,   549,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,     0,   354,
     355,   356,     0,     0,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   550,   551,   552,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   553,   554,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    64,     0,     0,     0,     0,     0,     0,     0,
     555,   556,   557,   558,   559,     0,   560,     0,   561,   562,
     563,   564,     0,     0,     0,   565,   566,     0,   567,   568,
     569,   570,   571,   572,    65,   573,   574,   575,     0,     0,
       0,     0,     0,     0,   576,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   577,   578,   579,     0,    15,     0,     0,   580,   581,
       0,     0,   531,   532,     0,     0,   582,     0,   583,  1707,
     584,   585,   533,   534,   535,   536,   537,     0,     0,     0,
       0,     0,   538,     0,   539,     0,     0,     0,   540,     0,
       0,     0,     0,     0,     0,     0,   541,     0,     0,     0,
       0,     0,   542,     0,     0,   543,     0,     0,   544,     0,
       0,     0,   545,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   546,     0,     0,   547,   548,     0,   332,   333,
     334,     0,   336,   337,   338,   339,   340,   549,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,     0,
     354,   355,   356,     0,     0,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   550,   551,   552,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   553,   554,
       0,     0,     0,     0,     0,     0,     0,  1728,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,     0,     0,     0,     0,     0,
       0,   555,   556,   557,   558,   559,     0,   560,     0,   561,
     562,   563,   564,     0,     0,     0,   565,   566,     0,   567,
     568,   569,   570,   571,   572,    65,   573,   574,   575,     0,
       0,     0,     0,     0,     0,   576,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   577,   578,   579,     0,    15,     0,     0,   580,
     581,     0,     0,   531,   532,     0,     0,   582,     0,   583,
       0,   584,   585,   533,   534,   535,   536,   537,     0,     0,
       0,     0,     0,   538,     0,   539,     0,     0,     0,   540,
       0,     0,     0,     0,     0,     0,     0,   541,     0,     0,
       0,     0,     0,   542,     0,     0,   543,     0,     0,   544,
       0,     0,     0,   545,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   546,     0,     0,   547,   548,     0,   332,
     333,   334,     0,   336,   337,   338,   339,   340,   549,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
       0,   354,   355,   356,     0,     0,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   368,   369,   370,   371,   372,
     373,   374,   375,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   550,   551,   552,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   553,
     554,     0,     0,     0,     0,     0,     0,     0,  1788,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,   555,   556,   557,   558,   559,     0,   560,     0,
     561,   562,   563,   564,     0,     0,     0,   565,   566,     0,
     567,   568,   569,   570,   571,   572,    65,   573,   574,   575,
       0,     0,     0,     0,     0,     0,   576,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   577,   578,   579,     0,    15,     0,     0,
     580,   581,     0,     0,   531,   532,     0,     0,   582,     0,
     583,     0,   584,   585,   533,   534,   535,   536,   537,     0,
       0,     0,     0,     0,   538,     0,   539,     0,     0,     0,
     540,     0,     0,     0,     0,     0,     0,     0,   541,     0,
       0,     0,     0,     0,   542,     0,     0,   543,     0,     0,
     544,     0,     0,     0,   545,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   546,     0,     0,   547,   548,     0,
     332,   333,   334,     0,   336,   337,   338,   339,   340,   549,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,     0,   354,   355,   356,     0,     0,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   550,   551,   552,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     553,   554,     0,     0,     0,     0,     0,     0,     0,  1789,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,     0,     0,     0,
       0,     0,     0,   555,   556,   557,   558,   559,     0,   560,
       0,   561,   562,   563,   564,     0,     0,     0,   565,   566,
       0,   567,   568,   569,   570,   571,   572,    65,   573,   574,
     575,     0,     0,     0,     0,     0,     0,   576,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   577,   578,   579,     0,    15,     0,
       0,   580,   581,     0,     0,   531,   532,     0,     0,   582,
       0,   583,     0,   584,   585,   533,   534,   535,   536,   537,
       0,     0,     0,     0,     0,   538,     0,   539,     0,     0,
       0,   540,     0,     0,     0,     0,     0,     0,     0,   541,
       0,     0,     0,     0,     0,   542,     0,     0,   543,     0,
       0,   544,     0,     0,     0,   545,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   546,     0,     0,   547,   548,
       0,   332,   333,   334,     0,   336,   337,   338,   339,   340,
     549,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,     0,   354,   355,   356,     0,     0,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   550,   551,   552,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   553,   554,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,   555,   556,   557,   558,   559,     0,
     560,     0,   561,   562,   563,   564,     0,     0,     0,   565,
     566,     0,   567,   568,   569,   570,   571,   572,    65,   573,
     574,   575,     0,     0,     0,     0,     0,     0,   576,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   577,   578,   579,     0,    15,
       0,     0,   580,   581,     0,     0,   531,   532,     0,     0,
     582,     0,   583,     0,   584,   585,   533,   534,   535,   536,
     537,     0,     0,     0,     0,     0,   538,     0,   539,     0,
       0,     0,   540,     0,     0,     0,     0,     0,     0,     0,
     541,     0,     0,     0,     0,     0,   542,     0,     0,   543,
       0,     0,   544,     0,     0,     0,   545,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   546,     0,     0,   547,
     548,     0,   332,   333,   334,     0,   336,   337,   338,   339,
     340,   549,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,     0,   354,   355,   356,     0,     0,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   550,   551,   552,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   553,   554,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    64,     0,     0,
       0,     0,     0,     0,     0,   555,   556,   557,   558,   559,
       0,   560,     0,   561,   562,   563,   564,     0,     0,     0,
     565,   566,     0,   567,   568,   569,   570,   571,   572,    65,
     573,   574,   575,     0,   798,   833,   834,     0,     0,   576,
     326,     0,     0,     0,     0,     0,   327,     0,     0,     0,
       0,     0,   328,     0,     0,     0,   577,   578,   579,     0,
      15,     0,   329,   580,   581,     0,     0,     0,     0,     0,
     330,  1532,     0,   583,     0,   584,   585,     0,     0,     0,
       0,     0,     0,     0,     0,   331,     0,     0,     0,     0,
       0,     0,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,     0,     0,
       0,     0,     0,   835,   836,   837,   838,   839,     0,     0,
     840,   841,   842,   843,     0,   844,   845,   846,   847,     0,
       0,     0,     0,   848,     0,   849,   850,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    64,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     388,     0,     0,     0,     0,     0,     0,     0,   800,   833,
     834,     0,     0,     0,   326,     0,     0,     0,     0,    65,
     327,     0,     0,     0,     0,     0,   328,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   329,   859,   860,   861,
     862,   863,   864,   865,   330,     0,     0,     0,     0,     0,
       0,     0,     0,   866,   867,     0,     0,     0,     0,   331,
       0,     0,     0,     0,     0,   389,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,     0,     0,     0,     0,     0,   835,   836,   837,
     838,   839,     0,     0,   840,     0,     0,   843,     0,   844,
     845,   846,   847,     0,     0,     0,     0,   848,     0,   849,
     850,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   388,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   326,     0,
       0,     0,     0,    65,   327,     0,     0,     0,     0,     0,
     328,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     329,   859,   860,   861,   862,   863,   864,   865,   330,     0,
       0,     0,     0,     0,     0,     0,     0,   866,   867,     0,
       0,     0,     0,   331,     0,     0,     0,     0,     0,   389,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   388,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   326,     0,     0,    65,     0,     0,
     327,     0,     0,     0,     0,     0,   328,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   329,     0,     0,     0,
       0,     0,     0,     0,   330,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   331,
       0,     0,     0,   389,     0,   648,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   388,     0,     0,     0,     0,     0,
       0,     0,     0,    13,     0,     0,     0,     0,   326,     0,
       0,     0,     0,   651,   327,     0,     0,     0,     0,     0,
     328,     0,     0,     0,     0,    14,     0,     0,     0,     0,
     329,   652,     0,     0,     0,     0,     0,     0,   330,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   331,     0,     0,     0,     0,     0,   389,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   388,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   326,     0,     0,   833,   834,    65,   327,     0,
       0,     0,     0,     0,   328,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   329,     0,     0,     0,     0,     0,
       0,     0,   330,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   331,     0,     0,
       0,     0,     0,   389,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,   365,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
       0,     0,     0,     0,     0,  1137,  1138,  1139,  1140,  1141,
    1142,  1143,  1144,   835,   836,   837,   838,   839,  1145,  1146,
     840,   841,   842,   843,  1147,   844,   845,   846,   847,   833,
     834,     0,     0,   848,   976,   849,   850,  1148,  1149,    64,
       0,   851,   852,   853,  1150,  1151,  1152,   854,     0,     0,
       0,     0,   388,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    13,     0,   833,   834,     0,     0,     0,     0,
       0,   651,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    14,     0,     0,     0,     0,     0,
       0,  1153,   855,     0,   856,   857,   858,   859,   860,   861,
     862,   863,   864,   865,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   866,   867,     0,     0,   389,   584,   942,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1137,
    1138,  1139,  1140,  1141,  1142,  1143,  1144,   835,   836,   837,
     838,   839,  1145,  1146,   840,   841,   842,   843,  1147,   844,
     845,   846,   847,  -455,     0,     0,     0,   848,   976,   849,
     850,  1148,  1149,   833,   834,   851,   852,   853,  1150,  1151,
    1152,   854,   835,   836,   837,   838,   839,     0,     0,   840,
     841,   842,   843,     0,   844,   845,   846,   847,     0,     0,
       0,     0,   848,     0,   849,   850,     0,     0,   833,   834,
     851,   852,   853,     0,     0,     0,   854,     0,     0,     0,
       0,     0,     0,     0,     0,  1153,   855,     0,   856,   857,
     858,   859,   860,   861,   862,   863,   864,   865,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   866,   867,     0,
       0,     0,   584,   942,     0,     0,     0,     0,     0,     0,
       0,   855,     0,   856,   857,   858,   859,   860,   861,   862,
     863,   864,   865,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   866,   867,     0,     0,   881,     0,     0,     0,
       0,   835,   836,   837,   838,   839,     0,     0,   840,   841,
     842,   843,     0,   844,   845,   846,   847,     0,     0,     0,
       0,   848,     0,   849,   850,   833,   834,     0,     0,   851,
     852,   853,     0,     0,     0,   854,   835,   836,   837,   838,
     839,     0,     0,   840,   841,   842,   843,     0,   844,   845,
     846,   847,     0,     0,     0,     0,   848,     0,   849,   850,
     833,   834,     0,     0,   851,   852,   853,     0,     0,     0,
     854,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     855,     0,   856,   857,   858,   859,   860,   861,   862,   863,
     864,   865,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   866,   867,     0,     0,   897,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   855,     0,   856,   857,   858,
     859,   860,   861,   862,   863,   864,   865,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   866,   867,     0,     0,
    1183,     0,     0,   835,   836,   837,   838,   839,     0,     0,
     840,   841,   842,   843,     0,   844,   845,   846,   847,     0,
       0,     0,     0,   848,     0,   849,   850,   833,   834,     0,
       0,   851,   852,   853,     0,     0,     0,   854,   835,   836,
     837,   838,   839,     0,     0,   840,   841,   842,   843,     0,
     844,   845,   846,   847,     0,     0,     0,     0,   848,     0,
     849,   850,   833,   834,     0,     0,   851,   852,   853,     0,
       0,     0,   854,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   855,     0,   856,   857,   858,   859,   860,   861,
     862,   863,   864,   865,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   866,   867,     0,     0,  1286,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   855,     0,   856,
     857,   858,   859,   860,   861,   862,   863,   864,   865,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   866,   867,
       0,     0,  1289,     0,     0,   835,   836,   837,   838,   839,
       0,     0,   840,   841,   842,   843,     0,   844,   845,   846,
     847,     0,     0,     0,     0,   848,     0,   849,   850,   833,
     834,     0,     0,   851,   852,   853,     0,     0,     0,   854,
     835,   836,   837,   838,   839,     0,     0,   840,   841,   842,
     843,     0,   844,   845,   846,   847,     0,     0,     0,     0,
     848,     0,   849,   850,   833,   834,     0,     0,   851,   852,
     853,     0,     0,     0,   854,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   855,     0,   856,   857,   858,   859,
     860,   861,   862,   863,   864,   865,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   866,   867,     0,     0,  1291,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   855,
       0,   856,   857,   858,   859,   860,   861,   862,   863,   864,
     865,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     866,   867,     0,     0,  1299,     0,     0,   835,   836,   837,
     838,   839,     0,     0,   840,   841,   842,   843,     0,   844,
     845,   846,   847,     0,     0,     0,     0,   848,     0,   849,
     850,   833,   834,     0,     0,   851,   852,   853,     0,     0,
       0,   854,   835,   836,   837,   838,   839,     0,     0,   840,
     841,   842,   843,     0,   844,   845,   846,   847,     0,     0,
       0,     0,   848,     0,   849,   850,   833,   834,     0,     0,
     851,   852,   853,     0,     0,     0,   854,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   855,     0,   856,   857,
     858,   859,   860,   861,   862,   863,   864,   865,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   866,   867,     0,
       0,  1300,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   855,     0,   856,   857,   858,   859,   860,   861,   862,
     863,   864,   865,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   866,   867,     0,     0,  1301,     0,     0,   835,
     836,   837,   838,   839,     0,     0,   840,   841,   842,   843,
       0,   844,   845,   846,   847,     0,     0,     0,     0,   848,
       0,   849,   850,   833,   834,     0,     0,   851,   852,   853,
       0,     0,     0,   854,   835,   836,   837,   838,   839,     0,
       0,   840,   841,   842,   843,     0,   844,   845,   846,   847,
       0,     0,     0,     0,   848,     0,   849,   850,   833,   834,
       0,     0,   851,   852,   853,     0,     0,     0,   854,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   855,     0,
     856,   857,   858,   859,   860,   861,   862,   863,   864,   865,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   866,
     867,     0,     0,  1302,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   855,     0,   856,   857,   858,   859,   860,
     861,   862,   863,   864,   865,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   866,   867,     0,     0,  1303,     0,
       0,   835,   836,   837,   838,   839,     0,     0,   840,   841,
     842,   843,     0,   844,   845,   846,   847,     0,     0,     0,
       0,   848,     0,   849,   850,   833,   834,     0,     0,   851,
     852,   853,     0,     0,     0,   854,   835,   836,   837,   838,
     839,     0,     0,   840,   841,   842,   843,     0,   844,   845,
     846,   847,     0,     0,     0,     0,   848,     0,   849,   850,
     833,   834,     0,     0,   851,   852,   853,     0,     0,     0,
     854,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     855,     0,   856,   857,   858,   859,   860,   861,   862,   863,
     864,   865,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   866,   867,     0,     0,  1304,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   855,     0,   856,   857,   858,
     859,   860,   861,   862,   863,   864,   865,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   866,   867,     0,     0,
    1391,     0,     0,   835,   836,   837,   838,   839,     0,     0,
     840,   841,   842,   843,     0,   844,   845,   846,   847,     0,
       0,     0,     0,   848,     0,   849,   850,   833,   834,     0,
       0,   851,   852,   853,     0,     0,     0,   854,   835,   836,
     837,   838,   839,     0,     0,   840,   841,   842,   843,     0,
     844,   845,   846,   847,     0,     0,     0,     0,   848,     0,
     849,   850,   833,   834,     0,     0,   851,   852,   853,     0,
       0,     0,   854,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   855,     0,   856,   857,   858,   859,   860,   861,
     862,   863,   864,   865,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   866,   867,     0,     0,  1474,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   855,     0,   856,
     857,   858,   859,   860,   861,   862,   863,   864,   865,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   866,   867,
       0,     0,  1477,     0,     0,   835,   836,   837,   838,   839,
       0,     0,   840,   841,   842,   843,     0,   844,   845,   846,
     847,     0,     0,     0,     0,   848,     0,   849,   850,   833,
     834,     0,     0,   851,   852,   853,     0,     0,     0,   854,
     835,   836,   837,   838,   839,     0,     0,   840,   841,   842,
     843,     0,   844,   845,   846,   847,     0,     0,     0,     0,
     848,     0,   849,   850,   833,   834,     0,     0,   851,   852,
     853,     0,     0,     0,   854,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   855,     0,   856,   857,   858,   859,
     860,   861,   862,   863,   864,   865,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   866,   867,     0,     0,  1522,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   855,
       0,   856,   857,   858,   859,   860,   861,   862,   863,   864,
     865,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     866,   867,     0,     0,  1572,     0,     0,   835,   836,   837,
     838,   839,     0,     0,   840,   841,   842,   843,     0,   844,
     845,   846,   847,     0,     0,     0,     0,   848,     0,   849,
     850,   833,   834,     0,     0,   851,   852,   853,     0,     0,
       0,   854,   835,   836,   837,   838,   839,     0,     0,   840,
     841,   842,   843,     0,   844,   845,   846,   847,     0,     0,
       0,     0,   848,     0,   849,   850,   833,   834,     0,     0,
     851,   852,   853,     0,     0,     0,   854,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   855,     0,   856,   857,
     858,   859,   860,   861,   862,   863,   864,   865,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   866,   867,     0,
       0,  1639,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   855,     0,   856,   857,   858,   859,   860,   861,   862,
     863,   864,   865,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   866,   867,     0,     0,  1640,     0,     0,   835,
     836,   837,   838,   839,     0,     0,   840,   841,   842,   843,
       0,   844,   845,   846,   847,     0,     0,     0,     0,   848,
       0,   849,   850,   833,   834,     0,     0,   851,   852,   853,
       0,     0,     0,   854,   835,   836,   837,   838,   839,     0,
       0,   840,   841,   842,   843,     0,   844,   845,   846,   847,
       0,     0,     0,     0,   848,     0,   849,   850,   833,   834,
       0,     0,   851,   852,   853,     0,     0,     0,   854,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   855,     0,
     856,   857,   858,   859,   860,   861,   862,   863,   864,   865,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   866,
     867,     0,     0,  1653,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   855,     0,   856,   857,   858,   859,   860,
     861,   862,   863,   864,   865,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   866,   867,     0,     0,  1655,     0,
       0,   835,   836,   837,   838,   839,     0,     0,   840,   841,
     842,   843,     0,   844,   845,   846,   847,     0,     0,     0,
       0,   848,     0,   849,   850,   833,   834,     0,     0,   851,
     852,   853,     0,     0,     0,   854,   835,   836,   837,   838,
     839,     0,     0,   840,   841,   842,   843,     0,   844,   845,
     846,   847,     0,     0,     0,     0,   848,     0,   849,   850,
     833,   834,     0,     0,   851,   852,   853,     0,     0,     0,
     854,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     855,     0,   856,   857,   858,   859,   860,   861,   862,   863,
     864,   865,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   866,   867,     0,     0,  1657,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   855,     0,   856,   857,   858,
     859,   860,   861,   862,   863,   864,   865,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   866,   867,     0,     0,
    1661,     0,     0,   835,   836,   837,   838,   839,     0,     0,
     840,   841,   842,   843,     0,   844,   845,   846,   847,     0,
       0,     0,     0,   848,     0,   849,   850,   833,   834,     0,
       0,   851,   852,   853,     0,     0,     0,   854,   835,   836,
     837,   838,   839,     0,     0,   840,   841,   842,   843,     0,
     844,   845,   846,   847,     0,     0,     0,     0,   848,     0,
     849,   850,   833,   834,     0,     0,   851,   852,   853,     0,
       0,     0,   854,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   855,     0,   856,   857,   858,   859,   860,   861,
     862,   863,   864,   865,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   866,   867,     0,     0,  1723,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   855,     0,   856,
     857,   858,   859,   860,   861,   862,   863,   864,   865,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   866,   867,
       0,     0,  1733,     0,     0,   835,   836,   837,   838,   839,
       0,     0,   840,   841,   842,   843,     0,   844,   845,   846,
     847,     0,     0,     0,     0,   848,     0,   849,   850,   833,
     834,     0,     0,   851,   852,   853,     0,     0,     0,   854,
     835,   836,   837,   838,   839,     0,     0,   840,   841,   842,
     843,     0,   844,   845,   846,   847,     0,     0,     0,     0,
     848,     0,   849,   850,   833,   834,     0,     0,   851,   852,
     853,     0,     0,     0,   854,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   855,     0,   856,   857,   858,   859,
     860,   861,   862,   863,   864,   865,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   866,   867,     0,     0,  1734,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   855,
       0,   856,   857,   858,   859,   860,   861,   862,   863,   864,
     865,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     866,   867,     0,     0,  1736,     0,     0,   835,   836,   837,
     838,   839,     0,     0,   840,   841,   842,   843,     0,   844,
     845,   846,   847,     0,     0,     0,     0,   848,     0,   849,
     850,   833,   834,     0,     0,   851,   852,   853,     0,     0,
       0,   854,   835,   836,   837,   838,   839,     0,     0,   840,
     841,   842,   843,     0,   844,   845,   846,   847,     0,     0,
       0,     0,   848,     0,   849,   850,   833,   834,     0,     0,
     851,   852,   853,     0,     0,     0,   854,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   855,     0,   856,   857,
     858,   859,   860,   861,   862,   863,   864,   865,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   866,   867,     0,
       0,  1737,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   855,     0,   856,   857,   858,   859,   860,   861,   862,
     863,   864,   865,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   866,   867,     0,     0,  1757,     0,     0,   835,
     836,   837,   838,   839,     0,     0,   840,   841,   842,   843,
       0,   844,   845,   846,   847,     0,     0,     0,     0,   848,
       0,   849,   850,   833,   834,     0,     0,   851,   852,   853,
       0,     0,     0,   854,   835,   836,   837,   838,   839,     0,
       0,   840,   841,   842,   843,     0,   844,   845,   846,   847,
       0,     0,     0,     0,   848,     0,   849,   850,   833,   834,
       0,     0,   851,   852,   853,     0,     0,     0,   854,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   855,     0,
     856,   857,   858,   859,   860,   861,   862,   863,   864,   865,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   866,
     867,     0,     0,  1760,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   855,     0,   856,   857,   858,   859,   860,
     861,   862,   863,   864,   865,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   866,   867,     0,     0,  1769,     0,
       0,   835,   836,   837,   838,   839,     0,     0,   840,   841,
     842,   843,     0,   844,   845,   846,   847,     0,     0,     0,
       0,   848,     0,   849,   850,   833,   834,     0,     0,   851,
     852,   853,     0,     0,     0,   854,   835,   836,   837,   838,
     839,     0,     0,   840,   841,   842,   843,     0,   844,   845,
     846,   847,     0,     0,     0,     0,   848,     0,   849,   850,
     833,   834,     0,     0,   851,   852,   853,     0,     0,     0,
     854,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     855,     0,   856,   857,   858,   859,   860,   861,   862,   863,
     864,   865,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   866,   867,     0,     0,  1840,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   855,     0,   856,   857,   858,
     859,   860,   861,   862,   863,   864,   865,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   866,   867,     0,     0,
    1841,     0,     0,   835,   836,   837,   838,   839,     0,     0,
     840,   841,   842,   843,     0,   844,   845,   846,   847,     0,
       0,     0,     0,   848,     0,   849,   850,   833,   834,     0,
       0,   851,   852,   853,     0,     0,     0,   854,   835,   836,
     837,   838,   839,     0,     0,   840,   841,   842,   843,     0,
     844,   845,   846,   847,     0,     0,     0,     0,   848,     0,
     849,   850,   833,   834,     0,     0,   851,   852,   853,     0,
       0,     0,   854,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   855,     0,   856,   857,   858,   859,   860,   861,
     862,   863,   864,   865,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   866,   867,   901,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   855,     0,   856,
     857,   858,   859,   860,   861,   862,   863,   864,   865,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   866,   867,
    1177,     0,     0,     0,     0,   835,   836,   837,   838,   839,
       0,     0,   840,   841,   842,   843,     0,   844,   845,   846,
     847,     0,     0,     0,     0,   848,     0,   849,   850,   833,
     834,     0,     0,   851,   852,   853,     0,     0,     0,   854,
     835,   836,   837,   838,   839,     0,     0,   840,   841,   842,
     843,     0,   844,   845,   846,   847,     0,     0,     0,     0,
     848,     0,   849,   850,   833,   834,     0,     0,   851,   852,
     853,     0,     0,     0,   854,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   855,     0,   856,   857,   858,   859,
     860,   861,   862,   863,   864,   865,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   866,   867,  1373,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   855,
       0,   856,   857,   858,   859,   860,   861,   862,   863,   864,
     865,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     866,   867,  1389,     0,     0,     0,     0,   835,   836,   837,
     838,   839,     0,     0,   840,   841,   842,   843,     0,   844,
     845,   846,   847,     0,     0,     0,     0,   848,     0,   849,
     850,     0,     0,     0,     0,   851,   852,   853,     0,     0,
       0,   854,   835,   836,   837,   838,   839,     0,     0,   840,
     841,   842,   843,     0,   844,   845,   846,   847,   404,   405,
       0,     0,   848,     0,   849,   850,     0,     0,     0,     0,
     851,   852,   853,     0,     0,   406,   854,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   855,     0,   856,   857,
     858,   859,   860,   861,   862,   863,   864,   865,     0,     0,
       0,     0,   833,   834,     0,     0,     0,   866,   867,  1553,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   855,     0,   856,   857,   858,   859,   860,   861,   862,
     863,   864,   865,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   866,   867,  1559,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   407,   408,
     409,   410,   411,   412,   413,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   423,   424,     0,     0,   425,   426,
     427,     0,     0,     0,     0,     0,     0,   428,   429,   430,
     431,   432,     0,     0,   433,   434,   435,   436,   437,   438,
     439,   833,   834,     0,     0,     0,     0,     0,     0,     0,
     835,   836,   837,   838,   839,     0,     0,   840,   841,   842,
     843,     0,   844,   845,   846,   847,     0,     0,     0,     0,
     848,     0,   849,   850,     0,     0,     0,     0,   851,   852,
     853,     0,     0,     0,   854,   440,     0,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,    52,     0,   451,
     452,     0,     0,     0,     0,     0,   453,   454,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   855,
       0,   856,   857,   858,   859,   860,   861,   862,   863,   864,
     865,     0,     0,   833,   834,     0,     0,     0,     0,     0,
     866,   867,     0,     0,     0,     0,     0,     0,     0,   835,
     836,   837,   838,   839,     0,     0,   840,   841,   842,   843,
       0,   844,   845,   846,   847,     0,     0,     0,     0,   848,
       0,   849,   850,   833,   834,     0,     0,   851,   852,   853,
       0,     0,     0,   854,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    13,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      14,     0,     0,     0,     0,     0,     0,     0,   855,     0,
     856,   857,   858,   859,   860,   861,   862,   863,   864,   865,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   866,
     867,   835,   836,   837,   838,   839,     0,     0,   840,   841,
     842,   843,     0,   844,   845,   846,   847,     0,   833,   834,
       0,   848,     0,   849,   850,     0,     0,  1041,     0,   851,
     852,   853,     0,     0,     0,   854,     0,     0,     0,     0,
       0,   835,   836,   837,   838,   839,     0,     0,   840,   841,
     842,   843,     0,   844,   845,   846,   847,   833,   834,     0,
       0,   848,     0,   849,   850,     0,     0,  1310,     0,   851,
     852,   853,     0,     0,     0,   854,     0,     0,     0,     0,
     855,     0,   856,   857,   858,   859,   860,   861,   862,   863,
     864,   865,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   866,   867,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     855,     0,   856,   857,   858,   859,   860,   861,   862,   863,
     864,   865,     0,     0,     0,     0,   835,   836,   837,   838,
     839,   866,   867,   840,   841,   842,   843,     0,   844,   845,
     846,   847,   833,   834,     0,     0,   848,     0,   849,   850,
       0,     0,     0,     0,   851,   852,   853,     0,     0,     0,
     854,     0,     0,     0,     0,   835,   836,   837,   838,   839,
       0,     0,   840,   841,   842,   843,     0,   844,   845,   846,
     847,   833,   834,     0,     0,   848,     0,   849,   850,     0,
       0,     0,     0,   851,   852,   853,     0,     0,     0,   854,
       0,     0,     0,     0,     0,   855,  1378,   856,   857,   858,
     859,   860,   861,   862,   863,   864,   865,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   866,   867,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   855,  1517,   856,   857,   858,   859,
     860,   861,   862,   863,   864,   865,     0,     0,     0,     0,
     835,   836,   837,   838,   839,   866,   867,   840,   841,   842,
     843,     0,   844,   845,   846,   847,   833,   834,     0,     0,
     848,     0,   849,   850,     0,     0,     0,     0,   851,   852,
     853,     0,     0,     0,   854,     0,     0,     0,     0,   835,
     836,   837,   838,   839,     0,     0,   840,   841,   842,   843,
       0,   844,   845,   846,   847,   833,   834,     0,     0,   848,
       0,   849,   850,     0,     0,     0,     0,   851,   852,   853,
       0,  1770,     0,   854,     0,     0,     0,     0,     0,   855,
       0,   856,   857,   858,   859,   860,   861,   862,   863,   864,
     865,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     866,   867,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   833,   834,     0,     0,     0,     0,     0,   855,     0,
     856,   857,   858,   859,   860,   861,   862,   863,   864,   865,
       0,     0,     0,     0,   835,   836,   837,   838,   839,   866,
     867,   840,   841,   842,   843,     0,   844,   845,   846,   847,
     833,   834,     0,     0,   848,     0,   849,   850,     0,     0,
       0,     0,   851,   852,   853,     0,     0,     0,  -898,     0,
       0,     0,     0,   835,   836,   837,   838,   839,     0,     0,
     840,   841,   842,   843,     0,   844,   845,   846,   847,     0,
       0,     0,     0,   848,     0,   849,   850,     0,     0,     0,
       0,   851,     0,   853,     0,     0,   833,   834,     0,     0,
       0,     0,     0,   855,     0,   856,   857,   858,   859,   860,
     861,   862,   863,   864,   865,     0,     0,     0,     0,   835,
     836,   837,   838,   839,   866,   867,   840,   841,   842,   843,
       0,   844,   845,   846,   847,     0,     0,     0,     0,   848,
       0,   849,   850,     0,   856,   857,   858,   859,   860,   861,
     862,   863,   864,   865,     0,     0,     0,     0,   835,   836,
     837,   838,   839,   866,   867,   840,   841,   842,   843,     0,
     844,   845,   846,   847,     0,     0,     0,     0,   848,     0,
     849,   850,     0,     0,     0,     0,   851,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     856,   857,   858,   859,   860,   861,   862,   863,   864,   865,
       0,     0,     0,     0,   835,   836,   837,   838,   839,   866,
     867,   840,   841,   842,   843,     0,   844,   845,   846,   847,
       0,     0,     0,     0,   848,     0,   849,   850,     0,   856,
     857,   858,   859,   860,   861,   862,   863,   864,   865,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   866,   867,
       0,     0,     0,     0,  1082,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   857,   858,   859,   860,
     861,   862,   863,   864,   865,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   866,   867,   332,   333,   334,  1086,
     336,   337,   338,   339,   340,   549,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,     0,   354,   355,
     356,     0,     0,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
       0,   332,   333,   334,     0,   336,   337,   338,   339,   340,
     549,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,     0,   354,   355,   356,     0,     0,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,     0,  1083,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1084,     0,     0,     0,  1374,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1087,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1088,   332,
     333,   334,     0,   336,   337,   338,   339,   340,   549,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
       0,   354,   355,   356,     0,     0,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   368,   369,   370,   371,   372,
     373,   374,   375,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   332,   333,   334,     0,   336,   337,   338,
     339,   340,   549,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,     0,   354,   355,   356,     0,     0,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   374,   375,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,     0,     0,  1375,
       0,     0,     0,     0,     0,     0,     0,     0,   247,     0,
       0,     0,     0,     0,     0,     0,  1376,     0,     0,     0,
       0,     0,     0,     0,  1122,  1123,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   248,     0,   249,     0,   250,   251,
     252,   253,   254,  1124,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,     0,   266,   267,   268,     0,
    1125,   269,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   294,   295,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1126,  1127,     0,   296,   297,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   298
};

static const yytype_int16 yycheck[] =
{
       1,   614,   637,   522,  1015,   241,   225,   618,   822,     7,
     736,   776,   826,   827,   564,    15,    16,   917,    19,   241,
     300,   628,   504,   630,   505,   632,   185,   522,   504,   505,
     831,   888,   528,   890,   801,   892,   473,   467,  1313,   522,
     718,   719,  1032,  1017,  1206,    87,    22,    20,  1038,     5,
      33,    53,   798,    20,   800,     4,   802,    20,    19,    20,
     113,    20,    34,   809,   149,   502,   799,   149,   801,    46,
      70,    71,    72,   819,    53,    20,     8,  1589,    20,    20,
      33,    57,  1682,   183,   150,   899,     5,     6,   167,   168,
     169,    63,   703,   150,   160,   166,   188,   175,   594,   100,
     149,   102,   149,   152,   153,   716,    25,   210,    40,   223,
     213,   149,    31,   181,   114,   115,   116,   117,   210,   216,
     198,   217,   149,   618,   216,   562,   563,   198,   242,   166,
     198,   150,   224,   131,   219,   618,  1298,   219,   241,   224,
      33,   160,    57,   150,  1569,   245,   243,   166,    63,    68,
      69,   217,   129,   160,  1754,   150,   315,   242,   130,   160,
     217,   198,  1724,   220,   243,   160,  1678,    60,    61,   125,
     126,   583,   219,   226,   242,   178,   922,   224,   654,   181,
     183,   219,   198,   659,   182,   227,   224,   159,   198,  1003,
     239,   240,   219,   994,   166,   242,   198,   224,   217,  1013,
     189,     7,  1016,  1628,   242,   242,   125,   126,   703,   167,
     217,   169,   152,   153,    55,   242,    62,   215,   216,  1781,
     703,   716,   217,  1712,  1499,  1714,   198,   228,   244,  1718,
    1719,   210,   242,   716,   183,    21,    22,   198,   150,   222,
     243,   241,   245,   216,    50,   210,   244,   166,   160,   216,
     216,   473,   228,   216,   147,   187,   188,   216,   151,   224,
     101,   233,   873,   239,   183,   184,   185,   223,  1279,   222,
     242,   216,   187,  1074,   216,   216,   318,   209,   210,   198,
     502,  1443,   504,   519,   520,   963,   242,   240,  1777,   208,
    1452,   703,   150,   246,   209,   188,   219,   519,   520,   239,
     240,   165,   160,   522,   223,   217,  1280,   526,   240,   528,
    1056,   309,   923,   178,   312,   213,   214,   210,   183,  1309,
     181,   240,   186,   216,   166,   218,   216,   175,   939,   222,
     216,   224,   225,     5,     6,   183,     8,   198,   775,   188,
     562,   563,  1075,   241,   208,   216,  1621,   240,  1026,   150,
     198,   241,   789,   246,   186,   141,   142,   243,  1614,   217,
     228,   210,   774,   149,    36,   151,   152,   153,   154,   150,
     186,   228,   784,   159,   198,   787,   208,   219,   873,   160,
     245,   188,   243,   619,   620,   621,   188,   623,   624,   389,
     873,   627,   208,   629,   242,   631,   245,   619,   620,   621,
     155,   623,   624,   210,   880,   627,   175,   629,   210,   631,
     217,   633,   244,   245,   216,   216,   217,   198,   219,   831,
     896,   222,   224,    47,   175,   705,   242,   166,   923,   198,
     188,  1687,  1688,   188,   714,   219,   217,   186,   657,   223,
     923,   222,   661,   242,   939,   216,  1346,   198,   944,  1705,
    1706,   173,   210,   239,   240,   210,   939,   869,   216,   208,
      47,   462,   220,   463,    21,    22,    90,    91,  1269,    93,
     219,   130,   243,   473,  1081,   216,   198,   199,   200,   130,
      67,   130,   965,   966,   967,   968,   969,   970,   971,   972,
     973,   974,   975,  1043,   977,   978,   979,   980,   981,   982,
     241,   938,   502,  1053,   504,  1761,  1762,   229,  1498,  1366,
     220,   923,   186,   166,   224,   516,   517,   239,  1508,   519,
     520,   522,   175,   524,   188,    15,    16,   528,  1004,   166,
     223,   223,   962,   173,   208,   535,   536,  1437,   175,  1217,
    1218,  1219,  1309,   180,   188,   198,   210,   188,   219,   242,
     242,   222,   188,   775,   991,   992,   220,  1324,   198,   199,
     200,   198,   562,   563,   189,  1002,   210,   789,   186,   210,
    1007,  1008,   216,  1010,   210,  1012,  1309,  1014,   188,  1312,
     224,   217,   994,  1756,   141,   142,   223,   181,   228,   229,
     208,  1324,   149,   175,  1767,   152,   153,   154,   216,   239,
     210,   760,   159,   216,   198,   216,   216,   239,    70,    71,
      72,   833,   834,   614,   224,   188,   198,  1029,   188,   619,
     620,   621,  1522,   623,   624,  1037,   848,   627,   198,   629,
     243,   631,   243,   633,  1807,  1808,   188,   210,   188,   188,
     210,   921,   216,   216,   866,   242,   216,   244,   245,   650,
     216,   931,   114,   115,   116,   117,    15,    16,   210,   187,
     210,   210,   898,   198,   216,   216,   216,   216,   948,   243,
     906,   951,  1529,   909,   224,   224,   898,   243,   198,   216,
     216,   209,   239,   240,   906,   198,   905,   909,  1170,   216,
     188,  1172,   243,   219,   223,   216,   222,  1173,   216,   189,
     190,   216,   192,   193,   194,   195,   243,   243,   198,   216,
     711,   201,   210,   242,   241,  1211,   938,   198,   216,  1484,
     721,   722,   243,  1580,   216,   243,   224,   223,   243,   202,
     203,   181,   188,   216,   241,   225,    33,   738,   739,   740,
     741,   216,   743,  1350,   244,   245,   242,   748,   198,   241,
     751,   188,   242,   216,   210,   245,   756,   993,   241,   198,
     216,   202,   998,    60,    61,   984,   241,   767,   224,   991,
     992,   993,   188,   210,   188,   775,   998,   198,   241,   216,
    1002,   188,   187,   188,   198,  1007,  1008,   224,  1010,   789,
    1012,   245,  1014,  1015,   210,   181,   210,   181,    79,  1532,
     216,    57,   220,   210,   209,   188,   224,    63,   224,   216,
    1046,  1047,   198,    94,   198,   198,   198,   224,    99,  1676,
     101,  1297,  1555,  1723,  1046,  1047,   181,   210,    57,   829,
     189,   190,    33,   192,   193,   194,   195,  1444,   233,   198,
      12,  1278,   201,   198,    75,  1046,  1047,  1284,    79,     0,
     147,    23,    24,  1579,   151,    57,     7,  1333,  1334,    60,
      61,    63,    93,    94,    96,    97,   225,    98,    99,   100,
     101,   189,   190,   191,    57,   876,    57,   195,    57,    30,
      63,    32,    63,    34,    63,   202,   245,   167,  1110,    40,
     189,   190,   191,  1305,   216,   245,   195,   219,   898,    50,
     222,   209,   903,   129,   212,    56,   906,   215,    57,   909,
      57,  1323,  1349,   198,    63,  1195,    63,  1329,    66,   220,
     198,   218,  1158,   224,  1336,   222,  1338,   224,   225,    80,
     217,   210,  1451,  1698,   210,   210,  1158,   216,   938,   210,
     216,   216,   219,   240,   198,   216,   147,   210,  1170,   246,
     151,  1187,  1766,   216,   220,   953,  1451,   210,   224,  1371,
     220,    35,    35,   216,   224,  1187,    21,    22,  1451,   220,
     242,  1383,   219,   224,   125,   126,  1388,   220,   220,  1586,
     242,   224,   224,    43,   985,   223,   220,  1801,   223,   990,
     224,   991,   992,   993,   242,   220,   220,  1473,   998,   224,
     224,   223,  1002,   209,   210,  1481,   212,  1007,  1008,   223,
    1010,   223,  1012,   223,  1014,  1015,   223,   218,   198,   199,
     200,   222,  1755,  1756,   225,   242,    33,  1439,   223,   223,
    1763,   167,   168,   169,  1767,  1768,    33,   188,   223,   240,
     202,   203,   204,   205,   242,   246,  1046,  1047,   202,   203,
     204,   220,   198,    60,    61,   198,  1278,  1279,   198,   210,
      22,  1696,  1284,    60,    61,   241,  1799,  1680,    21,    22,
     202,   203,   204,   198,  1807,  1808,  1626,   241,   229,   223,
     241,  1631,   198,   198,  1496,   240,   141,   142,   198,   240,
     209,   210,   211,   243,   149,   223,   151,   152,   153,   154,
     692,   693,   694,   242,   159,  1542,    10,    11,    12,  1521,
     223,   223,   242,   242,   223,   223,   131,   132,   133,   134,
     135,   136,   137,   138,   223,  1858,   223,  1349,   242,   223,
     223,   242,   223,   223,    43,  1136,   242,  1750,  1751,   242,
     147,  1752,   242,   150,   151,   160,   242,   242,   240,   224,
     147,   198,   241,   160,   151,   170,   171,   172,  1158,   245,
     243,  1162,   242,   242,   198,  1445,   219,   189,    10,   223,
    1170,    37,   227,   228,   229,    66,     8,   242,   223,  1792,
     223,   188,  1793,  1794,   239,   240,    33,  1187,   141,   142,
     223,   209,   216,    13,  1430,   241,   149,   216,   151,   152,
     153,   154,   242,   210,   216,  1441,   159,   243,  1430,   198,
     217,   218,   198,    60,    61,   222,   155,  1828,   225,  1441,
     224,   218,   198,  1459,   216,   222,   242,   224,   225,    43,
      14,   470,  1451,   240,   216,   198,   217,  1459,   189,   246,
     479,   219,  1679,   240,   245,    67,   198,   243,   198,   246,
     489,   216,   216,   209,   223,   243,   243,  1752,  1259,   242,
     499,  1541,   242,   242,   223,   242,     1,   242,   242,  1752,
     242,   242,   225,   226,   227,   228,   229,   224,  1278,  1279,
     224,   242,   242,   522,  1284,   243,   239,   240,   224,   217,
    1702,   198,   198,   217,   243,   243,  1576,   198,  1793,  1794,
     147,   243,  1582,   150,   151,   242,   198,   242,  1544,   243,
    1793,  1794,  1749,   160,   553,   554,   243,   242,   241,   241,
    1542,   241,  1544,   198,   243,   564,   242,   198,  1740,   198,
     223,   242,   242,  1828,   242,   198,   198,   576,   577,   578,
     579,   580,   581,   242,    43,  1828,    33,   243,   198,  1349,
     206,   224,   242,   242,   241,   243,   241,   198,   242,    12,
     198,   198,   241,   198,   242,   242,   242,    70,    21,    22,
     217,   218,   224,   243,   242,   222,  1788,   242,   225,   618,
     224,   242,   242,   242,   131,   132,   133,   134,   135,   136,
     137,   138,  1672,   240,   243,   210,   242,   242,   202,   246,
     243,    53,   242,   150,   243,   216,   209,   243,   647,   243,
     241,   209,   209,   160,   241,   243,   209,   243,   243,   243,
     243,   242,  1658,   170,   171,   172,   243,   243,   241,   241,
    1430,  1432,   241,   243,   808,    86,  1658,     1,   152,    89,
    1673,  1441,  1674,   238,   756,  1210,  1674,  1674,  1674,   688,
    1451,  1674,     1,  1213,  1672,  1539,  1358,  1679,  1462,  1459,
     699,  1593,  1262,  1594,   703,  1594,  1465,   706,  1748,   708,
      58,   523,   535,   535,   713,  1768,  1624,   716,   801,  1319,
      -1,   720,    -1,   341,    -1,   724,   139,   140,   141,   142,
     143,    -1,    -1,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,
      -1,    -1,    -1,   752,   167,   168,   169,    -1,  1798,    -1,
     173,    -1,    -1,    -1,    -1,    -1,    -1,  1749,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   776,   777,    -1,
      -1,   780,  1542,   782,  1544,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   792,   793,   794,   795,   796,   797,    -1,
     799,    -1,   801,    -1,    -1,   218,    -1,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,    -1,    -1,    -1,
      -1,    -1,    -1,  1584,    -1,    -1,   239,   240,    -1,  1590,
      -1,    -1,    -1,    -1,    -1,    -1,   835,   836,    -1,    -1,
     839,   840,   841,   842,    -1,   844,    -1,   846,   847,   848,
     849,   850,   851,   852,   853,   854,   855,   856,   857,   858,
     859,   860,   861,   862,   863,   864,   865,    -1,   867,    -1,
      -1,   870,    -1,    -1,   873,   717,   718,   719,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   736,    -1,    -1,    -1,  1658,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   750,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1679,
      -1,    -1,    -1,    33,   923,    -1,    -1,    -1,    -1,   928,
      -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    -1,    -1,
     939,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1724,    -1,   965,   966,   967,   968,
     969,   970,   971,   972,   973,   974,   975,   976,   977,   978,
     979,   980,   981,   982,    -1,    -1,    -1,    -1,    -1,  1749,
      -1,    33,  1753,    -1,    -1,    -1,   995,    -1,   997,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,    60,    61,
      -1,  1781,   159,   147,   161,   162,    -1,   151,  1027,  1028,
      -1,    -1,    -1,    -1,   876,  1796,    -1,   147,    -1,    -1,
      -1,   151,  1041,    -1,  1043,    -1,    -1,    -1,    -1,    -1,
    1049,    -1,    -1,    -1,  1053,    -1,    -1,    -1,    -1,  1058,
      -1,  1060,    -1,  1062,    -1,  1064,    -1,    -1,    -1,  1830,
      -1,  1832,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   926,   222,   223,   224,   225,   226,
     227,   228,   229,  1854,   218,    -1,    21,    22,   222,   941,
     224,   225,   239,   240,    -1,   147,    -1,    -1,   218,   151,
      -1,    -1,   222,    -1,   224,   225,   240,    -1,    -1,    -1,
      -1,   963,   246,    10,  1123,    -1,    -1,    -1,  1127,    -1,
     240,    -1,    -1,    -1,    21,    22,   246,    -1,  1137,  1138,
    1139,  1140,  1141,  1142,  1143,  1144,  1145,  1146,  1147,  1148,
    1149,  1150,  1151,  1152,  1153,   966,   967,   968,   969,   970,
     971,   972,   973,   974,   975,    -1,   977,   978,   979,   980,
     981,   982,    -1,    -1,    33,    -1,   218,    -1,    -1,  1178,
     222,    -1,   224,   225,  1026,  1184,    -1,    -1,    -1,    -1,
    1189,    -1,    -1,    -1,    -1,  1194,    -1,    -1,   240,    -1,
      -1,    60,    61,    -1,   246,    -1,    -1,    -1,    -1,    -1,
    1209,    -1,    -1,    -1,   139,   140,   141,   142,    -1,    -1,
      -1,    -1,    -1,    -1,   149,    -1,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,   159,    -1,   161,   162,    -1,    -1,
      -1,    -1,    -1,    -1,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,   159,   160,   161,   162,   163,   164,    -1,    33,
     167,   168,   169,   170,   171,   172,   173,    -1,   147,    -1,
      -1,    -1,   151,    -1,  1136,    -1,    -1,    -1,    21,    22,
     225,   226,   227,   228,   229,    -1,    60,    61,    -1,    -1,
      -1,  1310,    -1,    -1,   239,   240,    -1,    -1,    -1,  1318,
    1319,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,
     217,   218,    -1,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,    -1,    -1,    -1,    -1,    -1,  1347,  1348,
      60,    61,   239,   240,    -1,    -1,  1355,   244,   245,   218,
      -1,    -1,  1204,   222,    -1,   224,   225,    -1,    -1,  1368,
      -1,  1370,    -1,  1372,    -1,  1217,  1218,  1219,    -1,  1378,
    1222,   240,  1224,  1382,  1226,    -1,  1228,   246,  1230,    -1,
    1232,    -1,  1234,   147,  1236,    -1,  1238,   151,  1240,    -1,
    1242,    -1,    -1,  1245,    -1,  1247,    -1,  1249,    -1,  1251,
      -1,  1253,    -1,  1255,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,   147,   161,   162,
      -1,   151,    -1,  1442,   167,   168,   169,    -1,    -1,  1448,
     173,    -1,  1451,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,   218,    -1,    -1,    -1,   222,    -1,
     224,   225,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1484,   240,    -1,    -1,    -1,
      -1,    -1,   246,    -1,    -1,   218,    -1,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,    -1,   218,    -1,
      -1,    -1,   222,    -1,   224,   225,   239,   240,    -1,    -1,
      -1,   244,   245,    -1,  1523,  1524,  1525,    -1,    33,    -1,
     240,    -1,    -1,    33,    -1,    -1,   246,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1550,    -1,  1552,    -1,    60,    61,    -1,    -1,  1558,
      60,    61,    -1,    -1,    21,    22,    -1,    -1,    60,    61,
      -1,    -1,    -1,    -1,  1573,    -1,  1575,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,
    1599,   161,   162,  1602,    -1,    -1,    -1,   167,   168,   169,
      -1,  1610,  1611,  1612,    -1,    -1,    -1,    -1,  1617,    -1,
      -1,  1620,    -1,    -1,  1623,    -1,    -1,  1626,  1627,    -1,
      -1,    -1,  1631,  1632,    -1,  1634,  1635,    -1,    -1,    -1,
      -1,    -1,   147,    -1,    -1,    -1,   151,   147,    -1,    -1,
    1649,   151,    -1,    -1,    -1,   147,    -1,    -1,   218,   151,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
      -1,    -1,    -1,    -1,    -1,  1674,    -1,    -1,    -1,   239,
     240,    -1,   139,   140,   141,   142,   143,    -1,    -1,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,    -1,  1698,
      -1,    -1,   159,    -1,   161,   162,    -1,    -1,    -1,    -1,
     167,   168,   169,   218,    -1,    -1,   173,   222,   218,   224,
     225,  1720,   222,    -1,   224,   225,   218,    -1,    -1,  1728,
     222,    -1,   224,   225,    -1,   240,    -1,  1579,    -1,    -1,
     240,   246,  1741,    -1,    -1,    -1,   246,    -1,   240,    -1,
      -1,    -1,    -1,  1752,   246,    -1,    -1,    33,    -1,  1758,
    1759,   218,    -1,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,    -1,    -1,    -1,    -1,    -1,    -1,  1778,
    1779,    -1,   239,   240,    60,    61,   243,    -1,    -1,    -1,
    1789,    -1,    -1,    -1,  1793,  1794,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       1,    -1,    -1,    -1,     5,     6,     7,  1816,     9,    10,
      11,    -1,    13,    -1,    15,    16,    17,    18,    19,  1828,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    38,    39,    40,
      -1,    42,    -1,    44,    45,    -1,    -1,    48,    -1,    50,
      51,    52,    -1,    54,    55,    -1,    -1,    58,    59,    -1,
      -1,   147,    -1,    -1,    65,   151,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,    -1,    -1,
      -1,    -1,   218,    -1,    -1,    -1,   222,    -1,   224,   225,
     141,   142,    -1,    -1,  1796,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   240,   156,   157,   158,    -1,    -1,
     246,    -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   174,   175,   176,   177,   178,  1830,   180,
    1832,   182,   183,   184,   185,    -1,   187,   188,   189,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,    -1,  1854,    -1,    -1,    -1,    -1,   208,   209,   210,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   225,   226,   227,    -1,   229,    -1,
      -1,   232,   233,    -1,    -1,     5,     6,    -1,    -1,   240,
      -1,   242,    -1,   244,   245,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      60,    61,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,    -1,    -1,    -1,   147,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,   175,   176,   177,   178,    -1,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,   189,
     190,    -1,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,    -1,    -1,    -1,    -1,    -1,    -1,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,
      -1,    -1,   222,    -1,    -1,   225,   226,   227,    -1,   229,
      -1,    -1,   232,   233,    -1,    -1,    -1,     5,     6,    -1,
     240,    -1,   242,    -1,   244,   245,   246,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    60,    61,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,    -1,    -1,    -1,   147,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,
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
      -1,    -1,    -1,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,
      -1,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   222,    -1,    -1,   225,
     226,   227,    -1,   229,    -1,    -1,   232,   233,    -1,    -1,
      -1,     5,     6,    -1,   240,    -1,   242,    -1,   244,   245,
     246,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,   156,   157,   158,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,   175,   176,   177,   178,    33,   180,    -1,   182,   183,
     184,   185,    -1,    -1,    -1,   189,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,    -1,    -1,
      -1,    -1,    60,    61,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   225,   226,   227,    -1,   229,    -1,    -1,   232,   233,
      -1,    -1,     5,     6,    -1,    -1,   240,    -1,   242,    -1,
     244,   245,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,   147,
      -1,    -1,    65,   151,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,    -1,    -1,    -1,    -1,    -1,
     218,    -1,    -1,    -1,   222,    -1,   224,   225,   141,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,   240,   156,   157,   158,    -1,    -1,   246,    -1,
      -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   174,   175,   176,   177,   178,    33,   180,    -1,   182,
     183,   184,   185,    -1,    -1,    -1,   189,   190,    -1,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,    -1,
      -1,    -1,    -1,    60,    61,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   225,   226,   227,    -1,   229,    -1,    -1,   232,
     233,    -1,    -1,     5,     6,    -1,    -1,   240,    -1,   242,
      -1,   244,   245,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      52,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
     147,    -1,    -1,    65,   151,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,   218,    -1,    -1,    -1,   222,    -1,   224,   225,   141,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   240,    -1,    -1,    -1,    -1,    -1,   246,
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
      -1,    -1,    -1,    -1,    -1,   156,   157,   158,    -1,    -1,
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
      70,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
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
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,   189,
     190,    -1,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,    -1,    -1,    -1,    -1,    -1,    -1,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   225,   226,   227,    -1,   229,
      -1,    -1,   232,   233,    -1,    -1,     5,     6,    -1,    -1,
     240,    -1,   242,   243,   244,   245,    15,    16,    17,    18,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,   157,   158,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   174,   175,   176,   177,   178,
      -1,   180,    -1,   182,   183,   184,   185,    -1,    -1,    -1,
     189,   190,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   225,   226,   227,    -1,
     229,    -1,    -1,   232,   233,    -1,    -1,    -1,    -1,     5,
       6,   240,    -1,   242,    -1,   244,   245,    13,    -1,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    -1,    -1,    -1,    55,
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
      -1,    -1,     5,     6,   240,   241,   242,    -1,   244,   245,
      13,    -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
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
     233,    -1,    -1,    -1,    -1,     5,     6,   240,    -1,   242,
      -1,   244,   245,    13,    -1,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    49,
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
      -1,    -1,    -1,    -1,    -1,   225,   226,   227,    -1,   229,
      -1,    -1,   232,   233,    -1,    -1,    -1,    -1,     5,     6,
     240,   241,   242,    -1,   244,   245,    13,    -1,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    -1,    -1,    -1,    55,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   225,   226,
     227,    -1,   229,    -1,    -1,   232,   233,    -1,    -1,     5,
       6,    -1,    -1,   240,    -1,   242,    -1,   244,   245,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    70,    71,    72,    73,    -1,    75,
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
     176,   177,   178,    -1,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,   189,   190,    -1,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,
      -1,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   225,
     226,   227,    -1,   229,    -1,    -1,   232,   233,    -1,    -1,
       5,     6,    -1,    -1,   240,    -1,   242,    -1,   244,   245,
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
     175,   176,   177,   178,    -1,   180,   181,   182,   183,   184,
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
     174,   175,   176,   177,   178,    -1,   180,   181,   182,   183,
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
     233,    -1,    -1,    -1,    -1,     5,     6,   240,   241,   242,
      -1,   244,   245,    13,    -1,    15,    16,    17,    18,    19,
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
      -1,    -1,    -1,    -1,    -1,   225,   226,   227,    -1,   229,
      -1,    -1,   232,   233,    -1,    -1,    -1,    -1,     5,     6,
     240,    -1,   242,    -1,   244,   245,    13,    -1,    15,    16,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   225,   226,
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
      -1,    -1,    -1,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,    -1,    -1,    -1,    -1,
      -1,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   225,
     226,   227,    -1,   229,    -1,    -1,   232,   233,    -1,    -1,
       5,     6,    -1,    -1,   240,    -1,   242,    -1,   244,   245,
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
     175,   176,   177,   178,    -1,   180,   181,   182,   183,   184,
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
      -1,    55,    -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,
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
      -1,    -1,     5,     6,    -1,    -1,   240,    -1,   242,    -1,
     244,   245,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    -1,    -1,    58,    -1,    -1,    -1,    -1,
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
      -1,   244,   245,    15,    16,    17,    18,    19,    -1,    -1,
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
      -1,   223,    -1,   225,   226,   227,    -1,   229,    -1,    -1,
     232,   233,    -1,    -1,     5,     6,    -1,    -1,   240,    -1,
     242,    -1,   244,   245,    15,    16,    17,    18,    19,    -1,
      -1,    22,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,   225,   226,   227,    -1,   229,
      -1,    -1,   232,   233,    -1,    -1,     5,     6,    -1,    -1,
     240,    -1,   242,   243,   244,   245,    15,    16,    17,    18,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   174,   175,   176,   177,   178,
      -1,   180,    -1,   182,   183,   184,   185,    -1,    -1,    -1,
     189,   190,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   223,    -1,   225,   226,   227,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   225,   226,
     227,    -1,   229,    -1,    -1,   232,   233,    -1,    -1,     5,
       6,    -1,    -1,   240,    -1,   242,   243,   244,   245,    15,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   174,   175,   176,   177,   178,    -1,   180,    -1,   182,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   174,   175,   176,   177,   178,
      -1,   180,    -1,   182,   183,   184,   185,    -1,    -1,    -1,
     189,   190,    -1,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,    -1,    13,    21,    22,    -1,    -1,   208,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,   225,   226,   227,    -1,
     229,    -1,    41,   232,   233,    -1,    -1,    -1,    -1,    -1,
      49,   240,    -1,   242,    -1,   244,   245,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,    -1,
      -1,    -1,    -1,   159,    -1,   161,   162,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,    21,
      22,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,   198,
      25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    41,   223,   224,   225,
     226,   227,   228,   229,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   239,   240,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,   244,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,   146,    -1,    -1,   149,    -1,   151,
     152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,
     162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,   198,    25,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      41,   223,   224,   225,   226,   227,   228,   229,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,   240,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,   244,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,   198,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,   244,    -1,   246,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   188,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,   198,    25,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,   210,    -1,    -1,    -1,    -1,
      41,   216,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,   244,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    21,    22,   198,    25,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,   244,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
      -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,    21,
      22,    -1,    -1,   159,   160,   161,   162,   163,   164,   166,
      -1,   167,   168,   169,   170,   171,   172,   173,    -1,    -1,
      -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,    -1,    21,    22,    -1,    -1,    -1,    -1,
      -1,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   210,    -1,    -1,    -1,    -1,    -1,
      -1,   217,   218,    -1,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   239,   240,    -1,    -1,   244,   244,   245,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,    -1,    -1,    -1,   159,   160,   161,
     162,   163,   164,    21,    22,   167,   168,   169,   170,   171,
     172,   173,   139,   140,   141,   142,   143,    -1,    -1,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,   159,    -1,   161,   162,    -1,    -1,    21,    22,
     167,   168,   169,    -1,    -1,    -1,   173,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   217,   218,    -1,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,   240,    -1,
      -1,    -1,   244,   245,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   218,    -1,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   239,   240,    -1,    -1,   243,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,    -1,    -1,    -1,
      -1,   159,    -1,   161,   162,    21,    22,    -1,    -1,   167,
     168,   169,    -1,    -1,    -1,   173,   139,   140,   141,   142,
     143,    -1,    -1,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,
      21,    22,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     218,    -1,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   239,   240,    -1,    -1,   243,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   218,    -1,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   239,   240,    -1,    -1,
     243,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,    -1,
      -1,    -1,    -1,   159,    -1,   161,   162,    21,    22,    -1,
      -1,   167,   168,   169,    -1,    -1,    -1,   173,   139,   140,
     141,   142,   143,    -1,    -1,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
     161,   162,    21,    22,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   218,    -1,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   239,   240,    -1,    -1,   243,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,   240,
      -1,    -1,   243,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,    21,
      22,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,   173,
     139,   140,   141,   142,   143,    -1,    -1,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
     159,    -1,   161,   162,    21,    22,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   218,    -1,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   239,   240,    -1,    -1,   243,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,
      -1,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     239,   240,    -1,    -1,   243,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,
     162,    21,    22,    -1,    -1,   167,   168,   169,    -1,    -1,
      -1,   173,   139,   140,   141,   142,   143,    -1,    -1,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,   159,    -1,   161,   162,    21,    22,    -1,    -1,
     167,   168,   169,    -1,    -1,    -1,   173,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,   240,    -1,
      -1,   243,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   218,    -1,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   239,   240,    -1,    -1,   243,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,
      -1,   161,   162,    21,    22,    -1,    -1,   167,   168,   169,
      -1,    -1,    -1,   173,   139,   140,   141,   142,   143,    -1,
      -1,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,   159,    -1,   161,   162,    21,    22,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,   173,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,
     240,    -1,    -1,   243,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   218,    -1,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   239,   240,    -1,    -1,   243,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,    -1,    -1,    -1,
      -1,   159,    -1,   161,   162,    21,    22,    -1,    -1,   167,
     168,   169,    -1,    -1,    -1,   173,   139,   140,   141,   142,
     143,    -1,    -1,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,
      21,    22,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     218,    -1,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   239,   240,    -1,    -1,   243,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   218,    -1,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   239,   240,    -1,    -1,
     243,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,    -1,
      -1,    -1,    -1,   159,    -1,   161,   162,    21,    22,    -1,
      -1,   167,   168,   169,    -1,    -1,    -1,   173,   139,   140,
     141,   142,   143,    -1,    -1,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
     161,   162,    21,    22,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   218,    -1,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   239,   240,    -1,    -1,   243,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,   240,
      -1,    -1,   243,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,    21,
      22,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,   173,
     139,   140,   141,   142,   143,    -1,    -1,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
     159,    -1,   161,   162,    21,    22,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   218,    -1,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   239,   240,    -1,    -1,   243,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,
      -1,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     239,   240,    -1,    -1,   243,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,
     162,    21,    22,    -1,    -1,   167,   168,   169,    -1,    -1,
      -1,   173,   139,   140,   141,   142,   143,    -1,    -1,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,   159,    -1,   161,   162,    21,    22,    -1,    -1,
     167,   168,   169,    -1,    -1,    -1,   173,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,   240,    -1,
      -1,   243,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   218,    -1,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   239,   240,    -1,    -1,   243,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,
      -1,   161,   162,    21,    22,    -1,    -1,   167,   168,   169,
      -1,    -1,    -1,   173,   139,   140,   141,   142,   143,    -1,
      -1,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,   159,    -1,   161,   162,    21,    22,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,   173,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,
     240,    -1,    -1,   243,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   218,    -1,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   239,   240,    -1,    -1,   243,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,    -1,    -1,    -1,
      -1,   159,    -1,   161,   162,    21,    22,    -1,    -1,   167,
     168,   169,    -1,    -1,    -1,   173,   139,   140,   141,   142,
     143,    -1,    -1,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,
      21,    22,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     218,    -1,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   239,   240,    -1,    -1,   243,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   218,    -1,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   239,   240,    -1,    -1,
     243,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,    -1,
      -1,    -1,    -1,   159,    -1,   161,   162,    21,    22,    -1,
      -1,   167,   168,   169,    -1,    -1,    -1,   173,   139,   140,
     141,   142,   143,    -1,    -1,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
     161,   162,    21,    22,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   218,    -1,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   239,   240,    -1,    -1,   243,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,   240,
      -1,    -1,   243,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,    21,
      22,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,   173,
     139,   140,   141,   142,   143,    -1,    -1,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
     159,    -1,   161,   162,    21,    22,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   218,    -1,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   239,   240,    -1,    -1,   243,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,
      -1,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     239,   240,    -1,    -1,   243,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,
     162,    21,    22,    -1,    -1,   167,   168,   169,    -1,    -1,
      -1,   173,   139,   140,   141,   142,   143,    -1,    -1,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,   159,    -1,   161,   162,    21,    22,    -1,    -1,
     167,   168,   169,    -1,    -1,    -1,   173,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,   240,    -1,
      -1,   243,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   218,    -1,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   239,   240,    -1,    -1,   243,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,
      -1,   161,   162,    21,    22,    -1,    -1,   167,   168,   169,
      -1,    -1,    -1,   173,   139,   140,   141,   142,   143,    -1,
      -1,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,   159,    -1,   161,   162,    21,    22,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,   173,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,
     240,    -1,    -1,   243,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   218,    -1,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   239,   240,    -1,    -1,   243,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,    -1,    -1,    -1,
      -1,   159,    -1,   161,   162,    21,    22,    -1,    -1,   167,
     168,   169,    -1,    -1,    -1,   173,   139,   140,   141,   142,
     143,    -1,    -1,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,
      21,    22,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     218,    -1,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   239,   240,    -1,    -1,   243,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   218,    -1,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   239,   240,    -1,    -1,
     243,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,    -1,
      -1,    -1,    -1,   159,    -1,   161,   162,    21,    22,    -1,
      -1,   167,   168,   169,    -1,    -1,    -1,   173,   139,   140,
     141,   142,   143,    -1,    -1,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
     161,   162,    21,    22,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   218,    -1,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   239,   240,   241,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,   240,
     241,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,    21,
      22,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,   173,
     139,   140,   141,   142,   143,    -1,    -1,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
     159,    -1,   161,   162,    21,    22,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   218,    -1,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   239,   240,   241,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,
      -1,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     239,   240,   241,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,
     162,    -1,    -1,    -1,    -1,   167,   168,   169,    -1,    -1,
      -1,   173,   139,   140,   141,   142,   143,    -1,    -1,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,    21,    22,
      -1,    -1,   159,    -1,   161,   162,    -1,    -1,    -1,    -1,
     167,   168,   169,    -1,    -1,    38,   173,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,    -1,   239,   240,   241,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   218,    -1,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   239,   240,   241,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,    -1,    -1,   151,   152,
     153,    -1,    -1,    -1,    -1,    -1,    -1,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,   170,   171,   172,
     173,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
     159,    -1,   161,   162,    -1,    -1,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,   173,   218,    -1,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   186,    -1,   232,
     233,    -1,    -1,    -1,    -1,    -1,   239,   240,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,
      -1,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
     239,   240,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,
      -1,   161,   162,    21,    22,    -1,    -1,   167,   168,   169,
      -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     210,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,
     240,   139,   140,   141,   142,   143,    -1,    -1,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,    -1,    21,    22,
      -1,   159,    -1,   161,   162,    -1,    -1,   165,    -1,   167,
     168,   169,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,    21,    22,    -1,
      -1,   159,    -1,   161,   162,    -1,    -1,   165,    -1,   167,
     168,   169,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,
     218,    -1,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   239,   240,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     218,    -1,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,   239,   240,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,    21,    22,    -1,    -1,   159,    -1,   161,   162,
      -1,    -1,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,    21,    22,    -1,    -1,   159,    -1,   161,   162,    -1,
      -1,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,   173,
      -1,    -1,    -1,    -1,    -1,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   239,   240,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,   239,   240,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,    21,    22,    -1,    -1,
     159,    -1,   161,   162,    -1,    -1,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,    21,    22,    -1,    -1,   159,
      -1,   161,   162,    -1,    -1,    -1,    -1,   167,   168,   169,
      -1,   210,    -1,   173,    -1,    -1,    -1,    -1,    -1,   218,
      -1,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     239,   240,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,   218,    -1,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,   239,
     240,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
      21,    22,    -1,    -1,   159,    -1,   161,   162,    -1,    -1,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,   173,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,    -1,
      -1,    -1,    -1,   159,    -1,   161,   162,    -1,    -1,    -1,
      -1,   167,    -1,   169,    -1,    -1,    21,    22,    -1,    -1,
      -1,    -1,    -1,   218,    -1,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,   239,   240,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,
      -1,   161,   162,    -1,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,   239,   240,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
     161,   162,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,   239,
     240,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,   159,    -1,   161,   162,    -1,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,   240,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   221,   222,   223,   224,
     225,   226,   227,   228,   229,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   239,   240,    71,    72,    73,    19,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,    -1,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   198,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,    -1,    -1,   181,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   152,   153,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    -1,    73,    -1,    75,    76,
      77,    78,    79,   181,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
     198,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   239,   240,    -1,   141,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   198
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
      63,   424,    15,    16,   189,   190,   192,   193,   194,   195,
     198,   201,   225,   242,   245,   255,   295,   189,   272,   267,
     267,   267,   198,   266,   266,   198,   277,   187,   209,   313,
     429,   277,    57,    63,   250,   198,   198,   198,   198,   202,
     261,   243,   296,   300,   300,   300,   300,    57,    63,   308,
     310,   198,   435,   448,   312,   426,   202,   203,   204,   254,
     189,   190,   191,   195,    15,    16,   189,   190,   192,   193,
     194,   195,   198,   225,   245,   255,   292,   293,   245,   269,
     430,   277,   233,   252,   506,   496,   501,   202,   243,   311,
     219,   312,   129,   442,   443,   421,   183,   245,   297,   393,
     202,   203,   204,   189,   190,   191,   195,   245,   216,   243,
     198,   217,    66,   219,   457,   312,   312,    35,    71,    73,
      75,    76,    77,    78,    79,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    93,    94,    95,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   141,   142,   198,   305,
     309,    75,    79,    93,    94,    98,    99,   100,   101,   452,
     437,   198,   449,   188,   313,   422,   296,   295,   245,   277,
     175,   198,   419,   420,   198,   292,    19,    25,    31,    41,
      49,    64,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   179,   244,
     329,   451,   453,   454,   458,   464,   493,    79,    94,    99,
     101,   312,   497,   502,    21,    22,    38,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   151,   152,   153,   160,   161,
     162,   163,   164,   167,   168,   169,   170,   171,   172,   173,
     218,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   232,   233,   239,   240,    35,    35,   242,   303,   312,
     314,   312,   427,   219,   441,   312,   446,   393,   241,   295,
     242,    43,   216,   219,   222,   418,   223,   223,   223,   242,
     223,   223,   242,   457,   223,   223,   223,   223,   223,   242,
     329,    33,    60,    61,   147,   151,   218,   222,   225,   240,
     246,   463,   220,   507,   409,   412,   198,   198,   198,   241,
      22,   198,   241,   178,   243,   393,   404,   405,   406,   149,
     219,   304,   317,   439,   198,   277,   438,   329,   399,   420,
     241,     5,     6,    15,    16,    17,    18,    19,    25,    27,
      31,    39,    45,    48,    51,    55,    65,    68,    69,    80,
     125,   126,   127,   141,   142,   174,   175,   176,   177,   178,
     180,   182,   183,   184,   185,   189,   190,   192,   193,   194,
     195,   196,   197,   199,   200,   201,   208,   225,   226,   227,
     232,   233,   240,   242,   244,   245,   260,   262,   323,   329,
     334,   348,   355,   358,   361,   366,   369,   373,   374,   376,
     381,   384,   385,   392,   451,   509,   524,   535,   539,   552,
     555,   198,   175,   198,   464,   150,   160,   217,   417,   465,
     471,   473,   385,   475,   469,   198,   223,   477,   479,   481,
     483,   485,   487,   489,   491,   385,   223,   242,   320,    33,
     222,    33,   222,   240,   246,   241,   385,   240,   246,   464,
     456,   198,   216,   277,   407,   461,   493,   498,   198,   410,
     461,   503,   198,   131,   132,   133,   134,   135,   136,   137,
     138,   160,   170,   171,   172,   131,   132,   133,   134,   135,
     136,   137,   138,   150,   160,   170,   171,   172,   242,     7,
      50,   342,   277,   216,   277,   243,   493,   493,     1,     9,
      10,    11,    13,    26,    28,    29,    38,    40,    42,    44,
      52,    54,    58,    59,    65,   127,   128,   156,   157,   158,
     199,   273,   274,   277,   278,   281,   282,   284,   286,   287,
     288,   289,   313,   315,   316,   318,   323,   328,   330,   335,
     336,   337,   338,   339,   340,   341,   343,   347,   370,   372,
     385,   427,   217,   277,   313,    40,   240,   277,   302,   313,
     401,   223,   223,   223,   331,   453,   509,   242,   329,   223,
       5,   125,   126,   223,   242,   223,   242,   242,   223,   223,
     242,   223,   242,   223,   242,   223,   223,   242,   223,   223,
     385,   385,   242,   242,   242,   242,   242,   242,    13,   464,
      13,   464,    13,   385,   534,   550,   223,   223,   259,    13,
     321,   534,   551,   191,   385,   385,   385,   385,   385,    13,
      49,   319,   359,   385,   181,   198,   359,   510,   512,   245,
     198,   242,   302,    21,    22,   139,   140,   141,   142,   143,
     146,   147,   148,   149,   151,   152,   153,   154,   159,   161,
     162,   167,   168,   169,   173,   218,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   239,   240,   243,   242,
     242,    43,   277,   417,   328,   370,   385,   493,   493,   462,
     493,   243,   493,   493,   243,   224,   459,   493,   303,   493,
     303,   493,   303,   407,   408,   410,   411,   243,   467,   319,
     241,   241,   385,   198,   277,   508,   219,   461,   313,   219,
     461,   313,   385,   175,   198,   414,   415,   450,   406,   406,
     406,   385,   285,   150,   328,   359,   385,   314,    61,   385,
     291,   385,   198,   277,   189,    58,   385,   314,   223,   150,
     328,   385,   245,   314,   361,   365,   365,   365,   385,   277,
     277,   385,    10,    37,   361,   367,   277,   277,   277,   277,
     277,    66,   344,   155,   277,   131,   132,   133,   134,   135,
     136,   137,   138,   144,   145,   150,   160,   163,   164,   170,
     171,   172,   217,   367,   440,   385,   400,   301,     8,   393,
     398,   525,   527,   332,   242,   329,   223,   242,   356,   223,
     223,   223,   546,   359,   464,   321,   385,   349,   351,   385,
     353,   385,   548,   359,   531,   536,   359,   529,   464,   385,
     385,   385,   385,   385,   385,   450,    53,   225,   240,   242,
     385,   510,   513,   517,   533,   538,   450,   242,   513,   538,
     450,   165,   209,   210,   211,   518,   324,   326,   204,   205,
     254,   450,   209,   216,   554,   450,    13,   241,   216,   554,
     242,   150,   160,   217,   413,   554,   216,   554,   243,   175,
     180,   223,   329,   375,    70,   240,   243,   359,   512,     4,
     183,   364,    19,   181,   198,   451,    19,   181,   198,   451,
     385,   385,   385,   385,   385,   385,   198,   385,   181,   198,
     385,   385,   385,   451,   385,   385,   385,   385,   385,   385,
      22,   385,   385,   385,   385,   385,   385,   385,   385,   385,
     385,   385,   152,   153,   181,   198,   239,   240,   382,   451,
     385,   243,   359,   385,   198,   328,   385,   131,   132,   133,
     134,   135,   136,   137,   138,   144,   145,   150,   163,   164,
     170,   171,   172,   217,   277,   224,   224,   224,   461,   224,
     224,   198,   455,   224,   304,   224,   304,   224,   304,   224,
     461,   224,   461,   322,   493,   216,   554,   241,   217,   277,
     313,   493,   493,   243,   242,    43,   216,   219,   222,   413,
     314,   450,   328,   359,   216,    14,   385,   198,   314,   217,
     219,   189,   464,   328,   385,   245,   302,   314,   314,   283,
     312,   368,   183,   242,   346,   420,   365,   156,   157,   158,
     315,   371,   385,   371,   385,   371,   385,   371,   385,   371,
     385,   371,   385,   371,   385,   371,   385,   371,   385,   371,
     385,   371,   385,   385,   371,   385,   371,   385,   371,   385,
     371,   385,   371,   385,   371,   385,   313,   277,   198,   241,
      57,    63,   396,    67,   397,   277,   464,   464,   493,    70,
     359,   512,   523,   223,   385,   198,   385,   493,   540,   542,
     544,   464,   554,   224,   461,   243,   243,   464,   464,   243,
     464,   243,   464,   554,   464,   408,   554,   411,   224,   243,
     243,   243,   243,   243,   243,    20,   365,   240,   385,   243,
     165,   216,   210,   517,   213,   214,   241,   521,   216,   210,
     213,   241,   520,    20,   243,   517,   209,   212,   519,    20,
     385,   209,   534,   322,   322,   385,    20,   534,    20,   450,
     385,   385,   385,   385,   243,   181,   198,   242,   242,   377,
     379,   198,   243,   512,   510,   216,   243,   243,   242,   150,
     160,   198,   217,   222,   362,   363,   303,   223,   242,   223,
     242,   242,   242,   241,    19,   181,   198,   451,   219,   181,
     198,   385,   242,   242,   181,   198,   385,     1,   242,   241,
     243,   243,   277,   385,   385,   385,   385,   385,   385,   385,
     385,   385,   385,   385,   385,   385,   385,   385,   385,   385,
     466,   472,   474,   493,   476,   470,   216,   224,   277,   478,
     224,   482,   224,   486,   224,   490,   407,   492,   410,   224,
     461,   243,   385,   385,   198,   175,   198,   493,   385,    20,
     314,   217,   290,   224,   364,    12,    23,    24,   275,   276,
     385,   317,   302,   198,   345,   345,   365,   365,   365,   217,
     277,    47,   397,    46,   129,   394,   224,   224,   224,   512,
     243,   243,   243,   198,   243,   210,   224,   243,   224,   464,
     408,   411,   224,   243,   242,   464,   224,   224,   224,   224,
     243,   224,   224,   243,   224,   364,   242,   359,   513,   517,
     385,   510,   521,   241,   385,   533,   241,   359,   513,   209,
     212,   215,   522,   241,   359,   224,   224,   219,   257,   359,
     359,    20,   243,   242,   160,   413,   385,   385,   464,   303,
     243,   241,   240,   363,   198,   198,   242,   198,   198,   216,
     241,   304,   386,   385,   388,   385,   243,   359,   385,   223,
     242,   385,   242,   241,   385,   240,   243,   359,   242,   241,
     383,   243,   359,   198,   460,   198,   480,   484,   488,   468,
     493,   277,   243,   242,    43,   413,   359,   493,   385,   364,
     303,   314,   385,    12,   279,   313,   364,   216,   241,   243,
     493,    33,   395,   394,   396,   526,   528,   333,   243,   224,
     461,   198,   242,   357,   224,   224,   224,   547,   321,   224,
     350,   352,   354,   549,   532,   537,   530,   242,   243,   359,
     210,   517,   521,   210,   517,   241,   210,   325,   327,   258,
     206,   210,   210,   359,   160,   413,   385,   385,   385,   243,
     243,   224,   304,   243,   510,   243,   198,   362,   241,   165,
     314,   360,   464,   243,   493,   243,   243,   243,   390,   385,
     385,   243,   510,   243,   385,   243,   320,   385,   198,   385,
     314,   367,   304,   314,   280,   277,   303,   198,   241,   219,
     418,   277,   402,   395,   414,   415,   416,   242,   242,   385,
     198,   224,   385,   541,   543,   545,   242,   243,   242,   385,
     385,   385,   242,    70,   523,   242,   242,   243,   385,   243,
     385,   521,   385,   522,   534,   385,   320,   256,   534,   385,
     210,   385,   385,   243,   378,   224,   241,   243,   150,   385,
     224,   224,   493,   243,   243,   241,   243,   243,   360,   276,
      26,   128,   281,   335,   336,   337,   339,   385,   304,   219,
     418,   464,   417,   309,   403,   523,   523,   243,   224,   242,
     243,   242,   242,   242,   319,   321,   359,   523,   523,   243,
     210,   553,   553,   553,   202,   553,   553,   385,   160,   413,
     375,   380,   243,   385,   387,   389,   224,   243,   150,   150,
     385,   314,   464,   417,   417,   328,   385,   277,   309,   242,
     510,   514,   515,   516,   516,   385,   385,   523,   523,   510,
     511,   243,   243,   554,   516,   511,    53,   241,   209,   209,
     209,   241,   553,   385,   385,   375,   391,   385,   417,   328,
     385,   328,   385,   277,   314,   510,   216,   554,   243,   243,
     243,   243,   516,   516,   243,   243,   243,   243,   385,   241,
     241,   209,   241,   328,   385,   277,   277,   243,   242,   243,
     243,   277,   510,   243
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
     292,   292,   292,   293,   293,   294,   294,   294,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   296,   296,   297,   297,   297,
     298,   298,   298,   298,   299,   299,   300,   300,   300,   300,
     300,   300,   300,   301,   301,   302,   302,   303,   303,   303,
     304,   304,   304,   305,   305,   305,   305,   305,   305,   305,
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
     305,   305,   305,   305,   305,   306,   306,   307,   308,   308,
     308,   309,   311,   310,   312,   312,   313,   313,   314,   314,
     315,   315,   315,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   317,   317,   317,   318,   319,   319,
     320,   320,   321,   321,   322,   322,   324,   325,   323,   326,
     327,   323,   328,   328,   328,   328,   328,   329,   329,   329,
     330,   330,   332,   333,   331,   331,   334,   334,   334,   334,
     334,   334,   335,   336,   337,   337,   337,   338,   338,   338,
     339,   339,   340,   340,   340,   341,   342,   342,   342,   343,
     343,   344,   344,   345,   345,   346,   346,   346,   346,   346,
     346,   346,   346,   347,   347,   349,   350,   348,   351,   352,
     348,   353,   354,   348,   356,   357,   355,   358,   358,   358,
     358,   358,   358,   359,   359,   360,   360,   360,   361,   361,
     361,   362,   362,   362,   362,   362,   363,   363,   364,   364,
     364,   365,   365,   366,   368,   367,   369,   369,   369,   369,
     369,   369,   369,   369,   369,   370,   370,   370,   370,   370,
     370,   370,   370,   370,   370,   370,   370,   370,   370,   370,
     370,   370,   370,   370,   371,   371,   371,   371,   372,   372,
     372,   372,   372,   372,   372,   372,   372,   372,   372,   372,
     372,   372,   372,   372,   372,   373,   373,   374,   374,   375,
     375,   376,   377,   378,   376,   379,   380,   376,   381,   381,
     381,   381,   381,   381,   381,   382,   383,   381,   384,   384,
     384,   384,   384,   384,   384,   385,   385,   385,   385,   385,
     385,   385,   385,   385,   385,   385,   385,   385,   385,   385,
     385,   385,   385,   385,   385,   385,   385,   385,   385,   385,
     385,   385,   385,   385,   385,   385,   385,   385,   385,   385,
     385,   385,   385,   385,   385,   385,   385,   385,   385,   385,
     385,   385,   385,   385,   385,   385,   385,   385,   385,   385,
     385,   385,   386,   387,   385,   385,   385,   385,   388,   389,
     385,   385,   385,   390,   391,   385,   385,   385,   385,   385,
     385,   385,   385,   385,   385,   385,   385,   385,   385,   385,
     392,   392,   392,   392,   392,   392,   392,   392,   392,   392,
     392,   392,   392,   392,   392,   392,   393,   393,   393,   394,
     394,   394,   395,   395,   396,   396,   396,   397,   397,   398,
     399,   399,   400,   399,   401,   399,   402,   399,   403,   399,
     399,   404,   405,   405,   406,   406,   406,   406,   406,   407,
     407,   408,   408,   409,   409,   409,   410,   411,   411,   412,
     412,   412,   413,   413,   414,   414,   414,   415,   415,   416,
     416,   417,   417,   417,   418,   418,   419,   419,   419,   419,
     419,   419,   420,   420,   420,   420,   420,   421,   421,   422,
     421,   423,   423,   424,   424,   424,   425,   426,   425,   427,
     427,   427,   427,   428,   428,   428,   430,   429,   431,   431,
     432,   433,   432,   434,   434,   434,   435,   437,   438,   436,
     439,   440,   436,   441,   441,   442,   442,   443,   444,   444,
     444,   444,   445,   445,   445,   446,   446,   448,   449,   447,
     450,   450,   450,   450,   450,   450,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   452,   452,   452,   452,
     452,   452,   452,   452,   453,   454,   454,   454,   455,   455,
     455,   456,   456,   456,   456,   457,   457,   457,   457,   457,
     458,   459,   460,   458,   461,   461,   462,   462,   463,   463,
     464,   464,   464,   464,   464,   464,   465,   466,   464,   464,
     464,   467,   468,   464,   464,   464,   464,   464,   464,   464,
     464,   464,   464,   464,   464,   464,   469,   470,   464,   464,
     471,   472,   464,   473,   474,   464,   475,   476,   464,   464,
     477,   478,   464,   479,   480,   464,   464,   481,   482,   464,
     483,   484,   464,   464,   485,   486,   464,   487,   488,   464,
     489,   490,   464,   491,   492,   464,   493,   493,   493,   495,
     496,   497,   498,   494,   500,   501,   502,   503,   499,   505,
     506,   507,   508,   504,   509,   509,   509,   509,   509,   510,
     510,   510,   510,   510,   510,   510,   510,   511,   511,   512,
     513,   513,   514,   514,   515,   515,   516,   516,   517,   517,
     518,   518,   519,   519,   520,   520,   521,   521,   521,   522,
     522,   522,   523,   523,   524,   524,   524,   524,   524,   524,
     525,   526,   524,   527,   528,   524,   529,   530,   524,   531,
     532,   524,   533,   533,   533,   534,   534,   535,   536,   537,
     535,   538,   538,   539,   539,   539,   540,   541,   539,   542,
     543,   539,   544,   545,   539,   539,   546,   547,   539,   539,
     548,   549,   539,   550,   550,   551,   551,   552,   552,   552,
     552,   552,   553,   553,   554,   554,   555,   555,   555,   555,
     555,   555
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
       3,     1,     1,     2,     2,     1,     1,     1,     2,     1,
       2,     1,     1,     1,     3,     1,     1,     1,     3,     3,
       5,     3,     3,     4,     4,     3,     3,     3,     4,     3,
       4,     3,     3,     1,     5,     1,     3,     2,     3,     2,
       1,     1,     1,     1,     1,     4,     1,     2,     3,     3,
       3,     3,     2,     1,     3,     0,     3,     0,     2,     3,
       0,     2,     2,     1,     2,     2,     2,     2,     2,     2,
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     1,     4,     0,     1,
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
       4,     1,     6,     6,     0,     7,     1,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     6,     8,     5,     6,     1,
       4,     3,     0,     0,     8,     0,     0,     9,     3,     4,
       5,     6,     8,     5,     6,     0,     0,     5,     3,     4,
       4,     5,     4,     3,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     4,     3,     4,     5,     4,
       5,     3,     4,     1,     1,     2,     4,     4,     7,     8,
       3,     5,     0,     0,     8,     3,     3,     3,     0,     0,
       8,     3,     4,     0,     0,     9,     4,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     2,     4,     1,
       4,     4,     4,     4,     4,     1,     6,     7,     6,     6,
       7,     7,     6,     7,     6,     6,     0,     4,     1,     0,
       1,     1,     0,     1,     0,     1,     1,     0,     1,     5,
       0,     2,     0,     7,     0,     4,     0,     9,     0,    10,
       5,     3,     3,     4,     1,     1,     3,     3,     3,     1,
       3,     1,     3,     0,     2,     3,     3,     1,     3,     0,
       2,     3,     1,     1,     1,     2,     3,     3,     5,     1,
       1,     1,     1,     1,     0,     1,     1,     4,     3,     3,
       6,     5,     4,     6,     5,     5,     4,     0,     2,     0,
       4,     0,     1,     0,     1,     1,     6,     0,     6,     0,
       2,     3,     5,     0,     1,     1,     0,     5,     2,     3,
       4,     0,     4,     0,     1,     1,     1,     0,     0,     9,
       0,     0,    11,     0,     2,     0,     1,     3,     1,     1,
       2,     2,     0,     1,     1,     0,     3,     0,     0,     7,
       1,     4,     3,     3,     6,     5,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     4,     1,     3,
       3,     0,     2,     3,     5,     0,     2,     2,     2,     2,
       4,     0,     0,     7,     1,     1,     1,     3,     3,     4,
       1,     1,     1,     1,     2,     3,     0,     0,     6,     4,
       3,     0,     0,     8,     4,     2,     2,     3,     2,     3,
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

  case 106: /* annotation_argument_value: "unsigned integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",uint64_t((yyvsp[0].ui))); }
    break;

  case 107: /* annotation_argument_value: "unsigned int8 constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",uint64_t((yyvsp[0].ui))); }
    break;

  case 108: /* annotation_argument_value: '-' "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",int32_t(0u - uint32_t((yyvsp[0].i)))); }
    break;

  case 109: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 110: /* annotation_argument_value: '-' "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",-float((yyvsp[0].fd))); }
    break;

  case 111: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 112: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 113: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 114: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 115: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 116: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 117: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 118: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 119: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 120: /* annotation_argument: annotation_argument_name '=' '@' '@' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[0].s); delete (yyvsp[-4].s); }
    break;

  case 121: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 122: /* annotation_argument: annotation_argument_name '=' "long integer constant"  */
                                                               { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),int64_t((yyvsp[0].i64)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 123: /* annotation_argument: annotation_argument_name '=' '-' "long integer constant"  */
                                                                   { (yyval.aa) = new AnnotationArgument(*(yyvsp[-3].s),int64_t(0ull - uint64_t((yyvsp[0].i64))),tokAt(scanner,(yylsp[-3]))); delete (yyvsp[-3].s); }
    break;

  case 124: /* annotation_argument: annotation_argument_name '=' '-' "int64 minimum magnitude (requires minus)"  */
                                                                { (yyval.aa) = new AnnotationArgument(*(yyvsp[-3].s),int64_t(INT64_MIN),tokAt(scanner,(yylsp[-3]))); delete (yyvsp[-3].s); }
    break;

  case 125: /* annotation_argument: annotation_argument_name '=' "unsigned long integer constant"  */
                                                                        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),uint64_t((yyvsp[0].ui64)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 126: /* annotation_argument: annotation_argument_name '=' "unsigned integer constant"  */
                                                                   { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),uint64_t((yyvsp[0].ui)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 127: /* annotation_argument: annotation_argument_name '=' "unsigned int8 constant"  */
                                                                { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),uint64_t((yyvsp[0].ui)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 128: /* annotation_argument: annotation_argument_name '=' '-' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-3].s),int32_t(0u - uint32_t((yyvsp[0].i))),tokAt(scanner,(yylsp[-3]))); delete (yyvsp[-3].s); }
    break;

  case 129: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 130: /* annotation_argument: annotation_argument_name '=' '-' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-3].s),-float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-3]))); delete (yyvsp[-3].s); }
    break;

  case 131: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 132: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 133: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 134: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 135: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 136: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 137: /* metadata_argument_list: '@' annotation_argument  */
                                      {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 138: /* metadata_argument_list: metadata_argument_list '@' annotation_argument  */
                                                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 139: /* metadata_argument_list: metadata_argument_list semicolon  */
                                               {
        (yyval.aaList) = (yyvsp[-1].aaList);
    }
    break;

  case 140: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 141: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 142: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 143: /* annotation_declaration_name: "template"  */
                                    { (yyval.s) = new string("template"); }
    break;

  case 144: /* annotation_declaration_basic: annotation_declaration_name  */
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

  case 145: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 146: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 147: /* annotation_declaration: '!' annotation_declaration  */
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

  case 148: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 149: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 150: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 151: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 152: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 153: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 154: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 155: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 156: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 157: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 158: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 159: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 160: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yyloc));
    }
    break;

  case 161: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 162: /* optional_function_type: "->" type_declaration  */
                                           {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 163: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 164: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 165: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 166: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 167: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 168: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 169: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 170: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 171: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 172: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 173: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 174: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 175: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 176: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 177: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 178: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 179: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 180: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 181: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 182: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 183: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 184: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 185: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 186: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 187: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 188: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 189: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 190: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 191: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 192: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 193: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 194: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 195: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 196: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 197: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 198: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 199: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 200: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 201: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 202: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 203: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 204: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 205: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 206: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 207: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 208: /* function_name: "operator" '[' ']' "<-"  */
                                    { (yyval.s) = new string("[]<-"); }
    break;

  case 209: /* function_name: "operator" '[' ']' ":="  */
                                      { (yyval.s) = new string("[]:="); }
    break;

  case 210: /* function_name: "operator" '[' ']' "+="  */
                                     { (yyval.s) = new string("[]+="); }
    break;

  case 211: /* function_name: "operator" '[' ']' "-="  */
                                     { (yyval.s) = new string("[]-="); }
    break;

  case 212: /* function_name: "operator" '[' ']' "*="  */
                                     { (yyval.s) = new string("[]*="); }
    break;

  case 213: /* function_name: "operator" '[' ']' "/="  */
                                     { (yyval.s) = new string("[]/="); }
    break;

  case 214: /* function_name: "operator" '[' ']' "%="  */
                                     { (yyval.s) = new string("[]%="); }
    break;

  case 215: /* function_name: "operator" '[' ']' "&="  */
                                     { (yyval.s) = new string("[]&="); }
    break;

  case 216: /* function_name: "operator" '[' ']' "|="  */
                                     { (yyval.s) = new string("[]|="); }
    break;

  case 217: /* function_name: "operator" '[' ']' "^="  */
                                     { (yyval.s) = new string("[]^="); }
    break;

  case 218: /* function_name: "operator" '[' ']' "&&="  */
                                        { (yyval.s) = new string("[]&&="); }
    break;

  case 219: /* function_name: "operator" '[' ']' "||="  */
                                        { (yyval.s) = new string("[]||="); }
    break;

  case 220: /* function_name: "operator" '[' ']' "^^="  */
                                        { (yyval.s) = new string("[]^^="); }
    break;

  case 221: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 222: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 223: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 224: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 225: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 226: /* function_name: "operator" '.' "name" "+="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`+="); delete (yyvsp[-1].s); }
    break;

  case 227: /* function_name: "operator" '.' "name" "-="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`-="); delete (yyvsp[-1].s); }
    break;

  case 228: /* function_name: "operator" '.' "name" "*="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`*="); delete (yyvsp[-1].s); }
    break;

  case 229: /* function_name: "operator" '.' "name" "/="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`/="); delete (yyvsp[-1].s); }
    break;

  case 230: /* function_name: "operator" '.' "name" "%="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`%="); delete (yyvsp[-1].s); }
    break;

  case 231: /* function_name: "operator" '.' "name" "&="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&="); delete (yyvsp[-1].s); }
    break;

  case 232: /* function_name: "operator" '.' "name" "|="  */
                                          { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`|="); delete (yyvsp[-1].s); }
    break;

  case 233: /* function_name: "operator" '.' "name" "^="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^="); delete (yyvsp[-1].s); }
    break;

  case 234: /* function_name: "operator" '.' "name" "&&="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&&="); delete (yyvsp[-1].s); }
    break;

  case 235: /* function_name: "operator" '.' "name" "||="  */
                                            { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`||="); delete (yyvsp[-1].s); }
    break;

  case 236: /* function_name: "operator" '.' "name" "^^="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^^="); delete (yyvsp[-1].s); }
    break;

  case 237: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 238: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 239: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 240: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 241: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 242: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 243: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 244: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 245: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 246: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 247: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 248: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 249: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 250: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 251: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 252: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 253: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 254: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 255: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 256: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 257: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 258: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 259: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 260: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 261: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 262: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 263: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 264: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 265: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 266: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 267: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 268: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 269: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 270: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 271: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 272: /* function_name: "float16"  */
                     { (yyval.s) = new string("float16"); }
    break;

  case 273: /* function_name: "half2"  */
                     { (yyval.s) = new string("half2"); }
    break;

  case 274: /* function_name: "half3"  */
                     { (yyval.s) = new string("half3"); }
    break;

  case 275: /* function_name: "half4"  */
                     { (yyval.s) = new string("half4"); }
    break;

  case 276: /* function_name: "half8"  */
                     { (yyval.s) = new string("half8"); }
    break;

  case 277: /* function_name: "short2"  */
                     { (yyval.s) = new string("short2"); }
    break;

  case 278: /* function_name: "short3"  */
                     { (yyval.s) = new string("short3"); }
    break;

  case 279: /* function_name: "short4"  */
                     { (yyval.s) = new string("short4"); }
    break;

  case 280: /* function_name: "short8"  */
                     { (yyval.s) = new string("short8"); }
    break;

  case 281: /* function_name: "ushort2"  */
                     { (yyval.s) = new string("ushort2"); }
    break;

  case 282: /* function_name: "ushort3"  */
                     { (yyval.s) = new string("ushort3"); }
    break;

  case 283: /* function_name: "ushort4"  */
                     { (yyval.s) = new string("ushort4"); }
    break;

  case 284: /* function_name: "ushort8"  */
                     { (yyval.s) = new string("ushort8"); }
    break;

  case 285: /* function_name: "byte2"  */
                     { (yyval.s) = new string("byte2"); }
    break;

  case 286: /* function_name: "byte3"  */
                     { (yyval.s) = new string("byte3"); }
    break;

  case 287: /* function_name: "byte4"  */
                     { (yyval.s) = new string("byte4"); }
    break;

  case 288: /* function_name: "byte8"  */
                     { (yyval.s) = new string("byte8"); }
    break;

  case 289: /* function_name: "byte16"  */
                     { (yyval.s) = new string("byte16"); }
    break;

  case 290: /* function_name: "ubyte2"  */
                     { (yyval.s) = new string("ubyte2"); }
    break;

  case 291: /* function_name: "ubyte3"  */
                     { (yyval.s) = new string("ubyte3"); }
    break;

  case 292: /* function_name: "ubyte4"  */
                     { (yyval.s) = new string("ubyte4"); }
    break;

  case 293: /* function_name: "ubyte8"  */
                     { (yyval.s) = new string("ubyte8"); }
    break;

  case 294: /* function_name: "ubyte16"  */
                     { (yyval.s) = new string("ubyte16"); }
    break;

  case 295: /* optional_template: %empty  */
                                        { (yyval.b) = false; }
    break;

  case 296: /* optional_template: "template"  */
                                        { (yyval.b) = true; }
    break;

  case 297: /* global_function_declaration: optional_annotation_list "def" optional_template function_declaration  */
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

  case 298: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 299: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 300: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 301: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 302: /* $@8: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 303: /* function_declaration: optional_public_or_private_function $@8 function_declaration_header expression_block  */
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

  case 308: /* expression_block: open_block expressions close_block  */
                                                                  {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 309: /* expression_block: open_block expressions close_block "finally" open_block expressions close_block  */
                                                                                                                        {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        // gc_node — don't delete Expression
    }
    break;

  case 310: /* expr_call_pipe: expr expr_full_block_assumed_piped  */
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

  case 311: /* expr_call_pipe: expression_keyword expr_full_block_assumed_piped  */
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

  case 312: /* expr_call_pipe: "generator" '<' type_declaration_no_options '>' optional_capture_list expr_full_block_assumed_piped  */
                                                                                                                                             {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-3].pTypeDecl),(yyvsp[-1].pCaptList),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 313: /* expression_any: semicolon  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 314: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 315: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 316: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 317: /* expression_any: expr_assign semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 318: /* expression_any: expression_delete semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 319: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 320: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 321: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 322: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 323: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 324: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 325: /* expression_any: expression_break semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 326: /* expression_any: expression_continue semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 327: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 328: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 329: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 330: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 331: /* expression_any: expression_label semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 332: /* expression_any: expression_goto semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 333: /* expression_any: "pass" semicolon  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 334: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 335: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back((yyvsp[0].pExpression));
        }
    }
    break;

  case 336: /* expressions: expressions error  */
                                 {
        (void)(yyvsp[-1].pExpression); /* gc_node — don't delete Expression */ (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 337: /* expr_keyword: "keyword" expr expression_block  */
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

  case 338: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 339: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 340: /* optional_expr_list_in_braces: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 341: /* optional_expr_list_in_braces: '(' optional_expr_list optional_comma ')'  */
                                                             { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 342: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 343: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 344: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 345: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 346: /* $@9: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 347: /* $@10: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 348: /* expression_keyword: "keyword" '<' $@9 type_declaration_no_options_list '>' $@10 expr  */
                                                                                                                                                     {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 349: /* $@11: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 350: /* $@12: %empty  */
                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 351: /* expression_keyword: "type function" '<' $@11 type_declaration_no_options_list '>' $@12 optional_expr_list_in_braces  */
                                                                                                                                                                                   {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 352: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 353: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 354: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 355: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 356: /* expr_pipe: expr_call_pipe  */
                             {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 357: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 358: /* name_in_namespace: "name" "::" "name"  */
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

  case 359: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 360: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
    }
    break;

  case 361: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 362: /* $@13: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 363: /* $@14: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 364: /* new_type_declaration: '<' $@13 type_declaration '>' $@14  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 365: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 366: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pTypeDecl),false);
    }
    break;

  case 367: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 368: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 369: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),(yyvsp[-1].pExpression));
    }
    break;

  case 370: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),(yyvsp[-1].pExpression));
    }
    break;

  case 371: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 372: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 373: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 374: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 375: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 376: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 377: /* expression_return: expression_return_no_pipe semicolon  */
                                                    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 378: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 379: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 380: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 381: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 382: /* expression_yield: expression_yield_no_pipe semicolon  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 383: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 384: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 385: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 386: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 387: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 388: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 389: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 390: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 391: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 392: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 393: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 394: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 395: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-7].pNameList),tokAt(scanner,(yylsp[-7])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 396: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 397: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 398: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                           {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameList),tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 399: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr semicolon  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-6])));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 400: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-5])));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameList),tokAt(scanner,(yylsp[-5])),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 401: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr semicolon  */
                                                                                                        {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-5])));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameList),tokAt(scanner,(yylsp[-5])),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 402: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                   {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-4])));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameList),tokAt(scanner,(yylsp[-4])),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 403: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 404: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 405: /* $@15: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 406: /* $@16: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 407: /* expr_cast: "cast" '<' $@15 type_declaration_no_options '>' $@16 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
    }
    break;

  case 408: /* $@17: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 409: /* $@18: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 410: /* expr_cast: "upcast" '<' $@17 type_declaration_no_options '>' $@18 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 411: /* $@19: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 412: /* $@20: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 413: /* expr_cast: "reinterpret" '<' $@19 type_declaration_no_options '>' $@20 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 414: /* $@21: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 415: /* $@22: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 416: /* expr_type_decl: "type" '<' $@21 type_declaration '>' $@22  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 417: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 418: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 419: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" c_or_s "name" '>' expr ')'  */
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

  case 420: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
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

  case 421: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
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

  case 422: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" "end of expression" "name" '>' '(' expr ')'  */
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

  case 423: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 424: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 425: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 426: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = new ExprReturn(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->generated = true;
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 427: /* block_or_simple_block: "=>" "<-" expr  */
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

  case 428: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 429: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 430: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 431: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 432: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 433: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 434: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 435: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 436: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 437: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 438: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 439: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 440: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 441: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),(yyvsp[0].pExpression));
        closure->returnType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
    }
    break;

  case 442: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 443: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 444: /* $@23: %empty  */
                             {  yyextra->das_need_oxford_comma = false; }
    break;

  case 445: /* expr_full_block_assumed_piped: block_or_lambda $@23 optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 446: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstInt>(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 447: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstUInt>(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 448: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstInt64>(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 449: /* expr_numeric_const: '-' "int64 minimum magnitude (requires minus)"  */
                                             { (yyval.pExpression) = newConstLiteral<ExprConstInt64>(tokAt(scanner,(yylsp[0])),int64_t(INT64_MIN)); }
    break;

  case 450: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstUInt64>(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 451: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstUInt8>(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 452: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstFloat>(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 453: /* expr_numeric_const: "float16 constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstFloat16>(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 454: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstDouble>(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 455: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 456: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 457: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 458: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 459: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 460: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 461: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 462: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 463: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 464: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 465: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 466: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 467: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 468: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 469: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 470: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 471: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 472: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 473: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 474: /* expr_assign_pipe_right: "@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 475: /* expr_assign_pipe_right: "@@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 476: /* expr_assign_pipe_right: "$ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 477: /* expr_assign_pipe_right: expr_call_pipe  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 478: /* expr_assign_pipe: expr '=' expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 479: /* expr_assign_pipe: expr "<-" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 480: /* expr_assign_pipe: expr "&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 481: /* expr_assign_pipe: expr "|=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 482: /* expr_assign_pipe: expr "^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 483: /* expr_assign_pipe: expr "&&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 484: /* expr_assign_pipe: expr "||=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 485: /* expr_assign_pipe: expr "^^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 486: /* expr_assign_pipe: expr "+=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 487: /* expr_assign_pipe: expr "-=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 488: /* expr_assign_pipe: expr "*=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 489: /* expr_assign_pipe: expr "/=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 490: /* expr_assign_pipe: expr "%=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 491: /* expr_assign_pipe: expr "<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 492: /* expr_assign_pipe: expr ">>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 493: /* expr_assign_pipe: expr "<<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 494: /* expr_assign_pipe: expr ">>>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 495: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 496: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 497: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 498: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-3])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-5]),(yyloc));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 499: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 500: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 501: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 502: /* $@24: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 503: /* $@25: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 504: /* func_addr_expr: '@' '@' '<' $@24 type_declaration_no_options '>' $@25 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value));
        expr->funcType = (yyvsp[-3].pTypeDecl);
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 505: /* $@26: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 506: /* $@27: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 507: /* func_addr_expr: '@' '@' '<' $@26 optional_function_argument_list optional_function_type '>' $@27 func_addr_name  */
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

  case 508: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 509: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 510: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 511: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-3])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-5]),(yyloc));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 512: /* expr_field: expr '.' "name" '(' '[' make_struct_fields ']' ')'  */
                                                                                       {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->methodCall = true;
        nc->arguments = (yyvsp[-2].pMakeStruct);
        nc->nonNamedArguments.push_back((yyvsp[-7].pExpression));
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 513: /* expr_field: expr '.' basic_type_declaration '(' ')'  */
                                                                        {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), method_name);
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 514: /* expr_field: expr '.' basic_type_declaration '(' expr_list ')'  */
                                                                                             {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-3])), (yyvsp[-5].pExpression), method_name);
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-5]),(yyloc));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 515: /* $@28: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 516: /* $@29: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 517: /* expr_field: expr '.' $@28 error $@29  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), "");
        yyerrok;
    }
    break;

  case 518: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 519: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
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

  case 520: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
                                                               {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-3].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 521: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
                                                                                 {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-4])),*(yyvsp[-4].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-4].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 522: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 523: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 524: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 525: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 526: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 527: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 528: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 529: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 530: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 531: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 532: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 533: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 534: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 535: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",(yyvsp[0].pExpression)); }
    break;

  case 536: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",(yyvsp[0].pExpression)); }
    break;

  case 537: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",(yyvsp[0].pExpression)); }
    break;

  case 538: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",(yyvsp[0].pExpression)); }
    break;

  case 539: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 540: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 541: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 542: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 543: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 544: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 545: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 546: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 547: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 548: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 549: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 550: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 551: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 552: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 553: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 554: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 555: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 556: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 557: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 558: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 559: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 560: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back((yyvsp[-2].pExpression));
        itv->arguments.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = itv;
    }
    break;

  case 561: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", (yyvsp[0].pExpression)); }
    break;

  case 562: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", (yyvsp[0].pExpression)); }
    break;

  case 563: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", (yyvsp[-1].pExpression)); }
    break;

  case 564: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", (yyvsp[-1].pExpression)); }
    break;

  case 565: /* expr: '(' expr_list optional_comma ')'  */
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

  case 566: /* expr: '(' make_struct_single ')'  */
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

  case 567: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 568: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 569: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 570: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 571: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 572: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 573: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 574: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 575: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression)); }
    break;

  case 576: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 577: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 578: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 579: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 580: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 581: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
        }
    break;

  case 582: /* $@30: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 583: /* $@31: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 584: /* expr: expr "is" "type" '<' $@30 type_declaration_no_options '>' $@31  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 585: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type), tokAt(scanner,(yyloc)));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),vdecl);
    }
    break;

  case 586: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 587: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 588: /* $@32: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 589: /* $@33: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 590: /* expr: expr "as" "type" '<' $@32 type_declaration '>' $@33  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 591: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 592: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 593: /* $@34: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 594: /* $@35: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 595: /* expr: expr '?' "as" "type" '<' $@34 type_declaration '>' $@35  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-8].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 596: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 597: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 598: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 599: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 600: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 601: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 602: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 603: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 604: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 605: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 606: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 607: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 608: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 609: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 610: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 611: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 612: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 613: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 614: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 615: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 616: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 617: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 618: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 619: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 620: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 621: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 622: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 623: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-6].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 624: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 625: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 626: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 627: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); /*this one is gone when BRABRA is disabled*/ }
    break;

  case 628: /* optional_field_annotation: metadata_argument_list  */
                                                        { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 629: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 630: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 631: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 632: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 633: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 634: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 635: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 636: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 637: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 638: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 639: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 640: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 641: /* struct_variable_declaration_list: struct_variable_declaration_list semicolon  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 642: /* $@36: %empty  */
                                                               { yyextra->das_force_oxford_comma=true;}
    break;

  case 643: /* struct_variable_declaration_list: struct_variable_declaration_list "typedef" $@36 "name" '=' type_declaration semicolon  */
                                                                                                                                                         {
        (yyval.pVarDeclList) = (yyvsp[-6].pVarDeclList);
        ast_structureAlias(scanner,(yyvsp[-3].s),(yyvsp[-1].pTypeDecl),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 644: /* $@37: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 645: /* struct_variable_declaration_list: struct_variable_declaration_list $@37 structure_variable_declaration semicolon  */
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

  case 646: /* $@38: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 647: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@38 function_declaration_header semicolon  */
                                                          {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 648: /* $@39: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 649: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@39 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 650: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' semicolon  */
                                                                                       {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::invalid_annotation);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 651: /* function_argument_declaration_no_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_no_type  */
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

  case 652: /* function_argument_declaration_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_type  */
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

  case 653: /* function_argument_declaration_type: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))));
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none, tokAt(scanner,(yyloc))), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 654: /* function_argument_list: function_argument_declaration_no_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 655: /* function_argument_list: function_argument_declaration_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 656: /* function_argument_list: function_argument_declaration_no_type semicolon function_argument_list  */
                                                                                            { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 657: /* function_argument_list: function_argument_declaration_type semicolon function_argument_list  */
                                                                                            { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 658: /* function_argument_list: function_argument_declaration_type ',' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 659: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 660: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 661: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 662: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                          { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 663: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 664: /* tuple_alias_type_list: tuple_alias_type_list c_or_s  */
                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 665: /* tuple_alias_type_list: tuple_alias_type_list tuple_type c_or_s  */
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

  case 666: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 667: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 668: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 669: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 670: /* variant_alias_type_list: variant_alias_type_list c_or_s  */
                                           {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 671: /* variant_alias_type_list: variant_alias_type_list variant_type c_or_s  */
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

  case 672: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 673: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 674: /* variable_declaration_no_type: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[0])));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 675: /* variable_declaration_no_type: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-1])));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 676: /* variable_declaration_no_type: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-2])));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 677: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 678: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 679: /* variable_declaration: variable_declaration_type  */
                                        {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 680: /* variable_declaration: variable_declaration_no_type  */
                                           {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 681: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 682: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 683: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 684: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 685: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 686: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 687: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 688: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 689: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 690: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "$i" '(' expr ')'  */
                                                                               {
        (yyvsp[-5].pNameWithPosList)->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = (yyvsp[-5].pNameWithPosList);
    }
    break;

  case 691: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 692: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options semicolon  */
                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[-1]));
    }
    break;

  case 693: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[-1]));
    }
    break;

  case 694: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 695: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr semicolon  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-4])));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[-1]));
    }
    break;

  case 696: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-3])));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 697: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 698: /* global_variable_declaration_list: global_variable_declaration_list "end of line"  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 699: /* $@40: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 700: /* global_variable_declaration_list: global_variable_declaration_list $@40 optional_field_annotation let_variable_declaration  */
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

  case 701: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 702: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 703: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 704: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 705: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 706: /* global_let: kwd_let optional_shared optional_public_or_private_variable open_block global_variable_declaration_list close_block  */
                                                                                                                                                      {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 707: /* $@41: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 708: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@41 optional_field_annotation let_variable_declaration  */
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

  case 709: /* enum_list: %empty  */
        {
        (yyval.pEnumList) = new Enumeration();
    }
    break;

  case 710: /* enum_list: enum_list semicolon  */
                                {
        (yyval.pEnumList) = (yyvsp[-1].pEnumList);
    }
    break;

  case 711: /* enum_list: enum_list "name" semicolon  */
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

  case 712: /* enum_list: enum_list "name" '=' expr semicolon  */
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

  case 713: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 714: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 715: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 716: /* $@42: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 717: /* single_alias: optional_public_or_private_alias "name" $@42 '=' type_declaration  */
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

  case 721: /* $@43: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 723: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 724: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 725: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 726: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 727: /* $@44: %empty  */
                                                                                                                       {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 728: /* $@45: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 729: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name open_block $@44 enum_list $@45 close_block  */
                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-2].pEnumList),Type::tInt);
    }
    break;

  case 730: /* $@46: %empty  */
                                                                                                                                                            {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 731: /* $@47: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 732: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration open_block $@46 enum_list $@47 close_block  */
                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-2].pEnumList),(yyvsp[-5].type));
    }
    break;

  case 733: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 734: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 735: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 736: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 737: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 738: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 739: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 740: /* class_or_struct: "class" "template"  */
                                 { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 741: /* class_or_struct: "struct" "template"  */
                                 { (yyval.i) = CorS_StructTemplate; }
    break;

  case 742: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 743: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 744: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 745: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 746: /* optional_struct_variable_declaration_list: open_block struct_variable_declaration_list close_block  */
                                                                      {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 747: /* $@48: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 748: /* $@49: %empty  */
                         {
        if ( (yyvsp[0].pStructure) ) {
            (yyvsp[0].pStructure)->isClass = (yyvsp[-3].i)==CorS_Class || (yyvsp[-3].i)==CorS_ClassTemplate;
            (yyvsp[0].pStructure)->isTemplate = (yyvsp[-3].i)==CorS_ClassTemplate || (yyvsp[-3].i)==CorS_StructTemplate;
            (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b);
        }
    }
    break;

  case 749: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@48 structure_name $@49 optional_struct_variable_declaration_list  */
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

  case 750: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 751: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 752: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 753: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 754: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "$i" '(' expr ')'  */
                                                                           {
        (yyvsp[-5].pNameWithPosList)->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = (yyvsp[-5].pNameWithPosList);
    }
    break;

  case 755: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 756: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 757: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 758: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 759: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 760: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 761: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 762: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 763: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 764: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 765: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 766: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 767: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 768: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 769: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 770: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 771: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 772: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 773: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 774: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 775: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 776: /* basic_type_declaration: "float16"  */
                        { (yyval.type) = Type::tFloat16; }
    break;

  case 777: /* basic_type_declaration: "half2"  */
                        { (yyval.type) = Type::tHalf2; }
    break;

  case 778: /* basic_type_declaration: "half3"  */
                        { (yyval.type) = Type::tHalf3; }
    break;

  case 779: /* basic_type_declaration: "half4"  */
                        { (yyval.type) = Type::tHalf4; }
    break;

  case 780: /* basic_type_declaration: "half8"  */
                        { (yyval.type) = Type::tHalf8; }
    break;

  case 781: /* basic_type_declaration: "short2"  */
                        { (yyval.type) = Type::tShort2; }
    break;

  case 782: /* basic_type_declaration: "short3"  */
                        { (yyval.type) = Type::tShort3; }
    break;

  case 783: /* basic_type_declaration: "short4"  */
                        { (yyval.type) = Type::tShort4; }
    break;

  case 784: /* basic_type_declaration: "short8"  */
                        { (yyval.type) = Type::tShort8; }
    break;

  case 785: /* basic_type_declaration: "ushort2"  */
                        { (yyval.type) = Type::tUShort2; }
    break;

  case 786: /* basic_type_declaration: "ushort3"  */
                        { (yyval.type) = Type::tUShort3; }
    break;

  case 787: /* basic_type_declaration: "ushort4"  */
                        { (yyval.type) = Type::tUShort4; }
    break;

  case 788: /* basic_type_declaration: "ushort8"  */
                        { (yyval.type) = Type::tUShort8; }
    break;

  case 789: /* basic_type_declaration: "byte2"  */
                        { (yyval.type) = Type::tByte2; }
    break;

  case 790: /* basic_type_declaration: "byte3"  */
                        { (yyval.type) = Type::tByte3; }
    break;

  case 791: /* basic_type_declaration: "byte4"  */
                        { (yyval.type) = Type::tByte4; }
    break;

  case 792: /* basic_type_declaration: "byte8"  */
                        { (yyval.type) = Type::tByte8; }
    break;

  case 793: /* basic_type_declaration: "byte16"  */
                        { (yyval.type) = Type::tByte16; }
    break;

  case 794: /* basic_type_declaration: "ubyte2"  */
                        { (yyval.type) = Type::tUByte2; }
    break;

  case 795: /* basic_type_declaration: "ubyte3"  */
                        { (yyval.type) = Type::tUByte3; }
    break;

  case 796: /* basic_type_declaration: "ubyte4"  */
                        { (yyval.type) = Type::tUByte4; }
    break;

  case 797: /* basic_type_declaration: "ubyte8"  */
                        { (yyval.type) = Type::tUByte8; }
    break;

  case 798: /* basic_type_declaration: "ubyte16"  */
                        { (yyval.type) = Type::tUByte16; }
    break;

  case 799: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 800: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 801: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 802: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 803: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 804: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 805: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 806: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 807: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 808: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 809: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 810: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 811: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 812: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 813: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 814: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 815: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 816: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 817: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 818: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 819: /* bitfield_bits: bitfield_bits semicolon "name"  */
                                                 {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 820: /* bitfield_bits: bitfield_bits ',' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 821: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<tuple<string,Expression *>>();
        (yyval.pNameExprList) = pSL;

    }
    break;

  case 822: /* bitfield_alias_bits: bitfield_alias_bits semicolon  */
                                            {
        (yyval.pNameExprList) = (yyvsp[-1].pNameExprList);
    }
    break;

  case 823: /* bitfield_alias_bits: bitfield_alias_bits "name" semicolon  */
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

  case 824: /* bitfield_alias_bits: bitfield_alias_bits "name" '=' expr semicolon  */
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

  case 825: /* bitfield_basic_type_declaration: %empty  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 826: /* bitfield_basic_type_declaration: ':' "uint8"  */
                             { (yyval.type) = Type::tBitfield8; }
    break;

  case 827: /* bitfield_basic_type_declaration: ':' "uint16"  */
                             { (yyval.type) = Type::tBitfield16; }
    break;

  case 828: /* bitfield_basic_type_declaration: ':' "uint"  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 829: /* bitfield_basic_type_declaration: ':' "uint64"  */
                             { (yyval.type) = Type::tBitfield64; }
    break;

  case 830: /* bitfield_type_declaration: "bitfield" bitfield_basic_type_declaration '<' '>'  */
                                                                          {
            (yyval.pTypeDecl) = new TypeDecl((yyvsp[-2].type), tokAt(scanner,(yyloc)));
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-2]));
    }
    break;

  case 831: /* $@50: %empty  */
                                                                     { yyextra->das_arrow_depth ++; }
    break;

  case 832: /* $@51: %empty  */
                                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 833: /* bitfield_type_declaration: "bitfield" bitfield_basic_type_declaration '<' $@50 bitfield_bits '>' $@51  */
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

  case 836: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
        (yyval.aTypePair).secondType->at = (yyval.aTypePair).firstType->at;
    }
    break;

  case 837: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 838: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = appendDimExpr(nullptr, (yyvsp[-1].pExpression), tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 839: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = appendDimExpr((yyvsp[-3].pTypeDecl), (yyvsp[-1].pExpression), tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 840: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type), tokAt(scanner,(yyloc))); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 841: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 842: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 843: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 844: /* type_declaration_no_options: type_declaration_no_options dim_list  */
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

  case 845: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyval.pTypeDecl) = appendAutoDim((yyvsp[-2].pTypeDecl), tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 846: /* $@52: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 847: /* $@53: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 848: /* type_declaration_no_options: "type" '<' $@52 type_declaration '>' $@53  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 849: /* type_declaration_no_options: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->typeMacroExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 850: /* type_declaration_no_options: '$' name_in_namespace optional_expr_list_in_braces  */
                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-1]), (yylsp[0]));
        (yyval.pTypeDecl)->typeMacroExpr = sequenceToList((yyvsp[0].pExpression));
        (yyval.pTypeDecl)->typeMacroExpr.insert((yyval.pTypeDecl)->typeMacroExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-1])), *(yyvsp[-1].s)));
        delete (yyvsp[-1].s);
    }
    break;

  case 851: /* $@54: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 852: /* $@55: %empty  */
                                                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 853: /* type_declaration_no_options: '$' name_in_namespace '<' $@54 type_declaration_no_options_list '>' $@55 optional_expr_list_in_braces  */
                                                                                                                                                                                              {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-6]), (yylsp[0]));
        (yyval.pTypeDecl)->typeMacroExpr = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        (yyval.pTypeDecl)->typeMacroExpr.insert((yyval.pTypeDecl)->typeMacroExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-6])), *(yyvsp[-6].s)));
        delete (yyvsp[-6].s);
    }
    break;

  case 854: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 855: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 856: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 857: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 858: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 859: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 860: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 861: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 862: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 863: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 864: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 865: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 866: /* $@56: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 867: /* $@57: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 868: /* type_declaration_no_options: "smart_ptr" '<' $@56 type_declaration '>' $@57  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 869: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tPointer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 870: /* $@58: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 871: /* $@59: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 872: /* type_declaration_no_options: "array" '<' $@58 type_declaration '>' $@59  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 873: /* $@60: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 874: /* $@61: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 875: /* type_declaration_no_options: "table" '<' $@60 table_type_pair '>' $@61  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].aTypePair).firstType;
        (yyval.pTypeDecl)->secondType = (yyvsp[-2].aTypePair).secondType;
    }
    break;

  case 876: /* $@62: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 877: /* $@63: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 878: /* type_declaration_no_options: "iterator" '<' $@62 type_declaration '>' $@63  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 879: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = (yyval.pTypeDecl)->at;
    }
    break;

  case 880: /* $@64: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 881: /* $@65: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 882: /* type_declaration_no_options: "block" '<' $@64 type_declaration '>' $@65  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 883: /* $@66: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 884: /* $@67: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 885: /* type_declaration_no_options: "block" '<' $@66 optional_function_argument_list optional_function_type '>' $@67  */
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

  case 886: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = (yyval.pTypeDecl)->at;
    }
    break;

  case 887: /* $@68: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 888: /* $@69: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 889: /* type_declaration_no_options: "function" '<' $@68 type_declaration '>' $@69  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 890: /* $@70: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 891: /* $@71: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 892: /* type_declaration_no_options: "function" '<' $@70 optional_function_argument_list optional_function_type '>' $@71  */
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

  case 893: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = (yyval.pTypeDecl)->at;
    }
    break;

  case 894: /* $@72: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 895: /* $@73: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 896: /* type_declaration_no_options: "lambda" '<' $@72 type_declaration '>' $@73  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 897: /* $@74: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 898: /* $@75: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 899: /* type_declaration_no_options: "lambda" '<' $@74 optional_function_argument_list optional_function_type '>' $@75  */
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

  case 900: /* $@76: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 901: /* $@77: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 902: /* type_declaration_no_options: "tuple" '<' $@76 tuple_type_list '>' $@77  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 903: /* $@78: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 904: /* $@79: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 905: /* type_declaration_no_options: "variant" '<' $@78 variant_type_list '>' $@79  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 906: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 907: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 908: /* type_declaration: type_declaration '|' '#'  */
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

  case 909: /* $@80: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 910: /* $@81: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 911: /* $@82: %empty  */
                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 912: /* $@83: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 913: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@80 "name" $@81 open_block $@82 tuple_alias_type_list $@83 close_block  */
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

  case 914: /* $@84: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 915: /* $@85: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 916: /* $@86: %empty  */
                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 917: /* $@87: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 918: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@84 "name" $@85 open_block $@86 variant_alias_type_list $@87 close_block  */
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

  case 919: /* $@88: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 920: /* $@89: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 921: /* $@90: %empty  */
                                                            {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 922: /* $@91: %empty  */
                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-5]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 923: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@88 "name" $@89 bitfield_basic_type_declaration open_block $@90 bitfield_alias_bits $@91 close_block  */
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

  case 924: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 925: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 926: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 927: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 928: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 929: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 930: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 931: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 932: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 933: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 934: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 935: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 936: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 937: /* make_variant_dim: %empty  */
       {
        (yyval.pExpression) = ast_makeStructToMakeVariant(nullptr, LineInfo());
    }
    break;

  case 938: /* make_variant_dim: make_struct_fields  */
                              {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 939: /* make_struct_single: make_struct_fields optional_comma  */
                                               {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 940: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 941: /* make_struct_dim: make_struct_dim "end of expression" make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 942: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 943: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 944: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 945: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 946: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 947: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 948: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 949: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 962: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 963: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 964: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 965: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = (yyvsp[-2].pTypeDecl);
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 966: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = (yyvsp[-4].pTypeDecl);
        msd->useInitializer = true;
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = msd;
    }
    break;

  case 967: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-5].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 968: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back((yyvsp[-2].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 969: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 970: /* $@92: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 971: /* $@93: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 972: /* make_struct_decl: "struct" '<' $@92 type_declaration_no_options '>' $@93 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 973: /* $@94: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 974: /* $@95: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 975: /* make_struct_decl: "class" '<' $@94 type_declaration_no_options '>' $@95 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                           {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 976: /* $@96: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 977: /* $@97: %empty  */
                                                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 978: /* make_struct_decl: "variant" '<' $@96 variant_type_list '>' $@97 '(' use_initializer make_variant_dim ')'  */
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

  case 979: /* $@98: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 980: /* $@99: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 981: /* make_struct_decl: "default" '<' $@98 type_declaration_no_options '>' $@99 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = (yyvsp[-3].pTypeDecl);
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 982: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 983: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 984: /* make_tuple: make_tuple ',' expr  */
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

  case 985: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 986: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 987: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pExpression) = mkt;
    }
    break;

  case 988: /* $@100: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 989: /* $@101: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 990: /* make_tuple_call: "tuple" '<' $@100 tuple_type_list '>' $@101 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 991: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mka;
    }
    break;

  case 992: /* make_dim: make_dim "end of expression" make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 993: /* make_dim_decl: '[' optional_expr_list ']'  */
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

  case 994: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-2].pTypeDecl);
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 995: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-2].pTypeDecl);
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 996: /* $@102: %empty  */
                                       { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 997: /* $@103: %empty  */
                                                                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 998: /* make_dim_decl: "array" "struct" '<' $@102 type_declaration_no_options '>' $@103 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 999: /* $@104: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 1000: /* $@105: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 1001: /* make_dim_decl: "array" "tuple" '<' $@104 tuple_type_list '>' $@105 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 1002: /* $@106: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 1003: /* $@107: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 1004: /* make_dim_decl: "array" "variant" '<' $@106 variant_type_list '>' $@107 '(' make_variant_dim ')'  */
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

  case 1005: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
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

  case 1006: /* $@108: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 1007: /* $@109: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 1008: /* make_dim_decl: "array" '<' $@108 type_declaration_no_options '>' $@109 '(' optional_expr_list ')'  */
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

  case 1009: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 1010: /* $@110: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 1011: /* $@111: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 1012: /* make_dim_decl: "fixed_array" '<' $@110 type_declaration_no_options '>' $@111 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = (yyvsp[-6].pTypeDecl);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 1013: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mka;
    }
    break;

  case 1014: /* make_table: make_table "end of expression" make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 1015: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 1016: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 1017: /* make_table_decl: "begin of code block" optional_expr_map_tuple_list "end of code block"  */
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

  case 1018: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
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

  case 1019: /* make_table_decl: "table" '(' optional_expr_map_tuple_list ')'  */
                                                                       {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 1020: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 1021: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 1022: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 1023: /* array_comprehension_where: "end of expression" "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 1024: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 1025: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 1026: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 1027: /* array_comprehension: '[' "iterator" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                                 {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 1028: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true,false);
    }
    break;

  case 1029: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where "end of code block" ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),false,false);
    }
    break;

  case 1030: /* array_comprehension: "begin of code block" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                              {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
    }
    break;

  case 1031: /* array_comprehension: "{{" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block" "end of code block"  */
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
