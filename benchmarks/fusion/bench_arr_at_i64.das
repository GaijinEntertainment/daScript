options gen2

require dastest/testing_boost

// Apples-to-apples cost of int / int64 / uint64-indexed array access.
// All three benches use the same `for (i in <range-iter>(N))` shape,
// differing only in the index type. Write workload is `arr[i] = 1`
// (no cast in the body — isolates the index/fusion cost). The outer
// `for (j in range(OUTER))` amortizes the per-body harness overhead
// over OUTER * N inner ops.

let N = 10000
let OUTER = 10
let TOTAL = N * OUTER

[benchmark]
def arr_at_int_idx(b : B?) {
    var arr : array<int>
    arr |> resize(N)
    b |> run("write_int_idx/{TOTAL}", TOTAL) {
        for (_j in range(OUTER)) {
            for (i in range(N)) {
                arr[i] = 1
            }
        }
    }
    b |> run("read_int_idx/{TOTAL}", TOTAL) {
        var sum = 0
        for (_j in range(OUTER)) {
            for (i in range(N)) {
                sum += arr[i]
            }
        }
        strictEqual(b, length(arr), N)
    }
}

[benchmark]
def arr_at_int64_idx(b : B?) {
    var arr : array<int>
    arr |> resize(N)
    let N64 = int64(N)
    b |> run("write_int64_idx/{TOTAL}", TOTAL) {
        for (_j in range(OUTER)) {
            for (i in range64(N64)) {
                arr[i] = 1
            }
        }
    }
    b |> run("read_int64_idx/{TOTAL}", TOTAL) {
        var sum = 0
        for (_j in range(OUTER)) {
            for (i in range64(N64)) {
                sum += arr[i]
            }
        }
        strictEqual(b, length(arr), N)
    }
}

[benchmark]
def arr_at_uint64_idx(b : B?) {
    var arr : array<int>
    arr |> resize(N)
    let N64 = uint64(N)
    b |> run("write_uint64_idx/{TOTAL}", TOTAL) {
        for (_j in range(OUTER)) {
            for (i in urange64(N64)) {
                arr[i] = 1
            }
        }
    }
    b |> run("read_uint64_idx/{TOTAL}", TOTAL) {
        var sum = 0
        for (_j in range(OUTER)) {
            for (i in urange64(N64)) {
                sum += arr[i]
            }
        }
        strictEqual(b, length(arr), N)
    }
}
