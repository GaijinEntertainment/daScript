options gen2
options persistent_heap

require _common public

let TAKE_N = 10

// _order_by_descending(_.price) |> take(N) — top-N largest pattern.
// SQL: ORDER BY price DESC LIMIT N — engine emits partial-sort optimization when possible.
// m3 (plain LINQ) does a full descending sort then slices.
// m3f (spliced via plan_order_family Phase 3b) dispatches directly to ``top_n_by_descending``
// which uses partial_sort with a flipped comparator (array source) — O(M log N).

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let rows <- _sql(db |> select_from(type<Car>) |> _order_by_descending(_.price) |> take(TAKE_N))
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let rows <- (arr |> _order_by_descending(_.price) |> take(TAKE_N))
        if (empty(rows)) {
            b->failNow()
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let rows <- _fold(each(arr)._order_by_descending(_.price).take(TAKE_N).to_array())
        if (empty(rows)) {
            b->failNow()
        }
    }
}

[benchmark]
def order_take_desc_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def order_take_desc_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def order_take_desc_m3f(b : B?) {
    run_m3f(b, 100000)
}
