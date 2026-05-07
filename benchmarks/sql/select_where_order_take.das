options gen2
options persistent_heap

require _common public

let THRESHOLD = 500
let TAKE_N = 10

// --- m1: _sql over :memory: ---
def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let rows <- _sql(db |> select_from(type<Car>)
                                |> _where(_.price > THRESHOLD)
                                |> _order_by(_.price)
                                |> take(TAKE_N))
            if (length(rows) == 0) {
                b->failNow()
            }
        }
    }
}

// --- m3: array LINQ (materializing intermediate arrays) ---
def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let rows <- (arr |> _where(_.price > THRESHOLD)
                         |> _order_by(_.price)
                         |> take(TAKE_N))
        if (length(rows) == 0) {
            b->failNow()
        }
    }
}

// --- m3f: array LINQ folded into a single fused pass ---
def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let rows <- _fold(each(arr)._where(_.price > THRESHOLD)
                                   ._order_by(_.price)
                                   .take(TAKE_N)
                                   .to_array())
        if (length(rows) == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def select_where_order_take_10k_m1(b : B?) {
    run_m1(b, 10000)
}

[benchmark]
def select_where_order_take_10k_m3(b : B?) {
    run_m3(b, 10000)
}

[benchmark]
def select_where_order_take_10k_m3f(b : B?) {
    run_m3f(b, 10000)
}
