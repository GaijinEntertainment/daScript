options gen2
options persistent_heap

require _common public

let THRESHOLD = 500
let SENTINEL_ID = -1

// first_or_default mirrors first but returns a caller-provided default on empty/no-match
// instead of panicking. Same early-exit characteristics; default bound once at the top of
// the invoke block (matches linq.das line 2397 eager evaluation).

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let row = _sql(db |> select_from(type<Car>) |> _where(_.price > THRESHOLD) |> _first())
            b |> accept(row)
            if (row.price == 0) {
                b->failNow()
            }
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    let sentinel = Car(id = SENTINEL_ID, name = "none", price = 0, brand = 0, year = 0, dealer_id = 0)
    b |> run("m3f_array_fold/{n}", n) {
        let row = _fold(each(arr)._where(_.price > THRESHOLD).first_or_default(sentinel))
        b |> accept(row)
        if (row.id == SENTINEL_ID) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    let sentinel = (id = SENTINEL_ID, name = "none", price = 0, brand = 0, year = 0, dealer_id = 0)
    b |> run("m4_decs_fold/{n}", n) {
        let row = _fold(from_decs_template(type<DecsCar>)._where(_.price > THRESHOLD).first_or_default(sentinel))
        b |> accept(row)
        if (row.id == SENTINEL_ID) {
            b->failNow()
        }
    }
}

def run_m5f(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    let sentinel = Car(id = SENTINEL_ID, name = "none", price = 0, brand = 0, year = 0, dealer_id = 0)
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        b |> run("m5f_xml_fold/{n}", n) {
            let row = _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.price > THRESHOLD).first_or_default(sentinel))
            b |> accept(row)
            if (row.id == SENTINEL_ID) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def first_or_default_match_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def first_or_default_match_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def first_or_default_match_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def first_or_default_match_m5f(b : B?) {
    run_m5f(b, 100000)
}
