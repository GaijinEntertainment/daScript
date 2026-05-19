options gen2
options persistent_heap

require _common public

// where(price > 500) |> _group_by(_.brand) |> _select((Brand=key, N=length)).
// SQL: SELECT brand, COUNT(*) WHERE price > 500 GROUP BY brand.
// Splice mode (PR-B) fuses the upstream where_ into the per-element loop: the
// table update fires only for source elements that pass the filter.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let groups <- _sql(db |> select_from(type<Car>)
                                  |> _where(_.price > 500)
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
        let groups <- (arr._where(_.price > 500)._group_by(_.brand)._select((Brand = _._0, N = _._1 |> length)))
        if (empty(groups)) {
            b->failNow()
        }
    }
}
def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let groups <- _fold(each(arr)
                            ._where(_.price > 500)
                            ._group_by(_.brand)
                            ._select((Brand = _._0, N = _._1 |> length))
                            .to_array())
        if (empty(groups)) {
            b->failNow()
        }
    }
}

[benchmark]
def groupby_where_count_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def groupby_where_count_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def groupby_where_count_m3f(b : B?) {
    run_m3f(b, 100000)
}
