options gen2
options persistent_heap

require _common public

let TAKE_N = 1000
let THRESHOLD = 500

// take(N) |> _where(P) |> count — "first N elements, then keep matching" semantic
// (Theme 2, audit 5c). Distinct from `_where(P) |> take(N)` which is "first N matching":
// the take cap ticks unconditionally per element here, and _where gates only the
// per-element contribution to the count. plan_loop_or_count's postTakeWhereCond gate
// emits both behaviors in a single splice. SQL (m1): the take-before-where phase conflict
// diverts take into an inner subquery, so WHERE applies to the taken rows —
// `SELECT COUNT(*) FROM (SELECT * FROM Cars LIMIT ?) WHERE "price" > ?`. Companion to
// take_count_filtered.das which measures the _where-first form.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let c = _fold(db |> select_from(type<Car>) |> take(TAKE_N) |> _where(_.price > THRESHOLD) |> count())
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
        let c = _fold(each(arr).take(TAKE_N)._where(_.price > THRESHOLD).count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let c = _fold(from_decs_template(type<DecsCar>).take(TAKE_N)._where(_.price > THRESHOLD).count())
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
            let c = _fold(unsafe(from_xml_node(doc.document_element, type<Car>)).take(TAKE_N)._where(_.price > THRESHOLD).count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def take_where_count_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def take_where_count_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def take_where_count_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def take_where_count_m5f(b : B?) {
    run_m5f(b, 100000)
}
