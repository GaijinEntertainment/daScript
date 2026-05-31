options gen2
options persistent_heap

require _common public

// where(price > 500) |> _group_by(_.brand) |> _select((Brand=key, TotalPrice=sum)).
// SQL: SELECT brand, SUM(price) WHERE price > 500 GROUP BY brand.
// Splice mode (PR-B) fuses upstream where_ + inner-select-sum into the per-element
// loop: filter, then table update with `entry._1 += inner` only for survivors.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let groups <- _sql(db |> select_from(type<Car>)
                                  |> _where(_.price > 500)
                                  |> _group_by(_.brand)
                                  |> _select((Brand = _._0,
                                              TotalPrice = _._1 |> select($(c : Car) => c.price) |> sum())))
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
                            ._where(_.price > 500)
                            ._group_by(_.brand)
                            ._select((Brand = _._0,
                                      TotalPrice = _._1 |> select($(c : Car) => c.price) |> sum()))
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
                            ._where(_.price > 500)
                            ._group_by(_.brand)
                            ._select((Brand = _._0,
                                      TotalPrice = _._1 |> _select(_.price) |> sum()))
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
                                ._where(_.price > 500)
                                ._group_by(_.brand)
                                ._select((Brand = _._0,
                                          TotalPrice = _._1 |> select($(c : Car) => c.price) |> sum()))
                                .to_array())
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def groupby_where_sum_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def groupby_where_sum_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def groupby_where_sum_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def groupby_where_sum_m5f(b : B?) {
    run_m5f(b, 100000)
}
