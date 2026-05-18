options gen2
options persistent_heap

require _common public

let TAKE_N = 3

// _select(_.brand) |> distinct() |> take(N) — early-exit dedup pattern.
// SQL: SELECT DISTINCT brand FROM Cars LIMIT N — engine bounds the dedup hashset.
// m3 (plain LINQ) materializes full distinct array first, then slices.
// m3f (spliced via plan_distinct): streaming dedup with break-after-N. With BRAND_COUNT=5
// and TAKE_N=3, this exits the source scan as soon as 3 unique brands have been observed —
// in worst case ~3 elements (first 3 distinct brands at indices 0,1,2).

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let rows <- _sql(db |> select_from(type<Car>) |> _select(_.brand) |> distinct() |> take(TAKE_N))
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let rows <- (arr |> _select(_.brand) |> distinct() |> take(TAKE_N))
        if (empty(rows)) {
            b->failNow()
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let rows <- _fold(arr.to_sequence()._select(_.brand).distinct().take(TAKE_N).to_array())
        if (empty(rows)) {
            b->failNow()
        }
    }
}

[benchmark]
def distinct_take_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def distinct_take_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def distinct_take_m3f(b : B?) {
    run_m3f(b, 100000)
}
