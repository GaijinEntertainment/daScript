options gen2
options persistent_heap

require dastest/testing_boost

let M = 50000

var g_sink : int64

def private make_ints(n : int) {
    var res <- [for (i in range(n)); i & 1023]
    return <- res
}

//! `panic` is noreturn: its block leaves the loop, so `total`
//! stays in a register instead of a load and a store per element
[hint(noinline)]
def private checked_sum(a : array<int>; var total : int&) {
    for (x in a) {
        if (x < 0) {
            panic("negative input")
        }
        total += x
    }
}

[benchmark]
def attribute_effects(b : B?) {
    let ia <- make_ints(M)
    b |> run("noreturn_checked_sum/{M}", 1) {
        var t = 0
        checked_sum(ia, t)
        g_sink += int64(t)
    }
}
