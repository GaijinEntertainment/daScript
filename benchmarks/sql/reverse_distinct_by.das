options gen2
options persistent_heap

require _common public
require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// `each(arr).reverse()._distinct_by(_.brand).to_array()` — pick LAST row per brand
// in source/PK order. n=100k, BRAND_COUNT=5 → 5 rows. Theme 8 audit 2a (PR #2875).
// SQL (m1) — Arm B: `reverse() |> _distinct_by(K)` flips the bare-aggregate from
// MIN(pk) to MAX(pk). `SELECT … FROM (SELECT *, MAX(pk) FROM Cars GROUP BY brand) AS t0`.
// Bench's m1/m3f lanes use parallel chain shapes (the linq Theme 8 splice walks the
// array backward; the SQL form uses MAX(pk) bare-aggregate) — same "last per K"
// semantics, different engines. Note: SQL bare-aggregate returns rows in
// engine-defined order (group-key order, typically), while linq returns
// reverse-source-order. Bench doesn't assert order; for deterministic ordering add
// `_order_by` to the m1 chain.
// Decs (m4) — splice gates on `array_source` predicate (linq_fold.das:3221), so the
// arm is array-only. plan_decs_reverse has no backward-index analog (archetype walk
// is forward-only). Closed by design — see sqlite_linq_gaps.md "decs scope".

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let rows <- _sql(db |> select_from(type<Car>) |> reverse() |> _distinct_by(_.brand))
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
            let rows <- _fold(each(arr).reverse()._distinct_by(_.brand).to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
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
            unsafe {
                let rows <- (from_xml_node(doc.document_element, type<Car>) |> reverse() |> _distinct_by(_.brand) |> to_array())
                b |> accept(rows)
                if (empty(rows)) {
                    b->failNow()
                }
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
            unsafe {
                let rows <- _fold(from_xml_node(doc.document_element, type<Car>).reverse()._distinct_by(_.brand).to_array())
                b |> accept(rows)
                if (empty(rows)) {
                    b->failNow()
                }
            }
        }
    }
}

[benchmark]
def reverse_distinct_by_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def reverse_distinct_by_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def reverse_distinct_by_m5(b : B?) {
    run_m5(b, 100000)
}

[benchmark]
def reverse_distinct_by_m5f(b : B?) {
    run_m5f(b, 100000)
}
