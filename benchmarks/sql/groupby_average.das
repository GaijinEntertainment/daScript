options gen2
options persistent_heap

require _common public

// _group_by(_.brand) |> _select((Brand=key, AvgPrice=avg of price per group)).
// SQL: SELECT brand, AVG(price) GROUP BY brand.
// Splice mode (PR-A2 follow-up) fuses the inner select+average per group via a
// 2-slot per-key acc (sum : double + count : uint64) and divides at result-build.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let groups <- _sql(db |> select_from(type<Car>)
                                  |> _group_by(_.brand)
                                  |> _select((Brand = _._0,
                                              AvgPrice = _._1 |> select($(c : Car) => c.price) |> average())))
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
                                                       AvgPrice = _._1 |> select($(c : Car) => c.price) |> average())))
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
                                      AvgPrice = _._1 |> select($(c : Car) => c.price) |> average()))
                            .to_array())
        if (empty(groups)) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let groups <- _fold(from_decs_template(type<DecsCar>)
                            ._group_by(_.brand)
                            ._select((Brand = _._0,
                                      AvgPrice = _._1 |> _select(_.price) |> average()))
                            .to_array())
        if (empty(groups)) {
            b->failNow()
        }
    }
}

[benchmark]
def groupby_average_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def groupby_average_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def groupby_average_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def groupby_average_m4(b : B?) {
    run_m4(b, 100000)
}
