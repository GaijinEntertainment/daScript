options gen2
options persistent_heap

require _common public

// _group_by(_.brand) |> _having(_._1 |> length >= 5) |> _select((Brand=key, N=length)).
// SQL: SELECT brand, COUNT(*) FROM cars GROUP BY brand HAVING COUNT(*) >= 5.
// Splice mode (PR-D) recognizes that the having predicate's length(_._1) matches the
// select's length slot — the predicate rewrites to `kv._{slot} >= 5` and wraps the
// result-build push_clone in a per-bucket filter. Per-element loop is unchanged from
// PR-A2's count splice; the only extra cost is the post-table filter.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let groups <- _sql(db |> select_from(type<Car>)
                                  |> _group_by(_.brand)
                                  |> _having(_._1 |> length >= 5)
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
                            ._having(_._1 |> length >= 5)
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
                            ._having(_._1 |> length >= 5)
                            ._select((Brand = _._0, N = _._1 |> length))
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
                                ._having(_._1 |> length >= 5)
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
def groupby_having_count_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def groupby_having_count_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def groupby_having_count_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def groupby_having_count_m5f(b : B?) {
    run_m5f(b, 100000)
}
