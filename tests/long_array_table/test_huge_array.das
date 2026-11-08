options gen2
options persistent_heap = true   // >2 GB arrays need PersistentHeapAllocator

require dastest/testing_boost public
require daslib/fio
require daslib/functional
require daslib/array_boost

// Memory-gated probe: erase at int64 position / int64 range past
// INT_MAX. The int64 overloads of `erase` (daslib/builtin.das:457,
// 465) were already widened pre-PR-builtin and live-tested with small
// N via test_int64_overloads.das, but their behavior at positions >
// INT_MAX was never exercised. This probe fills that gap by
// allocating a 2.2 GB uint8 array and erasing near the tail (cheap
// memmove, exercises the full int64 path through
// __builtin_array_erase_i64 / __builtin_array_erase_range_i64).
//
// Skipped silently unless DASLANG_HUGE_HEAP_TESTS=1 on a 64-bit
// build.

def huge_enabled() : bool {
    static_if (typeinfo sizeof(type<int?>) < 8) {
        return false
    }
    return has_env_variable("DASLANG_HUGE_HEAP_TESTS") && get_env_variable("DASLANG_HUGE_HEAP_TESTS") == "1"
}

// 2.2 GB elements of uint8 -- exceeds INT_MAX (2.147 GB).
let HUGE_N = 2200_l * 1024_l * 1024_l

[test(description = "erase at an int64 position past INT_MAX removes that element of a 2.2 GB array")]
def test_erase_at_int64_past_int_max(t : T?) {
    if (!huge_enabled()) return
    var arr : array<uint8>
    arr |> reserve(HUGE_N)
    arr |> resize(HUGE_N)
    let before = long_length(arr)
    let at_pos = before - 2_l
    arr[at_pos] = uint8(0xDD)
    arr[at_pos + 1_l] = uint8(0xEE)
    arr |> erase(at_pos)
    t |> equal(long_length(arr), before - 1_l)
    t |> equal(arr[at_pos], uint8(0xEE))
    delete arr
}

[test(description = "erase of an int64 range past INT_MAX removes exactly that range of a 2.2 GB array")]
def test_erase_range_at_int64_past_int_max(t : T?) {
    if (!huge_enabled()) return
    var arr : array<uint8>
    arr |> reserve(HUGE_N)
    arr |> resize(HUGE_N)
    let before = long_length(arr)
    let at_pos = before - 5_l
    arr[at_pos + 4_l] = uint8(0xFF)
    arr |> erase(at_pos, 3_l)
    t |> equal(long_length(arr), before - 3_l)
    t |> equal(arr[at_pos + 1_l], uint8(0xFF))
    delete arr
}

// Memory-gated probe targeting the SimNode_ArrayAt offset-math
// widening. `array<int>` with stride=4 and ~1.1G elements puts `idx *
// stride` above UINT32_MAX, so the address computation MUST be
// `uint64_t(idx) * uint64_t(stride) + offset` (per Phase
// 2/runtime_array.h). A uint32 regression would wrap and read the
// wrong cache line — usually a segfault, occasionally a
// silently-wrong value.
//
// ~4.3 GB allocation. Skipped silently unless
// DASLANG_HUGE_HEAP_TESTS=1 on a 64-bit build.


// (UINT32_MAX / 4) + ~32M slack = 1.1G elements; idx*stride > 4 GB at
// the tail.
let STRIDE_OVERFLOW_N = 1_100_000_000_l

[test(description = "an int64 index past the 32-bit stride overflow point reads and writes the right element")]
def test_int_array_high_index_int64(t : T?) {
    if (!huge_enabled()) return
    var arr : array<int>
    arr |> reserve(STRIDE_OVERFLOW_N)
    arr |> resize(STRIDE_OVERFLOW_N)
    t |> equal(long_length(arr), STRIDE_OVERFLOW_N)
    // Write to a position where idx*4 > UINT32_MAX, then read it back.
    // If the address math is uint32, we either segfault or read garbage.
    let high = STRIDE_OVERFLOW_N - 1_l
    arr[high] = int(0x12345678)
    arr[0_l] = int(0x0BADC0DE)
    t |> equal(arr[high], int(0x12345678))
    t |> equal(arr[0_l], int(0x0BADC0DE))
    delete arr
}

[test(description = "a uint64 index past the 32-bit stride overflow point reads and writes the right element")]
def test_int_array_high_index_uint64(t : T?) {
    if (!huge_enabled()) return
    var arr : array<int>
    arr |> reserve(STRIDE_OVERFLOW_N)
    arr |> resize(STRIDE_OVERFLOW_N)
    // Same test, uint64 index instead of int64 — exercises the U64
    // SimNode variant's offset math.
    let high : uint64 = uint64(STRIDE_OVERFLOW_N) - 1_ul
    arr[high] = int(0x12345678)
    arr[0_ul] = int(0x0BADC0DE)
    t |> equal(arr[high], int(0x12345678))
    t |> equal(arr[0_ul], int(0x0BADC0DE))
    delete arr
}

