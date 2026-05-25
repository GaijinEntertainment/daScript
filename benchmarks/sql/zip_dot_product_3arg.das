options gen2
options persistent_heap

require _common public

// zip(a, b, sel) |> sum — dot product via 3-arg zip pre-lowering (Theme 1, audit 7a).
// The 3-arg form `zip(a, b, sel)` is pre-lowered by plan_zip to
// `zip(a, b) |> _select(sel-as-tuple)` so the standard zip+select fusion fires —
// this bench measures the natural dot-product idiom that previously cascaded.
// No SQL form (zip is not relational); no decs form (no zip on archetype walks).
// Companion to zip_dot_product.das which uses the explicit 2-arg + _select form.

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
        let s = _fold(zip(xs, ys, $(x : int; y : int) => x * y).sum())
        b |> accept(s)
        if (s == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def zip_dot_product_3arg_m3f(b : B?) {
    run_m3f(b, 100000)
}
