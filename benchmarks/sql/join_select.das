options gen2
options persistent_heap

require _common public

// _join(cars, dealers, ...) |> _select(_.CarName) |> to_array — join then project (Theme 1, audit 8b).
// SQL (m1): a standalone `_select` after `_join` rejects bare `_` (only the `into` lambda's parameter
// names are valid receivers), so the comparable SQL inlines the projection into the `into` lambda —
// `_join(..., $(c,d) => (CarName = c.name))` — yielding the same result (an array of CarName).
// Decs (m4): plan_decs_join terminal-_select extension — single bind of the join result
// per matched pair, then projection. Replaces the would-be cascade `join_impl + select`.
// Array (m3f): no array-side join splice exists; this lane measures the unspliced
// `join_impl` cascade for comparison.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let rows <- _sql(db |> select_from(type<Car>) |> _join(db |> select_from(type<Dealer>),
                                                                   $(c : Car, d : Dealer) => c.dealer_id == d.id,
                                                                   $(c : Car, d : Dealer) => (CarName = c.name)))
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

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

// m5_xml lane: non-fold baseline — the XML iterator joined with the dealers array, un-fused.
def run_m5(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    let dealers <- fixture_dealers_array()
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        b |> run("m5_xml/{n}", n) {
            let rows <- (unsafe(from_xml_node(doc.document_element, type<Car>)) |> _join(dealers,
                                        $(c : Car, d : Dealer) => c.dealer_id == d.id,
                                        $(c : Car, d : Dealer) => (CarName = c.name, DealerId = d.id))
                               |> _select(_.CarName))
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

// m5f_xml_fold lane: fused _fold over the XML iterator joined with the dealers array.
def run_m5f(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    let dealers <- fixture_dealers_array()
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        b |> run("m5f_xml_fold/{n}", n) {
            let rows <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>)) |> _join(dealers,
                                        $(c : Car, d : Dealer) => c.dealer_id == d.id,
                                        $(c : Car, d : Dealer) => (CarName = c.name, DealerId = d.id))
                               |> _select(_.CarName))
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def join_select_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def join_select_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def join_select_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def join_select_m5(b : B?) {
    run_m5(b, 100000)
}

[benchmark]
def join_select_m5f(b : B?) {
    run_m5f(b, 100000)
}
