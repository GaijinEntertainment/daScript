options gen2
options persistent_heap

require _common public

// Fixture car ids run 1..n in source order, so `id < THRESHOLD` is true for the leading
// THRESHOLD-1 elements then false from THRESHOLD on. take_while breaks on the first false,
// so splice visits only the prefix.
//
// SQL: `SELECT COUNT(*) FROM Cars WHERE id < THRESHOLD` — query planner uses pk index.
// Array and Decs lanes walk the prefix, break on the first false, count via in-loop
// accumulator (no buffer allocation).

let THRESHOLD = 50000

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let total = _sql(db |> select_from(type<Car>) |> _where(_.id < THRESHOLD) |> count())
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
        let total = _fold(each(arr)._take_while(_.id < THRESHOLD).count())
        b |> accept(total)
        if (total == 0) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let total = _fold(from_decs_template(type<DecsCar>)._take_while(_.id < THRESHOLD).count())
        b |> accept(total)
        if (total == 0) {
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
            let total = unsafe(from_xml_node(doc.document_element, type<Car>)) |> _take_while(_.id < THRESHOLD) |> count()
            b |> accept(total)
            if (total == 0) {
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
            let total = _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._take_while(_.id < THRESHOLD).count())
            b |> accept(total)
            if (total == 0) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def take_while_match_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def take_while_match_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def take_while_match_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def take_while_match_m5(b : B?) {
    run_m5(b, 100000)
}

[benchmark]
def take_while_match_m5f(b : B?) {
    run_m5f(b, 100000)
}
