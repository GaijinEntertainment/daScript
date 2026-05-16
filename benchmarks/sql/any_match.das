options gen2
options persistent_heap

require _common public

let THRESHOLD = 500

// SQL: SELECT ... LIMIT 1 — engine stops at first hit. Use _first_opt then is_some
// since sqlite_linq doesn't expose `_any()` as a chain terminal.
// Array variants use `_any(pred)` which short-circuits at the first match.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let opt = _sql(db |> select_from(type<Car>) |> _where(_.price > THRESHOLD) |> _first_opt())
            if (!is_some(opt)) {
                b->failNow()
            }
        }
    }
}

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let yes = arr |> _any(_.price > THRESHOLD)
        if (!yes) {
            b->failNow()
        }
    }
}

def run_m3f_old(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_old_array_fold/{n}", n) {
        let yes = _old_fold(each(arr)._any(_.price > THRESHOLD))
        if (!yes) {
            b->failNow()
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let yes = _fold(each(arr)._any(_.price > THRESHOLD))
        if (!yes) {
            b->failNow()
        }
    }
}

[benchmark]
def any_match_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def any_match_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def any_match_m3f_old(b : B?) {
    run_m3f_old(b, 100000)
}

[benchmark]
def any_match_m3f(b : B?) {
    run_m3f(b, 100000)
}
