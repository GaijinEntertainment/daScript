options gen2
options indenting = 4
options rtti
options gc
options persistent_heap
options disable_auto_inline

require dastest/testing_boost public
require _stackwalk_common

// comprehension lowering manufactures a closure whose only local is the accumulator
// array; it must gate live at parks inside the element expression - it is the only
// root of everything accumulated so far. disable_auto_inline keeps the lowered
// closure an invoked block rather than a splice.

def walk_len(var w : CollectingWalker?; s : string) : int {
    run_walk(w)
    return length(s)
}

def churn_strings(tag : string) : int {
    var junk <- [for (i in range(200)); "junk/{tag}/segment_{i}/{i * 3}_tail"]
    var total = 0
    for (s in junk) {
        total += length(s)
    }
    delete junk
    return total
}

def make_name(idx : int; var since : int&) : string {
    let nm = "root/dir_{idx / 5}/file_{idx % 5}.das"
    churn_strings("t_{idx}")
    since++
    if (since >= 10) {
        since = 0
        unsafe(heap_collect(true, true))
    }
    return nm
}

[test]
def test_stackwalk_comprehension(t : T?) {  // nolint:STYLE038 - flat list of independent test arms
    t |> run("one-line comprehension accumulator is live at an element park") @(t : T?) {
        var w = new CollectingWalker()
        let xs <- [for (i in range(3)); walk_len(w, "item_{i}")]
        t |> equal(length(xs), 3)
        t |> success(has_live_local(w, "__acomp"), "the accumulator must be live in the walk")
        unsafe {
            delete w
        }
    }
    t |> run("multi-line comprehension accumulator is live at an element park") @(t : T?) {
        var w = new CollectingWalker()
        let xs <- [for (i in range(3));
            walk_len(w, "item_{i}")]
        t |> equal(length(xs), 3)
        t |> success(has_live_local(w, "__acomp"), "the accumulator must be live in the walk")
        unsafe {
            delete w
        }
    }
    t |> run("accumulated strings survive collects during the comprehension") @(t : T?) {
        // probabilistic instrument: a regression sweeps the earlier elements (their only
        // root is the invisible accumulator), and the churn reuses the freed blocks
        var since = 0
        let names <- [for (i in range(60)); make_name(i, since)]
        var idx = 0
        for (nm in names) {
            if (nm != "root/dir_{idx / 5}/file_{idx % 5}.das") {
                t |> failure("corrupt entry at index {idx}: {nm}")
                return
            }
            idx++
        }
        t |> success(idx == 60, "walked all 60 entries")
    }
}
