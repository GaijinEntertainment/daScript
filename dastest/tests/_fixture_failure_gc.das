options gen2
options gc
options persistent_heap

require dastest/testing_boost public
require strings

[test]
def retained_failure_survives_collection(t : T?) {
    let retained = clone_string("retained failure before collection")
    let before = heap_bytes_allocated()
    t->log("ordinary log before collection")
    t |> failure(retained)
    let after = heap_bytes_allocated()
    t |> equal(after, before, "failure buffers belong to the runner heap")
    unsafe(heap_collect(true, true))
}

[test]
def fatal_failure_after_collection(t : T?) {
    unsafe(heap_collect(true, true))
    t->fatal("fatal failure after collection")
}

[test]
def runtime_error_after_collection() {
    unsafe(heap_collect(true, true))
    panic("runtime error after collection")
}

[test]
def nested_failure_after_collection(t : T?) {
    t |> run("nested failure") @(nested : T?) {
        let before = heap_bytes_allocated()
        nested |> failure("nested retained failure")
        nested |> equal(heap_bytes_allocated(), before)
        unsafe(heap_collect(true, true))
    }
}
