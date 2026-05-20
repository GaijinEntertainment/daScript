// Direct C++ unit tests for `AnyHeapAllocator::impl_allocateIterator` /
// `impl_freeIterator` — the iterator-allocation helper that prepends a
// 16-byte header storing the iterator's body size.
//
// Phase 1 invariant: the prepended size cell is now uint64_t (was uint32).
// The header total stays 16 bytes — the size goes in the first 8, the
// remaining 8 are padding/alignment slack.

#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/simulate/heap.h"

#include <cstring>

using namespace das;

TEST_CASE("impl_allocateIterator: user pointer sits at +16 from prefix") {
    PersistentHeapAllocator h;
    char * iter = h.impl_allocateIterator(64);
    REQUIRE(iter != nullptr);
    char * prefix = iter - 16;
    // The first 8 bytes of the prefix encode the body size after Phase 1.
    uint64_t storedSize = *((uint64_t *)prefix);
    CHECK(storedSize == 64);
    // Free reads the stored size and calls impl_free(prefix, size+16).
    h.impl_freeIterator(iter);
}

TEST_CASE("impl_allocateIterator: stored size survives free's readback") {
    // Phase 1 widening: the readback in impl_freeIterator must use the same
    // uint64_t cell as the write in impl_allocateIterator. If they
    // diverged, the wrong size would flow to impl_free → assert/crash.
    PersistentHeapAllocator h;
    char * iter1 = h.impl_allocateIterator(48);
    char * iter2 = h.impl_allocateIterator(256);
    char * iter3 = h.impl_allocateIterator(1024);
    REQUIRE(iter1 != nullptr);
    REQUIRE(iter2 != nullptr);
    REQUIRE(iter3 != nullptr);
    CHECK(*((uint64_t *)(iter1 - 16)) == 48);
    CHECK(*((uint64_t *)(iter2 - 16)) == 256);
    CHECK(*((uint64_t *)(iter3 - 16)) == 1024);
    h.impl_freeIterator(iter1);
    h.impl_freeIterator(iter2);
    h.impl_freeIterator(iter3);
    CHECK(h.bytesAllocated() == 0);          // round-trip is clean
}

TEST_CASE("impl_allocateIterator: user data is writable and isolated") {
    PersistentHeapAllocator h;
    char * iter = h.impl_allocateIterator(128);
    REQUIRE(iter != nullptr);
    // The 128 bytes at `iter` are the iterator body; the test writes a
    // pattern and reads it back to confirm no overlap with the prefix.
    memset(iter, 0xA5, 128);
    for ( int i=0; i<128; i++ ) {
        CHECK((unsigned char)iter[i] == 0xA5);
    }
    // Prefix's size cell must remain intact under the writes.
    CHECK(*((uint64_t *)(iter - 16)) == 128);
    h.impl_freeIterator(iter);
}

TEST_CASE("impl_allocateIterator: LinearHeapAllocator path") {
    // Linear allocator also routes through impl_allocateIterator (inherited
    // from AnyHeapAllocator). Verify the contract holds there too.
    LinearHeapAllocator h;
    char * iter = h.impl_allocateIterator(96);
    REQUIRE(iter != nullptr);
    CHECK(*((uint64_t *)(iter - 16)) == 96);
    // Linear free is a no-op for the body; bytes don't drop.
    h.impl_freeIterator(iter);
}
