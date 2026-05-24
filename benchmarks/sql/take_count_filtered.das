options gen2
options persistent_heap

require _common public

let TAKE_N = 1000
let THRESHOLD = 500

// `_sql` rejects `take(n) |> count()` — semantic mismatch: in SQL, LIMIT after an
// aggregate has no effect (aggregate collapses to one row), so the bound-then-count
// shape has no faithful SQL translation. [By design — no follow-up TODO.] Array and
// Decs lanes filter + bound + count over the source; exercises counter-lane take
// splice with an upstream `where` predicate.

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let c = _fold(each(arr)._where(_.price > THRESHOLD).take(TAKE_N).count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let c = _fold(from_decs_template(type<DecsCar>)._where(_.price > THRESHOLD).take(TAKE_N).count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def take_count_filtered_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def take_count_filtered_m4(b : B?) {
    run_m4(b, 100000)
}
