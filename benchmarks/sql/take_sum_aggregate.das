options gen2
options persistent_heap

require _common public

let TAKE_N = 1000

// `_sql` rejects `take(n) |> sum()` (LIMIT-before-aggregate has no effect in SQLite — aggregate
// collapses to one row regardless), so the m1 variant is omitted. m3/m3f_old/m3f bound the
// projection-sum loop to the first TAKE_N matched elements (no upstream where here, so
// "matched" == "every source element"). Exercises accumulator-lane take splice in _fold.

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let s = arr |> _select(_.price) |> take(TAKE_N) |> sum()
        if (s == 0) {
            b->failNow()
        }
    }
}

def run_m3f_old(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_old_array_fold/{n}", n) {
        let s = _old_fold(each(arr)._select(_.price).take(TAKE_N).sum())
        if (s == 0) {
            b->failNow()
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let s = _fold(each(arr)._select(_.price).take(TAKE_N).sum())
        if (s == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def take_sum_aggregate_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def take_sum_aggregate_m3f_old(b : B?) {
    run_m3f_old(b, 100000)
}

[benchmark]
def take_sum_aggregate_m3f(b : B?) {
    run_m3f(b, 100000)
}
