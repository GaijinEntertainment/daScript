#include "precomp.h"

#include "runtime_range.h"

#ifdef _MSC_VER

#include <windows.h>

namespace yzg
{
	float builtin_profile(int32_t count, char * category, Block block, Context * context) {
		count = max(count, 1);
		// profile
		uint64_t minT = -1;
		for (int32_t i = 0; i != count; ++i) {
			LARGE_INTEGER  t0;
			QueryPerformanceCounter(&t0);
			context->invoke(block, nullptr);
			LARGE_INTEGER  t1; 
			QueryPerformanceCounter(&t1);
			minT = min(uint64_t(t1.QuadPart-t0.QuadPart), minT);
		}
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);
		double tSec = double(minT) / double(freq.QuadPart);
		if (category) {
			cout << category << " took: " << fixed << tSec << "\n";
		}
		return (float) tSec;
	}
}


#else

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
            context->invoke(block, nullptr);
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

#endif
