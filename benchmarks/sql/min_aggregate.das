options gen2
options persistent_heap

require _common public

// SQL: SELECT MIN(price) FROM Cars.
// Array and Decs lanes scan the source tracking the minimum seen.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let m = _fold(db |> select_from(type<Car>) |> _select(_.price) |> min())
            b |> accept(m)
            if (m > 999) {
                b->failNow()
            }
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let m = _fold(each(arr)._select(_.price).min())
        b |> accept(m)
        if (m > 999) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let m = _fold(from_decs_template(type<DecsCar>)._select(_.price).min())
        b |> accept(m)
        if (m > 999) {
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
            let m = _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._select(_.price).min())
            b |> accept(m)
            if (m > 999) {
                b->failNow()
            }
        }
    }
}

def run_m6f(b : B?; n : int) {
    let jv = fixture_json(n)
    b |> run("m6f_json_fold/{n}", n) {
        let m = _fold(unsafe(from_json(jv, type<Car>))._select(_.price).min())
        b |> accept(m)
        if (m > 999) {
            b->failNow()
        }
    }
}

[benchmark]
def min_aggregate_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def min_aggregate_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def min_aggregate_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def min_aggregate_m5f(b : B?) {
    run_m5f(b, 100000)
}

[benchmark]
def min_aggregate_m6f(b : B?) {
    run_m6f(b, 100000)
}
