options gen2
options persistent_heap

require _common public

let THRESHOLD = 500

// where → select → to_array. Three-stage pipeline producing an array of ints.
// Distinguishes from select_where.das which lacks the select stage.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let prices <- _sql(db |> select_from(type<Car>) |> _where(_.price > THRESHOLD) |> _select(_.price))
            b |> accept(prices)
            if (empty(prices)) {
                b->failNow()
            }
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let prices <- _fold(each(arr)._where(_.price > THRESHOLD)._select(_.price).to_array())
        b |> accept(prices)
        if (empty(prices)) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let prices <- _fold(from_decs_template(type<DecsCar>)._where(_.price > THRESHOLD)._select(_.price).to_array())
        b |> accept(prices)
        if (empty(prices)) {
            b->failNow()
        }
    }
}

def run_m5(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        b |> run("m5_xml/{n}", n) {
            let prices <- unsafe(from_xml_node(doc.document_element, type<Car>)) |> _where(_.price > THRESHOLD) |> _select(_.price) |> to_array()
            b |> accept(prices)
            if (empty(prices)) {
                b->failNow()
            }
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
            let prices <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.price > THRESHOLD)._select(_.price).to_array())
            b |> accept(prices)
            if (empty(prices)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def to_array_filter_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def to_array_filter_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def to_array_filter_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def to_array_filter_m5(b : B?) {
    run_m5(b, 100000)
}

[benchmark]
def to_array_filter_m5f(b : B?) {
    run_m5f(b, 100000)
}
