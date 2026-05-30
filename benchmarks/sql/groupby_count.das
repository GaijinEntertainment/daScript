options gen2
options persistent_heap

require _common public

// _group_by(_.brand) |> _select((Brand=key, N=count)) — per-group count.
// SQL: SELECT brand, COUNT(*) GROUP BY brand — engine hashes once, returns BRAND_COUNT rows.
// Array and Decs lanes must buffer (group_by is fundamentally buffering); splice
// mode still inlines the projection and `length` aggregation per group.
// Expected result: BRAND_COUNT (5) groups, each ~n/5 in size.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let groups <- _sql(db |> select_from(type<Car>)
                                  |> _group_by(_.brand)
                                  |> _select((Brand = _._0, N = _._1 |> length)))
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
                            ._select((Brand = _._0, N = _._1 |> length))
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
                            ._select((Brand = _._0, N = _._1 |> length))
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
                          |> _select((Brand = _._0, N = _._1 |> length))
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
                                ._select((Brand = _._0, N = _._1 |> length))
                                .to_array())
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def groupby_count_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def groupby_count_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def groupby_count_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def groupby_count_m5(b : B?) {
    run_m5(b, 100000)
}

[benchmark]
def groupby_count_m5f(b : B?) {
    run_m5f(b, 100000)
}
