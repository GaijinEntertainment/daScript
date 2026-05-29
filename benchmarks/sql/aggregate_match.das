options gen2
options persistent_heap

require _common public

// User-supplied binary reducer: sum of prices over a where-filtered slice. SQL
// can express this as `SELECT SUM(price)`. The Array lane peels the block body
// and inlines alongside the where predicate — single pass with no per-element
// block invoke. Decs runs the same fused shape over the archetype walk.

let THRESHOLD = 200

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let total = _sql(db |> select_from(type<Car>) |> _where(_.price > THRESHOLD)
                                |> _select(_.price) |> sum())
            b |> accept(total)
            if (total == 0) {
                b->failNow()
            }
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let total = _fold(each(arr)._where(_.price > THRESHOLD)
            .aggregate(0, $(acc : int, c : Car) => acc + c.price))
        b |> accept(total)
        if (total == 0) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let total = _fold(from_decs_template(type<DecsCar>)._where(_.price > THRESHOLD)
            .aggregate(0, $(acc, c) => acc + c.price))
        b |> accept(total)
        if (total == 0) {
            b->failNow()
        }
    }
}

// m5_xml lane: non-fold baseline — plain iteration over from_xml_node.
def run_m5(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        let root = doc.document_element
        b |> run("m5_xml/{n}", n) {
            var total = 0
            for (car in from_xml_node(root, type<Car>)) {
                if (car.price > THRESHOLD) {
                    total += car.price
                }
            }
            b |> accept(total)
            if (total == 0) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def aggregate_match_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def aggregate_match_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def aggregate_match_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def aggregate_match_m5(b : B?) {
    run_m5(b, 100000)
}

// m5f_xml_fold lane: fused `_fold` over from_xml_node (pass 2b). XmlAdapter emits an inlined
// child-element walk and field-prunes the materialization to the attributes the chain reads.
def run_m5f(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        var root = doc.document_element
        b |> run("m5f_xml_fold/{n}", n) {
            let total = _fold(unsafe(from_xml_node(root, type<Car>))._where(_.price > THRESHOLD)
                .aggregate(0, $(acc : int, c : Car) => acc + c.price))
            b |> accept(total)
            if (total == 0) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def aggregate_match_m5f(b : B?) {
    run_m5f(b, 100000)
}
