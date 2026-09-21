options gen2
options indenting = 4

module live_gc shared public

//! Standalone-loop GC boundary, UI-neutral. The canonical application shape
//! (``init``/``update``/``shutdown`` + a standalone ``main`` loop) calls
//! :func:`maybe_collect_gc` once per iteration; under the daslang-live host it
//! self-disables (the host checks each frame). In the browser the ``main``
//! loop never runs - the lifecycle drives ``update`` per frame from C++ - and
//! that driver applies the same allocation-pressure policy itself
//! (``Context::collectHeapIfMostlyFree``), so a program needs no per-platform
//! edit. Lives beside the live_host module so windowless applications
//! (servers, tools) get the boundary without a GLFW dependency;
//! ``live/glfw_live`` re-exports it for its callers.

require live_host

def public maybe_collect_gc() {
    //! The shared runtime policy requires fresh allocation pressure after collecting.
    //! The live host and browser lifecycle invoke this policy outside update().
    return if (is_live_mode())
    unsafe(heap_collect_if_needed())
}
