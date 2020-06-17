#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"

#include "daScript/simulate/aot_builtin_string.h"
#include "daScript/misc/string_writer.h"
#include "daScript/misc/debug_break.h"
#include "daScript/misc/sysos.h"

MAKE_TYPE_FACTORY(StringBuilderWriter, StringBuilderWriter)

namespace das
{
    struct StringBuilderWriterAnnotation : ManagedStructureAnnotation <StringBuilderWriter,false> {
        StringBuilderWriterAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("StringBuilderWriter", ml) {
        }
    };

    int32_t get_character_at ( const char * str, int32_t index, Context * context ) {
        const uint32_t strLen = stringLengthSafe ( *context, str );
        if ( uint32_t(index)>=strLen ) {
            context->throw_error_ex("string character index out of range, %u of %u", uint32_t(index), strLen);
        }
        return ((uint8_t *)str)[index];
    }

    bool builtin_string_endswith ( const char * str, const char * cmp, Context * context ) {
        const uint32_t strLen = stringLengthSafe ( *context, str );
        const uint32_t cmpLen = stringLengthSafe ( *context, cmp );
        return (cmpLen > strLen) ? false : memcmp(&str[strLen - cmpLen], cmp, cmpLen) == 0;
    }

    bool builtin_string_startswith ( const char * str, const char * cmp, Context * context ) {
        const uint32_t strLen = stringLengthSafe ( *context, str );
        const uint32_t cmpLen = stringLengthSafe ( *context, cmp );
        return (cmpLen > strLen) ? false : memcmp(str, cmp, cmpLen) == 0;
    }

    static inline const char* strip_l(const char *str) {
        const char *t = str;
        while (((*t) != '\0') && isspace(*t))
            t++;
        return t;
    }

    static inline const char* strip_r(const char *str, uint32_t len) {
        if (len == 0)
            return str;
        const char *t = &str[len-1];
        while (t >= str && isspace(*t))
            t--;
        return t + 1;
    }

    char* builtin_string_strip ( const char *str, Context * context ) {
        const uint32_t strLen = stringLengthSafe ( *context, str );
        if (!strLen)
            return nullptr;
        const char *start = strip_l(str);
        const char *end = strip_r(str, strLen);
        return end > start ? context->stringHeap.allocateString(start, uint32_t(end-start)) : nullptr;
    }

    char* builtin_string_strip_left ( const char *str, Context * context ) {
        const uint32_t strLen = stringLengthSafe ( *context, str );
        if (!strLen)
            return nullptr;
        const char *start = strip_l(str);
        return uint32_t(start-str) < strLen ? context->stringHeap.allocateString(start, strLen-uint32_t(start-str)) : nullptr;
    }

    char* builtin_string_strip_right ( const char *str, Context * context ) {
        const uint32_t strLen = stringLengthSafe ( *context, str );
        if (!strLen)
            return nullptr;
        const char *end = strip_r(str, strLen);
        return end != str ? context->stringHeap.allocateString(str, uint32_t(end-str)) : nullptr;
    }

    static inline int clamp_int(int v, int minv, int maxv) {
        return (v < minv) ? minv : (v > maxv) ? maxv : v;
    }

    int builtin_string_find1 ( const char *str, const char *substr, int start, Context * context ) {
        const uint32_t strLen = stringLengthSafe ( *context, str );
        if (!strLen)
            return -1;
        const char *ret = strstr(&str[clamp_int(start, 0, strLen)], substr);
        return ret ? int(ret-str) : -1;
    }

    int builtin_string_find2 (const char *str, const char *substr) {
        if (!str)
            return -1;
        const char *ret = strstr(str, substr);
        return ret ? int(ret-str) : -1;
    }

    int builtin_string_length ( const char *str, Context * context ) {
        return stringLengthSafe ( *context, str );
    }

    char* builtin_string_chop(const char* str, int start, int length, Context* context) {
        if ( !str ) return nullptr;
        return context->stringHeap.allocateString(str + start, length);
    }

    char* builtin_string_slice1 ( const char *str, int start, int end, Context * context ) {
        const uint32_t strLen = stringLengthSafe ( *context, str );
        if (!strLen)
            return nullptr;
        start = clamp_int((start < 0) ? (strLen + start) : start, 0, strLen);
        end = clamp_int((end < 0) ? (strLen + end) : end, 0, strLen);
        return end > start ? context->stringHeap.allocateString(str + start, uint32_t(end-start)) : nullptr;
    }

