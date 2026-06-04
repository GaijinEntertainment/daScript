options gen2
options persistent_heap

require _common public
require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// `_distinct_by(_.dealer_id) |> _order_by(_.price) |> to_array` — pick first car per
// dealer (by source/PK order), sort by price. NO take suffix (compare against
// distinct_by_order_take, which adds `take(N)`). n=100k, DEALER_COUNT=100 → 100 rows.
// SQL (m1) — bare-aggregate trick + outer ORDER BY via Arm A passthrough (PR #2906's
// chunk N+1): `SELECT … FROM (SELECT *, MIN(pk) FROM Cars GROUP BY dealer_id) ORDER BY price ASC`.
// Array/Decs (m3f/m4) splice through emit_fused_prefilter (linq_fold.das:2351) — single
// fused loop with per-element dset gate then `order_inplace(buf, ...)` tail. Theme 8 3b.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let rows <- _fold(db |> select_from(type<Car>)
                                |> _distinct_by(_.dealer_id)
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
        unsafe {
            let rows <- _fold(each(arr)._distinct_by(_.dealer_id)._order_by(_.price).to_array())
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
            let rows <- _fold(from_decs_template(type<DecsCar>)._distinct_by(_.dealer_id)._order_by(_.price).to_array())
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
                let rows <- _fold(from_xml_node(doc.document_element, type<Car>)._distinct_by(_.dealer_id)._order_by(_.price).to_array())
                b |> accept(rows)
                if (empty(rows)) {
                    b->failNow()
                }
            }
        }
    }
}

def run_m6f(b : B?; n : int) {
    let jv = fixture_json(n)
    b |> run("m6f_json_fold/{n}", n) {
        unsafe {
            let rows <- _fold(from_json(jv, type<Car>)._distinct_by(_.dealer_id)._order_by(_.price).to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def distinct_by_order_to_array_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def distinct_by_order_to_array_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def distinct_by_order_to_array_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def distinct_by_order_to_array_m5f(b : B?) {
    run_m5f(b, 100000)
}

[benchmark]
def distinct_by_order_to_array_m6f(b : B?) {
    run_m6f(b, 100000)
}
