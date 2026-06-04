options gen2
options persistent_heap

require _common public

// cross_join(A, B, into) — the Cartesian product of two independent sources, materialized + counted.
// SQL: SELECT ... FROM Cars, Dealers (pushdown; rows marshalled back). In-memory (m3f/m5f): cross_join()
// builds the full |A|×|B| product array (UNFUSED — there is no _fold cross splice yet; this is the baseline
// the fusion optimization will improve). Because the product is O(n·m), n is SMALL here (CROSS_N, vs 100000
// in the other families) so the product stays ~100k rows.
//
// PR1 measures the engine ops in their current WORKING forms (typed lambdas, bare-pipe / _sql), NOT the
// reader's `_fold` emit (that lands in the cross_join engine PR). Lanes:
//   m1  SQL   — _sql(select_from |> _cross_join(select_from, into))
//   m3f array — cross_join(arrA, arrB, into)
//   m5f XML   — cross_join_to_array(from_xml_node, arrB, into)
// NO decs lane (m4): a standalone `from_decs_template` source yields anonymous tuples, so there is no clean
// typed-lambda cross form — decs cross_join arrives with the _fold engine integration (cross_join PR).

let CROSS_N = 1000   // 1000 cars × 100 dealers = 100k pairs

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let rows <- _sql(db |> select_from(type<Car>)
                                |> _cross_join(db |> select_from(type<Dealer>),
                                               $(ca : Car, de : Dealer) => (CarId = ca.id, DealerId = de.id)))
            let c = length(rows)
            b |> accept(c)
            if (c != n * DEALER_COUNT) {
                b->failNow()
            }
        }
    }
}

def run_m3f(b : B?; n : int) {
    let cars <- fixture_array(n)
    let dealers <- fixture_dealers_array()
    b |> run("m3f_array_fold/{n}", n) {
        let rows <- cross_join(cars, dealers, $(ca : Car, de : Dealer) => (CarId = ca.id, DealerId = de.id))
        let c = length(rows)
        b |> accept(c)
        if (c != n * DEALER_COUNT) {
            b->failNow()
        }
    }
}

def run_m5f(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    let dealers <- fixture_dealers_array()
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        b |> run("m5f_xml_fold/{n}", n) {
            let rows <- cross_join_to_array(unsafe(from_xml_node(doc.document_element, type<Car>)), dealers,
                                            $(ca : Car, de : Dealer) => (CarId = ca.id, DealerId = de.id))
            let c = length(rows)
            b |> accept(c)
            if (c != n * DEALER_COUNT) {
                b->failNow()
            }
        }
    }
}

def run_m6f(b : B?; n : int) {
    let jv = fixture_json(n)
    let dealers <- fixture_dealers_array()
    b |> run("m6f_json_fold/{n}", n) {
        let rows <- cross_join_to_array(unsafe(from_json(jv, type<Car>)), dealers,
                                        $(ca : Car, de : Dealer) => (CarId = ca.id, DealerId = de.id))
        let c = length(rows)
        b |> accept(c)
        if (c != n * DEALER_COUNT) {
            b->failNow()
        }
    }
}

[benchmark]
def cross_join_m1(b : B?) {
    run_m1(b, CROSS_N)
}

[benchmark]
def cross_join_m3f(b : B?) {
    run_m3f(b, CROSS_N)
}

[benchmark]
def cross_join_m5f(b : B?) {
    run_m5f(b, CROSS_N)
}

[benchmark]
def cross_join_m6f(b : B?) {
    run_m6f(b, CROSS_N)
}
