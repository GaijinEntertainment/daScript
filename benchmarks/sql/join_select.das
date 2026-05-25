options gen2
options persistent_heap

require _common public

// _join(cars, dealers, ...) |> _select(_.CarName) |> to_array — join then project (Theme 1, audit 8b).
// SQL: `_sql`'s `_select` after `_join` rejects bare `_` — only the `into` lambda's
// parameter names are valid receivers, so `... |> _join(...) |> _select(_.CarName)`
// doesn't lower. By design — sqlite_linq surface limitation; comparable SQL shape
// would inline the projection into the `into` lambda. Skip m1.
// Decs (m4): plan_decs_join terminal-_select extension — single bind of the join result
// per matched pair, then projection. Replaces the would-be cascade `join_impl + select`.
// Array (m3f): no array-side join splice exists; this lane measures the unspliced
// `join_impl` cascade for comparison.

def run_m3f(b : B?; n : int) {
    let cars <- fixture_array(n)
    let dealers <- fixture_dealers_array()
    b |> run("m3f_array_fold/{n}", n) {
        let rows <- _fold(cars |> _join(dealers,
                                        $(c : Car, d : Dealer) => c.dealer_id == d.id,
                                        $(c : Car, d : Dealer) => (CarName = c.name, DealerId = d.id))
                               |> _select(_.CarName))
        b |> accept(rows)
        if (empty(rows)) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs_cars_and_dealers(n)
    b |> run("m4_decs_fold/{n}", n) {
        unsafe {
            let rows <- _fold(from_decs_template(type<DecsCar>) |> _join(from_decs_template(type<DecsDealer>),
                                                                          $(l, r) => l.dealer_id == r.id,
                                                                          $(l, r) => (CarName = l.name, DealerId = r.id))
                                                                 |> _select(_.CarName)
                                                                 |> to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def join_select_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def join_select_m4(b : B?) {
    run_m4(b, 100000)
}
