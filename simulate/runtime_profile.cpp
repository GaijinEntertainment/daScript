#include "precomp.h"

#include "runtime_range.h"

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

#else

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
        uint64_t minT = -1;
        for ( int32_t i = 0; i != count; ++i ) {
            uint64_t t0 = profileGetTime();
            context->invoke(block, nullptr, nullptr);
            uint64_t t1 = profileGetTime();
            minT = min(t1-t0, minT);
        }
        double tSec = profileGetTimeSec(minT);
        if ( category ) {
            cout << "\"" << category << "\", " << fixed << tSec << ", " << count << "\n";
        }
        return (float) tSec;
    }
}

