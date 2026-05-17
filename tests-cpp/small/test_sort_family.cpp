// Template-level tests for the sort family in src/builtin/das_qsort_r.h.
//
// Covers:
//   - das_qsort_r        (baseline — existed pre-Phase-0 but had no cpp test)
//   - das_partial_sort_r (Phase 0 — introselect with smoothsort tail)
//   - das_nth_element_r  (Phase 0 — quickselect with introselect bound)
//   - das_make_heap_r / das_push_heap_r / das_pop_heap_r (Phase 0 — binary max-heap)
//
// These templates are the only path for partial_sort / nth_element / heap ops
// on user-defined struct types (std::sort templates on iterator type, not
// void*+width). Without these tests a user-struct regression goes unnoticed
// until a daslang test exercises the any-cblock binding path.

#include <doctest/doctest.h>

#include "../../src/builtin/das_qsort_r.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <vector>

using namespace das;

namespace {

    // 16-byte struct payload — exercises the byte_swap chunked-memcpy path
    // on widths that don't match a primitive type.
    struct Quad {
        int32_t key;
        int32_t payload_a;
        int32_t payload_b;
        int32_t payload_c;
    };

    template <typename T>
    auto less_cmp() {
        return [](const void * a, const void * b) {
            return *static_cast<const T*>(a) < *static_cast<const T*>(b);
        };
    }

    auto less_quad() {
        return [](const void * a, const void * b) {
            return static_cast<const Quad*>(a)->key < static_cast<const Quad*>(b)->key;
        };
    }

    template <typename T>
    bool is_sorted_range(const T * data, size_t n) {
        for (size_t i = 1; i < n; i++) if (data[i] < data[i-1]) return false;
        return true;
    }

} // namespace

// ============================================================================
// das_qsort_r baseline (no prior cpp coverage)
// ============================================================================

TEST_CASE("das_qsort_r baseline") {
    SUBCASE("int random") {
        std::vector<int32_t> a = { 5, 2, 8, 1, 9, 3, 7, 4, 6, 0 };
        das_qsort_r(a.data(), a.size(), sizeof(int32_t), less_cmp<int32_t>());
        CHECK(is_sorted_range(a.data(), a.size()));
    }
    SUBCASE("int already sorted") {
        std::vector<int32_t> a = { 1, 2, 3, 4, 5 };
        das_qsort_r(a.data(), a.size(), sizeof(int32_t), less_cmp<int32_t>());
        CHECK(is_sorted_range(a.data(), a.size()));
    }
    SUBCASE("int reverse sorted") {
        std::vector<int32_t> a = { 9, 7, 5, 3, 1 };
        das_qsort_r(a.data(), a.size(), sizeof(int32_t), less_cmp<int32_t>());
        CHECK(is_sorted_range(a.data(), a.size()));
    }
    SUBCASE("empty / single") {
        std::vector<int32_t> empty;
        das_qsort_r(empty.data(), empty.size(), sizeof(int32_t), less_cmp<int32_t>());
        CHECK(empty.size() == 0);
        std::vector<int32_t> one = { 42 };
        das_qsort_r(one.data(), one.size(), sizeof(int32_t), less_cmp<int32_t>());
        CHECK(one[0] == 42);
    }
    SUBCASE("16-byte struct by key field") {
        std::vector<Quad> a = {
            {5, 100, 200, 300},
            {2, 101, 201, 301},
            {8, 102, 202, 302},
            {1, 103, 203, 303},
        };
        das_qsort_r(a.data(), a.size(), sizeof(Quad), less_quad());
        CHECK(a[0].key == 1); CHECK(a[0].payload_a == 103);
        CHECK(a[1].key == 2); CHECK(a[1].payload_a == 101);
        CHECK(a[2].key == 5); CHECK(a[2].payload_a == 100);
        CHECK(a[3].key == 8); CHECK(a[3].payload_a == 102);
    }
    SUBCASE("descending comparator") {
        std::vector<int32_t> a = { 1, 2, 3, 4, 5 };
        auto desc = [](const void * x, const void * y) {
            return *static_cast<const int32_t*>(x) > *static_cast<const int32_t*>(y);
        };
        das_qsort_r(a.data(), a.size(), sizeof(int32_t), desc);
        CHECK(a[0] == 5); CHECK(a[1] == 4); CHECK(a[2] == 3); CHECK(a[3] == 2); CHECK(a[4] == 1);
    }
}

