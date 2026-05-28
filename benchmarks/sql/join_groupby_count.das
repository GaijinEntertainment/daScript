options gen2
options persistent_heap

require _common public

// _join(cars, dealers) |> _group_by(_.Brand) |> _select((Brand, N=_._1|>count)) — joined+grouped,
// per-group counts returned as an array of (Brand, N) tuples (no terminal count;
// the inner `count()` is the per-bucket reducer).
// SQL (m1): `SELECT ("t0"."brand"), COUNT(*) FROM "Cars" AS "t0" INNER JOIN "Dealers" AS "t1"
// ON "t0"."dealer_id" = "t1"."id" GROUP BY ("t0"."brand")`. push_group_key resolves the
// `_.Brand` key against the join's `into` projection registry — emits the qualified source
// column (`"t0"."brand"`) rather than the bare alias name. Chunk N+2 (sqlite_linq.das).
// Decs (m4): plan_decs_group_by's `isDecsJoin` adapter mode (Theme 3 Phase 1, audit C3).
// Single pass — hashB-collect + srcA-probe emits per-pair result-lam bind directly
// into plan_group_by_core's bucket update. Vs the tier-2 cascade's 3 intermediate
// allocations (dealers-array → join-array → group-map → output-array). plan_group_by_core
// itself is untouched — the abstraction holds.
// Array (m3f): no array-side join splice; cascade baseline for comparison.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let groups <- _sql(db |> select_from(type<Car>)
                |> _join(db |> select_from(type<Dealer>),
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
}

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
def join_groupby_count_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def join_groupby_count_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def join_groupby_count_m4(b : B?) {
    run_m4(b, 100000)
}
