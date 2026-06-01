options gen2
options persistent_heap

require _common public

let THRESHOLD = 500

// SQL: `SELECT ... WHERE price > THRESHOLD ORDER BY id DESC LIMIT 1` — engine reverses
// then takes one. Array and Decs lanes walk the source once, overwriting a single
// bind on each match.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let row = _fold(db |> select_from(type<Car>) |> _where(_.price > THRESHOLD)
                              |> _order_by_descending(_.id) |> _first())
            b |> accept(row)
            if (row.price == 0) {
                b->failNow()
            }
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let row = _fold(each(arr)._where(_.price > THRESHOLD).last())
        b |> accept(row)
        if (row.price == 0) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let row = _fold(from_decs_template(type<DecsCar>)._where(_.price > THRESHOLD).last())
        b |> accept(row)
        if (row.price == 0) {
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
            let row = _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.price > THRESHOLD).last())
            b |> accept(row)
            if (row.price == 0) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def last_match_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def last_match_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def last_match_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def last_match_m5f(b : B?) {
    run_m5f(b, 100000)
}
