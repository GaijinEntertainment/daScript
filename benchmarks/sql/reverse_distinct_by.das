options gen2
options persistent_heap

require _common public

// each(arr).reverse()._distinct_by(_.brand).to_array() — pick LAST row per brand
// in source order. n=100k, BRAND_COUNT=5 → output is 5 rows (the last car seen
// for each brand). Theme 8 audit 2a (PR #2875, R-2a row in linq_fold.das:3215).
// Array m3f splices through emit_reverse_backward_walk_dset_gate: single backward
// `for (k in 0..length(arr))` walk over `arr[length-1-k]`, `var rev_dset : table<int>`
// gating push by !key_exists. Saves cascade's reverse_to_array allocation AND second
// distinct_by_inplace pass — single source pass with set-gate.
// Decs (m4) — splice gates on `array_source` predicate (linq_fold.das:3221), so the
// arm is array-only. plan_decs_reverse has no backward-index analog (archetype walk
// is forward-only). Skip — see benchmarks/sql/sqlite_linq_gaps.md "decs scope".
// SQL (m1) — no faithful form. "Last row per group" requires
// `ROW_NUMBER() OVER (PARTITION BY brand ORDER BY id DESC) WHERE rn=1` (window
// function). sqlite_linq does not currently lower window functions — see
// sqlite_linq_gaps.md "window functions".

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

[benchmark]
def reverse_distinct_by_m3f(b : B?) {
    run_m3f(b, 100000)
}
