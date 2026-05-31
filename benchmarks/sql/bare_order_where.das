options gen2
options persistent_heap

require _common public

let THRESHOLD = 500

// _where(_.price > T) |> _order_by(_.price) — fused prefilter + sort, NO take.
// SQL: WHERE price > T ORDER BY price.
// Array (spliced via plan_order_family Phase 3c) emits a single fused loop that
// collects matching elements into one pre-allocated buffer, then sort_inplace's
// the buffer. Decs runs the same fused shape over the archetype walk.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let rows <- _sql(db |> select_from(type<Car>)
                                |> _where(_.price > THRESHOLD)
                                |> _order_by(_.price))
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
        let rows <- _fold(each(arr)._where(_.price > THRESHOLD)
                                   ._order_by(_.price)
                                   .to_array())
        b |> accept(rows)
        if (empty(rows)) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let rows <- _fold(from_decs_template(type<DecsCar>)._where(_.price > THRESHOLD)
                                   ._order_by(_.price)
                                   .to_array())
        b |> accept(rows)
        if (empty(rows)) {
            b->failNow()
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
            let rows <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.price > THRESHOLD)
                                       ._order_by(_.price)
                                       .to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def bare_order_where_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def bare_order_where_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def bare_order_where_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def bare_order_where_m5f(b : B?) {
    run_m5f(b, 100000)
}
