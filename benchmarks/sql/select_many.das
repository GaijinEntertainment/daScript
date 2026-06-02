options gen2
options persistent_heap

require _common public

// select_many_pair(src, collection_selector, result_selector) — flatten each order's `lines` (the correlated
// SelectMany: the inner collection is a field of the outer element). Flattened count = n * LINES_PER_ORDER.
//
// The both-vars form: result_selector sees BOTH the outer `o` and the inner `l`, so the projection is C#'s
// `select (o.id, l.sku)`. This is the engine op the correlated `%linq! from o … from l in o.lines select
// (o.id, l.sku)` reader emits. (PR1 measured the existing 1-arg-result `select_many` with an inner-only
// projection; this is the same flatten with a both-vars result selector.)
//
// ARRAY lane only (m3f). No other lane has a nested-collection source: SQL has no nested-collection flatten
// (correlated select_many over SQL is rejected); a decs component has no `array<…>` field; and the XML linq
// source `from_xml_node` materializes rows from flat ATTRIBUTES only (`build_xml_row`), so it cannot
// populate a nested `array<Line>` field. UNFUSED (no _fold select_many splice).

let SM_N = 100000

def run_m3f(b : B?; n : int) {
    let orders <- fixture_orders_nested(n)
    b |> run("m3f_array_fold/{n}", n) {
        // collection-selector yields a zero-copy view of o.lines via `each` — NOT `to_sequence`, whose
        // const-array overload clones the array per order (linq.das `var b := a`); that clone is not part of
        // select_many and would skew the baseline. This is the shape the correlated reader emits.
        let rows <- select_many_pair(orders, $(o : OrderWithLines) => unsafe(each(o.lines)),
            $(o : OrderWithLines; l : Line) => (id = o.id, sku = l.sku))
        let c = length(rows)
        b |> accept(c)
        if (c != n * LINES_PER_ORDER) {
            b->failNow()
        }
    }
}

[benchmark]
def select_many_m3f(b : B?) {
    run_m3f(b, SM_N)
}
