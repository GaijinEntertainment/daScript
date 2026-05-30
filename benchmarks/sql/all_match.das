options gen2
options persistent_heap

require _common public

// Predicate that is ALWAYS true (price < 9999), so the full scan runs.
// SQL equivalent of `all(pred)` is `count(where NOT pred) == 0` — no direct
// `_all` terminal in sqlite_linq, so we phrase it as a counter-example count.
// Array and Decs lanes use `_all(pred)` which short-circuits on first failing
// element; always-true predicate forces full traversal (fair upper-bound timing).

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let bad = _sql(db |> select_from(type<Car>) |> _where(_.price >= 9999) |> count())
            b |> accept(bad)
            if (bad != 0) {
                b->failNow()
            }
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let yes = _fold(each(arr)._all(_.price < 9999))
        b |> accept(yes)
        if (!yes) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let yes = _fold(from_decs_template(type<DecsCar>)._all(_.price < 9999))
        b |> accept(yes)
        if (!yes) {
            b->failNow()
        }
    }
}

def run_m5(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        b |> run("m5_xml/{n}", n) {
            let yes = unsafe(from_xml_node(doc.document_element, type<Car>)) |> _all(_.price < 9999)
            b |> accept(yes)
            if (!yes) {
                b->failNow()
            }
        }
    }
}

def run_m5f(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        b |> run("m5f_xml_fold/{n}", n) {
            let yes = _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._all(_.price < 9999))
            b |> accept(yes)
            if (!yes) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def all_match_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def all_match_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def all_match_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def all_match_m5(b : B?) {
    run_m5(b, 100000)
}

[benchmark]
def all_match_m5f(b : B?) {
    run_m5f(b, 100000)
}
