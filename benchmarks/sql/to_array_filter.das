options gen2
options persistent_heap

require _common public

let THRESHOLD = 500

// where → select → to_array. Three-stage pipeline producing an array of ints.
// Distinguishes from select_where.das which lacks the select stage.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let prices <- _sql(db |> select_from(type<Car>) |> _where(_.price > THRESHOLD) |> _select(_.price))
            if (empty(prices)) {
                b->failNow()
            }
        }
    }
}

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let prices <- (arr |> _where(_.price > THRESHOLD) |> _select(_.price))
        if (empty(prices)) {
            b->failNow()
        }
    }
}

def run_m3f_old(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_old_array_fold/{n}", n) {
        let prices <- _old_fold(each(arr)._where(_.price > THRESHOLD)._select(_.price).to_array())
        if (empty(prices)) {
            b->failNow()
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let prices <- _fold(each(arr)._where(_.price > THRESHOLD)._select(_.price).to_array())
        if (empty(prices)) {
            b->failNow()
        }
    }
}

[benchmark]
def to_array_filter_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def to_array_filter_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def to_array_filter_m3f_old(b : B?) {
    run_m3f_old(b, 100000)
}

[benchmark]
def to_array_filter_m3f(b : B?) {
    run_m3f(b, 100000)
}
