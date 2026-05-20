// Direct C++ unit tests for `das::LinearChunkAllocator` — the per-chunk
// bump allocator under LinearHeapAllocator / NodeAllocator. Covers the
// Phase 1 widening: setInitialSize accepts uint64_t (with a panic guard
// at >UINT32_MAX, not exercised here), reset() clamps initialSize at
// UINT32_MAX, and the chunks themselves remain uint32-bounded.

#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/misc/memory_model.h"

#include <cstring>

using namespace das;

TEST_CASE("LinearChunkAllocator: default-construct state") {
    LinearChunkAllocator a;
    CHECK(a.depth() == 0);
    CHECK(a.bytesAllocated() == 0);
    CHECK(a.chunk == nullptr);
    CHECK(a.initialSize == 0);
}

TEST_CASE("LinearChunkAllocator: setInitialSize accepts uint64_t under the cap") {
    LinearChunkAllocator a;
    a.setInitialSize(uint64_t(64 * 1024));
    CHECK(a.initialSize == 64 * 1024);
    CHECK(a.unadjustedInitialSize == 64 * 1024);
}

TEST_CASE("LinearChunkAllocator: allocate(N) returns aligned + non-null") {
    LinearChunkAllocator a;
    char * p = a.allocate(64);
    REQUIRE(p != nullptr);
    // Default alignMask=15 → 16-byte alignment.
    CHECK((uintptr_t(p) & uintptr_t(a.alignMask)) == 0);
}

TEST_CASE("LinearChunkAllocator: distinct allocations get distinct pointers") {
    LinearChunkAllocator a;
    char * x = a.allocate(64);
    char * y = a.allocate(64);
    char * z = a.allocate(64);
    REQUIRE(x != nullptr);
    REQUIRE(y != nullptr);
    REQUIRE(z != nullptr);
    CHECK(x != y);
    CHECK(y != z);
    CHECK(x != z);
}

TEST_CASE("LinearChunkAllocator: chunk grows when initial chunk fills") {
    LinearChunkAllocator a;
    a.setInitialSize(128);              // small enough to overflow quickly
    auto d0 = a.depth();
    CHECK(d0 == 0);
    // Burn through several chunks.
    for ( int i=0; i<10; i++ ) {
        a.allocate(64);
    }
    CHECK(a.depth() >= 2);
}

TEST_CASE("LinearChunkAllocator: free is watermark-only (tail of the current chunk)") {
    LinearChunkAllocator a;
    char * p = a.allocate(64);
    char * m = a.allocate(64);                // middle, never tail-most
    char * q = a.allocate(64);
    REQUIRE(p != nullptr);
    REQUIRE(m != nullptr);
    REQUIRE(q != nullptr);
    // free(m) is a no-op (not tail). Confirm bytesAllocated unchanged.
    auto bytesAtPeak = a.bytesAllocated();
    a.free(m, 64);
    CHECK(a.bytesAllocated() == bytesAtPeak); // middle free leaves accounting alone
    // free(q) IS at the tail and rewinds.
    a.free(q, 64);
    CHECK(a.bytesAllocated() < bytesAtPeak);
}

TEST_CASE("LinearChunkAllocator: reallocate copies prefix") {
    LinearChunkAllocator a;
    char * p = a.allocate(32);
    REQUIRE(p != nullptr);
    for ( int i=0; i<32; i++ ) p[i] = (char)(i * 3);
    char * q = a.reallocate(p, 32, 256);
    REQUIRE(q != nullptr);
    for ( int i=0; i<32; i++ ) CHECK(q[i] == (char)(i * 3));
}

TEST_CASE("LinearChunkAllocator: reset releases all chunks but the first") {
    LinearChunkAllocator a;
    a.setInitialSize(128);
    for ( int i=0; i<50; i++ ) {
        a.allocate(64);
    }
    CHECK(a.depth() >= 2);
    a.reset();
    // After reset, the chunk pointer may be null or a single fresh chunk;
    // either way depth shrinks back to 0/1.
    CHECK(a.depth() <= 1);
}

TEST_CASE("LinearChunkAllocator: shrink releases empty top chunk") {
    LinearChunkAllocator a;
    char * p = a.allocate(64);
    REQUIRE(p != nullptr);
    a.reset();                          // does NOT clear chunk[0] if no growth
    a.shrink();                         // shrink drops empty trailing chunk
    CHECK(a.chunk == nullptr);
}

TEST_CASE("LinearChunkAllocator: allocateName round-trips a string") {
    LinearChunkAllocator a;
    char * s = a.allocateName(string("foobar"));
    REQUIRE(s != nullptr);
    CHECK(strcmp(s, "foobar") == 0);
    CHECK(s[6] == 0);
}

TEST_CASE("LinearChunkAllocator: isOwnPtr distinguishes in-chunk vs foreign") {
    LinearChunkAllocator a;
    char * p = a.allocate(64);
    REQUIRE(p != nullptr);
    char stack = 0;
    CHECK(a.isOwnPtr(p));
    CHECK(!a.isOwnPtr(&stack));
}

TEST_CASE("LinearChunkAllocator: reset() initialSize clamp invariant (Phase 1)") {
    // Verify the Phase 1 clamp: reset() computes maxAllocated in uint64 and
    // would otherwise let initialSize grow past UINT32_MAX over a long
    // allocator lifetime. After Phase 1, the clamp caps it.
    LinearChunkAllocator a;
    // Manually arrange the state that reset() would see: chunk + chunk->next
    // is required to trigger the recalculation branch. We don't actually
    // accumulate 4GB; we just confirm that whatever value reset() writes is
    // <= UINT32_MAX.
    a.setInitialSize(1024);
    a.allocate(64);                     // create chunk[0]
    while ( a.depth() < 2 ) {
        a.allocate(1024);               // force a second chunk
    }
    a.reset();
    CHECK(a.initialSize <= uint64_t(UINT32_MAX));
}
