options gen2
options persistent_heap

require _common public

let THRESHOLD = 500
let YEAR_FLOOR = 2015

// where → where → count — two filter stages then count.
// SQL: SELECT COUNT(*) ... WHERE p1 AND p2 — engine combines predicates.
// Array and Decs lanes fuse `where(p1) |> where(p2) |> count` into a single
// pass with both predicates inlined (no per-stage materialization).

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let c = _fold(db |> select_from(type<Car>)
                            |> _where(_.price > THRESHOLD)
                            |> _where(_.year >= YEAR_FLOOR)
                            |> count())
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
        let c = _fold(each(arr)._where(_.price > THRESHOLD)
                               ._where(_.year >= YEAR_FLOOR)
                               .count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let c = _fold(from_decs_template(type<DecsCar>)._where(_.price > THRESHOLD)
                               ._where(_.year >= YEAR_FLOOR)
                               .count())
        b |> accept(c)
        if (c == 0) {
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
            let c = _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.price > THRESHOLD)
                                   ._where(_.year >= YEAR_FLOOR)
                                   .count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def chained_where_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def chained_where_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def chained_where_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def chained_where_m5f(b : B?) {
    run_m5f(b, 100000)
}
