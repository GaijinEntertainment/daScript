options gen2
options persistent_heap

require _common public

let SKIP_N = 1000
let TAKE_N = 100

// skip(M) |> take(K) |> to_array — windowing.
// SQL: LIMIT K OFFSET M.
// Same SQL-aggregate restriction as take_count: aggregate after take/skip is rejected,
// so we measure the materialized window and check length.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let rows <- _sql(db |> select_from(type<Car>) |> skip(SKIP_N) |> take(TAKE_N))
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let rows <- _fold(each(arr).skip(SKIP_N).take(TAKE_N).to_array())
        b |> accept(rows)
        if (empty(rows)) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let rows <- _fold(from_decs_template(type<DecsCar>).skip(SKIP_N).take(TAKE_N).to_array())
        b |> accept(rows)
        if (empty(rows)) {
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
            let rows <- unsafe(from_xml_node(doc.document_element, type<Car>)) |> skip(SKIP_N) |> take(TAKE_N) |> to_array()
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
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
            let rows <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>)).skip(SKIP_N).take(TAKE_N).to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def skip_take_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def skip_take_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def skip_take_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def skip_take_m5(b : B?) {
    run_m5(b, 100000)
}

[benchmark]
def skip_take_m5f(b : B?) {
    run_m5f(b, 100000)
}
