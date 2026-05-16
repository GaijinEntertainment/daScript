options gen2
options persistent_heap

require _common public

let THRESHOLD = 500

// where → select → sum — three-stage: filter then project then reduce.
// SQL: SELECT SUM(price) WHERE price > THRESHOLD — fully pushed to engine.
// Array variants today materialize the filtered array, project, then sum.
// Splice mode should fuse the three stages into a single accumulator loop.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let s = _sql(db |> select_from(type<Car>) |> _where(_.price > THRESHOLD) |> _select(_.price) |> sum())
            if (s == 0) {
                b->failNow()
            }
        }
    }
}

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let s = (arr |> _where(_.price > THRESHOLD) |> _select(_.price) |> sum())
        if (s == 0) {
            b->failNow()
        }
    }
}

def run_m3f_old(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_old_array_fold/{n}", n) {
        let s = _old_fold(each(arr)._where(_.price > THRESHOLD)._select(_.price).sum())
        if (s == 0) {
            b->failNow()
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let s = _fold(each(arr)._where(_.price > THRESHOLD)._select(_.price).sum())
        if (s == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def sum_where_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def sum_where_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def sum_where_m3f_old(b : B?) {
    run_m3f_old(b, 100000)
}

[benchmark]
def sum_where_m3f(b : B?) {
    run_m3f(b, 100000)
}
