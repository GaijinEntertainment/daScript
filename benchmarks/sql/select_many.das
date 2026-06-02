options gen2
options persistent_heap

require _common public

// select_many(src, collection_selector, result_selector) — flatten each order's `lines` (the correlated
// SelectMany: the inner collection is a field of the outer element). Flattened count = n * LINES_PER_ORDER.
//
// PR1 uses the EXISTING 1-arg-result form: result_selector sees ONLY the inner `Line`, so the projection is
// inner-only (`l.sku`). The both-vars `(outer, inner) => …` form — C#'s `select (o.id, l.sku)` — arrives in
// the correlated select_many engine PR; this file is swapped to it then and re-measured.
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
        // select_many and would skew the baseline. This is also the shape the correlated reader should emit.
        let rows <- select_many(orders, $(o : OrderWithLines) => unsafe(each(o.lines)), $(l : Line) => l.sku)
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
