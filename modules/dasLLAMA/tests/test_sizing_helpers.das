options gen2

require dastest/testing_boost public
require dasllama/dasllama_common

// The sizing helpers' own contract, fed directly: reserve_resize is exact at any element
// width, grow_resize keeps a reused buffer amortized across shrink-then-grow, and
// overwrite_resize is grow-only no-init — and all three stay panic-free past the
// max_unreserved_size guard (64 MB default), which is the reason they exist.

[test]
def test_reserve_resize(t : T?) {
    t |> run("exact capacity, both widths, any element type") @(t : T?) {
        var f : array<float>
        f |> reserve_resize(1000)
        t |> equal(length(f), 1000)
        t |> equal(capacity(f), 1000)
        var b : array<uint8>
        b |> reserve_resize(70000000l)   // past max_unreserved_size: reserved, so no panic
        t |> equal(long_length(b), 70000000l)
        t |> equal(long_capacity(b), 70000000l)
    }
}

[test]
def test_grow_resize(t : T?) {
    t |> run("geometric growth, reused across shrink and grow") @(t : T?) {
        var a : array<int>
        a |> grow_resize(100)
        let cap0 = capacity(a)
        t |> equal(length(a), 100)
        t |> success(cap0 >= 100)
        a |> grow_resize(10)             // shrink: capacity is kept, no realloc
        t |> equal(length(a), 10)
        t |> equal(capacity(a), cap0)
        a |> grow_resize(cap0 + 1)       // grow: at least doubles
        t |> success(capacity(a) >= cap0 * 2)
        var big : array<uint8>
        big |> grow_resize(70000000l)    // past max_unreserved_size: reserved, so no panic
        t |> equal(long_length(big), 70000000l)
    }
}

[test]
def test_overwrite_resize(t : T?) {
    t |> run("grow-only no-init scratch") @(t : T?) {
        var s : array<float>
        overwrite_resize(s, 1000l)
        t |> equal(long_length(s), 1000l)
        let cap0 = long_capacity(s)
        overwrite_resize(s, 10l)           // need below length: shrink, capacity kept
        t |> equal(long_length(s), 10l)
        t |> equal(long_capacity(s), cap0)
        overwrite_resize(s, 20000000l)     // 80 MB grow: exact reserve inside, no panic
        t |> equal(long_length(s), 20000000l)
        t |> equal(long_capacity(s), 20000000l)
    }
}

[test]
def test_zeroed_resize(t : T?) {
    t |> run("zero-init scratch: amortized growth, re-added tail zeroed") @(t : T?) {
        var a : array<int64>
        a |> zeroed_resize(100l)
        for (i in range64(100l)) {
            a[i] = i + 1l
        }
        a |> zeroed_resize(10l)          // shrink within capacity
        let cap0 = long_capacity(a)
        a |> zeroed_resize(100l)         // regrow: the re-added tail must read zero, not stale
        t |> equal(long_capacity(a), cap0)
        t |> equal(a[9], 10l, "kept elements survive")
        t |> equal(a[10], 0l, "re-added tail is zeroed")
        t |> equal(a[99], 0l, "re-added tail is zeroed to the end")
        a |> zeroed_resize(long_capacity(a) + 1l)   // grow: ensure_capacity doubles, never exact
        t |> success(long_capacity(a) >= cap0 * 2l, "growth stays amortized (an exact reserve reallocates every chunk)")
        var big : array<float>
        big |> zeroed_resize(20000000l)  // 80 MB grow past max_unreserved_size: reserved, so no panic
        t |> equal(long_length(big), 20000000l)
    }
}
