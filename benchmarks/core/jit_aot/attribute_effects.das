options gen2
options persistent_heap

require dastest/testing_boost

let M = 50000

var g_sink : int64

struct Acc {
    total : int
}

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

//! the heap allocator returns noalias: `p.total` is none of `a`'s
//! elements, so it stays in a register, though no runtime check
//! can cover the indirect read `a[i]`
[hint(noinline)]
def private gather_into_new(a : array<int>; idx : array<int>) : int {
    var p = new Acc()
    for (i in idx) {
        p.total += a[i]
    }
    let r = p.total
    unsafe {
        delete p
    }
    return r
}

[benchmark]
def attribute_effects(b : B?) {
    let ia <- make_ints(M)
    b |> run("noreturn_checked_sum/{M}", 1) {
        var t = 0
        checked_sum(ia, t)
        g_sink += int64(t)
    }
    let idx <- [for (i in range(M)); (i * 7919) % M]
    b |> run("noalias_alloc_gather/{M}", 1) {
        g_sink += int64(gather_into_new(ia, idx))
    }
}
