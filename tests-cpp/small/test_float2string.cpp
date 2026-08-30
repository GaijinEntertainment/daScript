// The borrowed Luau emitter (src/misc/luau_float2string.cpp) must spell every float
// byte-for-byte as the fmt "{}" path it replaced - the corpus pins ~60 assertions and the
// msl/glsl goldens on that spelling. CI runs the sampled sweep; DAS_F2S_EXHAUSTIVE=1 runs
// every finite float32 (minutes, multithreaded) - the arc's local validation.
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
    // every special and boundary shape
    const uint32_t fixtures[] = {
        0x00000000u, 0x80000000u,               // +0 -0
        0x7f800000u, 0xff800000u,               // +inf -inf
        0x7fc00000u, 0xffc00000u,               // nan -nan
        0x00000001u, 0x007fffffu,               // subnormals
        0x00800000u, 0x7f7fffffu,               // FLT_MIN FLT_MAX
        0x3f800000u, 0x3dcccccdu, 0x501502f9u,  // 1, 0.1, 1e10
    };
    for ( auto bits : fixtures ) {
        CHECK_MESSAGE(same_float(bits, bad), bad);
    }
    // every decade float reaches, in fixed steps through the mantissa
    for ( uint32_t exp = 0; exp <= 0xff; ++exp ) {
        for ( uint32_t m = 0; m < 0x800000u; m += 0x02467u ) {
            uint32_t bits = (exp << 23) | m;
            if ( !same_float(bits, bad) ) { FAIL(bad); return; }
            bits |= 0x80000000u;
            if ( !same_float(bits, bad) ) { FAIL(bad); return; }
        }
    }
    // doubles: the same walk at double density
    const uint64_t dfixtures[] = {
        0x0000000000000000ull, 0x8000000000000000ull,
        0x7ff0000000000000ull, 0xfff0000000000000ull,
        0x7ff8000000000000ull, 0xfff8000000000000ull,
        0x0000000000000001ull, 0x000fffffffffffffull,
        0x0010000000000000ull, 0x7fefffffffffffffull,
        0x3ff0000000000000ull, 0x3fb999999999999aull, 0x40c3880000000000ull,
    };
    for ( auto bits : dfixtures ) {
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
