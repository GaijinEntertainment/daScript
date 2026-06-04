options gen2
options persistent_heap

require _common public

// zip(a, b) |> reverse() |> to_array — pairwise zip with trailing reverse.
// Theme 8 audit C4 (PR #2875, plan_zip array lane). Two int sequences of n elements
// each → output is `array<tuple<int; int>>` reversed. Splice emits the zip lockstep
// loop into `buf`, then `_::reverse_inplace($i(bufName))` tail. Skips the cascade's
// zip_to_array iterator wrap + separate reverse_inplace pass.
// The accumulator lane (zip+select+reverse+sum) is identity — reverse drops out,
// the spliced code is equivalent to plain zip+sum (see zip_dot_product). The
// to_array lane is the one where the trailing reverse actually does work, so
// that's what this bench exercises.
// SQL (m1) / Decs (m4) — by design: `zip` is not a relational operation (no SQL
// form) and not naturally expressible over a single archetype walk (no decs form).
// See benchmarks/sql/sqlite_linq_gaps.md "zip family".

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
            let rows <- _fold(each(xs) |> zip(each(ys)) |> reverse() |> to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
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
            let rows <- _fold(zip(unsafe(from_xml_node(doc.document_element, type<Car>)), ys)
                              .reverse()
                              .to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

def run_m6f(b : B?; n : int) {
    let jv = fixture_json(n)
    let ys <- make_ints(n)
    b |> run("m6f_json_fold/{n}", n) {
        let rows <- _fold(zip(unsafe(from_json(jv, type<Car>)), ys)
                          .reverse()
                          .to_array())
        b |> accept(rows)
        if (empty(rows)) {
            b->failNow()
        }
    }
}

[benchmark]
def zip_reverse_to_array_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def zip_reverse_to_array_m5f(b : B?) {
    run_m5f(b, 100000)
}

[benchmark]
def zip_reverse_to_array_m6f(b : B?) {
    run_m6f(b, 100000)
}
