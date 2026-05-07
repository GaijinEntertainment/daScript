options gen2
options persistent_heap

require _common public

let THRESHOLD = 500

// SQL pushes COUNT(*) to the engine returning one row.
// M2/M3 must materialize the full filtered array, then walk to count it.
// Highest-asymmetry comparison among the three benchmark chains.

// --- M1m: _sql over :memory: ---
def run_m1m(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1m_sql/{n}", n) {
            let c = _sql(db |> select_from(type<Car>) |> _where(_.price > THRESHOLD) |> count())
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

// --- M1d: _sql over disk file ---
def run_m1d(b : B?; n : int) {
    let path = "_sql_bench_cnt_m1d_{n}.db"
    disk_db_setup(path, n)
    with_sqlite(path) $(db) {
        b |> run("m1d_sql/{n}", n) {
            let c = _sql(db |> select_from(type<Car>) |> _where(_.price > THRESHOLD) |> count())
            if (c == 0) {
                b->failNow()
            }
        }
    }
    disk_db_cleanup(path)
}

// --- M2m: no _sql, :memory: source ---
def run_m2m(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m2m_db_no_sql/{n}", n) {
            let c = db |> select_from(type<Car>) |> _where(_.price > THRESHOLD) |> count()
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

// --- M2d: no _sql, disk source ---
def run_m2d(b : B?; n : int) {
    let path = "_sql_bench_cnt_m2d_{n}.db"
    disk_db_setup(path, n)
    with_sqlite(path) $(db) {
        b |> run("m2d_db_no_sql/{n}", n) {
            let c = db |> select_from(type<Car>) |> _where(_.price > THRESHOLD) |> count()
            if (c == 0) {
                b->failNow()
            }
        }
    }
    disk_db_cleanup(path)
}

// --- M3: array LINQ (materializes intermediate filter array) ---
def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let c = arr |> _where(_.price > THRESHOLD) |> count()
        if (c == 0) {
            b->failNow()
        }
    }
}

// --- M3f: array LINQ folded into a single fused pass ---
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
def count_aggregate_10k_m1m(b : B?) {
    run_m1m(b, 10000)
}

[benchmark]
def count_aggregate_10k_m1d(b : B?) {
    run_m1d(b, 10000)
}

[benchmark]
def count_aggregate_10k_m2m(b : B?) {
    run_m2m(b, 10000)
}

[benchmark]
def count_aggregate_10k_m2d(b : B?) {
    run_m2d(b, 10000)
}

[benchmark]
def count_aggregate_10k_m3(b : B?) {
    run_m3(b, 10000)
}

[benchmark]
def count_aggregate_10k_m3f(b : B?) {
    run_m3f(b, 10000)
}