    char* builtin_string_slice2 ( const char *str, int start, Context * context ) {
        const uint32_t strLen = stringLengthSafe ( *context, str );
        if (!strLen)
            return nullptr;
        start = clamp_int((start < 0) ? (strLen + start) : start, 0, strLen);
        return strLen > uint32_t(start) ? context->stringHeap.allocateString(str + start, uint32_t(strLen-start)) : nullptr;
    }

    char* builtin_string_reverse ( const char *str, Context * context ) {
        const uint32_t strLen = stringLengthSafe ( *context, str );
        if (!strLen)
            return nullptr;
        char * ret = context->stringHeap.allocateString(str, strLen);
        str += strLen-1;
        for (char *d = ret, *end = ret + strLen; d != end; --str, ++d)
          *d = *str;
        return ret;
    }

    __forceinline char to_lower(char ch) {
        return (ch >= 'A' && ch <= 'Z') ? (ch - 'A' + 'a') : ch;
    }

    char* builtin_string_tolower ( const char *str, Context * context ) {
        const uint32_t strLen = stringLengthSafe ( *context, str );
        if (!strLen)
            return nullptr;
        char * ret = context->stringHeap.allocateString(str, strLen);
        for (char *d = ret, *end = ret + strLen; d != end; ++str, ++d)
          *d = (char)to_lower(*str);
        return ret;
    }

    char* builtin_string_tolower_in_place(char* str) {
        if (!str) return nullptr;
        char* pch = str;
        for (;;) {
            char ch = *pch;
            if (ch == 0) break;
            else if (ch >= 'A' && ch <= 'Z') *pch = ch - 'A' + 'a';
            pch++;
        }
        return str;
    }

    __forceinline char to_upper(char ch) {
        return (ch >= 'a' && ch <= 'z') ? (ch - 'a' + 'A') : ch;
    }

    char* builtin_string_toupper ( const char *str, Context * context ) {
        const uint32_t strLen = stringLengthSafe ( *context, str );
        if (!strLen)
            return nullptr;
        char * ret = context->stringHeap.allocateString(str, strLen);
        for (char *d = ret, *end = ret + strLen; d != end; ++str, ++d)
          *d = (char)to_upper(*str);
        return ret;
    }

    char* builtin_string_toupper_in_place ( char* str ) {
        if (!str) return nullptr;
        char* pch = str;
        for (;;) {
            char ch = *pch;
            if (ch == 0) break;
            else if (ch >= 'a' && ch <= 'z') *pch = ch - 'a' + 'A';
            pch++;
        }
        return str;
    }


    unsigned string_to_uint ( const char *str, Context * context ) {
        char *endptr;
        unsigned long int ret = strtoul(str, &endptr, 10);
        const uint32_t strLen = stringLengthSafe ( *context, str );
        if (endptr != str + strLen || strLen == 0)
        {
            context->throw_error("string-to-uint conversion failed. String is not an uint number");
            return 0;
        }
        return ret;
    }

    int string_to_int ( const char *str, Context * context ) {
        char *endptr;
        long int ret = strtol(str, &endptr, 10);
        const uint32_t strLen = stringLengthSafe ( *context, str );
        if (endptr != str + strLen || strLen == 0)
        {
            context->throw_error("string-to-int conversion failed. String is not an integer number");
            return 0;
        }
        return ret;
    }

    float string_to_float ( const char *str, Context * context ) {
        char *endptr;
        float ret = strtof(str, &endptr);
        const uint32_t strLen = stringLengthSafe ( *context, str );
        if (endptr != str + strLen || strLen == 0)
        {
            context->throw_error("string-to-float conversion failed. String is not an float number");
            return 0.f;
        }
        return ret;
    }

    double string_to_double ( const char *str, Context * context ) {
        char *endptr;
        double ret = strtod(str, &endptr);
        const uint32_t strLen = stringLengthSafe ( *context, str );
        if (endptr != str + strLen || strLen == 0)
        {
            context->throw_error("string-to-dobule conversion failed. String is not an dobule number");
            return 0.f;
        }
        return ret;
    }

    float fast_to_float ( const char *str ) {
        return str ? (float)atof(str) : 0.f;
    }

    int fast_to_int ( const char *str ) {
        return str ? atoi(str) : 0;
    }

