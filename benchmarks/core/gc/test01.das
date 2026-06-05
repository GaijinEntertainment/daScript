// Runtime heap GC benchmarks. Each sub-benchmark builds a heap shape, then measures the cost
// of a single heap_collect() cycle. Needs options gc + persistent_heap; the measured block is
// the collection itself (allocation columns are ~0 — the work is mark+sweep, not allocation).
//   build/daslang -jit dastest/dastest.das -- --bench --test benchmarks/core/gc/test01.das
options gen2
options persistent_heap
options gc
options no_aot

require dastest/testing_boost

struct Node {
    name : string
    vals : array<int>
    tags : table<int; int>
    next : Node?
}

// global so the live set is an unconditional GC root (no in-scope-local dependence)
var g_roots : array<Node?>

def build_wide(n : int) {
    g_roots |> reserve(length(g_roots) + n)
    for (i in range(n)) {
        var nd = new Node(name = "node_{i}", vals <- [for (x in range(8)); x * i])
        for (k in range(8)) {
            nd.tags |> insert(k, k * i)
        }
        g_roots |> push(nd)
    }
}

def alloc_garbage(n : int) {
    for (i in range(n)) {
        var _nd = new Node(name = "tmp_{i}", vals <- [for (x in range(4)); x])
    }
}

def build_chain(depth : int) {
    var head : Node? = null
    for (_i in range(depth)) {
        head = new Node(name = "n", next = head)
    }
    g_roots |> push(head)
}

// mark-bound: a large stable live set; each cycle re-marks the whole graph
[benchmark]
def gc_mark_heavy(b : B?) {
    g_roots |> clear()
    build_wide(20000)
    unsafe { heap_collect(true, false) }    // settle: drop setup garbage before measuring
    b |> run("gc_collect/mark_20k_nodes") {
        unsafe { heap_collect(true, false) }
    }
}

// sweep-bound: a tiny live set but a big garbage burst that inflates heap capacity; sweep
// scans that capacity every cycle (Decks persist after slots are freed)
[benchmark]
def gc_sweep_heavy(b : B?) {
    g_roots |> clear()
    build_wide(200)
    alloc_garbage(400000)
    b |> run("gc_collect/sweep_400k_capacity") {
        unsafe { heap_collect(true, false) }
    }
}

// deep: a deeply nested chain; exercises the bounded-recursion mark path (must stay fast)
[benchmark]
def gc_deep(b : B?) {
    g_roots |> clear()
    build_chain(500000)
    unsafe { heap_collect(true, false) }
    b |> run("gc_collect/deep_500k_chain") {
        unsafe { heap_collect(true, false) }
    }
}
