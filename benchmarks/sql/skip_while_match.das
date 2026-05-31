options gen2
options persistent_heap

require _common public

// Fixture car ids run 1..n in source order, so `id < THRESHOLD` is true for the leading
// THRESHOLD-1 elements then false from THRESHOLD on. skip_while flips on the first false
// and emits everything that follows; the splice walks the entire source.
// The splice gain is buffer elision — a reference `skip_while_impl` materializes an
// `array<T>` of survivors before count; splice fuses into the counter lane (in-loop ++).
//
// SQL: `SELECT COUNT(*) FROM Cars WHERE id >= THRESHOLD` — index scan + aggregate.
// Array and Decs lanes: single pass, no allocation, in-loop counter `++`.

let THRESHOLD = 50000

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let total = _sql(db |> select_from(type<Car>) |> _where(_.id >= THRESHOLD) |> count())
            b |> accept(total)
            if (total == 0) {
                b->failNow()
            }
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let total = _fold(each(arr)._skip_while(_.id < THRESHOLD).count())
        b |> accept(total)
        if (total == 0) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let total = _fold(from_decs_template(type<DecsCar>)._skip_while(_.id < THRESHOLD).count())
        b |> accept(total)
        if (total == 0) {
            b->failNow()
        }
    }
}

def run_m5f(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        b |> run("m5f_xml_fold/{n}", n) {
            let total = _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._skip_while(_.id < THRESHOLD).count())
            b |> accept(total)
            if (total == 0) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def skip_while_match_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def skip_while_match_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def skip_while_match_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def skip_while_match_m5f(b : B?) {
    run_m5f(b, 100000)
}
