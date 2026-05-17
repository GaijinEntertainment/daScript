options gen2
options persistent_heap

require _common public

let THRESHOLD = 500
let SENTINEL_ID = -1

// first_or_default mirrors first but returns a caller-provided default on empty/no-match
// instead of panicking. Same early-exit characteristics; default bound once at the top of
// the invoke block (matches linq.das line 2397 eager evaluation).

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
    let sentinel = Car(id = SENTINEL_ID, name = "none", price = 0, brand = 0, year = 0, dealer_id = 0)
    b |> run("m3_array/{n}", n) {
        let row = arr |> _where(_.price > THRESHOLD) |> first_or_default(sentinel)
        if (row.id == SENTINEL_ID) {
            b->failNow()
        }
    }
}

def run_m3f_old(b : B?; n : int) {
    let arr <- fixture_array(n)
    let sentinel = Car(id = SENTINEL_ID, name = "none", price = 0, brand = 0, year = 0, dealer_id = 0)
    b |> run("m3f_old_array_fold/{n}", n) {
        let row = _old_fold(each(arr)._where(_.price > THRESHOLD).first_or_default(sentinel))
        if (row.id == SENTINEL_ID) {
            b->failNow()
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    let sentinel = Car(id = SENTINEL_ID, name = "none", price = 0, brand = 0, year = 0, dealer_id = 0)
    b |> run("m3f_array_fold/{n}", n) {
        let row = _fold(each(arr)._where(_.price > THRESHOLD).first_or_default(sentinel))
        if (row.id == SENTINEL_ID) {
            b->failNow()
        }
    }
}

[benchmark]
def first_or_default_match_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def first_or_default_match_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def first_or_default_match_m3f_old(b : B?) {
    run_m3f_old(b, 100000)
}

[benchmark]
def first_or_default_match_m3f(b : B?) {
    run_m3f(b, 100000)
}
