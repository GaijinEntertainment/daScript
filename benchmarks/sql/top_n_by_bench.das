options gen2
options persistent_heap

require _common public
require daslib/sort_boost
require daslib/linq

// top_n_by(arr, N, key) vs the existing order_by(...) |> take(N). Adds the
// linq.top_n_by family (array + iterator sources). The m1 column is SQL
// ORDER BY ... LIMIT, which the engine implements as a partial sort + LIMIT
// (the same shape we're benchmarking).
//
// m3f / m3f_old are intentionally identical here — Phase 0 only adds the
// library function; PR B (BufferTopN emit mode in linq_fold) is what makes
// the m3f column move.

let N_TAKE = 10

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql_order_limit/{n}", n) {
            let rows <- _sql(db |> select_from(type<Car>) |> _order_by(_.price) |> take(N_TAKE))
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array_order_take/{n}", n) {
        let rows <- (arr |> _order_by(_.price) |> take(N_TAKE))
        if (empty(rows)) {
            b->failNow()
        }
    }
}

def run_m3_topn_array(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_topn_array/{n}", n) {
        let rows <- top_n_by(arr, N_TAKE, @@(c : Car -&) => c.price)
        if (empty(rows)) {
            b->failNow()
        }
    }
}

def run_m3_topn_iter(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_topn_iter/{n}", n) {
        let rows <- top_n_by(arr.to_sequence(), N_TAKE, @@(c : Car -&) => c.price)
        if (empty(rows)) {
            b->failNow()
        }
    }
}

def run_m3f_old(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_old_array_fold/{n}", n) {
        unsafe {
            let rows <- _old_fold(each(arr)._order_by(_.price).take(N_TAKE).to_array())
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
            let rows <- _fold(each(arr)._order_by(_.price).take(N_TAKE).to_array())
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def top_n_by_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def top_n_by_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def top_n_by_m3_topn_array(b : B?) {
    run_m3_topn_array(b, 100000)
}

[benchmark]
def top_n_by_m3_topn_iter(b : B?) {
    run_m3_topn_iter(b, 100000)
}

[benchmark]
def top_n_by_m3f_old(b : B?) {
    run_m3f_old(b, 100000)
}

[benchmark]
def top_n_by_m3f(b : B?) {
    run_m3f(b, 100000)
}
