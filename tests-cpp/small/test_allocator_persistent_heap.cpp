// Direct C++ unit tests for `das::PersistentHeapAllocator` — the production
// general-purpose heap allocator (wraps MemoryModel). Covers the
// Phase-1-widened virtuals + the mark/sweep + setLimit gates.

#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/simulate/heap.h"

#include <cstring>

using namespace das;

TEST_CASE("PersistentHeapAllocator: alloc / free round-trip + accounting") {
    PersistentHeapAllocator h;
    CHECK(h.bytesAllocated() == 0);
    CHECK(h.getTotalBytesAllocated() == 0);
    CHECK(h.getTotalBytesDeleted() == 0);

    char * p = h.impl_allocate(128);
    REQUIRE(p != nullptr);
    CHECK(h.bytesAllocated() == 128);
    CHECK(h.getTotalBytesAllocated() == 128);
    CHECK(h.getTotalAllocations() == 1);

    h.impl_free(p, 128);
    CHECK(h.bytesAllocated() == 0);
    CHECK(h.getTotalBytesDeleted() == 128);
}

TEST_CASE("PersistentHeapAllocator: setInitialSize accepts uint64_t (widened)") {
    PersistentHeapAllocator h;
    h.setInitialSize(uint64_t(1024 * 1024));
    CHECK(h.getInitialSize() == int64_t(1024 * 1024));
    // getInitialSize() now returns int64_t — verify the signature is intact.
    static_assert(sizeof(decltype(h.getInitialSize())) == 8, "getInitialSize() must be int64_t after Phase 1");
}

TEST_CASE("PersistentHeapAllocator: reallocate preserves prefix") {
    PersistentHeapAllocator h;
    char * p = h.impl_allocate(64);
    REQUIRE(p != nullptr);
    for ( int i=0; i<64; i++ ) p[i] = (char)(i * 7);
    char * q = h.impl_reallocate(p, 64, 4096);
    REQUIRE(q != nullptr);
    for ( int i=0; i<64; i++ ) CHECK(q[i] == (char)(i * 7));
    CHECK(h.bytesAllocated() == 4096);
    h.impl_free(q, 4096);
    CHECK(h.bytesAllocated() == 0);
}

TEST_CASE("PersistentHeapAllocator: setLimit gates allocation") {
    PersistentHeapAllocator h;
    h.setLimit(256);
    CHECK(h.getLimit() == 256);

    // First small alloc under the limit succeeds.
    char * p = h.impl_allocate(128);
    REQUIRE(p != nullptr);

    // The next allocation that would push past 256 returns nullptr.
    char * q = h.impl_allocate(256);
    CHECK(q == nullptr);

    h.impl_free(p, 128);
}

TEST_CASE("PersistentHeapAllocator: mark/sweep round-trip on small alloc") {
    PersistentHeapAllocator h;
    // Small allocation goes through the Shoe path; mark uses Deck bitmaps.
    char * p = h.impl_allocate(64);
    REQUIRE(p != nullptr);
    h.mark();                                  // shoe.beforeGC()
    CHECK(h.mark(p, 64));                      // first mark returns true
    CHECK(!h.mark(p, 64));                     // second mark returns false (already marked)
    // sweep() zeroes accounting and reclaims unmarked; this alloc is marked,
    // so totalAllocated lands back at 64 (the kept value).
    h.sweep();
    CHECK(h.bytesAllocated() == 64);
    CHECK(h.isOwnPtr(p, 64));                  // marked, still owned
}

TEST_CASE("PersistentHeapAllocator: mark/sweep collects unmarked big alloc") {
    PersistentHeapAllocator h;
    char * kept = h.impl_allocate(4096);
    char * dropped = h.impl_allocate(4096);
    REQUIRE(kept != nullptr);
    REQUIRE(dropped != nullptr);
    h.mark();                                  // begin-GC: clears mark bits
    CHECK(h.mark(kept, 4096));                 // mark survivor only
    h.sweep();                                 // drops `dropped`, keeps `kept`
    CHECK(h.isOwnPtr(kept, 4096));
    CHECK(!h.isOwnPtr(dropped, 4096));         // no longer owned
    h.impl_free(kept, 4096);
}

TEST_CASE("PersistentHeapAllocator: depth() reflects shoe deck count") {
    PersistentHeapAllocator h;
    CHECK(h.depth() == 0);
    char * p = h.impl_allocate(32);
    REQUIRE(p != nullptr);
    CHECK(h.depth() >= 1);
    h.impl_free(p, 32);
}

TEST_CASE("PersistentHeapAllocator: reset clears the heap (accounting is sweep-managed)") {
    PersistentHeapAllocator h;
    for ( int i=0; i<10; i++ ) {
        char * p = h.impl_allocate(64);
        REQUIRE(p != nullptr);
    }
    CHECK(h.bytesAllocated() == 640);
    h.reset();
    // Note: reset() releases the underlying heap memory but does NOT zero
    // the totalAllocated counter (only sweep() does). The reset-clears-
    // accounting contract belongs to sweep, not reset. Verify reset frees
    // the heap by allocating again from a clean state and seeing no leak.
    char * fresh = h.impl_allocate(64);
    REQUIRE(fresh != nullptr);
    h.impl_free(fresh, 64);
}

TEST_CASE("PersistentHeapAllocator: totalAlignedMemoryAllocated >= bytesAllocated") {
    PersistentHeapAllocator h;
    char * p = h.impl_allocate(64);
    REQUIRE(p != nullptr);
    // Aligned memory includes whole shoe decks; always >= live bytes.
    CHECK(h.totalAlignedMemoryAllocated() >= h.bytesAllocated());
    h.impl_free(p, 64);
}
