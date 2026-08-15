// fixture for test_gc_walk_refusals.cpp - see the .cpp for the scenario
options gen2
options gc
options persistent_heap

require gc_walk_test

def inner_collect() {
    var pad = 1
    unsafe(heap_collect(true, false))
    pad++
}

[export]
def outer_reenter() : bool {
    // a live heap-string local is the point: the walk must refuse rather than sweep it
    // (interpolated so it is a runtime allocation, not a static literal)
    var precious = "outer_holds_{1000 + 234}"
    var refused = false
    try {
        reenter_null_line(@@inner_collect)
    } recover {
        refused = true
    }
    return refused && precious == "outer_holds_1234"
}

[export]
def collect_at_root() : bool {
    var ok = true
    var pad = 1
    try {
        unsafe(heap_collect(true, false))
    } recover {
        ok = false
    }
    return ok && pad == 1
}
