options gen2
options persistent_heap

require _common public

// Predicate that is ALWAYS true (price < 9999), so the full scan runs.
// SQL equivalent of `all(pred)` is `count(where NOT pred) == 0` — no direct
// `_all` terminal in sqlite_linq, so we phrase it as a counter-example count.
// Array variants use `_all(pred)` which short-circuits on first failing element;
// always-true predicate forces full-array traversal (fair upper-bound timing).

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let bad = _sql(db |> select_from(type<Car>) |> _where(_.price >= 9999) |> count())
            if (bad != 0) {
                b->failNow()
            }
        }
    }
}

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let yes = arr |> _all(_.price < 9999)
        if (!yes) {
            b->failNow()
        }
    }
}

def run_m3f_old(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_old_array_fold/{n}", n) {
        let yes = _old_fold(each(arr)._all(_.price < 9999))
        if (!yes) {
            b->failNow()
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let yes = _fold(each(arr)._all(_.price < 9999))
        if (!yes) {
            b->failNow()
        }
    }
}

[benchmark]
def all_match_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def all_match_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def all_match_m3f_old(b : B?) {
    run_m3f_old(b, 100000)
}

[benchmark]
def all_match_m3f(b : B?) {
    run_m3f(b, 100000)
}
