options gen2
options persistent_heap

require _common public

let THRESHOLD = 1000

// _select(_.price * 2) |> _where(_ > T) |> count — where-after-select pattern.
// SQL: SELECT COUNT(*) FROM Cars WHERE price * 2 > T.
// Array and Decs lanes (spliced via Phase 3d via replaceVariablePeeling) substitute
// the projection into the where predicate (peeling the typer-inserted ExprRef2Value
// wrapper) and emit a single fused counter loop — no intermediate arrays.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            // SQL form folds projection into the WHERE filter — the engine evaluates
            // ``price * 2 > T`` per row and counts matches.
            let c = _sql(db |> select_from(type<Car>) |> _where(_.price * 2 > THRESHOLD) |> count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let c = _fold(each(arr)._select(_.price * 2)._where(_ > THRESHOLD).count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let c = _fold(from_decs_template(type<DecsCar>)._select(_.price * 2)._where(_ > THRESHOLD).count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

// m5_xml lane: non-fold baseline — the same linq chain over from_xml_node, un-fused
// (tier-2 cascade, materializes a full Car per element + intermediate stages). This is
// the reference the fused XML lane (m5f) is measured against: m5f ≈ m5 means the adapter
// is falling back to materialization (no fusion); m5f << m5 means fusion is working.
def run_m5(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        b |> run("m5_xml/{n}", n) {
            let c = unsafe(from_xml_node(doc.document_element, type<Car>)) |> _select(_.price * 2) |> _where(_ > THRESHOLD) |> count()
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def select_where_count_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def select_where_count_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def select_where_count_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def select_where_count_m5(b : B?) {
    run_m5(b, 100000)
}

// m5f_xml_fold lane: fused `_fold` over from_xml_node (pass 2b). XmlAdapter emits an inlined
// child-element walk and field-prunes the materialization to the attributes the chain reads.
def run_m5f(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        b |> run("m5f_xml_fold/{n}", n) {
            let c = _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._select(_.price * 2)._where(_ > THRESHOLD).count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def select_where_count_m5f(b : B?) {
    run_m5f(b, 100000)
}
