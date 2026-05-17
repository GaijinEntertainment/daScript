options gen2
options persistent_heap

require _common public

let TARGET_ID = 50000

// contains compares each element (or projected value) against a fixed needle. The needle
// is bound once at the top of the invoke block so it's evaluated only once even if the
// argument is expensive. Early-exit on first match.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            // SQL doesn't have a direct CONTAINS for arbitrary values; use _any with _where.
            let opt = _sql(db |> select_from(type<Car>) |> _where(_.id == TARGET_ID) |> _first_opt())
            if (!is_some(opt)) {
                b->failNow()
            }
        }
    }
}

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        // Project ids out then contains. Mirrors what `_fold(...select.contains(...))` does
        // — an array-source linq chain materializes `select` first then iterates contains.
        let yes = arr |> _select(_.id) |> contains(TARGET_ID)
        if (!yes) {
            b->failNow()
        }
    }
}

def run_m3f_old(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_old_array_fold/{n}", n) {
        let yes = _old_fold(each(arr)._select(_.id).contains(TARGET_ID))
        if (!yes) {
            b->failNow()
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let yes = _fold(each(arr)._select(_.id).contains(TARGET_ID))
        if (!yes) {
            b->failNow()
        }
    }
}

[benchmark]
def contains_match_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def contains_match_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def contains_match_m3f_old(b : B?) {
    run_m3f_old(b, 100000)
}

[benchmark]
def contains_match_m3f(b : B?) {
    run_m3f(b, 100000)
}
