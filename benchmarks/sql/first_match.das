options gen2
options persistent_heap

require _common public

let THRESHOLD = 500

// SQL: SELECT ... LIMIT 1 — engine stops at first hit.
// m3 materializes the full filtered array then indexes [0].
// m3f_old / m3f should ideally early-exit at the first matching row;
// splice mode is the main target here.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let row = _sql(db |> select_from(type<Car>) |> _where(_.price > THRESHOLD) |> _first())
            if (row.price == 0) {
                b->failNow()
            }
        }
    }
}

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let row = arr |> _where(_.price > THRESHOLD) |> first()
        if (row.price == 0) {
            b->failNow()
        }
    }
}

def run_m3f_old(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_old_array_fold/{n}", n) {
        let row = _old_fold(each(arr)._where(_.price > THRESHOLD).first())
        if (row.price == 0) {
            b->failNow()
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let row = _fold(each(arr)._where(_.price > THRESHOLD).first())
        if (row.price == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def first_match_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def first_match_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def first_match_m3f_old(b : B?) {
    run_m3f_old(b, 100000)
}

[benchmark]
def first_match_m3f(b : B?) {
    run_m3f(b, 100000)
}
