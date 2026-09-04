// A precise TU whose checks sit inside the per-function fast-math region the AOT emitter
// puts around every function of an `options fast_math` program (DAS_FAST_MATH_PUSH/POP in
// aot.h): is_nan / is_finite still answer IEEE there, on bit-built values.
#if defined(__FAST_MATH__) || defined(_M_FP_FAST)
#error "test_isnan_fastmath_region.cpp is the precise-TU half; the fast-math TU is test_isnan_fastmath.cpp"
#endif
#include <doctest/doctest.h>
#include "daScript/misc/platform.h"
#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"

// The header's helpers are `inline`, and the executable also links libDaScript: the linker
// keeps ONE copy per name, so without unique names this TU could test another TU's copy.
#define fisnan    fisnan_fast_math_region_tu
#define disnan    disnan_fast_math_region_tu
#define fisfinite fisfinite_fast_math_region_tu
#define disfinite disfinite_fast_math_region_tu
#include "daScript/simulate/aot_builtin_math.h"

#include <cstdint>
#include <cstring>

namespace {
    // prefixed: glibc's <math.h> defines SNAN and friends as macros
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

    struct Answers { bool fnan[COUNT], dnan[COUNT], ffinite[COUNT], dfinite[COUNT]; };

    DAS_FAST_MATH_PUSH
    Answers classify_inside_fast_math_region() {
        float f[COUNT];
        double d[COUNT];
        for (int i = 0; i != COUNT; ++i) {
            uint32_t fb = fbits[i]; memcpy(&f[i], &fb, sizeof(fb));
            uint64_t db = dbits[i]; memcpy(&d[i], &db, sizeof(db));
        }
        Answers a;
        for (int i = 0; i != COUNT; ++i) {
            a.fnan[i] = das::fisnan(f[i]);
            a.dnan[i] = das::disnan(d[i]);
            a.ffinite[i] = das::fisfinite(f[i]);
            a.dfinite[i] = das::disfinite(d[i]);
        }
        return a;
    }
    DAS_FAST_MATH_POP
}

TEST_CASE("is_nan / is_finite honor IEEE inside an options fast_math AOT region") {
    const Answers a = classify_inside_fast_math_region();
    for (int i = 0; i != COUNT; ++i) {
        INFO("pattern index ", i);
        CHECK_EQ(a.fnan[i], expect_nan[i]);
        CHECK_EQ(a.dnan[i], expect_nan[i]);
        CHECK_EQ(a.ffinite[i], expect_finite[i]);
        CHECK_EQ(a.dfinite[i], expect_finite[i]);
    }
}
