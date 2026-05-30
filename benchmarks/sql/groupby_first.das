options gen2
options persistent_heap

require _common public
require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// `_group_by(_.brand) |> _select((Brand=key, FirstCar=first source elem per group))` —
// for each brand, pair the key with the row that arrived first (by source/PK order).
// SQL (m1) — Arm C: tuple-projection over bare-aggregate inner subquery.
// `SELECT "brand", "id", "name", "price", "brand", "year", "dealer_id"
//   FROM (SELECT *, MIN(pk) FROM Cars GROUP BY brand) AS t0`.
// See sqlite_linq.das::is_first_of_group_call + build_row_make_struct.
// Array/Decs (m3f/m4) splice through plan_group_by_core's miss-branch-only loop (PR-A2).

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let groups <- _sql(db |> select_from(type<Car>)
                                  |> _group_by(_.brand)
                                  |> _select((Brand = _._0,
                                              FirstCar = _._1 |> first())))
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
                                      FirstCar = _._1 |> first()))
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
                                      FirstCar = _._1 |> first()))
                            .to_array())
        b |> accept(groups)
        if (empty(groups)) {
            b->failNow()
        }
    }
}

// m5_xml lane: non-fold baseline — the same chain over from_xml_node, un-fused (tier-2 cascade).
def run_m5(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        b |> run("m5_xml/{n}", n) {
            let groups <- (unsafe(from_xml_node(doc.document_element, type<Car>))
                          |> _group_by(_.brand)
                          |> _select((Brand = _._0,
                                      FirstCar = _._1 |> first()))
                          |> to_array())
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
    }
}

// m5f_xml_fold lane: fused _fold over from_xml_node.
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
                                          FirstCar = _._1 |> first()))
                                .to_array())
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def groupby_first_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def groupby_first_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def groupby_first_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def groupby_first_m5(b : B?) {
    run_m5(b, 100000)
}

[benchmark]
def groupby_first_m5f(b : B?) {
    run_m5f(b, 100000)
}
