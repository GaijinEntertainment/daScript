// Direct C++ unit tests for `das::StackAllocator` — the per-context call
// stack. STAYS uint32 by design (per-stack scope, bounded by setup-time
// stackSize). Phase 1 did NOT widen this allocator. The tests lock down
// the existing behavior so future widening work doesn't accidentally
// break it.

#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/simulate/heap.h"

using namespace das;

TEST_CASE("StackAllocator: construct with size, allocate, and reset") {
    StackAllocator s(4096);
    CHECK(s.size() == 4096);
    CHECK(s.empty());                       // empty = stackTop at top
    CHECK(s.free_size() == 4096);
}

TEST_CASE("StackAllocator: push consumes from top; pop restores") {
    StackAllocator s(4096);
    char *EP = nullptr, *SP = nullptr;
    CHECK(s.push(64, EP, SP));
    CHECK(!s.empty());                      // top moved
    CHECK(s.free_size() == 4096 - 64);
    s.pop(EP, SP);
    CHECK(s.empty());
    CHECK(s.free_size() == 4096);
}

TEST_CASE("StackAllocator: push returns false when out of space") {
    StackAllocator s(128);
    char *EP = nullptr, *SP = nullptr;
    CHECK(s.push(128, EP, SP));
    char *EP2 = nullptr, *SP2 = nullptr;
    CHECK(!s.push(1, EP2, SP2));            // no room left
    s.pop(EP, SP);
}

TEST_CASE("StackAllocator: spi / api report execution + allocation offsets") {
    StackAllocator s(4096);
    CHECK(s.spi() == 4096);                 // evalTop at top initially
    CHECK(s.api() == 4096);                 // stackTop at top
    char *EP = nullptr, *SP = nullptr;
    REQUIRE(s.push(256, EP, SP));
    CHECK(s.api() == 4096 - 256);           // stackTop dropped
    CHECK(s.spi() == s.api());              // evalTop tracks stackTop
    s.pop(EP, SP);
}

TEST_CASE("StackAllocator: invoke moves evalTop without changing stackTop") {
    StackAllocator s(4096);
    char *EP = nullptr, *SP = nullptr;
    s.invoke(128, EP, SP);                  // evalTop = bottom+128
    CHECK(s.spi() == 128);                  // execution offset shifted
    CHECK(s.api() == 4096);                 // allocation top untouched
    s.pop(EP, SP);
}

TEST_CASE("StackAllocator: push_invoke combines push + invoke") {
    StackAllocator s(4096);
    char *EP = nullptr, *SP = nullptr;
    CHECK(s.push_invoke(256, 128, EP, SP));
    CHECK(s.api() == 4096 - 256);
    CHECK(s.spi() == 128);
    s.pop(EP, SP);
    CHECK(s.empty());
}

TEST_CASE("StackAllocator: is_stack_ptr tells stack from heap") {
    StackAllocator s(4096);
    char *EP = nullptr, *SP = nullptr;
    REQUIRE(s.push(64, EP, SP));
    char * top = s.ap();
    CHECK(s.is_stack_ptr(top));
    char foreign = 0;
    CHECK(!s.is_stack_ptr(&foreign));
    s.pop(EP, SP);
}

TEST_CASE("StackAllocator: top - bottom == size") {
    StackAllocator s(4096);
    CHECK(uintptr_t(s.top() - s.bottom()) == 4096);
}

TEST_CASE("StackAllocator: watermark snapshots EP/SP without modifying state") {
    StackAllocator s(4096);
    char *EP = nullptr, *SP = nullptr;
    s.watermark(EP, SP);
    CHECK(EP == s.sp());
    CHECK(SP == s.ap());
}
