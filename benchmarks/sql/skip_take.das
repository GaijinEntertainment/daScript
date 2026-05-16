options gen2
options persistent_heap

require _common public

let SKIP_N = 1000
let TAKE_N = 100

// skip(M) |> take(K) |> to_array — windowing.
// SQL: LIMIT K OFFSET M.
// Same SQL-aggregate restriction as take_count: aggregate after take/skip is rejected,
// so we measure the materialized window and check length.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let rows <- _sql(db |> select_from(type<Car>) |> skip(SKIP_N) |> take(TAKE_N))
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let rows <- (arr |> skip(SKIP_N) |> take(TAKE_N))
        if (empty(rows)) {
            b->failNow()
        }
    }
}

def run_m3f_old(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_old_array_fold/{n}", n) {
        let rows <- _old_fold(each(arr).skip(SKIP_N).take(TAKE_N).to_array())
        if (empty(rows)) {
            b->failNow()
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let rows <- _fold(each(arr).skip(SKIP_N).take(TAKE_N).to_array())
        if (empty(rows)) {
            b->failNow()
        }
    }
}

[benchmark]
def skip_take_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def skip_take_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def skip_take_m3f_old(b : B?) {
    run_m3f_old(b, 100000)
}

[benchmark]
def skip_take_m3f(b : B?) {
    run_m3f(b, 100000)
}
