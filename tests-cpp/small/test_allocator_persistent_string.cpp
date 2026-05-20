// Direct C++ unit tests for `das::PersistentStringAllocator` — the
// production string heap. Phase 1 invariant: the intern path bypasses
// gracefully when length > UINT32_MAX (StrHashEntry::length is uint32).
// The underlying allocation path is 64-bit-safe.

#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/simulate/heap.h"

#include <cstring>

using namespace das;

TEST_CASE("PersistentStringAllocator: allocateString writes null-terminated copy") {
    PersistentStringAllocator h;
    const char * src = "hello, daslang";
    uint64_t len = strlen(src);
    char * s = h.impl_allocateString(nullptr, src, len, nullptr);
    REQUIRE(s != nullptr);
    CHECK(strcmp(s, src) == 0);
    CHECK(s[len] == 0);
    h.impl_freeString(s, len);
}

TEST_CASE("PersistentStringAllocator: empty allocation returns nullptr") {
    PersistentStringAllocator h;
    CHECK(h.impl_allocateString(nullptr, "", 0, nullptr) == nullptr);
}

TEST_CASE("PersistentStringAllocator: intern disabled by default") {
    PersistentStringAllocator h;
    CHECK(!h.isIntern());
    const char * src = "duplicate me";
    char * a = h.impl_allocateString(nullptr, src, strlen(src), nullptr);
    char * b = h.impl_allocateString(nullptr, src, strlen(src), nullptr);
    REQUIRE(a != nullptr);
    REQUIRE(b != nullptr);
    // Without intern, duplicates get distinct storage.
    CHECK(a != b);
    h.impl_freeString(a, strlen(src));
    h.impl_freeString(b, strlen(src));
}

TEST_CASE("PersistentStringAllocator: setIntern(true) deduplicates equal strings") {
    PersistentStringAllocator h;
    h.setIntern(true);
    CHECK(h.isIntern());
    const char * src = "interned";
    char * a = h.impl_allocateString(nullptr, src, strlen(src), nullptr);
    char * b = h.impl_allocateString(nullptr, src, strlen(src), nullptr);
    REQUIRE(a != nullptr);
    REQUIRE(b != nullptr);
    CHECK(a == b);                         // intern hit
    h.impl_freeString(a, strlen(src));
}

TEST_CASE("PersistentStringAllocator: intern() lookup hits after allocate") {
    PersistentStringAllocator h;
    h.setIntern(true);
    const char * src = "lookup-me";
    uint64_t len = strlen(src);
    char * s = h.impl_allocateString(nullptr, src, len, nullptr);
    REQUIRE(s != nullptr);
    CHECK(h.intern(src, len) == s);
    h.impl_freeString(s, len);
}

TEST_CASE("PersistentStringAllocator: intern() lookup misses when intern is off") {
    PersistentStringAllocator h;
    // intern() should return nullptr when needIntern == false even if the
    // string is in the heap — intern bookkeeping only runs in intern mode.
    CHECK(h.intern("nope", 4) == nullptr);
}

TEST_CASE("PersistentStringAllocator: setInitialSize accepts uint64_t (widened)") {
    PersistentStringAllocator h;
    h.setInitialSize(uint64_t(64 * 1024));
    CHECK(h.getInitialSize() == int64_t(64 * 1024));
    static_assert(sizeof(decltype(h.getInitialSize())) == 8, "getInitialSize() must be int64_t after Phase 1");
}

TEST_CASE("PersistentStringAllocator: reset clears the heap and intern table") {
    // Regression test for a latent bug: before the fix in heap.cpp,
    // PersistentStringAllocator::reset() only called model.reset() and
    // left internMap populated with pointers into now-reusable slots.
    // The next allocateString of a previously-interned string would hit
    // the dangling entry and return a stale pointer — silent corruption
    // in Release, Deck::free double-free assert in Debug.
    PersistentStringAllocator h;
    h.setIntern(true);
    char * a = h.impl_allocateString(nullptr, "alpha", 5, nullptr);
    char * b = h.impl_allocateString(nullptr, "beta", 4, nullptr);
    REQUIRE(a != nullptr);
    REQUIRE(b != nullptr);
    CHECK(h.bytesAllocated() > 0);
    CHECK(h.intern("alpha", 5) == a);              // intern hits pre-reset

    h.reset();
    // Note: reset() does NOT zero the totalAllocated counter (only sweep()
    // owns that contract) — but the intern table IS now cleared, and the
    // heap memory IS released, so any subsequent allocation lands in fresh
    // storage with the slot bookkeeping in sync.
    CHECK(h.intern("alpha", 5) == nullptr);        // intern table is empty

    // Round-trip a new allocation of the SAME interned string — this is the
    // exact path that triggered the latent bug (intern hit → dangling ptr →
    // free crashes on already-free slot).
    char * a2 = h.impl_allocateString(nullptr, "alpha", 5, nullptr);
    REQUIRE(a2 != nullptr);
    h.impl_freeString(a2, 5);                       // must not assert
}

TEST_CASE("PersistentStringAllocator: forEachString enumerates live strings") {
    PersistentStringAllocator h;
    char * a = h.impl_allocateString(nullptr, "x", 1, nullptr);
    char * b = h.impl_allocateString(nullptr, "yy", 2, nullptr);
    REQUIRE(a != nullptr);
    REQUIRE(b != nullptr);
    int count = 0;
    h.forEachString([&](const char * /*str*/) { count++; });
    CHECK(count == 2);
    h.impl_freeString(a, 1);
    h.impl_freeString(b, 2);
}
