options gen2
options persistent_heap

require _common public

// _group_by(_.brand) |> _having(_._1 |> select(price) |> sum > 50000) |> _select((Brand=key, N=length)).
// SQL: SELECT brand, COUNT(*) FROM cars GROUP BY brand HAVING SUM(price) > 50000.
// Splice mode (PR-E) recognizes that the having predicate's inner-select-sum on price has
// NO matching select-side slot — the planner synthesizes a hidden accumulator slot on the
// internal table value type; the per-element loop updates it alongside `length`; result-build
// projects only the user-visible (Brand, N) and applies the having filter against the hidden
// slot via `kv._{hidden} > 50000`. Without PR-E this chain cascaded to tier 2 (full bucket-
// array materialization).

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let groups <- _sql(db |> select_from(type<Car>)
                                  |> _group_by(_.brand)
                                  |> _having(_._1 |> select($(c : Car) => c.price) |> sum > 50000)
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
        let groups <- (arr._group_by(_.brand)._having(_._1 |> select($(c : Car) => c.price) |> sum > 50000)._select((Brand = _._0, N = _._1 |> length)))
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
                            ._having(_._1 |> select($(c : Car) => c.price) |> sum > 50000)
                            ._select((Brand = _._0, N = _._1 |> length))
                            .to_array())
        if (empty(groups)) {
            b->failNow()
        }
    }
}

[benchmark]
def groupby_having_hidden_sum_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def groupby_having_hidden_sum_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def groupby_having_hidden_sum_m3f(b : B?) {
    run_m3f(b, 100000)
}
