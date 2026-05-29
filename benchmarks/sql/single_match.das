options gen2
options persistent_heap

require _common public

// Fixture car ids run 1..n so id=42 is unique. `single` walks the full source
// (semantically — exactly-one-match), but the splice still fuses upstream where.
//
// SQL: SELECT * FROM Cars WHERE id = 42 — pk lookup returns the single row.
// Array and Decs lanes traverse the source filtering and asserting one survivor.

let TARGET_ID = 42

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let row = _sql(db |> select_from(type<Car>) |> _where(_.id == TARGET_ID) |> _first())
            b |> accept(row)
            if (row.id == 0) {
                b->failNow()
            }
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let row = _fold(each(arr)._where(_.id == TARGET_ID).single())
        b |> accept(row)
        if (row.id == 0) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let row = _fold(from_decs_template(type<DecsCar>)._where(_.id == TARGET_ID).single())
        b |> accept(row)
        if (row.id == 0) {
            b->failNow()
        }
    }
}

// m5_xml lane: non-fold baseline — plain iteration over from_xml_node, breaking
// on the (unique) match.
def run_m5(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        let root = doc.document_element
        b |> run("m5_xml/{n}", n) {
            var row = default<Car>
            for (car in from_xml_node(root, type<Car>)) {
                if (car.id == TARGET_ID) {
                    row = car
                    break
                }
            }
            b |> accept(row)
            if (row.id == 0) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def single_match_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def single_match_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def single_match_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def single_match_m5(b : B?) {
    run_m5(b, 100000)
}