// ============================================================================
// das_partial_sort_r — first N sorted; remaining unspecified
// ============================================================================

TEST_CASE("das_partial_sort_r") {
    SUBCASE("N < length picks N smallest in order") {
        std::vector<int32_t> a = { 5, 2, 8, 1, 9, 3, 7, 4, 6, 0 };
        das_partial_sort_r(a.data(), a.size(), 4, sizeof(int32_t), less_cmp<int32_t>());
        // First 4 must be {0,1,2,3} in order.
        CHECK(a[0] == 0); CHECK(a[1] == 1); CHECK(a[2] == 2); CHECK(a[3] == 3);
        // Remaining must all be >= 3.
        for (size_t i = 4; i < a.size(); i++) CHECK(a[i] >= 3);
    }
    SUBCASE("N = length sorts entire array") {
        std::vector<int32_t> a = { 5, 2, 8, 1, 9 };
        das_partial_sort_r(a.data(), a.size(), a.size(), sizeof(int32_t), less_cmp<int32_t>());
        CHECK(is_sorted_range(a.data(), a.size()));
    }
    SUBCASE("N = 0 is a no-op") {
        std::vector<int32_t> a = { 5, 2, 8, 1, 9 };
        std::vector<int32_t> orig = a;
        das_partial_sort_r(a.data(), a.size(), 0, sizeof(int32_t), less_cmp<int32_t>());
        CHECK(a == orig);
    }
    SUBCASE("N > length clamps to full sort") {
        std::vector<int32_t> a = { 5, 2, 8, 1, 9 };
        das_partial_sort_r(a.data(), a.size(), 100, sizeof(int32_t), less_cmp<int32_t>());
        CHECK(is_sorted_range(a.data(), a.size()));
    }
    SUBCASE("N = 1 places min at position 0") {
        std::vector<int32_t> a = { 5, 2, 8, 1, 9, 3 };
        das_partial_sort_r(a.data(), a.size(), 1, sizeof(int32_t), less_cmp<int32_t>());
        CHECK(a[0] == 1);
    }
    SUBCASE("struct payload — first N by key, payload follows element") {
        std::vector<Quad> a = {
            {5, 100, 200, 300},
            {2, 101, 201, 301},
            {8, 102, 202, 302},
            {1, 103, 203, 303},
            {9, 104, 204, 304},
        };
        das_partial_sort_r(a.data(), a.size(), 3, sizeof(Quad), less_quad());
        CHECK(a[0].key == 1); CHECK(a[0].payload_a == 103);  // moved as a unit
        CHECK(a[1].key == 2); CHECK(a[1].payload_a == 101);
        CHECK(a[2].key == 5); CHECK(a[2].payload_a == 100);
    }
}

// ============================================================================
// das_nth_element_r — kth element at position k; left ≤ kth ≤ right
// ============================================================================

