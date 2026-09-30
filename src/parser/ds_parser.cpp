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
  YYSYMBOL_UNSIGNED_INTEGER = 191,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 192,    /* "unsigned long integer constant"  */
  YYSYMBOL_UNSIGNED_INT8 = 193,            /* "unsigned int8 constant"  */
  YYSYMBOL_DAS_FLOAT = 194,                /* "floating point constant"  */
  YYSYMBOL_DAS_FLOAT16_CONST = 195,        /* "float16 constant"  */
  YYSYMBOL_DOUBLE = 196,                   /* "double constant"  */
  YYSYMBOL_NAME = 197,                     /* "name"  */
  YYSYMBOL_KEYWORD = 198,                  /* "keyword"  */
  YYSYMBOL_TYPE_FUNCTION = 199,            /* "type function"  */
  YYSYMBOL_BEGIN_STRING = 200,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 201,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 202,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 203,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 204,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 205,          /* "}"  */
  YYSYMBOL_END_OF_READ = 206,              /* "end of failed eader macro"  */
  YYSYMBOL_207_begin_of_code_block_ = 207, /* "begin of code block"  */
  YYSYMBOL_208_end_of_code_block_ = 208,   /* "end of code block"  */
  YYSYMBOL_209_end_of_expression_ = 209,   /* "end of expression"  */
  YYSYMBOL_SEMICOLON_CUR_CUR = 210,        /* ";}}"  */
  YYSYMBOL_SEMICOLON_CUR_SQR = 211,        /* ";}]"  */
  YYSYMBOL_SEMICOLON_SQR_SQR = 212,        /* ";]]"  */
  YYSYMBOL_COMMA_SQR_SQR = 213,            /* ",]]"  */
  YYSYMBOL_COMMA_CUR_SQR = 214,            /* ",}]"  */
  YYSYMBOL_215_ = 215,                     /* ','  */
  YYSYMBOL_216_ = 216,                     /* '='  */
  YYSYMBOL_217_ = 217,                     /* '?'  */
  YYSYMBOL_218_ = 218,                     /* ':'  */
  YYSYMBOL_219_ = 219,                     /* '|'  */
  YYSYMBOL_220_ = 220,                     /* '^'  */
  YYSYMBOL_221_ = 221,                     /* '&'  */
  YYSYMBOL_222_ = 222,                     /* '<'  */
  YYSYMBOL_223_ = 223,                     /* '>'  */
  YYSYMBOL_224_ = 224,                     /* '-'  */
  YYSYMBOL_225_ = 225,                     /* '+'  */
  YYSYMBOL_226_ = 226,                     /* '*'  */
  YYSYMBOL_227_ = 227,                     /* '/'  */
  YYSYMBOL_228_ = 228,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 229,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 230,               /* UNARY_PLUS  */
  YYSYMBOL_231_ = 231,                     /* '~'  */
  YYSYMBOL_232_ = 232,                     /* '!'  */
  YYSYMBOL_PRE_INC = 233,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 234,                  /* PRE_DEC  */
  YYSYMBOL_POST_INC = 235,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 236,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 237,                    /* DEREF  */
  YYSYMBOL_238_ = 238,                     /* '.'  */
  YYSYMBOL_239_ = 239,                     /* '['  */
  YYSYMBOL_240_ = 240,                     /* ']'  */
  YYSYMBOL_241_ = 241,                     /* '('  */
  YYSYMBOL_242_ = 242,                     /* ')'  */
  YYSYMBOL_243_ = 243,                     /* '$'  */
  YYSYMBOL_244_ = 244,                     /* '@'  */
  YYSYMBOL_245_ = 245,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 246,                 /* $accept  */
  YYSYMBOL_program = 247,                  /* program  */
  YYSYMBOL_top_level_reader_macro = 248,   /* top_level_reader_macro  */
  YYSYMBOL_optional_public_or_private_module = 249, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 250,              /* module_name  */
  YYSYMBOL_optional_not_required = 251,    /* optional_not_required  */
  YYSYMBOL_module_declaration = 252,       /* module_declaration  */
  YYSYMBOL_character_sequence = 253,       /* character_sequence  */
  YYSYMBOL_string_constant = 254,          /* string_constant  */
  YYSYMBOL_format_string = 255,            /* format_string  */
  YYSYMBOL_optional_format_string = 256,   /* optional_format_string  */
  YYSYMBOL_257_1 = 257,                    /* $@1  */
  YYSYMBOL_string_builder_body = 258,      /* string_builder_body  */
  YYSYMBOL_string_builder = 259,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 260, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 261,              /* expr_reader  */
  YYSYMBOL_262_2 = 262,                    /* $@2  */
  YYSYMBOL_options_declaration = 263,      /* options_declaration  */
  YYSYMBOL_require_declaration = 264,      /* require_declaration  */
  YYSYMBOL_keyword_or_name = 265,          /* keyword_or_name  */
  YYSYMBOL_require_module_name = 266,      /* require_module_name  */
  YYSYMBOL_require_module = 267,           /* require_module  */
  YYSYMBOL_is_public_module = 268,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 269,       /* expect_declaration  */
  YYSYMBOL_expect_list = 270,              /* expect_list  */
  YYSYMBOL_expect_error = 271,             /* expect_error  */
  YYSYMBOL_expression_label = 272,         /* expression_label  */
  YYSYMBOL_expression_goto = 273,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 274,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 275,          /* expression_else  */
  YYSYMBOL_semicolon = 276,                /* semicolon  */
  YYSYMBOL_if_or_static_if = 277,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 278, /* expression_else_one_liner  */
  YYSYMBOL_279_3 = 279,                    /* $@3  */
  YYSYMBOL_expression_if_one_liner = 280,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 281,  /* expression_if_then_else  */
  YYSYMBOL_282_4 = 282,                    /* $@4  */
  YYSYMBOL_expression_for_loop = 283,      /* expression_for_loop  */
  YYSYMBOL_284_5 = 284,                    /* $@5  */
  YYSYMBOL_expression_unsafe = 285,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 286,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 287,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 288,    /* expression_with_alias  */
  YYSYMBOL_289_6 = 289,                    /* $@6  */
  YYSYMBOL_290_7 = 290,                    /* $@7  */
  YYSYMBOL_annotation_argument_value = 291, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 292, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 293, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 294,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 295, /* annotation_argument_list  */
  YYSYMBOL_metadata_argument_list = 296,   /* metadata_argument_list  */
  YYSYMBOL_annotation_declaration_name = 297, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 298, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 299,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 300,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 301, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 302, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 303,   /* optional_function_type  */
  YYSYMBOL_function_name = 304,            /* function_name  */
  YYSYMBOL_optional_template = 305,        /* optional_template  */
  YYSYMBOL_global_function_declaration = 306, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 307, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 308, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 309,     /* function_declaration  */
  YYSYMBOL_310_8 = 310,                    /* $@8  */
  YYSYMBOL_open_block = 311,               /* open_block  */
  YYSYMBOL_close_block = 312,              /* close_block  */
  YYSYMBOL_expression_block = 313,         /* expression_block  */
  YYSYMBOL_expr_call_pipe = 314,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 315,           /* expression_any  */
  YYSYMBOL_expressions = 316,              /* expressions  */
  YYSYMBOL_expr_keyword = 317,             /* expr_keyword  */
  YYSYMBOL_optional_expr_list = 318,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_list_in_braces = 319, /* optional_expr_list_in_braces  */
  YYSYMBOL_optional_expr_map_tuple_list = 320, /* optional_expr_map_tuple_list  */
  YYSYMBOL_type_declaration_no_options_list = 321, /* type_declaration_no_options_list  */
  YYSYMBOL_expression_keyword = 322,       /* expression_keyword  */
  YYSYMBOL_323_9 = 323,                    /* $@9  */
  YYSYMBOL_324_10 = 324,                   /* $@10  */
  YYSYMBOL_325_11 = 325,                   /* $@11  */
  YYSYMBOL_326_12 = 326,                   /* $@12  */
  YYSYMBOL_expr_pipe = 327,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 328,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 329,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 330,     /* new_type_declaration  */
  YYSYMBOL_331_13 = 331,                   /* $@13  */
  YYSYMBOL_332_14 = 332,                   /* $@14  */
  YYSYMBOL_expr_new = 333,                 /* expr_new  */
  YYSYMBOL_expression_break = 334,         /* expression_break  */
  YYSYMBOL_expression_continue = 335,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 336, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 337,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 338, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 339,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 340,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 341,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 342,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 343,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 344,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 345, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 346,           /* expression_let  */
  YYSYMBOL_expr_cast = 347,                /* expr_cast  */
  YYSYMBOL_348_15 = 348,                   /* $@15  */
  YYSYMBOL_349_16 = 349,                   /* $@16  */
  YYSYMBOL_350_17 = 350,                   /* $@17  */
  YYSYMBOL_351_18 = 351,                   /* $@18  */
  YYSYMBOL_352_19 = 352,                   /* $@19  */
  YYSYMBOL_353_20 = 353,                   /* $@20  */
  YYSYMBOL_expr_type_decl = 354,           /* expr_type_decl  */
  YYSYMBOL_355_21 = 355,                   /* $@21  */
  YYSYMBOL_356_22 = 356,                   /* $@22  */
  YYSYMBOL_expr_type_info = 357,           /* expr_type_info  */
  YYSYMBOL_expr_list = 358,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 359,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 360,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 361,            /* capture_entry  */
  YYSYMBOL_capture_list = 362,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 363,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 364,               /* expr_block  */
  YYSYMBOL_expr_full_block = 365,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 366, /* expr_full_block_assumed_piped  */
  YYSYMBOL_367_23 = 367,                   /* $@23  */
  YYSYMBOL_expr_numeric_const = 368,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 369,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe_right = 370,   /* expr_assign_pipe_right  */
  YYSYMBOL_expr_assign_pipe = 371,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 372,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 373,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 374,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 375,           /* func_addr_expr  */
  YYSYMBOL_376_24 = 376,                   /* $@24  */
  YYSYMBOL_377_25 = 377,                   /* $@25  */
  YYSYMBOL_378_26 = 378,                   /* $@26  */
  YYSYMBOL_379_27 = 379,                   /* $@27  */
  YYSYMBOL_expr_field = 380,               /* expr_field  */
  YYSYMBOL_381_28 = 381,                   /* $@28  */
  YYSYMBOL_382_29 = 382,                   /* $@29  */
  YYSYMBOL_expr_call = 383,                /* expr_call  */
  YYSYMBOL_expr = 384,                     /* expr  */
  YYSYMBOL_385_30 = 385,                   /* $@30  */
  YYSYMBOL_386_31 = 386,                   /* $@31  */
  YYSYMBOL_387_32 = 387,                   /* $@32  */
  YYSYMBOL_388_33 = 388,                   /* $@33  */
  YYSYMBOL_389_34 = 389,                   /* $@34  */
  YYSYMBOL_390_35 = 390,                   /* $@35  */
  YYSYMBOL_expr_mtag = 391,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 392, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 393,        /* optional_override  */
  YYSYMBOL_optional_constant = 394,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 395, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 396, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 397, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 398, /* struct_variable_declaration_list  */
  YYSYMBOL_399_36 = 399,                   /* $@36  */
  YYSYMBOL_400_37 = 400,                   /* $@37  */
  YYSYMBOL_401_38 = 401,                   /* $@38  */
  YYSYMBOL_402_39 = 402,                   /* $@39  */
  YYSYMBOL_function_argument_declaration_no_type = 403, /* function_argument_declaration_no_type  */
  YYSYMBOL_function_argument_declaration_type = 404, /* function_argument_declaration_type  */
  YYSYMBOL_function_argument_list = 405,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 406,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 407,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 408,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 409,             /* variant_type  */
  YYSYMBOL_variant_type_list = 410,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 411,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 412,             /* copy_or_move  */
  YYSYMBOL_variable_declaration_no_type = 413, /* variable_declaration_no_type  */
  YYSYMBOL_variable_declaration_type = 414, /* variable_declaration_type  */
  YYSYMBOL_variable_declaration = 415,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 416,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 417,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 418, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 419, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 420, /* global_variable_declaration_list  */
  YYSYMBOL_421_40 = 421,                   /* $@40  */
  YYSYMBOL_optional_shared = 422,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 423, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 424,               /* global_let  */
  YYSYMBOL_425_41 = 425,                   /* $@41  */
  YYSYMBOL_enum_list = 426,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 427, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 428,             /* single_alias  */
  YYSYMBOL_429_42 = 429,                   /* $@42  */
  YYSYMBOL_alias_list = 430,               /* alias_list  */
  YYSYMBOL_alias_declaration = 431,        /* alias_declaration  */
  YYSYMBOL_432_43 = 432,                   /* $@43  */
  YYSYMBOL_optional_public_or_private_enum = 433, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 434,                /* enum_name  */
  YYSYMBOL_enum_declaration = 435,         /* enum_declaration  */
  YYSYMBOL_436_44 = 436,                   /* $@44  */
  YYSYMBOL_437_45 = 437,                   /* $@45  */
  YYSYMBOL_438_46 = 438,                   /* $@46  */
  YYSYMBOL_439_47 = 439,                   /* $@47  */
  YYSYMBOL_optional_structure_parent = 440, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 441,          /* optional_sealed  */
  YYSYMBOL_structure_name = 442,           /* structure_name  */
  YYSYMBOL_class_or_struct = 443,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 444, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 445, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 446,    /* structure_declaration  */
  YYSYMBOL_447_48 = 447,                   /* $@48  */
  YYSYMBOL_448_49 = 448,                   /* $@49  */
  YYSYMBOL_variable_name_with_pos_list = 449, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 450,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 451, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 452, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 453,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 454,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 455,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_basic_type_declaration = 456, /* bitfield_basic_type_declaration  */
  YYSYMBOL_bitfield_type_declaration = 457, /* bitfield_type_declaration  */
  YYSYMBOL_458_50 = 458,                   /* $@50  */
  YYSYMBOL_459_51 = 459,                   /* $@51  */
  YYSYMBOL_c_or_s = 460,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 461,          /* table_type_pair  */
  YYSYMBOL_dim_list = 462,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 463, /* type_declaration_no_options  */
  YYSYMBOL_464_52 = 464,                   /* $@52  */
  YYSYMBOL_465_53 = 465,                   /* $@53  */
  YYSYMBOL_466_54 = 466,                   /* $@54  */
  YYSYMBOL_467_55 = 467,                   /* $@55  */
  YYSYMBOL_468_56 = 468,                   /* $@56  */
  YYSYMBOL_469_57 = 469,                   /* $@57  */
  YYSYMBOL_470_58 = 470,                   /* $@58  */
  YYSYMBOL_471_59 = 471,                   /* $@59  */
  YYSYMBOL_472_60 = 472,                   /* $@60  */
  YYSYMBOL_473_61 = 473,                   /* $@61  */
  YYSYMBOL_474_62 = 474,                   /* $@62  */
  YYSYMBOL_475_63 = 475,                   /* $@63  */
  YYSYMBOL_476_64 = 476,                   /* $@64  */
  YYSYMBOL_477_65 = 477,                   /* $@65  */
  YYSYMBOL_478_66 = 478,                   /* $@66  */
  YYSYMBOL_479_67 = 479,                   /* $@67  */
  YYSYMBOL_480_68 = 480,                   /* $@68  */
  YYSYMBOL_481_69 = 481,                   /* $@69  */
  YYSYMBOL_482_70 = 482,                   /* $@70  */
  YYSYMBOL_483_71 = 483,                   /* $@71  */
  YYSYMBOL_484_72 = 484,                   /* $@72  */
  YYSYMBOL_485_73 = 485,                   /* $@73  */
  YYSYMBOL_486_74 = 486,                   /* $@74  */
  YYSYMBOL_487_75 = 487,                   /* $@75  */
  YYSYMBOL_488_76 = 488,                   /* $@76  */
  YYSYMBOL_489_77 = 489,                   /* $@77  */
  YYSYMBOL_490_78 = 490,                   /* $@78  */
  YYSYMBOL_type_declaration = 491,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 492,  /* tuple_alias_declaration  */
  YYSYMBOL_493_79 = 493,                   /* $@79  */
  YYSYMBOL_494_80 = 494,                   /* $@80  */
  YYSYMBOL_495_81 = 495,                   /* $@81  */
  YYSYMBOL_496_82 = 496,                   /* $@82  */
  YYSYMBOL_variant_alias_declaration = 497, /* variant_alias_declaration  */
  YYSYMBOL_498_83 = 498,                   /* $@83  */
  YYSYMBOL_499_84 = 499,                   /* $@84  */
  YYSYMBOL_500_85 = 500,                   /* $@85  */
  YYSYMBOL_501_86 = 501,                   /* $@86  */
  YYSYMBOL_bitfield_alias_declaration = 502, /* bitfield_alias_declaration  */
  YYSYMBOL_503_87 = 503,                   /* $@87  */
  YYSYMBOL_504_88 = 504,                   /* $@88  */
  YYSYMBOL_505_89 = 505,                   /* $@89  */
  YYSYMBOL_506_90 = 506,                   /* $@90  */
  YYSYMBOL_make_decl = 507,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 508,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 509,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 510,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim = 511,          /* make_struct_dim  */
  YYSYMBOL_make_struct_dim_list = 512,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 513,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 514, /* optional_make_struct_dim_decl  */
  YYSYMBOL_optional_block = 515,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_cur_cur = 516, /* optional_trailing_semicolon_cur_cur  */
  YYSYMBOL_optional_trailing_semicolon_cur_sqr = 517, /* optional_trailing_semicolon_cur_sqr  */
  YYSYMBOL_optional_trailing_semicolon_sqr_sqr = 518, /* optional_trailing_semicolon_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_sqr_sqr = 519, /* optional_trailing_delim_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_cur_sqr = 520, /* optional_trailing_delim_cur_sqr  */
  YYSYMBOL_use_initializer = 521,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 522,         /* make_struct_decl  */
  YYSYMBOL_523_91 = 523,                   /* $@91  */
  YYSYMBOL_524_92 = 524,                   /* $@92  */
  YYSYMBOL_525_93 = 525,                   /* $@93  */
  YYSYMBOL_526_94 = 526,                   /* $@94  */
  YYSYMBOL_527_95 = 527,                   /* $@95  */
  YYSYMBOL_528_96 = 528,                   /* $@96  */
  YYSYMBOL_529_97 = 529,                   /* $@97  */
  YYSYMBOL_530_98 = 530,                   /* $@98  */
  YYSYMBOL_make_tuple = 531,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 532,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 533,          /* make_tuple_call  */
  YYSYMBOL_534_99 = 534,                   /* $@99  */
  YYSYMBOL_535_100 = 535,                  /* $@100  */
  YYSYMBOL_make_dim = 536,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 537,            /* make_dim_decl  */
  YYSYMBOL_538_101 = 538,                  /* $@101  */
  YYSYMBOL_539_102 = 539,                  /* $@102  */
  YYSYMBOL_540_103 = 540,                  /* $@103  */
  YYSYMBOL_541_104 = 541,                  /* $@104  */
  YYSYMBOL_542_105 = 542,                  /* $@105  */
  YYSYMBOL_543_106 = 543,                  /* $@106  */
  YYSYMBOL_544_107 = 544,                  /* $@107  */
  YYSYMBOL_545_108 = 545,                  /* $@108  */
  YYSYMBOL_546_109 = 546,                  /* $@109  */
  YYSYMBOL_547_110 = 547,                  /* $@110  */
  YYSYMBOL_make_table = 548,               /* make_table  */
  YYSYMBOL_expr_map_tuple_list = 549,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 550,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 551, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 552,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 553       /* array_comprehension  */
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
#define YYLAST   16360

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  246
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  308
/* YYNRULES -- Number of rules.  */
#define YYNRULES  1017
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1850

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   473


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
       2,     2,     2,   232,     2,   245,   243,   228,   221,     2,
     241,   242,   226,   225,   215,   224,   238,   227,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   218,   209,
     222,   216,   223,   217,   244,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   239,     2,   240,   220,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   207,   219,   208,   231,     2,     2,     2,
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
     205,   206,   210,   211,   212,   213,   214,   229,   230,   233,
     234,   235,   236,   237
};

#if DAS_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   593,   593,   594,   599,   600,   601,   602,   603,   604,
     605,   606,   607,   608,   609,   610,   611,   615,   621,   622,
     623,   627,   628,   632,   633,   637,   656,   657,   658,   659,
     663,   664,   668,   669,   673,   674,   674,   678,   683,   692,
     707,   723,   728,   736,   736,   781,   811,   815,   816,   817,
     821,   824,   828,   832,   836,   840,   846,   855,   858,   864,
     865,   869,   873,   874,   878,   881,   887,   893,   896,   902,
     903,   907,   908,   909,   918,   919,   923,   924,   928,   929,
     929,   935,   936,   937,   938,   939,   943,   949,   949,   955,
     955,   961,   969,   979,   988,   988,   992,   992,   998,   999,
    1000,  1001,  1002,  1003,  1004,  1005,  1006,  1010,  1015,  1023,
    1024,  1025,  1029,  1030,  1031,  1032,  1033,  1034,  1035,  1036,
    1037,  1038,  1039,  1045,  1048,  1054,  1057,  1060,  1066,  1067,
    1068,  1069,  1073,  1090,  1112,  1115,  1125,  1140,  1155,  1170,
    1173,  1180,  1184,  1191,  1192,  1196,  1197,  1198,  1202,  1206,
    1210,  1217,  1221,  1222,  1223,  1224,  1225,  1226,  1227,  1228,
    1229,  1230,  1231,  1232,  1233,  1234,  1235,  1236,  1237,  1238,
    1239,  1240,  1241,  1242,  1243,  1244,  1245,  1246,  1247,  1248,
    1249,  1250,  1251,  1252,  1253,  1254,  1255,  1256,  1257,  1258,
    1259,  1260,  1261,  1262,  1263,  1264,  1265,  1266,  1267,  1268,
    1269,  1270,  1271,  1272,  1273,  1274,  1275,  1276,  1277,  1278,
    1279,  1280,  1281,  1282,  1283,  1284,  1285,  1286,  1287,  1288,
    1289,  1290,  1291,  1292,  1293,  1294,  1295,  1296,  1297,  1298,
    1299,  1300,  1301,  1302,  1303,  1304,  1305,  1306,  1307,  1308,
    1309,  1310,  1311,  1312,  1313,  1314,  1315,  1316,  1317,  1318,
    1319,  1320,  1321,  1322,  1323,  1324,  1325,  1326,  1327,  1328,
    1329,  1330,  1331,  1332,  1333,  1334,  1335,  1336,  1337,  1338,
    1339,  1340,  1341,  1342,  1343,  1344,  1345,  1346,  1347,  1348,
    1349,  1350,  1351,  1355,  1356,  1360,  1379,  1380,  1381,  1385,
    1391,  1391,  1409,  1410,  1413,  1414,  1417,  1421,  1432,  1441,
    1450,  1456,  1457,  1458,  1459,  1460,  1461,  1462,  1463,  1464,
    1465,  1466,  1467,  1468,  1469,  1470,  1471,  1472,  1473,  1474,
    1475,  1476,  1480,  1485,  1491,  1497,  1508,  1509,  1513,  1514,
    1518,  1519,  1523,  1527,  1534,  1534,  1534,  1540,  1540,  1540,
    1549,  1583,  1586,  1589,  1592,  1598,  1599,  1610,  1614,  1617,
    1625,  1625,  1625,  1628,  1634,  1637,  1641,  1645,  1652,  1659,
    1665,  1669,  1673,  1676,  1679,  1687,  1690,  1693,  1701,  1704,
    1712,  1715,  1718,  1726,  1732,  1733,  1734,  1738,  1739,  1743,
    1744,  1748,  1753,  1761,  1767,  1773,  1779,  1785,  1793,  1801,
    1809,  1820,  1823,  1829,  1829,  1829,  1832,  1832,  1832,  1837,
    1837,  1837,  1845,  1845,  1845,  1851,  1861,  1872,  1885,  1895,
    1906,  1921,  1924,  1930,  1931,  1939,  1951,  1952,  1953,  1957,
    1958,  1959,  1960,  1961,  1965,  1970,  1978,  1979,  1980,  1984,
    1989,  1996,  2003,  2003,  2012,  2013,  2014,  2015,  2016,  2017,
    2018,  2019,  2023,  2024,  2025,  2026,  2027,  2028,  2029,  2030,
    2031,  2032,  2033,  2034,  2035,  2036,  2037,  2038,  2039,  2040,
    2041,  2045,  2046,  2047,  2048,  2053,  2054,  2055,  2056,  2057,
    2058,  2059,  2060,  2061,  2062,  2063,  2064,  2065,  2066,  2067,
    2068,  2069,  2074,  2080,  2091,  2097,  2108,  2112,  2119,  2122,
    2122,  2122,  2127,  2127,  2127,  2140,  2144,  2148,  2154,  2162,
    2170,  2176,  2184,  2184,  2184,  2191,  2195,  2204,  2212,  2220,
    2224,  2227,  2233,  2234,  2235,  2236,  2237,  2238,  2239,  2240,
    2241,  2242,  2243,  2244,  2245,  2246,  2247,  2248,  2249,  2250,
    2251,  2252,  2253,  2254,  2255,  2256,  2257,  2258,  2259,  2260,
    2261,  2262,  2263,  2264,  2265,  2266,  2267,  2268,  2274,  2275,
    2276,  2277,  2278,  2293,  2302,  2303,  2304,  2305,  2306,  2307,
    2308,  2309,  2310,  2311,  2312,  2313,  2316,  2319,  2320,  2323,
    2323,  2323,  2326,  2331,  2335,  2339,  2339,  2339,  2344,  2347,
    2351,  2351,  2351,  2356,  2359,  2360,  2361,  2362,  2363,  2364,
    2365,  2366,  2367,  2369,  2373,  2374,  2379,  2383,  2384,  2385,
    2386,  2387,  2388,  2389,  2393,  2397,  2401,  2405,  2409,  2413,
    2417,  2421,  2425,  2432,  2433,  2434,  2438,  2439,  2440,  2444,
    2445,  2449,  2450,  2451,  2455,  2456,  2460,  2471,  2474,  2477,
    2477,  2481,  2481,  2500,  2499,  2515,  2514,  2528,  2537,  2549,
    2558,  2568,  2569,  2570,  2571,  2572,  2576,  2579,  2588,  2589,
    2593,  2596,  2599,  2614,  2623,  2624,  2628,  2631,  2634,  2647,
    2648,  2652,  2657,  2662,  2670,  2673,  2680,  2683,  2689,  2690,
    2691,  2695,  2696,  2700,  2707,  2712,  2721,  2727,  2731,  2742,
    2746,  2752,  2758,  2766,  2777,  2780,  2783,  2783,  2803,  2804,
    2808,  2809,  2810,  2814,  2817,  2817,  2836,  2839,  2842,  2857,
    2876,  2877,  2878,  2883,  2883,  2909,  2910,  2914,  2915,  2915,
    2919,  2920,  2921,  2925,  2935,  2940,  2935,  2952,  2957,  2952,
    2972,  2973,  2977,  2978,  2982,  2988,  2989,  2990,  2991,  2995,
    2996,  2997,  3001,  3004,  3010,  3015,  3010,  3035,  3042,  3047,
    3056,  3062,  3066,  3077,  3078,  3079,  3080,  3081,  3082,  3083,
    3084,  3085,  3086,  3087,  3088,  3089,  3090,  3091,  3092,  3093,
    3094,  3095,  3096,  3097,  3098,  3099,  3100,  3101,  3102,  3103,
    3104,  3105,  3106,  3107,  3108,  3109,  3110,  3111,  3112,  3113,
    3114,  3115,  3116,  3117,  3118,  3119,  3120,  3121,  3122,  3123,
    3124,  3125,  3126,  3130,  3131,  3132,  3133,  3134,  3135,  3136,
    3137,  3141,  3152,  3156,  3163,  3175,  3182,  3188,  3197,  3202,
    3205,  3215,  3228,  3229,  3230,  3231,  3232,  3236,  3240,  3240,
    3240,  3254,  3255,  3259,  3264,  3271,  3274,  3280,  3281,  3282,
    3283,  3284,  3294,  3297,  3297,  3297,  3301,  3306,  3313,  3313,
    3320,  3324,  3328,  3333,  3338,  3343,  3348,  3352,  3356,  3361,
    3365,  3369,  3374,  3374,  3374,  3380,  3387,  3387,  3387,  3392,
    3392,  3392,  3398,  3398,  3398,  3403,  3409,  3409,  3409,  3414,
    3414,  3414,  3423,  3429,  3429,  3429,  3434,  3434,  3434,  3443,
    3449,  3449,  3449,  3454,  3454,  3454,  3463,  3463,  3463,  3469,
    3469,  3469,  3478,  3481,  3492,  3508,  3508,  3513,  3518,  3508,
    3543,  3543,  3548,  3554,  3543,  3579,  3579,  3584,  3589,  3579,
    3629,  3630,  3631,  3632,  3633,  3637,  3644,  3651,  3657,  3663,
    3670,  3677,  3683,  3692,  3695,  3701,  3709,  3714,  3721,  3726,
    3733,  3738,  3744,  3745,  3749,  3750,  3755,  3756,  3760,  3761,
    3765,  3766,  3770,  3771,  3772,  3776,  3777,  3778,  3782,  3783,
    3787,  3793,  3800,  3808,  3815,  3823,  3832,  3832,  3832,  3840,
    3840,  3840,  3847,  3847,  3847,  3857,  3857,  3857,  3868,  3871,
    3877,  3891,  3897,  3903,  3909,  3909,  3909,  3922,  3927,  3934,
    3953,  3958,  3965,  3965,  3965,  3975,  3975,  3975,  3988,  3988,
    3988,  4001,  4010,  4010,  4010,  4030,  4037,  4037,  4037,  4047,
    4052,  4059,  4062,  4068,  4087,  4098,  4106,  4126,  4151,  4152,
    4156,  4157,  4162,  4165,  4168,  4171,  4174,  4177
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
  "\"long integer constant\"", "\"unsigned integer constant\"",
  "\"unsigned long integer constant\"", "\"unsigned int8 constant\"",
  "\"floating point constant\"", "\"float16 constant\"",
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

