#include "daScript/misc/platform.h"

#include <signal.h>

void os_debug_break()
{
#ifdef _MSC_VER
    __debugbreak();
#else
    raise(SIGTRAP);
#endif
}
