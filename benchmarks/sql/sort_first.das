options gen2
options persistent_heap

require _common public

// _order_by(_.price) |> first — sort then take first (minimum by sort key).
// SQL: ORDER BY price LIMIT 1 — engine can use an index or partial sort.
// Array variants without specific min-detection must fully sort then index [0].

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let row = _sql(db |> select_from(type<Car>) |> _order_by(_.price) |> _first())
            if (row.id == 0) {
                b->failNow()
            }
        }
    }
}

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let row = arr |> _order_by(_.price) |> first()
        if (row.id == 0) {
            b->failNow()
        }
    }
}

def run_m3f_old(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_old_array_fold/{n}", n) {
        let row = _old_fold(each(arr)._order_by(_.price).first())
        if (row.id == 0) {
            b->failNow()
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let row = _fold(each(arr)._order_by(_.price).first())
        if (row.id == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def sort_first_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def sort_first_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def sort_first_m3f_old(b : B?) {
    run_m3f_old(b, 100000)
}

[benchmark]
def sort_first_m3f(b : B?) {
    run_m3f(b, 100000)
}