#define YYPACT_NINF (-1602)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-884)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1602,   130, -1602, -1602,    44,   -99,   355,   406, -1602,    60,
      63,    63,    63, -1602, -1602,   236,    66, -1602, -1602,   471,
   -1602, -1602, -1602, -1602,   348, -1602,    35, -1602, -1602, -1602,
   -1602, -1602, -1602, -1602, -1602, -1602, -1602,   -81, -1602,    -9,
     -28,    30, -1602,    48, -1602, -1602, -1602,   398,    57, -1602,
      34, -1602, -1602, -1602,    63,    63, -1602, -1602,    35, -1602,
   -1602, -1602, -1602, -1602,   128,   439, -1602, -1602, -1602, -1602,
      66,    66,    66,   414, -1602,   747,  -119, -1602, -1602,   505,
     583,   609,    68,   316, -1602,   786,    53,    44,   547,   -99,
     355,   355,   511,   355,   587, -1602,   763,   763, -1602,   597,
     471,     4,   471,   788,   617,   632,   634, -1602,   643,   601,
   -1602, -1602,   -57,    44,    66,    66,    66,    66, -1602, -1602,
   -1602, -1602,   842, -1602, -1602,   670, -1602, -1602, -1602, -1602,
   -1602,   406, -1602, -1602, -1602, -1602, -1602,   802,   458,    65,
     628, -1602, -1602, -1602, -1602,   511,   511,   511,   818, -1602,
   -1602, -1602, -1602, -1602, -1602, -1602,   471, -1602, -1602, -1602,
     666, -1602, -1602, -1602, -1602, -1602,   687, -1602,   163, -1602,
     399,   736,   747, -1602, -1602, -1602, -1602, -1602,   152,   801,
   -1602,   -95, -1602, -1602, -1602,   839, -1602, -1602, -1602, -1602,
   -1602, -1602, -1602,   610,   701, -1602, -1602,   252,   755, -1602,
     765, -1602,   918, -1602,   773,   406,   406, -1602, -1602, 16163,
     865, -1602, -1602,   796, -1602,   373,    44,    44,    -7,    71,
   -1602, -1602, -1602, -1602, -1602,   817,    65, -1602, -1602, 11753,
   -1602,   889,   406, -1602, -1602, 14930, -1602, -1602, -1602, -1602,
   -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602,
   -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602,
   -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602,
   -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602,
   -1602, -1602, -1602, -1602,   994,   995, -1602,   808,   406, -1602,
   -1602, -1602, -1602, -1602, -1602, -1602, -1602,   406, -1602,   850,
     406, -1602, -1602,   -95,   -83, -1602,    44, -1602,   819,  1003,
     472, -1602, -1602, -1602,   852,   856,   857,   840,   860,   867,
   -1602, -1602, -1602,   858, -1602, -1602, -1602, -1602, -1602,   549,
   -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602,
   -1602,   876, -1602, -1602, -1602,   888,   890, -1602, -1602, -1602,
   -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602,
   -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602,
   -1602, -1602, -1602, -1602,   891,   892,   870,   236, -1602, -1602,
   -1602, -1602, -1602,  1070,   897, -1602, -1602, -1602, -1602, -1602,
   -1602, -1602,   864,   887, -1602, -1602, -1602, -1602, -1602, -1602,
   -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602,
   -1602, -1602, -1602, -1602,   921,   877, -1602, -1602, -1602, -1602,
   -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602,  1099, -1602,
   -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602,
   -1602,   922,   883, -1602, -1602,   138,   -73, -1602, -1602, -1602,
     563,   236, -1602, -1602, -1602,    71,   885, -1602, 10640,   930,
     145, 11753, -1602,   -16, -1602, -1602, -1602, 10640, -1602, -1602,
     932,   910,   -72,   -64,   -44, -1602, -1602, 10640,   284, -1602,
   -1602, -1602,    27, -1602, -1602, -1602,    46,  6496, -1602,   898,
   11405, -1602, 11580,   704, -1602, -1602, -1602, -1602,   942,   920,
    1207,   901, -1602,    88,   471,   480,   902, 11753, 11753, -1602,
    2575, -1602,   310, -1602,   554, -1602,    43, -1602, -1602,   924,
     927, -1602, -1602,   167,    37,   928,    42, -1602,   459,   911,
     929,   931,   913,   933,   915,   501,   941, -1602,   605,   944,
     945, 10640, 10640,   923,   935,   938,   940,   943,   947, -1602,
   11059, 11232,  6728, -1602, -1602, -1602, -1602, -1602, -1602, -1602,
   -1602,   948,   949, -1602,  6960, 10640, 10640, 10640, 10640, 10640,
    5576,  7190, -1602,   899, -1602, -1602, -1602,   125, -1602, -1602,
   -1602, -1602,   936, -1602, -1602, -1602, -1602, -1602, -1602, -1602,
    2106, -1602,   952, -1602, -1602, -1602, -1602, -1602, -1602, -1602,
     954,  1126,   498, -1602, -1602, -1602,  4188, 11753, 11753, 11753,
   12096, 11753, 11753,   955,   960, 11753,   808, 11753,   808, 11753,
     808, 11926,   992, 12205, -1602, 10640, -1602, -1602, -1602, -1602,
   -1602,   950, -1602, -1602, 14477, 10640, -1602,  1070,   623,   -68,
   -1602, -1602,   541, -1602,   897,   554,   981,   541, -1602,   554,
   -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602,
   -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602,
   -1602, -1602, -1602, -1602, -1602, -1602, 10640, -1602, -1602,   200,
     177,   177,   177, -1602,   897,   897, -1602, 10640, -1602, -1602,
   -1602,  3498, -1602,   406,  7420, -1602, 10640,  1005, -1602,   471,
    1014,  7650,   142,   983,  3728,   209,   209,   209,  7880,   471,
     471, -1602, 10640,  1196, -1602, -1602, -1602, -1602, -1602, -1602,
    1172, -1602, -1602, -1602,    91, -1602,   471,   471,   471,   471,
   -1602,   471, -1602, -1602,  1147, -1602,     6, -1602,  1937,   563,
   10640, -1602, -1602, -1602,    66, -1602,  1206, -1602,   -95, -1602,
   -1602, -1602,   974, -1602, -1602,   236,   606, -1602,   997,  1001,
    1002, -1602, 10640, 11753, 10640, 10640, -1602, -1602, 10640, -1602,
   10640, -1602, 10640, -1602, -1602, 10640, -1602, 11753,   768,   768,
   10640, 10640, 10640, 10640, 10640, 10640,   200,  2805,   200,  3036,
     200, 15161, -1602,   886, -1602, -1602,   862,   200,  1021, -1602,
    1015,   768,   768,   -11,   768,   768,   200,  1221,   998,  1024,
   15523,   999,   -27,  1024,  1026,  1000,   454, -1602,  4418,    50,
   15826, 15881, 10640, 10640, -1602, -1602, 10640, 10640, 10640, 10640,
    1047, 10640,   383, 10640, 10640, 10640, 10640, 10640, 10640, 10640,
   10640, 10640,  8110, 10640, 10640, 10640, 10640, 10640, 10640, 10640,
   10640, 10640, 10640, 16062, 10640, -1602,  8340, 10640,  1049, -1602,
    4188, -1602,  1093, 11927,   651,   667,  1027,   497, -1602,   777,
     790, -1602, -1602,  1054,   792,   -73,   793,   -73,   800,   -73,
   -1602,   415, -1602,   456, -1602, 11753,  1037, -1602, -1602, 14512,
     430, -1602,   554, 11753, -1602, -1602, 11753, -1602, -1602, 12240,
    1012,  1212, -1602, -1602,   213, -1602, -1602, -1602, 14973,   200,
    4188, -1602,  1041, 12061,  1244, 10640, 15523,  1062, 14973,  1044,
   -1602,  1050,  1073, 15523, -1602, 11753,  4188, -1602, 12061,  1023,
   -1602,   936, -1602, -1602, -1602, 14973, -1602, -1602, 14973, -1602,
     406, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602,   144,
     209, -1602,  4648,  4648,  4648,  4648,  4648,  4648,  4648,  4648,
    4648,  4648,  4648, 10640,  4648,  4648,  4648,  4648,  4648,  4648,
   -1602,   554, 15067,  1072,   198,   843,  1208,   471, 11753, 11753,
   11753,  5806,  8570,  1077, 10640, 11753, -1602, -1602, -1602, 11753,
    1024,   977,  1038, 12347, 11753, 11753, 12382, 11753, 12489, 11753,
    1024, 11753, 11926,  1024,   992,   123, 12524, 12631, 12666, 12773,
   12808, 12915,    23,   209,  3267,  4880,  6036, 15201,  1066,     9,
     366,  1067,   192,    28,  6266,     9,   761,    58, 10640,  1075,
   10640, -1602, -1602, 11753, 11753, -1602, 10640,   312,    64, -1602,
   10640, -1602,    73,   200, -1602, 10640, -1602, 10640, -1602, 10640,
   -1602, 10640,  1046,   543, -1602, -1602,  1045,  1048,   -36, -1602,
   -1602,   -34,  5112, -1602,   301,  1055,  1051,   116,   808,  1068,
    1057, -1602, -1602,  1078,  1058, -1602, -1602,   934,   934,   656,
     656, 11225, 11225,  1063,   712,  1064, -1602, 14619,   -25,   -25,
     952,   934,   934,  2329,  1822,  1413, 15563, 16008, 15295, 15389,
   11061,  1251,   656,   656,   435,   435,   712,   712,   712,   572,
   10640,  1065,  1076,   573, 10640,  1306,  1079, 14654, -1602,   360,
   12950, -1602, -1602, 11927, 10640, 10640, 10640, 10640, 10640, 10640,
   10640, 10640, 10640, 10640, 10640, 10640, 10640, 10640, 10640, 10640,
   10640, -1602, -1602, -1602, -1602, 11753, -1602, -1602, -1602,   577,
   -1602,  1090, -1602,  1091, -1602,  1096, -1602, 11926, -1602,   992,
     578,   897, -1602,  1074, -1602, 10640, -1602, -1602,   897,   897,
   -1602, 10640,  1124,   244, 11753, -1602, 10640, -1602,    84, -1602,
    1041, 10640,   406, 15523,  1106, -1602, -1602, -1602, -1602,   987,
   -1602, 12061, -1602,    50, -1602,   845, 10640, -1602,   936,  1134,
    1134, -1602, -1602, -1602,   209,   209,   209, -1602, -1602,  2289,
   -1602,  2289, -1602,  2289, -1602,  2289, -1602,  2289, -1602,  2289,
   -1602,  2289, -1602,  2289, -1602,  2289, -1602,  2289, -1602,  2289,
   15523, -1602,  2289, -1602,  2289, -1602,  2289, -1602,  2289, -1602,
    2289, -1602,  2289, -1602, -1602,  1107,   471, -1602, -1602,   172,
   -1602,    36, -1602,  1040,  1636,   805,   638,   401,  1095,  1104,
    1135, 13057,   381, 13092,   812, 11753, 11926,   992,  1979,  1122,
    1092, 11753, -1602, -1602,  1993,  2145, -1602,  2168, -1602,  2498,
    1123,  3190,   615,  1127,   633,    50, -1602, -1602, -1602, -1602,
   -1602,  1094, 10640, -1602,  5344, 14477,    12, 10640,   543,   638,
     366, -1602, -1602,  1128, -1602, 10640, 10640, -1602,  1130, -1602,
   10640,   638,   571,  1131, -1602, -1602, 10640, 15523, -1602, -1602,
     635,   664, 15335, 10640, -1602, 10640,    86, 15523, 13199, 15523,
   15523, -1602,  1133,   176, 10640, 10640, 11753,   808,   243, -1602,
    1138,   210, 10870, -1602, -1602,   116,  1175,  1178,  1140,  1179,
    1185, -1602,   281,   -73, -1602, 10640, -1602, 10640,  8800, 10640,
   -1602,  1161,  1143, -1602, -1602, 10640,  1144, -1602, 14761, 10640,
    9030,  1145, -1602, 14796, -1602,  9260, -1602, -1602, -1602, -1602,
   15523, 15523, 15523, 15523, 15523, 15523, 15523, 15523, 15523, 15523,
   15523, 15523, 15523, 15523, 15523, 15523, 15523, -1602, -1602, -1602,
     897, -1602, -1602,  1190, -1602,  1191, -1602, -1602, -1602, -1602,
   -1602, -1602, -1602, -1602, -1602, -1602,  1148, 11753, -1602, 15067,
   13234, -1602,  1155,  1358,   -76, 15523, 10640, -1602, 11753, 10640,
      50,   808,   406, -1602, -1602, 10640, -1602,  1609,  2575,    50,
   -1602,   287,   412, -1602, -1602, -1602, 11753, -1602,  1373,    36,
   -1602, -1602,   843, -1602, -1602, -1602,  1165, -1602, -1602, -1602,
     703, -1602,  1211,  1168, -1602, -1602,  3421,   719,   723, -1602,
   -1602, 10640,  3651, -1602, -1602, -1602, -1602, -1602, -1602, -1602,
   -1602, -1602,  1170,  9490,   693,     9,   366, 15523,  1066, -1602,
   -1602, 15523,  1067, -1602,   714,     9,  1174, -1602, -1602, -1602,
   -1602,   724, -1602, -1602, -1602,  1210,   732,   734, 10640,   249,
   10640, 10640, 10640, 13341, 13376,  3881,   -73, -1602,  1177,  5112,
     425, -1602, -1602,  1219, -1602, -1602,   116,  1180,   384, 11753,
   13483, 11753, 13518, -1602,   427, 13625, -1602, 10640, 15657, 10640,
   -1602, 13660,  5112, -1602,   441, 10640, -1602, -1602, -1602,   442,
   -1602, -1602, -1602, -1602, -1602, -1602, -1602,   897, -1602, -1602,
   10640,  1224, 10640,   251,   897, 15523,   828,   -73, -1602, 14973,
   -1602,   471, -1602,   808,  1225,  1183,   227,   422, -1602, -1602,
    1373,   200,  1184,  1186, -1602, -1602, 10640,  1229,  1209, 10640,
   -1602, -1602, -1602, -1602,  1188,  1189,  1192, 10640, 10640, 10640,
    1197,  1360,  1198,  1199,  9720, -1602,   455, 10640,   366, -1602,
   10640,   571, -1602, 10640, 10640,  1148, -1602, -1602, 10640, 10640,
     783, 10640, 10640, 13767, 15523, 15523, -1602, -1602, -1602,  1218,
   -1602,   290, -1602,  1200, -1602, -1602,  9950, -1602, -1602,  4111,
   -1602,   813, -1602, -1602, -1602, 11753, 13802, 13909, -1602,   305,
   -1602, 13944, -1602, 14051, -1602, 15523, -1602, -1602,   384,   845,
    3958, -1602,   -73, -1602,   242, 11753,   -16, -1602, 16163, -1602,
   -1602, -1602, -1602,  1360,  1360, 14086,  1220,  1204, 14193,  1214,
    1216,  1217, 10640, -1602, 10640,   656,   656,   656, 10640, -1602,
   -1602,  1360,  1360, -1602, 14228, -1602, 15429, -1602, 15429, -1602,
    1241,   656, -1602,  1252,  1241, 15429, 10640, 15523, 15523,   294,
     289, -1602,  1223, -1602, 10640, 15563, -1602, -1602,   820, -1602,
   -1602,  1227, -1602, -1602, -1602, -1602, 10180, 10410, -1602, -1602,
   -1602, -1602, -1602, 15523,   406, 11753,   -16,  1991,  4188,   471,
   16163,   -89,   -89, -1602, 10640, 10640, -1602,  1360,  1360,   638,
    1228,  1238,  1024,   -89,   638, -1602,  1399,  1222,  1255,  1258,
   -1602,  1259,  1231, 15429, 10640, 10640, -1602,   289, -1602, 15657,
   -1602, -1602, -1602, -1602, 10640, 10640, 15523, -1602,  1991,  4188,
    4188, -1602, 11927, -1602,   406,   638,  1066,  1245, -1602,  1239,
    1240, 14335, 14370,   -89,   -89,  1066,  1242, -1602, -1602,  1243,
    1249,  1250, 10640,  1253,  1254,  1275, -1602, -1602,  1256, 15523,
   15523, -1602, -1602, 15523,  4188, -1602, 11927, -1602, 11927, -1602,
   -1602,   479,  1260, -1602, -1602, -1602, -1602, -1602,  1261,  1262,
   -1602, -1602, -1602, -1602, 15523, -1602, -1602, -1602, -1602, -1602,
   11927, -1602, -1602, -1602,   638, -1602, -1602, -1602,   490, -1602
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   143,     1,   377,     0,     0,     0,   708,   378,     0,
     700,   700,   700,    74,    75,     0,     0,    15,     3,     0,
      10,     9,     8,    16,     0,     7,   688,     6,    11,     5,
       4,    13,    12,    14,   110,   111,   109,   121,   123,    45,
      64,    61,    62,     0,    47,    48,    49,     0,     0,    50,
      59,    46,   293,   292,   700,   700,    22,    21,   688,   702,
     701,   905,   895,   900,     0,   345,    43,   129,   130,   131,
       0,     0,     0,   132,   134,   141,     0,   128,    17,   726,
     725,   283,   710,   729,   689,   690,     0,     0,     0,     0,
       0,     0,    51,     0,     0,    60,     0,     0,    57,     0,
       0,   700,     0,    18,     0,     0,     0,   347,     0,     0,
     140,   135,     0,     0,     0,     0,     0,     0,   144,   728,
     727,   284,   286,   712,   711,     0,   731,   730,   734,   692,
     691,   694,   119,   120,   115,   117,   113,     0,     0,     0,
       0,   112,   124,    65,    63,    53,    54,    52,    59,    56,
      55,   703,   705,   295,   294,   707,     0,   709,    19,    20,
      23,   906,   896,   901,   346,    41,    44,   139,     0,   136,
     137,   138,   142,   288,   287,   290,   285,   713,     0,   722,
     684,   613,    26,    27,    31,     0,   116,   118,   105,   106,
     101,   103,    99,     0,     0,    98,   107,     0,     0,    58,
       0,   706,     0,    25,   812,     0,     0,    42,   133,     0,
       0,   714,   723,     0,   735,   686,     0,     0,   615,     0,
      28,    29,    30,   102,   104,     0,     0,   122,   114,     0,
      24,     0,     0,   897,   902,     0,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,     0,     0,   151,   145,     0,   793,
     796,   799,   800,   794,   797,   795,   798,     0,   696,   720,
     732,   685,   693,   613,     0,   125,     0,   127,     0,   673,
     671,   695,   100,   108,     0,     0,     0,     0,     0,     0,
     743,   786,   744,   802,   745,   749,   750,   751,   752,   792,
     756,   757,   758,   759,   760,   761,   762,   787,   788,   789,
     790,   865,   748,   755,   791,   872,   879,   746,   753,   747,
     754,   763,   764,   765,   766,   767,   768,   769,   770,   771,
     772,   773,   774,   775,   776,   777,   778,   779,   780,   781,
     782,   783,   784,   785,     0,     0,     0,     0,   801,   827,
     830,   828,   829,   892,   704,   815,   816,   813,   814,   907,
     650,   656,   229,   230,   227,   154,   155,   157,   156,   158,
     159,   160,   161,   187,   188,   185,   186,   178,   189,   190,
     179,   176,   177,   228,   211,     0,   226,   191,   192,   193,
     194,   165,   166,   167,   162,   163,   164,   175,     0,   181,
     182,   180,   173,   174,   169,   168,   170,   171,   172,   153,
     152,   210,     0,   183,   184,   613,   148,   322,   291,   717,
     715,     0,   724,   627,   736,     0,     0,   126,     0,     0,
       0,     0,   672,     0,   833,   856,   859,     0,   862,   852,
       0,     0,   866,   873,   880,   886,   889,     0,   328,   842,
     847,   841,     0,   855,   851,   844,     0,     0,   846,   831,
       0,   808,   898,   903,   231,   232,   225,   209,   233,   212,
     195,     0,   146,   376,   641,   642,     0,     0,     0,   289,
       0,   696,     0,   697,     0,   721,   631,   687,   614,     0,
       0,   518,   519,     0,     0,     0,     0,   512,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   792,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   602,
       0,     0,     0,   434,   436,   435,   437,   438,   439,   440,
     441,     0,     0,    37,   330,     0,     0,     0,     0,     0,
     326,     0,   416,   417,   516,   515,   596,   513,   587,   586,
     585,   584,   143,   590,   514,   589,   588,   560,   520,   561,
       0,   521,     0,   517,   910,   914,   911,   912,   913,   675,
       0,   676,     0,   669,   670,   668,     0,     0,     0,     0,
       0,     0,     0,     0,   818,     0,   145,     0,   145,     0,
     145,     0,     0,     0,   838,   326,   837,   849,   850,   843,
     845,     0,   848,   832,     0,     0,   894,   893,   908,   345,
     821,   822,     0,   651,   646,     0,     0,     0,   657,     0,
     234,   214,   215,   217,   216,   218,   219,   220,   221,   213,
     222,   223,   224,   198,   199,   201,   200,   202,   203,   204,
     205,   196,   197,   206,   207,   208,     0,   374,   375,     0,
     613,   613,   613,   147,   150,   149,   324,     0,    76,    77,
      89,   362,   360,     0,     0,    96,     0,     0,   361,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   301,     0,     0,   317,   312,   309,   308,   310,   311,
     296,   344,   323,   303,   596,   302,     0,    84,    85,    82,
     315,    83,   316,   318,   380,   307,     0,   304,   442,   718,
       0,   698,   716,   629,     0,   628,     0,   733,   613,   956,
     959,   350,   354,   353,   359,     0,     0,   402,     0,     0,
       0,   992,     0,     0,   330,     0,   393,   396,     0,   399,
       0,   996,     0,   965,   974,     0,   962,     0,   548,   549,
       0,     0,     0,     0,     0,     0,     0,   934,     0,     0,
       0,   972,   999,     0,   334,   337,     0,     0,     0,  1001,
    1010,   525,   524,   562,   523,   522,     0,     0,     0,  1010,
     411,     0,   345,  1010,  1010,     0,   418,   594,     0,   426,
       0,     0,     0,     0,   550,   551,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   502,     0,   674,     0,     0,     0,   679,
       0,   683,     0,   442,     0,     0,     0,   823,   836,     0,
       0,   803,   817,     0,     0,   148,     0,   148,     0,   148,
     648,     0,   654,     0,   804,     0,  1010,   840,   825,     0,
       0,   809,     0,     0,   652,   899,     0,   658,   904,     0,
       0,   737,   638,   639,   661,   643,   645,   644,     0,     0,
       0,   366,   363,   411,     0,     0,   348,     0,     0,     0,
     321,     0,     0,    68,    91,     0,     0,   371,   368,   417,
     429,   143,   343,   341,   342,     0,   319,   320,     0,    87,
       0,   432,   299,   306,   313,   314,   365,   370,   379,     0,
       0,   305,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     298,     0,     0,     0,     0,   621,   624,     0,     0,     0,
       0,   948,     0,     0,     0,     0,   982,   985,   988,     0,
    1010,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1010,     0,     0,  1010,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   968,   926,   934,
       0,   977,     0,     0,     0,   934,     0,     0,     0,     0,
       0,   937,  1004,     0,     0,    40,     0,    38,     0,  1003,
    1011,   331,     0,     0,   979,  1011,   327,     0,   660,     0,
     659,     0,     0,  1011,   925,   553,     0,     0,   489,   486,
     488,     0,   326,   505,     0,     0,     0,     0,   145,     0,
       0,   573,   572,     0,     0,   574,   578,   526,   527,   539,
     540,   537,   538,     0,   567,     0,   558,     0,   591,   592,
     593,   528,   529,   544,   545,   546,   547,     0,     0,   542,
     543,   541,   535,   536,   531,   530,   532,   533,   534,     0,
       0,     0,   495,     0,     0,     0,     0,     0,   510,     0,
       0,   678,   681,   442,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   682,   834,   857,   860,     0,   863,   853,   805,     0,
     867,     0,   874,     0,   881,     0,   887,     0,   890,     0,
       0,   332,  1011,     0,   826,     0,   810,   909,   647,   653,
     640,     0,     0,     0,     0,   662,     0,    92,     0,   367,
     364,     0,     0,   349,     0,    93,    94,    66,    67,     0,
     372,   369,   418,   426,   325,    71,     0,   322,   143,     0,
       0,   392,   391,   340,     0,     0,     0,   464,   473,   452,
     474,   453,   476,   455,   475,   454,   477,   456,   467,   446,
     468,   447,   469,   448,   478,   457,   479,   458,   466,   444,
     445,   480,   459,   481,   460,   470,   449,   471,   450,   472,
     451,   465,   443,   719,   699,     0,   144,   622,   623,   624,
     625,   616,   632,     0,     0,     0,   949,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1005,   563,     0,     0,   564,     0,   595,     0,
       0,     0,     0,     0,     0,   426,   597,   598,   599,   600,
     601,     0,     0,   935,     0,   411,   934,     0,     0,     0,
       0,   943,   944,     0,   951,     0,     0,   941,     0,   980,
       0,     0,     0,     0,   939,   981,     0,   971,   936,  1000,
       0,     0,    34,     0,  1002,     0,     0,   412,     0,   916,
     915,   552,     0,     0,     0,     0,     0,   145,     0,   506,
       0,     0,     0,   509,   507,     0,     0,     0,     0,     0,
       0,   424,     0,   148,   569,     0,   575,     0,     0,     0,
     556,     0,     0,   579,   583,     0,     0,   559,     0,     0,
       0,     0,   496,     0,   503,     0,   554,   511,   677,   680,
     452,   453,   455,   454,   456,   446,   447,   448,   457,   458,
     444,   459,   460,   449,   450,   451,   443,   835,   858,   861,
     824,   864,   854,     0,   819,     0,   868,   870,   875,   877,
     882,   884,   888,   649,   891,   655,   328,     0,   329,     0,
       0,   739,     0,   740,   664,   663,     0,   373,     0,     0,
     426,   145,     0,    69,    70,     0,    86,    78,     0,   426,
     381,     0,     0,   463,   461,   462,     0,   637,   619,   616,
     617,   618,   621,   957,   960,   351,     0,   356,   357,   355,
       0,   405,     0,     0,   408,   403,     0,     0,     0,   993,
     991,   330,     0,   394,   397,   400,   997,   995,   966,   975,
     973,   963,     0,     0,     0,   934,     0,   969,   927,   950,
     942,   970,   978,   940,     0,   934,     0,   946,   947,   954,
     938,     0,   335,   338,    35,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   148,   508,     0,   326,
       0,   421,   422,     0,   420,   419,     0,     0,     0,     0,
       0,     0,     0,   484,     0,     0,   580,     0,   568,     0,
     557,     0,   326,   497,     0,     0,   555,   504,   500,     0,
     807,   820,   806,   871,   878,   885,   839,   333,   811,   738,
       0,     0,     0,     0,    97,    95,     0,   148,    72,     0,
      79,     0,   297,   145,     0,     0,   671,     0,   620,   633,
     619,     0,     0,     0,   352,   358,     0,     0,     0,     0,
     404,   983,   986,   989,     0,     0,     0,     0,     0,     0,
       0,   948,     0,     0,     0,   603,     0,     0,     0,   952,
       0,     0,   945,     0,     0,   328,    32,    39,     0,     0,
       0,     0,     0,     0,   918,   917,   487,   612,   490,     0,
     482,     0,   428,     0,   425,   427,     0,   413,   431,     0,
     611,     0,   609,   485,   606,     0,     0,     0,   605,     0,
     498,     0,   501,     0,   742,   665,    90,   300,     0,    71,
       0,    88,   148,   382,   671,     0,     0,   630,     0,   635,
     667,   666,   626,   948,   948,     0,     0,     0,     0,     0,
       0,     0,   326,  1006,   330,   395,   398,   401,     0,   949,
     967,   948,   948,   565,     0,   604,  1008,   953,  1008,   955,
    1008,   336,   339,    36,  1008,  1008,     0,   920,   919,     0,
       0,   493,     0,   423,     0,   414,   570,   576,     0,   610,
     608,     0,   607,   741,   430,    73,   362,     0,    80,    84,
      85,    82,    83,    81,     0,     0,     0,     0,     0,     0,
       0,   933,   933,   406,     0,     0,   409,   948,   948,   923,
       0,     0,  1010,   933,   923,   566,     0,     0,     0,     0,
      33,     0,     0,  1008,     0,     0,   491,     0,   483,   415,
     571,   577,   581,   499,     0,     0,   368,   433,     0,     0,
       0,   390,   442,   634,     0,     0,   930,  1010,   932,     0,
       0,     0,     0,   933,   933,   924,     0,   994,  1007,     0,
       0,     0,     0,     0,     0,     0,  1016,  1012,     0,   922,
     921,   494,   582,   369,     0,   388,   442,   386,   442,   389,
     636,     0,  1011,   931,   958,   961,   407,   410,     0,     0,
     990,   998,   976,   964,  1009,  1014,  1015,  1017,  1013,   384,
     442,   387,   385,   928,     0,   984,   987,   383,     0,   929
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1602, -1602, -1602, -1602, -1602, -1602, -1602,   663,  1409, -1602,
   -1602, -1602, -1602, -1602, -1602,  1496, -1602, -1602, -1602,   925,
     448, -1602,  1354, -1602, -1602,  1417, -1602, -1602, -1602,  -152,
      -1, -1602, -1602, -1602,  -151, -1602, -1602, -1602, -1602, -1602,
   -1602, -1602, -1602, -1602, -1602,  1282, -1602, -1602,   -46,   -40,
   -1602, -1602, -1602,   631,   766,  -488,  -606,  -856, -1602, -1602,
   -1602, -1602, -1582, -1602, -1602,    17,  -210,  -285,  -503, -1602,
     314, -1602,  -623, -1371,  -753,  -174,  -484, -1602, -1602, -1602,
   -1602,  -602,     0, -1602, -1602, -1602, -1602, -1602,  -148,  -147,
    -145, -1602,  -144, -1602, -1602, -1602,  1516, -1602,   318, -1602,
   -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602,
   -1602, -1602,  -190,  -138,   781,    -5,   178, -1149,  -670, -1602,
    -711, -1602, -1602,  -501,   394, -1602, -1602, -1602, -1601, -1602,
   -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602,   760, -1602,
   -1602, -1602, -1602, -1602, -1602, -1602,  -150,    77,   -58,    75,
     283, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602,   424,
    -465,  -964, -1602,  -468,  -962, -1602,  -887,   -48,   -47, -1602,
    -596, -1489, -1602,  -416, -1602, -1602,  1477, -1602, -1602, -1602,
    1028,  1097,   260, -1602, -1602, -1602, -1602, -1602, -1602, -1602,
   -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602,
   -1602, -1602, -1602,  -731,  -221, -1602,  1025, -1602, -1602, -1602,
    1234, -1602, -1602, -1602,  -443, -1602, -1602,  -432, -1602, -1602,
   -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602,
   -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602,
   -1602, -1602, -1602, -1602, -1602,  -197, -1602, -1602, -1602, -1602,
   -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602,
   -1602,  1034,  -767,  -217,  -765,  -756, -1602, -1602, -1306,  -973,
   -1602, -1602, -1602, -1260,   -65, -1174, -1602, -1602, -1602, -1602,
   -1602, -1602, -1602, -1602, -1602,   232,  -551, -1602, -1602, -1602,
     762, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602, -1602,
   -1602, -1602, -1602, -1602, -1602, -1333,  -779, -1602
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    17,   160,    58,   203,    18,   185,   195,  1703,
    1505,  1616,   796,   574,   166,   575,   109,    20,    21,    49,
      50,    51,    98,    22,    41,    42,   709,   710,  1435,  1436,
     641,   712,  1571,  1660,   713,   714,  1196,   715,   909,   716,
     717,   718,   719,  1429,   917,   196,   197,    37,    38,    39,
     218,    73,    74,    75,    76,    24,   446,   509,   287,   122,
      25,   175,   288,   176,   209,   447,   155,   930,  1207,   722,
     510,   723,   808,   626,   798,  1160,   576,  1033,  1614,  1034,
    1615,   725,   577,   726,   752,   980,  1584,   578,   727,   728,
     729,   730,   731,   732,   733,   679,   734,   949,  1441,  1201,
     735,   579,   994,  1597,   995,  1598,   997,  1599,   580,   985,
    1590,   581,   809,  1638,   582,  1351,  1352,  1068,   932,   583,
     970,  1198,   584,   862,  1208,   737,   585,   586,  1060,   587,
    1336,  1710,  1337,  1767,   588,  1115,  1547,   589,   810,  1529,
    1770,  1531,  1771,  1645,  1812,   591,   503,  1452,  1579,  1249,
    1251,   977,   516,   973,   748,  1668,  1740,   504,   505,   506,
     880,   881,   492,   882,   883,   493,  1051,   902,   903,  1672,
     606,   463,   310,   311,   215,   303,    85,   131,    27,   181,
     450,    99,   100,   200,   101,    28,    55,   125,   178,    29,
     298,   514,   511,   971,   452,   213,   214,    83,   128,   454,
      30,   179,   300,   904,   592,   297,   380,   381,  1149,   638,
     232,   382,   873,  1551,  1157,   866,   489,   383,   607,  1397,
     885,   612,  1402,   608,  1398,   609,  1399,   611,  1401,   615,
    1406,   616,  1553,   617,  1408,   618,  1554,   619,  1410,   620,
    1555,   621,  1412,   622,  1414,   644,    31,   105,   205,   390,
     645,    32,   106,   206,   391,   649,    33,   104,   204,   491,
     892,   593,   814,  1796,   815,  1019,  1787,  1788,  1789,  1020,
    1032,  1315,  1309,  1304,  1499,  1259,   594,   978,  1582,   979,
    1583,  1004,  1603,  1001,  1601,  1021,   799,   595,  1002,  1602,
    1022,   596,  1265,  1679,  1266,  1680,  1267,  1681,   989,  1594,
     999,  1600,   793,   800,   597,  1757,  1041,   598
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      23,   792,   886,   448,   861,   302,   860,   721,   379,   736,
     875,   992,   877,   942,   879,    66,    77,  1176,    78,  1151,
    1018,  1153,  1018,  1155,    54,   647,   724,   642,   746,   602,
    1046,   219,   384,  1025,  1052,  1054,   933,   934,  1282,   517,
    1489,   142,  1284,  1292,  1431,  1556,  1300,   758,  1310,   643,
     648,  -143,  1312,  1065,  1066,  1012,    94,  1023,   637,  1027,
     627,    59,  1013,    34,    35,  1013,  1038,    60,   132,   133,
      77,    77,    77,   168,  1048,  1042,   507,  -869,  1316,   629,
     188,   189,  1450,   743,  1323,  -876,  1739,  1666,   216,   911,
      40,    95,   811,  1325,   819,   677,   117,    84,   108,   152,
      67,   157,   927,   721,  1426,  -883,  1508,  1163,  1338,  1766,
     114,   115,   116,  -492,    77,    77,    77,    77,   787,   789,
      59,   118,   724,  1048,   830,   123,    60,   832,   833,    68,
       2,   124,    87,  1049,   603,    86,  1482,     3,   678,   108,
    1050,   832,   833,   490,   604,   508,  -869,   811,   180,   217,
     893,  -869,  1785,   455,  -876,   201,   479,   456,  1784,  -876,
       4,   950,     5,  1338,     6,  1451,  1811,   759,   760,  -869,
       7,   305,   519,   520,  -883,  1736,   304,  -876,  1178,  -883,
       8,    13,  -492,   480,   481,   167,     9,  -492,   721,  1050,
      88,   153,   526,   811,    13,   211,    69,  -883,   528,   894,
     605,   721,    14,    64,   897,  -492,    87,   724,  1339,  1338,
      10,  1269,   154,   853,   854,    14,  1258,   307,  1299,  1448,
     724,  1280,   233,   234,  1283,    70,  1609,   853,   854,   378,
     153,    13,    64,  1067,    65,   535,   536,   306,  1173,  1250,
     379,    36,   134,  1173,  1702,    89,   308,   135,   628,   389,
     136,   154,    14,   137,   190,    11,    12,    56,  1122,   191,
     457,    96,   192,    65,   761,   137,  1346,   630,   309,   379,
     482,   379,    97,  1173,   483,    90,  1347,   138,   755,  1173,
    1203,  1566,   744,   762,    93,   631,   379,   379,  1173,   193,
    1573,   632,   538,   539,   139,  1341,  1340,   140,    71,  1173,
     720,  1173,  1467,    57,   742,  1468,   747,    72,  1179,   194,
     684,   685,  1326,  1348,   449,   102,   501,   453,    13,   308,
     600,   216,   817,  1486,  1190,   107,  1048,  1199,    52,   379,
     379,   991,  1349,    64,   572,   929,  1511,  1350,    52,    14,
     484,   309,   601,  1293,   485,  1005,  1285,   486,  1697,    53,
     550,   551,   552,    79,    80,   501,    81,   721,    15,    53,
     216,   156,   487,  1048,    65,  1758,   818,  1759,   488,    16,
     210,  1761,  1762,   126,   564,   900,   724,   478,    87,   127,
     502,   813,   217,   770,    82,  1200,   379,   379,   379,   751,
     379,   379,  1050,  1048,   379,    52,   379,   901,   379,  1048,
     379,  1306,    64,  1049,  1307,   208,   570,   721,   914,  1621,
     864,   865,   867,   117,   869,   870,    53,   924,   874,  1422,
     876,   217,   878,   721,  1145,  1298,   724,  1690,  1173,  1050,
    1808,  1174,  1308,    65,  1175,   895,  1790,    52,  1246,   898,
    1159,  1423,   724,  1193,  1048,  1665,  1512,  1800,   462,   513,
    1518,   515,   572,   929,  1764,    64,   820,   821,    53,  1050,
    1735,   378,  1353,   462,  1056,  1050,  1181,   226,   724,   724,
     724,   724,   724,   724,   724,   724,   724,   724,   724,  1319,
     724,   724,   724,   724,   724,   724,    65,  1828,  1829,  1324,
     378,  1456,   378,  1189,   227,    92,  1526,  1528,    13,  1741,
    1742,   912,  1574,   680,   682,  1298,   624,   378,   378,   711,
    1050,   741,  1608,   220,   221,   745,  1342,  1753,  1754,    14,
    1298,  1527,  1611,   378,   756,   625,   740,  1575,    43,  1018,
    1712,   479,  1488,  1202,  1443,  1444,  1445,  1562,   145,   146,
    1485,   147,   379,  1343,  1018,  1721,  1253,  1254,  1271,  1636,
     378,   378,    44,    45,    46,  1495,   379,  1268,   480,   481,
     153,   301,  1274,  1275,  1085,  1277,   114,  1279,   116,  1281,
      52,    43,   990,  1793,  1794,  1181,   824,   825,  1301,  1302,
    1086,   154,  1000,    47,   830,  1003,   831,   832,   833,   834,
    1462,    53,    52,    48,   835,    44,    45,    46,   976,  1072,
    1076,   859,  1377,    13,  1463,   108,  1303,   378,   378,   378,
      13,   378,   378,    53,  1090,   378,  1181,   378,    13,   378,
      64,   378,  1622,  1177,    14,    91,    47,  1574,  1064,  1056,
     640,    14,  1116,  1185,  1057,   119,    48,   891,  1156,    14,
    1526,   490,  1181,  1457,    13,   482,  1165,   186,   603,   483,
    1194,    65,   187,  1195,  1576,   113,  1181,  1181,   604,    13,
    1629,   850,   851,   852,   379,    14,  1119,  1632,    13,  1643,
    1181,   640,   379,   853,   854,   379,  1058,   820,   821,  1158,
      14,   763,  1167,  1650,  1652,    13,    13,   460,  1161,    14,
     461,  1415,  1413,   462,  1298,   681,  1168,  1695,   920,  1169,
     764,   110,   111,   112,   379,  1298,    14,    14,   936,   937,
    1439,  1658,   640,   120,   605,   484,   490,  1417,  1595,   485,
    1180,  1843,   486,   771,  1332,   943,   944,   945,   946,    13,
     947,  1516,  1849,   820,   821,   951,   143,   487,   513,   121,
    1333,   153,   772,   488,    77,   169,   170,   171,   172,    97,
      14,    13,  1631,  1366,  1371,   982,   640,   379,   379,   379,
     512,  1243,   154,   378,   379,    13,    13,   231,   379,  1367,
    1372,  -812,    14,   379,   379,  1649,   379,   378,   379,  1496,
     379,   379,  1497,  1255,   148,  1498,    14,    14,  1264,   820,
     821,  1257,  1403,   640,   151,   822,   823,   824,   825,   223,
    1404,  1416,   165,    13,   224,   830,  1734,   831,   832,   833,
     834,    13,   379,   379,   161,   835,  1059,   836,   837,   811,
     890,    13,  1765,    13,    14,  1567,   813,   774,   983,   162,
     640,   163,    14,  1466,   813,  1338,  1161,  1161,  1479,  1472,
     164,  1159,    14,   129,    14,   158,   775,   984,   640,   130,
     640,   159,    13,   824,   825,  1657,  1481,  1432,  1502,  1320,
    1321,   830,  1141,   831,   832,   833,   834,   177,  1433,  1434,
     490,   835,   198,    14,  1142,    95,  1364,  1417,  1417,   640,
     848,   849,   850,   851,   852,   378,   490,  1503,   207,  1166,
    1143,    13,    13,   378,   853,   854,   378,  1427,   202,   173,
    1247,   646,  1607,   114,  1515,   174,  1248,    13,  1181,   824,
     825,    13,    14,    14,   114,   115,   116,   830,   640,   640,
     832,   833,   834,  1610,   379,   378,  1586,   835,    14,  1181,
     212,  1751,    14,  1613,   640,   721,   379,   736,   640,  1181,
     289,  1618,  1592,  1619,   290,   225,  1593,  1181,  1400,  1181,
     853,   854,   228,   379,   724,   820,   821,  1197,   291,   292,
      44,    45,    46,   293,   294,   295,   296,  1662,   385,  1313,
    1306,  1244,  1314,  1799,  1786,  1786,  1252,  1424,   378,   378,
     378,   229,  1795,   386,   230,   378,  1786,  1795,   387,   378,
     388,   231,  1706,   299,   378,   378,   490,   378,  1181,   378,
    1146,   378,   378,   182,   183,   184,   853,   854,  1823,   490,
     479,   490,   490,  1147,   312,  1150,  1152,  1587,  1821,   490,
     479,   149,   150,  1154,   490,  1159,  1786,  1786,  1455,   443,
     444,   490,   490,   378,   378,  1465,  1717,   480,   481,   490,
     220,   221,   222,  1772,   379,   379,   459,   480,   481,   445,
     379,   651,   652,   653,   654,   655,   656,   657,   658,  1750,
     458,   494,  1700,   182,   183,  1035,  1036,  1704,   451,  1604,
    1738,   572,   929,   479,   464,   824,   825,  1848,   465,   466,
     659,   467,   468,   830,   495,   831,   832,   833,   834,   469,
     660,   661,   662,   835,  1029,  1030,  1031,  1639,   472,   470,
     480,   481,  1484,   479,   905,   906,   907,    61,    62,    63,
     473,   477,   474,   475,   476,   379,   490,   497,   496,   499,
    1494,   498,  1379,   500,   482,   518,  1501,   599,   483,   613,
     480,   481,   614,  1506,   482,  1507,  1781,   635,   483,   650,
    1779,  1780,   676,   816,   683,   378,   749,  1568,  1405,   750,
     757,   766,   765,   767,   768,   769,   770,   378,   848,   849,
     850,   851,   852,   773,   780,    13,   776,   777,  1534,   858,
     794,   795,   853,   854,   378,    16,   781,  1815,  1817,   782,
    1544,   783,  1814,   872,   784,  1549,    14,   482,   785,   646,
     887,   483,   640,   856,   484,   857,   379,   871,   485,   896,
    1270,   486,   919,   921,   484,   925,   939,   379,   485,   940,
    1430,   486,  1839,   948,   975,   981,   487,   482,   590,   986,
    1557,   483,   488,   987,   988,   379,   487,   610,  1572,  1039,
    1040,  1564,   488,  1737,  1043,   721,  1563,   623,  1044,  1045,
    1047,  1053,  1055,  1637,  1083,  1447,  1121,   634,   950,  1577,
    1144,  1148,  1162,  1171,   724,  1172,  1181,   484,  1182,  1184,
    1186,   485,  1188,  1453,   486,   378,   378,  1192,  1187,  1245,
     738,   378,   820,   821,  1262,  1250,   721,   721,  1656,   487,
    1272,  1298,  1305,  1318,  1659,   488,  1334,   484,  1331,  1335,
    1354,   485,  1345,  1606,   486,   724,   724,  1344,  1355,  1357,
    1356,   778,   779,  1778,  1358,  1359,  1369,  1374,   379,   487,
     379,   721,   791,  1407,  1409,   488,  1418,  1370,  1620,  1411,
    1375,  1421,  1428,  1446,   791,   801,   802,   803,   804,   805,
     724,  1440,  1460,  1471,  1641,  1483,   378,  1458,   663,   664,
     665,   666,   667,   668,   669,   670,  1459,  1210,  1212,  1214,
    1216,  1218,  1220,  1222,  1224,  1226,  1228,   671,  1231,  1233,
    1235,  1237,  1239,  1241,  1470,  1477,   863,   672,  1490,  1480,
    1493,  1500,  1521,  1637,  1510,  1522,  1524,   673,   674,   675,
    1517,  1523,  1525,  1536,  1537,  1539,  1545,  1550,  1552,   625,
     822,   823,   824,   825,   826,   889,  1560,   827,   828,   829,
     830,  1561,   831,   832,   833,   834,  1578,  1585,  1588,  1589,
     835,  1604,   836,   837,  1612,  1617,  1633,   378,  1558,  1630,
    1635,  1654,  1663,  1664,   379,  1673,  1676,  1674,   378,  1682,
    1689,  1683,  1677,  1684,   820,   821,   899,   711,  1688,  1691,
    1692,  1711,  1713,  1744,   379,  1745,   378,   908,  1718,  1777,
    1756,   913,  1802,  1760,   916,  1747,   918,  1748,  1749,  1037,
    1822,   923,  1803,  1804,   928,  1768,  1805,  1806,   935,  1773,
    1797,  1807,   938,   846,   847,   848,   849,   850,   851,   852,
    1798,  1824,  1825,  1837,  1830,  1831,   931,   931,   931,   853,
     854,  1832,  1833,  1835,  1836,   141,  1838,    19,  1752,  1820,
     972,  1844,   199,  1845,  1846,   941,   144,  1725,   313,  1728,
     974,  1438,  1729,  1730,   379,  1731,  1732,    26,  1442,   941,
    1724,  1634,  1669,  1520,   791,   993,  1580,  1581,   996,   378,
     998,   378,  1449,  1670,  1671,   103,   912,  1801,  1492,   739,
    1006,  1007,  1008,  1009,  1010,  1011,  1699,  1017,   753,  1017,
       0,  1026,   822,   823,   824,   825,   826,   754,     0,   827,
     828,   829,   830,   471,   831,   832,   833,   834,     0,     0,
    1661,     0,   835,     0,   836,   837,  1667,     0,     0,     0,
     838,     0,  1077,  1078,  1180,     0,  1079,  1080,  1081,  1082,
       0,  1084,     0,  1087,  1088,  1089,  1091,  1092,  1093,  1094,
    1095,  1096,  1098,  1099,  1100,  1101,  1102,  1103,  1104,  1105,
    1106,  1107,  1108,     0,  1117,     0,     0,  1120,     0,     0,
    1123,  1570,     0,     0,     0,     0,     0,     0,     0,     0,
     820,   821,   843,   844,   845,   846,   847,   848,   849,   850,
     851,   852,     0,     0,   941,   378,     0,     0,     0,     0,
       0,   853,   854,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   378,     0,     0,     0,   479,
     913,     0,     0,     0,     0,  1183,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1191,     0,     0,     0,
       0,     0,     0,     0,   941,     0,   480,   481,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   941,
    1059,     0,  1209,  1211,  1213,  1215,  1217,  1219,  1221,  1223,
    1225,  1227,  1229,  1230,  1232,  1234,  1236,  1238,  1240,  1242,
       0,   931,     0,     0,     0,   378,     0,     0,  1783,     0,
       0,     0,  1261,     0,  1263,     0,     0,     0,   822,   823,
     824,   825,   826,     0,     0,   827,   828,   829,   830,     0,
     831,   832,   833,   834,     0,     0,     0,  1059,   835,     0,
     836,   837,     0,     0,   801,  1295,   838,   839,   840,     0,
       0,  1819,   841,   482,     0,     0,     0,   483,  1317,     0,
     791,     0,     0,     0,   931,     0,  1322,     0,     0,     0,
     791,     0,     0,     0,     0,  1327,     0,  1328,     0,  1329,
       0,  1330,     0,     0,     0,  1841,     0,  1842,     0,     0,
       0,     0,     0,     0,     0,     0,   842,     0,   843,   844,
     845,   846,   847,   848,   849,   850,   851,   852,     0,  1847,
       0,     0,     0,   820,   821,     0,     0,   853,   854,     0,
       0,     0,     0,   484,     0,     0,     0,   485,     0,  1454,
     486,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1368,     0,     0,     0,  1373,   487,     0,     0,     0,     0,
       0,   488,     0,     0,  1380,  1381,  1382,  1383,  1384,  1385,
    1386,  1387,  1388,  1389,  1390,  1391,  1392,  1393,  1394,  1395,
    1396,     0,     0,     0,   941,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1419,     0,     0,     0,     0,
       0,  1420,     0,     0,     0,     0,  1425,     0,     0,     0,
       0,  1327,     0,     0,     0,     0,     0,   -81,     0,     0,
       0,     0,     0,     0,     0,     0,  1437,     0,   820,   821,
       0,   822,   823,   824,   825,   826,     0,     0,   827,   828,
     829,   830,   941,   831,   832,   833,   834,     0,     0,     0,
       0,   835,     0,   836,   837,   931,   931,   931,     0,   838,
     941,   840,   941,     0,   941,     0,   941,     0,   941,     0,
     941,     0,   941,     0,   941,     0,   941,     0,   941,     0,
     941,     0,   479,   941,     0,   941,     0,   941,     0,   941,
       0,   941,     0,   941,   479,     0,   479,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   480,
     481,   843,   844,   845,   846,   847,   848,   849,   850,   851,
     852,   480,   481,   480,   481,     0,     0,  1487,     0,     0,
     853,   854,     0,     0,     0,  1491,  1017,     0,   952,   953,
     954,   955,   956,   957,   958,   959,   822,   823,   824,   825,
     826,   960,   961,   827,   828,   829,   830,   962,   831,   832,
     833,   834,     0,     0,  1513,  1514,   835,   963,   836,   837,
     964,   965,  1327,     0,   838,   839,   840,   966,   967,   968,
     841,     0,     0,     0,     0,  1530,     0,  1532,     0,  1535,
       0,     0,     0,     0,     0,  1538,   482,   820,   821,  1541,
     483,     0,     0,     0,     0,     0,     0,     0,   482,     0,
     482,   603,   483,     0,   483,     0,     0,     0,     0,     0,
       0,   604,     0,   969,   842,     0,   843,   844,   845,   846,
     847,   848,   849,   850,   851,   852,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   853,   854,     0,   479,     0,
     572,   929,     0,     0,     0,     0,     0,     0,     0,  1565,
       0,     0,     0,     0,     0,  1569,   484,     0,   738,     0,
     485,   479,  1469,   486,     0,   480,   481,   605,   484,     0,
     484,     0,   485,     0,   485,   486,  1473,   486,   487,     0,
       0,     0,     0,     0,   488,     0,     0,     0,   480,   481,
     487,   791,   487,     0,     0,     0,   488,     0,   488,     0,
       0,     0,     0,     0,     0,   822,   823,   824,   825,   826,
       0,     0,   827,   828,   829,   830,     0,   831,   832,   833,
     834,     0,     0,     0,     0,   835,     0,   836,   837,     0,
    1623,  1624,  1625,   838,   839,   840,     0,     0,     0,   841,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   482,     0,     0,     0,   483,  1646,     0,  1647,
       0,     0,     0,     0,     0,  1651,     0,     0,     0,     0,
     820,   821,     0,     0,     0,   482,     0,     0,     0,   483,
    1653,     0,  1655,   842,     0,   843,   844,   845,   846,   847,
     848,   849,   850,   851,   852,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   853,   854,  1675,   941,   855,  1678,
     820,   821,     0,     0,     0,     0,     0,  1685,  1686,  1687,
       0,     0,   484,     0,  1694,     0,   485,  1696,  1474,   486,
    1698,     0,     0,   791,  1701,     0,     0,     0,   791,  1705,
       0,  1707,  1708,     0,   487,   484,     0,     0,     0,   485,
     488,  1475,   486,     0,     0,     0,  1715,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   487,     0,     0,
       0,     0,     0,   488,     0,     0,     0,     0,     0,     0,
    1733,     0,     0,     0,     0,     0,     0,     0,   822,   823,
     824,   825,   826,     0,     0,   827,   828,   829,   830,     0,
     831,   832,   833,   834,   791,     0,     0,     0,   835,     0,
     836,   837,     0,     0,     0,     0,   838,   839,   840,     0,
       0,     0,   841,     0,     0,     0,  1763,     0,   822,   823,
     824,   825,   826,     0,  1769,   827,   828,   829,   830,     0,
     831,   832,   833,   834,     0,     0,     0,  1776,   835,     0,
     836,   837,     0,     0,     0,     0,     0,     0,  1782,     0,
       0,     0,     0,     0,  1791,  1792,   842,     0,   843,   844,
     845,   846,   847,   848,   849,   850,   851,   852,     0,     0,
       0,     0,     0,     0,  1809,  1810,     0,   853,   854,     0,
       0,   479,   572,   929,     0,  1813,     0,     0,     0,  1816,
    1818,     0,     0,     0,     0,     0,     0,     0,   843,   844,
     845,   846,   847,   848,   849,   850,   851,   852,   480,   481,
       0,     0,  1834,   941,     0,     0,     0,   853,   854,     0,
       0,     0,     0,     0,  1840,     0,   686,     0,     0,     0,
     519,   520,     3,     0,   687,   688,   689,     0,   690,     0,
     521,   522,   523,   524,   525,     0,     0,   941,     0,   941,
     526,   691,   527,   692,   693,     0,   528,     0,     0,     0,
       0,     0,     0,   694,   529,   695,     0,   696,     0,   697,
     530,   941,     0,   531,     0,     8,   532,   698,     0,   699,
     533,     0,     0,   700,   701,     0,     0,     0,     0,     0,
     702,     0,     0,   535,   536,   482,   320,   321,   322,   483,
     324,   325,   326,   327,   328,   537,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,     0,   342,   343,
     344,     0,     0,   347,   348,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     538,   539,   703,   704,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   484,   541,   542,     0,   485,
       0,  1476,   486,     0,     0,     0,     0,     0,     0,     0,
       0,   705,   706,   707,     0,     0,     0,   487,     0,     0,
       0,    64,     0,   488,     0,     0,     0,     0,     0,   543,
     544,   545,   546,   547,     0,   548,     0,   549,   550,   551,
     552,     0,   153,    13,   553,   554,   555,   556,   557,   558,
     559,   560,    65,   708,   562,   563,     0,     0,     0,     0,
       0,     0,   564,   154,    14,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   565,
     566,   567,     0,    15,     0,     0,   568,   569,     0,     0,
     519,   520,     0,     0,   570,     0,   571,     0,   572,   573,
     521,   522,   523,   524,   525,     0,     0,     0,     0,     0,
     526,     0,   527,     0,     0,     0,   528,     0,   479,     0,
       0,     0,     0,     0,   529,     0,     0,     0,     0,     0,
     530,     0,     0,   531,     0,     0,   532,     0,  1013,     0,
     533,     0,     0,     0,     0,   480,   481,     0,     0,     0,
     534,     0,     0,   535,   536,     0,   320,   321,   322,     0,
     324,   325,   326,   327,   328,   537,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,     0,   342,   343,
     344,     0,     0,   347,   348,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     538,   539,   540,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   541,   542,     0,     0,
       0,     0,   482,     0,     0,     0,   483,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,   543,
     544,   545,   546,   547,     0,   548,   811,   549,   550,   551,
     552,     0,     0,     0,   553,   554,   555,   556,   557,   558,
     559,   560,   812,   561,   562,   563,     0,     0,     0,     0,
       0,     0,   564,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   484,     0,     0,     0,   485,     0,     0,  1014,
     566,   567,     0,    15,     0,     0,   568,   569,     0,     0,
       0,   519,   520,     0,  1015,     0,  1016,     0,   572,   573,
     488,   521,   522,   523,   524,   525,     0,     0,     0,     0,
       0,   526,     0,   527,     0,     0,     0,   528,     0,   479,
       0,     0,     0,     0,     0,   529,     0,     0,     0,     0,
       0,   530,     0,     0,   531,     0,     0,   532,     0,     0,
       0,   533,     0,     0,     0,     0,   480,   481,     0,     0,
       0,   534,     0,     0,   535,   536,     0,   320,   321,   322,
       0,   324,   325,   326,   327,   328,   537,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,     0,   342,
     343,   344,     0,     0,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   368,   369,   370,   371,   372,
     373,   538,   539,   540,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   541,   542,     0,
       0,     0,     0,   482,     0,     0,     0,   483,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    64,     0,     0,     0,     0,     0,     0,     0,
     543,   544,   545,   546,   547,     0,   548,   811,   549,   550,
     551,   552,     0,   479,     0,   553,   554,   555,   556,   557,
     558,   559,   560,   812,   561,   562,   563,     0,     0,     0,
       0,     0,     0,   564,     0,     0,     0,     0,     0,     0,
     480,   481,     0,   484,     0,     0,     0,   485,     0,     0,
    1014,   566,   567,     0,    15,     0,     0,   568,   569,     0,
       0,     0,   519,   520,     0,  1015,     0,  1024,     0,   572,
     573,   488,   521,   522,   523,   524,   525,     0,     0,     0,
       0,     0,   526,     0,   527,     0,     0,     0,   528,     0,
     629,     0,     0,     0,     0,     0,   529,     0,     0,     0,
       0,     0,   530,     0,     0,   531,     0,     0,   532,     0,
       0,     0,   533,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   534,     0,     0,   535,   536,   482,   320,   321,
     322,   483,   324,   325,   326,   327,   328,   537,   330,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,     0,
     342,   343,   344,     0,     0,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   538,   539,   540,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   484,   541,   542,
       0,   485,     0,  1478,   486,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   487,
       0,     0,     0,    64,     0,   488,     0,     0,     0,     0,
       0,   543,   544,   545,   546,   547,     0,   548,     0,   549,
     550,   551,   552,     0,   479,     0,   553,   554,   555,   556,
     557,   558,   559,   560,    65,   561,   562,   563,     0,     0,
       0,     0,     0,     0,   564,     0,     0,     0,     0,     0,
       0,   480,   481,     0,     0,     0,     0,     0,   630,     0,
       0,   565,   566,   567,     0,    15,     0,     0,   568,   569,
       0,     0,     0,   519,   520,     0,  1294,     0,   571,     0,
     572,   573,   632,   521,   522,   523,   524,   525,     0,     0,
       0,     0,     0,   526,     0,   527,     0,     0,     0,   528,
       0,     0,     0,     0,     0,     0,     0,   529,     0,     0,
       0,     0,     0,   530,     0,     0,   531,     0,     0,   532,
       0,     0,     0,   533,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   534,     0,     0,   535,   536,   482,   320,
     321,   322,   483,   324,   325,   326,   327,   328,   537,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
       0,   342,   343,   344,     0,     0,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   538,   539,   703,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   484,   541,
     542,     0,   485,     0,  1591,   486,     0,     0,   910,     0,
       0,     0,     0,     0,   705,   706,   707,     0,     0,     0,
     487,     0,     0,     0,    64,     0,   488,     0,     0,     0,
       0,     0,   543,   544,   545,   546,   547,     0,   548,     0,
     549,   550,   551,   552,   479,     0,     0,   553,   554,   555,
     556,   557,   558,   559,   560,    65,   561,   562,   563,     0,
       0,     0,     0,     0,     0,   564,     0,     0,     0,     0,
       0,   480,   481,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   565,   566,   567,     0,    15,     0,     0,   568,
     569,     0,     0,   519,   520,     0,     0,   570,     0,   571,
       0,   572,   573,   521,   522,   523,   524,   525,     0,     0,
       0,     0,     0,   526,     0,   527,     0,     0,     0,   528,
       0,     0,     0,     0,     0,     0,     0,   529,     0,     0,
       0,     0,     0,   530,     0,     0,   531,     0,     0,   532,
       0,     0,     0,   533,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   534,     0,     0,   535,   536,   482,   320,
     321,   322,   483,   324,   325,   326,   327,   328,   537,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
       0,   342,   343,   344,     0,     0,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   538,   539,   703,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   484,   541,
     542,     0,   485,     0,  1596,   486,     0,     0,   926,     0,
       0,     0,     0,     0,   705,   706,   707,     0,     0,     0,
     487,     0,     0,     0,    64,     0,   488,     0,     0,     0,
       0,     0,   543,   544,   545,   546,   547,     0,   548,     0,
     549,   550,   551,   552,   479,     0,     0,   553,   554,   555,
     556,   557,   558,   559,   560,    65,   561,   562,   563,     0,
       0,     0,     0,     0,     0,   564,     0,     0,     0,     0,
       0,   480,   481,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   565,   566,   567,     0,    15,     0,     0,   568,
     569,     0,     0,   519,   520,     0,     0,   570,     0,   571,
       0,   572,   573,   521,   522,   523,   524,   525,     0,     0,
       0,     0,     0,   526,  1726,   527,   692,     0,     0,   528,
       0,     0,     0,     0,     0,     0,     0,   529,     0,     0,
       0,     0,     0,   530,     0,     0,   531,     0,     0,   532,
     698,     0,     0,   533,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   534,     0,     0,   535,   536,   482,   320,
     321,   322,   483,   324,   325,   326,   327,   328,   537,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
       0,   342,   343,   344,     0,     0,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   538,   539,   540,  1727,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   484,   541,
     542,     0,   485,     0,  1628,   486,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     487,     0,     0,     0,    64,     0,   488,     0,     0,     0,
       0,     0,   543,   544,   545,   546,   547,     0,   548,     0,
     549,   550,   551,   552,   479,     0,     0,   553,   554,   555,
     556,   557,   558,   559,   560,    65,   561,   562,   563,     0,
       0,     0,     0,     0,     0,   564,     0,     0,     0,     0,
       0,   480,   481,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   565,   566,   567,     0,    15,     0,     0,   568,
     569,     0,     0,   519,   520,     0,     0,   570,     0,   571,
       0,   572,   573,   521,   522,   523,   524,   525,     0,     0,
       0,     0,     0,   526,     0,   527,     0,     0,     0,   528,
       0,     0,     0,     0,     0,     0,     0,   529,     0,     0,
       0,     0,     0,   530,     0,     0,   531,     0,     0,   532,
       0,     0,     0,   533,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   534,     0,     0,   535,   536,   482,   320,
     321,   322,   483,   324,   325,   326,   327,   328,   537,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
       0,   342,   343,   344,     0,     0,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   538,   539,   703,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   484,   541,
     542,     0,   485,     0,  1716,   486,     0,     0,     0,     0,
       0,     0,     0,     0,   705,   706,   707,     0,     0,     0,
     487,     0,     0,     0,    64,     0,   488,     0,     0,     0,
       0,     0,   543,   544,   545,   546,   547,     0,   548,     0,
     549,   550,   551,   552,     0,     0,     0,   553,   554,   555,
     556,   557,   558,   559,   560,    65,   561,   562,   563,     0,
       0,     0,     0,     0,     0,   564,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   565,   566,   567,     0,    15,     0,     0,   568,
     569,     0,     0,   519,   520,     0,     0,   570,     0,   571,
       0,   572,   573,   521,   522,   523,   524,   525,     0,     0,
       0,     0,     0,   526,     0,   527,     0,     0,     0,   528,
       0,     0,     0,     0,     0,     0,     0,   529,     0,     0,
       0,     0,     0,   530,     0,     0,   531,     0,     0,   532,
       0,     0,     0,   533,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   534,     0,     0,   535,   536,  1061,   320,
     321,   322,     0,   324,   325,   326,   327,   328,   537,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
       0,   342,   343,   344,     0,     0,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   538,   539,   540,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   541,
     542,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,   543,   544,   545,   546,   547,     0,   548,   811,
     549,   550,   551,   552,     0,     0,     0,   553,   554,   555,
     556,   557,   558,   559,   560,   812,   561,   562,   563,     0,
       0,     0,     0,     0,     0,   564,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   565,   566,   567,     0,    15,     0,     0,   568,
     569,     0,     0,   519,   520,     0,     0,  1062,     0,   571,
    1063,   572,   573,   521,   522,   523,   524,   525,     0,     0,
       0,     0,     0,   526,     0,   527,     0,     0,     0,   528,
       0,     0,     0,     0,     0,     0,     0,   529,     0,     0,
       0,     0,     0,   530,     0,     0,   531,     0,     0,   532,
       0,     0,     0,   533,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   534,     0,     0,   535,   536,     0,   320,
     321,   322,     0,   324,   325,   326,   327,   328,   537,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
       0,   342,   343,   344,     0,     0,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   538,   539,   703,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   541,
     542,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1204,  1205,  1206,     0,     0,     0,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,   543,   544,   545,   546,   547,     0,   548,     0,
     549,   550,   551,   552,     0,     0,     0,   553,   554,   555,
     556,   557,   558,   559,   560,    65,   561,   562,   563,     0,
       0,     0,     0,     0,     0,   564,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   565,   566,   567,     0,    15,     0,     0,   568,
     569,     0,     0,     0,     0,   519,   520,   570,     0,   571,
       0,   572,   573,   806,     0,   521,   522,   523,   524,   525,
       0,     0,     0,     0,     0,   526,     0,   527,     0,     0,
       0,   528,     0,     0,     0,     0,     0,     0,     0,   529,
       0,     0,     0,     0,     0,   530,     0,     0,   531,   807,
       0,   532,     0,     0,     0,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,     0,     0,   535,   536,
       0,   320,   321,   322,     0,   324,   325,   326,   327,   328,
     537,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,     0,   342,   343,   344,     0,     0,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   538,   539,   540,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   541,   542,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,   543,   544,   545,   546,   547,     0,
     548,     0,   549,   550,   551,   552,     0,     0,     0,   553,
     554,   555,   556,   557,   558,   559,   560,    65,   561,   562,
     563,     0,     0,     0,     0,     0,     0,   564,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   565,   566,   567,     0,    15,     0,
       0,   568,   569,     0,     0,     0,     0,   519,   520,   570,
     633,   571,     0,   572,   573,   806,     0,   521,   522,   523,
     524,   525,     0,     0,     0,     0,     0,   526,     0,   527,
       0,     0,     0,   528,     0,     0,     0,     0,     0,     0,
       0,   529,     0,     0,     0,     0,     0,   530,     0,     0,
     531,   807,     0,   532,     0,     0,     0,   533,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   534,     0,     0,
     535,   536,     0,   320,   321,   322,     0,   324,   325,   326,
     327,   328,   537,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,     0,   342,   343,   344,     0,     0,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   364,   365,   366,
     367,   368,   369,   370,   371,   372,   373,   538,   539,   540,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   541,   542,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
       0,     0,     0,     0,     0,     0,   543,   544,   545,   546,
     547,     0,   548,   811,   549,   550,   551,   552,     0,     0,
       0,   553,   554,   555,   556,   557,   558,   559,   560,   812,
     561,   562,   563,     0,     0,     0,     0,     0,     0,   564,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   565,   566,   567,     0,
      15,     0,     0,   568,   569,     0,     0,     0,     0,   519,
     520,   570,     0,   571,     0,   572,   573,   806,     0,   521,
     522,   523,   524,   525,     0,     0,     0,     0,     0,   526,
       0,   527,     0,     0,     0,   528,     0,     0,     0,     0,
       0,     0,     0,   529,     0,     0,     0,     0,     0,   530,
       0,     0,   531,   807,     0,   532,     0,     0,     0,   533,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   534,
       0,     0,   535,   536,     0,   320,   321,   322,     0,   324,
     325,   326,   327,   328,   537,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,     0,   342,   343,   344,
       0,     0,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   538,
     539,   540,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   541,   542,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,     0,     0,     0,     0,     0,     0,   543,   544,
     545,   546,   547,     0,   548,     0,   549,   550,   551,   552,
       0,     0,     0,   553,   554,   555,   556,   557,   558,   559,
     560,    65,   561,   562,   563,     0,     0,     0,     0,     0,
       0,   564,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   565,   566,
     567,     0,    15,     0,     0,   568,   569,     0,     0,     0,
       0,   519,   520,   570,   887,   571,     0,   572,   573,   806,
       0,   521,   522,   523,   524,   525,     0,     0,     0,     0,
       0,   526,     0,   527,     0,     0,     0,   528,     0,     0,
       0,     0,     0,     0,     0,   529,     0,     0,     0,     0,
       0,   530,     0,     0,   531,   807,     0,   532,     0,     0,
       0,   533,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   534,     0,     0,   535,   536,     0,   320,   321,   322,
       0,   324,   325,   326,   327,   328,   537,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,     0,   342,
     343,   344,     0,     0,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   368,   369,   370,   371,   372,
     373,   538,   539,   540,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   541,   542,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    64,     0,     0,     0,     0,     0,     0,     0,
     543,   544,   545,   546,   547,     0,   548,     0,   549,   550,
     551,   552,     0,     0,     0,   553,   554,   555,   556,   557,
     558,   559,   560,    65,   561,   562,   563,     0,     0,     0,
       0,     0,     0,   564,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     565,   566,   567,     0,    15,     0,     0,   568,   569,     0,
       0,   519,   520,     0,     0,   570,     0,   571,     0,   572,
     573,   521,   522,   523,   524,   525,     0,     0,     0,     0,
       0,   526,     0,   527,     0,     0,     0,   528,     0,     0,
       0,     0,     0,     0,     0,   529,     0,     0,     0,     0,
       0,   530,     0,     0,   531,     0,     0,   532,     0,     0,
       0,   533,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   534,     0,     0,   535,   536,  1256,   320,   321,   322,
       0,   324,   325,   326,   327,   328,   537,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,     0,   342,
     343,   344,     0,     0,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   368,   369,   370,   371,   372,
     373,   538,   539,   540,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   541,   542,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    64,     0,     0,     0,     0,     0,     0,     0,
     543,   544,   545,   546,   547,     0,   548,   811,   549,   550,
     551,   552,     0,     0,     0,   553,   554,   555,   556,   557,
     558,   559,   560,   812,   561,   562,   563,     0,     0,     0,
       0,     0,     0,   564,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     565,   566,   567,     0,    15,     0,     0,   568,   569,     0,
       0,   519,   520,     0,     0,   570,     0,   571,     0,   572,
     573,   521,   522,   523,   524,   525,     0,     0,     0,     0,
       0,   526,     0,   527,     0,     0,     0,   528,     0,     0,
       0,     0,     0,     0,     0,   529,     0,     0,     0,     0,
       0,   530,     0,     0,   531,     0,     0,   532,     0,     0,
       0,   533,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   534,     0,     0,   535,   536,     0,   320,   321,   322,
       0,   324,   325,   326,   327,   328,   537,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,     0,   342,
     343,   344,     0,     0,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   368,   369,   370,   371,   372,
     373,   538,   539,   540,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   541,   542,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    64,     0,     0,     0,     0,     0,     0,     0,
     543,   544,   545,   546,   547,     0,   548,   811,   549,   550,
     551,   552,     0,     0,     0,   553,   554,   555,   556,   557,
     558,   559,   560,   812,   561,   562,   563,     0,     0,     0,
       0,     0,     0,   564,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     565,   566,   567,     0,    15,     0,     0,   568,   569,     0,
       0,   519,   520,     0,     0,   570,     0,   571,  1296,   572,
     573,   521,   522,   523,   524,   525,     0,     0,     0,     0,
       0,   526,     0,   527,     0,     0,     0,   528,     0,     0,
       0,     0,     0,     0,     0,   529,     0,     0,     0,     0,
       0,   530,     0,     0,   531,     0,     0,   532,     0,     0,
       0,   533,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   534,     0,     0,   535,   536,     0,   320,   321,   322,
       0,   324,   325,   326,   327,   328,   537,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,     0,   342,
     343,   344,     0,     0,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   368,   369,   370,   371,   372,
     373,   538,   539,   540,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   541,   542,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    64,     0,     0,     0,     0,     0,     0,     0,
     543,   544,   545,   546,   547,     0,   548,   811,   549,   550,
     551,   552,     0,     0,     0,   553,   554,   555,   556,   557,
     558,   559,   560,   812,   561,   562,   563,     0,     0,     0,
       0,     0,     0,   564,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     565,   566,   567,     0,    15,     0,     0,   568,   569,     0,
       0,   519,   520,     0,     0,   570,     0,   571,  1311,   572,
     573,   521,   522,   523,   524,   525,     0,     0,     0,     0,
       0,   526,     0,   527,     0,     0,     0,   528,     0,     0,
       0,     0,     0,     0,     0,   529,     0,     0,     0,     0,
       0,   530,     0,     0,   531,     0,     0,   532,     0,     0,
       0,   533,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   534,     0,     0,   535,   536,     0,   320,   321,   322,
       0,   324,   325,   326,   327,   328,   537,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,     0,   342,
     343,   344,     0,     0,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   368,   369,   370,   371,   372,
     373,   538,   539,   540,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   541,   542,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    64,     0,     0,     0,     0,     0,     0,     0,
     543,   544,   545,   546,   547,     0,   548,     0,   549,   550,
     551,   552,     0,     0,     0,   553,   554,   555,   556,   557,
     558,   559,   560,    65,   561,   562,   563,     0,     0,     0,
       0,     0,     0,   564,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     565,   566,   567,     0,    15,     0,     0,   568,   569,     0,
       0,     0,     0,   519,   520,   570,   633,   571,     0,   572,
     573,   790,     0,   521,   522,   523,   524,   525,     0,     0,
       0,     0,     0,   526,     0,   527,     0,     0,     0,   528,
       0,     0,     0,     0,     0,     0,     0,   529,     0,     0,
       0,     0,     0,   530,     0,     0,   531,     0,     0,   532,
       0,     0,     0,   533,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   534,     0,     0,   535,   536,     0,   320,
     321,   322,     0,   324,   325,   326,   327,   328,   537,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
       0,   342,   343,   344,     0,     0,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   538,   539,   540,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   541,
     542,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,   543,   544,   545,   546,   547,     0,   548,     0,
     549,   550,   551,   552,     0,     0,     0,   553,   554,   555,
     556,   557,   558,   559,   560,    65,   561,   562,   563,     0,
       0,     0,     0,     0,     0,   564,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   565,   566,   567,     0,    15,     0,     0,   568,
     569,     0,     0,     0,     0,   519,   520,   570,     0,   571,
       0,   572,   573,   797,     0,   521,   522,   523,   524,   525,
       0,     0,     0,     0,     0,   526,     0,   527,     0,     0,
       0,   528,     0,     0,     0,     0,     0,     0,     0,   529,
       0,     0,     0,     0,     0,   530,     0,     0,   531,     0,
       0,   532,     0,     0,     0,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,     0,     0,   535,   536,
       0,   320,   321,   322,     0,   324,   325,   326,   327,   328,
     537,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,     0,   342,   343,   344,     0,     0,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   538,   539,   540,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   541,   542,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,   543,   544,   545,   546,   547,     0,
     548,     0,   549,   550,   551,   552,     0,     0,     0,   553,
     554,   555,   556,   557,   558,   559,   560,    65,   561,   562,
     563,     0,     0,     0,     0,     0,     0,   564,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   565,   566,   567,     0,    15,     0,
       0,   568,   569,     0,     0,   519,   520,     0,     0,   570,
       0,   571,     0,   572,   573,   521,   522,   523,   524,   525,
       0,     0,     0,     0,     0,   526,     0,   527,     0,     0,
       0,   528,     0,     0,     0,     0,     0,     0,     0,   529,
       0,     0,     0,     0,     0,   530,     0,     0,   531,     0,
       0,   532,     0,     0,     0,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,     0,     0,   535,   536,
       0,   320,   321,   322,     0,   324,   325,   326,   327,   328,
     537,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,     0,   342,   343,   344,     0,     0,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   538,   539,   540,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   541,   542,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,   543,   544,   545,   546,   547,     0,
     548,   811,   549,   550,   551,   552,     0,     0,     0,   553,
     554,   555,   556,   557,   558,   559,   560,   812,   561,   562,
     563,     0,     0,     0,     0,     0,     0,   564,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   565,   566,   567,     0,    15,     0,
       0,   568,   569,     0,     0,   519,   520,     0,     0,   570,
       0,   571,     0,   572,   573,   521,   522,   523,   524,   525,
       0,     0,     0,     0,     0,   526,     0,   527,     0,     0,
       0,   528,     0,     0,     0,     0,     0,     0,     0,   529,
       0,     0,     0,     0,     0,   530,     0,     0,   531,     0,
       0,   532,     0,     0,     0,   533,     0,     0,     0,     0,
       0,   915,     0,     0,     0,   534,     0,     0,   535,   536,
       0,   320,   321,   322,     0,   324,   325,   326,   327,   328,
     537,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,     0,   342,   343,   344,     0,     0,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   538,   539,   540,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   541,   542,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,   543,   544,   545,   546,   547,     0,
     548,     0,   549,   550,   551,   552,     0,     0,     0,   553,
     554,   555,   556,   557,   558,   559,   560,    65,   561,   562,
     563,     0,     0,     0,     0,     0,     0,   564,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   565,   566,   567,     0,    15,     0,
       0,   568,   569,     0,     0,   519,   520,     0,     0,   570,
       0,   571,     0,   572,   573,   521,   522,   523,   524,   525,
       0,     0,     0,     0,     0,   526,     0,   527,     0,     0,
       0,   528,     0,     0,     0,     0,     0,     0,     0,   529,
       0,     0,     0,     0,     0,   530,     0,     0,   531,     0,
       0,   532,     0,     0,     0,   533,     0,     0,   922,     0,
       0,     0,     0,     0,     0,   534,     0,     0,   535,   536,
       0,   320,   321,   322,     0,   324,   325,   326,   327,   328,
     537,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,     0,   342,   343,   344,     0,     0,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   538,   539,   540,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   541,   542,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,   543,   544,   545,   546,   547,     0,
     548,     0,   549,   550,   551,   552,     0,     0,     0,   553,
     554,   555,   556,   557,   558,   559,   560,    65,   561,   562,
     563,     0,     0,     0,     0,     0,     0,   564,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   565,   566,   567,     0,    15,     0,
       0,   568,   569,     0,     0,   519,   520,     0,     0,   570,
       0,   571,     0,   572,   573,   521,   522,   523,   524,   525,
       0,     0,     0,     0,     0,   526,     0,   527,     0,     0,
       0,   528,     0,     0,     0,     0,     0,     0,     0,   529,
       0,     0,     0,     0,     0,   530,     0,     0,   531,     0,
       0,   532,     0,     0,     0,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,     0,     0,   535,   536,
       0,   320,   321,   322,     0,   324,   325,   326,   327,   328,
     537,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,     0,   342,   343,   344,     0,     0,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   538,   539,   540,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   541,   542,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,   543,   544,   545,   546,   547,     0,
     548,     0,   549,   550,   551,   552,     0,     0,     0,   553,
     554,   555,   556,   557,   558,   559,   560,    65,   561,   562,
     563,     0,     0,     0,     0,     0,     0,   564,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   794,     0,   565,   566,   567,     0,    15,     0,
       0,   568,   569,     0,     0,   519,   520,     0,     0,   570,
       0,   571,     0,   572,   573,   521,   522,   523,   524,   525,
       0,     0,  1097,     0,     0,   526,     0,   527,     0,     0,
       0,   528,     0,     0,     0,     0,     0,     0,     0,   529,
       0,     0,     0,     0,     0,   530,     0,     0,   531,     0,
       0,   532,     0,     0,     0,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,     0,     0,   535,   536,
       0,   320,   321,   322,     0,   324,   325,   326,   327,   328,
     537,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,     0,   342,   343,   344,     0,     0,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   538,   539,   540,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   541,   542,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,   543,   544,   545,   546,   547,     0,
     548,     0,   549,   550,   551,   552,     0,     0,     0,   553,
     554,   555,   556,   557,   558,   559,   560,    65,   561,   562,
     563,     0,     0,     0,     0,     0,     0,   564,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   565,   566,   567,     0,    15,     0,
       0,   568,   569,     0,     0,   519,   520,     0,     0,   570,
       0,   571,     0,   572,   573,   521,   522,   523,   524,   525,
       0,     0,     0,     0,     0,   526,     0,   527,     0,     0,
       0,   528,     0,     0,     0,     0,     0,     0,     0,   529,
       0,     0,     0,     0,     0,   530,     0,     0,   531,     0,
       0,   532,     0,     0,     0,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,     0,     0,   535,   536,
       0,   320,   321,   322,     0,   324,   325,   326,   327,   328,
     537,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,     0,   342,   343,   344,     0,     0,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   538,   539,   540,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   541,   542,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,   543,   544,   545,   546,   547,     0,
     548,     0,   549,   550,   551,   552,     0,     0,     0,   553,
     554,   555,   556,   557,   558,   559,   560,    65,   561,   562,
     563,     0,     0,     0,     0,     0,     0,   564,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   565,   566,   567,     0,    15,     0,
       0,   568,   569,     0,     0,   519,   520,     0,     0,   570,
       0,   571,  1118,   572,   573,   521,   522,   523,   524,   525,
       0,     0,     0,     0,     0,   526,     0,   527,     0,     0,
       0,   528,     0,     0,     0,     0,     0,     0,     0,   529,
       0,     0,     0,     0,     0,   530,     0,     0,   531,     0,
       0,   532,     0,     0,     0,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,     0,     0,   535,   536,
       0,   320,   321,   322,     0,   324,   325,   326,   327,   328,
     537,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,     0,   342,   343,   344,     0,     0,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   538,   539,   540,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   541,   542,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,   543,   544,   545,   546,   547,     0,
     548,     0,   549,   550,   551,   552,     0,     0,     0,   553,
     554,   555,   556,   557,   558,   559,   560,    65,   561,   562,
     563,     0,     0,     0,     0,     0,     0,   564,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1260,     0,   565,   566,   567,     0,    15,     0,
       0,   568,   569,     0,     0,   519,   520,     0,     0,   570,
       0,   571,     0,   572,   573,   521,   522,   523,   524,   525,
       0,     0,     0,     0,     0,   526,     0,   527,     0,     0,
       0,   528,     0,     0,     0,     0,     0,     0,     0,   529,
       0,     0,     0,     0,     0,   530,     0,     0,   531,     0,
       0,   532,     0,     0,     0,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,     0,     0,   535,   536,
       0,   320,   321,   322,     0,   324,   325,   326,   327,   328,
     537,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,     0,   342,   343,   344,     0,     0,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   538,   539,   540,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   541,   542,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,   543,   544,   545,   546,   547,     0,
     548,     0,   549,   550,   551,   552,     0,     0,     0,   553,
     554,   555,   556,   557,   558,   559,   560,    65,   561,   562,
     563,     0,     0,     0,     0,     0,     0,   564,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   565,   566,   567,     0,    15,     0,
       0,   568,   569,     0,     0,   519,   520,     0,     0,   570,
       0,   571,  1533,   572,   573,   521,   522,   523,   524,   525,
       0,     0,     0,     0,     0,   526,     0,   527,     0,     0,
       0,   528,     0,     0,     0,     0,     0,     0,     0,   529,
       0,     0,     0,     0,     0,   530,     0,     0,   531,     0,
       0,   532,     0,     0,     0,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,     0,     0,   535,   536,
       0,   320,   321,   322,     0,   324,   325,   326,   327,   328,
     537,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,     0,   342,   343,   344,     0,     0,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   538,   539,   540,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   541,   542,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,   543,   544,   545,   546,   547,     0,
     548,     0,   549,   550,   551,   552,     0,     0,     0,   553,
     554,   555,   556,   557,   558,   559,   560,    65,   561,   562,
     563,     0,     0,     0,     0,     0,     0,   564,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   565,   566,   567,     0,    15,     0,
       0,   568,   569,     0,     0,   519,   520,     0,     0,  1542,
       0,   571,  1543,   572,   573,   521,   522,   523,   524,   525,
       0,     0,     0,     0,     0,   526,     0,   527,     0,     0,
       0,   528,     0,     0,     0,     0,     0,     0,     0,   529,
       0,     0,     0,     0,     0,   530,     0,     0,   531,     0,
       0,   532,     0,     0,     0,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,     0,     0,   535,   536,
       0,   320,   321,   322,     0,   324,   325,   326,   327,   328,
     537,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,     0,   342,   343,   344,     0,     0,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   538,   539,   540,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   541,   542,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,   543,   544,   545,   546,   547,     0,
     548,     0,   549,   550,   551,   552,     0,     0,     0,   553,
     554,   555,   556,   557,   558,   559,   560,    65,   561,   562,
     563,     0,     0,     0,     0,     0,     0,   564,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   565,   566,   567,     0,    15,     0,
       0,   568,   569,     0,     0,   519,   520,     0,     0,   570,
       0,   571,  1548,   572,   573,   521,   522,   523,   524,   525,
       0,     0,     0,     0,     0,   526,     0,   527,     0,     0,
       0,   528,     0,     0,     0,     0,     0,     0,     0,   529,
       0,     0,     0,     0,     0,   530,     0,     0,   531,     0,
       0,   532,     0,     0,     0,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,     0,     0,   535,   536,
       0,   320,   321,   322,     0,   324,   325,   326,   327,   328,
     537,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,     0,   342,   343,   344,     0,     0,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   538,   539,   540,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   541,   542,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,   543,   544,   545,   546,   547,     0,
     548,     0,   549,   550,   551,   552,     0,     0,     0,   553,
     554,   555,   556,   557,   558,   559,   560,    65,   561,   562,
     563,     0,     0,     0,     0,     0,     0,   564,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   565,   566,   567,     0,    15,     0,
       0,   568,   569,     0,     0,   519,   520,     0,     0,   570,
       0,   571,  1605,   572,   573,   521,   522,   523,   524,   525,
       0,     0,     0,     0,     0,   526,     0,   527,     0,     0,
       0,   528,     0,     0,     0,     0,     0,     0,     0,   529,
       0,     0,     0,     0,     0,   530,     0,     0,   531,     0,
       0,   532,     0,     0,     0,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,     0,     0,   535,   536,
       0,   320,   321,   322,     0,   324,   325,   326,   327,   328,
     537,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,     0,   342,   343,   344,     0,     0,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   538,   539,   540,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   541,   542,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,   543,   544,   545,   546,   547,     0,
     548,     0,   549,   550,   551,   552,     0,     0,     0,   553,
     554,   555,   556,   557,   558,   559,   560,    65,   561,   562,
     563,     0,     0,     0,     0,     0,     0,   564,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   565,   566,   567,     0,    15,     0,
       0,   568,   569,     0,     0,   519,   520,     0,     0,   570,
       0,   571,  1693,   572,   573,   521,   522,   523,   524,   525,
       0,     0,     0,     0,     0,   526,     0,   527,     0,     0,
       0,   528,     0,     0,     0,     0,     0,     0,     0,   529,
       0,     0,     0,     0,     0,   530,     0,     0,   531,     0,
       0,   532,     0,     0,     0,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,     0,     0,   535,   536,
       0,   320,   321,   322,     0,   324,   325,   326,   327,   328,
     537,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,     0,   342,   343,   344,     0,     0,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   538,   539,   540,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   541,   542,     0,     0,     0,     0,     0,     0,     0,
    1714,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,   543,   544,   545,   546,   547,     0,
     548,     0,   549,   550,   551,   552,     0,     0,     0,   553,
     554,   555,   556,   557,   558,   559,   560,    65,   561,   562,
     563,     0,     0,     0,     0,     0,     0,   564,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   565,   566,   567,     0,    15,     0,
       0,   568,   569,     0,     0,   519,   520,     0,     0,   570,
       0,   571,     0,   572,   573,   521,   522,   523,   524,   525,
       0,     0,     0,     0,     0,   526,     0,   527,     0,     0,
       0,   528,     0,     0,     0,     0,     0,     0,     0,   529,
       0,     0,     0,     0,     0,   530,     0,     0,   531,     0,
       0,   532,     0,     0,     0,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,     0,     0,   535,   536,
       0,   320,   321,   322,     0,   324,   325,   326,   327,   328,
     537,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,     0,   342,   343,   344,     0,     0,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   538,   539,   540,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   541,   542,     0,     0,     0,     0,     0,     0,     0,
    1774,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,   543,   544,   545,   546,   547,     0,
     548,     0,   549,   550,   551,   552,     0,     0,     0,   553,
     554,   555,   556,   557,   558,   559,   560,    65,   561,   562,
     563,     0,     0,     0,     0,     0,     0,   564,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   565,   566,   567,     0,    15,     0,
       0,   568,   569,     0,     0,   519,   520,     0,     0,   570,
       0,   571,     0,   572,   573,   521,   522,   523,   524,   525,
       0,     0,     0,     0,     0,   526,     0,   527,     0,     0,
       0,   528,     0,     0,     0,     0,     0,     0,     0,   529,
       0,     0,     0,     0,     0,   530,     0,     0,   531,     0,
       0,   532,     0,     0,     0,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,     0,     0,   535,   536,
       0,   320,   321,   322,     0,   324,   325,   326,   327,   328,
     537,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,     0,   342,   343,   344,     0,     0,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   538,   539,   540,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   541,   542,     0,     0,     0,     0,     0,     0,     0,
    1775,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,   543,   544,   545,   546,   547,     0,
     548,     0,   549,   550,   551,   552,     0,     0,     0,   553,
     554,   555,   556,   557,   558,   559,   560,    65,   561,   562,
     563,     0,     0,     0,     0,     0,     0,   564,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   565,   566,   567,     0,    15,     0,
       0,   568,   569,     0,     0,   519,   520,     0,     0,   570,
       0,   571,     0,   572,   573,   521,   522,   523,   524,   525,
       0,     0,     0,     0,     0,   526,     0,   527,     0,     0,
       0,   528,     0,     0,     0,     0,     0,     0,     0,   529,
       0,     0,     0,     0,     0,   530,     0,     0,   531,     0,
       0,   532,     0,     0,     0,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,     0,     0,   535,   536,
       0,   320,   321,   322,     0,   324,   325,   326,   327,   328,
     537,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,     0,   342,   343,   344,     0,     0,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   538,   539,   540,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   541,   542,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,   543,   544,   545,   546,   547,     0,
     548,     0,   549,   550,   551,   552,     0,     0,     0,   553,
     554,   555,   556,   557,   558,   559,   560,    65,   561,   562,
     563,     0,     0,     0,     0,     0,     0,   564,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   565,   566,   567,     0,    15,     0,
       0,   568,   569,     0,     0,   519,   520,     0,     0,   570,
       0,   571,     0,   572,   573,   521,   522,   523,   524,   525,
       0,     0,     0,     0,     0,   526,     0,   527,     0,     0,
       0,   528,     0,     0,     0,     0,     0,     0,     0,   529,
       0,     0,     0,     0,     0,   530,     0,     0,   531,     0,
       0,   532,     0,     0,     0,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   534,     0,     0,   535,   536,
       0,   320,   321,   322,     0,   324,   325,   326,   327,   328,
     537,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,     0,   342,   343,   344,     0,     0,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   538,   539,   540,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   541,   542,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,   543,   544,   545,   546,   547,     0,
     548,     0,   549,   550,   551,   552,     0,     0,     0,   553,
     554,   555,   556,   557,   558,   559,   560,    65,   561,   562,
     563,     0,   786,     0,     0,     0,     0,   564,   314,     0,
       0,     0,   820,   821,   315,     0,     0,     0,     0,     0,
     316,     0,     0,     0,   565,   566,   567,     0,    15,     0,
     317,   568,   569,     0,     0,     0,     0,     0,   318,  1519,
       0,   571,     0,   572,   573,     0,     0,     0,     0,     0,
       0,     0,     0,   319,     0,     0,     0,     0,     0,     0,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     822,   823,   824,   825,   826,     0,     0,   827,   828,   829,
     830,     0,   831,   832,   833,   834,     0,     0,     0,     0,
     835,     0,   836,   837,     0,    64,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   376,     0,
       0,     0,     0,     0,     0,   788,   820,   821,     0,     0,
       0,   314,     0,     0,     0,     0,    65,   315,     0,     0,
       0,     0,     0,   316,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   317,     0,     0,     0,     0,     0,     0,
       0,   318,   845,   846,   847,   848,   849,   850,   851,   852,
       0,     0,     0,     0,     0,     0,   319,     0,     0,   853,
     854,     0,   377,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   364,   365,   366,
     367,   368,   369,   370,   371,   372,   373,   374,   375,     0,
       0,     0,     0,     0,   822,   823,   824,   825,   826,     0,
       0,   827,     0,     0,   830,     0,   831,   832,   833,   834,
       0,     0,     0,     0,   835,     0,   836,   837,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   376,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   314,     0,     0,     0,     0,    65,
     315,     0,     0,     0,     0,     0,   316,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   317,   846,   847,   848,
     849,   850,   851,   852,   318,     0,     0,     0,     0,     0,
       0,     0,     0,   853,   854,     0,     0,     0,     0,   319,
       0,     0,     0,     0,     0,   377,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   376,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   314,
       0,     0,    65,     0,     0,   315,     0,     0,     0,     0,
       0,   316,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   317,     0,     0,     0,     0,     0,     0,     0,   318,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   319,     0,     0,     0,   377,     0,
     636,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   374,   375,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   376,
       0,     0,     0,     0,     0,     0,     0,     0,    13,     0,
       0,     0,   314,     0,     0,     0,     0,   639,   315,     0,
       0,     0,     0,     0,   316,     0,     0,     0,     0,    14,
       0,     0,     0,     0,   317,   640,     0,     0,     0,     0,
       0,     0,   318,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   319,     0,     0,
       0,     0,     0,   377,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   376,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   314,     0,     0,   820,   821,
      65,   315,     0,     0,     0,     0,     0,   316,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   317,     0,     0,
       0,     0,     0,     0,     0,   318,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     319,     0,     0,     0,     0,     0,   377,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   368,   369,   370,   371,   372,
     373,   374,   375,     0,     0,     0,     0,     0,  1124,  1125,
    1126,  1127,  1128,  1129,  1130,  1131,   822,   823,   824,   825,
     826,  1132,  1133,   827,   828,   829,   830,  1134,   831,   832,
     833,   834,   820,   821,     0,     0,   835,   963,   836,   837,
    1135,  1136,    64,     0,   838,   839,   840,  1137,  1138,  1139,
     841,     0,     0,     0,     0,   376,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    13,     0,   820,   821,     0,
       0,     0,     0,   639,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    14,     0,     0,     0,
       0,     0,     0,  1140,   842,     0,   843,   844,   845,   846,
     847,   848,   849,   850,   851,   852,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   853,   854,     0,     0,   377,
     572,   929,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1124,  1125,  1126,  1127,  1128,  1129,  1130,  1131,
     822,   823,   824,   825,   826,  1132,  1133,   827,   828,   829,
     830,  1134,   831,   832,   833,   834,  -442,     0,     0,     0,
     835,   963,   836,   837,  1135,  1136,   820,   821,   838,   839,
     840,  1137,  1138,  1139,   841,   822,   823,   824,   825,   826,
       0,     0,   827,   828,   829,   830,     0,   831,   832,   833,
     834,     0,     0,     0,     0,   835,     0,   836,   837,     0,
       0,   820,   821,   838,   839,   840,     0,     0,     0,   841,
       0,     0,     0,     0,     0,     0,     0,  1140,   842,     0,
     843,   844,   845,   846,   847,   848,   849,   850,   851,   852,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   853,
     854,     0,     0,     0,   572,   929,     0,     0,     0,     0,
       0,     0,     0,   842,     0,   843,   844,   845,   846,   847,
     848,   849,   850,   851,   852,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   853,   854,     0,     0,   868,     0,
       0,     0,     0,     0,   822,   823,   824,   825,   826,     0,
       0,   827,   828,   829,   830,     0,   831,   832,   833,   834,
       0,     0,     0,     0,   835,     0,   836,   837,   820,   821,
       0,     0,   838,   839,   840,     0,     0,     0,   841,   822,
     823,   824,   825,   826,     0,     0,   827,   828,   829,   830,
       0,   831,   832,   833,   834,     0,     0,     0,     0,   835,
       0,   836,   837,   820,   821,     0,     0,   838,   839,   840,
       0,     0,     0,   841,     0,     0,     0,     0,     0,     0,
       0,     0,   842,     0,   843,   844,   845,   846,   847,   848,
     849,   850,   851,   852,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   853,   854,     0,     0,   884,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   842,     0,   843,
     844,   845,   846,   847,   848,   849,   850,   851,   852,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   853,   854,
       0,     0,  1170,     0,     0,     0,   822,   823,   824,   825,
     826,     0,     0,   827,   828,   829,   830,     0,   831,   832,
     833,   834,     0,     0,     0,     0,   835,     0,   836,   837,
     820,   821,     0,     0,   838,   839,   840,     0,     0,     0,
     841,   822,   823,   824,   825,   826,     0,     0,   827,   828,
     829,   830,     0,   831,   832,   833,   834,     0,     0,     0,
       0,   835,     0,   836,   837,   820,   821,     0,     0,   838,
     839,   840,     0,     0,     0,   841,     0,     0,     0,     0,
       0,     0,     0,     0,   842,     0,   843,   844,   845,   846,
     847,   848,   849,   850,   851,   852,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   853,   854,     0,     0,  1273,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   842,
       0,   843,   844,   845,   846,   847,   848,   849,   850,   851,
     852,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     853,   854,     0,     0,  1276,     0,     0,     0,   822,   823,
     824,   825,   826,     0,     0,   827,   828,   829,   830,     0,
     831,   832,   833,   834,     0,     0,     0,     0,   835,     0,
     836,   837,   820,   821,     0,     0,   838,   839,   840,     0,
       0,     0,   841,   822,   823,   824,   825,   826,     0,     0,
     827,   828,   829,   830,     0,   831,   832,   833,   834,     0,
       0,     0,     0,   835,     0,   836,   837,   820,   821,     0,
       0,   838,   839,   840,     0,     0,     0,   841,     0,     0,
       0,     0,     0,     0,     0,     0,   842,     0,   843,   844,
     845,   846,   847,   848,   849,   850,   851,   852,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   853,   854,     0,
       0,  1278,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   842,     0,   843,   844,   845,   846,   847,   848,   849,
     850,   851,   852,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   853,   854,     0,     0,  1286,     0,     0,     0,
     822,   823,   824,   825,   826,     0,     0,   827,   828,   829,
     830,     0,   831,   832,   833,   834,     0,     0,     0,     0,
     835,     0,   836,   837,   820,   821,     0,     0,   838,   839,
     840,     0,     0,     0,   841,   822,   823,   824,   825,   826,
       0,     0,   827,   828,   829,   830,     0,   831,   832,   833,
     834,     0,     0,     0,     0,   835,     0,   836,   837,   820,
     821,     0,     0,   838,   839,   840,     0,     0,     0,   841,
       0,     0,     0,     0,     0,     0,     0,     0,   842,     0,
     843,   844,   845,   846,   847,   848,   849,   850,   851,   852,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   853,
     854,     0,     0,  1287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   842,     0,   843,   844,   845,   846,   847,
     848,   849,   850,   851,   852,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   853,   854,     0,     0,  1288,     0,
       0,     0,   822,   823,   824,   825,   826,     0,     0,   827,
     828,   829,   830,     0,   831,   832,   833,   834,     0,     0,
       0,     0,   835,     0,   836,   837,   820,   821,     0,     0,
     838,   839,   840,     0,     0,     0,   841,   822,   823,   824,
     825,   826,     0,     0,   827,   828,   829,   830,     0,   831,
     832,   833,   834,     0,     0,     0,     0,   835,     0,   836,
     837,   820,   821,     0,     0,   838,   839,   840,     0,     0,
       0,   841,     0,     0,     0,     0,     0,     0,     0,     0,
     842,     0,   843,   844,   845,   846,   847,   848,   849,   850,
     851,   852,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   853,   854,     0,     0,  1289,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   842,     0,   843,   844,   845,
     846,   847,   848,   849,   850,   851,   852,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   853,   854,     0,     0,
    1290,     0,     0,     0,   822,   823,   824,   825,   826,     0,
       0,   827,   828,   829,   830,     0,   831,   832,   833,   834,
       0,     0,     0,     0,   835,     0,   836,   837,   820,   821,
       0,     0,   838,   839,   840,     0,     0,     0,   841,   822,
     823,   824,   825,   826,     0,     0,   827,   828,   829,   830,
       0,   831,   832,   833,   834,     0,     0,     0,     0,   835,
       0,   836,   837,   820,   821,     0,     0,   838,   839,   840,
       0,     0,     0,   841,     0,     0,     0,     0,     0,     0,
       0,     0,   842,     0,   843,   844,   845,   846,   847,   848,
     849,   850,   851,   852,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   853,   854,     0,     0,  1291,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   842,     0,   843,
     844,   845,   846,   847,   848,   849,   850,   851,   852,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   853,   854,
       0,     0,  1378,     0,     0,     0,   822,   823,   824,   825,
     826,     0,     0,   827,   828,   829,   830,     0,   831,   832,
     833,   834,     0,     0,     0,     0,   835,     0,   836,   837,
     820,   821,     0,     0,   838,   839,   840,     0,     0,     0,
     841,   822,   823,   824,   825,   826,     0,     0,   827,   828,
     829,   830,     0,   831,   832,   833,   834,     0,     0,     0,
       0,   835,     0,   836,   837,   820,   821,     0,     0,   838,
     839,   840,     0,     0,     0,   841,     0,     0,     0,     0,
       0,     0,     0,     0,   842,     0,   843,   844,   845,   846,
     847,   848,   849,   850,   851,   852,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   853,   854,     0,     0,  1461,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   842,
       0,   843,   844,   845,   846,   847,   848,   849,   850,   851,
     852,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     853,   854,     0,     0,  1464,     0,     0,     0,   822,   823,
     824,   825,   826,     0,     0,   827,   828,   829,   830,     0,
     831,   832,   833,   834,     0,     0,     0,     0,   835,     0,
     836,   837,   820,   821,     0,     0,   838,   839,   840,     0,
       0,     0,   841,   822,   823,   824,   825,   826,     0,     0,
     827,   828,   829,   830,     0,   831,   832,   833,   834,     0,
       0,     0,     0,   835,     0,   836,   837,   820,   821,     0,
       0,   838,   839,   840,     0,     0,     0,   841,     0,     0,
       0,     0,     0,     0,     0,     0,   842,     0,   843,   844,
     845,   846,   847,   848,   849,   850,   851,   852,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   853,   854,     0,
       0,  1509,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   842,     0,   843,   844,   845,   846,   847,   848,   849,
     850,   851,   852,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   853,   854,     0,     0,  1559,     0,     0,     0,
     822,   823,   824,   825,   826,     0,     0,   827,   828,   829,
     830,     0,   831,   832,   833,   834,     0,     0,     0,     0,
     835,     0,   836,   837,   820,   821,     0,     0,   838,   839,
     840,     0,     0,     0,   841,   822,   823,   824,   825,   826,
       0,     0,   827,   828,   829,   830,     0,   831,   832,   833,
     834,     0,     0,     0,     0,   835,     0,   836,   837,   820,
     821,     0,     0,   838,   839,   840,     0,     0,     0,   841,
       0,     0,     0,     0,     0,     0,     0,     0,   842,     0,
     843,   844,   845,   846,   847,   848,   849,   850,   851,   852,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   853,
     854,     0,     0,  1626,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   842,     0,   843,   844,   845,   846,   847,
     848,   849,   850,   851,   852,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   853,   854,     0,     0,  1627,     0,
       0,     0,   822,   823,   824,   825,   826,     0,     0,   827,
     828,   829,   830,     0,   831,   832,   833,   834,     0,     0,
       0,     0,   835,     0,   836,   837,   820,   821,     0,     0,
     838,   839,   840,     0,     0,     0,   841,   822,   823,   824,
     825,   826,     0,     0,   827,   828,   829,   830,     0,   831,
     832,   833,   834,     0,     0,     0,     0,   835,     0,   836,
     837,   820,   821,     0,     0,   838,   839,   840,     0,     0,
       0,   841,     0,     0,     0,     0,     0,     0,     0,     0,
     842,     0,   843,   844,   845,   846,   847,   848,   849,   850,
     851,   852,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   853,   854,     0,     0,  1640,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   842,     0,   843,   844,   845,
     846,   847,   848,   849,   850,   851,   852,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   853,   854,     0,     0,
    1642,     0,     0,     0,   822,   823,   824,   825,   826,     0,
       0,   827,   828,   829,   830,     0,   831,   832,   833,   834,
       0,     0,     0,     0,   835,     0,   836,   837,   820,   821,
       0,     0,   838,   839,   840,     0,     0,     0,   841,   822,
     823,   824,   825,   826,     0,     0,   827,   828,   829,   830,
       0,   831,   832,   833,   834,     0,     0,     0,     0,   835,
       0,   836,   837,   820,   821,     0,     0,   838,   839,   840,
       0,     0,     0,   841,     0,     0,     0,     0,     0,     0,
       0,     0,   842,     0,   843,   844,   845,   846,   847,   848,
     849,   850,   851,   852,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   853,   854,     0,     0,  1644,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   842,     0,   843,
     844,   845,   846,   847,   848,   849,   850,   851,   852,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   853,   854,
       0,     0,  1648,     0,     0,     0,   822,   823,   824,   825,
     826,     0,     0,   827,   828,   829,   830,     0,   831,   832,
     833,   834,     0,     0,     0,     0,   835,     0,   836,   837,
     820,   821,     0,     0,   838,   839,   840,     0,     0,     0,
     841,   822,   823,   824,   825,   826,     0,     0,   827,   828,
     829,   830,     0,   831,   832,   833,   834,     0,     0,     0,
       0,   835,     0,   836,   837,   820,   821,     0,     0,   838,
     839,   840,     0,     0,     0,   841,     0,     0,     0,     0,
       0,     0,     0,     0,   842,     0,   843,   844,   845,   846,
     847,   848,   849,   850,   851,   852,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   853,   854,     0,     0,  1709,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   842,
       0,   843,   844,   845,   846,   847,   848,   849,   850,   851,
     852,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     853,   854,     0,     0,  1719,     0,     0,     0,   822,   823,
     824,   825,   826,     0,     0,   827,   828,   829,   830,     0,
     831,   832,   833,   834,     0,     0,     0,     0,   835,     0,
     836,   837,   820,   821,     0,     0,   838,   839,   840,     0,
       0,     0,   841,   822,   823,   824,   825,   826,     0,     0,
     827,   828,   829,   830,     0,   831,   832,   833,   834,     0,
       0,     0,     0,   835,     0,   836,   837,   820,   821,     0,
       0,   838,   839,   840,     0,     0,     0,   841,     0,     0,
       0,     0,     0,     0,     0,     0,   842,     0,   843,   844,
     845,   846,   847,   848,   849,   850,   851,   852,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   853,   854,     0,
       0,  1720,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   842,     0,   843,   844,   845,   846,   847,   848,   849,
     850,   851,   852,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   853,   854,     0,     0,  1722,     0,     0,     0,
     822,   823,   824,   825,   826,     0,     0,   827,   828,   829,
     830,     0,   831,   832,   833,   834,     0,     0,     0,     0,
     835,     0,   836,   837,   820,   821,     0,     0,   838,   839,
     840,     0,     0,     0,   841,   822,   823,   824,   825,   826,
       0,     0,   827,   828,   829,   830,     0,   831,   832,   833,
     834,     0,     0,     0,     0,   835,     0,   836,   837,   820,
     821,     0,     0,   838,   839,   840,     0,     0,     0,   841,
       0,     0,     0,     0,     0,     0,     0,     0,   842,     0,
     843,   844,   845,   846,   847,   848,   849,   850,   851,   852,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   853,
     854,     0,     0,  1723,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   842,     0,   843,   844,   845,   846,   847,
     848,   849,   850,   851,   852,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   853,   854,     0,     0,  1743,     0,
       0,     0,   822,   823,   824,   825,   826,     0,     0,   827,
     828,   829,   830,     0,   831,   832,   833,   834,     0,     0,
       0,     0,   835,     0,   836,   837,   820,   821,     0,     0,
     838,   839,   840,     0,     0,     0,   841,   822,   823,   824,
     825,   826,     0,     0,   827,   828,   829,   830,     0,   831,
     832,   833,   834,     0,     0,     0,     0,   835,     0,   836,
     837,   820,   821,     0,     0,   838,   839,   840,     0,     0,
       0,   841,     0,     0,     0,     0,     0,     0,     0,     0,
     842,     0,   843,   844,   845,   846,   847,   848,   849,   850,
     851,   852,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   853,   854,     0,     0,  1746,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   842,     0,   843,   844,   845,
     846,   847,   848,   849,   850,   851,   852,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   853,   854,     0,     0,
    1755,     0,     0,     0,   822,   823,   824,   825,   826,     0,
       0,   827,   828,   829,   830,     0,   831,   832,   833,   834,
       0,     0,     0,     0,   835,     0,   836,   837,   820,   821,
       0,     0,   838,   839,   840,     0,     0,     0,   841,   822,
     823,   824,   825,   826,     0,     0,   827,   828,   829,   830,
       0,   831,   832,   833,   834,     0,     0,     0,     0,   835,
       0,   836,   837,   820,   821,     0,     0,   838,   839,   840,
       0,     0,     0,   841,     0,     0,     0,     0,     0,     0,
       0,     0,   842,     0,   843,   844,   845,   846,   847,   848,
     849,   850,   851,   852,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   853,   854,     0,     0,  1826,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   842,     0,   843,
     844,   845,   846,   847,   848,   849,   850,   851,   852,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   853,   854,
       0,     0,  1827,     0,     0,     0,   822,   823,   824,   825,
     826,     0,     0,   827,   828,   829,   830,     0,   831,   832,
     833,   834,     0,     0,     0,     0,   835,     0,   836,   837,
     820,   821,     0,     0,   838,   839,   840,     0,     0,     0,
     841,   822,   823,   824,   825,   826,     0,     0,   827,   828,
     829,   830,     0,   831,   832,   833,   834,     0,     0,     0,
       0,   835,     0,   836,   837,   820,   821,     0,     0,   838,
     839,   840,     0,     0,     0,   841,     0,     0,     0,     0,
       0,     0,     0,     0,   842,     0,   843,   844,   845,   846,
     847,   848,   849,   850,   851,   852,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   853,   854,   888,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   842,
       0,   843,   844,   845,   846,   847,   848,   849,   850,   851,
     852,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     853,   854,  1164,     0,     0,     0,     0,     0,   822,   823,
     824,   825,   826,     0,     0,   827,   828,   829,   830,     0,
     831,   832,   833,   834,     0,     0,     0,     0,   835,     0,
     836,   837,   820,   821,     0,     0,   838,   839,   840,     0,
       0,     0,   841,   822,   823,   824,   825,   826,     0,     0,
     827,   828,   829,   830,     0,   831,   832,   833,   834,     0,
       0,     0,     0,   835,     0,   836,   837,   820,   821,     0,
       0,   838,   839,   840,     0,     0,     0,   841,     0,     0,
       0,     0,     0,     0,     0,     0,   842,     0,   843,   844,
     845,   846,   847,   848,   849,   850,   851,   852,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   853,   854,  1360,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   842,     0,   843,   844,   845,   846,   847,   848,   849,
     850,   851,   852,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   853,   854,  1376,     0,     0,     0,     0,     0,
     822,   823,   824,   825,   826,     0,     0,   827,   828,   829,
     830,     0,   831,   832,   833,   834,     0,     0,     0,     0,
     835,     0,   836,   837,     0,     0,     0,     0,   838,   839,
     840,     0,     0,     0,   841,   822,   823,   824,   825,   826,
       0,     0,   827,   828,   829,   830,     0,   831,   832,   833,
     834,   392,   393,     0,     0,   835,     0,   836,   837,     0,
       0,     0,     0,   838,   839,   840,     0,     0,   394,   841,
       0,     0,     0,     0,     0,     0,     0,     0,   842,     0,
     843,   844,   845,   846,   847,   848,   849,   850,   851,   852,
       0,     0,     0,     0,   820,   821,     0,     0,     0,   853,
     854,  1540,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   842,     0,   843,   844,   845,   846,   847,
     848,   849,   850,   851,   852,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   853,   854,  1546,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   395,   396,   397,   398,   399,   400,   401,   402,   403,
     404,   405,   406,   407,   408,   409,   410,   411,   412,     0,
       0,   413,   414,   415,     0,     0,     0,     0,   820,   821,
     416,   417,   418,   419,   420,     0,     0,   421,   422,   423,
     424,   425,   426,   427,     0,     0,     0,     0,     0,     0,
       0,     0,   822,   823,   824,   825,   826,     0,     0,   827,
     828,   829,   830,     0,   831,   832,   833,   834,     0,     0,
       0,     0,   835,     0,   836,   837,     0,     0,     0,     0,
     838,   839,   840,     0,     0,     0,   841,   428,     0,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,    52,
       0,   439,   440,     0,     0,     0,     0,     0,   441,   442,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,     0,   820,   821,     0,     0,     0,     0,     0,     0,
     842,     0,   843,   844,   845,   846,   847,   848,   849,   850,
     851,   852,     0,     0,     0,     0,   822,   823,   824,   825,
     826,   853,   854,   827,   828,   829,   830,     0,   831,   832,
     833,   834,   820,   821,     0,     0,   835,     0,   836,   837,
       0,     0,     0,     0,   838,   839,   840,     0,     0,     0,
     841,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    13,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    14,     0,     0,     0,
       0,     0,     0,     0,   842,     0,   843,   844,   845,   846,
     847,   848,   849,   850,   851,   852,     0,     0,     0,     0,
     822,   823,   824,   825,   826,   853,   854,   827,   828,   829,
     830,     0,   831,   832,   833,   834,   820,   821,     0,     0,
     835,     0,   836,   837,     0,     0,  1028,     0,   838,   839,
     840,     0,     0,     0,   841,     0,     0,     0,     0,     0,
     822,   823,   824,   825,   826,     0,     0,   827,   828,   829,
     830,     0,   831,   832,   833,   834,   820,   821,     0,     0,
     835,     0,   836,   837,     0,     0,  1297,     0,   838,   839,
     840,     0,     0,     0,   841,     0,     0,     0,   842,     0,
     843,   844,   845,   846,   847,   848,   849,   850,   851,   852,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   853,
     854,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     820,   821,     0,     0,     0,     0,     0,     0,   842,     0,
     843,   844,   845,   846,   847,   848,   849,   850,   851,   852,
       0,     0,     0,     0,   822,   823,   824,   825,   826,   853,
     854,   827,   828,   829,   830,     0,   831,   832,   833,   834,
     820,   821,     0,     0,   835,     0,   836,   837,     0,     0,
       0,     0,   838,   839,   840,     0,     0,     0,   841,     0,
       0,     0,     0,     0,   822,   823,   824,   825,   826,     0,
       0,   827,   828,   829,   830,     0,   831,   832,   833,   834,
       0,     0,     0,     0,   835,     0,   836,   837,     0,     0,
       0,     0,   838,   839,   840,     0,     0,     0,   841,     0,
       0,     0,   842,  1365,   843,   844,   845,   846,   847,   848,
     849,   850,   851,   852,     0,     0,     0,     0,   822,   823,
     824,   825,   826,   853,   854,   827,   828,   829,   830,     0,
     831,   832,   833,   834,   820,   821,     0,     0,   835,     0,
     836,   837,   842,  1504,   843,   844,   845,   846,   847,   848,
     849,   850,   851,   852,     0,     0,     0,     0,   822,   823,
     824,   825,   826,   853,   854,   827,   828,   829,   830,     0,
     831,   832,   833,   834,   820,   821,     0,     0,   835,     0,
     836,   837,     0,     0,     0,     0,   838,   839,   840,     0,
       0,     0,   841,     0,     0,     0,     0,     0,     0,   844,
     845,   846,   847,   848,   849,   850,   851,   852,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   853,   854,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1756,     0,
       0,     0,     0,     0,     0,     0,   842,     0,   843,   844,
     845,   846,   847,   848,   849,   850,   851,   852,     0,     0,
       0,     0,   822,   823,   824,   825,   826,   853,   854,   827,
     828,   829,   830,     0,   831,   832,   833,   834,   820,   821,
       0,     0,   835,     0,   836,   837,     0,     0,     0,     0,
     838,   839,   840,     0,     0,     0,   841,     0,     0,     0,
       0,     0,   822,   823,   824,   825,   826,     0,     0,   827,
     828,   829,   830,     0,   831,   832,   833,   834,     0,     0,
       0,     0,   835,     0,   836,   837,     0,     0,     0,     0,
     838,   839,   840,     0,     0,     0,  -884,     0,     0,     0,
     842,     0,   843,   844,   845,   846,   847,   848,   849,   850,
     851,   852,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   853,   854,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     842,     0,   843,   844,   845,   846,   847,   848,   849,   850,
     851,   852,     0,     0,     0,     0,   822,   823,   824,   825,
     826,   853,   854,   827,   828,   829,   830,     0,   831,   832,
     833,   834,     0,     0,     0,     0,   835,     0,   836,   837,
       0,     0,     0,     0,   838,   839,   840,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1069,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   842,     0,   843,   844,   845,   846,
     847,   848,   849,   850,   851,   852,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   853,   854,   320,   321,   322,
    1073,   324,   325,   326,   327,   328,   537,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,     0,   342,
     343,   344,     0,     0,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   368,   369,   370,   371,   372,
     373,     0,   320,   321,   322,     0,   324,   325,   326,   327,
     328,   537,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,     0,   342,   343,   344,     0,     0,   347,
     348,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,   365,   366,   367,
     368,   369,   370,   371,   372,   373,     0,  1070,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1071,     0,     0,     0,  1361,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1074,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1075,   320,
     321,   322,     0,   324,   325,   326,   327,   328,   537,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
       0,   342,   343,   344,     0,     0,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   320,   321,   322,     0,   324,   325,   326,
     327,   328,   537,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,     0,   342,   343,   344,     0,     0,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   364,   365,   366,
     367,   368,   369,   370,   371,   372,   373,     0,     0,  1362,
       0,     0,     0,     0,     0,     0,     0,     0,   235,     0,
       0,     0,     0,     0,     0,  1363,     0,     0,     0,     0,
       0,     0,     0,     0,  1109,  1110,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   236,     0,   237,     0,   238,   239,
     240,   241,   242,  1111,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,  1112,
       0,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   281,   282,   283,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1113,  1114,     0,     0,   284,   285,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     286
};

