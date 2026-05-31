options gen2
options persistent_heap

require _common public

let THRESHOLD = 500

// SQL: SELECT ... LIMIT 1 — engine stops at first hit. Use _first_opt then is_some
// since sqlite_linq doesn't expose `_any()` as a chain terminal.
// Array and Decs lanes use `_any(pred)` which short-circuits at the first match.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let opt = _sql(db |> select_from(type<Car>) |> _where(_.price > THRESHOLD) |> _first_opt())
            b |> accept(opt)
            if (!is_some(opt)) {
                b->failNow()
            }
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let yes = _fold(each(arr)._any(_.price > THRESHOLD))
        b |> accept(yes)
        if (!yes) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let yes = _fold(from_decs_template(type<DecsCar>)._any(_.price > THRESHOLD))
        b |> accept(yes)
        if (!yes) {
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
            let yes = _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._any(_.price > THRESHOLD))
            b |> accept(yes)
            if (!yes) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def any_match_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def any_match_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def any_match_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def any_match_m5f(b : B?) {
    run_m5f(b, 100000)
}
