options gen2
options persistent_heap

require _common public

// Indexed point-lookup: _where(_.id == K) where id is the PRIMARY KEY.
// SQLite uses the PK b-tree -> O(log n).
// Array has no index -> O(n) linear scan.
// Decs uses the eid hash (query(eid, ...) wraps for_eid_archetype) -> O(1).
// SQL wins over Array by complexity-class margin; Decs wins over SQL by skipping
// the b-tree descent.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        let key = n / 2
        b |> run("m1_sql/{n}") {
            let c = _fold(db |> select_from(type<Car>) |> _where(_.id == key) |> count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

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

def run_m5f(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    let key = n / 2
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        b |> run("m5f_xml_fold/{n}") {
            let c = _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.id == key).count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

// Decs eid lookup (O(1) hash): query(eid, $(...)) wraps for_eid_archetype, which
// hashes the eid into entityLookup and dispatches the block once if the archetype
// matches. The block-arg `car_id` is the request-shape witness AND a fixture-drift
// gate: fixture_decs_capture_mid sets id = i+1 on the n/2-th entity, so reading
// car_id back gives us a non-trivial value that proves the lookup actually visited
// the expected entity (not a coincidence).
def run_m4(b : B?; n : int) {
    let target_eid = fixture_decs_capture_mid(n)
    let expected_id = n / 2 + 1
    b |> run("m4_decs_eid/{n}") {
        var found = 0
        query(target_eid) $(car_id : int) {
            found = car_id
        }
        b |> accept(found)
        if (found != expected_id) {
            b->failNow()
        }
    }
}

[benchmark]
def indexed_lookup_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def indexed_lookup_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def indexed_lookup_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def indexed_lookup_m5f(b : B?) {
    run_m5f(b, 100000)
}
