options gen2
options persistent_heap

require _common public

let TAKE_N = 1000

// take(N) |> to_array — bounded materialization (SQL can't put count() after take()
// because LIMIT-then-aggregate collapses to one row; we measure the array result
// and use length() as the correctness gate). Expected length: TAKE_N (assuming N >= TAKE_N).

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let rows <- _sql(db |> select_from(type<Car>) |> take(TAKE_N))
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let rows <- _fold(each(arr).take(TAKE_N).to_array())
        b |> accept(rows)
        if (empty(rows)) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let rows <- _fold(from_decs_template(type<DecsCar>).take(TAKE_N).to_array())
        b |> accept(rows)
        if (empty(rows)) {
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
            let rows <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>)).take(TAKE_N).to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def take_count_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def take_count_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def take_count_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def take_count_m5f(b : B?) {
    run_m5f(b, 100000)
}
