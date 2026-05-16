options gen2
options persistent_heap

require _common public

// _group_by(_.brand) |> _select((Brand=key, N=count)) — per-group count.
// SQL: SELECT brand, COUNT(*) GROUP BY brand — engine hashes once, returns BRAND_COUNT rows.
// Array variants must buffer (group_by is fundamentally buffering); splice mode
// can still inline the projection and `length` aggregation per group.
// Expected result: BRAND_COUNT (5) groups, each ~n/5 in size.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let groups <- _sql(db |> select_from(type<Car>)
                                  |> _group_by(_.brand)
                                  |> _select((Brand = _._0, N = _._1 |> length)))
            if (empty(groups)) {
                b->failNow()
            }
        }
    }
}

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let groups <- (arr._group_by(_.brand)._select((Brand = _._0, N = _._1 |> length)))
        if (empty(groups)) {
            b->failNow()
        }
    }
}

def run_m3f_old(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_old_array_fold/{n}", n) {
        let groups <- _old_fold(each(arr)
                                ._group_by(_.brand)
                                ._select((Brand = _._0, N = _._1 |> length))
                                .to_array())
        if (empty(groups)) {
            b->failNow()
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let groups <- _fold(each(arr)
                            ._group_by(_.brand)
                            ._select((Brand = _._0, N = _._1 |> length))
                            .to_array())
        if (empty(groups)) {
            b->failNow()
        }
    }
}

[benchmark]
def groupby_count_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def groupby_count_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def groupby_count_m3f_old(b : B?) {
    run_m3f_old(b, 100000)
}

[benchmark]
def groupby_count_m3f(b : B?) {
    run_m3f(b, 100000)
}
