// Direct C++ unit tests for `das::MemoryModel` — the byte-level allocator
// underneath every persistent/string heap. Covers the small (Shoe) path,
// the large (bigStuff) path, mark/sweep, and the Phase 1 invariants:
//
//   - bigStuff value type is uint64_t (can store > UINT32_MAX size)
//   - DAS_PAGE_GC_MASK is now bit 63 — packing/unpacking still round-trips
//     when sizes use the wider range
//
// These tests target MemoryModel directly (not via Context); no daslang
// program is compiled.

#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/misc/memory_model.h"

#include <cstring>

using namespace das;

TEST_CASE("MemoryModel: default-construct state") {
    MemoryModel mm;
    CHECK(mm.bytesAllocated() == 0);
    CHECK(mm.maxBytesAllocated() == 0);
    CHECK(mm.depth() == 0);
    CHECK(mm.bigStuff.empty());
}

TEST_CASE("MemoryModel: setInitialSize accepts uint64_t (widened)") {
    MemoryModel mm;
    mm.setInitialSize(uint64_t(1024 * 1024));
    CHECK(mm.initialSize == uint64_t(1024 * 1024));
}

TEST_CASE("MemoryModel: allocate(0) returns null, accounting unchanged") {
    MemoryModel mm;
    CHECK(mm.allocate(0) == nullptr);
    CHECK(mm.bytesAllocated() == 0);
}

TEST_CASE("MemoryModel: small allocation routes through Shoe") {
    MemoryModel mm;
    // 64 bytes is well below the 256B Shoe ceiling.
    char * p = mm.allocate(64);
    REQUIRE(p != nullptr);
    CHECK(mm.bytesAllocated() == 64);
    CHECK(mm.maxBytesAllocated() == 64);
    CHECK(mm.isOwnPtr(p, 64));
    CHECK(mm.isAllocatedPtr(p, 64));
    CHECK(mm.bigStuff.empty());          // small → Shoe, not bigStuff
    CHECK(mm.depth() >= 1);              // Shoe created at least one Deck

    CHECK(mm.free(p, 64));
    CHECK(mm.bytesAllocated() == 0);
    CHECK(mm.maxBytesAllocated() == 64); // watermark preserved
}

TEST_CASE("MemoryModel: large allocation routes through bigStuff") {
    MemoryModel mm;
    // 4096 bytes exceeds DAS_MAX_SHOE_ALLOCATION (256), forces bigStuff.
    char * p = mm.allocate(4096);
    REQUIRE(p != nullptr);
    CHECK(mm.bytesAllocated() == 4096);
    CHECK(mm.bigStuff.size() == 1);
    auto it = mm.bigStuff.find(p);
    REQUIRE(it != mm.bigStuff.end());
    CHECK(it->second == 4096);           // value is the allocation size
    CHECK(mm.isOwnPtr(p, 4096));

    CHECK(mm.free(p, 4096));
    CHECK(mm.bytesAllocated() == 0);
    CHECK(mm.bigStuff.empty());
}

TEST_CASE("MemoryModel: bigStuff value type holds > UINT32_MAX (Phase 1 invariant)") {
    // This is the critical Phase 1 widening invariant: bigStuff is now
    // das_hash_map<void*, uint64_t>, so a stored allocation size can exceed
    // UINT32_MAX without wrap-around. We don't actually allocate >4GB
    // (gated tests do that) — we manually insert into the map and read
    // back to confirm the value-type semantics survived the widening.
    MemoryModel mm;
    char dummy = 0;
    const uint64_t big = (uint64_t(1) << 33) + 7;   // ~8.6 GB
    mm.bigStuff[&dummy] = big;
    auto it = mm.bigStuff.find(&dummy);
    REQUIRE(it != mm.bigStuff.end());
    CHECK(it->second == big);
    CHECK((it->second >> 32) != 0);                  // upper 32 bits survive
    // The destructor walks bigStuff and das_aligned_free16(entry.first); our
    // stack-pointer dummy would crash that path. Erase before scope exit.
    mm.bigStuff.erase(&dummy);
}

TEST_CASE("MemoryModel: DAS_PAGE_GC_MASK packs at bit 63 (Phase 1 invariant)") {
    // The mark bit was at bit 31 before widening. Phase 1 moves it to
    // bit 63 so it doesn't collide with a >2GB allocation size. Verify
    // the packing/unpacking round-trips with a size in the gap.
    MemoryModel mm;
    char dummy = 0;
    const uint64_t size = (uint64_t(1) << 33);       // 8 GB — was a collision before
    mm.bigStuff[&dummy] = size;

    // Mark the entry — packed as size | mask
    auto it = mm.bigStuff.find(&dummy);
    REQUIRE(it != mm.bigStuff.end());
    CHECK((it->second & DAS_PAGE_GC_MASK) == 0);     // unmarked initially
    it->second |= DAS_PAGE_GC_MASK;
    CHECK((it->second & DAS_PAGE_GC_MASK) != 0);     // marked
    CHECK((it->second & ~DAS_PAGE_GC_MASK) == size); // size still recoverable

    // Sanity: the mask is bit 63.
    CHECK(DAS_PAGE_GC_MASK == (uint64_t(1) << 63));
    // Same cleanup as above — stack-pointer entry must not survive scope exit.
    mm.bigStuff.erase(&dummy);
}

