options gen2
options persistent_heap

require _common public

// _join(cars, dealers) |> _group_by(_.Brand) |> _select((Brand, Total=sum price)) |> to_array.
// SQL (m1): `SELECT ("t0"."brand"), SUM("t0"."price") FROM "Cars" AS "t0" INNER JOIN
// "Dealers" AS "t1" ON "t0"."dealer_id" = "t1"."id" GROUP BY ("t0"."brand")`.
// push_group_key resolves the `_.Brand` key, and try_translate_group_aggregate resolves
// the inner `_select(_.Price)` to the qualified column — both consult the join's `into`
// projection snapshot. Chunk N+2 (sqlite_linq.das).
// Decs (m4): plan_decs_group_by's `isDecsJoin` adapter mode (Theme 3 Phase 1, audit C3).
// Same shape as join_groupby_count.das but with a reducing aggregate. Single pass,
// no intermediate allocations.
// Array (m3f): no array-side join splice; cascade baseline for comparison.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let groups <- _fold(db |> select_from(type<Car>)
                |> _join(db |> select_from(type<Dealer>),
                         $(c : Car, d : Dealer) => c.dealer_id == d.id,
                         $(c : Car, d : Dealer) => (Brand = c.brand, Price = c.price))
                |> _group_by(_.Brand)
                |> _select((Brand = _._0,
                            Total = _._1 |> _select(_.Price) |> sum())))
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

def run_m5f(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    let dealers <- fixture_dealers_array()
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        b |> run("m5f_xml_fold/{n}", n) {
            let groups <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>)) |> _join(dealers,
                                          $(c : Car, d : Dealer) => c.dealer_id == d.id,
                                          $(c : Car, _d : Dealer) => (Brand = c.brand, Price = c.price))
                                 |> _group_by(_.Brand)
                                 |> _select((Brand = _._0,
                                             Total = _._1 |> select($(t : tuple<Brand : int; Price : int>) => t.Price) |> sum())))
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def join_groupby_to_array_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def join_groupby_to_array_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def join_groupby_to_array_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def join_groupby_to_array_m5f(b : B?) {
    run_m5f(b, 100000)
}
