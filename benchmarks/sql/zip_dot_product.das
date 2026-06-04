options gen2
options persistent_heap

require _common public

// zip(a, b) |> select(t => int64(t._0) * int64(t._1)) |> sum — dot product of two int sequences.
// SQL (m1) is the degenerate same-row form `_select(int64(_.price) * int64(_.year)) |> sum()`: the int64
// cast renders `CAST("price" AS INTEGER) * CAST("year" AS INTEGER)` and drives the result read-back type,
// so the 64-bit `SUM(price*year)` (~1e11 at n=100k) reads back faithfully — without the cast the int
// projection truncates. Splice mode fuses zip + select + sum into a single accumulator loop. The Decs
// lane is the intra-archetype analogue: price * year on the same DecsCar entity, summed with int64 casts.

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
        let s = _fold(zip(xs, ys)._select(int64(_._0) * int64(_._1)).sum())
        b |> accept(s)
        if (s == 0l) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let s = _fold(from_decs_template(type<DecsCar>)._select(int64(_.price) * int64(_.year)).sum())
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
            let s = _fold(zip(unsafe(from_xml_node(doc.document_element, type<Car>)), ys)
                          ._select(int64(_._0.price) * int64(_._1))
                          .sum())
            b |> accept(s)
            if (s == 0l) {
                b->failNow()
            }
        }
    }
}

def run_m6f(b : B?; n : int) {
    let jv = fixture_json(n)
    let ys <- make_ints(n)
    b |> run("m6f_json_fold/{n}", n) {
        let s = _fold(zip(unsafe(from_json(jv, type<Car>)), ys)
                      ._select(int64(_._0.price) * int64(_._1))
                      .sum())
        b |> accept(s)
        if (s == 0l) {
            b->failNow()
        }
    }
}

[benchmark]
def zip_dot_product_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def zip_dot_product_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def zip_dot_product_m4(b : B?) {
    run_m4(b, 100000)
}

[benchmark]
def zip_dot_product_m5f(b : B?) {
    run_m5f(b, 100000)
}

[benchmark]
def zip_dot_product_m6f(b : B?) {
    run_m6f(b, 100000)
}
