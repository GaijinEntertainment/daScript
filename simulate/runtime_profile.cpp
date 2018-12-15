#include "precomp.h"

#include "runtime_range.h"

#include <mach/mach.h>
#include <mach/mach_time.h>

namespace yzg
{
    float builtin_profile ( int32_t count, char * category, Block block, Context * context ) {
        count = max(count,1);
        // profile
        uint64_t minT = -1;
        for ( int32_t i = 0; i != count; ++i ) {
            uint64_t t0 = mach_absolute_time();
            context->invoke(block);
            uint64_t t1 = mach_absolute_time();
            minT = min(t1-t0, minT);
        }
        mach_timebase_info_data_t s_timebase_info;
        mach_timebase_info(&s_timebase_info);
        double tSec = minT * s_timebase_info.numer / (1000000000.0 * s_timebase_info.denom);
        if ( category ) {
            cout << category << " took: " << fixed << tSec << "\n";
        }
        return tSec;
    }
}

