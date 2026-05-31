options gen2
options persistent_heap

require _common public

// _group_by(_.brand) |> _select((Brand, Total)) |> _where(_.Total > THRESHOLD) — post-aggregate HAVING.
// SQL: SELECT brand, SUM(price) AS Total FROM Cars GROUP BY brand HAVING Total > T.
// Array/Decs splice through plan_group_by_core's trailing-_where extension (Theme 2,
// audit 4a/4e): binds the constructed output tuple once per bucket and gates buf-emit
// on the post-aggregate predicate. Distinct from the existing `_having(P)` slot
// (which is pre-select and can lift hidden reducer slots); both can fire on the
// same chain. Companion to groupby_having_count.das which uses `_having`.

let THRESHOLD = 9000000

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let groups <- _sql(db |> select_from(type<Car>)
                                  |> _group_by(_.brand)
                                  |> _select((Brand = _._0,
                                              Total = _._1 |> select($(c : Car) => c.price) |> sum()))
                                  |> _where(_.Total > THRESHOLD))
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let groups <- _fold(each(arr)
                            ._group_by(_.brand)
                            ._select((Brand = _._0,
                                      Total = _._1 |> select($(c : Car) => c.price) |> sum()))
                            ._where(_.Total > THRESHOLD)
                            .to_array())
        b |> accept(groups)
        if (empty(groups)) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let groups <- _fold(from_decs_template(type<DecsCar>)
                            ._group_by(_.brand)
                            ._select((Brand = _._0,
                                      Total = _._1 |> _select(_.price) |> sum()))
                            ._where(_.Total > THRESHOLD)
                            .to_array())
        b |> accept(groups)
        if (empty(groups)) {
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
            let groups <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))
                                ._group_by(_.brand)
                                ._select((Brand = _._0,
                                          Total = _._1 |> select($(c : Car) => c.price) |> sum()))
                                ._where(_.Total > THRESHOLD)
                                .to_array())
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def groupby_having_post_where_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def groupby_having_post_where_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def groupby_having_post_where_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def groupby_having_post_where_m5f(b : B?) {
    run_m5f(b, 100000)
}
