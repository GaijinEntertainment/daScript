options gen2
options persistent_heap

require _common public

let TAKE_N = 1000
let THRESHOLD = 500

// `_sql` rejects `take(n) |> count()` — semantic mismatch: in SQL, LIMIT after an
// aggregate has no effect (aggregate collapses to one row), so the bound-then-count
// shape has no faithful SQL translation. [By design — no follow-up TODO.] Array and
// Decs lanes filter + bound + count over the source; exercises counter-lane take
// splice with an upstream `where` predicate.

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let c = _fold(each(arr)._where(_.price > THRESHOLD).take(TAKE_N).count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let c = _fold(from_decs_template(type<DecsCar>)._where(_.price > THRESHOLD).take(TAKE_N).count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

// m5_xml lane: non-fold baseline — plain iteration over from_xml_node, capping
// at TAKE_N matches.
def run_m5(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        let root = doc.document_element
        b |> run("m5_xml/{n}", n) {
            var c = 0
            for (car in from_xml_node(root, type<Car>)) {
                if (car.price > THRESHOLD) {
                    c++
                    if (c >= TAKE_N) {
                        break
                    }
                }
            }
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def take_count_filtered_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def take_count_filtered_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def take_count_filtered_m5(b : B?) {
    run_m5(b, 100000)
}

// m5f_xml_fold lane: fused `_fold` over from_xml_node (pass 2a). XmlAdapter emits an inlined
// child-element walk in place of the generator; full row materialization is retained until 2b prunes it.
def run_m5f(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        var root = doc.document_element
        b |> run("m5f_xml_fold/{n}", n) {
            let c = _fold(unsafe(from_xml_node(root, type<Car>))._where(_.price > THRESHOLD).take(TAKE_N).count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def take_count_filtered_m5f(b : B?) {
    run_m5f(b, 100000)
}
