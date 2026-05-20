// Direct C++ unit tests for `das::NodePrefix` and `das::NodeAllocator` —
// the size-tracking prefix written before every SimNode and AST node.
//
// Phase 1 invariants under test:
//   - sizeof(NodePrefix) == sizeof(vec4f)  (kept 16 bytes by dropping padd1)
//   - NodePrefix::size widened to uint64_t
//   - NodeAllocator::makeNode<T> places NodePrefix immediately before TT,
//     and prefix->size correctly stores sizeof(TT).

#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/simulate/heap.h"

using namespace das;

namespace {
struct AlignmentProbe {
    int  field0;
    int  field1;
    char field2;
    AlignmentProbe() : field0(0), field1(0), field2(0) {}
};
}

TEST_CASE("NodePrefix: sizeof matches vec4f after widening") {
    static_assert(sizeof(NodePrefix) == sizeof(vec4f), "NodePrefix sizeof drifted from vec4f");
    CHECK(sizeof(NodePrefix) == sizeof(vec4f));
}

TEST_CASE("NodePrefix: size field is uint64_t (Phase 1 widening)") {
    NodePrefix p{size_t(42)};                  // brace-init avoids most-vexing-parse
    CHECK(p.size == 42);
    static_assert(sizeof(p.size) == 8, "NodePrefix::size must be uint64_t after Phase 1");
    static_assert(sizeof(p.magic) == 4, "NodePrefix::magic stays uint32_t");
}

TEST_CASE("NodePrefix: stores size > UINT32_MAX without truncation") {
    // The whole point of the widening — a single per-node size can exceed
    // 4GB in principle (no realistic SimNode does, but the field type is
    // load-bearing for future big-node patterns).
    if constexpr ( sizeof(size_t) < 8 ) {
        // 32-bit builds: size_t can't hold the value, so the constructor
        // path can't actually be exercised. The widened storage is still
        // confirmed by the uint64-direct test below.
        WARN("32-bit build: size_t too narrow to exercise the constructor path");
    } else {
        const uint64_t big = (uint64_t(1) << 33) + 17;
        NodePrefix p{size_t(big)};
        CHECK(p.size == big);
        CHECK((p.size >> 32) != 0);
    }
    // Independent check: the field itself can hold a >UINT32_MAX value via
    // direct assignment, regardless of host bitness.
    NodePrefix q{size_t(0)};
    const uint64_t big2 = (uint64_t(1) << 35) + 5;
    q.size = big2;
    CHECK(q.size == big2);
    CHECK((q.size >> 32) != 0);
}

#ifndef DAS_NO_ASSERTIONS
TEST_CASE("NodePrefix: magic constant is set in debug builds") {
    NodePrefix p{size_t(1)};
    CHECK(p.magic == 0xdeadc0de);
}
#endif

TEST_CASE("NodeAllocator: makeNode<T> stamps a prefix that reads back as sizeof(T)") {
    NodeAllocator a;
    a.setInitialSize(64 * 1024);
    CHECK(a.prefixWithHeader == true);   // default

    AlignmentProbe * node = a.makeNode<AlignmentProbe>();
    REQUIRE(node != nullptr);

    // Prefix sits immediately before the node — that's the contract used
    // throughout the simulator (e.g. simulate.cpp's prefix->size walks).
    NodePrefix * prefix = ((NodePrefix *)node) - 1;
    CHECK(prefix->size == sizeof(AlignmentProbe));
#ifndef DAS_NO_ASSERTIONS
    CHECK(prefix->magic == 0xdeadc0de);
#endif
}

TEST_CASE("NodeAllocator: totalNodesAllocated counter increments per makeNode") {
    NodeAllocator a;
    a.setInitialSize(64 * 1024);
    CHECK(a.totalNodesAllocated == 0);
    a.makeNode<AlignmentProbe>();
    a.makeNode<AlignmentProbe>();
    a.makeNode<AlignmentProbe>();
    CHECK(a.totalNodesAllocated == 3);
}

TEST_CASE("NodeAllocator: prefixWithHeader=false skips the prefix") {
    NodeAllocator a;
    a.setInitialSize(64 * 1024);
    a.prefixWithHeader = false;
    AlignmentProbe * node = a.makeNode<AlignmentProbe>();
    REQUIRE(node != nullptr);
    // No prefix — the previous bytes belong to the chunk header or
    // a different allocation. We just confirm the node itself is usable.
    node->field0 = 17;
    CHECK(node->field0 == 17);
}

TEST_CASE("NodeAllocator: node data is 16-byte aligned after prefix") {
    // The whole point of keeping sizeof(NodePrefix) == sizeof(vec4f) (16).
    // If padding were wrong, `node` would not be 16-aligned and SimNode
    // vec4f fields would crash on aligned-load.
    NodeAllocator a;
    a.setInitialSize(64 * 1024);
    AlignmentProbe * node = a.makeNode<AlignmentProbe>();
    REQUIRE(node != nullptr);
    CHECK((uintptr_t(node) & uintptr_t(15)) == 0);
}
