options gen2
options persistent_heap

require dastest/testing_boost
require strings

let M = 50000

var g_sink : int64

def private make_ints(n : int) {
    var res <- [for (i in range(n)); i]
    return <- res
}

def private make_keys(n : int; prefix : string) {
    var res <- [for (i in range(n)); "{prefix}key_{i % 97}_padding_tail"]
    return <- res
}

//! a typical string helper: peek_data takes a block, so each call sets up a frame too
def private count_byte(s : string; c : int) : int {
    var n = 0
    peek_data(s) $(bytes) {
        for (x in bytes) {
            if (int(x) == c) {
                n++
            }
        }
    }
    return n
}

//! the block form of a table lookup, one block frame per call
def private lookup_or_zero(tab : table<int; int>; k : int) : int {
    var r = 0
    tab |> get(k) $(v) {
        r = v
    }
    return r
}

[benchmark]
def per_call_overhead(b : B?) {
    let small <- make_ints(4)
    b |> run("array_loop_entry/{M}", M) {
        var s = 0
        for (i in range(M)) {
            for (x in small) {
                s = (s << 1) ^ x ^ i
            }
        }
        g_sink += int64(s)
    }
    var tab <- { 1 => 1, 2 => 2, 3 => 3, 4 => 4 }
    b |> run("table_keys_loop_entry/{M}", M) {
        var s = 0
        for (i in range(M)) {
            for (k in keys(tab)) {
                s = (s << 1) ^ k ^ i
            }
        }
        g_sink += int64(s)
    }
    b |> run("table_get_block/{M}", M) {
        var s = 0
        for (i in range(M)) {
            s += lookup_or_zero(tab, i & 7)
        }
        g_sink += int64(s)
    }
    let ka <- make_keys(M, "")
    b |> run("peek_data_helper/{M}", M) {
        var s = 0
        for (i in range(M)) {
            s += count_byte(ka[i], '_')
        }
        g_sink += int64(s)
    }
    let kz <- [for (k in ka); "{k}z"]
    b |> run("string_compare_long_prefix/{M}", M) {
        var c = 0
        for (i in range(M)) {
            if (ka[i] != kz[i]) {
                c = (c << 1) ^ 1
            }
        }
        g_sink += int64(c)
    }
}
