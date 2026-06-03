options gen2
options persistent_heap

require _common public

// zip(a, b, sel) |> sum — dot product via 3-arg zip pre-lowering (Theme 1, audit 7a).
// The 3-arg form `zip(a, b, sel)` is pre-lowered by plan_zip to
// `zip(a, b) |> _select(sel-as-tuple)` so the standard zip+select fusion fires —
// this bench measures the natural dot-product idiom that previously cascaded.
// SQL (m1) is the degenerate same-row form `_select(int64(_.price) * int64(_.year)) |> sum()` — the int64
// cast renders `CAST(... AS INTEGER)` and drives the int64 read-back, so the 64-bit `SUM(price*year)`
// (~1e11 at n=100k) is faithful (without the cast the int projection truncates). Same SQL lane as
// zip_dot_product (SQL has no positional zip, so both 2-arg+_select and 3-arg forms degenerate to it).
// No decs form (no zip on archetype walks). int64 cast in the selector avoids int32 overflow for n=100k.

def make_ints(n : int) : array<int> {
    var a : array<int>
    a |> resize(n)
    for (i in range(n)) {
        a[i] = i
    }
    return <- a
}

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let s = _fold(db |> select_from(type<Car>) |> _select(int64(_.price) * int64(_.year)) |> sum())
            b |> accept(s)
            if (s == 0l) {
                b->failNow()
            }
        }
    }
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
def zip_dot_product_3arg_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def zip_dot_product_3arg_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def zip_dot_product_3arg_m5f(b : B?) {
    run_m5f(b, 100000)
}
