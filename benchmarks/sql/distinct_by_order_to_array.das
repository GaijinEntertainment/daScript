options gen2
options persistent_heap

require _common public

// _distinct_by(_.dealer_id) |> _order_by(_.price) |> to_array — pick first car per
// dealer, sort by price. NO take suffix (compare against distinct_by_order_take,
// which adds `take(N)` and routes through emit_bounded_heap). n=100k, DEALER_COUNT=100
// → output is 100 rows. Theme 8 audit 3b (PR #2875).
// Both lanes splice through emit_fused_prefilter (linq_fold.das:2351) — single fused
// loop: per-element dset gate (`var order_dset : table<int>`) writes into fresh
// `buf`, then `order_inplace(buf, ...)` tail. Saves cascade's distinct_by_to_array
// intermediate iterator setup. Composes across source modes via the shared FoldArraySpec
// dispatch (PR F4) — same emit fn drives array (each(arr)) and decs
// (from_decs_template(...)) sources.
// SQL (m1) — same window-function story as distinct_by_order_take: "one row per
// dealer" needs `ROW_NUMBER() OVER (PARTITION BY dealer_id ORDER BY price) WHERE rn=1`.
// sqlite_linq does not currently lower window functions — see
// benchmarks/sql/sqlite_linq_gaps.md "window functions".

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

[benchmark]
def distinct_by_order_to_array_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def distinct_by_order_to_array_m4(b : B?) {
    run_m4(b, 100000)
}
