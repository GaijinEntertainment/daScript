options gen2
options persistent_heap

require _common public

// User-supplied binary reducer: sum of prices over a where-filtered slice. SQL
// can express this as `SELECT SUM(price)`. m3 traverses the array with the
// user-block invoked per element; m3f peels the block body and inlines alongside
// the where predicate — single pass with no per-element block invoke.

let THRESHOLD = 200

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let total = _sql(db |> select_from(type<Car>) |> _where(_.price > THRESHOLD)
                                |> _select(_.price) |> sum())
            if (total == 0) {
                b->failNow()
            }
        }
    }
}

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let total = (arr |> _where(_.price > THRESHOLD)
                         |> aggregate(0, $(acc : int, c : Car) => acc + c.price))
        if (total == 0) {
            b->failNow()
        }
    }
}
def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let total = _fold(each(arr)._where(_.price > THRESHOLD)
            .aggregate(0, $(acc : int, c : Car) => acc + c.price))
        if (total == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def aggregate_match_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def aggregate_match_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def aggregate_match_m3f(b : B?) {
    run_m3f(b, 100000)
}
