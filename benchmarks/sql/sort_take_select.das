options gen2
options persistent_heap

require _common public

let TAKE_N = 10

// _order_by(_.price) |> take(N) |> _select(_.name) |> to_array — top-N then project.
// SQL: SELECT name FROM Cars ORDER BY price LIMIT N.
// Array/Decs splice through plan_order_family / plan_decs_order_family terminal-_select
// extension (Theme 1, audit 1a/1e): bounded-heap holds raw element; projection runs
// at most K times at return — vs the unspliced cascade which would project N then sort.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let rows <- _fold(db |> select_from(type<Car>) |> _order_by(_.price) |> take(TAKE_N) |> _select(_.name))
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
            let rows <- _fold(each(arr)._order_by(_.price).take(TAKE_N)._select(_.name).to_array())
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
            let rows <- _fold(from_decs_template(type<DecsCar>)._order_by(_.price).take(TAKE_N)._select(_.name).to_array())
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
                let rows <- _fold(from_xml_node(doc.document_element, type<Car>)._order_by(_.price).take(TAKE_N)._select(_.name).to_array())
                b |> accept(rows)
                if (empty(rows)) {
                    b->failNow()
                }
            }
        }
    }
}

def run_m6f(b : B?; n : int) {
    let jv = fixture_json(n)
    b |> run("m6f_json_fold/{n}", n) {
        unsafe {
            let rows <- _fold(from_json(jv, type<Car>)._order_by(_.price).take(TAKE_N)._select(_.name).to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def sort_take_select_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def sort_take_select_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def sort_take_select_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def sort_take_select_m5f(b : B?) {
    run_m5f(b, 100000)
}

[benchmark]
def sort_take_select_m6f(b : B?) {
    run_m6f(b, 100000)
}
