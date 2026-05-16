options gen2
options persistent_heap

require _common public

// _group_by(_.brand) |> _select((Brand=key, TotalPrice=sum of price per group)).
// SQL: SELECT brand, SUM(price) GROUP BY brand.
// Array variants buffer per-group; splice mode can inline the inner select+sum
// per group as a fused accumulator loop.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let groups <- _sql(db |> select_from(type<Car>)
                                  |> _group_by(_.brand)
                                  |> _select((Brand = _._0,
                                              TotalPrice = _._1 |> select($(c : Car) => c.price) |> sum())))
            if (empty(groups)) {
                b->failNow()
            }
        }
    }
}

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let groups <- (arr._group_by(_.brand)._select((Brand = _._0,
                                                       TotalPrice = _._1 |> select($(c : Car) => c.price) |> sum())))
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
                                ._select((Brand = _._0,
                                          TotalPrice = _._1 |> select($(c : Car) => c.price) |> sum()))
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
                            ._select((Brand = _._0,
                                      TotalPrice = _._1 |> select($(c : Car) => c.price) |> sum()))
                            .to_array())
        if (empty(groups)) {
            b->failNow()
        }
    }
}

[benchmark]
def groupby_sum_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def groupby_sum_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def groupby_sum_m3f_old(b : B?) {
    run_m3f_old(b, 100000)
}

[benchmark]
def groupby_sum_m3f(b : B?) {
    run_m3f(b, 100000)
}
