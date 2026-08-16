options gen2
options persistent_heap = true
options very_safe_context

// scope: the array half of the scratch flag under very_safe_context
// heap: persistent, 16-byte accounting, no pow2 bucketing - array asserts are exact
// ladder: reserve 16..4096 of array<int> - every generation a 16-multiple
// eager expectation: cap*4 (only the final generation lives)
// deferred expectation: cap*8 - 64 (the whole ladder lives)
// windows: a window holds only set_scratch (allocates nothing) and reserves - any
// resize/clear stays OUTSIDE, since first-in-context they can lazily allocate
// instrument state under the CI Coverage rig; ladders are fixed_array (stack)

require dastest/testing_boost public

// one global per arm: a stack-promoted local never reaches array_reserve at all,
// and the arm would read green against a broken predicate
var g_eager : array<int>
var g_deferred : array<int>
var g_oneshot_unmarked : array<int>
var g_oneshot_marked : array<int>
var g_src : array<int>
var g_dst : array<int>
var g_deleted : array<int>
var g_cleared : array<int>
var g_unmarked : array<int>

[test]
def test_scratch_eager_vs_deferred(t : T?) {
    t |> run("very_safe + scratch takes the eager realloc/free arm") @(t : T?) {
        let before = heap_bytes_allocated()
        unsafe(g_eager |> set_scratch(true))
        for (c in fixed_array(16l, 32l, 64l, 128l, 256l, 512l, 1024l, 2048l, 4096l)) {
            g_eager |> reserve(c)
        }
        let grew = heap_bytes_allocated() - before
        t |> equal(long_capacity(g_eager), 4096l)
        t |> equal(grew, uint64(long_capacity(g_eager)) * 4ul)   // 16384
    }
    t |> run("no scratch defers - every generation stays live") @(t : T?) {
        let before = heap_bytes_allocated()
        for (c in fixed_array(16l, 32l, 64l, 128l, 256l, 512l, 1024l, 2048l, 4096l)) {
            g_deferred |> reserve(c)
        }
        let grew = heap_bytes_allocated() - before
        t |> equal(long_capacity(g_deferred), 4096l)
        t |> equal(grew, uint64(long_capacity(g_deferred)) * 8ul - 64ul)   // 32704
    }
    t |> run("set_scratch(false) re-arms the deferral") @(t : T?) {
        let before = heap_bytes_allocated()
        unsafe(g_unmarked |> set_scratch(true))
        unsafe(g_unmarked |> set_scratch(false))
        t |> equal(is_scratch(g_unmarked), false)
        for (c in fixed_array(16l, 32l, 64l, 128l, 256l, 512l, 1024l, 2048l, 4096l)) {
            g_unmarked |> reserve(c)
        }
        let grew = heap_bytes_allocated() - before
        t |> equal(grew, uint64(long_capacity(g_unmarked)) * 8ul - 64ul)   // deferred: the clear took
    }
}

[test]
def test_scratch_reserve_forces_eager(t : T?) {
    t |> run("scratch_reserve is eager with the bit unset") @(t : T?) {
        let before = heap_bytes_allocated()
        for (c in fixed_array(16l, 32l, 64l, 128l, 256l, 512l, 1024l, 2048l, 4096l)) {
            unsafe(g_oneshot_unmarked |> scratch_reserve(c))
        }
        let grew = heap_bytes_allocated() - before
        t |> equal(grew, uint64(long_capacity(g_oneshot_unmarked)) * 4ul)
        t |> equal(is_scratch(g_oneshot_unmarked), false)   // the one-shot leaves no state behind
    }
    t |> run("scratch_reserve is eager with the bit set too") @(t : T?) {
        let before = heap_bytes_allocated()
        unsafe(g_oneshot_marked |> set_scratch(true))
        for (c in fixed_array(16l, 32l, 64l, 128l, 256l, 512l, 1024l, 2048l, 4096l)) {
            unsafe(g_oneshot_marked |> scratch_reserve(c))
        }
        let grew = heap_bytes_allocated() - before
        t |> equal(grew, uint64(long_capacity(g_oneshot_marked)) * 4ul)
    }
}

[test]
def test_scratch_lifecycle(t : T?) {
    t |> run("the bit survives a move") @(t : T?) {
        let before = heap_bytes_allocated()
        unsafe(g_src |> set_scratch(true))
        g_src |> reserve(16l)
        g_dst <- g_src   // nolint:PERF030 — the target global is fresh-empty; struct copied wholesale, source zeroed
        t |> equal(long_capacity(g_src), 0l)
        for (c in fixed_array(32l, 64l, 128l, 256l, 512l, 1024l, 2048l, 4096l)) {
            g_dst |> reserve(c)
        }
        let grew = heap_bytes_allocated() - before
        t |> equal(grew, uint64(long_capacity(g_dst)) * 4ul)   // eager: the bit moved with the buffer
    }
    t |> run("delete clears the bit - grown and never-grown alike") @(t : T?) {
        // never-grown: no backing store to free, but delete still resets the mark
        unsafe(g_deleted |> set_scratch(true))
        delete g_deleted
        t |> equal(is_scratch(g_deleted), false)
        unsafe(g_deleted |> set_scratch(true))
        g_deleted |> reserve(16l)
        delete g_deleted
        t |> equal(is_scratch(g_deleted), false)
        let before = heap_bytes_allocated()
        for (c in fixed_array(16l, 32l, 64l, 128l, 256l, 512l, 1024l, 2048l, 4096l)) {
            g_deleted |> reserve(c)
        }
        let grew = heap_bytes_allocated() - before
        t |> equal(grew, uint64(long_capacity(g_deleted)) * 8ul - 64ul)   // deferred again
    }
    t |> run("clear keeps the bit - the recycled-queue pattern") @(t : T?) {
        // setup stays OUTSIDE the window: under the CI Coverage rig, first-in-context
        // resize/clear can lazily allocate instrument state (+144 bytes each, linux
        // directory sweep) - the window holds only the ladder, the actual instrument
        unsafe(g_cleared |> set_scratch(true))
        g_cleared |> reserve(16l)
        g_cleared |> resize(16l)
        g_cleared |> clear()   // size=0 only; capacity and flags untouched
        t |> equal(is_scratch(g_cleared), true)
        let before = heap_bytes_allocated()
        for (c in fixed_array(32l, 64l, 128l, 256l, 512l, 1024l, 2048l, 4096l)) {
            g_cleared |> reserve(c)
        }
        let grew = heap_bytes_allocated() - before
        // still eager; -64 = the pre-window 16-cap buffer, freed at the first regrow
        t |> equal(grew, uint64(long_capacity(g_cleared)) * 4ul - 64ul)
    }
}
