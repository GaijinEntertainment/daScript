options gen2
options persistent_heap

require _common public

// Indexed point-lookup: _where(_.id == K) where id is the PRIMARY KEY.
// SQLite uses the PK b-tree -> O(log n).
// m3/m3f have no index -> O(n) linear scan over the array.
// m4 uses the decs eid hash (query(eid, ...) wraps for_eid_archetype) -> O(1).
// m1 wins over m3/m3f by complexity-class margin; m4 wins over m1 by skipping the b-tree descent.

// --- m1: _sql over :memory: ---
def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        let key = n / 2
        b |> run("m1_sql/{n}") {
            let c = _sql(db |> select_from(type<Car>) |> _where(_.id == key) |> count())
            b |> accept(c)
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
        b |> accept(c)
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
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

// --- m4: decs eid lookup (O(1) hash) ---
// query(eid, $(...)) wraps for_eid_archetype, which hashes the eid into entityLookup
// and dispatches the block once if the archetype matches. The block-arg `car_id` is
// the request-shape witness — any field of DecsCar would do; we use id to mirror m1/m3.
def run_m4(b : B?; n : int) {
    let target_eid = fixture_decs_capture_mid(n)
    b |> run("m4_decs_eid/{n}") {
        var found = 0
        query(target_eid) $(car_id : int) {
            found = 1
        }
        b |> accept(found)
        if (found == 0) {
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
def indexed_lookup_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def indexed_lookup_m4(b : B?) {
    run_m4(b, 100000)
}