// Memory-gated probe: iterate a > INT_MAX-element array four
// different ways and assert each visits exactly long_length(arr)
// elements. If any iteration shape uses an int counter internally,
// the count check fails (or wraps).
//
// Auto-discovered by dastest; inline `huge_enabled()` gate
// silent-returns when the env var is unset, so CI sees a no-op pass.
// Manual run actually allocates.


// 2.2 GB elements of uint8 — exceeds INT_MAX (2.147 GB), stays under
// UINT32_MAX (4 GB) for faster runs.

[test(description = "iterating a 2.2 GB array with range64 over long_length visits every element")]
def test_iterate_via_range64_long_length(t : T?) {
    if (!huge_enabled()) return
    var arr : array<uint8>
    arr |> reserve(HUGE_N)
    arr |> resize(HUGE_N)
    var count = 0_l
    for (_i in range64(0_l, long_length(arr))) {
        count++
    }
    t |> equal(count, HUGE_N)
    delete arr
}

[test(description = "iterating a 2.2 GB array with the long iterator range visits every element")]
def test_iterate_via_long_iter_range(t : T?) {
    if (!huge_enabled()) return
    var arr : array<uint8>
    arr |> reserve(HUGE_N)
    arr |> resize(HUGE_N)
    var count = 0_l
    for (_i in long_iter_range(arr)) {
        count++
    }
    t |> equal(count, HUGE_N)
    delete arr
}

[test(description = "reading a 2.2 GB array by index through the whole range returns the stored values")]
def test_iterate_via_index_read(t : T?) {
    if (!huge_enabled()) return
    var arr : array<uint8>
    arr |> reserve(HUGE_N)
    arr |> resize(HUGE_N)
    // Write a sentinel at the last index, iterate with int64 counter and
    // assert we read it back via arr[i] indexing (stride math).
    arr[HUGE_N - 1_l] = uint8(0x77)
    var sum = 0_l
    for (i in range64(0_l, long_length(arr))) {
        sum += int64(arr[i])
    }
    t |> equal(sum, 119_l)  // only one non-zero byte at the very end (0x77)
    delete arr
}

[test(description = "long_enumerate over a 2.2 GB array yields matching int64 indices and values")]
def test_iterate_via_long_enumerate(t : T?) {
    if (!huge_enabled()) return
    var arr : array<uint8>
    arr |> reserve(HUGE_N)
    arr |> resize(HUGE_N)
    var last_index = -1_l
    var count = 0_l
    for ((i, _v) in long_enumerate(unsafe(each(arr)))) {
        last_index = i
        count++
    }
    t |> equal(count, HUGE_N)
    t |> equal(last_index, HUGE_N - 1_l)
    delete arr
}

// Memory-gated probe: resize an array past INT_MAX, then
// push/emplace/ push_clone one element and assert the new tail reads
// back correctly.
//
// The plan flagged
// `tests/long_array_table/test_push_returns_int64.das` as owed once
// Phase 4b lands `arr[i64]` (now landed). The daslib `push` wrapper
// doesn't surface the underlying `__builtin_array_push_back` int64
// position, so we verify indirectly: long_length grows by 1, and
// arr[before] reads back the pushed value via int64 indexing (which
// only works if the position the wrapper used internally was the
// correct int64).
//
// Skipped silently unless DASLANG_HUGE_HEAP_TESTS=1 on a 64-bit
// build.


// 2.2 GB elements of uint8 — exceeds INT_MAX (2.147 GB).

[test(description = "push on an array resized past INT_MAX grows long_length by one and the tail reads back")]
def test_push_past_int_max(t : T?) {
    if (!huge_enabled()) return
    var arr : array<uint8>
    arr |> reserve(HUGE_N)
    arr |> resize(HUGE_N)
    let before = long_length(arr)
    arr |> push(uint8(0xAB))
    t |> equal(long_length(arr), before + 1_l)
    t |> equal(arr[before], uint8(0xAB))
    delete arr
}

[test(description = "emplace on an array resized past INT_MAX grows long_length by one and the tail reads back")]
def test_emplace_past_int_max(t : T?) {
    if (!huge_enabled()) return
    var arr : array<uint8>
    arr |> reserve(HUGE_N)
    arr |> resize(HUGE_N)
    let before = long_length(arr)
    var v : uint8 = uint8(0xCD)
    arr |> emplace(v)
    t |> equal(long_length(arr), before + 1_l)
    t |> equal(arr[before], uint8(0xCD))
    delete arr
}

