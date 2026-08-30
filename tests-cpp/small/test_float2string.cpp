// DAS_F2S_EXHAUSTIVE=1 runs the exhaustive case below: every finite float32, minutes, all cores.
#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include "daScript/misc/float2string.h"
#include "misc/include_fmt.h"

#include <atomic>
#include <cstdint>
#include <cstring>
#include <string>
#include <thread>
#include <vector>

using namespace das;

static const uint32_t kPosZero = 0x00000000u, kNegZero = 0x80000000u;
static const uint32_t kPosInf = 0x7f800000u, kNegInf = 0xff800000u;
static const uint32_t kNan = 0x7fc00000u, kNegNan = 0xffc00000u;
static const uint32_t kMinSubnormal = 0x00000001u, kMaxSubnormal = 0x007fffffu;
static const uint32_t kFltMin = 0x00800000u, kFltMax = 0x7f7fffffu;
static const uint32_t kOne = 0x3f800000u, kPointOne = 0x3dcccccdu, kOneE10 = 0x501502f9u;

static const uint64_t kDPosZero = 0x0000000000000000ull, kDNegZero = 0x8000000000000000ull;
static const uint64_t kDPosInf = 0x7ff0000000000000ull, kDNegInf = 0xfff0000000000000ull;
static const uint64_t kDNan = 0x7ff8000000000000ull, kDNegNan = 0xfff8000000000000ull;
static const uint64_t kDMinSubnormal = 0x0000000000000001ull, kDMaxSubnormal = 0x000fffffffffffffull;
static const uint64_t kDblMin = 0x0010000000000000ull, kDblMax = 0x7fefffffffffffffull;
static const uint64_t kDOne = 0x3ff0000000000000ull, kDPointOne = 0x3fb999999999999aull, kDOneE4 = 0x40c3880000000000ull;

static bool same_float ( uint32_t bits, std::string & bad ) {
    float x;
    memcpy(&x, &bits, sizeof(x));
    char ours[128];
    char * end = float2string(ours, x);
    char theirs[128];
    auto tend = fmt::format_to(theirs, FMT_STRING("{}"), x);
    if ( end - ours != tend - theirs || memcmp(ours, theirs, size_t(end - ours)) != 0 ) {
        bad = "bits 0x" + std::to_string(bits) + ": ours '" + std::string(ours, end)
            + "' fmt '" + std::string(theirs, tend) + "'";
        return false;
    }
    return true;
}

static bool same_double ( uint64_t bits, std::string & bad ) {
    double x;
    memcpy(&x, &bits, sizeof(x));
    char ours[128];
    char * end = double2string(ours, x);
    char theirs[128];
    auto tend = fmt::format_to(theirs, FMT_STRING("{}"), x);
    if ( end - ours != tend - theirs || memcmp(ours, theirs, size_t(end - ours)) != 0 ) {
        bad = "bits 0x" + std::to_string(bits) + ": ours '" + std::string(ours, end)
            + "' fmt '" + std::string(theirs, tend) + "'";
        return false;
    }
    return true;
}

TEST_CASE("float2string matches the fmt spelling it replaced") {
    std::string bad;
    const uint32_t specialAndBoundaryBits[] = {
        kPosZero, kNegZero, kPosInf, kNegInf, kNan, kNegNan,
        kMinSubnormal, kMaxSubnormal, kFltMin, kFltMax, kOne, kPointOne, kOneE10,
    };
    for ( auto bits : specialAndBoundaryBits ) {
        CHECK_MESSAGE(same_float(bits, bad), bad);
    }
    for ( uint32_t exp = 0; exp <= 0xff; ++exp ) {
        for ( uint32_t m = 0; m < 0x800000u; m += 0x02467u ) {
            uint32_t bits = (exp << 23) | m;
            if ( !same_float(bits, bad) ) { FAIL(bad); return; }
            bits |= 0x80000000u;
            if ( !same_float(bits, bad) ) { FAIL(bad); return; }
        }
    }
    const uint64_t specialAndBoundaryDoubleBits[] = {
        kDPosZero, kDNegZero, kDPosInf, kDNegInf, kDNan, kDNegNan,
        kDMinSubnormal, kDMaxSubnormal, kDblMin, kDblMax, kDOne, kDPointOne, kDOneE4,
    };
    for ( auto bits : specialAndBoundaryDoubleBits ) {
        CHECK_MESSAGE(same_double(bits, bad), bad);
    }
    uint64_t dbits = 1;
    for ( int i = 0; i < 2000000; ++i ) {
        dbits = dbits * 6364136223846793005ull + 1442695040888963407ull;
        uint64_t b = dbits;
        if ( (b & 0x7ff0000000000000ull) == 0x7ff0000000000000ull ) continue;
        if ( !same_double(b, bad) ) { FAIL(bad); return; }
    }
    CHECK(true);
}

TEST_CASE("float2string matches fmt on every finite float32" * doctest::skip(getenv("DAS_F2S_EXHAUSTIVE") == nullptr)) {
    const unsigned workers = std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() : 8;
    std::atomic<uint64_t> next(0);
    std::atomic<bool> ok(true);
    std::vector<std::string> bads(workers);
    std::vector<std::thread> pool;
    for ( unsigned w = 0; w != workers; ++w ) {
        pool.emplace_back([&, w]() {
            std::string bad;
            const uint64_t chunk = 1u << 20;
            for ( ;; ) {
                uint64_t base = next.fetch_add(chunk);
                if ( base > 0xffffffffull || !ok.load(std::memory_order_relaxed) ) break;
                uint64_t top = base + chunk;
                if ( top > 0x100000000ull ) top = 0x100000000ull;
                for ( uint64_t b = base; b != top; ++b ) {
                    if ( !same_float(uint32_t(b), bad) ) {
                        bads[w] = bad;
                        ok = false;
                        break;
                    }
                }
            }
        });
    }
    for ( auto & t : pool ) t.join();
    for ( auto & bad : bads ) {
        CHECK_MESSAGE(bad.empty(), bad);
    }
    CHECK(ok.load());
}