TEST_CASE("das_nth_element_r") {
    SUBCASE("k = 4 (median of 9)") {
        std::vector<int32_t> a = { 7, 3, 1, 9, 5, 8, 0, 4, 2 };
        das_nth_element_r(a.data(), a.size(), 4, sizeof(int32_t), less_cmp<int32_t>());
        // Position 4 must be the 5th-smallest = 4.
        CHECK(a[4] == 4);
        // All to the left ≤ 4.
        for (size_t i = 0; i < 4; i++) CHECK(a[i] <= 4);
        // All to the right ≥ 4.
        for (size_t i = 5; i < a.size(); i++) CHECK(a[i] >= 4);
    }
    SUBCASE("k = 0 puts min at front") {
        std::vector<int32_t> a = { 5, 2, 8, 1, 9, 3 };
        das_nth_element_r(a.data(), a.size(), 0, sizeof(int32_t), less_cmp<int32_t>());
        CHECK(a[0] == 1);
    }
    SUBCASE("k = length-1 puts max at end") {
        std::vector<int32_t> a = { 5, 2, 8, 1, 9, 3 };
        das_nth_element_r(a.data(), a.size(), 5, sizeof(int32_t), less_cmp<int32_t>());
        CHECK(a[5] == 9);
    }
    SUBCASE("already-sorted input stays correctly placed") {
        std::vector<int32_t> a = { 1, 2, 3, 4, 5, 6, 7, 8 };
        das_nth_element_r(a.data(), a.size(), 3, sizeof(int32_t), less_cmp<int32_t>());
        CHECK(a[3] == 4);
    }
    SUBCASE("reverse-sorted input is fixed") {
        std::vector<int32_t> a = { 8, 7, 6, 5, 4, 3, 2, 1 };
        das_nth_element_r(a.data(), a.size(), 3, sizeof(int32_t), less_cmp<int32_t>());
        CHECK(a[3] == 4);
    }
    SUBCASE("k out-of-range is no-op") {
        std::vector<int32_t> a = { 5, 2, 8, 1, 9 };
        std::vector<int32_t> orig = a;
        das_nth_element_r(a.data(), a.size(), 100, sizeof(int32_t), less_cmp<int32_t>());
        CHECK(a == orig);
    }
    SUBCASE("struct payload — kth-by-key carries payload") {
        std::vector<Quad> a = {
            {5, 100, 200, 300},
            {2, 101, 201, 301},
            {8, 102, 202, 302},
            {1, 103, 203, 303},
            {9, 104, 204, 304},
        };
        das_nth_element_r(a.data(), a.size(), 2, sizeof(Quad), less_quad());
        CHECK(a[2].key == 5);
        CHECK(a[2].payload_a == 100);  // payload came with element
    }
    SUBCASE("introselect depth bound — pathological-looking input") {
        // Worst-case-ish for plain quickselect (sorted plus reversed sections).
        // Just verify we get the right kth element regardless of pivot luck.
        std::vector<int32_t> a;
        for (int i = 0; i < 100; i++) a.push_back(i);
        for (int i = 99; i >= 0; i--) a.push_back(i);
        // a has 200 elements, two copies of 0..99. The median (k=99 zero-indexed
        // in a 200-element array) is 49.
        das_nth_element_r(a.data(), a.size(), 99, sizeof(int32_t), less_cmp<int32_t>());
        CHECK(a[99] == 49);
    }
}

// ============================================================================
// das_make_heap_r / das_push_heap_r / das_pop_heap_r — binary max-heap
// ============================================================================

namespace {

    template <typename T>
    bool is_max_heap(const T * data, size_t n) {
        for (size_t i = 0; i < n; i++) {
            size_t left = 2 * i + 1;
            size_t right = 2 * i + 2;
            if (left < n && data[i] < data[left]) return false;
            if (right < n && data[i] < data[right]) return false;
        }
        return true;
    }

} // namespace

TEST_CASE("das_make_heap_r — Floyd bottom-up build") {
    SUBCASE("int random") {
        std::vector<int32_t> a = { 5, 2, 8, 1, 9, 3, 7, 4, 6, 0 };
        das_make_heap_r(a.data(), a.size(), sizeof(int32_t), less_cmp<int32_t>());
        CHECK(is_max_heap(a.data(), a.size()));
    }
    SUBCASE("already-sorted ascending becomes a valid heap") {
        std::vector<int32_t> a = { 1, 2, 3, 4, 5, 6, 7, 8 };
        das_make_heap_r(a.data(), a.size(), sizeof(int32_t), less_cmp<int32_t>());
        CHECK(is_max_heap(a.data(), a.size()));
    }
    SUBCASE("already-sorted descending IS a max-heap") {
        std::vector<int32_t> a = { 8, 7, 6, 5, 4, 3, 2, 1 };
        das_make_heap_r(a.data(), a.size(), sizeof(int32_t), less_cmp<int32_t>());
        CHECK(is_max_heap(a.data(), a.size()));
        CHECK(a[0] == 8);
    }
    SUBCASE("struct payload — heap structure preserves payload-with-element") {
        std::vector<Quad> a = {
            {5, 100, 200, 300},
            {2, 101, 201, 301},
            {8, 102, 202, 302},
            {1, 103, 203, 303},
        };
        das_make_heap_r(a.data(), a.size(), sizeof(Quad), less_quad());
        CHECK(a[0].key == 8);
        CHECK(a[0].payload_a == 102);  // payload follows the {8,…} record
    }
}

