options gen2
options persistent_heap

require _common public

// _select(_.brand) |> _select(_ + 1) |> distinct() |> count — two adjacent _select calls
// upstream of plan_distinct (Theme 7, audit 7c-family). The collapse_chained_selects
// pre-pass folds the two pure projections into a single composed _select(g(f(_))) before
// plan_distinct dispatches; the downstream splice sees one _select instead of two,
// dropping a per-element bind+invoke. Pure-inner gate fires on the INNER projection
// (here `_.brand`, a field read with no sideeffects per the macro_boost.das classifier);
// the outer `_ + 1` is irrelevant to the gate. The inner expression may be evaluated
// zero or many times after collapse depending on how the outer references its param,
// which is why purity of the inner is what makes the rewrite safe.
// SQL: `_sql` rejects `distinct() |> count()` as non-translatable — it would lower
// to `COUNT(DISTINCT ...)` over the computed projection but sqlite_linq's surface
// doesn't currently emit that form. By design — no follow-up.

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let c = _fold(each(arr) |> _select(_.brand) |> _select(_ + 1) |> distinct() |> count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let c = _fold(from_decs_template(type<DecsCar>) |> _select(_.brand) |> _select(_ + 1) |> distinct() |> count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

// m5_xml lane: non-fold baseline — the same linq chain over from_xml_node, un-fused
// (tier-2 cascade, materializes a full Car per element + intermediate stages). This is
// the reference the fused XML lane (m5f) is measured against: m5f ≈ m5 means the adapter
// is falling back to materialization (no fusion); m5f << m5 means fusion is working.
def run_m5(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        b |> run("m5_xml/{n}", n) {
            let c = unsafe(from_xml_node(doc.document_element, type<Car>)) |> _select(_.brand) |> _select(_ + 1) |> distinct() |> count()
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
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
        b |> run("m5f_xml_fold/{n}", n) {
            let c = _fold(unsafe(from_xml_node(doc.document_element, type<Car>)) |> _select(_.brand) |> _select(_ + 1) |> distinct() |> count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def chained_select_collapse_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def chained_select_collapse_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def chained_select_collapse_m5(b : B?) {
    run_m5(b, 100000)
}

[benchmark]
def chained_select_collapse_m5f(b : B?) {
    run_m5f(b, 100000)
}
