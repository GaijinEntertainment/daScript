options gen2
options persistent_heap

require _common public

let TAKE_N = 10

// _reverse() |> take(N) — last N rows of the source, reversed.
// SQL: SELECT * FROM Cars ORDER BY id DESC LIMIT N — engine indexes-and-limits.
// Array and Decs lanes splice through plan_reverse + skip-into-tail (PR #2834):
// 2-pass walk that sums archetype/array size first, then for_each_archetype_find
// with whole-arch-skip + partial-arch skip-counter + early-exit after N elements.
// O(N) work regardless of source length.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let rows <- _fold(db |> select_from(type<Car>) |> _order_by_descending(_.id) |> take(TAKE_N))
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
            let rows <- _fold(each(arr).reverse().take(TAKE_N).to_array())
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
            let rows <- _fold(from_decs_template(type<DecsCar>).reverse().take(TAKE_N).to_array())
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
                let rows <- _fold(from_xml_node(doc.document_element, type<Car>).reverse().take(TAKE_N).to_array())
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
            let rows <- _fold(from_json(jv, type<Car>).reverse().take(TAKE_N).to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def reverse_take_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def reverse_take_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def reverse_take_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def reverse_take_m5f(b : B?) {
    run_m5f(b, 100000)
}

[benchmark]
def reverse_take_m6f(b : B?) {
    run_m6f(b, 100000)
}
