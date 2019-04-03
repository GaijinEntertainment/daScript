#pragma once

namespace das {
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
    char* builtin_string_toupper ( const char *str, Context * context );
    unsigned string_to_uint ( const char *str, Context * context );
    int string_to_int ( const char *str, Context * context );
    float string_to_float ( const char *str, Context * context );
    float fast_to_float ( const char *str );
    int fast_to_int ( const char *str );
    char * to_das_string(const string & str, Context * ctx);
    void set_das_string(string & str, const char * bs);
    void peek_das_string(const string & str, const Block & block, Context * context);
}
