options gen2
options persistent_heap

require _common public

let TAKE_N = 5

// _order_by(_) |> distinct() |> take(N) |> to_array — sort, dedup whole-element, take N
// (Theme 3 Phase 3, audit C5). plain distinct() (no key) requires duplicate-prone source
// elements to exercise the set-gate; we synthesize an `array<int>` of `i % BRAND_COUNT`
// so duplicates are dense.
// Array (m3f) splices through plan_order_family's bounded-heap + set-gate path
// (same code path as C1, distinct gate triggered on element instead of key).
// Decs (m4) uses DecsBrand fixture (`_common.das`) — single-int template populated
// `i % BRAND_COUNT` so n=100k has dense duplicates that exercise the C5 set-gate.
// The m4 vs m3f ratio is NOT apples-to-apples: m3f operates on `array<int>` where
// each element is a workhorse type and `unique_key(int)` (linq.das:614) returns the
// int directly with no allocation; m4 operates on DecsBrand which is a struct, so
// `unique_key(DecsBrand)` falls into the `"{a}"` string-interpolation branch (one
// string allocation per element). The 6× m4 overhead is the per-entity string-hash
// cost, not decs-walk vs array-walk. For a key-based dedup variant on decs that
// avoids unique_key's string path entirely, see `distinct_by_count_m4` which dedups
// on a workhorse key via `_distinct_by(_.brand)`.
// SQL (m1) uses the `Brand` 1-column fixture (`_common.das`) — `_sql`'s
// `_order_by` requires a `_.Field` (column-ref) key, not bare `_`, so the
// chain wraps the single column in a named tuple via `_select((value=_.value))`
// before sorting. Emits the clean `SELECT DISTINCT "value" FROM "Brands"
// ORDER BY "value" ASC LIMIT ?` form (no subquery wrap).

def make_brands(n : int) : array<int> {
    var a : array<int>
    a |> resize(n)
    for (i in range(n)) {
        a[i] = i % BRAND_COUNT
    }
    return <- a
}

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db_brands(db, n)
        b |> run("m1_sql/{n}", n) {
            let rows <- _sql(db |> select_from(type<Brand>) |> _select((value = _.value)) |> distinct() |> _order_by(_.value) |> take(TAKE_N))
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

def run_m3f(b : B?; n : int) {
    let brands <- make_brands(n)
    b |> run("m3f_array_fold/{n}", n) {
        unsafe {
            let rows <- _fold(each(brands)._order_by(_).distinct().take(TAKE_N).to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs_brands(n)
    b |> run("m4_decs_fold/{n}", n) {
        unsafe {
            let rows <- _fold(from_decs_template(type<DecsBrand>)._order_by(_.value).distinct().take(TAKE_N).to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

// m5_xml lane: non-fold baseline. The XML source carries whole Cars, so the brand stream is projected
// out with a leading _select(_.brand) (the m3f/m4 lanes source a precomputed brand array). Un-fused.
def run_m5(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        b |> run("m5_xml/{n}", n) {
            unsafe {
                let rows <- (from_xml_node(doc.document_element, type<Car>) |> _select(_.brand) |> _order_by(_) |> distinct() |> take(TAKE_N) |> to_array())
                b |> accept(rows)
                if (empty(rows)) {
                    b->failNow()
                }
            }
        }
    }
}

// m5f_xml_fold lane: fused _fold over from_xml_node, with the same _select(_.brand) projection prefix.
def run_m5f(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        b |> run("m5f_xml_fold/{n}", n) {
            unsafe {
                let rows <- _fold(from_xml_node(doc.document_element, type<Car>)._select(_.brand)._order_by(_).distinct().take(TAKE_N).to_array())
                b |> accept(rows)
                if (empty(rows)) {
                    b->failNow()
                }
            }
        }
    }
}

[benchmark]
def order_distinct_take_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def order_distinct_take_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def order_distinct_take_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def order_distinct_take_m5(b : B?) {
    run_m5(b, 100000)
}

[benchmark]
def order_distinct_take_m5f(b : B?) {
    run_m5f(b, 100000)
}
