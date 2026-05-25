options gen2
options persistent_heap

require _common public

let TAKE_N = 10

// _distinct_by(_.dealer_id) |> _order_by(_.price) |> take(N) |> to_array — pick one car
// per dealer, sort by price, take top N (Theme 3 Phase 3, audit C1).
// SQL: no faithful translation — "pick one row per dealer" requires window functions
// (ROW_NUMBER OVER PARTITION BY dealer_id) which sqlite_linq does not lower. Follow-up
// TODO 2026-05-25.
// Array/Decs splice through plan_order_family / plan_decs_order_family's bounded-heap
// + set-gate path: declares `var dset : table<typedecl(...)>` above source loop, wraps
// per-element push by `if (!key_exists(dset, dkey)) { dset|>insert(dkey); HEAP_UPDATE }`.
// Single source pass, no full distinct materialization.

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

[benchmark]
def distinct_by_order_take_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def distinct_by_order_take_m4(b : B?) {
    run_m4(b, 100000)
}
