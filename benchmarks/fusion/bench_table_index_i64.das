options gen2

require dastest/testing_boost

// Side-by-side cost of int-keyed vs int64-keyed table indexing.
// The fusion engine already had int64/uint64 key variants
// registered via IMPLEMENT_SETOP_NUMERIC(TableIndex); this
// benchmark confirms the int64 path stays within ~10% of the
// int32 path on the same workload.

let N = 10000

[benchmark]
def table_index_int_key(b : B?) {
    var tab : table<int; int>
    for (i in range(N)) {
        tab[i] = i * 2
    }
    b |> run("read_int_key/{N}", N) {
        var sum = 0
        for (i in range(N)) {
            sum += tab[i]
        }
        strictEqual(b, sum, N * (N - 1))
    }
}

[benchmark]
def table_index_int64_key(b : B?) {
    var tab : table<int64; int>
    let N64 = int64(N)
    var i = 0_l
    while (i < N64) {
        tab[i] = int(i) * 2
        i++
    }
    b |> run("read_int64_key/{N}", N) {
        var sum = 0
        var j = 0_l
        while (j < N64) {
            sum += tab[j]
            j++
        }
        strictEqual(b, sum, N * (N - 1))
    }
}

[benchmark]
def table_index_uint64_key(b : B?) {
    var tab : table<uint64; int>
    let N64 = uint64(N)
    var i = 0_ul
    while (i < N64) {
        tab[i] = int(i) * 2
        i++
    }
    b |> run("read_uint64_key/{N}", N) {
        var sum = 0
        var j = 0_ul
        while (j < N64) {
            sum += tab[j]
            j++
        }
        strictEqual(b, sum, N * (N - 1))
    }
}
