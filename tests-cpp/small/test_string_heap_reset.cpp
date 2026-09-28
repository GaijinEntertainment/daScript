#include <doctest/doctest.h>

#include "daScript/daScript.h"

using namespace das;

TEST_CASE("a linear string heap's reset forgets the strings it interned") {
    LinearStringAllocator strings;
    strings.setIntern(true);
    REQUIRE(strings.impl_allocateString(nullptr, "abc", 3) != nullptr);
    REQUIRE(strings.intern("abc", 3) != nullptr);
    strings.reset();
    CHECK(strings.intern("abc", 3) == nullptr);
}

TEST_CASE("a persistent string heap's reset forgets the strings it interned") {
    PersistentStringAllocator strings;
    strings.setIntern(true);
    REQUIRE(strings.impl_allocateString(nullptr, "abc", 3) != nullptr);
    REQUIRE(strings.intern("abc", 3) != nullptr);
    strings.reset();
    CHECK(strings.intern("abc", 3) == nullptr);
}
