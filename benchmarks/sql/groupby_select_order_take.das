options gen2
options persistent_heap

require _common public

// _group_by(_.brand) |> _select((Brand, Total)) |> _order_by(_.Total) |> to_array — GROUP BY ... ORDER BY.
// SQL: SELECT brand, SUM(price) AS Total FROM Cars GROUP BY brand ORDER BY Total.
// Array/Decs splice through plan_group_by_core's trailing-_order_by extension
// (Theme 3 Phase 2, audit C2): inline-cmp sort(buf, ...) tail mutates the same
// bucket-fill buffer in place. Vs the tier-2 cascade's three allocations
// (group_by_lazy_to_array → select → fresh-array sort).

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let groups <- _sql(db |> select_from(type<Car>)
                                  |> _group_by(_.brand)
                                  |> _select((Brand = _._0,
                                              Total = _._1 |> select($(c : Car) => c.price) |> sum()))
                                  |> _order_by(_.Total))
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let groups <- _fold(each(arr)
                            ._group_by(_.brand)
                            ._select((Brand = _._0,
                                      Total = _._1 |> select($(c : Car) => c.price) |> sum()))
                            ._order_by(_.Total)
                            .to_array())
        b |> accept(groups)
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
                                      Total = _._1 |> _select(_.price) |> sum()))
                            ._order_by(_.Total)
                            .to_array())
        b |> accept(groups)
        if (empty(groups)) {
            b->failNow()
        }
    }
}

[benchmark]
def groupby_select_order_take_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def groupby_select_order_take_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def groupby_select_order_take_m4(b : B?) {
    run_m4(b, 100000)
}
