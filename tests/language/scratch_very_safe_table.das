options gen2
options persistent_heap = true
options very_safe_context

// scope: the table half of the scratch flag under very_safe_context
// growth: insert-driven only - table reserve jumps to the final pow2 in one allocation
// layout: perSlot is internal policy, so no byte count is pinned - ratios + self-calibration
// rehash-survival: an isolated grow's eager delta equals the old block, measured by this test

require dastest/testing_boost public

var g_eager : table<int; int>
var g_deferred : table<int; int>
var g_unmarked : table<int; int>
var g_deleted : table<int; int>

[test]
def test_scratch_table(t : T?) {
    t |> run("eager free on rehash, deferred without, bit rides every rehash") @(t : T?) {
        // eager: 500 inserts drive growth 8 -> 1024; old blocks freed at each rehash
        let b0 = heap_bytes_allocated()
        unsafe(g_eager |> set_scratch(true))
        for (i in range(500)) {
            g_eager[i] = i
        }
        let grewEager = heap_bytes_allocated() - b0
        t |> equal(long_capacity(g_eager), 1024l)
        t |> equal(is_scratch(g_eager), true)
        // deferred: the identical insert run leaves every generation live
        t |> equal(is_scratch(g_deferred), false)
        let b1 = heap_bytes_allocated()
        for (i in range(500)) {
            g_deferred[i] = i
        }
        let grewDeferred = heap_bytes_allocated() - b1
        t |> equal(long_capacity(g_deferred), 1024l)
        t |> success(grewEager < grewDeferred, "deferred must retain more than eager")
        t |> success(grewDeferred >= grewEager * 3ul / 2ul, "deferred ~2x eager: the generation ladder stays live")
        // set_scratch(false) re-arms the deferral: same shape as the deferred run, so the
        // deltas must be exactly equal
        let b2 = heap_bytes_allocated()
        unsafe(g_unmarked |> set_scratch(true))
        unsafe(g_unmarked |> set_scratch(false))
        t |> equal(is_scratch(g_unmarked), false)
        for (i in range(500)) {
            g_unmarked[i] = i
        }
        let grewUnmarked = heap_bytes_allocated() - b2
        t |> equal(grewUnmarked, grewDeferred)
        // the bit survives every newTab.flags copy + swap of those rehashes: one more
        // isolated grow 1024 -> 2048. Eager delta = new(2x) - freed old(1x) = old block,
        // which is exactly what grewEager measured; a lost bit would read 2x that.
        let b3 = heap_bytes_allocated()
        for (i in range(500, 700)) {
            g_eager[i] = i
        }
        let lateDelta = heap_bytes_allocated() - b3
        t |> equal(long_capacity(g_eager), 2048l)
        t |> equal(lateDelta, grewEager)
    }
    t |> run("delete clears the bit - grown and never-grown alike") @(t : T?) {
        // never-grown: no backing store to free, but delete still resets the mark
        unsafe(g_deleted |> set_scratch(true))
        delete g_deleted
        t |> equal(is_scratch(g_deleted), false)
        unsafe(g_deleted |> set_scratch(true))
        g_deleted[1] = 1
        delete g_deleted
        t |> equal(is_scratch(g_deleted), false)
    }
}
