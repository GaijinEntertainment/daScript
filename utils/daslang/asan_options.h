#pragma once

#include "daScript/misc/das_asan.h"

#if DAS_ASAN
extern "C" const char * __asan_default_options() {
    return "quarantine_size_mb=32";
}
#endif
