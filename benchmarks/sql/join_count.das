options gen2
options persistent_heap

require _common public

// _join(cars, dealers, on=c.dealer_id == d.id, into=...) |> count.
// SQL lane works via `_sql(... |> _join(db |> select_from(type<Dealer>), ...))`
// — the projection must be a named-tuple (positional tuples like `(c.name, d.name)`
// are rejected by `_sql`'s _select shape check; use `(CarName=..., DealerName=...)`).
// Decs lane: plan_decs_join (linq_fold.das) builds a hash from the smaller side
// (dealers) inside one `for_each_archetype` pass, then walks the larger side
// (cars) and probes — same algorithm as `join_impl` in linq.das (line 1674).
// Auto-typed result lambda (`$(l,r)=>...`) skips the verbose tuple type spelling
// over decs sources.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let c = _fold(db |> select_from(type<Car>) |> _join(db |> select_from(type<Dealer>),
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

def run_m4(b : B?; n : int) {
    fixture_decs_cars_and_dealers(n)
    b |> run("m4_decs_fold/{n}", n) {
        let c = _fold(from_decs_template(type<DecsCar>) |> _join(from_decs_template(type<DecsDealer>),
                                                                  $(l, r) => l.dealer_id == r.id,
                                                                  $(l, r) => (CarName = l.name, DealerName = r.name))
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
            let c = _fold(unsafe(from_xml_node(doc.document_element, type<Car>)) |> _join(dealers,
                                                                          $(c : Car, d : Dealer) => c.dealer_id == d.id,
                                                                          $(c : Car, d : Dealer) => (c.name, d.name))
                                                                 |> count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

def run_m6f(b : B?; n : int) {
    let jv = fixture_json(n)
    let dealers <- fixture_dealers_array()
    b |> run("m6f_json_fold/{n}", n) {
        let c = _fold(unsafe(from_json(jv, type<Car>)) |> _join(dealers,
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

[benchmark]
def join_count_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def join_count_m5f(b : B?) {
    run_m5f(b, 100000)
}

[benchmark]
def join_count_m6f(b : B?) {
    run_m6f(b, 100000)
}
