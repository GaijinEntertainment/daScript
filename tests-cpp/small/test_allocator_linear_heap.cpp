// Direct C++ unit tests for `das::LinearHeapAllocator` — the linear/bump
// allocator used by NodeAllocator and friends. impl_free is intentionally
// a no-op (linear allocators only release at reset()).

#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/simulate/heap.h"

#include <cstring>

using namespace das;

TEST_CASE("LinearHeapAllocator: alloc returns distinct sequential pointers") {
    LinearHeapAllocator h;
    char * a = h.impl_allocate(64);
    char * b = h.impl_allocate(64);
    char * c = h.impl_allocate(64);
    REQUIRE(a != nullptr);
    REQUIRE(b != nullptr);
    REQUIRE(c != nullptr);
    CHECK(a != b);
    CHECK(b != c);
    CHECK(a != c);
}

TEST_CASE("LinearHeapAllocator: setInitialSize accepts uint64_t (widened)") {
    LinearHeapAllocator h;
    h.setInitialSize(uint64_t(64 * 1024));
    CHECK(h.getInitialSize() == int64_t(64 * 1024));
    static_assert(sizeof(decltype(h.getInitialSize())) == 8, "getInitialSize() must be int64_t after Phase 1");
}

TEST_CASE("LinearHeapAllocator: impl_free is a no-op (live bytes only drop at reset)") {
    LinearHeapAllocator h;
    char * a = h.impl_allocate(64);
    char * b = h.impl_allocate(64);
    REQUIRE(a != nullptr);
    REQUIRE(b != nullptr);
    uint64_t beforeFree = h.bytesAllocated();
    h.impl_free(a, 64);
    // In linear allocators, freeing in the middle is a no-op; bytesAllocated
    // doesn't shrink. (totalBytesDeleted does increment though.)
    CHECK(h.bytesAllocated() == beforeFree);
    CHECK(h.getTotalBytesDeleted() == 64);
}

TEST_CASE("LinearHeapAllocator: reset() releases all chunks") {
    LinearHeapAllocator h;
    h.setInitialSize(4096);
    for ( int i=0; i<100; i++ ) {
        char * p = h.impl_allocate(64);
        REQUIRE(p != nullptr);
    }
    CHECK(h.bytesAllocated() > 0);
    h.reset();
    CHECK(h.bytesAllocated() == 0);
    // Subsequent alloc works from a fresh state.
    char * fresh = h.impl_allocate(64);
    REQUIRE(fresh != nullptr);
}

TEST_CASE("LinearHeapAllocator: reallocate copies prefix") {
    LinearHeapAllocator h;
    char * p = h.impl_allocate(32);
    REQUIRE(p != nullptr);
    for ( int i=0; i<32; i++ ) p[i] = (char)(i + 1);
    char * q = h.impl_reallocate(p, 32, 128);
    REQUIRE(q != nullptr);
    for ( int i=0; i<32; i++ ) CHECK(q[i] == (char)(i + 1));
}

TEST_CASE("LinearHeapAllocator: isOwnPtr true for in-chunk; isValidPtr always true") {
    LinearHeapAllocator h;
    char * p = h.impl_allocate(64);
    REQUIRE(p != nullptr);
    // isOwnPtr ignores the size argument for linear allocators (it scans chunks).
    CHECK(h.isOwnPtr(p, 0));
    char stack = 0;
    CHECK(!h.isOwnPtr(&stack, 0));
    // isValidPtr is hard-coded to true in LinearHeapAllocator (no per-ptr
    // bookkeeping). Documents the contract.
    CHECK(h.isValidPtr(p, 0));
    CHECK(h.isValidPtr(&stack, 0));  // intentional — see docstring above
}

TEST_CASE("LinearHeapAllocator: mark() returns false (linear allocator can't GC)") {
    LinearHeapAllocator h;
    CHECK(!h.mark());
}

TEST_CASE("LinearHeapAllocator: depth grows with allocation pressure") {
    LinearHeapAllocator h;
    h.setInitialSize(64);                 // tiny initial chunk so we grow soon
    auto d0 = h.depth();
    for ( int i=0; i<200; i++ ) {
        h.impl_allocate(64);
    }
    CHECK(h.depth() > d0);                 // multiple chunks now
    h.reset();
}

TEST_CASE("LinearHeapAllocator: shrink() releases unused chunks after reset") {
    LinearHeapAllocator h;
    for ( int i=0; i<100; i++ ) {
        h.impl_allocate(64);
    }
    h.reset();
    h.shrink();
    // After reset+shrink the depth should be 0 (no chunks held).
    CHECK(h.depth() == 0);
}
