options gen2

require dastest/testing_boost

// Apples-to-apples cost of int / int64 / uint64-indexed array access.
// All three loops use the same `for (i in <range-iter>(N))` shape,
// differing only in the index type. Pre-Phase-5: int64 / uint64 paths
// fell off fusion (no _I64 / _U64 fusion variant). Post-Phase-5: those
// paths fuse via SimNode_Op2ArrayAt_I64 / _U64.

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
        strictEqual(b, length(arr), N)
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
    let N64 = int64(N)
    for (i in range64(N64)) {
        arr[i] = int(i)
    }
    b |> run("read_int64_idx/{N}", N) {
        var sum = 0
        for (i in range64(N64)) {
            sum += arr[i]
        }
        strictEqual(b, length(arr), N)
    }
    b |> run("write_int64_idx/{N}", N) {
        for (i in range64(N64)) {
            arr[i] = int(i) * 2
        }
    }
}

[benchmark]
def arr_at_uint64_idx(b : B?) {
    var arr : array<int>
    arr |> resize(N)
    let N64 = uint64(N)
    for (i in urange64(N64)) {
        arr[i] = int(i)
    }
    b |> run("read_uint64_idx/{N}", N) {
        var sum = 0
        for (i in urange64(N64)) {
            sum += arr[i]
        }
        strictEqual(b, length(arr), N)
    }
    b |> run("write_uint64_idx/{N}", N) {
        for (i in urange64(N64)) {
            arr[i] = int(i) * 2
        }
    }
}
