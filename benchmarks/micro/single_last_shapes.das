options gen2
options persistent_heap

require pugixml/PUGIXML_boost
require dastest/testing_boost
require strings

// Micro-bench: the two emit shapes for a bounded-output XML chain whose whole row escapes but only
// ONE element survives — `where(pred) |> single()` and `where(pred) |> last()`. Validates the
// "materialize-under-guard" design before touching build_xml_materializer (see the
// profile-hand-coded-before-macro methodology).
//
//   C (current)   — full row materialized per source element (build_xml_row reads all 6 attrs incl.
//                   the `name` clone), THEN the where guard runs. N clones for 1 survivor.
//   A (candidate) — read only the predicate field cheaply (read_xml_field, no clone), and materialize
//                   the full row INSIDE the match guard. Clones only for elements that pass the predicate.
//
// Self-contained (no _common dependency) so it stays out of the results.md sweep over benchmarks/sql/.

let N = 100000
let BRAND_COUNT = 5
let DEALER_COUNT = 100
let THRESHOLD = 500
let TARGET_ID = 42

struct Car {
    id        : int
    name      : string
    price     : int
    brand     : int
    year      : int
    dealer_id : int
}

def fixture_xml_string(n : int) : string {
    return build_string() $(var w) {
        w |> write("<cars>")
        for (i in range(n)) {
            w |> write("<car id=\"{i + 1}\" name=\"Car{i}\" price=\"{(i * 37) % 1000}\" brand=\"{i % BRAND_COUNT}\" year=\"{2010 + (i * 7) % 16}\" dealer_id=\"{(i % DEALER_COUNT) + 1}\"/>")
        }
        w |> write("</cars>")
    }
}

// ===== single: where(id == TARGET_ID) |> single() — exactly one survivor =====

// C — full materialize per element (what the macro emits today: allUsed → build_xml_row per element).
def single_current(de : xml_node) : Car {
    var found = 0
    var result : Car
    for (c in from_xml_node(de, type<Car>)) {
        if (c.id == TARGET_ID) {
            result = c
            found++
        }
    }
    verify(found == 1)
    return result
}

// A — under-guard: read only `id` cheaply; build the full row only for the matching element.
def single_under_guard(de : xml_node) : Car {
    var found = 0
    var result : Car
    let xdef = default<Car>
    var ch = de.first_child
    while (ch.ok) {
        let cur = ch
        ch = ch.next_sibling
        if (cur._type == xml_node_type.node_element) {
            let xf_id = read_xml_field(cur, "id", xdef.id)
            if (xf_id == TARGET_ID) {
                var it = default<Car>
                build_xml_row(cur, it)
                result = it
                found++
            }
        }
    }
    verify(found == 1)
    return result
}

// ===== last: where(price > THRESHOLD) |> last() — ~half the rows match, one survivor =====

// C — full materialize per element, rebind whole row on each match.
def last_current(de : xml_node) : Car {
    var result : Car
    var any = false
    for (c in from_xml_node(de, type<Car>)) {
        if (c.price > THRESHOLD) {
            result = c
            any = true
        }
    }
    verify(any)
    return result
}

// A — under-guard: read only `price` cheaply; build the full row only for matching elements.
def last_under_guard(de : xml_node) : Car {
    var result : Car
    var any = false
    let xdef = default<Car>
    var ch = de.first_child
    while (ch.ok) {
        let cur = ch
        ch = ch.next_sibling
        if (cur._type == xml_node_type.node_element) {
            let xf_price = read_xml_field(cur, "price", xdef.price)
            if (xf_price > THRESHOLD) {
                var it = default<Car>
                build_xml_row(cur, it)
                result = it
                any = true
            }
        }
    }
    verify(any)
    return result
}

// ===== string-typed guard: where(name == TARGET) |> single — read (clones per element) vs peek (view) =====
// Demonstrates why the under-guard path emits peek_xml_field, not read_xml_field, for guard fields:
// read_xml_field clones every string guard read; peek_xml_field returns a non-owning view (the local is
// consumed by the cond and never stored), so only the surviving full row clones.

