options gen2
options persistent_heap

require _common public

let THRESHOLD = 500

// SQL pushes COUNT(*) to the engine returning one row.
// m3 must materialize the full filtered array, then walk to count it.
// m3f folds where+count into a single fused pass (no intermediate array).
// Highest-asymmetry comparison among the three benchmark chains.

// --- m1: _sql over :memory: ---
def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let c = _sql(db |> select_from(type<Car>) |> _where(_.price > THRESHOLD) |> count())
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

// --- m3: array LINQ (materializes intermediate filter array) ---
def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let c = arr |> _where(_.price > THRESHOLD) |> count()
        if (c == 0) {
            b->failNow()
        }
    }
}

// --- m3f: array LINQ folded into a single fused pass ---
def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let c = _fold(each(arr)._where(_.price > THRESHOLD).count())
        if (c == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def count_aggregate_1m_m1(b : B?) {
    run_m1(b, 1000000)
}

[benchmark]
def count_aggregate_1m_m3(b : B?) {
    run_m3(b, 1000000)
}

[benchmark]
def count_aggregate_1m_m3f(b : B?) {
    run_m3f(b, 1000000)
}
