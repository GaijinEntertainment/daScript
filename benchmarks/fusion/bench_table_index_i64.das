options gen2

require dastest/testing_boost

// Apples-to-apples cost of int / int64 / uint64-keyed table indexing.
// All three benches use the same `for (k in <range-iter>(N))` shape,
// differing only in the key type. Read workload is `sum += tab[k]`
// (no cast in the body). The outer `for (j in range(OUTER))` amortizes
// per-body harness overhead over OUTER * N inner ops.

let N = 10000
let OUTER = 10
let TOTAL = N * OUTER

[benchmark]
def table_index_int_key(b : B?) {
    var tab : table<int; int>
    for (k in range(N)) {
        tab[k] = 1
    }
    b |> run("read_int_key/{TOTAL}", TOTAL) {
        var sum = 0
        for (_j in range(OUTER)) {
            for (k in range(N)) {
                sum += tab[k]
            }
        }
        strictEqual(b, length(tab), N)
    }
}

[benchmark]
def table_index_int64_key(b : B?) {
    var tab : table<int64; int>
    let N64 = int64(N)
    for (k in range64(N64)) {
        tab[k] = 1
    }
    b |> run("read_int64_key/{TOTAL}", TOTAL) {
        var sum = 0
        for (_j in range(OUTER)) {
            for (k in range64(N64)) {
                sum += tab[k]
            }
        }
        strictEqual(b, length(tab), N)
    }
}

[benchmark]
def table_index_uint64_key(b : B?) {
    var tab : table<uint64; int>
    let N64 = uint64(N)
    for (k in urange64(N64)) {
        tab[k] = 1
    }
    b |> run("read_uint64_key/{TOTAL}", TOTAL) {
        var sum = 0
        for (_j in range(OUTER)) {
            for (k in urange64(N64)) {
                sum += tab[k]
            }
        }
        strictEqual(b, length(tab), N)
    }
}
