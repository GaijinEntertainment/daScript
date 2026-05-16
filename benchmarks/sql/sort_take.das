options gen2
options persistent_heap

require _common public

let TAKE_N = 10

// _order_by(_.price) |> take(N) — top-N (here bottom-N since ascending) pattern.
// SQL: ORDER BY price LIMIT N — engine emits partial-sort optimization when possible.
// Array variants must fully sort before indexing the first N.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let rows <- _sql(db |> select_from(type<Car>) |> _order_by(_.price) |> take(TAKE_N))
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let rows <- (arr |> _order_by(_.price) |> take(TAKE_N))
        if (empty(rows)) {
            b->failNow()
        }
    }
}

def run_m3f_old(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_old_array_fold/{n}", n) {
        let rows <- _old_fold(each(arr)._order_by(_.price).take(TAKE_N).to_array())
        if (empty(rows)) {
            b->failNow()
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let rows <- _fold(each(arr)._order_by(_.price).take(TAKE_N).to_array())
        if (empty(rows)) {
            b->failNow()
        }
    }
}

[benchmark]
def sort_take_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def sort_take_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def sort_take_m3f_old(b : B?) {
    run_m3f_old(b, 100000)
}

[benchmark]
def sort_take_m3f(b : B?) {
    run_m3f(b, 100000)
}