static const yytype_int16 yycheck[] =
{
       1,   552,   625,   288,   606,   215,   602,   510,   229,   510,
     616,   764,   618,   724,   620,    15,    16,   904,    19,   875,
     787,   877,   789,   879,     7,   493,   510,   492,   516,   461,
     809,   181,   229,   789,   813,   814,   706,   707,  1002,   455,
    1300,    87,  1004,    20,  1193,  1416,  1019,     5,    20,   492,
     493,     8,  1025,   818,     4,   786,    22,   788,   490,   790,
      33,    57,    53,    19,    20,    53,   797,    63,    15,    16,
      70,    71,    72,   113,   150,   806,   149,   149,    20,    33,
      15,    16,    46,    40,    20,   149,  1668,  1576,   183,   691,
     189,    57,   181,    20,   582,     7,   215,    62,   166,   100,
      34,   102,   704,   606,    20,   149,    20,   886,   197,  1710,
     167,   168,   169,   149,   114,   115,   116,   117,   550,   551,
      57,   240,   606,   150,   149,    57,    63,   152,   153,    63,
       0,    63,   215,   160,   150,   216,  1285,     7,    50,   166,
     216,   152,   153,   219,   160,   218,   218,   181,   131,   244,
     218,   223,   241,   303,   218,   156,    33,   240,  1740,   223,
      30,   155,    32,   197,    34,   129,  1767,   125,   126,   241,
      40,   217,     5,     6,   218,  1664,   216,   241,   909,   223,
      50,   188,   218,    60,    61,   242,    56,   223,   691,   216,
     218,   187,    25,   181,   188,   178,   130,   241,    31,   642,
     216,   704,   209,   166,   647,   241,   215,   691,   242,   197,
      80,   990,   208,   238,   239,   209,   981,   218,   209,    47,
     704,  1000,   205,   206,  1003,   159,  1486,   238,   239,   229,
     187,   188,   166,   183,   197,    68,    69,   244,   215,    67,
     461,   197,   189,   215,  1615,   215,   175,   194,   221,   232,
     197,   208,   209,   200,   189,   125,   126,   197,   860,   194,
     306,   227,   197,   197,   222,   200,   150,   221,   197,   490,
     147,   492,   238,   215,   151,   227,   160,   224,   241,   215,
     950,  1430,   239,   241,   227,   239,   507,   508,   215,   224,
    1439,   245,   125,   126,   241,  1062,  1061,   244,   232,   215,
     510,   215,  1266,   243,   514,  1267,   516,   241,   910,   244,
     507,   508,  1043,   197,   297,    55,   178,   300,   188,   175,
     175,   183,   197,  1296,   926,   197,   150,   183,   186,   550,
     551,   763,   216,   166,   243,   244,   160,   221,   186,   209,
     217,   197,   197,  1013,   221,   777,   223,   224,  1608,   207,
     183,   184,   185,     5,     6,   178,     8,   860,   228,   207,
     183,   101,   239,   150,   197,  1698,   241,  1700,   245,   239,
     218,  1704,  1705,    57,   207,   175,   860,   377,   215,    63,
     242,   571,   244,   241,    36,   241,   607,   608,   609,   222,
     611,   612,   216,   150,   615,   186,   617,   197,   619,   150,
     621,   209,   166,   160,   212,   242,   239,   910,   693,   160,
     607,   608,   609,   215,   611,   612,   207,   702,   615,   175,
     617,   244,   619,   926,   867,   215,   910,  1601,   215,   216,
    1763,   218,   240,   197,   221,   645,  1742,   186,   240,   649,
     883,   197,   926,   931,   150,   218,  1333,  1753,   221,   450,
     240,   451,   243,   244,   160,   166,    21,    22,   207,   216,
     218,   461,  1068,   221,   175,   216,   215,   215,   952,   953,
     954,   955,   956,   957,   958,   959,   960,   961,   962,  1030,
     964,   965,   966,   967,   968,   969,   197,  1793,  1794,  1040,
     490,  1256,   492,   925,   242,    47,   215,  1353,   188,  1673,
    1674,   691,   215,   504,   505,   215,   222,   507,   508,   510,
     216,   512,  1485,   201,   202,   516,   215,  1691,  1692,   209,
     215,   240,  1495,   523,   524,   241,   216,   240,   173,  1296,
     240,    33,  1299,   949,  1204,  1205,  1206,  1424,    90,    91,
    1296,    93,   763,   242,  1311,   240,   978,   979,   991,   165,
     550,   551,   197,   198,   199,  1311,   777,   989,    60,    61,
     187,   188,   994,   995,   181,   997,   167,   999,   169,  1001,
     186,   173,   762,  1747,  1748,   215,   141,   142,   212,   213,
     197,   208,   772,   228,   149,   775,   151,   152,   153,   154,
     209,   207,   186,   238,   159,   197,   198,   199,   748,   820,
     821,   602,   242,   188,   223,   166,   240,   607,   608,   609,
     188,   611,   612,   207,   835,   615,   215,   617,   188,   619,
     166,   621,  1509,   908,   209,   227,   228,   215,   818,   175,
     215,   209,   853,   918,   180,   130,   238,   638,   223,   209,
     215,   219,   215,   242,   188,   147,   216,   189,   150,   151,
     935,   197,   194,   938,   242,   241,   215,   215,   160,   188,
    1516,   226,   227,   228,   885,   209,   856,   242,   188,   242,
     215,   215,   893,   238,   239,   896,   222,    21,    22,   223,
     209,   222,   892,   242,   242,   188,   188,   215,   885,   209,
     218,  1159,  1157,   221,   215,   215,   893,   242,   699,   896,
     241,    70,    71,    72,   925,   215,   209,   209,   709,   710,
    1198,  1567,   215,   130,   216,   217,   219,  1160,  1471,   221,
     910,   242,   224,   222,   181,   726,   727,   728,   729,   188,
     731,  1337,   242,    21,    22,   736,   189,   239,   739,   130,
     197,   187,   241,   245,   744,   114,   115,   116,   117,   238,
     209,   188,  1519,   181,   181,   755,   215,   978,   979,   980,
     197,   971,   208,   763,   985,   188,   188,   218,   989,   197,
     197,   222,   209,   994,   995,  1542,   997,   777,   999,   208,
    1001,  1002,   211,   980,   197,   214,   209,   209,   985,    21,
      22,   981,   215,   215,   197,   139,   140,   141,   142,   189,
     223,   223,   201,   188,   194,   149,  1662,   151,   152,   153,
     154,   188,  1033,  1034,   197,   159,   816,   161,   162,   181,
     197,   188,  1709,   188,   209,  1431,  1016,   222,   222,   197,
     215,   197,   209,  1265,  1024,   197,  1033,  1034,   223,  1271,
     197,  1284,   209,    57,   209,    57,   241,   241,   215,    63,
     215,    63,   188,   141,   142,  1566,   223,    12,   223,  1033,
    1034,   149,   863,   151,   152,   153,   154,   197,    23,    24,
     219,   159,   244,   209,   223,    57,  1097,  1320,  1321,   215,
     224,   225,   226,   227,   228,   885,   219,   223,   201,   890,
     223,   188,   188,   893,   238,   239,   896,  1182,   232,    57,
      57,   197,   209,   167,  1336,    63,    63,   188,   215,   141,
     142,   188,   209,   209,   167,   168,   169,   149,   215,   215,
     152,   153,   154,   209,  1145,   925,   223,   159,   209,   215,
     129,  1684,   209,   209,   215,  1438,  1157,  1438,   215,   215,
      75,   209,   223,   209,    79,   244,   223,   215,  1145,   215,
     238,   239,   197,  1174,  1438,    21,    22,   940,    93,    94,
     197,   198,   199,    98,    99,   100,   101,  1573,    79,   208,
     209,   972,   211,  1752,  1741,  1742,   977,  1174,   978,   979,
     980,   216,  1749,    94,    66,   985,  1753,  1754,    99,   989,
     101,   218,   209,   197,   994,   995,   219,   997,   215,   999,
     223,  1001,  1002,   201,   202,   203,   238,   239,  1787,   219,
      33,   219,   219,   223,   197,   223,   223,  1460,  1785,   219,
      33,    96,    97,   223,   219,  1468,  1793,  1794,   223,    35,
      35,   219,   219,  1033,  1034,   223,   223,    60,    61,   219,
     201,   202,   203,   223,  1265,  1266,    43,    60,    61,   241,
    1271,   131,   132,   133,   134,   135,   136,   137,   138,  1682,
     241,   197,  1613,   201,   202,   203,   204,  1618,   218,   241,
    1666,   243,   244,    33,   222,   141,   142,  1844,   222,   222,
     160,   241,   222,   149,   197,   151,   152,   153,   154,   222,
     170,   171,   172,   159,   208,   209,   210,  1529,   222,   241,
      60,    61,  1292,    33,   680,   681,   682,    10,    11,    12,
     222,   241,   222,   222,   222,  1336,   219,   240,   197,   197,
    1310,    22,  1123,   240,   147,   240,  1316,   197,   151,   197,
      60,    61,   222,  1323,   147,  1325,  1738,   239,   151,   197,
    1736,  1737,   241,   244,   242,  1145,   222,  1432,  1149,   222,
     222,   222,   241,   222,   241,   222,   241,  1157,   224,   225,
     226,   227,   228,   222,   241,   188,   222,   222,  1358,    43,
     222,   222,   238,   239,  1174,   239,   241,  1779,  1780,   241,
    1370,   241,  1778,   223,   241,  1375,   209,   147,   241,   197,
     240,   151,   215,   241,   217,   241,  1417,   242,   221,   218,
     223,   224,   197,   189,   217,   222,    10,  1428,   221,    37,
     223,   224,  1814,    66,     8,   241,   239,   147,   458,   222,
    1417,   151,   245,   222,   222,  1446,   239,   467,  1438,   208,
     215,  1428,   245,  1665,    13,  1738,  1426,   477,   240,   215,
     241,   215,   242,  1528,   197,  1246,   197,   487,   155,  1446,
     223,   197,   215,   241,  1738,    43,   215,   217,    14,   197,
     216,   221,   189,   223,   224,  1265,  1266,   244,   218,   197,
     510,  1271,    21,    22,   197,    67,  1779,  1780,  1563,   239,
     242,   215,   215,   208,  1569,   245,   241,   217,   242,   241,
     222,   221,   241,  1483,   224,  1779,  1780,   242,   241,   241,
     222,   541,   542,  1735,   241,   241,   241,     1,  1529,   239,
    1531,  1814,   552,   223,   223,   245,   242,   241,  1508,   223,
     241,   197,   216,   216,   564,   565,   566,   567,   568,   569,
    1814,   197,   197,   241,  1531,   241,  1336,   242,   131,   132,
     133,   134,   135,   136,   137,   138,   242,   953,   954,   955,
     956,   957,   958,   959,   960,   961,   962,   150,   964,   965,
     966,   967,   968,   969,   242,   242,   606,   160,   240,   242,
     240,   240,   197,  1658,   241,   197,   197,   170,   171,   172,
     242,   241,   197,   222,   241,   241,   241,   197,   197,   241,
     139,   140,   141,   142,   143,   635,   241,   146,   147,   148,
     149,    43,   151,   152,   153,   154,    33,   242,   197,   241,
     159,   241,   161,   162,   240,   205,   197,  1417,  1419,   242,
     240,   197,   197,   240,  1645,   241,   197,   241,  1428,   241,
      70,   242,   223,   241,    21,    22,   676,  1438,   241,   241,
     241,   223,   242,   223,  1665,   241,  1446,   687,  1645,  1734,
     209,   691,    53,   201,   694,   241,   696,   241,   241,   796,
     215,   701,   240,   208,   704,   242,   208,   208,   708,   242,
     242,   240,   712,   222,   223,   224,   225,   226,   227,   228,
     242,   242,   242,   208,   242,   242,   705,   706,   707,   238,
     239,   242,   242,   240,   240,    86,   240,     1,  1688,  1784,
     740,   241,   148,   242,   242,   724,    89,  1659,   226,  1660,
     744,  1197,  1660,  1660,  1735,  1660,  1660,     1,  1200,   738,
    1658,  1526,  1580,  1345,   764,   765,  1449,  1452,   768,  1529,
     770,  1531,  1249,  1581,  1581,    58,  1726,  1754,  1306,   511,
     780,   781,   782,   783,   784,   785,  1611,   787,   523,   789,
      -1,   789,   139,   140,   141,   142,   143,   523,    -1,   146,
     147,   148,   149,   329,   151,   152,   153,   154,    -1,    -1,
    1571,    -1,   159,    -1,   161,   162,  1577,    -1,    -1,    -1,
     167,    -1,   822,   823,  1774,    -1,   826,   827,   828,   829,
      -1,   831,    -1,   833,   834,   835,   836,   837,   838,   839,
     840,   841,   842,   843,   844,   845,   846,   847,   848,   849,
     850,   851,   852,    -1,   854,    -1,    -1,   857,    -1,    -1,
     860,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,    -1,    -1,   863,  1645,    -1,    -1,    -1,    -1,
      -1,   238,   239,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1665,    -1,    -1,    -1,    33,
     910,    -1,    -1,    -1,    -1,   915,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   926,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   913,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   928,
    1710,    -1,   952,   953,   954,   955,   956,   957,   958,   959,
     960,   961,   962,   963,   964,   965,   966,   967,   968,   969,
      -1,   950,    -1,    -1,    -1,  1735,    -1,    -1,  1739,    -1,
      -1,    -1,   982,    -1,   984,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,    -1,    -1,    -1,  1767,   159,    -1,
     161,   162,    -1,    -1,  1014,  1015,   167,   168,   169,    -1,
      -1,  1782,   173,   147,    -1,    -1,    -1,   151,  1028,    -1,
    1030,    -1,    -1,    -1,  1013,    -1,  1036,    -1,    -1,    -1,
    1040,    -1,    -1,    -1,    -1,  1045,    -1,  1047,    -1,  1049,
      -1,  1051,    -1,    -1,    -1,  1816,    -1,  1818,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,    -1,  1840,
      -1,    -1,    -1,    21,    22,    -1,    -1,   238,   239,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,   221,    -1,   223,
     224,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1110,    -1,    -1,    -1,  1114,   239,    -1,    -1,    -1,    -1,
      -1,   245,    -1,    -1,  1124,  1125,  1126,  1127,  1128,  1129,
    1130,  1131,  1132,  1133,  1134,  1135,  1136,  1137,  1138,  1139,
    1140,    -1,    -1,    -1,  1123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1165,    -1,    -1,    -1,    -1,
      -1,  1171,    -1,    -1,    -1,    -1,  1176,    -1,    -1,    -1,
      -1,  1181,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1196,    -1,    21,    22,
      -1,   139,   140,   141,   142,   143,    -1,    -1,   146,   147,
     148,   149,  1191,   151,   152,   153,   154,    -1,    -1,    -1,
      -1,   159,    -1,   161,   162,  1204,  1205,  1206,    -1,   167,
    1209,   169,  1211,    -1,  1213,    -1,  1215,    -1,  1217,    -1,
    1219,    -1,  1221,    -1,  1223,    -1,  1225,    -1,  1227,    -1,
    1229,    -1,    33,  1232,    -1,  1234,    -1,  1236,    -1,  1238,
      -1,  1240,    -1,  1242,    33,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,
      61,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,    60,    61,    60,    61,    -1,    -1,  1297,    -1,    -1,
     238,   239,    -1,    -1,    -1,  1305,  1306,    -1,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,    -1,    -1,  1334,  1335,   159,   160,   161,   162,
     163,   164,  1342,    -1,   167,   168,   169,   170,   171,   172,
     173,    -1,    -1,    -1,    -1,  1355,    -1,  1357,    -1,  1359,
      -1,    -1,    -1,    -1,    -1,  1365,   147,    21,    22,  1369,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,    -1,
     147,   150,   151,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,   216,   217,    -1,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   238,   239,    -1,    33,    -1,
     243,   244,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1429,
      -1,    -1,    -1,    -1,    -1,  1435,   217,    -1,  1438,    -1,
     221,    33,   223,   224,    -1,    60,    61,   216,   217,    -1,
     217,    -1,   221,    -1,   221,   224,   223,   224,   239,    -1,
      -1,    -1,    -1,    -1,   245,    -1,    -1,    -1,    60,    61,
     239,  1471,   239,    -1,    -1,    -1,   245,    -1,   245,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,    -1,
    1510,  1511,  1512,   167,   168,   169,    -1,    -1,    -1,   173,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147,    -1,    -1,    -1,   151,  1537,    -1,  1539,
      -1,    -1,    -1,    -1,    -1,  1545,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,    -1,   147,    -1,    -1,    -1,   151,
    1560,    -1,  1562,   217,    -1,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   238,   239,  1586,  1566,   242,  1589,
      21,    22,    -1,    -1,    -1,    -1,    -1,  1597,  1598,  1599,
      -1,    -1,   217,    -1,  1604,    -1,   221,  1607,   223,   224,
    1610,    -1,    -1,  1613,  1614,    -1,    -1,    -1,  1618,  1619,
      -1,  1621,  1622,    -1,   239,   217,    -1,    -1,    -1,   221,
     245,   223,   224,    -1,    -1,    -1,  1636,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,    -1,    -1,
      -1,    -1,    -1,   245,    -1,    -1,    -1,    -1,    -1,    -1,
    1660,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,  1684,    -1,    -1,    -1,   159,    -1,
     161,   162,    -1,    -1,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,   173,    -1,    -1,    -1,  1706,    -1,   139,   140,
     141,   142,   143,    -1,  1714,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,    -1,    -1,    -1,  1727,   159,    -1,
     161,   162,    -1,    -1,    -1,    -1,    -1,    -1,  1738,    -1,
      -1,    -1,    -1,    -1,  1744,  1745,   217,    -1,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,    -1,    -1,
      -1,    -1,    -1,    -1,  1764,  1765,    -1,   238,   239,    -1,
      -1,    33,   243,   244,    -1,  1775,    -1,    -1,    -1,  1779,
    1780,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,    60,    61,
      -1,    -1,  1802,  1782,    -1,    -1,    -1,   238,   239,    -1,
      -1,    -1,    -1,    -1,  1814,    -1,     1,    -1,    -1,    -1,
       5,     6,     7,    -1,     9,    10,    11,    -1,    13,    -1,
      15,    16,    17,    18,    19,    -1,    -1,  1816,    -1,  1818,
      25,    26,    27,    28,    29,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    39,    40,    -1,    42,    -1,    44,
      45,  1840,    -1,    48,    -1,    50,    51,    52,    -1,    54,
      55,    -1,    -1,    58,    59,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,   147,    71,    72,    73,   151,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   217,   141,   142,    -1,   221,
      -1,   223,   224,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,   157,   158,    -1,    -1,    -1,   239,    -1,    -1,
      -1,   166,    -1,   245,    -1,    -1,    -1,    -1,    -1,   174,
     175,   176,   177,   178,    -1,   180,    -1,   182,   183,   184,
     185,    -1,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,    -1,    -1,    -1,    -1,
      -1,    -1,   207,   208,   209,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
     225,   226,    -1,   228,    -1,    -1,   231,   232,    -1,    -1,
       5,     6,    -1,    -1,   239,    -1,   241,    -1,   243,   244,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    51,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    60,    61,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,    -1,
      -1,    -1,   147,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,
     175,   176,   177,   178,    -1,   180,   181,   182,   183,   184,
     185,    -1,    -1,    -1,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,    -1,    -1,    -1,    -1,
      -1,    -1,   207,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   217,    -1,    -1,    -1,   221,    -1,    -1,   224,
     225,   226,    -1,   228,    -1,    -1,   231,   232,    -1,    -1,
      -1,     5,     6,    -1,   239,    -1,   241,    -1,   243,   244,
     245,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    60,    61,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
      -1,    -1,    -1,   147,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,   175,   176,   177,   178,    -1,   180,   181,   182,   183,
     184,   185,    -1,    33,    -1,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,    -1,    -1,    -1,
      -1,    -1,    -1,   207,    -1,    -1,    -1,    -1,    -1,    -1,
      60,    61,    -1,   217,    -1,    -1,    -1,   221,    -1,    -1,
     224,   225,   226,    -1,   228,    -1,    -1,   231,   232,    -1,
      -1,    -1,     5,     6,    -1,   239,    -1,   241,    -1,   243,
     244,   245,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    68,    69,   147,    71,    72,
      73,   151,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,   141,   142,
      -1,   221,    -1,   223,   224,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,
      -1,    -1,    -1,   166,    -1,   245,    -1,    -1,    -1,    -1,
      -1,   174,   175,   176,   177,   178,    -1,   180,    -1,   182,
     183,   184,   185,    -1,    33,    -1,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    -1,    -1,
      -1,    -1,    -1,    -1,   207,    -1,    -1,    -1,    -1,    -1,
      -1,    60,    61,    -1,    -1,    -1,    -1,    -1,   221,    -1,
      -1,   224,   225,   226,    -1,   228,    -1,    -1,   231,   232,
      -1,    -1,    -1,     5,     6,    -1,   239,    -1,   241,    -1,
     243,   244,   245,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,   147,    71,
      72,    73,   151,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,   141,
     142,    -1,   221,    -1,   223,   224,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,   156,   157,   158,    -1,    -1,    -1,
     239,    -1,    -1,    -1,   166,    -1,   245,    -1,    -1,    -1,
      -1,    -1,   174,   175,   176,   177,   178,    -1,   180,    -1,
     182,   183,   184,   185,    33,    -1,    -1,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,    -1,
      -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,    -1,    -1,
      -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   224,   225,   226,    -1,   228,    -1,    -1,   231,
     232,    -1,    -1,     5,     6,    -1,    -1,   239,    -1,   241,
      -1,   243,   244,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,   147,    71,
      72,    73,   151,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,   141,
     142,    -1,   221,    -1,   223,   224,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,   156,   157,   158,    -1,    -1,    -1,
     239,    -1,    -1,    -1,   166,    -1,   245,    -1,    -1,    -1,
      -1,    -1,   174,   175,   176,   177,   178,    -1,   180,    -1,
     182,   183,   184,   185,    33,    -1,    -1,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,    -1,
      -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,    -1,    -1,
      -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   224,   225,   226,    -1,   228,    -1,    -1,   231,
     232,    -1,    -1,     5,     6,    -1,    -1,   239,    -1,   241,
      -1,   243,   244,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      52,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,   147,    71,
      72,    73,   151,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,   141,
     142,    -1,   221,    -1,   223,   224,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     239,    -1,    -1,    -1,   166,    -1,   245,    -1,    -1,    -1,
      -1,    -1,   174,   175,   176,   177,   178,    -1,   180,    -1,
     182,   183,   184,   185,    33,    -1,    -1,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,    -1,
      -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,    -1,    -1,
      -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   224,   225,   226,    -1,   228,    -1,    -1,   231,
     232,    -1,    -1,     5,     6,    -1,    -1,   239,    -1,   241,
      -1,   243,   244,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,   147,    71,
      72,    73,   151,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,   141,
     142,    -1,   221,    -1,   223,   224,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,   157,   158,    -1,    -1,    -1,
     239,    -1,    -1,    -1,   166,    -1,   245,    -1,    -1,    -1,
      -1,    -1,   174,   175,   176,   177,   178,    -1,   180,    -1,
     182,   183,   184,   185,    -1,    -1,    -1,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,    -1,
      -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   224,   225,   226,    -1,   228,    -1,    -1,   231,
     232,    -1,    -1,     5,     6,    -1,    -1,   239,    -1,   241,
      -1,   243,   244,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    70,    71,
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
     182,   183,   184,   185,    -1,    -1,    -1,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,    -1,
      -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   224,   225,   226,    -1,   228,    -1,    -1,   231,
     232,    -1,    -1,     5,     6,    -1,    -1,   239,    -1,   241,
     242,   243,   244,    15,    16,    17,    18,    19,    -1,    -1,
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
      -1,    -1,    -1,    -1,   156,   157,   158,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,   175,   176,   177,   178,    -1,   180,    -1,
     182,   183,   184,   185,    -1,    -1,    -1,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,    -1,
      -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   224,   225,   226,    -1,   228,    -1,    -1,   231,
     232,    -1,    -1,    -1,    -1,     5,     6,   239,    -1,   241,
      -1,   243,   244,    13,    -1,    15,    16,    17,    18,    19,
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
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   224,   225,   226,    -1,   228,    -1,
      -1,   231,   232,    -1,    -1,    -1,    -1,     5,     6,   239,
     240,   241,    -1,   243,   244,    13,    -1,    15,    16,    17,
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
     178,    -1,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,   207,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   224,   225,   226,    -1,
     228,    -1,    -1,   231,   232,    -1,    -1,    -1,    -1,     5,
       6,   239,    -1,   241,    -1,   243,   244,    13,    -1,    15,
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
      -1,    -1,    -1,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,    -1,    -1,    -1,    -1,    -1,
      -1,   207,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,   225,
     226,    -1,   228,    -1,    -1,   231,   232,    -1,    -1,    -1,
      -1,     5,     6,   239,   240,   241,    -1,   243,   244,    13,
      -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,    -1,
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
     194,   195,   196,   197,   198,   199,   200,    -1,    -1,    -1,
      -1,    -1,    -1,   207,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     224,   225,   226,    -1,   228,    -1,    -1,   231,   232,    -1,
      -1,     5,     6,    -1,    -1,   239,    -1,   241,    -1,   243,
     244,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
     184,   185,    -1,    -1,    -1,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,    -1,    -1,    -1,
      -1,    -1,    -1,   207,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     224,   225,   226,    -1,   228,    -1,    -1,   231,   232,    -1,
      -1,     5,     6,    -1,    -1,   239,    -1,   241,    -1,   243,
     244,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
     184,   185,    -1,    -1,    -1,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,    -1,    -1,    -1,
      -1,    -1,    -1,   207,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     224,   225,   226,    -1,   228,    -1,    -1,   231,   232,    -1,
      -1,     5,     6,    -1,    -1,   239,    -1,   241,   242,   243,
     244,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
     184,   185,    -1,    -1,    -1,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,    -1,    -1,    -1,
      -1,    -1,    -1,   207,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     224,   225,   226,    -1,   228,    -1,    -1,   231,   232,    -1,
      -1,     5,     6,    -1,    -1,   239,    -1,   241,   242,   243,
     244,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
     194,   195,   196,   197,   198,   199,   200,    -1,    -1,    -1,
      -1,    -1,    -1,   207,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     224,   225,   226,    -1,   228,    -1,    -1,   231,   232,    -1,
      -1,    -1,    -1,     5,     6,   239,   240,   241,    -1,   243,
     244,    13,    -1,    15,    16,    17,    18,    19,    -1,    -1,
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
     182,   183,   184,   185,    -1,    -1,    -1,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,    -1,
      -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   224,   225,   226,    -1,   228,    -1,    -1,   231,
     232,    -1,    -1,    -1,    -1,     5,     6,   239,    -1,   241,
      -1,   243,   244,    13,    -1,    15,    16,    17,    18,    19,
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
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   224,   225,   226,    -1,   228,    -1,
      -1,   231,   232,    -1,    -1,     5,     6,    -1,    -1,   239,
      -1,   241,    -1,   243,   244,    15,    16,    17,    18,    19,
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
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   224,   225,   226,    -1,   228,    -1,
      -1,   231,   232,    -1,    -1,     5,     6,    -1,    -1,   239,
      -1,   241,    -1,   243,   244,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
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
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   224,   225,   226,    -1,   228,    -1,
      -1,   231,   232,    -1,    -1,     5,     6,    -1,    -1,   239,
      -1,   241,    -1,   243,   244,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    58,    -1,
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
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   224,   225,   226,    -1,   228,    -1,
      -1,   231,   232,    -1,    -1,     5,     6,    -1,    -1,   239,
      -1,   241,    -1,   243,   244,    15,    16,    17,    18,    19,
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
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   222,    -1,   224,   225,   226,    -1,   228,    -1,
      -1,   231,   232,    -1,    -1,     5,     6,    -1,    -1,   239,
      -1,   241,    -1,   243,   244,    15,    16,    17,    18,    19,
      -1,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,    -1,
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
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   224,   225,   226,    -1,   228,    -1,
      -1,   231,   232,    -1,    -1,     5,     6,    -1,    -1,   239,
      -1,   241,    -1,   243,   244,    15,    16,    17,    18,    19,
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
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   224,   225,   226,    -1,   228,    -1,
      -1,   231,   232,    -1,    -1,     5,     6,    -1,    -1,   239,
      -1,   241,   242,   243,   244,    15,    16,    17,    18,    19,
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
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   222,    -1,   224,   225,   226,    -1,   228,    -1,
      -1,   231,   232,    -1,    -1,     5,     6,    -1,    -1,   239,
      -1,   241,    -1,   243,   244,    15,    16,    17,    18,    19,
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
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   224,   225,   226,    -1,   228,    -1,
      -1,   231,   232,    -1,    -1,     5,     6,    -1,    -1,   239,
      -1,   241,   242,   243,   244,    15,    16,    17,    18,    19,
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
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   224,   225,   226,    -1,   228,    -1,
      -1,   231,   232,    -1,    -1,     5,     6,    -1,    -1,   239,
      -1,   241,   242,   243,   244,    15,    16,    17,    18,    19,
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
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   224,   225,   226,    -1,   228,    -1,
      -1,   231,   232,    -1,    -1,     5,     6,    -1,    -1,   239,
      -1,   241,   242,   243,   244,    15,    16,    17,    18,    19,
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
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   224,   225,   226,    -1,   228,    -1,
      -1,   231,   232,    -1,    -1,     5,     6,    -1,    -1,   239,
      -1,   241,   242,   243,   244,    15,    16,    17,    18,    19,
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
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   224,   225,   226,    -1,   228,    -1,
      -1,   231,   232,    -1,    -1,     5,     6,    -1,    -1,   239,
      -1,   241,   242,   243,   244,    15,    16,    17,    18,    19,
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
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   224,   225,   226,    -1,   228,    -1,
      -1,   231,   232,    -1,    -1,     5,     6,    -1,    -1,   239,
      -1,   241,    -1,   243,   244,    15,    16,    17,    18,    19,
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
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   224,   225,   226,    -1,   228,    -1,
      -1,   231,   232,    -1,    -1,     5,     6,    -1,    -1,   239,
      -1,   241,    -1,   243,   244,    15,    16,    17,    18,    19,
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
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   224,   225,   226,    -1,   228,    -1,
      -1,   231,   232,    -1,    -1,     5,     6,    -1,    -1,   239,
      -1,   241,    -1,   243,   244,    15,    16,    17,    18,    19,
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
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   224,   225,   226,    -1,   228,    -1,
      -1,   231,   232,    -1,    -1,     5,     6,    -1,    -1,   239,
      -1,   241,    -1,   243,   244,    15,    16,    17,    18,    19,
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
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,    13,    -1,    -1,    -1,    -1,   207,    19,    -1,
      -1,    -1,    21,    22,    25,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,   224,   225,   226,    -1,   228,    -1,
      41,   231,   232,    -1,    -1,    -1,    -1,    -1,    49,   239,
      -1,   241,    -1,   243,   244,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
     159,    -1,   161,   162,    -1,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,
      -1,    -1,    -1,    -1,    -1,    13,    21,    22,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,   197,    25,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,   221,   222,   223,   224,   225,   226,   227,   228,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,   238,
     239,    -1,   243,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,   146,    -1,    -1,   149,    -1,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,   159,    -1,   161,   162,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,   197,
      25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    41,   222,   223,   224,
     225,   226,   227,   228,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   238,   239,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,   243,    71,    72,    73,    74,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,   197,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,   243,    -1,
     245,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   179,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,   197,    25,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,   209,
      -1,    -1,    -1,    -1,    41,   215,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,   243,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    21,    22,
     197,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,   243,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,    -1,    -1,    -1,    -1,    -1,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,    21,    22,    -1,    -1,   159,   160,   161,   162,
     163,   164,   166,    -1,   167,   168,   169,   170,   171,   172,
     173,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,    -1,    21,    22,    -1,
      -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,    -1,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   238,   239,    -1,    -1,   243,
     243,   244,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,    -1,    -1,    -1,
     159,   160,   161,   162,   163,   164,    21,    22,   167,   168,
     169,   170,   171,   172,   173,   139,   140,   141,   142,   143,
      -1,    -1,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,    -1,
      -1,    21,    22,   167,   168,   169,    -1,    -1,    -1,   173,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   238,
     239,    -1,    -1,    -1,   243,   244,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   238,   239,    -1,    -1,   242,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,   159,    -1,   161,   162,    21,    22,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,   173,   139,
     140,   141,   142,   143,    -1,    -1,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,
      -1,   161,   162,    21,    22,    -1,    -1,   167,   168,   169,
      -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   217,    -1,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   238,   239,    -1,    -1,   242,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   238,   239,
      -1,    -1,   242,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,
      21,    22,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
     173,   139,   140,   141,   142,   143,    -1,    -1,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,    -1,    -1,    -1,
      -1,   159,    -1,   161,   162,    21,    22,    -1,    -1,   167,
     168,   169,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   238,   239,    -1,    -1,   242,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,
      -1,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     238,   239,    -1,    -1,   242,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
     161,   162,    21,    22,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,   173,   139,   140,   141,   142,   143,    -1,    -1,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,    -1,
      -1,    -1,    -1,   159,    -1,   161,   162,    21,    22,    -1,
      -1,   167,   168,   169,    -1,    -1,    -1,   173,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   238,   239,    -1,
      -1,   242,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   217,    -1,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   238,   239,    -1,    -1,   242,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
     159,    -1,   161,   162,    21,    22,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,   173,   139,   140,   141,   142,   143,
      -1,    -1,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,    21,
      22,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,   173,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   238,
     239,    -1,    -1,   242,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   238,   239,    -1,    -1,   242,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,   159,    -1,   161,   162,    21,    22,    -1,    -1,
     167,   168,   169,    -1,    -1,    -1,   173,   139,   140,   141,
     142,   143,    -1,    -1,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,
     162,    21,    22,    -1,    -1,   167,   168,   169,    -1,    -1,
      -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,    -1,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   238,   239,    -1,    -1,   242,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   217,    -1,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   238,   239,    -1,    -1,
     242,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,   159,    -1,   161,   162,    21,    22,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,   173,   139,
     140,   141,   142,   143,    -1,    -1,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,
      -1,   161,   162,    21,    22,    -1,    -1,   167,   168,   169,
      -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   217,    -1,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   238,   239,    -1,    -1,   242,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   238,   239,
      -1,    -1,   242,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,
      21,    22,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
     173,   139,   140,   141,   142,   143,    -1,    -1,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,    -1,    -1,    -1,
      -1,   159,    -1,   161,   162,    21,    22,    -1,    -1,   167,
     168,   169,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   238,   239,    -1,    -1,   242,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,
      -1,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     238,   239,    -1,    -1,   242,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
     161,   162,    21,    22,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,   173,   139,   140,   141,   142,   143,    -1,    -1,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,    -1,
      -1,    -1,    -1,   159,    -1,   161,   162,    21,    22,    -1,
      -1,   167,   168,   169,    -1,    -1,    -1,   173,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   238,   239,    -1,
      -1,   242,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   217,    -1,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   238,   239,    -1,    -1,   242,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
     159,    -1,   161,   162,    21,    22,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,   173,   139,   140,   141,   142,   143,
      -1,    -1,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,    21,
      22,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,   173,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   238,
     239,    -1,    -1,   242,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   238,   239,    -1,    -1,   242,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,   159,    -1,   161,   162,    21,    22,    -1,    -1,
     167,   168,   169,    -1,    -1,    -1,   173,   139,   140,   141,
     142,   143,    -1,    -1,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,
     162,    21,    22,    -1,    -1,   167,   168,   169,    -1,    -1,
      -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,    -1,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   238,   239,    -1,    -1,   242,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   217,    -1,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   238,   239,    -1,    -1,
     242,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,   159,    -1,   161,   162,    21,    22,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,   173,   139,
     140,   141,   142,   143,    -1,    -1,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,
      -1,   161,   162,    21,    22,    -1,    -1,   167,   168,   169,
      -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   217,    -1,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   238,   239,    -1,    -1,   242,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   238,   239,
      -1,    -1,   242,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,
      21,    22,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
     173,   139,   140,   141,   142,   143,    -1,    -1,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,    -1,    -1,    -1,
      -1,   159,    -1,   161,   162,    21,    22,    -1,    -1,   167,
     168,   169,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   238,   239,    -1,    -1,   242,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,
      -1,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     238,   239,    -1,    -1,   242,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
     161,   162,    21,    22,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,   173,   139,   140,   141,   142,   143,    -1,    -1,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,    -1,
      -1,    -1,    -1,   159,    -1,   161,   162,    21,    22,    -1,
      -1,   167,   168,   169,    -1,    -1,    -1,   173,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   238,   239,    -1,
      -1,   242,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   217,    -1,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   238,   239,    -1,    -1,   242,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
     159,    -1,   161,   162,    21,    22,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,   173,   139,   140,   141,   142,   143,
      -1,    -1,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,    21,
      22,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,   173,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   238,
     239,    -1,    -1,   242,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   238,   239,    -1,    -1,   242,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,   159,    -1,   161,   162,    21,    22,    -1,    -1,
     167,   168,   169,    -1,    -1,    -1,   173,   139,   140,   141,
     142,   143,    -1,    -1,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,
     162,    21,    22,    -1,    -1,   167,   168,   169,    -1,    -1,
      -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,    -1,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   238,   239,    -1,    -1,   242,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   217,    -1,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   238,   239,    -1,    -1,
     242,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,   159,    -1,   161,   162,    21,    22,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,   173,   139,
     140,   141,   142,   143,    -1,    -1,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,
      -1,   161,   162,    21,    22,    -1,    -1,   167,   168,   169,
      -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   217,    -1,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   238,   239,    -1,    -1,   242,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   238,   239,
      -1,    -1,   242,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,
      21,    22,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
     173,   139,   140,   141,   142,   143,    -1,    -1,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,    -1,    -1,    -1,
      -1,   159,    -1,   161,   162,    21,    22,    -1,    -1,   167,
     168,   169,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   238,   239,   240,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,
      -1,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     238,   239,   240,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,   159,    -1,
     161,   162,    21,    22,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,   173,   139,   140,   141,   142,   143,    -1,    -1,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,    -1,
      -1,    -1,    -1,   159,    -1,   161,   162,    21,    22,    -1,
      -1,   167,   168,   169,    -1,    -1,    -1,   173,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   238,   239,   240,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   217,    -1,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   238,   239,   240,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,    -1,    -1,    -1,    -1,
     159,    -1,   161,   162,    -1,    -1,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,   173,   139,   140,   141,   142,   143,
      -1,    -1,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,    21,    22,    -1,    -1,   159,    -1,   161,   162,    -1,
      -1,    -1,    -1,   167,   168,   169,    -1,    -1,    38,   173,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,   238,
     239,   240,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   238,   239,   240,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,    -1,
      -1,   151,   152,   153,    -1,    -1,    -1,    -1,    21,    22,
     160,   161,   162,   163,   164,    -1,    -1,   167,   168,   169,
     170,   171,   172,   173,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,   159,    -1,   161,   162,    -1,    -1,    -1,    -1,
     167,   168,   169,    -1,    -1,    -1,   173,   217,    -1,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   186,
      -1,   231,   232,    -1,    -1,    -1,    -1,    -1,   238,   239,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     207,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
     217,    -1,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,   238,   239,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,    21,    22,    -1,    -1,   159,    -1,   161,   162,
      -1,    -1,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,   238,   239,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,    21,    22,    -1,    -1,
     159,    -1,   161,   162,    -1,    -1,   165,    -1,   167,   168,
     169,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,    21,    22,    -1,    -1,
     159,    -1,   161,   162,    -1,    -1,   165,    -1,   167,   168,
     169,    -1,    -1,    -1,   173,    -1,    -1,    -1,   217,    -1,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   238,
     239,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,   238,
     239,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
      21,    22,    -1,    -1,   159,    -1,   161,   162,    -1,    -1,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,   173,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
      -1,    -1,    -1,    -1,   159,    -1,   161,   162,    -1,    -1,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,   173,    -1,
      -1,    -1,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,   238,   239,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,    21,    22,    -1,    -1,   159,    -1,
     161,   162,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,   238,   239,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,    21,    22,    -1,    -1,   159,    -1,
     161,   162,    -1,    -1,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,   220,
     221,   222,   223,   224,   225,   226,   227,   228,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   238,   239,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,   238,   239,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,    21,    22,
      -1,    -1,   159,    -1,   161,   162,    -1,    -1,    -1,    -1,
     167,   168,   169,    -1,    -1,    -1,   173,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,    -1,    -1,
      -1,    -1,   159,    -1,   161,   162,    -1,    -1,    -1,    -1,
     167,   168,   169,    -1,    -1,    -1,   173,    -1,    -1,    -1,
     217,    -1,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   238,   239,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,    -1,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,   238,   239,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,    -1,    -1,    -1,    -1,   159,    -1,   161,   162,
      -1,    -1,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   238,   239,    71,    72,    73,
      19,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,    -1,   181,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,    71,
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
      -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   152,   153,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    -1,    73,    -1,    75,    76,
      77,    78,    79,   181,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,   197,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     238,   239,    -1,    -1,   141,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   247,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   125,   126,   188,   209,   228,   239,   248,   252,   261,
     263,   264,   269,   276,   301,   306,   342,   424,   431,   435,
     446,   492,   497,   502,    19,    20,   197,   293,   294,   295,
     189,   270,   271,   173,   197,   198,   199,   228,   238,   265,
     266,   267,   186,   207,   311,   432,   197,   243,   250,    57,
      63,   427,   427,   427,   166,   197,   328,    34,    63,   130,
     159,   232,   241,   297,   298,   299,   300,   328,   276,     5,
       6,     8,    36,   443,    62,   422,   216,   215,   218,   215,
     227,   227,   266,   227,    22,    57,   227,   238,   268,   427,
     428,   430,   428,   422,   503,   493,   498,   197,   166,   262,
     299,   299,   299,   241,   167,   168,   169,   215,   240,   130,
     130,   130,   305,    57,    63,   433,    57,    63,   444,    57,
      63,   423,    15,    16,   189,   194,   197,   200,   224,   241,
     244,   254,   294,   189,   271,   266,   266,   266,   197,   265,
     265,   197,   276,   187,   208,   312,   428,   276,    57,    63,
     249,   197,   197,   197,   197,   201,   260,   242,   295,   299,
     299,   299,   299,    57,    63,   307,   309,   197,   434,   447,
     311,   425,   201,   202,   203,   253,   189,   194,    15,    16,
     189,   194,   197,   224,   244,   254,   291,   292,   244,   268,
     429,   276,   232,   251,   504,   494,   499,   201,   242,   310,
     218,   311,   129,   441,   442,   420,   183,   244,   296,   392,
     201,   202,   203,   189,   194,   244,   215,   242,   197,   216,
      66,   218,   456,   311,   311,    35,    71,    73,    75,    76,
      77,    78,    79,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    93,    94,    95,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   141,   142,   197,   304,   308,    75,
      79,    93,    94,    98,    99,   100,   101,   451,   436,   197,
     448,   188,   312,   421,   295,   294,   244,   276,   175,   197,
     418,   419,   197,   291,    19,    25,    31,    41,    49,    64,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   179,   243,   328,   450,
     452,   453,   457,   463,   491,    79,    94,    99,   101,   311,
     495,   500,    21,    22,    38,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   151,   152,   153,   160,   161,   162,   163,
     164,   167,   168,   169,   170,   171,   172,   173,   217,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   231,
     232,   238,   239,    35,    35,   241,   302,   311,   313,   311,
     426,   218,   440,   311,   445,   392,   240,   294,   241,    43,
     215,   218,   221,   417,   222,   222,   222,   241,   222,   222,
     241,   456,   222,   222,   222,   222,   222,   241,   328,    33,
      60,    61,   147,   151,   217,   221,   224,   239,   245,   462,
     219,   505,   408,   411,   197,   197,   197,   240,    22,   197,
     240,   178,   242,   392,   403,   404,   405,   149,   218,   303,
     316,   438,   197,   276,   437,   328,   398,   419,   240,     5,
       6,    15,    16,    17,    18,    19,    25,    27,    31,    39,
      45,    48,    51,    55,    65,    68,    69,    80,   125,   126,
     127,   141,   142,   174,   175,   176,   177,   178,   180,   182,
     183,   184,   185,   189,   190,   191,   192,   193,   194,   195,
     196,   198,   199,   200,   207,   224,   225,   226,   231,   232,
     239,   241,   243,   244,   259,   261,   322,   328,   333,   347,
     354,   357,   360,   365,   368,   372,   373,   375,   380,   383,
     384,   391,   450,   507,   522,   533,   537,   550,   553,   197,
     175,   197,   463,   150,   160,   216,   416,   464,   469,   471,
     384,   473,   467,   197,   222,   475,   477,   479,   481,   483,
     485,   487,   489,   384,   222,   241,   319,    33,   221,    33,
     221,   239,   245,   240,   384,   239,   245,   463,   455,   197,
     215,   276,   406,   460,   491,   496,   197,   409,   460,   501,
     197,   131,   132,   133,   134,   135,   136,   137,   138,   160,
     170,   171,   172,   131,   132,   133,   134,   135,   136,   137,
     138,   150,   160,   170,   171,   172,   241,     7,    50,   341,
     276,   215,   276,   242,   491,   491,     1,     9,    10,    11,
      13,    26,    28,    29,    38,    40,    42,    44,    52,    54,
      58,    59,    65,   127,   128,   156,   157,   158,   198,   272,
     273,   276,   277,   280,   281,   283,   285,   286,   287,   288,
     312,   314,   315,   317,   322,   327,   329,   334,   335,   336,
     337,   338,   339,   340,   342,   346,   369,   371,   384,   426,
     216,   276,   312,    40,   239,   276,   301,   312,   400,   222,
     222,   222,   330,   452,   507,   241,   328,   222,     5,   125,
     126,   222,   241,   222,   241,   241,   222,   222,   241,   222,
     241,   222,   241,   222,   222,   241,   222,   222,   384,   384,
     241,   241,   241,   241,   241,   241,    13,   463,    13,   463,
      13,   384,   532,   548,   222,   222,   258,    13,   320,   532,
     549,   384,   384,   384,   384,   384,    13,    49,   318,   358,
     384,   181,   197,   358,   508,   510,   244,   197,   241,   301,
      21,    22,   139,   140,   141,   142,   143,   146,   147,   148,
     149,   151,   152,   153,   154,   159,   161,   162,   167,   168,
     169,   173,   217,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   238,   239,   242,   241,   241,    43,   276,
     416,   327,   369,   384,   491,   491,   461,   491,   242,   491,
     491,   242,   223,   458,   491,   302,   491,   302,   491,   302,
     406,   407,   409,   410,   242,   466,   318,   240,   240,   384,
     197,   276,   506,   218,   460,   312,   218,   460,   312,   384,
     175,   197,   413,   414,   449,   405,   405,   405,   384,   284,
     150,   327,   358,   384,   313,    61,   384,   290,   384,   197,
     276,   189,    58,   384,   313,   222,   150,   327,   384,   244,
     313,   360,   364,   364,   364,   384,   276,   276,   384,    10,
      37,   360,   366,   276,   276,   276,   276,   276,    66,   343,
     155,   276,   131,   132,   133,   134,   135,   136,   137,   138,
     144,   145,   150,   160,   163,   164,   170,   171,   172,   216,
     366,   439,   384,   399,   300,     8,   392,   397,   523,   525,
     331,   241,   328,   222,   241,   355,   222,   222,   222,   544,
     358,   463,   320,   384,   348,   350,   384,   352,   384,   546,
     358,   529,   534,   358,   527,   463,   384,   384,   384,   384,
     384,   384,   449,    53,   224,   239,   241,   384,   508,   511,
     515,   531,   536,   449,   241,   511,   536,   449,   165,   208,
     209,   210,   516,   323,   325,   203,   204,   253,   449,   208,
     215,   552,   449,    13,   240,   215,   552,   241,   150,   160,
     216,   412,   552,   215,   552,   242,   175,   180,   222,   328,
     374,    70,   239,   242,   358,   510,     4,   183,   363,    19,
     181,   197,   450,    19,   181,   197,   450,   384,   384,   384,
     384,   384,   384,   197,   384,   181,   197,   384,   384,   384,
     450,   384,   384,   384,   384,   384,   384,    22,   384,   384,
     384,   384,   384,   384,   384,   384,   384,   384,   384,   152,
     153,   181,   197,   238,   239,   381,   450,   384,   242,   358,
     384,   197,   327,   384,   131,   132,   133,   134,   135,   136,
     137,   138,   144,   145,   150,   163,   164,   170,   171,   172,
     216,   276,   223,   223,   223,   460,   223,   223,   197,   454,
     223,   303,   223,   303,   223,   303,   223,   460,   223,   460,
     321,   491,   215,   552,   240,   216,   276,   312,   491,   491,
     242,   241,    43,   215,   218,   221,   412,   313,   449,   327,
     358,   215,    14,   384,   197,   313,   216,   218,   189,   463,
     327,   384,   244,   301,   313,   313,   282,   311,   367,   183,
     241,   345,   419,   364,   156,   157,   158,   314,   370,   384,
     370,   384,   370,   384,   370,   384,   370,   384,   370,   384,
     370,   384,   370,   384,   370,   384,   370,   384,   370,   384,
     384,   370,   384,   370,   384,   370,   384,   370,   384,   370,
     384,   370,   384,   312,   276,   197,   240,    57,    63,   395,
      67,   396,   276,   463,   463,   491,    70,   358,   510,   521,
     222,   384,   197,   384,   491,   538,   540,   542,   463,   552,
     223,   460,   242,   242,   463,   463,   242,   463,   242,   463,
     552,   463,   407,   552,   410,   223,   242,   242,   242,   242,
     242,   242,    20,   364,   239,   384,   242,   165,   215,   209,
     515,   212,   213,   240,   519,   215,   209,   212,   240,   518,
      20,   242,   515,   208,   211,   517,    20,   384,   208,   532,
     321,   321,   384,    20,   532,    20,   449,   384,   384,   384,
     384,   242,   181,   197,   241,   241,   376,   378,   197,   242,
     510,   508,   215,   242,   242,   241,   150,   160,   197,   216,
     221,   361,   362,   302,   222,   241,   222,   241,   241,   241,
     240,    19,   181,   197,   450,   218,   181,   197,   384,   241,
     241,   181,   197,   384,     1,   241,   240,   242,   242,   276,
     384,   384,   384,   384,   384,   384,   384,   384,   384,   384,
     384,   384,   384,   384,   384,   384,   384,   465,   470,   472,
     491,   474,   468,   215,   223,   276,   476,   223,   480,   223,
     484,   223,   488,   406,   490,   409,   223,   460,   242,   384,
     384,   197,   175,   197,   491,   384,    20,   313,   216,   289,
     223,   363,    12,    23,    24,   274,   275,   384,   316,   301,
     197,   344,   344,   364,   364,   364,   216,   276,    47,   396,
      46,   129,   393,   223,   223,   223,   510,   242,   242,   242,
     197,   242,   209,   223,   242,   223,   463,   407,   410,   223,
     242,   241,   463,   223,   223,   223,   223,   242,   223,   223,
     242,   223,   363,   241,   358,   511,   515,   384,   508,   519,
     240,   384,   531,   240,   358,   511,   208,   211,   214,   520,
     240,   358,   223,   223,   218,   256,   358,   358,    20,   242,
     241,   160,   412,   384,   384,   463,   302,   242,   240,   239,
     362,   197,   197,   241,   197,   197,   215,   240,   303,   385,
     384,   387,   384,   242,   358,   384,   222,   241,   384,   241,
     240,   384,   239,   242,   358,   241,   240,   382,   242,   358,
     197,   459,   197,   478,   482,   486,   319,   491,   276,   242,
     241,    43,   412,   358,   491,   384,   363,   302,   313,   384,
      12,   278,   312,   363,   215,   240,   242,   491,    33,   394,
     393,   395,   524,   526,   332,   242,   223,   460,   197,   241,
     356,   223,   223,   223,   545,   320,   223,   349,   351,   353,
     547,   530,   535,   528,   241,   242,   358,   209,   515,   519,
     209,   515,   240,   209,   324,   326,   257,   205,   209,   209,
     358,   160,   412,   384,   384,   384,   242,   242,   223,   303,
     242,   508,   242,   197,   361,   240,   165,   313,   359,   463,
     242,   491,   242,   242,   242,   389,   384,   384,   242,   508,
     242,   384,   242,   384,   197,   384,   313,   366,   303,   313,
     279,   276,   302,   197,   240,   218,   417,   276,   401,   394,
     413,   414,   415,   241,   241,   384,   197,   223,   384,   539,
     541,   543,   241,   242,   241,   384,   384,   384,   241,    70,
     521,   241,   241,   242,   384,   242,   384,   519,   384,   520,
     532,   384,   319,   255,   532,   384,   209,   384,   384,   242,
     377,   223,   240,   242,   150,   384,   223,   223,   491,   242,
     242,   240,   242,   242,   359,   275,    26,   128,   280,   334,
     335,   336,   338,   384,   303,   218,   417,   463,   416,   308,
     402,   521,   521,   242,   223,   241,   242,   241,   241,   241,
     318,   320,   358,   521,   521,   242,   209,   551,   551,   551,
     201,   551,   551,   384,   160,   412,   374,   379,   242,   384,
     386,   388,   223,   242,   150,   150,   384,   313,   463,   416,
     416,   327,   384,   276,   308,   241,   508,   512,   513,   514,
     514,   384,   384,   521,   521,   508,   509,   242,   242,   552,
     514,   509,    53,   240,   208,   208,   208,   240,   551,   384,
     384,   374,   390,   384,   416,   327,   384,   327,   384,   276,
     313,   508,   215,   552,   242,   242,   242,   242,   514,   514,
     242,   242,   242,   242,   384,   240,   240,   208,   240,   327,
     384,   276,   276,   242,   241,   242,   242,   276,   508,   242
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   246,   247,   247,   247,   247,   247,   247,   247,   247,
     247,   247,   247,   247,   247,   247,   247,   248,   249,   249,
     249,   250,   250,   251,   251,   252,   253,   253,   253,   253,
     254,   254,   255,   255,   256,   257,   256,   258,   258,   258,
     259,   260,   260,   262,   261,   263,   264,   265,   265,   265,
     266,   266,   266,   266,   266,   266,   266,   267,   267,   268,
     268,   269,   270,   270,   271,   271,   272,   273,   273,   274,
     274,   275,   275,   275,   276,   276,   277,   277,   278,   279,
     278,   280,   280,   280,   280,   280,   281,   282,   281,   284,
     283,   285,   286,   287,   289,   288,   290,   288,   291,   291,
     291,   291,   291,   291,   291,   291,   291,   292,   292,   293,
     293,   293,   294,   294,   294,   294,   294,   294,   294,   294,
     294,   294,   294,   295,   295,   296,   296,   296,   297,   297,
     297,   297,   298,   298,   299,   299,   299,   299,   299,   299,
     299,   300,   300,   301,   301,   302,   302,   302,   303,   303,
     303,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   305,   305,   306,   307,   307,   307,   308,
     310,   309,   311,   311,   312,   312,   313,   313,   314,   314,
     314,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   316,   316,   316,   317,   318,   318,   319,   319,
     320,   320,   321,   321,   323,   324,   322,   325,   326,   322,
     327,   327,   327,   327,   327,   328,   328,   328,   329,   329,
     331,   332,   330,   330,   333,   333,   333,   333,   333,   333,
     334,   335,   336,   336,   336,   337,   337,   337,   338,   338,
     339,   339,   339,   340,   341,   341,   341,   342,   342,   343,
     343,   344,   344,   345,   345,   345,   345,   345,   345,   345,
     345,   346,   346,   348,   349,   347,   350,   351,   347,   352,
     353,   347,   355,   356,   354,   357,   357,   357,   357,   357,
     357,   358,   358,   359,   359,   359,   360,   360,   360,   361,
     361,   361,   361,   361,   362,   362,   363,   363,   363,   364,
     364,   365,   367,   366,   368,   368,   368,   368,   368,   368,
     368,   368,   369,   369,   369,   369,   369,   369,   369,   369,
     369,   369,   369,   369,   369,   369,   369,   369,   369,   369,
     369,   370,   370,   370,   370,   371,   371,   371,   371,   371,
     371,   371,   371,   371,   371,   371,   371,   371,   371,   371,
     371,   371,   372,   372,   373,   373,   374,   374,   375,   376,
     377,   375,   378,   379,   375,   380,   380,   380,   380,   380,
     380,   380,   381,   382,   380,   383,   383,   383,   383,   383,
     383,   383,   384,   384,   384,   384,   384,   384,   384,   384,
     384,   384,   384,   384,   384,   384,   384,   384,   384,   384,
     384,   384,   384,   384,   384,   384,   384,   384,   384,   384,
     384,   384,   384,   384,   384,   384,   384,   384,   384,   384,
     384,   384,   384,   384,   384,   384,   384,   384,   384,   384,
     384,   384,   384,   384,   384,   384,   384,   384,   384,   385,
     386,   384,   384,   384,   384,   387,   388,   384,   384,   384,
     389,   390,   384,   384,   384,   384,   384,   384,   384,   384,
     384,   384,   384,   384,   384,   384,   384,   391,   391,   391,
     391,   391,   391,   391,   391,   391,   391,   391,   391,   391,
     391,   391,   391,   392,   392,   392,   393,   393,   393,   394,
     394,   395,   395,   395,   396,   396,   397,   398,   398,   399,
     398,   400,   398,   401,   398,   402,   398,   398,   403,   404,
     404,   405,   405,   405,   405,   405,   406,   406,   407,   407,
     408,   408,   408,   409,   410,   410,   411,   411,   411,   412,
     412,   413,   413,   413,   414,   414,   415,   415,   416,   416,
     416,   417,   417,   418,   418,   418,   418,   418,   418,   419,
     419,   419,   419,   419,   420,   420,   421,   420,   422,   422,
     423,   423,   423,   424,   425,   424,   426,   426,   426,   426,
     427,   427,   427,   429,   428,   430,   430,   431,   432,   431,
     433,   433,   433,   434,   436,   437,   435,   438,   439,   435,
     440,   440,   441,   441,   442,   443,   443,   443,   443,   444,
     444,   444,   445,   445,   447,   448,   446,   449,   449,   449,
     449,   449,   449,   450,   450,   450,   450,   450,   450,   450,
     450,   450,   450,   450,   450,   450,   450,   450,   450,   450,
     450,   450,   450,   450,   450,   450,   450,   450,   450,   450,
     450,   450,   450,   450,   450,   450,   450,   450,   450,   450,
     450,   450,   450,   450,   450,   450,   450,   450,   450,   450,
     450,   450,   450,   451,   451,   451,   451,   451,   451,   451,
     451,   452,   453,   453,   453,   454,   454,   454,   455,   455,
     455,   455,   456,   456,   456,   456,   456,   457,   458,   459,
     457,   460,   460,   461,   461,   462,   462,   463,   463,   463,
     463,   463,   463,   464,   465,   463,   463,   463,   466,   463,
     463,   463,   463,   463,   463,   463,   463,   463,   463,   463,
     463,   463,   467,   468,   463,   463,   469,   470,   463,   471,
     472,   463,   473,   474,   463,   463,   475,   476,   463,   477,
     478,   463,   463,   479,   480,   463,   481,   482,   463,   463,
     483,   484,   463,   485,   486,   463,   487,   488,   463,   489,
     490,   463,   491,   491,   491,   493,   494,   495,   496,   492,
     498,   499,   500,   501,   497,   503,   504,   505,   506,   502,
     507,   507,   507,   507,   507,   508,   508,   508,   508,   508,
     508,   508,   508,   509,   509,   510,   511,   511,   512,   512,
     513,   513,   514,   514,   515,   515,   516,   516,   517,   517,
     518,   518,   519,   519,   519,   520,   520,   520,   521,   521,
     522,   522,   522,   522,   522,   522,   523,   524,   522,   525,
     526,   522,   527,   528,   522,   529,   530,   522,   531,   531,
     531,   532,   532,   533,   534,   535,   533,   536,   536,   537,
     537,   537,   538,   539,   537,   540,   541,   537,   542,   543,
     537,   537,   544,   545,   537,   537,   546,   547,   537,   548,
     548,   549,   549,   550,   550,   550,   550,   550,   551,   551,
     552,   552,   553,   553,   553,   553,   553,   553
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
       3,     1,     2,     1,     2,     1,     1,     1,     3,     1,
       1,     1,     3,     3,     5,     3,     4,     3,     4,     3,
       3,     1,     5,     1,     3,     2,     3,     2,     1,     1,
       1,     1,     1,     4,     1,     2,     3,     3,     3,     3,
       2,     1,     3,     0,     3,     0,     2,     3,     0,     2,
       2,     1,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     3,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     3,
       2,     2,     3,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     3,     2,     2,     2,     2,
       2,     3,     3,     3,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     1,     4,     0,     1,     1,     3,
       0,     4,     1,     1,     1,     1,     3,     7,     2,     2,
       6,     1,     1,     1,     1,     2,     2,     1,     1,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     1,     2,
       2,     2,     0,     2,     2,     3,     0,     2,     0,     4,
       0,     2,     1,     3,     0,     0,     7,     0,     0,     7,
       3,     2,     2,     2,     1,     1,     3,     2,     2,     3,
       0,     0,     5,     1,     2,     5,     5,     5,     6,     2,
       1,     1,     1,     2,     3,     2,     2,     3,     2,     3,
       2,     2,     3,     4,     1,     1,     0,     1,     1,     1,
       0,     1,     3,     9,     8,     8,     7,     8,     7,     7,
       6,     3,     3,     0,     0,     7,     0,     0,     7,     0,
       0,     7,     0,     0,     6,     5,     8,    10,     5,     8,
      10,     1,     3,     1,     2,     3,     1,     1,     2,     2,
       2,     2,     2,     4,     1,     3,     0,     4,     4,     1,
       6,     6,     0,     7,     1,     1,     1,     1,     1,     1,
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
       7,     0,     4,     0,     9,     0,    10,     5,     3,     3,
       4,     1,     1,     3,     3,     3,     1,     3,     1,     3,
       0,     2,     3,     3,     1,     3,     0,     2,     3,     1,
       1,     1,     2,     3,     3,     5,     1,     1,     1,     1,
       1,     0,     1,     1,     4,     3,     3,     6,     5,     4,
       6,     5,     5,     4,     0,     2,     0,     4,     0,     1,
       0,     1,     1,     6,     0,     6,     0,     2,     3,     5,
       0,     1,     1,     0,     5,     2,     3,     4,     0,     4,
       0,     1,     1,     1,     0,     0,     9,     0,     0,    11,
       0,     2,     0,     1,     3,     1,     1,     2,     2,     0,
       1,     1,     0,     3,     0,     0,     7,     1,     4,     3,
       3,     6,     5,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     4,     1,     3,     3,     0,     2,
       3,     5,     0,     2,     2,     2,     2,     4,     0,     0,
       7,     1,     1,     1,     3,     3,     4,     1,     1,     1,
       1,     2,     3,     0,     0,     6,     4,     3,     0,     7,
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

  case 102: /* annotation_argument_value: '-' "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",int32_t(0u - uint32_t((yyvsp[0].i)))); }
    break;

  case 103: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 104: /* annotation_argument_value: '-' "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",-float((yyvsp[0].fd))); }
    break;

  case 105: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 106: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 107: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 108: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 109: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 110: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 111: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 112: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 113: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 114: /* annotation_argument: annotation_argument_name '=' '@' '@' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[0].s); delete (yyvsp[-4].s); }
    break;

  case 115: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 116: /* annotation_argument: annotation_argument_name '=' '-' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-3].s),int32_t(0u - uint32_t((yyvsp[0].i))),tokAt(scanner,(yylsp[-3]))); delete (yyvsp[-3].s); }
    break;

  case 117: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 118: /* annotation_argument: annotation_argument_name '=' '-' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-3].s),-float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-3]))); delete (yyvsp[-3].s); }
    break;

  case 119: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 120: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 121: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 122: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 123: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 124: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 125: /* metadata_argument_list: '@' annotation_argument  */
                                      {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 126: /* metadata_argument_list: metadata_argument_list '@' annotation_argument  */
                                                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 127: /* metadata_argument_list: metadata_argument_list semicolon  */
                                               {
        (yyval.aaList) = (yyvsp[-1].aaList);
    }
    break;

  case 128: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 129: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 130: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 131: /* annotation_declaration_name: "template"  */
                                    { (yyval.s) = new string("template"); }
    break;

  case 132: /* annotation_declaration_basic: annotation_declaration_name  */
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

  case 133: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 134: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 135: /* annotation_declaration: '!' annotation_declaration  */
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

  case 136: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 137: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 138: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 139: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 140: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 141: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 142: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 143: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 144: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 145: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 146: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 147: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 148: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yyloc));
    }
    break;

  case 149: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 150: /* optional_function_type: "->" type_declaration  */
                                           {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 151: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 152: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 153: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 154: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 155: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 156: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 157: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 158: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 159: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 160: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 161: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 162: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 163: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 164: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 165: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 166: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 167: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 168: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 169: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 170: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 171: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 172: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 173: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 174: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 175: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 176: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 177: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 178: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 179: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 180: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 181: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 182: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 183: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 184: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 185: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 186: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 187: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 188: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 189: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 190: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 191: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 192: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 193: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 194: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 195: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 196: /* function_name: "operator" '[' ']' "<-"  */
                                    { (yyval.s) = new string("[]<-"); }
    break;

  case 197: /* function_name: "operator" '[' ']' ":="  */
                                      { (yyval.s) = new string("[]:="); }
    break;

  case 198: /* function_name: "operator" '[' ']' "+="  */
                                     { (yyval.s) = new string("[]+="); }
    break;

  case 199: /* function_name: "operator" '[' ']' "-="  */
                                     { (yyval.s) = new string("[]-="); }
    break;

  case 200: /* function_name: "operator" '[' ']' "*="  */
                                     { (yyval.s) = new string("[]*="); }
    break;

  case 201: /* function_name: "operator" '[' ']' "/="  */
                                     { (yyval.s) = new string("[]/="); }
    break;

  case 202: /* function_name: "operator" '[' ']' "%="  */
                                     { (yyval.s) = new string("[]%="); }
    break;

  case 203: /* function_name: "operator" '[' ']' "&="  */
                                     { (yyval.s) = new string("[]&="); }
    break;

  case 204: /* function_name: "operator" '[' ']' "|="  */
                                     { (yyval.s) = new string("[]|="); }
    break;

  case 205: /* function_name: "operator" '[' ']' "^="  */
                                     { (yyval.s) = new string("[]^="); }
    break;

  case 206: /* function_name: "operator" '[' ']' "&&="  */
                                        { (yyval.s) = new string("[]&&="); }
    break;

  case 207: /* function_name: "operator" '[' ']' "||="  */
                                        { (yyval.s) = new string("[]||="); }
    break;

  case 208: /* function_name: "operator" '[' ']' "^^="  */
                                        { (yyval.s) = new string("[]^^="); }
    break;

  case 209: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 210: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 211: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 212: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 213: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 214: /* function_name: "operator" '.' "name" "+="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`+="); delete (yyvsp[-1].s); }
    break;

  case 215: /* function_name: "operator" '.' "name" "-="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`-="); delete (yyvsp[-1].s); }
    break;

  case 216: /* function_name: "operator" '.' "name" "*="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`*="); delete (yyvsp[-1].s); }
    break;

  case 217: /* function_name: "operator" '.' "name" "/="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`/="); delete (yyvsp[-1].s); }
    break;

  case 218: /* function_name: "operator" '.' "name" "%="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`%="); delete (yyvsp[-1].s); }
    break;

  case 219: /* function_name: "operator" '.' "name" "&="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&="); delete (yyvsp[-1].s); }
    break;

  case 220: /* function_name: "operator" '.' "name" "|="  */
                                          { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`|="); delete (yyvsp[-1].s); }
    break;

  case 221: /* function_name: "operator" '.' "name" "^="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^="); delete (yyvsp[-1].s); }
    break;

  case 222: /* function_name: "operator" '.' "name" "&&="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&&="); delete (yyvsp[-1].s); }
    break;

  case 223: /* function_name: "operator" '.' "name" "||="  */
                                            { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`||="); delete (yyvsp[-1].s); }
    break;

  case 224: /* function_name: "operator" '.' "name" "^^="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^^="); delete (yyvsp[-1].s); }
    break;

  case 225: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 226: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 227: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 228: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 229: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 230: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 231: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 232: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 233: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 234: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 235: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 236: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 237: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 238: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 239: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 240: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 241: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 242: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 243: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 244: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 245: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 246: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 247: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 248: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 249: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 250: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 251: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 252: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 253: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 254: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 255: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 256: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 257: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 258: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 259: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 260: /* function_name: "float16"  */
                     { (yyval.s) = new string("float16"); }
    break;

  case 261: /* function_name: "half2"  */
                     { (yyval.s) = new string("half2"); }
    break;

  case 262: /* function_name: "half3"  */
                     { (yyval.s) = new string("half3"); }
    break;

  case 263: /* function_name: "half4"  */
                     { (yyval.s) = new string("half4"); }
    break;

  case 264: /* function_name: "half8"  */
                     { (yyval.s) = new string("half8"); }
    break;

  case 265: /* function_name: "short2"  */
                     { (yyval.s) = new string("short2"); }
    break;

  case 266: /* function_name: "short3"  */
                     { (yyval.s) = new string("short3"); }
    break;

  case 267: /* function_name: "short4"  */
                     { (yyval.s) = new string("short4"); }
    break;

  case 268: /* function_name: "short8"  */
                     { (yyval.s) = new string("short8"); }
    break;

  case 269: /* function_name: "ushort2"  */
                     { (yyval.s) = new string("ushort2"); }
    break;

  case 270: /* function_name: "ushort3"  */
                     { (yyval.s) = new string("ushort3"); }
    break;

  case 271: /* function_name: "ushort4"  */
                     { (yyval.s) = new string("ushort4"); }
    break;

  case 272: /* function_name: "ushort8"  */
                     { (yyval.s) = new string("ushort8"); }
    break;

  case 273: /* function_name: "byte2"  */
                     { (yyval.s) = new string("byte2"); }
    break;

  case 274: /* function_name: "byte3"  */
                     { (yyval.s) = new string("byte3"); }
    break;

  case 275: /* function_name: "byte4"  */
                     { (yyval.s) = new string("byte4"); }
    break;

  case 276: /* function_name: "byte8"  */
                     { (yyval.s) = new string("byte8"); }
    break;

  case 277: /* function_name: "byte16"  */
                     { (yyval.s) = new string("byte16"); }
    break;

  case 278: /* function_name: "ubyte2"  */
                     { (yyval.s) = new string("ubyte2"); }
    break;

  case 279: /* function_name: "ubyte3"  */
                     { (yyval.s) = new string("ubyte3"); }
    break;

  case 280: /* function_name: "ubyte4"  */
                     { (yyval.s) = new string("ubyte4"); }
    break;

  case 281: /* function_name: "ubyte8"  */
                     { (yyval.s) = new string("ubyte8"); }
    break;

  case 282: /* function_name: "ubyte16"  */
                     { (yyval.s) = new string("ubyte16"); }
    break;

  case 283: /* optional_template: %empty  */
                                        { (yyval.b) = false; }
    break;

  case 284: /* optional_template: "template"  */
                                        { (yyval.b) = true; }
    break;

  case 285: /* global_function_declaration: optional_annotation_list "def" optional_template function_declaration  */
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

  case 286: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 287: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 288: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 289: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 290: /* $@8: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 291: /* function_declaration: optional_public_or_private_function $@8 function_declaration_header expression_block  */
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

  case 296: /* expression_block: open_block expressions close_block  */
                                                                  {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 297: /* expression_block: open_block expressions close_block "finally" open_block expressions close_block  */
                                                                                                                        {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        // gc_node — don't delete Expression
    }
    break;

  case 298: /* expr_call_pipe: expr expr_full_block_assumed_piped  */
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

  case 299: /* expr_call_pipe: expression_keyword expr_full_block_assumed_piped  */
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

  case 300: /* expr_call_pipe: "generator" '<' type_declaration_no_options '>' optional_capture_list expr_full_block_assumed_piped  */
                                                                                                                                             {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-3].pTypeDecl),(yyvsp[-1].pCaptList),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 301: /* expression_any: semicolon  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 302: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 303: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 304: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 305: /* expression_any: expr_assign semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 306: /* expression_any: expression_delete semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 307: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 308: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 309: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 310: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 311: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 312: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 313: /* expression_any: expression_break semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 314: /* expression_any: expression_continue semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 315: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 316: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 317: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 318: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 319: /* expression_any: expression_label semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 320: /* expression_any: expression_goto semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 321: /* expression_any: "pass" semicolon  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 322: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 323: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back((yyvsp[0].pExpression));
        }
    }
    break;

  case 324: /* expressions: expressions error  */
                                 {
        (void)(yyvsp[-1].pExpression); /* gc_node — don't delete Expression */ (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 325: /* expr_keyword: "keyword" expr expression_block  */
                                                           {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s));
        pCall->arguments.push_back((yyvsp[-1].pExpression));
        auto resT = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        auto blk = ast_makeBlock(scanner,0,nullptr,nullptr,nullptr,resT,(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),LineInfo());
        pCall->arguments.push_back(blk);
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 326: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 327: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 328: /* optional_expr_list_in_braces: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 329: /* optional_expr_list_in_braces: '(' optional_expr_list optional_comma ')'  */
                                                             { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 330: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 331: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 332: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 333: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 334: /* $@9: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 335: /* $@10: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 336: /* expression_keyword: "keyword" '<' $@9 type_declaration_no_options_list '>' $@10 expr  */
                                                                                                                                                     {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 337: /* $@11: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 338: /* $@12: %empty  */
                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 339: /* expression_keyword: "type function" '<' $@11 type_declaration_no_options_list '>' $@12 optional_expr_list_in_braces  */
                                                                                                                                                                                   {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 340: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 341: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 342: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 343: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 344: /* expr_pipe: expr_call_pipe  */
                             {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 345: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 346: /* name_in_namespace: "name" "::" "name"  */
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

  case 347: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 348: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
    }
    break;

  case 349: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 350: /* $@13: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 351: /* $@14: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 352: /* new_type_declaration: '<' $@13 type_declaration '>' $@14  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 353: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 354: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pTypeDecl),false);
    }
    break;

  case 355: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 356: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 357: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),(yyvsp[-1].pExpression));
    }
    break;

  case 358: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),(yyvsp[-1].pExpression));
    }
    break;

  case 359: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 360: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 361: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 362: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 363: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 364: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 365: /* expression_return: expression_return_no_pipe semicolon  */
                                                    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 366: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 367: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 368: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 369: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 370: /* expression_yield: expression_yield_no_pipe semicolon  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 371: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 372: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 373: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 374: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 375: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 376: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 377: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 378: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 379: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 380: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 381: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 382: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 383: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-7].pNameList),tokAt(scanner,(yylsp[-7])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 384: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 385: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 386: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                           {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameList),tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 387: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr semicolon  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-6])));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 388: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-5])));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameList),tokAt(scanner,(yylsp[-5])),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 389: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr semicolon  */
                                                                                                        {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-5])));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameList),tokAt(scanner,(yylsp[-5])),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 390: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                   {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-4])));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameList),tokAt(scanner,(yylsp[-4])),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 391: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 392: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 393: /* $@15: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 394: /* $@16: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 395: /* expr_cast: "cast" '<' $@15 type_declaration_no_options '>' $@16 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
    }
    break;

  case 396: /* $@17: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 397: /* $@18: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 398: /* expr_cast: "upcast" '<' $@17 type_declaration_no_options '>' $@18 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 399: /* $@19: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 400: /* $@20: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 401: /* expr_cast: "reinterpret" '<' $@19 type_declaration_no_options '>' $@20 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 402: /* $@21: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 403: /* $@22: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 404: /* expr_type_decl: "type" '<' $@21 type_declaration '>' $@22  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 405: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 406: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 407: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" c_or_s "name" '>' expr ')'  */
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

  case 408: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
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

  case 409: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
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

  case 410: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" "end of expression" "name" '>' '(' expr ')'  */
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

  case 411: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 412: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 413: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 414: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = new ExprReturn(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->generated = true;
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 415: /* block_or_simple_block: "=>" "<-" expr  */
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

  case 416: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 417: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 418: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 419: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 420: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 421: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 422: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 423: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 424: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 425: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 426: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 427: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 428: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 429: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),(yyvsp[0].pExpression));
        closure->returnType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
    }
    break;

  case 430: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 431: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 432: /* $@23: %empty  */
                             {  yyextra->das_need_oxford_comma = false; }
    break;

  case 433: /* expr_full_block_assumed_piped: block_or_lambda $@23 optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 434: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstInt>(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 435: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstUInt>(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 436: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstInt64>(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 437: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstUInt64>(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 438: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstUInt8>(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 439: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstFloat>(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 440: /* expr_numeric_const: "float16 constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstFloat16>(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 441: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = newConstLiteral<ExprConstDouble>(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 442: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 443: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 444: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 445: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 446: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 447: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 448: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 449: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 450: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 451: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 452: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 453: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 454: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 455: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 456: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 457: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 458: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 459: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 460: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 461: /* expr_assign_pipe_right: "@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 462: /* expr_assign_pipe_right: "@@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 463: /* expr_assign_pipe_right: "$ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 464: /* expr_assign_pipe_right: expr_call_pipe  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 465: /* expr_assign_pipe: expr '=' expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 466: /* expr_assign_pipe: expr "<-" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 467: /* expr_assign_pipe: expr "&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 468: /* expr_assign_pipe: expr "|=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 469: /* expr_assign_pipe: expr "^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 470: /* expr_assign_pipe: expr "&&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 471: /* expr_assign_pipe: expr "||=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 472: /* expr_assign_pipe: expr "^^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 473: /* expr_assign_pipe: expr "+=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 474: /* expr_assign_pipe: expr "-=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 475: /* expr_assign_pipe: expr "*=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 476: /* expr_assign_pipe: expr "/=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 477: /* expr_assign_pipe: expr "%=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 478: /* expr_assign_pipe: expr "<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 479: /* expr_assign_pipe: expr ">>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 480: /* expr_assign_pipe: expr "<<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 481: /* expr_assign_pipe: expr ">>>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 482: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 483: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 484: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 485: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-3])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-5]),(yyloc));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 486: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 487: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 488: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 489: /* $@24: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 490: /* $@25: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 491: /* func_addr_expr: '@' '@' '<' $@24 type_declaration_no_options '>' $@25 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value));
        expr->funcType = (yyvsp[-3].pTypeDecl);
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 492: /* $@26: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 493: /* $@27: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 494: /* func_addr_expr: '@' '@' '<' $@26 optional_function_argument_list optional_function_type '>' $@27 func_addr_name  */
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

  case 495: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 496: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 497: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 498: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-3])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-5]),(yyloc));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 499: /* expr_field: expr '.' "name" '(' '[' make_struct_fields ']' ')'  */
                                                                                       {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->methodCall = true;
        nc->arguments = (yyvsp[-2].pMakeStruct);
        nc->nonNamedArguments.push_back((yyvsp[-7].pExpression));
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 500: /* expr_field: expr '.' basic_type_declaration '(' ')'  */
                                                                        {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), method_name);
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-4]),(yyloc));
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 501: /* expr_field: expr '.' basic_type_declaration '(' expr_list ')'  */
                                                                                             {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-3])), (yyvsp[-5].pExpression), method_name);
        pInvoke->atEnclosure = tokRangeAt(scanner,(yylsp[-5]),(yyloc));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 502: /* $@28: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 503: /* $@29: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 504: /* expr_field: expr '.' $@28 error $@29  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), "");
        yyerrok;
    }
    break;

  case 505: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 506: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
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

  case 507: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
                                                               {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-3].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 508: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
                                                                                 {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-4])),*(yyvsp[-4].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-4].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 509: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 510: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 511: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 512: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 513: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 514: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 515: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 516: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 517: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 518: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 519: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 520: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 521: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 522: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",(yyvsp[0].pExpression)); }
    break;

  case 523: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",(yyvsp[0].pExpression)); }
    break;

  case 524: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",(yyvsp[0].pExpression)); }
    break;

  case 525: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",(yyvsp[0].pExpression)); }
    break;

  case 526: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 527: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 528: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 529: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 530: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 531: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 532: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 533: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 534: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 535: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 536: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 537: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 538: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 539: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 540: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 541: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 542: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 543: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 544: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 545: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 546: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 547: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back((yyvsp[-2].pExpression));
        itv->arguments.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = itv;
    }
    break;

  case 548: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", (yyvsp[0].pExpression)); }
    break;

  case 549: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", (yyvsp[0].pExpression)); }
    break;

  case 550: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", (yyvsp[-1].pExpression)); }
    break;

  case 551: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", (yyvsp[-1].pExpression)); }
    break;

  case 552: /* expr: '(' expr_list optional_comma ')'  */
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

  case 553: /* expr: '(' make_struct_single ')'  */
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

  case 554: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 555: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 556: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 557: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 558: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 559: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 560: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 561: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 562: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression)); }
    break;

  case 563: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 564: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 565: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 566: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 567: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 568: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
        }
    break;

  case 569: /* $@30: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 570: /* $@31: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 571: /* expr: expr "is" "type" '<' $@30 type_declaration_no_options '>' $@31  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 572: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type), tokAt(scanner,(yyloc)));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),vdecl);
    }
    break;

  case 573: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 574: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 575: /* $@32: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 576: /* $@33: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 577: /* expr: expr "as" "type" '<' $@32 type_declaration '>' $@33  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 578: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 579: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 580: /* $@34: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 581: /* $@35: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 582: /* expr: expr '?' "as" "type" '<' $@34 type_declaration '>' $@35  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-8].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 583: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 584: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 585: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 586: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 587: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 588: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 589: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 590: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 591: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 592: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 593: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 594: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 595: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 596: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 597: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 598: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 599: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 600: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 601: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 602: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 603: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 604: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 605: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 606: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 607: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 608: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 609: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 610: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-6].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 611: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 612: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 613: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 614: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); /*this one is gone when BRABRA is disabled*/ }
    break;

  case 615: /* optional_field_annotation: metadata_argument_list  */
                                                        { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 616: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 617: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 618: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 619: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 620: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 621: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 622: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 623: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 624: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 625: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 626: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 627: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 628: /* struct_variable_declaration_list: struct_variable_declaration_list semicolon  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 629: /* $@36: %empty  */
                                                               { yyextra->das_force_oxford_comma=true;}
    break;

  case 630: /* struct_variable_declaration_list: struct_variable_declaration_list "typedef" $@36 "name" '=' type_declaration semicolon  */
                                                                                                                                                         {
        (yyval.pVarDeclList) = (yyvsp[-6].pVarDeclList);
        ast_structureAlias(scanner,(yyvsp[-3].s),(yyvsp[-1].pTypeDecl),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 631: /* $@37: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 632: /* struct_variable_declaration_list: struct_variable_declaration_list $@37 structure_variable_declaration semicolon  */
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

  case 633: /* $@38: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 634: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@38 function_declaration_header semicolon  */
                                                          {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 635: /* $@39: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 636: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@39 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 637: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' semicolon  */
                                                                                       {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::invalid_annotation);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 638: /* function_argument_declaration_no_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_no_type  */
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

  case 639: /* function_argument_declaration_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_type  */
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

  case 640: /* function_argument_declaration_type: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))));
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none, tokAt(scanner,(yyloc))), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 641: /* function_argument_list: function_argument_declaration_no_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 642: /* function_argument_list: function_argument_declaration_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 643: /* function_argument_list: function_argument_declaration_no_type semicolon function_argument_list  */
                                                                                            { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 644: /* function_argument_list: function_argument_declaration_type semicolon function_argument_list  */
                                                                                            { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 645: /* function_argument_list: function_argument_declaration_type ',' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 646: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 647: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 648: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 649: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                          { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 650: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 651: /* tuple_alias_type_list: tuple_alias_type_list c_or_s  */
                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 652: /* tuple_alias_type_list: tuple_alias_type_list tuple_type c_or_s  */
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

  case 653: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 654: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 655: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 656: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 657: /* variant_alias_type_list: variant_alias_type_list c_or_s  */
                                           {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 658: /* variant_alias_type_list: variant_alias_type_list variant_type c_or_s  */
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

  case 659: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 660: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 661: /* variable_declaration_no_type: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[0])));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 662: /* variable_declaration_no_type: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-1])));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 663: /* variable_declaration_no_type: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-2])));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 664: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 665: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 666: /* variable_declaration: variable_declaration_type  */
                                        {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 667: /* variable_declaration: variable_declaration_no_type  */
                                           {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 668: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 669: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 670: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 671: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 672: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 673: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 674: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 675: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 676: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 677: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "$i" '(' expr ')'  */
                                                                               {
        (yyvsp[-5].pNameWithPosList)->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = (yyvsp[-5].pNameWithPosList);
    }
    break;

  case 678: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 679: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options semicolon  */
                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[-1]));
    }
    break;

  case 680: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[-1]));
    }
    break;

  case 681: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 682: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr semicolon  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-4])));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[-1]));
    }
    break;

  case 683: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer, tokAt(scanner,(yylsp[-3])));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->atEnd = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 684: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 685: /* global_variable_declaration_list: global_variable_declaration_list "end of line"  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 686: /* $@40: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 687: /* global_variable_declaration_list: global_variable_declaration_list $@40 optional_field_annotation let_variable_declaration  */
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

  case 688: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 689: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 690: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 691: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 692: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 693: /* global_let: kwd_let optional_shared optional_public_or_private_variable open_block global_variable_declaration_list close_block  */
                                                                                                                                                      {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 694: /* $@41: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 695: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@41 optional_field_annotation let_variable_declaration  */
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

  case 696: /* enum_list: %empty  */
        {
        (yyval.pEnumList) = new Enumeration();
    }
    break;

  case 697: /* enum_list: enum_list semicolon  */
                                {
        (yyval.pEnumList) = (yyvsp[-1].pEnumList);
    }
    break;

  case 698: /* enum_list: enum_list "name" semicolon  */
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

  case 699: /* enum_list: enum_list "name" '=' expr semicolon  */
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

  case 700: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 701: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 702: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 703: /* $@42: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 704: /* single_alias: optional_public_or_private_alias "name" $@42 '=' type_declaration  */
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

  case 708: /* $@43: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 710: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 711: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 712: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 713: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 714: /* $@44: %empty  */
                                                                                                                       {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 715: /* $@45: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 716: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name open_block $@44 enum_list $@45 close_block  */
                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-2].pEnumList),Type::tInt);
    }
    break;

  case 717: /* $@46: %empty  */
                                                                                                                                                            {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 718: /* $@47: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 719: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration open_block $@46 enum_list $@47 close_block  */
                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-2].pEnumList),(yyvsp[-5].type));
    }
    break;

  case 720: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 721: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 722: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 723: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 724: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 725: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 726: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 727: /* class_or_struct: "class" "template"  */
                                 { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 728: /* class_or_struct: "struct" "template"  */
                                 { (yyval.i) = CorS_StructTemplate; }
    break;

  case 729: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 730: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 731: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 732: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 733: /* optional_struct_variable_declaration_list: open_block struct_variable_declaration_list close_block  */
                                                                      {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 734: /* $@48: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 735: /* $@49: %empty  */
                         {
        if ( (yyvsp[0].pStructure) ) {
            (yyvsp[0].pStructure)->isClass = (yyvsp[-3].i)==CorS_Class || (yyvsp[-3].i)==CorS_ClassTemplate;
            (yyvsp[0].pStructure)->isTemplate = (yyvsp[-3].i)==CorS_ClassTemplate || (yyvsp[-3].i)==CorS_StructTemplate;
            (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b);
        }
    }
    break;

  case 736: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@48 structure_name $@49 optional_struct_variable_declaration_list  */
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

  case 737: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 738: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 739: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 740: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 741: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "$i" '(' expr ')'  */
                                                                           {
        (yyvsp[-5].pNameWithPosList)->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = (yyvsp[-5].pNameWithPosList);
    }
    break;

  case 742: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 743: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 744: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 745: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 746: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 747: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 748: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 749: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 750: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 751: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 752: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 753: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 754: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 755: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 756: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 757: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 758: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 759: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 760: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 761: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 762: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 763: /* basic_type_declaration: "float16"  */
                        { (yyval.type) = Type::tFloat16; }
    break;

  case 764: /* basic_type_declaration: "half2"  */
                        { (yyval.type) = Type::tHalf2; }
    break;

  case 765: /* basic_type_declaration: "half3"  */
                        { (yyval.type) = Type::tHalf3; }
    break;

  case 766: /* basic_type_declaration: "half4"  */
                        { (yyval.type) = Type::tHalf4; }
    break;

  case 767: /* basic_type_declaration: "half8"  */
                        { (yyval.type) = Type::tHalf8; }
    break;

  case 768: /* basic_type_declaration: "short2"  */
                        { (yyval.type) = Type::tShort2; }
    break;

  case 769: /* basic_type_declaration: "short3"  */
                        { (yyval.type) = Type::tShort3; }
    break;

  case 770: /* basic_type_declaration: "short4"  */
                        { (yyval.type) = Type::tShort4; }
    break;

  case 771: /* basic_type_declaration: "short8"  */
                        { (yyval.type) = Type::tShort8; }
    break;

  case 772: /* basic_type_declaration: "ushort2"  */
                        { (yyval.type) = Type::tUShort2; }
    break;

  case 773: /* basic_type_declaration: "ushort3"  */
                        { (yyval.type) = Type::tUShort3; }
    break;

  case 774: /* basic_type_declaration: "ushort4"  */
                        { (yyval.type) = Type::tUShort4; }
    break;

  case 775: /* basic_type_declaration: "ushort8"  */
                        { (yyval.type) = Type::tUShort8; }
    break;

  case 776: /* basic_type_declaration: "byte2"  */
                        { (yyval.type) = Type::tByte2; }
    break;

  case 777: /* basic_type_declaration: "byte3"  */
                        { (yyval.type) = Type::tByte3; }
    break;

  case 778: /* basic_type_declaration: "byte4"  */
                        { (yyval.type) = Type::tByte4; }
    break;

  case 779: /* basic_type_declaration: "byte8"  */
                        { (yyval.type) = Type::tByte8; }
    break;

  case 780: /* basic_type_declaration: "byte16"  */
                        { (yyval.type) = Type::tByte16; }
    break;

  case 781: /* basic_type_declaration: "ubyte2"  */
                        { (yyval.type) = Type::tUByte2; }
    break;

  case 782: /* basic_type_declaration: "ubyte3"  */
                        { (yyval.type) = Type::tUByte3; }
    break;

  case 783: /* basic_type_declaration: "ubyte4"  */
                        { (yyval.type) = Type::tUByte4; }
    break;

  case 784: /* basic_type_declaration: "ubyte8"  */
                        { (yyval.type) = Type::tUByte8; }
    break;

  case 785: /* basic_type_declaration: "ubyte16"  */
                        { (yyval.type) = Type::tUByte16; }
    break;

  case 786: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 787: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 788: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 789: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 790: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 791: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 792: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 793: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 794: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 795: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 796: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 797: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 798: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 799: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 800: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 801: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 802: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 803: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 804: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 805: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 806: /* bitfield_bits: bitfield_bits semicolon "name"  */
                                                 {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 807: /* bitfield_bits: bitfield_bits ',' "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 808: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<tuple<string,Expression *>>();
        (yyval.pNameExprList) = pSL;

    }
    break;

  case 809: /* bitfield_alias_bits: bitfield_alias_bits semicolon  */
                                            {
        (yyval.pNameExprList) = (yyvsp[-1].pNameExprList);
    }
    break;

  case 810: /* bitfield_alias_bits: bitfield_alias_bits "name" semicolon  */
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

  case 811: /* bitfield_alias_bits: bitfield_alias_bits "name" '=' expr semicolon  */
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

  case 812: /* bitfield_basic_type_declaration: %empty  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 813: /* bitfield_basic_type_declaration: ':' "uint8"  */
                             { (yyval.type) = Type::tBitfield8; }
    break;

  case 814: /* bitfield_basic_type_declaration: ':' "uint16"  */
                             { (yyval.type) = Type::tBitfield16; }
    break;

  case 815: /* bitfield_basic_type_declaration: ':' "uint"  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 816: /* bitfield_basic_type_declaration: ':' "uint64"  */
                             { (yyval.type) = Type::tBitfield64; }
    break;

  case 817: /* bitfield_type_declaration: "bitfield" bitfield_basic_type_declaration '<' '>'  */
                                                                          {
            (yyval.pTypeDecl) = new TypeDecl((yyvsp[-2].type), tokAt(scanner,(yyloc)));
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-2]));
    }
    break;

  case 818: /* $@50: %empty  */
                                                                     { yyextra->das_arrow_depth ++; }
    break;

  case 819: /* $@51: %empty  */
                                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 820: /* bitfield_type_declaration: "bitfield" bitfield_basic_type_declaration '<' $@50 bitfield_bits '>' $@51  */
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

  case 823: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
        (yyval.aTypePair).secondType->at = (yyval.aTypePair).firstType->at;
    }
    break;

  case 824: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 825: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = appendDimExpr(nullptr, (yyvsp[-1].pExpression), tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 826: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = appendDimExpr((yyvsp[-3].pTypeDecl), (yyvsp[-1].pExpression), tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 827: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type), tokAt(scanner,(yyloc))); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 828: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 829: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 830: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 831: /* type_declaration_no_options: type_declaration_no_options dim_list  */
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

  case 832: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyval.pTypeDecl) = appendAutoDim((yyvsp[-2].pTypeDecl), tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 833: /* $@52: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 834: /* $@53: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 835: /* type_declaration_no_options: "type" '<' $@52 type_declaration '>' $@53  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 836: /* type_declaration_no_options: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->typeMacroExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 837: /* type_declaration_no_options: '$' name_in_namespace optional_expr_list_in_braces  */
                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-1]), (yylsp[0]));
        (yyval.pTypeDecl)->typeMacroExpr = sequenceToList((yyvsp[0].pExpression));
        (yyval.pTypeDecl)->typeMacroExpr.insert((yyval.pTypeDecl)->typeMacroExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-1])), *(yyvsp[-1].s)));
        delete (yyvsp[-1].s);
    }
    break;

  case 838: /* $@54: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 839: /* type_declaration_no_options: '$' name_in_namespace '<' $@54 type_declaration_no_options_list '>' optional_expr_list_in_braces  */
                                                                                                                                                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-5]), (yylsp[0]));
        (yyval.pTypeDecl)->typeMacroExpr = typesAndSequenceToList((yyvsp[-2].pTypeDeclList),(yyvsp[0].pExpression));
        (yyval.pTypeDecl)->typeMacroExpr.insert((yyval.pTypeDecl)->typeMacroExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-5])), *(yyvsp[-5].s)));
        delete (yyvsp[-5].s);
    }
    break;

  case 840: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 841: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 842: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 843: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 844: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 845: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 846: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 847: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 848: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 849: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 850: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 851: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 852: /* $@55: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 853: /* $@56: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 854: /* type_declaration_no_options: "smart_ptr" '<' $@55 type_declaration '>' $@56  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 855: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tPointer, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 856: /* $@57: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 857: /* $@58: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 858: /* type_declaration_no_options: "array" '<' $@57 type_declaration '>' $@58  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 859: /* $@59: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 860: /* $@60: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 861: /* type_declaration_no_options: "table" '<' $@59 table_type_pair '>' $@60  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].aTypePair).firstType;
        (yyval.pTypeDecl)->secondType = (yyvsp[-2].aTypePair).secondType;
    }
    break;

  case 862: /* $@61: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 863: /* $@62: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 864: /* type_declaration_no_options: "iterator" '<' $@61 type_declaration '>' $@62  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 865: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = (yyval.pTypeDecl)->at;
    }
    break;

  case 866: /* $@63: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 867: /* $@64: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 868: /* type_declaration_no_options: "block" '<' $@63 type_declaration '>' $@64  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 869: /* $@65: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 870: /* $@66: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 871: /* type_declaration_no_options: "block" '<' $@65 optional_function_argument_list optional_function_type '>' $@66  */
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

  case 872: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = (yyval.pTypeDecl)->at;
    }
    break;

  case 873: /* $@67: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 874: /* $@68: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 875: /* type_declaration_no_options: "function" '<' $@67 type_declaration '>' $@68  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 876: /* $@69: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 877: /* $@70: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 878: /* type_declaration_no_options: "function" '<' $@69 optional_function_argument_list optional_function_type '>' $@70  */
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

  case 879: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pTypeDecl)->firstType = new TypeDecl(Type::tVoid, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->firstType->at = (yyval.pTypeDecl)->at;
    }
    break;

  case 880: /* $@71: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 881: /* $@72: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 882: /* type_declaration_no_options: "lambda" '<' $@71 type_declaration '>' $@72  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 883: /* $@73: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 884: /* $@74: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 885: /* type_declaration_no_options: "lambda" '<' $@73 optional_function_argument_list optional_function_type '>' $@74  */
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

  case 886: /* $@75: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 887: /* $@76: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 888: /* type_declaration_no_options: "tuple" '<' $@75 tuple_type_list '>' $@76  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 889: /* $@77: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 890: /* $@78: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 891: /* type_declaration_no_options: "variant" '<' $@77 variant_type_list '>' $@78  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant, tokAt(scanner,(yyloc)));
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 892: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 893: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 894: /* type_declaration: type_declaration '|' '#'  */
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

  case 895: /* $@79: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 896: /* $@80: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 897: /* $@81: %empty  */
                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 898: /* $@82: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 899: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@79 "name" $@80 open_block $@81 tuple_alias_type_list $@82 close_block  */
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

  case 900: /* $@83: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 901: /* $@84: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 902: /* $@85: %empty  */
                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 903: /* $@86: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 904: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@83 "name" $@84 open_block $@85 variant_alias_type_list $@86 close_block  */
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

  case 905: /* $@87: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 906: /* $@88: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 907: /* $@89: %empty  */
                                                            {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 908: /* $@90: %empty  */
                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-5]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 909: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@87 "name" $@88 bitfield_basic_type_declaration open_block $@89 bitfield_alias_bits $@90 close_block  */
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

  case 910: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 911: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 912: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 913: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 914: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 915: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 916: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 917: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 918: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 919: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 920: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 921: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 922: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = new MakeFieldDecl(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 923: /* make_variant_dim: %empty  */
       {
        (yyval.pExpression) = ast_makeStructToMakeVariant(nullptr, LineInfo());
    }
    break;

  case 924: /* make_variant_dim: make_struct_fields  */
                              {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 925: /* make_struct_single: make_struct_fields optional_comma  */
                                               {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 926: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 927: /* make_struct_dim: make_struct_dim "end of expression" make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 928: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 929: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 930: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 931: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 932: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 933: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 934: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 935: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 948: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 949: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 950: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 951: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = (yyvsp[-2].pTypeDecl);
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 952: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = (yyvsp[-4].pTypeDecl);
        msd->useInitializer = true;
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = msd;
    }
    break;

  case 953: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-5].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 954: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back((yyvsp[-2].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 955: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 956: /* $@91: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 957: /* $@92: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 958: /* make_struct_decl: "struct" '<' $@91 type_declaration_no_options '>' $@92 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 959: /* $@93: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 960: /* $@94: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 961: /* make_struct_decl: "class" '<' $@93 type_declaration_no_options '>' $@94 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                           {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 962: /* $@95: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 963: /* $@96: %empty  */
                                                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 964: /* make_struct_decl: "variant" '<' $@95 variant_type_list '>' $@96 '(' use_initializer make_variant_dim ')'  */
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

  case 965: /* $@97: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 966: /* $@98: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 967: /* make_struct_decl: "default" '<' $@97 type_declaration_no_options '>' $@98 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = (yyvsp[-3].pTypeDecl);
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 968: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 969: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 970: /* make_tuple: make_tuple ',' expr  */
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

  case 971: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 972: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 973: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        (yyval.pExpression) = mkt;
    }
    break;

  case 974: /* $@99: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 975: /* $@100: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 976: /* make_tuple_call: "tuple" '<' $@99 tuple_type_list '>' $@100 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 977: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mka;
    }
    break;

  case 978: /* make_dim: make_dim "end of expression" make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 979: /* make_dim_decl: '[' optional_expr_list ']'  */
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

  case 980: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-2].pTypeDecl);
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 981: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-2].pTypeDecl);
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 982: /* $@101: %empty  */
                                       { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 983: /* $@102: %empty  */
                                                                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 984: /* make_dim_decl: "array" "struct" '<' $@101 type_declaration_no_options '>' $@102 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 985: /* $@103: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 986: /* $@104: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 987: /* make_dim_decl: "array" "tuple" '<' $@103 tuple_type_list '>' $@104 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 988: /* $@105: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 989: /* $@106: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 990: /* make_dim_decl: "array" "variant" '<' $@105 variant_type_list '>' $@106 '(' make_variant_dim ')'  */
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

  case 991: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
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

  case 992: /* $@107: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 993: /* $@108: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 994: /* make_dim_decl: "array" '<' $@107 type_declaration_no_options '>' $@108 '(' optional_expr_list ')'  */
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

  case 995: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 996: /* $@109: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 997: /* $@110: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 998: /* make_dim_decl: "fixed_array" '<' $@109 type_declaration_no_options '>' $@110 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = (yyvsp[-6].pTypeDecl);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 999: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mka;
    }
    break;

  case 1000: /* make_table: make_table "end of expression" make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 1001: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 1002: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 1003: /* make_table_decl: "begin of code block" optional_expr_map_tuple_list "end of code block"  */
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

  case 1004: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
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

  case 1005: /* make_table_decl: "table" '(' optional_expr_map_tuple_list ')'  */
                                                                       {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = new TypeDecl(Type::autoinfer, tokAt(scanner,(yyloc)));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 1006: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 1007: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 1008: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 1009: /* array_comprehension_where: "end of expression" "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 1010: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 1011: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 1012: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 1013: /* array_comprehension: '[' "iterator" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                                 {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 1014: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true,false);
    }
    break;

  case 1015: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where "end of code block" ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),false,false);
    }
    break;

  case 1016: /* array_comprehension: "begin of code block" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                              {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
    }
    break;

  case 1017: /* array_comprehension: "{{" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block" "end of code block"  */
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
