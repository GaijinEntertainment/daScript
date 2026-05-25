options gen2
options persistent_heap

require _common public

// _join(cars, dealers) |> _group_by(_.Brand) |> _select((Brand, N=_._1|>count)) — joined+grouped,
// per-group counts returned as an array of (Brand, N) tuples (no terminal count;
// the inner `count()` is the per-bucket reducer).
// SQL: SELECT brand, COUNT(*) FROM Cars c JOIN Dealers d ON c.dealer_id = d.id GROUP BY brand.
// sqlite_linq's `_group_by` after `_join` doesn't currently lower (key column is from
// the joined projection, not a base table column). Follow-up TODO 2026-05-25.
// Decs (m4): plan_decs_group_by's `isDecsJoin` adapter mode (Theme 3 Phase 1, audit C3).
// Single pass — hashB-collect + srcA-probe emits per-pair result-lam bind directly
// into plan_group_by_core's bucket update. Vs the tier-2 cascade's 3 intermediate
// allocations (dealers-array → join-array → group-map → output-array). plan_group_by_core
// itself is untouched — the abstraction holds.
// Array (m3f): no array-side join splice; cascade baseline for comparison.

def run_m3f(b : B?; n : int) {
    let cars <- fixture_array(n)
    let dealers <- fixture_dealers_array()
    b |> run("m3f_array_fold/{n}", n) {
        let groups <- _fold(cars |> _join(dealers,
                                          $(c : Car, d : Dealer) => c.dealer_id == d.id,
                                          $(c : Car, d : Dealer) => (Brand = c.brand, DealerId = d.id))
                                 |> _group_by(_.Brand)
                                 |> _select((Brand = _._0, N = _._1 |> count())))
        b |> accept(groups)
        if (empty(groups)) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs_cars_and_dealers(n)
    b |> run("m4_decs_fold/{n}", n) {
        unsafe {
            let groups <- _fold(from_decs_template(type<DecsCar>)
                |> _join(from_decs_template(type<DecsDealer>),
                         $(l, r) => l.dealer_id == r.id,
                         $(l, r) => (Brand = l.brand, DealerId = r.id))
                |> _group_by(_.Brand)
                |> _select((Brand = _._0, N = _._1 |> count()))
                |> to_array())
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def join_groupby_count_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def join_groupby_count_m4(b : B?) {
    run_m4(b, 100000)
}
