options gen2
options persistent_heap

require _common public

let THRESHOLD = 50000

// zip(xs, ys) |> _select(_._0 * _._1) |> count(P) — count of zipped products satisfying P.
// SQL: no relational form (zip is not relational).
// Array (m3f): plan_zip's 2-arg count(P) extension (Theme 4, plan_zip arm).
// The predicate is captured into a dedicated counter-predicate gate around `acc++`
// INSIDE the upstream select wrap, so eager `select(F).count(P)` ordering is preserved
// (F runs once per element, then P decides whether to count). With _select, the
// predicate peels against the projected value via a `vproj` bind. Length-shortcut
// suppressed once P is present.

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
        unsafe {
            let c = _fold(each(xs) |> zip(each(ys)) |> _select(_._0 * _._1) |> count($(v) => v > THRESHOLD))
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def zip_count_pred_m3f(b : B?) {
    run_m3f(b, 100000)
}
