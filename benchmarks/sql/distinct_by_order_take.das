options gen2
options persistent_heap

require _common public
require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

let TAKE_N = 10

// `_distinct_by(_.dealer_id) |> _order_by(_.price) |> take(N) |> to_array` — pick first
// car per dealer (by source/PK order), sort by price, take top N.
// SQL (m1) — lowers via the bare-aggregate trick from PR #2906 + the Arm A passthrough
// composition: `SELECT … FROM (SELECT *, MIN(pk) FROM Cars GROUP BY dealer_id) AS t0
// ORDER BY price ASC LIMIT N`. See sqlite_linq.das::finalize_distinct_by_passthrough_wrap.
// Array/Decs (m3f/m4) splice through plan_order_family / plan_decs_order_family's
// bounded-heap + set-gate path (Theme 3 Phase 3).

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let rows <- _fold(db |> select_from(type<Car>)
                                |> _distinct_by(_.dealer_id)
                                |> _order_by(_.price)
                                |> take(TAKE_N))
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
        unsafe {
            let rows <- _fold(each(arr)._distinct_by(_.dealer_id)._order_by(_.price).take(TAKE_N).to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        unsafe {
            let rows <- _fold(from_decs_template(type<DecsCar>)._distinct_by(_.dealer_id)._order_by(_.price).take(TAKE_N).to_array())
            b |> accept(rows)
            if (empty(rows)) {
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
            unsafe {
                let rows <- _fold(from_xml_node(doc.document_element, type<Car>)._distinct_by(_.dealer_id)._order_by(_.price).take(TAKE_N).to_array())
                b |> accept(rows)
                if (empty(rows)) {
                    b->failNow()
                }
            }
        }
    }
}

[benchmark]
def distinct_by_order_take_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def distinct_by_order_take_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def distinct_by_order_take_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def distinct_by_order_take_m5f(b : B?) {
    run_m5f(b, 100000)
}
