options gen2
options persistent_heap

require _common public

let TAKE_N = 1000
let THRESHOLD = 500

// _where(P) |> take(N) |> count — "first N matching, then count". SQL (m1): take before an
// aggregate wraps the pre-aggregate chain into an inner subquery (LIMIT stays inner) and
// counts the outer — `SELECT COUNT(*) FROM (SELECT * FROM Cars WHERE "price" > ? LIMIT ?)`.
// Array and Decs lanes filter + bound + count over the source. Companion to
// take_where_count.das which measures the take-first form.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let c = _fold(db |> select_from(type<Car>) |> _where(_.price > THRESHOLD) |> take(TAKE_N) |> count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let c = _fold(each(arr)._where(_.price > THRESHOLD).take(TAKE_N).count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let c = _fold(from_decs_template(type<DecsCar>)._where(_.price > THRESHOLD).take(TAKE_N).count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def take_count_filtered_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def take_count_filtered_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def take_count_filtered_m4(b : B?) {
    run_m4(b, 100000)
}

def run_m5f(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        b |> run("m5f_xml_fold/{n}", n) {
            let c = _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.price > THRESHOLD).take(TAKE_N).count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

def run_m6f(b : B?; n : int) {
    let jv = fixture_json(n)
    b |> run("m6f_json_fold/{n}", n) {
        let c = _fold(unsafe(from_json(jv, type<Car>))._where(_.price > THRESHOLD).take(TAKE_N).count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def take_count_filtered_m5f(b : B?) {
    run_m5f(b, 100000)
}

[benchmark]
def take_count_filtered_m6f(b : B?) {
    run_m6f(b, 100000)
}
