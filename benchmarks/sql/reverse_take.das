options gen2
options persistent_heap

require _common public

let TAKE_N = 10

// _reverse() |> take(N) — last N rows of the source, reversed.
// SQL: SELECT * FROM Cars ORDER BY id DESC LIMIT N — engine indexes-and-limits.
// m3 (plain LINQ iterator-reverse): generator captures the source, fills an internal buffer,
// then yields buffer[len-i-1] lazily — take(N) only triggers N yields.
// m3f (spliced via plan_reverse): single prefilter pass into a buffer + reverse_inplace
// + resize(min(N, length(buf))). The full O(length) reverse is paid before resize, so for
// TAKE_N << length this is a REGRESSION vs m3 (~34 vs ~22 ns/op at 100K). See LINQ.md
// "Phase 3+" notes — a backward-index-loop optimization is deferred.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let rows <- _sql(db |> select_from(type<Car>) |> _order_by_descending(_.id) |> take(TAKE_N))
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
            let rows <- (each(arr) |> reverse() |> take(TAKE_N) |> to_array())
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
            let rows <- _fold(each(arr).reverse().take(TAKE_N).to_array())
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def reverse_take_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def reverse_take_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def reverse_take_m3f(b : B?) {
    run_m3f(b, 100000)
}