    char * to_das_string(const string & str, Context * ctx) {
        return ctx->stringHeap.allocateString(str);
    }

    void set_das_string(string & str, const char * bs) {
        str = bs ? bs : "";
    }

    char * builtin_build_string ( const TBlock<void,StringBuilderWriter> & block, Context * context ) {
        StringBuilderWriter writer(context->stringHeap);
        vec4f args[1];
        args[0] = cast<StringBuilderWriter *>::from(&writer);
        context->invoke(block, args, nullptr);
        return writer.c_str();
    }

    vec4f builtin_write_string ( Context &, SimNode_CallBase * call, vec4f * args ) {
        StringBuilderWriter * writer = cast<StringBuilderWriter *>::to(args[0]);
        DebugDataWalker<StringBuilderWriter> walker(*writer, PrintFlags::string_builder);
        walker.walk(args[1], call->types[1]);
        return v_zero();
    }

    void write_string_char ( StringBuilderWriter & writer, int32_t ch ) {
        char buf[2];
        buf[0] = char(ch);
        buf[1] = 0;
        writer.writeStr(buf, 1);
    }

    void write_string_chars ( StringBuilderWriter & writer, int32_t ch, int32_t count ) {
        if ( count >0 ) writer.writeChars(char(ch), count);
    }

    void write_escape_string ( StringBuilderWriter & writer, char * str ) {
        if ( !str ) return;
        auto estr = escapeString(str,false);
        writer.writeStr(estr.c_str(), estr.length());
    }

    char * to_string_char ( int ch, Context * context ) {
        auto st = context->stringHeap.allocateString(nullptr, 1);
        *st = char(ch);
        return st;
    }

    char * string_repeat ( const char * str, int count, Context * context ) {
        uint32_t len = stringLengthSafe ( *context, str );
        if ( !len ) return nullptr;
        char * res = context->stringHeap.allocateString(nullptr, len * count);
        for ( char * s = res; count; count--, s+=len ) {
            memcpy ( s, str, len );
        }
        return res;
    }

    void peek_das_string(const string & str, const TBlock<void,TTemporary<const char *>> & block, Context * context) {
        vec4f args[1];
        args[0] = cast<const char *>::from(str.c_str());
        context->invoke(block, args, nullptr);
    }

    void builtin_string_split_by_char ( const char * str, const char * delim, const Block & block, Context * context ) {
        if ( !str ) str = "";
        if ( !delim ) delim = "";
        vector<const char *> tokens;
        vector<string> words;
        const char * ch = str;
        auto delimLen = stringLengthSafe(*context,delim);
        if ( delimLen ) {
            while ( *ch ) {
                const char * tok = ch;
                while ( *ch && !strchr(delim,*ch) ) ch++;
                words.push_back(string(tok,ch-tok));
                if ( !*ch ) break;
                while ( *ch && strchr(delim,*ch) ) ch++;
                if ( !*ch ) words.push_back("");
            }
        } else {
            auto len = stringLengthSafe(*context,str);
            words.reserve(len);
            while ( *ch ) {
                words.push_back(string(1,*ch));
                ch ++;
            }
        }
        tokens.reserve(words.size());
        for ( auto & tok : words ) {
            tokens.push_back(tok.c_str());
        }
        if ( tokens.empty() ) tokens.push_back("");
        Array arr;
        arr.data = (char *) tokens.data();
        arr.capacity = arr.size = uint32_t(tokens.size());
        arr.lock = 1;
        vec4f args[1];
        args[0] = cast<Array *>::from(&arr);
        context->invoke(block, args, nullptr);
    }

    void builtin_string_split ( const char * str, const char * delim, const Block & block, Context * context ) {
        if ( !str ) str = "";
        if ( !delim ) delim = "";
        vector<const char *> tokens;
        vector<string> words;
        const char * ch = str;
        auto delimLen = stringLengthSafe(*context,delim);
        if ( delimLen ) {
            while ( *ch ) {
                const char * tok = ch;
                while ( *ch && memcmp(delim,ch,delimLen)!=0 ) ch++;
                words.push_back(string(tok,ch-tok));
                if ( !*ch ) break;
                while ( *ch && memcmp(delim,ch,delimLen)==0 ) ch+=delimLen;
                if ( !*ch ) words.push_back("");
            }
        } else {
            auto len = stringLengthSafe(*context,str);
            words.reserve(len);
            while ( *ch ) {
                words.push_back(string(1,*ch));
                ch ++;
            }
        }
        tokens.reserve(words.size());
        for ( auto & tok : words ) {
            tokens.push_back(tok.c_str());
        }
        if ( tokens.empty() ) tokens.push_back("");
        Array arr;
        arr.data = (char *) tokens.data();
        arr.capacity = arr.size = uint32_t(tokens.size());
        arr.lock = 1;
        vec4f args[1];
        args[0] = cast<Array *>::from(&arr);
        context->invoke(block, args, nullptr);
    }

