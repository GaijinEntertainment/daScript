options gen2
options persistent_heap

require _common public

// _distinct_by(_.brand) |> count — count distinct brand values.
// Exercises plan_group_by_core's peeled keyBlock + plan_distinct/_decs_distinct's
// peeled distinctKeyBlock (the per-element `invoke(keyBlock, _)` is now a direct
// field read). Companion to distinct_count which uses _select(_.brand).distinct()
// on a pre-projected scalar — this one keeps the full source element and uses
// distinct_by, the path that goes through the invoke-block-on-tuple shape.
//
// SQL: `SELECT COUNT(DISTINCT brand) FROM Cars` — sqlite_linq doesn't lower
// `_distinct_by(...) |> count()`. [Follow-up TODO 2026-05-23: error[50503]
// `_sql: unsupported chain root or operator. Got: __::linq\`distinct_by\`...` —
// would need a new dispatch arm in sqlite_linq for distinct/distinct_by.]
//
// Expected result: BRAND_COUNT (5).

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let c = _fold(each(arr)._distinct_by(_.brand).count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let c = _fold(from_decs_template(type<DecsCar>)._distinct_by(_.brand).count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def distinct_by_count_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def distinct_by_count_m4(b : B?) {
    run_m4(b, 100000)
}
