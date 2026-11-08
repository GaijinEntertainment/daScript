options gen2
options rtti
options gc
options persistent_heap
options no_aot
options disable_auto_inline

require dastest/testing_boost public
require _stackwalk_common

// the generated class-constructor wrapper holds `let self` - the ONLY root of the
// object while the user initializer runs; it must gate live at parks inside the ctor.
// disable_auto_inline pins the wrapper as a real interpreted frame.

class Talker {
    msg : string
    churned : int
    def Talker(var w : CollectingWalker?) {
        run_walk(w)
        msg = "hello"
    }
    def Talker(seed : int) {
        let tag = "keep_{seed}"
        churned = churn_strings(tag)
        unsafe(heap_collect(true, true))
        msg = tag
    }
}

var g_walker : CollectingWalker?

class Base {
    note : string
    def Base() {
        if (g_walker != null) {
            run_walk(g_walker)
        }
        note = "base"
    }
}

// no own ctor: `new Derived()` goes through the generated chain wrapper, whose
// `let self` is the local under test while Base's ctor runs
class Derived : Base {}

[test(description = "the generated ctor wrapper's self is live while a user or ancestor ctor runs, and an object under construction survives a ctor-time collect")]
def test_stackwalk_class_ctor(t : T?) {
    t |> run("wrapper-frame self is live while the user ctor runs") @(t : T?) {
        var w = new CollectingWalker()
        var tk = new Talker(w)
        t |> equal(tk.msg, "hello")
        t |> success(has_live_local(w, "self"), "ctor wrapper's self must be live in the walk")
        unsafe {
            delete tk
            delete w
        }
    }
    t |> run("chain-ctor self is live while the ancestor ctor runs") @(t : T?) {
        var w = new CollectingWalker()
        g_walker = w
        var d = new Derived()
        g_walker = null
        t |> equal(d.note, "base")
        t |> success(has_live_local(w, "self"), "chain wrapper's self must be live in the walk")
        unsafe {
            delete d
            delete w
        }
    }
    t |> run("object under construction survives a ctor-time collect") @(t : T?) {
        // probabilistic instrument: a regression sweeps the half-built object, and the
        // assert fires when the freed block gets reused by the next iteration's churn
        for (i in range(20)) {
            var tk = new Talker(i)
            t |> equal(tk.msg, "keep_{i}")
            t |> success(tk.churned > 0)
            unsafe {
                delete tk
            }
        }
    }
}

// comprehension lowering manufactures a closure whose only local is the accumulator
// array - the only root of everything accumulated so far. disable_auto_inline keeps
// the lowered closure an invoked block rather than a splice.

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

[test(description = "a comprehension's accumulator array is live at element parks, one-line or multi-line, and its strings survive collects")]
def test_stackwalk_comprehension(t : T?) {
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
        // per-instance block infos + owner-tagged positions keep it gated live through the loop
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
