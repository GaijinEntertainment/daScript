options gen2
options persistent_heap

require _common public

// zip(a, b) |> select(t => t._0 * t._1) |> sum — dot product of two int sequences.
// No clean SQL form (zip is not a natural relational operation), so m1 is omitted.
// Splice mode should fuse zip + select + sum into a single accumulator loop.

def make_ints(n : int) : array<int> {
    var a : array<int>
    a |> resize(n)
    for (i in range(n)) {
        a[i] = i
    }
    return <- a
}

def run_m3(b : B?; n : int) {
    let xs <- make_ints(n)
    let ys <- make_ints(n)
    b |> run("m3_array/{n}", n) {
        let s = (zip(xs, ys) |> _select(_._0 * _._1) |> sum())
        if (s == 0) {
            b->failNow()
        }
    }
}

def run_m3f_old(b : B?; n : int) {
    let xs <- make_ints(n)
    let ys <- make_ints(n)
    b |> run("m3f_old_array_fold/{n}", n) {
        let s = _old_fold(zip(xs, ys)._select(_._0 * _._1).sum())
        if (s == 0) {
            b->failNow()
        }
    }
}

def run_m3f(b : B?; n : int) {
    let xs <- make_ints(n)
    let ys <- make_ints(n)
    b |> run("m3f_array_fold/{n}", n) {
        let s = _fold(zip(xs, ys)._select(_._0 * _._1).sum())
        if (s == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def zip_dot_product_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def zip_dot_product_m3f_old(b : B?) {
    run_m3f_old(b, 100000)
}

[benchmark]
def zip_dot_product_m3f(b : B?) {
    run_m3f(b, 100000)
}
