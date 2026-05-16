options gen2
options persistent_heap

require _common public

// _select |> count — projection followed by counter. The projection has no effect on count
// semantics, but on the array path m3 materializes the projected array before counting.
// Phase-2A `_fold` recognizes the counter lane and emits a bare-loop counter that ignores
// the projection entirely (no allocation). `_old_fold` lacks a [select, count] pattern in
// g_foldSeq so it falls to the default nested-pass form (pass_0 = select(...); count(pass_0))
// — materializing the same way m3 does.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let c = _sql(db |> select_from(type<Car>) |> count())
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let c = arr |> _select(_.price * 2) |> count()
        if (c == 0) {
            b->failNow()
        }
    }
}

def run_m3f_old(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_old_array_fold/{n}", n) {
        let c = _old_fold(each(arr)._select(_.price * 2).count())
        if (c == 0) {
            b->failNow()
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let c = _fold(each(arr)._select(_.price * 2).count())
        if (c == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def select_count_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def select_count_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def select_count_m3f_old(b : B?) {
    run_m3f_old(b, 100000)
}

[benchmark]
def select_count_m3f(b : B?) {
    run_m3f(b, 100000)
}
