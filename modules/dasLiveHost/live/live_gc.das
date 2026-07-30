options gen2
options indenting = 4

module live_gc shared public

//! Standalone-loop GC boundary, UI-neutral. The canonical application shape
//! (``init``/``update``/``shutdown`` + a standalone ``main`` loop) calls
//! :func:`maybe_collect_gc` once per iteration; under the daslang-live host it
//! self-disables (the host collects each frame). Lives beside the live_host
//! module so windowless applications (servers, tools) get the boundary without
//! a GLFW dependency; ``live/glfw_live`` re-exports it for its callers.

require live_host

def public maybe_collect_gc() {
    //! Standalone-loop GC. Call once per iteration after your update/render: the daslang-live host
    //! collects the heap for you, but a standalone `def main` loop must do it itself. Collects a heap
    //! only when it is mostly FREE (cheap compaction — little live data to walk), so it is fine to
    //! call every frame. The string heap fills faster, so it gets the lower threshold and is checked
    //! first. Requires `options gc` + `options persistent_heap` on the program (heap_collect throws
    //! otherwise).
    return if (is_live_mode())       // host already collects each frame — only a standalone loop needs this
    let s_used = string_heap_bytes_allocated()
    let s_total = string_heap_total_allocated()
    if (s_total > 0ul && s_used * 3ul < s_total * 2ul) {        // string heap > 1/3 unused
        unsafe(heap_collect(true, false))
        return
    }
    let h_used = heap_bytes_allocated()
    let h_total = heap_total_allocated()
    if (h_total > 0ul && h_used * 3ul < h_total) {              // heap > 2/3 unused
        unsafe(heap_collect(true, false))                      // both heaps — the GC walk is shared, so reclaiming the string heap too is near-free
    }
}
