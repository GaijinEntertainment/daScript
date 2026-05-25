options gen2
options persistent_heap

require _common public

let TAKE_N = 10

// _order_by(_.price).reverse().take(N) — sort ascending, reverse for descending, take top N.
// Theme 5 normalization (audit 1b/2b): the pre-dispatch `normalize_order_reverse` rewrite
// flips this to `_order_by_descending(_.price).take(N)` so the same bounded-heap arm
// fires as for the explicit descending form. Without normalization, the chain would
// cascade (reverse expects a finite source — `_order_by` produces an iterator, so the
// chain would materialize, reverse, then take). This bench measures parity with
// `_order_by_descending(_.price).take(N).to_array()` — splice perf should match.
// SQL: SELECT * FROM Cars ORDER BY price DESC LIMIT N.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let rows <- _sql(db |> select_from(type<Car>) |> _order_by_descending(_.price) |> take(TAKE_N))
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
            let rows <- _fold(each(arr)._order_by(_.price).reverse().take(TAKE_N).to_array())
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
            let rows <- _fold(from_decs_template(type<DecsCar>)._order_by(_.price).reverse().take(TAKE_N).to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def order_reverse_normalized_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def order_reverse_normalized_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def order_reverse_normalized_m4(b : B?) {
    run_m4(b, 100000)
}
