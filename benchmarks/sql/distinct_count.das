options gen2
options persistent_heap

require _common public

// _select(_.brand) |> distinct() |> to_array — distinct values then materialize.
// SQL: SELECT DISTINCT brand FROM Cars — engine builds a hashset.
// Array variants must materialize a set/dedup before producing results.
// (sqlite_linq's `distinct() |> count()` form raises a not-implemented error
// for `COUNT(DISTINCT col)`; we measure the materialized distinct set instead.)
// Expected length: BRAND_COUNT (5).

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let rows <- _sql(db |> select_from(type<Car>) |> _select(_.brand) |> distinct())
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let rows <- (arr |> _select(_.brand) |> distinct())
        if (empty(rows)) {
            b->failNow()
        }
    }
}

def run_m3f_old(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_old_array_fold/{n}", n) {
        let rows <- _old_fold(each(arr)._select(_.brand).distinct().to_array())
        if (empty(rows)) {
            b->failNow()
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let rows <- _fold(each(arr)._select(_.brand).distinct().to_array())
        if (empty(rows)) {
            b->failNow()
        }
    }
}

[benchmark]
def distinct_count_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def distinct_count_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def distinct_count_m3f_old(b : B?) {
    run_m3f_old(b, 100000)
}

[benchmark]
def distinct_count_m3f(b : B?) {
    run_m3f(b, 100000)
}
