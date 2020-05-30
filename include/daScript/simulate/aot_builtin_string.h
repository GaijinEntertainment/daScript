#pragma once

#include "daScript/ast/ast_typefactory.h"

namespace das {

    char * builtin_build_string ( const TBlock<void,StringBuilderWriter> & block, Context * context );
    vec4f builtin_write_string ( Context & context, SimNode_CallBase * call, vec4f * args );

    char * builtin_string_escape ( const char *str, Context * context );
    char * builtin_string_unescape ( const char *str, Context * context );

    vec4f builtin_strdup ( Context &, SimNode_CallBase * call, vec4f * args );

    int32_t get_character_at ( const char * str, int32_t index, Context * context );

    bool builtin_string_endswith ( const char * str, const char * cmp, Context * context );
    bool builtin_string_startswith ( const char * str, const char * cmp, Context * context );
    char* builtin_string_strip ( const char *str, Context * context );
    char* builtin_string_strip_left ( const char *str, Context * context );
    char* builtin_string_strip_right ( const char *str, Context * context );
    int builtin_string_find1 ( const char *str, const char *substr, int start, Context * context );
    int builtin_string_find2 (const char *str, const char *substr);
    int builtin_string_length ( const char *str, Context * context );
    char* builtin_string_slice1 ( const char *str, int start, int end, Context * context );
    char* builtin_string_slice2 ( const char *str, int start, Context * context );
    char* builtin_string_reverse ( const char *str, Context * context );
    char* builtin_string_tolower ( const char *str, Context * context );
    char* builtin_string_tolower_in_place ( char* str );
    char* builtin_string_toupper ( const char *str, Context * context );
    char* builtin_string_toupper_in_place ( char* str );
    char* builtin_string_chop( const char * str, int start, int length, Context * context );
    char* builtin_as_string(const TArray<uint8_t>& arr, Context* context);
    char* builtin_append_char(char* str, int32_t Ch, Context* context);
    unsigned string_to_uint ( const char *str, Context * context );
    int string_to_int ( const char *str, Context * context );
    float string_to_float ( const char *str, Context * context );
    double string_to_double ( const char *str, Context * context );
    float fast_to_float ( const char *str );
    int fast_to_int ( const char *str );
    char * to_das_string(const string & str, Context * ctx);
    void set_das_string(string & str, const char * bs);
    void peek_das_string(const string & str, const TBlock<void,TTemporary<const char *>> & block, Context * context);
    char * string_repeat ( const char * str, int count, Context * context );
    char * to_string_char(int ch, Context * context);
    void write_string_char(StringBuilderWriter & writer, int32_t ch);
    void write_string_chars(StringBuilderWriter & writer, int32_t ch, int32_t count);
    void builtin_string_split ( const char * str, const char * delim, const Block & sblk, Context * context );
    char * builtin_string_clone ( const char *str, Context * context );
    __forceinline bool builtin_empty(const char* str) { return !str || str[0] == 0; }
    __forceinline bool builtin_empty_das_string(const string & str) { return str.empty(); }

    __forceinline void das_clone ( string & dst, const string & src ) { dst = src; }

    template <typename TT>
    __forceinline char * format ( const char * fmt, TT value, Context * context ) {
        char buf[256];
        snprintf(buf, 256, fmt, value);
        return context->stringHeap.allocateString(buf, uint32_t(strlen(buf)));
    }

    template <typename TT>
    void format_and_write ( StringBuilderWriter & writer, const char * fmt, TT value  ) {
        char buf[256];
        snprintf(buf, 256, fmt, value);
        writer.writeStr(buf, strlen(buf));
    }

    template <typename TT>
    void peek_das_string_T(const string & str, TT && block, Context *) {
        block((char *)str.c_str());
    }

    template <typename TT>
    char * builtin_build_string_T ( TT && block, Context * context ) {
        StringBuilderWriter writer(context->stringHeap);
        block(writer);
        return writer.c_str();
    }

    __forceinline bool is_alpha ( int32_t ch ) { return (ch>='a' && ch<='z') || (ch>='A' && ch<='Z'); }
    __forceinline bool is_white_space ( int32_t ch ) { return  ch==' ' || ch=='\n' || ch=='\r' || ch=='\t'; }
    __forceinline bool is_number ( int32_t ch ) { return (ch>='0' && ch<='9'); }
}
