options gen2
options persistent_heap

require _common public

// SQL: SELECT SUM(price) FROM Cars — single-row scalar reduction pushed to the engine.
// m3/m3f_old/m3f traverse the array projecting and accumulating.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let s = _sql(db |> select_from(type<Car>) |> _select(_.price) |> sum())
            if (s == 0) {
                b->failNow()
            }
        }
    }
}

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let s = arr |> _select(_.price) |> sum()
        if (s == 0) {
            b->failNow()
        }
    }
}

def run_m3f_old(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_old_array_fold/{n}", n) {
        let s = _old_fold(each(arr)._select(_.price).sum())
        if (s == 0) {
            b->failNow()
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let s = _fold(each(arr)._select(_.price).sum())
        if (s == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def sum_aggregate_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def sum_aggregate_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def sum_aggregate_m3f_old(b : B?) {
    run_m3f_old(b, 100000)
}

[benchmark]
def sum_aggregate_m3f(b : B?) {
    run_m3f(b, 100000)
}
