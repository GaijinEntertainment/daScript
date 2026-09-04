// This TU is compiled with the host's fast-math flag (tests-cpp/CMakeLists.txt sets it per
// source): is_nan / is_finite answer IEEE even in a runtime built -ffast-math / -fp:fast.
// Without the flag the test would pass precise and prove nothing, so its absence is an error.
#if !defined(__FAST_MATH__) && !defined(_M_FP_FAST)
#error "test_isnan_fastmath.cpp must be compiled with -ffast-math / /fp:fast (tests-cpp/CMakeLists.txt)"
#endif
#include <doctest/doctest.h>
#include "daScript/misc/platform.h"
#include "daScript/misc/vectypes.h"

// The header's precise path defines these `inline`, and the executable also links
// libDaScript: the linker keeps ONE copy per name, so without unique names this TU would
// silently test the precise copy from module_builtin_math.cpp.
#define fisnan    fisnan_fast_math_tu
#define disnan    disnan_fast_math_tu
#define fisfinite fisfinite_fast_math_tu
#define disfinite disfinite_fast_math_tu
#include "daScript/simulate/aot_builtin_math.h"

#include <cstdint>
#include <cstring>

namespace {
    // bit patterns, not arithmetic: nothing here is a float operation the compiler could fold
    // (prefixed: glibc's <math.h> defines SNAN and friends as macros)
    enum { PAT_QNAN, PAT_NEG_QNAN, PAT_SNAN, PAT_INF, PAT_NEG_INF, PAT_MAX_FINITE, PAT_DENORMAL, PAT_TWO_AND_HALF, PAT_NEG_ZERO, COUNT };

    volatile uint32_t fbits[COUNT] = {
        0x7FC00000u, 0xFFC00000u, 0x7F800001u, 0x7F800000u, 0xFF800000u,
        0x7F7FFFFFu, 0x00000001u, 0x40200000u, 0x80000000u,
    };
    volatile uint64_t dbits[COUNT] = {
        0x7FF8000000000000ull, 0xFFF8000000000000ull, 0x7FF0000000000001ull, 0x7FF0000000000000ull, 0xFFF0000000000000ull,
        0x7FEFFFFFFFFFFFFFull, 0x0000000000000001ull, 0x4004000000000000ull, 0x8000000000000000ull,
    };

    const bool expect_nan[COUNT]    = { true,  true,  true,  false, false, false, false, false, false };
    const bool expect_finite[COUNT] = { false, false, false, false, false, true,  true,  true,  true  };
}

TEST_CASE("is_nan / is_finite honor IEEE under the host's fast-math flag") {
    float f[COUNT];
    double d[COUNT];
    for (int i = 0; i != COUNT; ++i) {
        uint32_t fb = fbits[i]; memcpy(&f[i], &fb, sizeof(fb));
        uint64_t db = dbits[i]; memcpy(&d[i], &db, sizeof(db));
    }
    for (int i = 0; i != COUNT; ++i) {
        INFO("pattern index ", i);
        CHECK_EQ(das::fisnan(f[i]), expect_nan[i]);
        CHECK_EQ(das::disnan(d[i]), expect_nan[i]);
        CHECK_EQ(das::fisfinite(f[i]), expect_finite[i]);
        CHECK_EQ(das::disfinite(d[i]), expect_finite[i]);
    }
}
