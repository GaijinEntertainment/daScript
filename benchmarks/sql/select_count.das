options gen2
options persistent_heap

require _common public

// _select |> count — projection followed by counter. The final count value doesn't depend
// on the projection, but plain LINQ `count(select(src, f))` still evaluates `f` per element
// so user-visible side effects fire. Phase-2A `_fold` matches that: the counter lane binds
// the final projection to a discardable local per matched element (side effects preserved)
// and skips array materialization. The optimizer DCEs the binding for pure projections
// like `_.price * 2`, leaving a bare-loop counter for the common case.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let c = _sql(db |> select_from(type<Car>) |> count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let c = _fold(each(arr)._select(_.price * 2).count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let c = _fold(from_decs_template(type<DecsCar>)._select(_.price * 2).count())
        b |> accept(c)
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
def select_count_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def select_count_m4(b : B?) {
    run_m4(b, 100000)
}