TEST_CASE("MemoryModel: reallocate preserves prefix and updates accounting") {
    MemoryModel mm;
    char * p = mm.allocate(128);
    REQUIRE(p != nullptr);
    for ( int i=0; i<128; i++ ) p[i] = (char)i;

    char * q = mm.reallocate(p, 128, 512);
    REQUIRE(q != nullptr);
    for ( int i=0; i<128; i++ ) CHECK(q[i] == (char)i);
    CHECK(mm.bytesAllocated() == 512);

    char * r = mm.reallocate(q, 512, 64);             // shrink path
    REQUIRE(r != nullptr);
    for ( int i=0; i<64; i++ ) CHECK(r[i] == (char)i);
    CHECK(mm.bytesAllocated() == 64);

    CHECK(mm.free(r, 64));
}

TEST_CASE("MemoryModel: reallocate of null is just allocate") {
    MemoryModel mm;
    char * p = mm.reallocate(nullptr, 0, 256);
    REQUIRE(p != nullptr);
    CHECK(mm.bytesAllocated() == 256);
    CHECK(mm.free(p, 256));
}

TEST_CASE("MemoryModel: reset() clears small and large allocations") {
    MemoryModel mm;
    char * small1 = mm.allocate(32);
    char * small2 = mm.allocate(48);
    char * big = mm.allocate(8192);
    REQUIRE(small1 != nullptr);
    REQUIRE(small2 != nullptr);
    REQUIRE(big != nullptr);
    CHECK(mm.bytesAllocated() > 0);
    CHECK(!mm.bigStuff.empty());

    mm.reset();
    CHECK(mm.bigStuff.empty());
    // After reset, subsequent allocations succeed from a clean state.
    char * fresh = mm.allocate(64);
    REQUIRE(fresh != nullptr);
    CHECK(mm.free(fresh, 64));
}

TEST_CASE("MemoryModel: sweep() drops unmarked bigStuff entries (mark/sweep round-trip)") {
    // The bigStuff sweep path is where the bit-63 mask move matters most.
    // Allocate two big blocks, mark one, sweep, confirm the marked stays
    // and the unmarked is collected.
    MemoryModel mm;
    char * kept = mm.allocate(4096);
    char * dropped = mm.allocate(4096);
    REQUIRE(kept != nullptr);
    REQUIRE(dropped != nullptr);
    CHECK(mm.bigStuff.size() == 2);

    // Mark only `kept`.
    auto itKept = mm.bigStuff.find(kept);
    REQUIRE(itKept != mm.bigStuff.end());
    itKept->second |= DAS_PAGE_GC_MASK;

    mm.sweep();
    CHECK(mm.bigStuff.size() == 1);
    CHECK(mm.bigStuff.find(kept) != mm.bigStuff.end());
    CHECK(mm.bigStuff.find(dropped) == mm.bigStuff.end());

    // After sweep the mark bit is cleared from kept; size is recoverable.
    auto itAfter = mm.bigStuff.find(kept);
    REQUIRE(itAfter != mm.bigStuff.end());
    CHECK((itAfter->second & DAS_PAGE_GC_MASK) == 0);
    CHECK(itAfter->second == 4096);

    CHECK(mm.free(kept, 4096));
}

TEST_CASE("MemoryModel: totalAlignedMemoryAllocated() includes shoe + bigStuff") {
    MemoryModel mm;
    char * s = mm.allocate(64);
    char * b = mm.allocate(8192);
    REQUIRE(s != nullptr);
    REQUIRE(b != nullptr);
    // The shoe path allocates more than the requested size (whole decks),
    // so totalAligned is >= the sum of requested. Sanity: at least the
    // big block contributes.
    CHECK(mm.totalAlignedMemoryAllocated() >= 8192);
    CHECK(mm.free(s, 64));
    CHECK(mm.free(b, 8192));
}

TEST_CASE("MemoryModel: isOwnPtr distinguishes owned vs foreign pointers") {
    MemoryModel mm;
    char * mine = mm.allocate(128);
    REQUIRE(mine != nullptr);
    // Foreign pointer probe MUST use a valid shoe-size (multiple of 16,
    // 16..256) — Shoe::isOwnPtr asserts size && size<=256 and indexes
    // chunks[(size>>4)-1]. Passing size=1 underflows to chunks[~0u].
    char foreign[128] = {0};
    CHECK(mm.isOwnPtr(mine, 128));
    CHECK(!mm.isOwnPtr(foreign, 128));
    CHECK(mm.free(mine, 128));
}
