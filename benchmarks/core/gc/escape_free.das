// Escape-analysis scope-free benchmark. A loop allocates a non-escaping pointer (with an owned
// array field) per iteration. With escape-free ON the object + its array are reclaimed at scope
// exit, so the heap stays flat and the auto-GC rarely (or never) fires; with it OFF the garbage
// accumulates until the GC kicks in. Compare the two by flipping `options force_escape_free`:
//   build/daslang -jit dastest/dastest.das -- --bench --test benchmarks/core/gc/escape_free.das
options gen2
options persistent_heap
options gc
options no_aot
options force_escape_free = true

require dastest/testing_boost

struct Node {
    x : int
    vals : array<int>
}

// every use of `p` is a field access -> non-escaping -> reclaimed at scope exit
def churn(n : int) : int {
    var acc = 0
    for (i in range(n)) {
        var p = new Node(x = i, vals <- [for (z in range(8)); z * i])
        acc += p.x + length(p.vals)
    }
    return acc
}

[benchmark]
def gc_escape_pointer_churn(b : B?) {
    b |> run("escape_free/pointer_churn_200k") {
        let _ = churn(200000)
    }
}
