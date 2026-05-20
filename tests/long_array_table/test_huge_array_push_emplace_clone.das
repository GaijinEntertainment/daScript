options gen2
options persistent_heap = true   // >2 GB arrays need PersistentHeapAllocator

require dastest/testing_boost public
require daslib/fio

// Memory-gated probe: resize an array past INT_MAX, then push/emplace/
// push_clone one element and assert the new tail reads back correctly.
//
// The plan flagged `tests/long_array_table/test_push_returns_int64.das` as
// owed once Phase 4b lands `arr[i64]` (now landed). The daslib `push` wrapper
// doesn't surface the underlying `__builtin_array_push_back` int64 position,
// so we verify indirectly: long_length grows by 1, and arr[before] reads
// back the pushed value via int64 indexing (which only works if the position
// the wrapper used internally was the correct int64).
//
// Skipped silently unless DASLANG_HUGE_HEAP_TESTS=1 on a 64-bit build.

def huge_enabled() : bool {
    static_if (typeinfo sizeof(type<int?>) < 8) {
        return false
    }
    return has_env_variable("DASLANG_HUGE_HEAP_TESTS") && get_env_variable("DASLANG_HUGE_HEAP_TESTS") == "1"
}

// 2.2 GB elements of uint8 — exceeds INT_MAX (2.147 GB).
let HUGE_N = 2200_l * 1024_l * 1024_l

[test]
def test_push_past_int_max(t : T?) {
    if (!huge_enabled()) return
    var arr : array<uint8>
    arr |> resize(HUGE_N)
    let before = long_length(arr)
    arr |> push(uint8(0xAB))
    t |> equal(long_length(arr), before + 1_l)
    t |> equal(arr[before], uint8(0xAB))
    delete arr
}

[test]
def test_emplace_past_int_max(t : T?) {
    if (!huge_enabled()) return
    var arr : array<uint8>
    arr |> resize(HUGE_N)
    let before = long_length(arr)
    var v : uint8 = uint8(0xCD)
    arr |> emplace(v)
    t |> equal(long_length(arr), before + 1_l)
    t |> equal(arr[before], uint8(0xCD))
    delete arr
}

[test]
def test_push_clone_past_int_max(t : T?) {
    if (!huge_enabled()) return
    var arr : array<uint8>
    arr |> resize(HUGE_N)
    let before = long_length(arr)
    let v : uint8 = uint8(0xEF)
    arr |> push_clone(v)
    t |> equal(long_length(arr), before + 1_l)
    t |> equal(arr[before], uint8(0xEF))
    delete arr
}
