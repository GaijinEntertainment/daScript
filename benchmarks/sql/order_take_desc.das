options gen2
options persistent_heap

require _common public

let TAKE_N = 10

// _order_by_descending(_.price) |> take(N) — top-N largest pattern.
// SQL: ORDER BY price DESC LIMIT N — engine emits partial-sort optimization when possible.
// Array (spliced via plan_order_family Phase 3b) dispatches directly to
// ``top_n_by_descending`` which uses partial_sort with a flipped comparator — O(M log N).
// Decs runs the same shape over the archetype walk with a bounded heap of size N.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let rows <- _sql(db |> select_from(type<Car>) |> _order_by_descending(_.price) |> take(TAKE_N))
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
        let rows <- _fold(each(arr)._order_by_descending(_.price).take(TAKE_N).to_array())
        b |> accept(rows)
        if (empty(rows)) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let rows <- _fold(from_decs_template(type<DecsCar>)._order_by_descending(_.price).take(TAKE_N).to_array())
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
            let rows <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._order_by_descending(_.price).take(TAKE_N).to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def order_take_desc_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def order_take_desc_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def order_take_desc_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def order_take_desc_m5f(b : B?) {
    run_m5f(b, 100000)
}
