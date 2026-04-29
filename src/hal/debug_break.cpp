#include "daScript/misc/platform.h"

#include "daScript/misc/smart_ptr.h"
#include "daScript/misc/job_que.h"

#include <signal.h>

DAS_API void os_debug_break()
{
#ifdef _MSC_VER
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