let TARGET_NAME = "Car42"

def single_string_read(de : xml_node) : Car {
    var found = 0
    var result : Car
    let xdef = default<Car>
    var ch = de.first_child
    while (ch.ok) {
        let cur = ch
        ch = ch.next_sibling
        if (cur._type == xml_node_type.node_element) {
            let xf_name = read_xml_field(cur, "name", xdef.name)   // clones name every element
            if (xf_name == TARGET_NAME) {
                var it = default<Car>
                build_xml_row(cur, it)
                result = it
                found++
            }
        }
    }
    verify(found == 1)
    return result
}

def single_string_peek(de : xml_node) : Car {
    var found = 0
    var result : Car
    let xdef = default<Car>
    var ch = de.first_child
    while (ch.ok) {
        let cur = ch
        ch = ch.next_sibling
        if (cur._type == xml_node_type.node_element) {
            let xf_name = peek_xml_field(cur, "name", xdef.name)   // non-owning view, no clone
            if (xf_name == TARGET_NAME) {
                var it = default<Car>
                build_xml_row(cur, it)
                result = it
                found++
            }
        }
    }
    verify(found == 1)
    return result
}

// touch the whole survivor (id + name length) so the name clone can't be elided
def check(c : Car) : int {
    return c.id + length(c.name)
}

def run_single_current(b : B?) {
    let xml = fixture_xml_string(N)
    parse_xml(xml) $(doc, ok) {
        if (!ok) { b->failNow(); return }
        let de = doc.document_element
        b |> run("C_single_current/{N}", N) {
            let r = single_current(de)
            if (check(r) == 0) { b->failNow() }
        }
    }
}

def run_single_under_guard(b : B?) {
    let xml = fixture_xml_string(N)
    parse_xml(xml) $(doc, ok) {
        if (!ok) { b->failNow(); return }
        let de = doc.document_element
        b |> run("A_single_under_guard/{N}", N) {
            let r = single_under_guard(de)
            if (check(r) == 0) { b->failNow() }
        }
    }
}

def run_last_current(b : B?) {
    let xml = fixture_xml_string(N)
    parse_xml(xml) $(doc, ok) {
        if (!ok) { b->failNow(); return }
        let de = doc.document_element
        b |> run("C_last_current/{N}", N) {
            let r = last_current(de)
            if (check(r) == 0) { b->failNow() }
        }
    }
}

def run_last_under_guard(b : B?) {
    let xml = fixture_xml_string(N)
    parse_xml(xml) $(doc, ok) {
        if (!ok) { b->failNow(); return }
        let de = doc.document_element
        b |> run("A_last_under_guard/{N}", N) {
            let r = last_under_guard(de)
            if (check(r) == 0) { b->failNow() }
        }
    }
}

def run_single_string_read(b : B?) {
    let xml = fixture_xml_string(N)
    parse_xml(xml) $(doc, ok) {
        if (!ok) { b->failNow(); return }
        let de = doc.document_element
        b |> run("C_single_string_read_clones/{N}", N) {
            let r = single_string_read(de)
            if (check(r) == 0) { b->failNow() }
        }
    }
}

def run_single_string_peek(b : B?) {
    let xml = fixture_xml_string(N)
    parse_xml(xml) $(doc, ok) {
        if (!ok) { b->failNow(); return }
        let de = doc.document_element
        b |> run("A_single_string_peek_view/{N}", N) {
            let r = single_string_peek(de)
            if (check(r) == 0) { b->failNow() }
        }
    }
}

[benchmark]
def probe_single_C_current(b : B?) {
    run_single_current(b)
}

[benchmark]
def probe_single_A_under_guard(b : B?) {
    run_single_under_guard(b)
}

[benchmark]
def probe_last_C_current(b : B?) {
    run_last_current(b)
}

[benchmark]
def probe_last_A_under_guard(b : B?) {
    run_last_under_guard(b)
}

[benchmark]
def probe_single_string_C_read(b : B?) {
    run_single_string_read(b)
}

[benchmark]
def probe_single_string_A_peek(b : B?) {
    run_single_string_peek(b)
}
