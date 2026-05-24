options gen2
options persistent_heap

require _common public

// _join(cars, dealers, on=c.dealer_id == d.id, into=...) |> count.
// SQL lane works via `_sql(... |> _join(db |> select_from(type<Dealer>), ...))`
// — the projection must be a named-tuple (positional tuples like `(c.name, d.name)`
// are rejected by `_sql`'s _select shape check; use `(CarName=..., DealerName=...)`).
// The Decs lane is absent — `_join` doesn't lower onto the archetype walk, and
// faithfully porting cross-archetype lookup would need a Dealer decs template +
// hash-by-id index. [Follow-up TODO 2026-05-23: decs join / cross-archetype lookup
// machinery — see plan_join in linq_fold.das, currently only handles iterator/array.]

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let c = _sql(db |> select_from(type<Car>) |> _join(db |> select_from(type<Dealer>),
                                                               $(c : Car, d : Dealer) => c.dealer_id == d.id,
                                                               $(c : Car, d : Dealer) => (CarName = c.name, DealerName = d.name))
                                                      |> count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

def run_m3f(b : B?; n : int) {
    let cars <- fixture_array(n)
    let dealers <- fixture_dealers_array()
    b |> run("m3f_array_fold/{n}", n) {
        let c = _fold(cars |> _join(dealers,
                                    $(c : Car, d : Dealer) => c.dealer_id == d.id,
                                    $(c : Car, d : Dealer) => (c.name, d.name))
                           |> count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def join_count_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def join_count_m3f(b : B?) {
    run_m3f(b, 100000)
}