TEST_CASE("das_push_heap_r — sift-up appends a new element to existing heap") {
    SUBCASE("push 7 onto a 5-element heap") {
        // Build a small heap first.
        std::vector<int32_t> a = { 1, 2, 3, 4, 5 };
        das_make_heap_r(a.data(), a.size(), sizeof(int32_t), less_cmp<int32_t>());
        REQUIRE(is_max_heap(a.data(), a.size()));
        // Push 7 — append, then sift.
        a.push_back(7);
        das_push_heap_r(a.data(), a.size(), sizeof(int32_t), less_cmp<int32_t>());
        CHECK(is_max_heap(a.data(), a.size()));
        CHECK(a[0] == 7);
    }
    SUBCASE("push smaller-than-root preserves root") {
        std::vector<int32_t> a = { 10, 5, 8, 1, 3, 6 };
        das_make_heap_r(a.data(), a.size(), sizeof(int32_t), less_cmp<int32_t>());
        REQUIRE(is_max_heap(a.data(), a.size()));
        a.push_back(2);
        das_push_heap_r(a.data(), a.size(), sizeof(int32_t), less_cmp<int32_t>());
        CHECK(is_max_heap(a.data(), a.size()));
        CHECK(a[0] == 10);
    }
}

TEST_CASE("das_pop_heap_r — move max to last position, sift-down [0..n-2]") {
    SUBCASE("repeated pop yields sorted-ascending sequence") {
        std::vector<int32_t> a = { 5, 2, 8, 1, 9, 3, 7, 4, 6, 0 };
        das_make_heap_r(a.data(), a.size(), sizeof(int32_t), less_cmp<int32_t>());
        std::vector<int32_t> popped;
        while (a.size() > 0) {
            das_pop_heap_r(a.data(), a.size(), sizeof(int32_t), less_cmp<int32_t>());
            popped.push_back(a.back());
            a.pop_back();
        }
        // popped is descending (we extract max each time).
        for (size_t i = 1; i < popped.size(); i++) CHECK(popped[i] <= popped[i-1]);
        // Original elements all present, none lost.
        CHECK(popped.size() == 10);
    }
    SUBCASE("pop on 1-element heap is a no-op (length stays >=1 in caller)") {
        std::vector<int32_t> a = { 42 };
        das_pop_heap_r(a.data(), a.size(), sizeof(int32_t), less_cmp<int32_t>());
        CHECK(a.size() == 1);
        CHECK(a[0] == 42);
    }
}

TEST_CASE("bounded-N streaming heap (top_n_by usage pattern)") {
    // Mimics how daslib's top_n_by would maintain a bounded heap during scan:
    // keep a max-heap of size N; for each new element smaller than root,
    // replace + sift. After all elements seen, heap contains N smallest.
    const int32_t N = 3;
    std::vector<int32_t> source = { 7, 3, 5, 1, 9, 2, 8, 4, 6, 0 };
    std::vector<int32_t> buf;
    buf.reserve(N);
    for (int32_t x : source) {
        if (int32_t(buf.size()) < N) {
            buf.push_back(x);
            das_push_heap_r(buf.data(), buf.size(), sizeof(int32_t), less_cmp<int32_t>());
        } else if (x < buf[0]) {
            das_pop_heap_r(buf.data(), buf.size(), sizeof(int32_t), less_cmp<int32_t>());
            buf.back() = x;
            das_push_heap_r(buf.data(), buf.size(), sizeof(int32_t), less_cmp<int32_t>());
        }
    }
    // buf should be the 3 smallest values: 0, 1, 2 (heap-ordered).
    std::sort(buf.begin(), buf.end());
    CHECK(buf.size() == 3);
    CHECK(buf[0] == 0);
    CHECK(buf[1] == 1);
    CHECK(buf[2] == 2);
}
