options gen2
options persistent_heap

require _common public

let THRESHOLD = 50000l
let THRESHOLD_SQL = 1000000

// zip(xs, ys) |> _select(int64(_._0) * int64(_._1)) |> count(P) — count of zipped
// products satisfying P. int64 cast avoids int32 overflow for n=100k (i*i wraps past
// INT_MAX, producing negative products and unstable predicate counts).
// SQL (m1): zip has no positional relational form, so this lane measures the degenerate
// same-row interpretation — count of rows whose `price * year` exceeds a threshold, with
// the product pushed into the predicate (`_where(_.price * _.year > ?)`). The per-row
// product fits int32 (only the sum-over-all form overflowed, see zip_dot_product).
// Array (m3f): plan_zip's 2-arg count(P) extension (Theme 4, plan_zip arm).
// The predicate is captured into a dedicated counter-predicate gate around `acc++`
// INSIDE the upstream select wrap, so eager `select(F).count(P)` ordering is preserved
// (F runs once per element, then P decides whether to count). With _select, the
// predicate peels against the projected value via a `vproj` bind. Length-shortcut
// suppressed once P is present.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let c = _sql(db |> select_from(type<Car>) |> _where(_.price * _.year > THRESHOLD_SQL) |> count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

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
        unsafe {
            let c = _fold(each(xs) |> zip(each(ys)) |> _select(int64(_._0) * int64(_._1)) |> count($(v) => v > THRESHOLD))
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

// m5_xml lane: zip the XML Car price-stream against a synthetic int array, un-fused (tier-2 cascade).
def run_m5(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    let ys <- make_ints(n)
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        b |> run("m5_xml/{n}", n) {
            let c = (unsafe(from_xml_node(doc.document_element, type<Car>)) |> zip(ys)
                     |> _select(int64(_._0.price) * int64(_._1))
                     |> count($(v) => v > THRESHOLD))
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

// m5f_xml_fold lane: fused _fold over the same zip(from_xml_node, array) with trailing count(P).
def run_m5f(b : B?; n : int) {
    let xml = fixture_xml_string(n)
    let ys <- make_ints(n)
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        b |> run("m5f_xml_fold/{n}", n) {
            let c = _fold(zip(unsafe(from_xml_node(doc.document_element, type<Car>)), ys)
                          ._select(int64(_._0.price) * int64(_._1))
                          .count($(v) => v > THRESHOLD))
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def zip_count_pred_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def zip_count_pred_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def zip_count_pred_m5(b : B?) {
    run_m5(b, 100000)
}

[benchmark]
def zip_count_pred_m5f(b : B?) {
    run_m5f(b, 100000)
}
