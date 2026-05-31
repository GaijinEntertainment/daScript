options gen2
options persistent_heap

require _common public

// _group_by(price % 100) |> _select((K = key, S = sum(price))).
// SQL: `SELECT (price % 100), SUM(price) FROM Cars GROUP BY (price % 100)` — exercises
// sqlite_linq's expression-key support in _group_by (PR S2). The m1 form uses
// `_group_by(_.price % 100)` + explicit inner select inside SUM (the shape the
// grouped-aggregate dispatch recognises); the m3f/m4 forms keep their splice-friendly
// `_select(_.price) |> _group_by(_ % 100) |> _select((K, S=_._1 |> sum()))` pattern,
// which fuses the upstream Car→price projection into an intermediate var bind.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let groups <- _sql(db |> select_from(type<Car>)
                                  |> _group_by(_.price % 100)
                                  |> _select((K = _._0, S = _._1 |> select($(c : Car) => c.price) |> sum())))
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
                            ._select(_.price)
                            ._group_by(_ % 100)
                            ._select((K = _._0, S = _._1 |> sum()))
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
                            ._select(_.price)
                            ._group_by(_ % 100)
                            ._select((K = _._0, S = _._1 |> sum()))
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
                                ._select(_.price)
                                ._group_by(_ % 100)
                                ._select((K = _._0, S = _._1 |> sum()))
                                .to_array())
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def groupby_select_sum_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def groupby_select_sum_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def groupby_select_sum_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def groupby_select_sum_m5f(b : B?) {
    run_m5f(b, 100000)
}