    char * builtin_string_replace ( const char * str, const char * toSearch, const char * replaceStr, Context * context ) {
        auto toSearchSize = stringLengthSafe(*context, toSearch);
        if ( !toSearchSize ) return (char *) str;
        string data = str ? str : "";
        auto replaceStrSize = stringLengthSafe(*context,replaceStr);
        const char * repl = replaceStr ? replaceStr : "";
        const char * toss = toSearch ? toSearch : "";
        size_t pos = data.find(toss);
        while ( pos != string::npos ) {
            data.replace(pos, toSearchSize, repl);
            pos = data.find(toss, pos + replaceStrSize);
        }
        return context->stringHeap.allocateString(data);
    }

    char * builtin_string_clone ( const char *str, Context * context ) {
        const uint32_t strLen = stringLengthSafe ( *context, str );
        if (!strLen)
            return nullptr;
        return context->stringHeap.allocateString(str, strLen);
    }

    class StrdupDataWalker : public DataWalker {
        virtual void String ( char * & str ) {
            if (str) str = strdup(str);
        }
    };

    vec4f builtin_strdup ( Context &, SimNode_CallBase * call, vec4f * args ) {
        StrdupDataWalker walker;
        walker.walk(args[0], call->types[0]);
        return v_zero();
    }

    char* builtin_as_string(const TArray<uint8_t>& arr, Context* context) {
        return arr.size ? context->stringHeap.allocateString(arr.data, arr.size) : nullptr;
    }

    char* builtin_append_char(char* str, int32_t Ch, Context* context) {
        if ( !str ) {
            StringHeader* header = (StringHeader *) context->stringHeap.allocate(sizeof(StringHeader) + 2);
            header->length = 1;
            header->hash = 0;
#if DAS_TRACK_ALLOCATIONS
            if ( g_tracker_string==g_breakpoint_string ) os_debug_break();
            header->tracking_id = g_tracker_string ++;
#endif
            str = (char*)(header + 1);
            str[0] = (char) Ch;
            str[1] = 0;
            return str;
        } else if ( context->stringHeap.isOwnPtr(str) ) {
            char* hstr = str - sizeof(StringHeader);
            StringHeader* header = (StringHeader*)hstr;
            uint32_t length = header->length;
            uint32_t size = sizeof(StringHeader) + length + 1;
            char* nstr = context->stringHeap.reallocate(hstr, size, size + 1);
            if (nstr != hstr) {
                header = (StringHeader*)nstr;
#if DAS_TRACK_ALLOCATIONS
                if ( g_tracker_string==g_breakpoint_string ) os_debug_break();
                header->tracking_id = g_tracker_string ++;
#endif
            }
            header->length = length + 1;
            nstr += sizeof(StringHeader);
            nstr[length] = (char) Ch;
            nstr[length + 1] = 0;
            return nstr;
        } else {
            uint32_t length = uint32_t(strlen(str));
            char* nstr = context->stringHeap.allocateString(nullptr, length + 1);
            memcpy(nstr, str, length);
            nstr[length] = (char) Ch;
            nstr[length + 1] = 0;
            return nstr;
        }
    }

    char * builtin_string_escape ( const char *str, Context * context ) {
        if ( !str ) return nullptr;
        return context->stringHeap.allocateString(escapeString(str,false));
    }

    char * builtin_string_unescape ( const char *str, Context * context ) {
        if ( !str ) return nullptr;
        bool err = false;
        auto estr = unescapeString(str, &err, false);
        if ( err ) context->throw_error("invalid escape sequence");
        return context->stringHeap.allocateString(estr);
    }

    int builtin_find_first_of ( const char * str, const char * substr, Context * context ) {
        uint32_t strlen = stringLengthSafe ( *context, str );
        uint32_t substrlen = stringLengthSafe ( *context, str );
        for ( uint32_t o=0; o!=strlen; ++o ) {
            auto ch = str[o];
            for ( uint32_t s=0; s!=substrlen; ++s ) {
                if ( ch == substr[s] ) return int(o);
            }
        }
        return -1;
    }

