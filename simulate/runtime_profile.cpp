#include "precomp.h"

#include "runtime_range.h"

#include <mach/mach.h>
#include <mach/mach_time.h>

namespace yzg
{
    __m128 builtin_profile ( Context & context, SimNode_Call *, __m128 * args ) {
        int32_t count = max( cast<int32_t>::to(args[0]), 1);
        char * category = cast<char *>::to(args[1]);
        Block block = cast<Block>::to(args[2]);
        // profile
        uint64_t minT = -1;
        for ( int32_t i = 0; i != count; ++i ) {
            uint64_t t0 = mach_absolute_time();
            context.invoke(block);
            uint64_t t1 = mach_absolute_time();
            minT = min(t1-t0, minT);
        }
        mach_timebase_info_data_t s_timebase_info;
        mach_timebase_info(&s_timebase_info);
        double tSec = minT * s_timebase_info.numer / (1000000000.0 * s_timebase_info.denom);
        if ( category ) {
            cout << category << " took: " << fixed << tSec << "\n";
        }
        return cast<float>::from(float(tSec));
    }
}

