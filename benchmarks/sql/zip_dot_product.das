options gen2
options persistent_heap

require _common public

// zip(a, b) |> select(t => int64(t._0) * int64(t._1)) |> sum — dot product of two int sequences.
// No clean SQL form (zip is not a natural relational operation), so the SQL lane is
// omitted. Splice mode fuses zip + select + sum into a single accumulator loop.
// The Decs lane is the intra-archetype analogue: two int components on the same
// DecsCar entity (price * year), summed over all entities. Wave 4 pruning keeps
// only those two get_ro slots, so per-element cost matches the Array two-column read.
// int64 cast in the projection avoids int32 overflow on i*i for n=100k (sum of
// squares is ~3.3e14, well past INT_MAX).

def make_ints(n : int) : array<int> {
    var a : array<int>
    a |> resize(n)
    for (i in range(n)) {
        a[i] = i
    }
    return <- a
}

def run_m3f(b : B?; n : int) {
    let xs <- make_ints(n)
    let ys <- make_ints(n)
    b |> run("m3f_array_fold/{n}", n) {
        let s = _fold(zip(xs, ys)._select(int64(_._0) * int64(_._1)).sum())
        b |> accept(s)
        if (s == 0l) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let s = _fold(from_decs_template(type<DecsCar>)._select(int64(_.price) * int64(_.year)).sum())
        b |> accept(s)
        if (s == 0l) {
            b->failNow()
        }
    }
}

[benchmark]
def zip_dot_product_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def zip_dot_product_m4(b : B?) {
    run_m4(b, 100000)
}
