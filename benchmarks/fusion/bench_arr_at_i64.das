options gen2

require dastest/testing_boost

// Side-by-side cost of int-indexed vs int64-indexed array access.
// Pre-Phase-5: int64 path fell off fusion (no _I64 fusion variant).
// Post-Phase-5: int64 path fuses via SimNode_Op2ArrayAt_I64.
// The gap between these two benchmarks measures the fusion benefit
// on the i64 path. Workload is identical except for the index type.

let N = 10000

[benchmark]
def arr_at_int_idx(b : B?) {
    var arr : array<int>
    arr |> resize(N)
    for (i in range(N)) {
        arr[i] = i
    }
    b |> run("read_int_idx/{N}", N) {
        var sum = 0
        for (i in range(N)) {
            sum += arr[i]
        }
        strictEqual(b, sum, N * (N - 1) / 2)
    }
    b |> run("write_int_idx/{N}", N) {
        for (i in range(N)) {
            arr[i] = i * 2
        }
    }
}

[benchmark]
def arr_at_int64_idx(b : B?) {
    var arr : array<int>
    arr |> resize(N)
    for (i in range(N)) {
        arr[i] = i
    }
    let N64 = int64(N)
    b |> run("read_int64_idx/{N}", N) {
        var sum = 0
        var i = 0_l
        while (i < N64) {
            sum += arr[i]
            i++
        }
        strictEqual(b, sum, N * (N - 1) / 2)
    }
    b |> run("write_int64_idx/{N}", N) {
        var i = 0_l
        while (i < N64) {
            arr[i] = int(i) * 2
            i++
        }
    }
}

[benchmark]
def arr_at_uint64_idx(b : B?) {
    var arr : array<int>
    arr |> resize(N)
    for (i in range(N)) {
        arr[i] = i
    }
    let N64 = uint64(N)
    b |> run("read_uint64_idx/{N}", N) {
        var sum = 0
        var i = 0_ul
        while (i < N64) {
            sum += arr[i]
            i++
        }
        strictEqual(b, sum, N * (N - 1) / 2)
    }
}
