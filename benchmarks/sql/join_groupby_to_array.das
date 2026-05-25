options gen2
options persistent_heap

require _common public

// _join(cars, dealers) |> _group_by(_.Brand) |> _select((Brand, Total=sum price)) |> to_array.
// SQL: SELECT brand, SUM(price) FROM Cars c JOIN Dealers d ON c.dealer_id = d.id GROUP BY brand.
// sqlite_linq's `_group_by` after `_join` doesn't currently lower (key column is from
// the joined projection). Follow-up TODO 2026-05-25.
// Decs (m4): plan_decs_group_by's `isDecsJoin` adapter mode (Theme 3 Phase 1, audit C3).
// Same shape as join_groupby_count.das but with a reducing aggregate. Single pass,
// no intermediate allocations.
// Array (m3f): no array-side join splice; cascade baseline for comparison.

def run_m3f(b : B?; n : int) {
    let cars <- fixture_array(n)
    let dealers <- fixture_dealers_array()
    b |> run("m3f_array_fold/{n}", n) {
        let groups <- _fold(cars |> _join(dealers,
                                          $(c : Car, d : Dealer) => c.dealer_id == d.id,
                                          $(c : Car, d : Dealer) => (Brand = c.brand, Price = c.price))
                                 |> _group_by(_.Brand)
                                 |> _select((Brand = _._0,
                                             Total = _._1 |> select($(t : tuple<Brand : int; Price : int>) => t.Price) |> sum())))
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
                         $(l, r) => (Brand = l.brand, Price = l.price))
                |> _group_by(_.Brand)
                |> _select((Brand = _._0,
                            Total = _._1 |> _select(_.Price) |> sum()))
                |> to_array())
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def join_groupby_to_array_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def join_groupby_to_array_m4(b : B?) {
    run_m4(b, 100000)
}
