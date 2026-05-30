options gen2
options persistent_heap

require _common public

// _order_by(_.price) |> first — sort then take first (minimum by sort key).
// SQL: ORDER BY price LIMIT 1 — engine can use an index or partial sort.
// Array variants without specific min-detection must fully sort then index [0].

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let row = _sql(db |> select_from(type<Car>) |> _order_by(_.price) |> _first())
            b |> accept(row)
            if (row.id == 0) {
                b->failNow()
            }
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let row = _fold(each(arr)._order_by(_.price).first())
        b |> accept(row)
        if (row.id == 0) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let row = _fold(from_decs_template(type<DecsCar>)._order_by(_.price).first())
        b |> accept(row)
        if (row.id == 0) {
            b->failNow()
        }
    }
}

// m5_xml lane: non-fold baseline — the same chain over from_xml_node, un-fused (tier-2 cascade).
def run_m5(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        b |> run("m5_xml/{n}", n) {
            let row = unsafe(from_xml_node(doc.document_element, type<Car>)) |> _order_by(_.price) |> first()
            b |> accept(row)
            if (row.id == 0) {
                b->failNow()
            }
        }
    }
}

// m5f_xml_fold lane: fused _fold over from_xml_node.
def run_m5f(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        b |> run("m5f_xml_fold/{n}", n) {
            let row = _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._order_by(_.price).first())
            b |> accept(row)
            if (row.id == 0) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def sort_first_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def sort_first_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def sort_first_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def sort_first_m5(b : B?) {
    run_m5(b, 100000)
}

[benchmark]
def sort_first_m5f(b : B?) {
    run_m5f(b, 100000)
}
