options gen2
options persistent_heap

require _common public

let TAKE_N = 3

// _select(_.brand) |> distinct() |> take(N) — early-exit dedup pattern.
// SQL: SELECT DISTINCT brand FROM Cars LIMIT N — engine bounds the dedup hashset.
// Both m3 and m3f use each(arr) for apples-to-apples comparison.
// m3 (plain LINQ on iterator) routes through linq.das:668 — a lazy `distinct()` generator
// composed with lazy `take(N)`, both yielding per element. Stops at N distinct yields.
// m3f (spliced via plan_distinct): single fused for-loop with outer-loop take-guard
// at the top — breaks as soon as the Nth distinct value has been consumed (true O(N)
// source elements worst-case). No generator overhead. With BRAND_COUNT=5 and TAKE_N=3
// the splice exits at source position ~3.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let rows <- _sql(db |> select_from(type<Car>) |> _select(_.brand) |> distinct() |> take(TAKE_N))
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        unsafe {
            let rows <- (each(arr) |> _select(_.brand) |> distinct() |> take(TAKE_N) |> to_array())
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
            let rows <- _fold(each(arr)._select(_.brand).distinct().take(TAKE_N).to_array())
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
            let rows <- _fold(from_decs_template(type<DecsCar>)._select(_.brand).distinct().take(TAKE_N).to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def distinct_take_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def distinct_take_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def distinct_take_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def distinct_take_m4(b : B?) {
    run_m4(b, 100000)
}
