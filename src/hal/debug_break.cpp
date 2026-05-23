#include "daScript/misc/platform.h"

#include "daScript/misc/smart_ptr.h"
#include "daScript/misc/job_que.h"

#include <signal.h>

DAS_API void os_debug_break()
{
#ifdef _WIN32
    // __debugbreak is a clang/MSVC intrinsic that compiles on all Windows
    // toolchains (MSVC, clang-cl, clang-mingw, gcc-mingw — gcc-mingw provides
    // it via its <intrin.h>).
    __debugbreak();
#else
    raise(SIGTRAP);
#endif
}

namespace das {
    class Context;
    DAS_API void os_keepalive_call ( Context * ) {}
    DAS_API void os_keepalive_loop ( Context * ) {}
}

