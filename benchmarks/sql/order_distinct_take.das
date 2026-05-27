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
// SQL: `_sql`'s `_order_by` requires a `_.Field` (column-ref) key, not bare `_`
// (`error[50503]: _sql: _order_by: key must be `_.Field`, a `string` expression,
// or a tuple of either; got: _`). This is independent of distinct/take ordering —
// `distinct_take.das` shows `_sql` does lower `distinct() |> take(N)` when the
// source has a named column. Bench operates on a synthesized bare-scalar
// `array<int>`, so the SQL form has no `.Field` to project. By design — to add
// a SQL lane we'd need a 1-column table fixture and lower `_order_by(_.col)`.

def make_brands(n : int) : array<int> {
    var a : array<int>
    a |> resize(n)
    for (i in range(n)) {
        a[i] = i % BRAND_COUNT
    }
    return <- a
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

[benchmark]
def order_distinct_take_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def order_distinct_take_m4(b : B?) {
    run_m4(b, 100000)
}
