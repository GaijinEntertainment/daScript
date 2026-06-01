options gen2
options persistent_heap

require _common public

let TAKE_N = 10

// _reverse() |> take(N) |> _select(_.name) |> to_array — last-N rows reversed then project.
// SQL: SELECT name FROM Cars ORDER BY id DESC LIMIT N.
// Array/Decs splice through plan_reverse / plan_decs_reverse terminal-_select extension
// (Theme 1): skip-into-tail two-pass walk fills the R1-R4 buffer with raw elements,
// then the projection F runs at most K times at return. NOT accepted: reverse._select.take
// (user must reorder).

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let rows <- _fold(db |> select_from(type<Car>) |> _order_by_descending(_.id) |> take(TAKE_N) |> _select(_.name))
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
        unsafe {
            let rows <- _fold(each(arr).reverse().take(TAKE_N)._select(_.name).to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        unsafe {
            let rows <- _fold(from_decs_template(type<DecsCar>).reverse().take(TAKE_N)._select(_.name).to_array())
            b |> accept(rows)
            if (empty(rows)) {
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
            unsafe {
                let rows <- _fold(from_xml_node(doc.document_element, type<Car>).reverse().take(TAKE_N)._select(_.name).to_array())
                b |> accept(rows)
                if (empty(rows)) {
                    b->failNow()
                }
            }
        }
    }
}

[benchmark]
def reverse_take_select_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def reverse_take_select_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def reverse_take_select_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def reverse_take_select_m5f(b : B?) {
    run_m5f(b, 100000)
}
