#include <doctest/doctest.h>

#include "daScript/misc/platform.h"     // memory_model.h is not self-contained
#include "daScript/misc/memory_model.h"

#include <vector>
#include <cstring>

using namespace das;

// Exercises the shoe allocator's chunk-growth path (Deck chain) at a moderate,
// CI-friendly scale: enough allocations to force several geometric grow() steps so
// the 64-bit Deck index/byte math is exercised, without the 4 GB the actual overflow
// boundary needs (that lives in tests-cpp/big/memory_model_4gb).
TEST_CASE("MemoryModel shoe growth: many small allocations stay valid across chunks") {
    MemoryModel mm;
    const uint64_t es = 256;            // largest shoe size-class
    const int N = 200000;               // ~50 MB — multiple chunk growths
    std::vector<char*> ptrs;
    ptrs.reserve(N);
    for ( int i=0; i<N; ++i ) {
        char * p = mm.allocate(es);
        REQUIRE(p != nullptr);
        REQUIRE((reinterpret_cast<uintptr_t>(p) & 15) == 0);   // 16-aligned
        memset(p, char(i & 0xff), size_t(es));                  // write the whole block
        ptrs.push_back(p);
    }
    // Every pointer is owned by the model and its bytes survived (no aliasing/overlap).
    for ( int i=0; i<N; ++i ) {
        CHECK(mm.isOwnPtr(ptrs[i], es));
        CHECK(ptrs[i][0] == char(i & 0xff));
        CHECK(ptrs[i][es-1] == char(i & 0xff));
    }
    CHECK(mm.bytesAllocated() == uint64_t(N) * es);

    // Free every other block, then re-allocate the same count — exercises the free
    // bitmap + reuse path across the grown chunks.
    for ( int i=0; i<N; i+=2 ) mm.free(ptrs[i], es);
    CHECK(mm.bytesAllocated() == uint64_t(N/2) * es);
    for ( int i=0; i<N/2; ++i ) {
        char * p = mm.allocate(es);
        REQUIRE(p != nullptr);
    }
    CHECK(mm.bytesAllocated() == uint64_t(N) * es);
}

// grow() returns a 64-bit entry count: it must carry a value above UINT32_MAX
// without truncation (the old code returned uint32_t and clamped the first-chunk
// path to UINT32_MAX). Drive grow() directly via customGrow — no 4 GB allocation
// (the real boundary lives in tests-cpp/big/memory_model_4gb).
TEST_CASE("MemoryModel::grow is 64-bit (no UINT32 clamp / truncation)") {
    MemoryModel mm;
    mm.setInitialSize(4096);
    char * p = mm.allocate(16);            // materialize the 16-byte-class chunk (si==0)
    REQUIRE(p != nullptr);
    // With a chunk present, grow() consults customGrow(currentTotal); returning a
    // value past 2^32 must round-trip intact through the uint64_t return.
    mm.customGrow = [](uint64_t) -> uint64_t { return uint64_t(1) << 33; };
    CHECK(mm.grow(0) == (uint64_t(1) << 33));
}
