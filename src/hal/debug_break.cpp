#include "daScript/misc/platform.h"

#include "daScript/misc/smart_ptr.h"

#include <signal.h>

uint64_t               das::ptr_ref_count::ref_count_total = 0;
uint64_t               das::ptr_ref_count::ref_count_track = 0;
uint64_t               das::ptr_ref_count::ref_count_track_destructor = 0;
das::ptr_ref_count *   das::ptr_ref_count::ref_count_head = nullptr;
das::mutex             das::ptr_ref_count::ref_count_mutex;

DAS_API das::atomic<uint64_t> das::g_smart_ptr_total {0};

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

