#include "daScript/misc/platform.h"

#include "daScript/misc/smart_ptr.h"
#include "daScript/misc/job_que.h"

#include <signal.h>

#if defined(_WIN32) && !defined(_MSC_VER)
// gcc-mingw doesn't expose __debugbreak as a builtin (you'd need <intrin.h>
// and the right configuration), and clang-mingw's behavior varies by version.
// kernel32!DebugBreak is the portable Windows API that always works, at the
// cost of one extra call frame — negligible vs the win of mingw-safe builds.
#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

DAS_API void os_debug_break()
{
#if defined(_MSC_VER)
    // MSVC (incl. clang-cl) — single-instruction intrinsic, no extra frame.
    __debugbreak();
#elif defined(_WIN32)
    // mingw — WinAPI works regardless of which Windows clang/gcc flavor.
    DebugBreak();
#else
    raise(SIGTRAP);
#endif
}

namespace das {
    class Context;
    DAS_API void os_keepalive_call ( Context * ) {}
    DAS_API void os_keepalive_loop ( Context * ) {}
}

