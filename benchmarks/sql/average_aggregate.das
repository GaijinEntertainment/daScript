options gen2
options persistent_heap

require _common public

// SQL: SELECT AVG(price) FROM Cars.
// Array and Decs lanes sum and divide on a single pass.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let a = _sql(db |> select_from(type<Car>) |> _select(_.price) |> average())
            b |> accept(a)
            if (a == 0.0lf) {
                b->failNow()
            }
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let a = _fold(each(arr)._select(double(_.price)).average())
        b |> accept(a)
        if (a == 0.0lf) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let a = _fold(from_decs_template(type<DecsCar>)._select(double(_.price)).average())
        b |> accept(a)
        if (a == 0.0lf) {
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
            let a = _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._select(double(_.price)).average())
            b |> accept(a)
            if (a == 0.0lf) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def average_aggregate_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def average_aggregate_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def average_aggregate_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def average_aggregate_m5f(b : B?) {
    run_m5f(b, 100000)
}
