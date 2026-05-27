options gen2
options persistent_heap

require _common public

let THRESHOLD = 2009

// _distinct_by(_.brand) |> count(P) — distinct brands whose first-occurrence Car
// satisfies P (Theme 4, audit 3c).
// SQL (m1) uses the SQLite bare-aggregate `_distinct_by` wrap added in
// sqlite_linq.das::finalize_distinct_by_count_wrap: emits
// `SELECT COUNT(*) FROM (SELECT *, MIN("id") FROM "Cars" GROUP BY "brand") WHERE "year" > ?`.
// The inner subquery's MIN(id) drives SQLite's bare-aggregate optimization to
// pick the row with the smallest Id per brand — matches linq's "first row per key"
// semantics when Id is monotonic with insertion order (fixture sets Id = i + 1).
// User idiom inside `_sql(...)` is the `_count(_.field > X)` linq_boost shorthand,
// NOT the explicit `count($(c) => ...)` lambda form which sqlite_linq's macro
// boundary can't type-bind.
// Array/Decs splice through plan_distinct / plan_decs_distinct's 2-arg-count extension:
// dedup table built unconditionally so `distinct_by` semantics keeps FIRST occurrence
// per key; a separate counter increments only when P matches that first occurrence.
// Companion to distinct_by_count.das which has no predicate.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let c = _sql(db |> select_from(type<Car>) |> _distinct_by(_.brand) |> _count(_.year > THRESHOLD))
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        unsafe {
            let c = _fold(each(arr) |> _distinct_by(_.brand) |> count($(c) => c.year > THRESHOLD))
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        unsafe {
            let c = _fold(from_decs_template(type<DecsCar>) |> _distinct_by(_.brand) |> count($(c) => c.year > THRESHOLD))
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def distinct_count_pred_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def distinct_count_pred_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def distinct_count_pred_m4(b : B?) {
    run_m4(b, 100000)
}