    int builtin_find_first_char_of ( const char * str, int Ch, Context * context ) {
        uint32_t strlen = stringLengthSafe ( *context, str );
        for ( uint32_t o=0; o!=strlen; ++o ) {
            if ( str[o]==Ch ) {
                return o;
            }
        }
        return -1;
    }

    char * builtin_string_from_array ( const TArray<uint8_t> & bytes, Context * context ) {
        if ( !bytes.size ) return nullptr;
        return context->stringHeap.allocateString(bytes.data, bytes.size);
    }

    char * builtin_das_root ( Context * context ) {
        return context->stringHeap.allocateString(getDasRoot());
    }

    void delete_string ( char * & str, Context * context ) {
        if ( !str ) return;
        uint32_t len = stringLengthSafe(*context, str);
        context->stringHeap.freeString(str, len);
        str = nullptr;
    }

    void builtin_append_char_to_string(string & str, int32_t Ch) {
        str += char(Ch);
    }

    bool builtin_string_ends_with(const string &str, char * substr, Context * context ) {
        if ( substr==nullptr ) return false;
        auto sz = str.length();
        auto slen = stringLengthSafe(*context,substr);
        if ( slen>sz ) return false;
        return memcmp ( str.data() + sz - slen, substr, slen )==0;
    }

    int32_t builtin_ext_string_length(string & str) {
        return int32_t(str.size());
    }
    void builtin_resize_string(string & str, int32_t newLength) {
        str.resize(newLength);
    }

