options gen2
options persistent_heap

require _common public

let THRESHOLD = 500
let INDEX = 100

// SQL: SELECT ... WHERE price > T LIMIT 1 OFFSET N — engine skips N then takes 1.
// m3 materializes full filtered array then indexes [N].
// m3f counts matches and early-exits when counter hits N.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let row = _sql(db |> select_from(type<Car>) |> _where(_.price > THRESHOLD)
                              |> skip(INDEX) |> _first())
            if (row.price == 0) {
                b->failNow()
            }
        }
    }
}

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let row = arr |> _where(_.price > THRESHOLD) |> element_at(INDEX)
        if (row.price == 0) {
            b->failNow()
        }
    }
}
def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let row = _fold(each(arr)._where(_.price > THRESHOLD).element_at(INDEX))
        if (row.price == 0) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let row = _fold(from_decs_template(type<DecsCar>)._where(_.price > THRESHOLD).element_at(INDEX))
        if (row.price == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def element_at_match_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def element_at_match_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def element_at_match_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def element_at_match_m4(b : B?) {
    run_m4(b, 100000)
}
