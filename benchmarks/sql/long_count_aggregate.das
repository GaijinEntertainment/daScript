options gen2
options persistent_heap

require _common public

let THRESHOLD = 500

// long_count mirrors count but with int64 accumulator semantics — same access pattern,
// same fusion shape, just a wider counter. The 4-way comparison validates that the
// accumulator-lane planner matches counter-lane perf for the int64 variant.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let c = _sql(db |> select_from(type<Car>) |> _where(_.price > THRESHOLD) |> count())
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let c = arr |> _where(_.price > THRESHOLD) |> long_count()
        if (c == 0l) {
            b->failNow()
        }
    }
}

def run_m3f_old(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_old_array_fold/{n}", n) {
        let c = _old_fold(each(arr)._where(_.price > THRESHOLD).long_count())
        if (c == 0l) {
            b->failNow()
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let c = _fold(each(arr)._where(_.price > THRESHOLD).long_count())
        if (c == 0l) {
            b->failNow()
        }
    }
}

[benchmark]
def long_count_aggregate_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def long_count_aggregate_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def long_count_aggregate_m3f_old(b : B?) {
    run_m3f_old(b, 100000)
}

[benchmark]
def long_count_aggregate_m3f(b : B?) {
    run_m3f(b, 100000)
}
