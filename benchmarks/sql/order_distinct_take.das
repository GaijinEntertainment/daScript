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
// Decs (m4) skipped: no duplicate-prone decs template in _common.das; the splice path
// IS verified in tests/linq/test_linq_fold_theme3_c1_c5_distinct_order_take.das
// (test_c5_decs_order_distinct_take_to_array). Follow-up TODO: add DecsBrand fixture.
// SQL: SELECT DISTINCT brand FROM (... ORDER BY brand) LIMIT N — sqlite_linq's
// `distinct() |> take(N)` shape rejects on `_sql`. By design — no follow-up TODO.

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

[benchmark]
def order_distinct_take_m3f(b : B?) {
    run_m3f(b, 100000)
}
