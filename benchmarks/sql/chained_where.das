options gen2
options persistent_heap

require _common public

let THRESHOLD = 500
let YEAR_FLOOR = 2015

// where → where → count — two filter stages then count.
// SQL: SELECT COUNT(*) ... WHERE p1 AND p2 — engine combines predicates.
// Array variants today: m3 materializes after each where; m3f folds two whereS into one fused loop.
// Splice mode should fuse `where(p1) |> where(p2) |> count` into a single pass with both predicates inlined.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let c = _sql(db |> select_from(type<Car>)
                            |> _where(_.price > THRESHOLD)
                            |> _where(_.year >= YEAR_FLOOR)
                            |> count())
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let c = (arr |> _where(_.price > THRESHOLD)
                     |> _where(_.year >= YEAR_FLOOR)
                     |> count())
        if (c == 0) {
            b->failNow()
        }
    }
}

def run_m3f_old(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_old_array_fold/{n}", n) {
        let c = _old_fold(each(arr)._where(_.price > THRESHOLD)
                                   ._where(_.year >= YEAR_FLOOR)
                                   .count())
        if (c == 0) {
            b->failNow()
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let c = _fold(each(arr)._where(_.price > THRESHOLD)
                               ._where(_.year >= YEAR_FLOOR)
                               .count())
        if (c == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def chained_where_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def chained_where_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def chained_where_m3f_old(b : B?) {
    run_m3f_old(b, 100000)
}

[benchmark]
def chained_where_m3f(b : B?) {
    run_m3f(b, 100000)
}
