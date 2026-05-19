options gen2
options persistent_heap

require _common public

// _group_by(_.brand) |> _select 4-slot named tuple — fuses count + sum + max into one pass.
// SQL: SELECT brand, COUNT(*), SUM(price), MAX(price) GROUP BY brand.
// Splice (PR-A2 A.4) walks the ReducerSpec array once per source element, emitting per-slot
// miss-init + hit-update for all 3 reducers — no per-reducer separate passes.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let groups <- _sql(db |> select_from(type<Car>)
                                  |> _group_by(_.brand)
                                  |> _select((Brand = _._0,
                                              N = _._1 |> length,
                                              TotalPrice = _._1 |> select($(c : Car) => c.price) |> sum(),
                                              MaxPrice   = _._1 |> select($(c : Car) => c.price) |> max())))
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
                                                       N = _._1 |> length,
                                                       TotalPrice = _._1 |> select($(c : Car) => c.price) |> sum(),
                                                       MaxPrice   = _._1 |> select($(c : Car) => c.price) |> max())))
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
                                      N = _._1 |> length,
                                      TotalPrice = _._1 |> select($(c : Car) => c.price) |> sum(),
                                      MaxPrice   = _._1 |> select($(c : Car) => c.price) |> max()))
                            .to_array())
        if (empty(groups)) {
            b->failNow()
        }
    }
}

[benchmark]
def groupby_multi_reducer_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def groupby_multi_reducer_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def groupby_multi_reducer_m3f(b : B?) {
    run_m3f(b, 100000)
}
