options gen2

require dastest/testing_boost

// Apples-to-apples cost of int / int64 / uint64-keyed table indexing.
// All three loops use the same `for (k in <range-iter>(N))` shape,
// differing only in the key type. The fusion engine already had
// int64/uint64 key variants registered via
// IMPLEMENT_SETOP_NUMERIC(TableIndex) — this bench confirms the int64
// path stays close to the int32 path on the same workload.

let N = 10000

[benchmark]
def table_index_int_key(b : B?) {
    var tab : table<int; int>
    for (k in range(N)) {
        tab[k] = k * 2
    }
    b |> run("read_int_key/{N}", N) {
        var sum = 0
        for (k in range(N)) {
            sum += tab[k]
        }
        strictEqual(b, length(tab), N)
    }
}

[benchmark]
def table_index_int64_key(b : B?) {
    var tab : table<int64; int>
    let N64 = int64(N)
    for (k in range64(N64)) {
        tab[k] = int(k) * 2
    }
    b |> run("read_int64_key/{N}", N) {
        var sum = 0
        for (k in range64(N64)) {
            sum += tab[k]
        }
        strictEqual(b, length(tab), N)
    }
}

[benchmark]
def table_index_uint64_key(b : B?) {
    var tab : table<uint64; int>
    let N64 = uint64(N)
    for (k in urange64(N64)) {
        tab[k] = int(k) * 2
    }
    b |> run("read_uint64_key/{N}", N) {
        var sum = 0
        for (k in urange64(N64)) {
            sum += tab[k]
        }
        strictEqual(b, length(tab), N)
    }
}
