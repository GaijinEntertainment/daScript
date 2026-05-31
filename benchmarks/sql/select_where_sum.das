options gen2
options persistent_heap

require _common public

let THRESHOLD = 1000

// _select(_.price * 2) |> _where(_ > T) |> sum — where-after-select pattern with the
// sum terminator. SQL: SELECT SUM(price * 2) FROM Cars WHERE price * 2 > T.
//
// Array and Decs lanes (spliced via Phase 3d single-eval) bind the projection to a
// local once per element OUTSIDE the if-wrap, then the rewritten predicate reads the
// bind AND the sum's valueExpr reads the bind — projection (`_.price * 2`) evaluates
// exactly once per element across the splice. Without the dedup it evaluates twice
// (once in the inlined predicate, once in valueExpr) for ARRAY/ACCUMULATOR/EARLY_EXIT
// lanes.
//
// SQL uses ``query_scalar`` with raw SQL because ``_sql``'s ``_select`` clause only
// accepts a single column or named-tuple shape — arbitrary scalar expressions like
// ``_.price * 2`` aren't representable in the typed DSL. The engine still folds the
// projection into both the WHERE filter and the SUM, so per-row work is identical to
// what the typed form would emit.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let s = db |> query_scalar("SELECT SUM(price * 2) FROM Cars WHERE price * 2 > {THRESHOLD}", type<int>)
            b |> accept(s)
            if (s == 0) {
                b->failNow()
            }
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let s = _fold(each(arr)._select(_.price * 2)._where(_ > THRESHOLD).sum())
        b |> accept(s)
        if (s == 0) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let s = _fold(from_decs_template(type<DecsCar>)._select(_.price * 2)._where(_ > THRESHOLD).sum())
        b |> accept(s)
        if (s == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def select_where_sum_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def select_where_sum_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def select_where_sum_m4(b : B?) {
    run_m4(b, 100000)
}

def run_m5f(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        b |> run("m5f_xml_fold/{n}", n) {
            let s = _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._select(_.price * 2)._where(_ > THRESHOLD).sum())
            b |> accept(s)
            if (s == 0) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def select_where_sum_m5f(b : B?) {
    run_m5f(b, 100000)
}
