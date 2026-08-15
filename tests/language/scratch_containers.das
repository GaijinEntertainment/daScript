options gen2
options persistent_heap = true

// The scratch one-shot family's capacity/length semantics, plus the inert-outside-very-safe
// control: this file deliberately does NOT set very_safe_context, so the scratch bit must
// change nothing here. The very-safe halves live in scratch_very_safe_array/table.das.

require dastest/testing_boost public

// globals: a stack-promoted local never reaches array_reserve, and the heap delta in the
// control arm below would read zero against a broken predicate
var g_marked : array<int>
var g_plain : array<int>

[test]
def test_scratch_reserve(t : T?) {
    t |> run("scratch_reserve is exact - reserve never rounds") @(t : T?) {
        var inscope a : array<int>
        unsafe(a |> scratch_reserve(300))
        t |> equal(capacity(a), 300)
        t |> equal(length(a), 0)
    }
    t |> run("int64 overload") @(t : T?) {
        var inscope a : array<int>
        unsafe(a |> scratch_reserve(300l))
        t |> equal(long_capacity(a), 300l)
    }
}

[test]
def test_scratch_ensure_capacity(t : T?) {
    t |> run("no-op at or below capacity") @(t : T?) {
        var inscope a : array<int>
        unsafe(a |> scratch_reserve(64))
        unsafe(a |> scratch_ensure_capacity(32))
        t |> equal(capacity(a), 64)
        t |> equal(length(a), 0)
    }
    t |> run("doubling wins when it covers the ask") @(t : T?) {
        var inscope a : array<int>
        unsafe(a |> scratch_reserve(64))
        unsafe(a |> scratch_ensure_capacity(100))
        t |> equal(capacity(a), 128)
    }
    t |> run("exact ask wins past the double") @(t : T?) {
        var inscope a : array<int>
        unsafe(a |> scratch_reserve(64))
        unsafe(a |> scratch_ensure_capacity(500))
        t |> equal(capacity(a), 500)
    }
    t |> run("int overload forwards, doubling-from-zero edge") @(t : T?) {
        var inscope a : array<int>
        unsafe(a |> scratch_ensure_capacity(10))
        t |> equal(capacity(a), 10)
    }
}

[test]
def test_scratch_resize(t : T?) {
    t |> run("grow reserves the exact size - plain resize pow2-rounds") @(t : T?) {
        var inscope a : array<int>
        unsafe(a |> scratch_resize(300))
        t |> equal(length(a), 300)
        t |> equal(capacity(a), 300)
        var inscope b : array<int>
        b |> resize(300)   // the contrast: same size through the plain path
        t |> equal(capacity(b), 512)
    }
    t |> run("shrink keeps capacity") @(t : T?) {
        var inscope a : array<int>
        unsafe(a |> scratch_resize(300))
        unsafe(a |> scratch_resize(10))
        t |> equal(length(a), 10)
        t |> equal(capacity(a), 300)
    }
    t |> run("zero size") @(t : T?) {
        var inscope a : array<int>
        unsafe(a |> scratch_resize(300))
        unsafe(a |> scratch_resize(0))
        t |> equal(length(a), 0)
        t |> equal(capacity(a), 300)
    }
    t |> run("negative size is a no-op, both widths") @(t : T?) {
        var inscope a : array<int>
        unsafe(a |> scratch_reserve(64))
        unsafe(a |> scratch_reserve(-1))
        t |> equal(capacity(a), 64)
        unsafe(a |> scratch_reserve(-1l))
        t |> equal(capacity(a), 64)
    }
}

[test]
def test_scratch_inert_outside_very_safe(t : T?) {
    t |> run("a normal context grows identically with and without the bit") @(t : T?) {
        let before = heap_bytes_allocated()
        unsafe(g_marked |> set_scratch(true))
        for (c in fixed_array(16l, 32l, 64l, 128l, 256l, 512l, 1024l, 2048l, 4096l)) {
            g_marked |> reserve(c)
        }
        let grewMarked = heap_bytes_allocated() - before
        let b2 = heap_bytes_allocated()
        for (c in fixed_array(16l, 32l, 64l, 128l, 256l, 512l, 1024l, 2048l, 4096l)) {
            g_plain |> reserve(c)
        }
        let grewPlain = heap_bytes_allocated() - b2
        // eager both ways: only the final generation stays live
        t |> equal(grewMarked, uint64(long_capacity(g_marked)) * 4ul)
        t |> equal(grewPlain, uint64(long_capacity(g_plain)) * 4ul)
        t |> equal(is_scratch(g_marked), true)
        t |> equal(is_scratch(g_plain), false)
    }
}
