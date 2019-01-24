#include "daScript/misc/platform.h"

#include "daScript/simulate/runtime_range.h"

#ifdef _MSC_VER

#include <windows.h>

uint64_t profileGetTime () {
    LARGE_INTEGER  t0;
    QueryPerformanceCounter(&t0);
    return t0.QuadPart;
}

double profileGetTimeSec ( uint64_t minT ) {
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    return double(minT) / double(freq.QuadPart);
}

#elif __linux__

const uint64_t NSEC_IN_SEC = 1000000000LL;

uint64_t profileGetTime () {
    timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
      assert(false);
        return -1;
    }

    return ts.tv_sec * NSEC_IN_SEC + ts.tv_nsec;
}

double profileGetTimeSec ( uint64_t minT ) {
  return double(minT) / NSEC_IN_SEC;
}

#else // __linux__

#include <mach/mach.h>
#include <mach/mach_time.h>

uint64_t profileGetTime() {
    return mach_absolute_time();
}

double profileGetTimeSec ( uint64_t minT ) {
    mach_timebase_info_data_t s_timebase_info;
    mach_timebase_info(&s_timebase_info);
    return minT * s_timebase_info.numer / (1000000000.0 * s_timebase_info.denom);
}

#endif

namespace das
{
    float builtin_profile ( int32_t count, char * category, Block block, Context * context ) {
        count = max(count,1);
        uint64_t minT = -1U;
        for ( int32_t i = 0; i != count; ++i ) {
            uint64_t t0 = profileGetTime();
            context->invoke(block, nullptr, nullptr);
			if (context->stopFlags & EvalFlags::stopForThrow) return 0.0f;
            uint64_t t1 = profileGetTime();
            minT = min(t1-t0, minT);
        }
        double tSec = profileGetTimeSec(minT);
        if ( category ) {
            stringstream ss;
            ss << "\"" << category << "\", " << fixed << tSec << ", " << count << "\n";
            context->to_out(ss.str().c_str());
        }
        return (float) tSec;
    }
}

