#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"

#include "daScript/simulate/aot_builtin_string.h"
#include "daScript/misc/string_writer.h"

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
        if ( index<0 || index>=strLen ) {
            context->throw_error("string character index out of range");
        }
        return str[index];
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
        return end > start ? context->heap.allocateString(start, uint32_t(end-start)) : nullptr;
    }

    char* builtin_string_strip_left ( const char *str, Context * context ) {
        const uint32_t strLen = stringLengthSafe ( *context, str );
        if (!strLen)
            return nullptr;
        const char *start = strip_l(str);
        return start-str < strLen ? context->heap.allocateString(start, strLen-uint32_t(start-str)) : nullptr;
    }

    char* builtin_string_strip_right ( const char *str, Context * context ) {
        const uint32_t strLen = stringLengthSafe ( *context, str );
        if (!strLen)
            return nullptr;
        const char *end = strip_r(str, strLen);
        return end != str ? context->heap.allocateString(str, uint32_t(end-str)) : nullptr;
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

    char* builtin_string_slice1 ( const char *str, int start, int end, Context * context ) {
        const uint32_t strLen = stringLengthSafe ( *context, str );
        if (!strLen)
            return nullptr;
        start = clamp_int((start < 0) ? (strLen + start) : start, 0, strLen);
        end = clamp_int((end < 0) ? (strLen + end) : end, 0, strLen);
        return end > start ? context->heap.allocateString(str + start, uint32_t(end-start)) : nullptr;
    }

    char* builtin_string_slice2 ( const char *str, int start, Context * context ) {
        const uint32_t strLen = stringLengthSafe ( *context, str );
        if (!strLen)
            return nullptr;
        start = clamp_int((start < 0) ? (strLen + start) : start, 0, strLen);
        return strLen > uint32_t(start) ? context->heap.allocateString(str + start, uint32_t(strLen-start)) : nullptr;
    }

    char* builtin_string_reverse ( const char *str, Context * context ) {
        const uint32_t strLen = stringLengthSafe ( *context, str );
        if (!strLen)
            return nullptr;
        char * ret = context->heap.allocateString(str, strLen);
        str += strLen-1;
        for (char *d = ret, *end = ret + strLen; d != end; --str, ++d)
          *d = *str;
        return ret;
    }

    char* builtin_string_tolower ( const char *str, Context * context ) {
        const uint32_t strLen = stringLengthSafe ( *context, str );
        if (!strLen)
            return nullptr;
        char * ret = context->heap.allocateString(str, strLen);
        for (char *d = ret, *end = ret + strLen; d != end; ++str, ++d)
          *d = (char)tolower(*str);
        return ret;
    }

    char* builtin_string_toupper ( const char *str, Context * context ) {
        const uint32_t strLen = stringLengthSafe ( *context, str );
        if (!strLen)
            return nullptr;
        char * ret = context->heap.allocateString(str, strLen);
        for (char *d = ret, *end = ret + strLen; d != end; ++str, ++d)
          *d = (char)toupper(*str);
        return ret;
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

    float fast_to_float ( const char *str ) {
        return str ? (float)atof(str) : 0.f;
    }

    int fast_to_int ( const char *str ) {
        return str ? atoi(str) : 0;
    }

    char * to_das_string(const string & str, Context * ctx) {
        return ctx->heap.allocateString(str);
    }

    void set_das_string(string & str, const char * bs) {
        str = bs ? bs : "";
    }

    void peek_das_string(const string & str, const Block & block, Context * context) {
        vec4f args[1];
        args[0] = cast<const char *>::from(str.c_str());
        context->invoke(block, args, nullptr);
    }

    char * builtin_build_string ( const Block & block, Context * context ) {
        StringBuilderWriter writer(context->heap);
        vec4f args[1];
        args[0] = cast<StringBuilderWriter *>::from(&writer);
        context->invoke(block, args, nullptr);
        auto pStr = writer.c_str();
        if ( !pStr ) {
            context->throw_error("can't allocate string builder result, out of heap");
        }
        return pStr;
    }

    vec4f builtin_write_string ( Context & context, SimNode_CallBase * call, vec4f * args ) {
        StringBuilderWriter * writer = cast<StringBuilderWriter *>::to(args[0]);
        DebugDataWalker<StringBuilderWriter> walker(*writer, PrintFlags::string_builder);
        walker.walk(args[1], call->types[1]);
        return v_zero();
    }

    void Module_BuiltIn::addString(ModuleLibrary & lib) {
        // string builder writer
        addAnnotation(make_shared<StringBuilderWriterAnnotation>(lib));
        addExtern<DAS_BIND_FUN(builtin_build_string)>(*this, lib, "_builtin_build_string", SideEffects::modifyExternal,"builtin_build_string");
        addInterop<builtin_write_string,void,StringBuilderWriter,vec4f> (*this, lib, "write",
            SideEffects::modifyExternal, "builtin_write_string");
        addExtern<DAS_BIND_FUN(format_and_write<int32_t>)> (*this, lib, "format", SideEffects::modifyExternal, "format_and_write<int32_t>");
        addExtern<DAS_BIND_FUN(format_and_write<uint32_t>)>(*this, lib, "format", SideEffects::modifyExternal, "format_and_write<uint32_t>");
        addExtern<DAS_BIND_FUN(format_and_write<int64_t>)> (*this, lib, "format", SideEffects::modifyExternal, "format_and_write<int64_t>");
        addExtern<DAS_BIND_FUN(format_and_write<uint64_t>)>(*this, lib, "format", SideEffects::modifyExternal, "format_and_write<uint64_t>");
        addExtern<DAS_BIND_FUN(format_and_write<float>)>   (*this, lib, "format", SideEffects::modifyExternal, "format_and_write<float>");
        addExtern<DAS_BIND_FUN(format_and_write<double>)>  (*this, lib, "format", SideEffects::modifyExternal, "format_and_write<double>");
        // das string binding
        addAnnotation(make_shared<DasStringTypeAnnotation>());
        addExtern<DAS_BIND_FUN(to_das_string)>(*this, lib, "string", SideEffects::none, "to_das_string");
        addExtern<DAS_BIND_FUN(set_das_string)>(*this, lib, "set", SideEffects::modifyArgument,"set_das_string");
        addExtern<DAS_BIND_FUN(peek_das_string)>(*this, lib, "_builtin_peek", SideEffects::none,"peek_das_string");
        // regular string
        addExtern<DAS_BIND_FUN(get_character_at)>(*this, lib, "characterat", SideEffects::none, "get_character_at");
        addExtern<DAS_BIND_FUN(builtin_string_endswith)>(*this, lib, "endswith", SideEffects::none, "builtin_string_endswith");
        addExtern<DAS_BIND_FUN(builtin_string_startswith)>(*this, lib, "startswith", SideEffects::none, "builtin_string_startswith");
        addExtern<DAS_BIND_FUN(builtin_string_strip)>(*this, lib, "strip", SideEffects::none, "builtin_string_strip");
        addExtern<DAS_BIND_FUN(builtin_string_strip_right)>(*this, lib, "strip_right", SideEffects::none, "builtin_string_strip_right");
        addExtern<DAS_BIND_FUN(builtin_string_strip_left)>(*this, lib, "strip_left", SideEffects::none, "builtin_string_strip_left");
        addExtern<DAS_BIND_FUN(builtin_string_slice1)>(*this, lib, "slice", SideEffects::none, "builtin_string_slice1");
        addExtern<DAS_BIND_FUN(builtin_string_slice2)>(*this, lib, "slice", SideEffects::none, "builtin_string_slice2");
        addExtern<DAS_BIND_FUN(builtin_string_find1)>(*this, lib, "find", SideEffects::none, "builtin_string_find1");
        addExtern<DAS_BIND_FUN(builtin_string_find2)>(*this, lib, "find", SideEffects::none, "builtin_string_find2");
        addExtern<DAS_BIND_FUN(builtin_string_length)>(*this, lib, "length", SideEffects::none, "builtin_string_length");
        addExtern<DAS_BIND_FUN(builtin_string_reverse)>(*this, lib, "reverse", SideEffects::none, "builtin_string_reverse");
        addExtern<DAS_BIND_FUN(builtin_string_toupper)>(*this, lib, "toupper", SideEffects::none, "builtin_string_toupper");
        addExtern<DAS_BIND_FUN(builtin_string_tolower)>(*this, lib, "tolower", SideEffects::none, "builtin_string_tolower");
        addExtern<DAS_BIND_FUN(string_to_int)>(*this, lib, "int", SideEffects::none, "string_to_int");
        addExtern<DAS_BIND_FUN(string_to_uint)>(*this, lib, "uint", SideEffects::none, "string_to_uint");
        addExtern<DAS_BIND_FUN(string_to_float)>(*this, lib, "float", SideEffects::none, "string_to_float");
        addExtern<DAS_BIND_FUN(fast_to_int)>(*this, lib, "to_int", SideEffects::none, "fast_to_int");
        addExtern<DAS_BIND_FUN(fast_to_float)>(*this, lib, "to_float", SideEffects::none, "fast_to_float");
        // format
        addExtern<DAS_BIND_FUN(format<int32_t>)> (*this, lib, "format", SideEffects::none, "format<int32_t>");
        addExtern<DAS_BIND_FUN(format<uint32_t>)>(*this, lib, "format", SideEffects::none, "format<uint32_t>");
        addExtern<DAS_BIND_FUN(format<int64_t>)> (*this, lib, "format", SideEffects::none, "format<int64_t>");
        addExtern<DAS_BIND_FUN(format<uint64_t>)>(*this, lib, "format", SideEffects::none, "format<uint64_t>");
        addExtern<DAS_BIND_FUN(format<float>)>   (*this, lib, "format", SideEffects::none, "format<float>");
        addExtern<DAS_BIND_FUN(format<double>)>  (*this, lib, "format", SideEffects::none, "format<double>");
    }
}
