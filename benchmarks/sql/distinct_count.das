options gen2
options persistent_heap

require _common public

// _select(_.brand) |> distinct() |> to_array — distinct values then materialize.
// SQL: SELECT DISTINCT brand FROM Cars — engine builds a hashset.
// Array and Decs lanes must materialize a set/dedup before producing results.
// (sqlite_linq's `distinct() |> count()` form raises a not-implemented error
// for `COUNT(DISTINCT col)`; we measure the materialized distinct set instead.)
// Expected length: BRAND_COUNT (5).

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let rows <- _fold(db |> select_from(type<Car>) |> _select(_.brand) |> distinct())
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
        let rows <- _fold(each(arr)._select(_.brand).distinct().to_array())
        b |> accept(rows)
        if (empty(rows)) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let rows <- _fold(from_decs_template(type<DecsCar>)._select(_.brand).distinct().to_array())
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
            let rows <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._select(_.brand).distinct().to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def distinct_count_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def distinct_count_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def distinct_count_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def distinct_count_m5f(b : B?) {
    run_m5f(b, 100000)
}
