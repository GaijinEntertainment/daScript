options gen2
options persistent_heap

require _common public
require daslib/sort_boost
require daslib/linq

let TAKE_N = 10

// _order_by(_.price) |> take(N) — top-N (here bottom-N since ascending) pattern.
// SQL: ORDER BY price LIMIT N — engine emits partial-sort optimization when possible.
// Array variants must fully sort before indexing the first N.
//
// The m3_topn_* columns measure the new ``linq.top_n_by`` family: array source
// uses ``partial_sort`` under the hood (O(M log N)), iterator source uses a
// bounded heap of size N during the scan (max N elements resident).
//
// m3f / m3f_old are intentionally identical for top-N here — Phase 0 only adds
// the library function; PR B (BufferTopN emit mode in linq_fold) is what makes
// the m3f column move.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let rows <- _sql(db |> select_from(type<Car>) |> _order_by(_.price) |> take(TAKE_N))
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let rows <- (arr |> _order_by(_.price) |> take(TAKE_N))
        if (empty(rows)) {
            b->failNow()
        }
    }
}

def run_m3f_old(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_old_array_fold/{n}", n) {
        unsafe {
            let rows <- _old_fold(each(arr)._order_by(_.price).take(TAKE_N).to_array())
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
            let rows <- _fold(each(arr)._order_by(_.price).take(TAKE_N).to_array())
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

def run_m3_topn_array(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_topn_array/{n}", n) {
        let rows <- top_n_by(arr, TAKE_N, @@(c : Car -&) => c.price)
        if (empty(rows)) {
            b->failNow()
        }
    }
}

def run_m3_topn_iter(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_topn_iter/{n}", n) {
        let rows <- top_n_by(arr.to_sequence(), TAKE_N, @@(c : Car -&) => c.price)
        if (empty(rows)) {
            b->failNow()
        }
    }
}

[benchmark]
def sort_take_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def sort_take_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def sort_take_m3f_old(b : B?) {
    run_m3f_old(b, 100000)
}

[benchmark]
def sort_take_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def sort_take_m3_topn_array(b : B?) {
    run_m3_topn_array(b, 100000)
}

[benchmark]
def sort_take_m3_topn_iter(b : B?) {
    run_m3_topn_iter(b, 100000)
}
