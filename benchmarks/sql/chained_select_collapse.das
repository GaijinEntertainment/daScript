options gen2
options persistent_heap

require _common public

// _select(_.brand) |> _select(_ + 1) |> distinct() |> count — two adjacent _select calls
// upstream of plan_distinct (Theme 7, audit 7c-family). The collapse_chained_selects
// pre-pass folds the two pure projections into a single composed _select(g(f(_))) before
// plan_distinct dispatches; the downstream splice sees one _select instead of two,
// dropping a per-element bind+invoke. Pure inner gate satisfied (`_ + 1` has no
// sideeffects per the macro_boost.das classifier).
// SQL: sqlite_linq's _distinct_by lowering rejects computed-projection keys
// (`error[50503]: _distinct_by key must be <arg>.<field>`). By design — no follow-up.

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let c = _fold(each(arr) |> _select(_.brand) |> _select(_ + 1) |> distinct() |> count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let c = _fold(from_decs_template(type<DecsCar>) |> _select(_.brand) |> _select(_ + 1) |> distinct() |> count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def chained_select_collapse_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def chained_select_collapse_m4(b : B?) {
    run_m4(b, 100000)
}
