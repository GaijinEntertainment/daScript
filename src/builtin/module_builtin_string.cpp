#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/ast/ast_interop.h"

#include "daScript/simulate/runtime_array.h"

namespace das
{
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

    static const char* strip_l(const char *str)
    {
        const char *t = str;
        while (((*t) != '\0') && isspace(*t))
            t++;
        return t;
    }

    static const char* strip_r(const char *str, uint32_t len)
    {
        if (len == 0)
            return str;
        const char *t = &str[len-1];
        while (t >= str && isspace(*t))
            t--;
        return t + 1;
    }

    char* builtin_string_strip ( const char *str, Context * context )
    {
        const uint32_t strLen = stringLengthSafe ( *context, str );
        if (!strLen)
            return nullptr;
        const char *start = strip_l(str);
        const char *end = strip_r(str, strLen);
        return end > start ? context->heap.allocateString(start, uint32_t(end-start)) : nullptr;
    }
    char* builtin_string_strip_left ( const char *str, Context * context )
    {
        const uint32_t strLen = stringLengthSafe ( *context, str );
        if (!strLen)
            return nullptr;
        const char *start = strip_l(str);
        return start-str < strLen ? context->heap.allocateString(start, strLen-uint32_t(start-str)) : nullptr;
    }
    char* builtin_string_strip_right ( const char *str, Context * context )
    {
        const uint32_t strLen = stringLengthSafe ( *context, str );
        if (!strLen)
            return nullptr;
        const char *end = strip_r(str, strLen);
        return end != str ? context->heap.allocateString(str, uint32_t(end-str)) : nullptr;
    }

    void Module_BuiltIn::addString(ModuleLibrary & lib) {
        addExtern<DAS_BIND_FUN(builtin_string_endswith)>(*this, lib, "endswith", false);
        addExtern<DAS_BIND_FUN(builtin_string_startswith)>(*this, lib, "startswith", false);
        addExtern<DAS_BIND_FUN(builtin_string_strip)>(*this, lib, "strip", false);
        addExtern<DAS_BIND_FUN(builtin_string_strip_right)>(*this, lib, "strip_right", false);
        addExtern<DAS_BIND_FUN(builtin_string_strip_left)>(*this, lib, "strip_left", false);
    }
}
