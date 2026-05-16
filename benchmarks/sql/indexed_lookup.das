options gen2
options persistent_heap

require _common public

// Indexed point-lookup: _where(_.id == K) where id is the PRIMARY KEY.
// SQLite uses the PK b-tree -> O(log n).
// m3/m3f have no index -> O(n) linear scan over the array.
// Inverse-asymmetry of count_aggregate: m1 wins by complexity-class margin.

// --- m1: _sql over :memory: ---
def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        let key = n / 2
        b |> run("m1_sql/{n}") {
            let c = _sql(db |> select_from(type<Car>) |> _where(_.id == key) |> count())
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

// --- m3: array LINQ (linear scan) ---
def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    let key = n / 2
    b |> run("m3_array/{n}") {
        let c = arr |> _where(_.id == key) |> count()
        if (c == 0) {
            b->failNow()
        }
    }
}

// --- m3f_old: pre-rewrite baseline fold ---
def run_m3f_old(b : B?; n : int) {
    let arr <- fixture_array(n)
    let key = n / 2
    b |> run("m3f_old_array_fold/{n}") {
        let c = _old_fold(each(arr)._where(_.id == key).count())
        if (c == 0) {
            b->failNow()
        }
    }
}

// --- m3f: array LINQ folded into a single fused pass ---
def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    let key = n / 2
    b |> run("m3f_array_fold/{n}") {
        let c = _fold(each(arr)._where(_.id == key).count())
        if (c == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def indexed_lookup_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def indexed_lookup_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def indexed_lookup_m3f_old(b : B?) {
    run_m3f_old(b, 100000)
}

[benchmark]
def indexed_lookup_m3f(b : B?) {
    run_m3f(b, 100000)
}
