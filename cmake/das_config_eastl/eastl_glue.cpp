// EASTL integration glue for the build_eastl CI job. EASTL is not header-only:
// it requires the user to supply these operator new[] overloads and, with
// EASTL_EASTDC_VSNPRINTF=0 (set to avoid the EAStdC dependency), the Vsnprintf*
// family declared in EASTL/string.h. Compiled alongside eastl/source/*.cpp into
// libeastl.a and linked into daslang.
#include <new>
#include <cstddef>
#include <cstdarg>
#include <cstdio>

void * operator new[] ( size_t size, const char *, int, unsigned, const char *, int ) {
    return ::operator new[](size);
}
void * operator new[] ( size_t size, size_t alignment, size_t, const char *, int, unsigned, const char *, int ) {
    return ::operator new[](size, std::align_val_t(alignment));
}

int Vsnprintf8 ( char * dst, size_t n, const char * fmt, va_list args ) {
    return ::vsnprintf(dst, n, fmt, args);
}
int Vsnprintf16 ( char16_t *, size_t, const char16_t *, va_list ) { return -1; }
int Vsnprintf32 ( char32_t *, size_t, const char32_t *, va_list ) { return -1; }
