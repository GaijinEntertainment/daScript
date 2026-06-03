options gen2
options persistent_heap

require _common public

let THRESHOLD = 500

// _where(_.price > T) |> _order_by_keys((_.brand, _.price), 0u) — fused prefilter + multi-key sort, NO take.
// Multi-key orderby lowers to ONE composite-comparator stable_sort (C# OrderBy/ThenBy parity on full ties),
// unlike single-key _order_by which keeps the cheaper unstable sort. This bench measures the cost of that
// stability guarantee on the full-sort-no-take path — the worst case for the stable algorithm. brand has
// BRAND_COUNT(=5) distinct values, so the first key produces dense ties the second key (price) resolves.
// SQL: WHERE price > T ORDER BY brand, price.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let rows <- _fold(db |> select_from(type<Car>)
                                |> _where(_.price > THRESHOLD)
                                |> _order_by_keys((_.brand, _.price), 0u))
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
                                   ._order_by_keys((_.brand, _.price), 0u)
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
                                   ._order_by_keys((_.brand, _.price), 0u)
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
                                       ._order_by_keys((_.brand, _.price), 0u)
                                       .to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def order_by_multi_key_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def order_by_multi_key_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def order_by_multi_key_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def order_by_multi_key_m5f(b : B?) {
    run_m5f(b, 100000)
}
