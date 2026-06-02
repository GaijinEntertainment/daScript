options gen2
options persistent_heap

require _common public

// _where(srcA-pred) |> _join(cars, dealers, ...) |> count — pre-join filter then joined count.
// SQL: SELECT COUNT(*) FROM Cars c JOIN Dealers d ON c.dealer_id = d.id WHERE c.price > T (filter
// pushed to the left table). In-memory (m3f/m4/m5f): the leading `_where` fuses into the join probe
// loop as `if (pred(a)) { <probe> }` — no intermediate filtered-srcA array (join_general's lead_where
// slot; see doc/source/reference/linq_fold_patterns.rst). Complements join_where_count (trailing where).

let THRESHOLD = 500

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let c = _fold(db |> select_from(type<Car>) |> _where(_.price > THRESHOLD)
                                                       |> _join(db |> select_from(type<Dealer>),
                                                                $(c : Car, d : Dealer) => c.dealer_id == d.id,
                                                                $(c : Car, d : Dealer) => (CarPrice = c.price, DealerId = d.id))
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
        let c = _fold(cars |> _where(_.price > THRESHOLD)
                           |> _join(dealers,
                                    $(c : Car, d : Dealer) => c.dealer_id == d.id,
                                    $(c : Car, d : Dealer) => (CarPrice = c.price, DealerId = d.id))
                           |> count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs_cars_and_dealers(n)
    b |> run("m4_decs_fold/{n}", n) {
        let c = _fold(from_decs_template(type<DecsCar>) |> _where(_.price > THRESHOLD)
                                                        |> _join(from_decs_template(type<DecsDealer>),
                                                                 $(l, r) => l.dealer_id == r.id,
                                                                 $(l, r) => (CarPrice = l.price, DealerId = r.id))
                                                        |> count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
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
            let c = _fold(unsafe(from_xml_node(doc.document_element, type<Car>)) |> _where(_.price > THRESHOLD)
                           |> _join(dealers,
                                    $(c : Car, d : Dealer) => c.dealer_id == d.id,
                                    $(c : Car, d : Dealer) => (CarPrice = c.price, DealerId = d.id))
                           |> count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def where_join_count_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def where_join_count_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def where_join_count_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def where_join_count_m5f(b : B?) {
    run_m5f(b, 100000)
}
