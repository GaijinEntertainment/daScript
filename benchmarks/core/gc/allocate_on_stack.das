// allocate-on-stack benchmark. A loop allocates a non-escaping pointer per iteration (every use is
// a field access). With `options force_allocate_on_stack` the pointee lives in the stack frame, so
// B/op and allocs/op drop to zero; with it off each iteration hits the heap. Compare by flipping the
// option:
//   build/daslang dastest/dastest.das -- --run benchmarks/core/gc/allocate_on_stack.das --bench
options gen2
options persistent_heap
options no_aot
options force_allocate_on_stack = true

require dastest/testing_boost

struct Node {
    x : int
    y : int
}

// `new Node(x=.., y=..)` -> ascend lowering; every use is a field access -> non-escaping
def churn_ascend(n : int) : int {
    var acc = 0
    for (i in range(n)) {
        var p = new Node(x = i, y = i * 2)
        acc += p.x + p.y
    }
    return acc
}

// plain `new Node()` -> ExprNew lowering
def churn_plain(n : int) : int {
    var acc = 0
    for (i in range(n)) {
        var p = new Node()  // nolint:STYLE013
        p.x = i
        p.y = i * 2
        acc += p.x + p.y
    }
    return acc
}

[benchmark]
def bench_allocate_on_stack(b : B?) {
    b |> run("allocate_on_stack/ascend_churn_1m") {
        let _ = churn_ascend(1000000)
    }
    b |> run("allocate_on_stack/plain_churn_1m") {
        let _ = churn_plain(1000000)
    }
}
