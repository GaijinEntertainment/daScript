options gen2
options persistent_heap

require _common public

// zip(a, b, sel) |> sum — dot product via 3-arg zip pre-lowering (Theme 1, audit 7a).
// The 3-arg form `zip(a, b, sel)` is pre-lowered by plan_zip to
// `zip(a, b) |> _select(sel-as-tuple)` so the standard zip+select fusion fires —
// this bench measures the natural dot-product idiom that previously cascaded.
// SQL (m1) deferred for the same reason as zip_dot_product: the degenerate `SUM(price*year*brand)`
// overflows int and needs an int64-typed computed projection (cast follow-up). No decs form (no zip
// on archetype walks). Companion to zip_dot_product.das which uses the explicit 2-arg + _select form.
// int64 cast in the selector avoids int32 overflow for n=100k.

def make_ints(n : int) : array<int> {
    var a : array<int>
    a |> resize(n)
    for (i in range(n)) {
        a[i] = i
    }
    return <- a
}

def run_m3f(b : B?; n : int) {
    let xs <- make_ints(n)
    let ys <- make_ints(n)
    b |> run("m3f_array_fold/{n}", n) {
        let s = _fold(zip(xs, ys, $(x : int; y : int) => int64(x) * int64(y)).sum())
        b |> accept(s)
        if (s == 0l) {
            b->failNow()
        }
    }
}

// m5_xml lane: 3-arg zip of the XML Car price-stream against a synthetic int array, un-fused (tier-2).
def run_m5(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    let ys <- make_ints(n)
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        b |> run("m5_xml/{n}", n) {
            let s = (zip(unsafe(from_xml_node(doc.document_element, type<Car>)), ys,
                         $(c : Car; y : int) => int64(c.price) * int64(y)) |> sum())
            b |> accept(s)
            if (s == 0l) {
                b->failNow()
            }
        }
    }
}

// m5f_xml_fold lane: fused _fold over the same 3-arg zip(from_xml_node, array, sel).
def run_m5f(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    let ys <- make_ints(n)
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        b |> run("m5f_xml_fold/{n}", n) {
            let s = _fold(zip(unsafe(from_xml_node(doc.document_element, type<Car>)), ys,
                              $(c : Car; y : int) => int64(c.price) * int64(y)).sum())
            b |> accept(s)
            if (s == 0l) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def zip_dot_product_3arg_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def zip_dot_product_3arg_m5(b : B?) {
    run_m5(b, 100000)
}

[benchmark]
def zip_dot_product_3arg_m5f(b : B?) {
    run_m5f(b, 100000)
}
