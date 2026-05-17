options gen2
options persistent_heap

require _common public

let TAKE_N = 1000
let THRESHOLD = 500

// `_sql` rejects `take(n) |> count()` (LIMIT-before-aggregate collapses to one row regardless
// in SQLite), so the m1 variant is omitted. m3/m3f_old/m3f filter + bound + count over the
// array. Exercises counter-lane take splice with an upstream `where` predicate.

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let c = arr |> _where(_.price > THRESHOLD) |> take(TAKE_N) |> count()
        if (c == 0) {
            b->failNow()
        }
    }
}

def run_m3f_old(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_old_array_fold/{n}", n) {
        let c = _old_fold(each(arr)._where(_.price > THRESHOLD).take(TAKE_N).count())
        if (c == 0) {
            b->failNow()
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let c = _fold(each(arr)._where(_.price > THRESHOLD).take(TAKE_N).count())
        if (c == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def take_count_filtered_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def take_count_filtered_m3f_old(b : B?) {
    run_m3f_old(b, 100000)
}

[benchmark]
def take_count_filtered_m3f(b : B?) {
    run_m3f(b, 100000)
}
