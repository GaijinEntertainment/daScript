options gen2
options persistent_heap

require _common public

// Bare `.last()` — no predicate, no order. The last element in source/storage/document order.
//
// No m1 (SQL) lane: SQL has no inherent row ordering, so `_sql` rejects bare `reverse()` / `last()`
// (sqlite_linq.das — "use _order_by_descending(...) for sort reversal"). Bare last is only meaningful
// for sources with a natural order: array (index), decs (archetype storage order), XML (document order),
// JSON (array order).
//
// Decs (m4): reads the last archetype's [size-1] directly via get_ro index (emit_decs_last_random_index),
// O(num_archetypes) — was a full O(N) walk before. XML (m5f): backward DOM step (PR #3009). JSON (m6f):
// random-indexes the backing array. Array (m3f): arr[len-1].

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let row = _fold(each(arr).last())
        b |> accept(row)
        if (row.price == 0) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let row = _fold(from_decs_template(type<DecsCar>).last())
        b |> accept(row)
        if (row.price == 0) {
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
            let row = _fold(unsafe(from_xml_node(doc.document_element, type<Car>)).last())
            b |> accept(row)
            if (row.price == 0) {
                b->failNow()
            }
        }
    }
}

def run_m6f(b : B?; n : int) {
    let jv = fixture_json(n)
    b |> run("m6f_json_fold/{n}", n) {
        let row = _fold(unsafe(from_json(jv, type<Car>)).last())
        b |> accept(row)
        if (row.price == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def bare_last_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def bare_last_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def bare_last_m5f(b : B?) {
    run_m5f(b, 100000)
}

[benchmark]
def bare_last_m6f(b : B?) {
    run_m6f(b, 100000)
}
