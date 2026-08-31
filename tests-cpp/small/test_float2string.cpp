#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include "daScript/misc/float2string.h"
#include "misc/include_fmt.h"

#include <atomic>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <thread>
#include <vector>

using namespace das;

static bool same_float ( uint32_t bits, std::string & mismatch ) {
    float x;
    memcpy(&x, &bits, sizeof(x));
    char ours[128];
    char * end = float2string(ours, x);
    char fmtText[128];
    auto fmtEnd = fmt::format_to(fmtText, FMT_STRING("{}"), x);
    if ( end - ours != fmtEnd - fmtText || memcmp(ours, fmtText, size_t(end - ours)) != 0 ) {
        mismatch = fmt::format("bits 0x{:08x}: ours '{}' fmt '{}'", bits,
            std::string(ours, end), std::string(fmtText, fmtEnd));
        return false;
    }
    return true;
}

static bool same_double ( uint64_t bits, std::string & mismatch ) {
    double x;
    memcpy(&x, &bits, sizeof(x));
    char ours[128];
    char * end = double2string(ours, x);
    char fmtText[128];
    auto fmtEnd = fmt::format_to(fmtText, FMT_STRING("{}"), x);
    if ( end - ours != fmtEnd - fmtText || memcmp(ours, fmtText, size_t(end - ours)) != 0 ) {
        mismatch = fmt::format("bits 0x{:016x}: ours '{}' fmt '{}'", bits,
            std::string(ours, end), std::string(fmtText, fmtEnd));
        return false;
    }
    return true;
}

static const uint32_t kPosZero = 0x00000000u, kNegZero = 0x80000000u;
static const uint32_t kPosInf = 0x7f800000u, kNegInf = 0xff800000u;
static const uint32_t kNan = 0x7fc00000u, kNegNan = 0xffc00000u;
static const uint32_t kMinSubnormal = 0x00000001u, kMaxSubnormal = 0x007fffffu;
static const uint32_t kFltMin = 0x00800000u, kFltMax = 0x7f7fffffu;
static const uint32_t kOne = 0x3f800000u, kPointOne = 0x3dcccccdu, kOneE10 = 0x501502f9u;
static const uint32_t kMantissaStride = 0x02467u;

TEST_CASE("float2string matches the fmt spelling it replaced") {
    std::string mismatch;
    const uint32_t specialAndBoundaryBits[] = {
        kPosZero, kNegZero, kPosInf, kNegInf, kNan, kNegNan,
        kMinSubnormal, kMaxSubnormal, kFltMin, kFltMax, kOne, kPointOne, kOneE10,
    };
    for ( auto bits : specialAndBoundaryBits ) {
        CHECK_MESSAGE(same_float(bits, mismatch), mismatch);
    }
    const float kTwoDigitScientific[] = { 1.5e20f, 2.5e-9f, -7.5e18f };
    for ( float v : kTwoDigitScientific ) {
        uint32_t b;
        memcpy(&b, &v, sizeof(b));
        CHECK_MESSAGE(same_float(b, mismatch), mismatch);
    }
    for ( uint32_t exp = 0; exp <= 0xffu; ++exp ) {
        for ( uint32_t m = 0; m < 0x800000u; m += kMantissaStride ) {
            uint32_t bits = (exp << 23) | m;
            if ( !same_float(bits, mismatch) ) { FAIL(mismatch); return; }
            bits |= 0x80000000u;
            if ( !same_float(bits, mismatch) ) { FAIL(mismatch); return; }
        }
    }
}

static const uint64_t kDPosZero = 0x0000000000000000ull, kDNegZero = 0x8000000000000000ull;
static const uint64_t kDPosInf = 0x7ff0000000000000ull, kDNegInf = 0xfff0000000000000ull;
static const uint64_t kDNan = 0x7ff8000000000000ull, kDNegNan = 0xfff8000000000000ull;
static const uint64_t kDMinSubnormal = 0x0000000000000001ull, kDMaxSubnormal = 0x000fffffffffffffull;
static const uint64_t kDblMin = 0x0010000000000000ull, kDblMax = 0x7fefffffffffffffull;
static const uint64_t kDOne = 0x3ff0000000000000ull, kDPointOne = 0x3fb999999999999aull, kDOneE4 = 0x40c3880000000000ull;
static const uint64_t kDExpMask = 0x7ff0000000000000ull;
static const uint64_t kLcgMul = 6364136223846793005ull, kLcgAdd = 1442695040888963407ull;
static const int kDoubleSamples = 2000000;

TEST_CASE("double2string matches the fmt spelling it replaced") {
    std::string mismatch;
    const uint64_t specialAndBoundaryDoubleBits[] = {
        kDPosZero, kDNegZero, kDPosInf, kDNegInf, kDNan, kDNegNan,
        kDMinSubnormal, kDMaxSubnormal, kDblMin, kDblMax, kDOne, kDPointOne, kDOneE4,
    };
    for ( auto bits : specialAndBoundaryDoubleBits ) {
        CHECK_MESSAGE(same_double(bits, mismatch), mismatch);
    }
    const double kTwoDigitScientificD[] = { 1.5e17, 2.5e-7, -7.5e300 };
    for ( double v : kTwoDigitScientificD ) {
        uint64_t b;
        memcpy(&b, &v, sizeof(b));
        CHECK_MESSAGE(same_double(b, mismatch), mismatch);
    }
    uint64_t dbits = 1;
    for ( int i = 0; i < kDoubleSamples; ++i ) {
        dbits = dbits * kLcgMul + kLcgAdd;
        if ( (dbits & kDExpMask) == kDExpMask ) continue;
        if ( !same_double(dbits, mismatch) ) { FAIL(mismatch); return; }
    }
}

TEST_CASE("float2string matches fmt on every finite float32 - set DASLANG_F2S_EXHAUSTIVE=1, minutes, all cores"
          * doctest::skip(getenv("DASLANG_F2S_EXHAUSTIVE") == nullptr)) {
    const unsigned workers = std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() : 8;
    std::atomic<uint64_t> next(0);
    std::atomic<bool> ok(true);
    std::vector<std::string> mismatches(workers);
    std::vector<std::thread> pool;
    for ( unsigned w = 0; w != workers; ++w ) {
        pool.emplace_back([&, w]() {
            std::string mismatch;
            const uint64_t chunk = 1u << 20;
            for ( ;; ) {
                uint64_t base = next.fetch_add(chunk);
                if ( base > 0xffffffffull || !ok.load(std::memory_order_relaxed) ) break;
                uint64_t top = base + chunk;
                if ( top > 0x100000000ull ) top = 0x100000000ull;
                for ( uint64_t b = base; b != top; ++b ) {
                    if ( !same_float(uint32_t(b), mismatch) ) {
                        mismatches[w] = mismatch;
                        ok = false;
                        break;
                    }
                }
            }
        });
    }
    for ( auto & t : pool ) t.join();
    for ( auto & m : mismatches ) {
        CHECK_MESSAGE(m.empty(), m);
    }
}