    void Module_BuiltIn::addString(ModuleLibrary & lib) {
        // string builder writer
        addAnnotation(make_smart<StringBuilderWriterAnnotation>(lib));
        addExtern<DAS_BIND_FUN(builtin_das_root)>(*this, lib, "get_das_root",
            SideEffects::accessExternal,"builtin_das_root");
        addExtern<DAS_BIND_FUN(delete_string)>(*this, lib, "delete_string",
            SideEffects::modifyExternal,"delete_string")->unsafeOperation = true;
        addExtern<DAS_BIND_FUN(builtin_build_string)>(*this, lib, "build_string",
            SideEffects::modifyExternal,"builtin_build_string_T")->setAotTemplate();
        addInterop<builtin_write_string,void,StringBuilderWriter,vec4f> (*this, lib, "write",
            SideEffects::modifyExternal, "builtin_write_string");
        addExtern<DAS_BIND_FUN(write_string_char)>(*this, lib, "write_char", SideEffects::modifyExternal, "write_string_char");
        addExtern<DAS_BIND_FUN(write_string_chars)>(*this, lib, "write_chars", SideEffects::modifyExternal, "write_string_chars");
        addExtern<DAS_BIND_FUN(write_escape_string)>(*this, lib, "write_escape_string", SideEffects::modifyExternal, "write_escape_string");
        addExtern<DAS_BIND_FUN(format_and_write<int32_t>)> (*this, lib, "format", SideEffects::modifyExternal, "format_and_write<int32_t>");
        addExtern<DAS_BIND_FUN(format_and_write<uint32_t>)>(*this, lib, "format", SideEffects::modifyExternal, "format_and_write<uint32_t>");
        addExtern<DAS_BIND_FUN(format_and_write<int64_t>)> (*this, lib, "format", SideEffects::modifyExternal, "format_and_write<int64_t>");
        addExtern<DAS_BIND_FUN(format_and_write<uint64_t>)>(*this, lib, "format", SideEffects::modifyExternal, "format_and_write<uint64_t>");
        addExtern<DAS_BIND_FUN(format_and_write<float>)>   (*this, lib, "format", SideEffects::modifyExternal, "format_and_write<float>");
        addExtern<DAS_BIND_FUN(format_and_write<double>)>  (*this, lib, "format", SideEffects::modifyExternal, "format_and_write<double>");
        addExtern<DAS_BIND_FUN(builtin_string_from_array)>(*this, lib, "string", SideEffects::none, "builtin_string_from_array");
        // dup
        auto bsd = addInterop<builtin_strdup,void,vec4f> (*this, lib, "builtin_strdup",
                                                          SideEffects::modifyArgumentAndExternal, "builtin_strdup");
        bsd->unsafeOperation = true;
        // das string binding
        addAnnotation(make_smart<DasStringTypeAnnotation>());
        addExtern<DAS_BIND_FUN(to_das_string)>(*this, lib, "string", SideEffects::none, "to_das_string");
        addExtern<DAS_BIND_FUN(set_das_string)>(*this, lib, "set", SideEffects::modifyArgument,"set_das_string");
        addExtern<DAS_BIND_FUN(peek_das_string)>(*this, lib, "peek",
            SideEffects::modifyExternal,"peek_das_string_T")->setAotTemplate();
        // regular string
        addExtern<DAS_BIND_FUN(get_character_at)>(*this, lib, "character_at", SideEffects::none, "get_character_at");
        addExtern<DAS_BIND_FUN(get_character_uat)>(*this, lib, "character_uat", SideEffects::none, "get_character_uat")->unsafeOperation = true;
        addExtern<DAS_BIND_FUN(string_repeat)>(*this, lib, "repeat", SideEffects::none, "string_repeat");
        addExtern<DAS_BIND_FUN(to_string_char)>(*this, lib, "to_char", SideEffects::none, "to_string_char");
        addExtern<DAS_BIND_FUN(builtin_string_endswith)>(*this, lib, "ends_with", SideEffects::none, "builtin_string_endswith");
        addExtern<DAS_BIND_FUN(builtin_string_startswith)>(*this, lib, "starts_with", SideEffects::none, "builtin_string_startswith");
        addExtern<DAS_BIND_FUN(builtin_string_strip)>(*this, lib, "strip", SideEffects::none, "builtin_string_strip");
        addExtern<DAS_BIND_FUN(builtin_string_strip_right)>(*this, lib, "strip_right", SideEffects::none, "builtin_string_strip_right");
        addExtern<DAS_BIND_FUN(builtin_string_strip_left)>(*this, lib, "strip_left", SideEffects::none, "builtin_string_strip_left");
        addExtern<DAS_BIND_FUN(builtin_string_chop)>(*this, lib, "chop",
            SideEffects::none, "builtin_string_chop")->unsafeOperation = true;
        addExtern<DAS_BIND_FUN(builtin_as_string)>(*this, lib, "as_string", SideEffects::none, "builtin_as_string");
        addExtern<DAS_BIND_FUN(builtin_string_slice1)>(*this, lib, "slice", SideEffects::none, "builtin_string_slice1");
        addExtern<DAS_BIND_FUN(builtin_string_slice2)>(*this, lib, "slice", SideEffects::none, "builtin_string_slice2");
        addExtern<DAS_BIND_FUN(builtin_string_find1)>(*this, lib, "find", SideEffects::none, "builtin_string_find1");
        addExtern<DAS_BIND_FUN(builtin_string_find2)>(*this, lib, "find", SideEffects::none, "builtin_string_find2");
        addExtern<DAS_BIND_FUN(builtin_find_first_of)>(*this, lib, "find_first_of", SideEffects::none, "builtin_find_first_of");
        addExtern<DAS_BIND_FUN(builtin_find_first_char_of)>(*this, lib, "find_first_of", SideEffects::none, "builtin_find_first_char_of");
        addExtern<DAS_BIND_FUN(builtin_string_length)>(*this, lib, "length", SideEffects::none, "builtin_string_length");
        addExtern<DAS_BIND_FUN(builtin_string_reverse)>(*this, lib, "reverse", SideEffects::none, "builtin_string_reverse");
        addExtern<DAS_BIND_FUN(builtin_append_char)>(*this, lib, "append",
            SideEffects::modifyArgumentAndExternal, "builtin_append_char")->unsafeOperation = true;
        addExtern<DAS_BIND_FUN(builtin_append_char_to_string)>(*this, lib, "append",
            SideEffects::modifyArgumentAndExternal, "builtin_append_char_to_string");
        addExtern<DAS_BIND_FUN(builtin_string_ends_with)>(*this, lib, "ends_with",
            SideEffects::none, "builtin_string_ends_with");
        addExtern<DAS_BIND_FUN(builtin_resize_string)>(*this, lib, "resize",
            SideEffects::modifyArgumentAndExternal, "builtin_resize_string");
        addExtern<DAS_BIND_FUN(builtin_ext_string_length)>(*this, lib, "length",
            SideEffects::none, "builtin_ext_string_length");
        addExtern<DAS_BIND_FUN(builtin_string_toupper)>(*this, lib, "to_upper", SideEffects::none, "builtin_string_toupper");
        addExtern<DAS_BIND_FUN(builtin_string_tolower)>(*this, lib, "to_lower", SideEffects::none, "builtin_string_tolower");
        addExtern<DAS_BIND_FUN(builtin_empty)>(*this, lib, "empty", SideEffects::none, "builtin_empty");
        addExtern<DAS_BIND_FUN(builtin_empty_das_string)>(*this, lib, "empty", SideEffects::none, "builtin_empty_das_string");
        addExtern<DAS_BIND_FUN(builtin_string_tolower_in_place)>(*this, lib, "to_lower_in_place",
            SideEffects::none, "builtin_string_tolower_in_place")->unsafeOperation = true;
        addExtern<DAS_BIND_FUN(builtin_string_toupper_in_place)>(*this, lib, "to_upper_in_place",
            SideEffects::none, "builtin_string_toupper_in_place")->unsafeOperation = true;
        addExtern<DAS_BIND_FUN(builtin_string_split_by_char)>(*this, lib, "builtin_string_split_by_char",
            SideEffects::modifyExternal, "builtin_string_split_by_char");
        addExtern<DAS_BIND_FUN(builtin_string_split)>(*this, lib, "builtin_string_split",
            SideEffects::modifyExternal, "builtin_string_split");
        addExtern<DAS_BIND_FUN(builtin_string_clone)>(*this, lib, "clone_string", SideEffects::none, "builtin_string_clone");
        addExtern<DAS_BIND_FUN(string_to_int)>(*this, lib, "int", SideEffects::none, "string_to_int");
        addExtern<DAS_BIND_FUN(string_to_uint)>(*this, lib, "uint", SideEffects::none, "string_to_uint");
        addExtern<DAS_BIND_FUN(string_to_float)>(*this, lib, "float", SideEffects::none, "string_to_float");
        addExtern<DAS_BIND_FUN(string_to_double)>(*this, lib, "double", SideEffects::none, "string_to_double");
        addExtern<DAS_BIND_FUN(fast_to_int)>(*this, lib, "to_int", SideEffects::none, "fast_to_int");
        addExtern<DAS_BIND_FUN(fast_to_float)>(*this, lib, "to_float", SideEffects::none, "fast_to_float");
        addExtern<DAS_BIND_FUN(builtin_string_escape)>(*this, lib, "escape", SideEffects::none, "builtin_string_escape");
        addExtern<DAS_BIND_FUN(builtin_string_unescape)>(*this, lib, "unescape", SideEffects::none, "builtin_string_unescape");
        addExtern<DAS_BIND_FUN(builtin_string_replace)>(*this, lib, "replace", SideEffects::none, "builtin_string_replace");
        // format
        addExtern<DAS_BIND_FUN(format<int32_t>)> (*this, lib, "format", SideEffects::none, "format<int32_t>");
        addExtern<DAS_BIND_FUN(format<uint32_t>)>(*this, lib, "format", SideEffects::none, "format<uint32_t>");
        addExtern<DAS_BIND_FUN(format<int64_t>)> (*this, lib, "format", SideEffects::none, "format<int64_t>");
        addExtern<DAS_BIND_FUN(format<uint64_t>)>(*this, lib, "format", SideEffects::none, "format<uint64_t>");
        addExtern<DAS_BIND_FUN(format<float>)>   (*this, lib, "format", SideEffects::none, "format<float>");
        addExtern<DAS_BIND_FUN(format<double>)>  (*this, lib, "format", SideEffects::none, "format<double>");
        // queries
        addExtern<DAS_BIND_FUN(is_alpha)> (*this, lib, "is_alpha", SideEffects::none, "is_alpha");
        addExtern<DAS_BIND_FUN(is_new_line)> (*this, lib, "is_new_line", SideEffects::none, "is_new_line");
        addExtern<DAS_BIND_FUN(is_white_space)> (*this, lib, "is_white_space", SideEffects::none, "is_white_space");
        addExtern<DAS_BIND_FUN(is_number)> (*this, lib, "is_number", SideEffects::none, "is_number");
        // bitset helpers
        addExtern<DAS_BIND_FUN(is_char_in_set)>(*this, lib, "is_char_in_set",
            SideEffects::none,"is_char_in_set");
    }
}
