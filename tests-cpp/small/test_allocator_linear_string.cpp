// Direct C++ unit tests for `das::LinearStringAllocator` — the linear
// (bump) string heap. Free is a no-op; reset releases everything.

#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/simulate/heap.h"

#include <cstring>

using namespace das;

TEST_CASE("LinearStringAllocator: allocateString writes a null-terminated copy") {
    LinearStringAllocator h;
    const char * src = "linear strings";
    uint64_t len = strlen(src);
    char * s = h.impl_allocateString(nullptr, src, len, nullptr);
    REQUIRE(s != nullptr);
    CHECK(strcmp(s, src) == 0);
    CHECK(s[len] == 0);
}

TEST_CASE("LinearStringAllocator: distinct allocations get distinct pointers") {
    LinearStringAllocator h;
    char * a = h.impl_allocateString(nullptr, "aa", 2, nullptr);
    char * b = h.impl_allocateString(nullptr, "bb", 2, nullptr);
    REQUIRE(a != nullptr);
    REQUIRE(b != nullptr);
    CHECK(a != b);
}

TEST_CASE("LinearStringAllocator: empty allocation returns nullptr") {
    LinearStringAllocator h;
    CHECK(h.impl_allocateString(nullptr, "", 0, nullptr) == nullptr);
}

TEST_CASE("LinearStringAllocator: forEachString visits everything allocated") {
    LinearStringAllocator h;
    const char * srcs[] = { "alpha", "beta", "gamma" };
    for ( const char * s : srcs ) {
        REQUIRE(h.impl_allocateString(nullptr, s, strlen(s), nullptr) != nullptr);
    }
    int count = 0;
    h.forEachString([&](const char * /*s*/) { count++; });
    CHECK(count == 3);
}

TEST_CASE("LinearStringAllocator: reset releases the linear chunks") {
    LinearStringAllocator h;
    for ( int i=0; i<50; i++ ) {
        REQUIRE(h.impl_allocateString(nullptr, "x", 1, nullptr) != nullptr);
    }
    CHECK(h.bytesAllocated() > 0);
    h.reset();
    CHECK(h.bytesAllocated() == 0);
}

TEST_CASE("LinearStringAllocator: setInitialSize accepts uint64_t (widened)") {
    LinearStringAllocator h;
    h.setInitialSize(uint64_t(64 * 1024));
    CHECK(h.getInitialSize() == int64_t(64 * 1024));
}

TEST_CASE("LinearStringAllocator: mark() returns false (linear cannot GC)") {
    LinearStringAllocator h;
    CHECK(!h.mark());
}