[test(description = "push_clone on an array resized past INT_MAX grows long_length by one and the tail reads back")]
def test_push_clone_past_int_max(t : T?) {
    if (!huge_enabled()) return
    var arr : array<uint8>
    arr |> reserve(HUGE_N)
    arr |> resize(HUGE_N)
    let before = long_length(arr)
    let v : uint8 = uint8(0xEF)
    arr |> push_clone(v)
    t |> equal(long_length(arr), before + 1_l)
    t |> equal(arr[before], uint8(0xEF))
    delete arr
}

// at-position insertion variants. The int|int64 disjunction on the
// daslib push/emplace/push_clone wrappers (PR-builtin) lets `at` be
// int64 directly, which is required to reach insertion positions past
// INT_MAX. Inserting at (before-1) keeps the memmove cost to one
// element while exercising the full int64 cascade through
// __builtin_array_push.

[test(description = "push at an int64 position past INT_MAX inserts the element there")]
def test_push_at_int64_past_int_max(t : T?) {
    if (!huge_enabled()) return
    var arr : array<uint8>
    arr |> reserve(HUGE_N)
    arr |> resize(HUGE_N)
    let before = long_length(arr)
    let at_pos = before - 1_l
    arr[at_pos] = uint8(0x11)
    arr |> push(uint8(0xAB), at_pos)
    t |> equal(long_length(arr), before + 1_l)
    t |> equal(arr[at_pos], uint8(0xAB))
    t |> equal(arr[at_pos + 1_l], uint8(0x11))
    delete arr
}

[test(description = "emplace at an int64 position past INT_MAX inserts the element there")]
def test_emplace_at_int64_past_int_max(t : T?) {
    if (!huge_enabled()) return
    var arr : array<uint8>
    arr |> reserve(HUGE_N)
    arr |> resize(HUGE_N)
    let before = long_length(arr)
    let at_pos = before - 1_l
    arr[at_pos] = uint8(0x22)
    var v : uint8 = uint8(0xCD)
    arr |> emplace(v, at_pos)
    t |> equal(long_length(arr), before + 1_l)
    t |> equal(arr[at_pos], uint8(0xCD))
    t |> equal(arr[at_pos + 1_l], uint8(0x22))
    delete arr
}

[test(description = "push_clone at an int64 position past INT_MAX inserts the element there")]
def test_push_clone_at_int64_past_int_max(t : T?) {
    if (!huge_enabled()) return
    var arr : array<uint8>
    arr |> reserve(HUGE_N)
    arr |> resize(HUGE_N)
    let before = long_length(arr)
    let at_pos = before - 1_l
    arr[at_pos] = uint8(0x33)
    let v : uint8 = uint8(0xEF)
    arr |> push_clone(v, at_pos)
    t |> equal(long_length(arr), before + 1_l)
    t |> equal(arr[at_pos], uint8(0xEF))
    t |> equal(arr[at_pos + 1_l], uint8(0x33))
    delete arr
}

// Memory-gated probe. Allocates a 5 GB `array<uint8>` and exercises
// int64 indexing at index 0 and index N-1 (a high position that
// requires `uint64_t(idx)*uint64_t(stride) + offset` for the address
// math).
//
// Skipped silently unless:
//   * running a 64-bit daslang (static_if on pointer size)
//   * DASLANG_HUGE_HEAP_TESTS=1 in the environment
//
// dastest auto-discovers this file (filename starts with `test_`),
// but the inline `huge_enabled()` gate silent-returns when the env
// var is unset, so CI sees a no-op pass. Run manually with the env
// var to actually allocate: $env:DASLANG_HUGE_HEAP_TESTS = "1"
// bin/Release/daslang.exe dastest/dastest.das -- --test
// tests/long_array_table/test_huge_array.das


[test(description = "an array resized past 4 GB reads back what was written at its ends")]
def test_huge_array_resize_round_trip(t : T?) {
    if (!huge_enabled()) return
    let N = 5_l * 1024_l * 1024_l * 1024_l  // 5 GB elements of uint8
    var arr : array<uint8>
    arr |> reserve(N)
    arr |> resize(N)
    t |> equal(long_length(arr), N)
    arr[0_l] = uint8(0xC0)
    arr[N - 1_l] = uint8(0xDE)
    t |> equal(arr[0_l], uint8(0xC0))
    t |> equal(arr[N - 1_l], uint8(0xDE))
    delete arr
}

[test(description = "long_length reports the true size of an array past INT_MAX")]
def test_huge_array_length_panics_long_length_ok(t : T?) {
    if (!huge_enabled()) return
    // 2.2 GB > INT_MAX (2.147 GB). long_length is safe; length() should
    // panic per the surface contract added in PR #2746.
    let N = 2200_l * 1024_l * 1024_l
    var arr : array<uint8>
    arr |> reserve(N)
    arr |> resize(N)
    t |> equal(long_length(arr), N)
    // length(arr) here should panic — but try/recover is banned for panic
    // UX testing (feedback_no_try_recover_for_soft_fail), so we don't
    // assert the panic here. Reproduce manually by uncommenting: let _l =
    // length(arr)
    delete arr
}

// Memory-gated probe: exercise array_boost.array_view with int64
// offset/length on a > INT_MAX element source. PR-G consolidates the
// prior int+int64 overload pair to a single int|int64 signature with
// static_if dispatch (mirrors the PR-builtin pattern). The int64 path
// uses long_length and _builtin_make_temp_array_i64, and the bounds
// check uses uint64 arithmetic to avoid signed overflow.
//
// Skipped silently unless DASLANG_HUGE_HEAP_TESTS=1 on a 64-bit
// build.


// 2.2 GB elements of uint8 -- exceeds INT_MAX (2.147 GB).

[test(description = "an array view at an int64 offset past INT_MAX addresses the right elements")]
def test_array_view_offset_int64_past_int_max(t : T?) {
    if (!huge_enabled()) return
    var arr : array<uint8>
    arr |> reserve(HUGE_N)
    arr |> resize(HUGE_N)
    // Sentinel near the tail (offset past INT_MAX).
    arr[HUGE_N - 5_l] = uint8(0xAA)
    arr[HUGE_N - 4_l] = uint8(0xBB)
    arr[HUGE_N - 3_l] = uint8(0xCC)
    array_view(arr, HUGE_N - 5_l, 3_l) $(view) {
        t |> equal(length(view), 3)
        t |> equal(view[0], uint8(0xAA))
        t |> equal(view[1], uint8(0xBB))
        t |> equal(view[2], uint8(0xCC))
    }
    delete arr
}

[test(description = "a mutable array view at an int64 offset past INT_MAX writes through to the right elements")]
def test_array_view_var_int64_past_int_max(t : T?) {
    if (!huge_enabled()) return
    var arr : array<uint8>
    arr |> reserve(HUGE_N)
    arr |> resize(HUGE_N)
    arr[HUGE_N - 2_l] = uint8(0x77)
    arr[HUGE_N - 1_l] = uint8(0x88)
    array_view(arr, HUGE_N - 2_l, 2_l) $(var view) {
        view[0] = uint8(0x99)
        view[1] = uint8(0xAA)
    }
    t |> equal(arr[HUGE_N - 2_l], uint8(0x99))
    t |> equal(arr[HUGE_N - 1_l], uint8(0xAA))
    delete arr
}

// Memory-gated probe: exercise array_boost.temp_array(arr) on a >
// INT_MAX element source. PR-G widens the private `array_helper` (the
// path that `temp_array(arr)` delegates to) to use long_length +
// _builtin_make_temp_array_i64 so the temp view sees the full int64
// length. Pre-PR-G the helper used `_::length(arr)` (int) and
// panicked when arr.size > INT_MAX.
//
// Also covers temp_array(data, lenA, a) on an int64 lenA -- this is
// the surface that callers reach when they have a raw pointer and a
// big length.
//
// Skipped silently unless DASLANG_HUGE_HEAP_TESTS=1 on a 64-bit
// build.


// 2.2 GB elements of uint8 -- exceeds INT_MAX (2.147 GB).

[test(description = "a temp array made over a huge buffer reads and writes through the original")]
def test_temp_array_arr_huge(t : T?) {
    if (!huge_enabled()) return
    var arr : array<uint8>
    arr |> reserve(HUGE_N)
    arr |> resize(HUGE_N)
    arr[0_l] = uint8(0xC0)
    arr[HUGE_N - 1_l] = uint8(0xDE)
    let view <- unsafe(temp_array(arr))
    t |> equal(long_length(view), HUGE_N)
    t |> equal(view[0_l], uint8(0xC0))
    t |> equal(view[HUGE_N - 1_l], uint8(0xDE))
    delete arr
}

[test(description = "a temp array made from data and a 64-bit length reports the full huge length")]
def test_temp_array_data_lenA_huge(t : T?) {
    if (!huge_enabled()) return
    var arr : array<uint8>
    arr |> reserve(HUGE_N)
    arr |> resize(HUGE_N)
    arr[0_l] = uint8(0x11)
    arr[HUGE_N - 1_l] = uint8(0x22)
    unsafe {
        let view <- temp_array(addr(arr[0_l]), HUGE_N, type<uint8>)
        t |> equal(long_length(view), HUGE_N)
        t |> equal(view[0_l], uint8(0x11))
        t |> equal(view[HUGE_N - 1_l], uint8(0x22))
    }
    delete arr
}
