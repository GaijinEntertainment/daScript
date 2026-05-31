options gen2
options persistent_heap

require pugixml/PUGIXML_boost
require dastest/testing_boost
require daslib/linq
require strings

// Micro-bench: the three output shapes the join_select macro could emit, side-by-side, to validate
// the fused-materialize design over the streaming generator (see the no-intermediate-materialization
// methodology). Self-contained (no _common dependency) so it lives in benchmarks/micro/ without
// interfering with the results.md table-building sweep over benchmarks/sql/.
//
// Chain modeled: from_xml_node(Car) |> _join(dealers, c.dealer_id==d.id, (CarName=c.name,DealerId=d.id)) |> _select(_.CarName)
// All three return iterator<string> (car names), consumed identically (sum of lengths), so the delta
// isolates output shape. C = tier-2 status quo (materialize full tuples), A = fused-but-materialized
// (the wrapIter path — chosen winner), B = streaming generator.
//
// Finding (2026-05-30): A wins in every config (INTERP/JIT, with/without the from_xml string clone).
// Streaming is inherently slower (generator-resume overhead); the per-element string clone is a flat
// ~23-34ns cost orthogonal to output shape.

let BRAND_COUNT = 5
let DEALER_COUNT = 100
let N = 100000

struct Car {
    id        : int
    name      : string
    price     : int
    brand     : int
    year      : int
    dealer_id : int
}

struct Dealer {
    id   : int
    name : string
}

typedef JoinRow = tuple<CarName : string; DealerId : int>

def fixture_xml_string(n : int) : string {
    return build_string() $(var w) {
        w |> write("<cars>")
        for (i in range(n)) {
            w |> write("<car id=\"{i + 1}\" name=\"Car{i}\" price=\"{(i * 37) % 1000}\" brand=\"{i % BRAND_COUNT}\" year=\"{2010 + (i * 7) % 16}\" dealer_id=\"{(i % DEALER_COUNT) + 1}\"/>")
        }
        w |> write("</cars>")
    }
}

def fixture_dealers_array() : array<Dealer> {
    var arr : array<Dealer>
    arr |> resize(DEALER_COUNT)
    for (i in range(DEALER_COUNT)) {
        arr[i] = Dealer(id = i + 1, name = "Dealer{i}")
    }
    return <- arr
}

def build_dealer_hash(dealers : array<Dealer>) : table<int; array<Dealer>> {
    var hash : table<int; array<Dealer>>
    for (d in dealers) {   // nolint:PERF022 per-key bucket insert, not an array bulk-copy
        hash[d.id] |> push_clone(d)   // nolint:PERF006 per-key bucket size unknown
    }
    return <- hash
}

// C — tier-2 status quo: materialize full result tuples, then lazy-select projects CarName.
def join_select_tier2(de : xml_node; dealers : array<Dealer>) : iterator<string> {
    let hash <- build_dealer_hash(dealers)
    var buf : array<JoinRow>
    for (c in from_xml_node(de, type<Car>)) {
        var b = unsafe(hash?[c.dealer_id])
        if (b != null) {
            for (m in *b) {
                buf |> push_clone((CarName = c.name, DealerId = m.id))   // nolint:PERF006 join match count unknown
            }
        }
    }
    return <- generator<string> capture(<- buf) {
        for (row in buf) {
            yield row.CarName
        }
        return false
    }
}

// A — fused-but-materialized (the wrapIter path, chosen): field-pruned walk, push projected CarName
// into a string buffer, wrap as sequence.
def join_select_materialize(de : xml_node; dealers : array<Dealer>) : iterator<string> {
    let hash <- build_dealer_hash(dealers)
    var buf : array<string>
    for (c in from_xml_node(de, type<Car>)) {
        var b = unsafe(hash?[c.dealer_id])
        if (b != null) {
            for (_m in *b) {
                buf |> push_clone(c.name)
            }
        }
    }
    return <- buf.to_sequence_move()
}

// B — streaming generator: build the hash, then walk the DOM yielding each projected match. No result buffer.
def join_select_stream(de : xml_node; dealers : array<Dealer>) : iterator<string> {
    var hash <- build_dealer_hash(dealers)
    return <- generator<string> capture(<- hash, := de) {
        for (c in unsafe(from_xml_node(de, type<Car>))) {
            var b = unsafe(hash?[c.dealer_id])
            if (b != null) {
                for (_m in *b) {
                    yield c.name
                }
            }
        }
        return false
    }
}

def consume(var seq : iterator<string>) : int {
    var total = 0
    for (s in seq) {
        total += length(s)
    }
    return total
}

def run_tier2(b : B?) {
    let xml = fixture_xml_string(N)
    let dealers <- fixture_dealers_array()
    parse_xml(xml) $(doc, ok) {
        if (!ok) { b->failNow(); return }
        let de = doc.document_element
        b |> run("C_tier2_materialize_tuples/{N}", N) {
            var seq <- join_select_tier2(de, dealers)
            let t = consume(seq)
            if (t == 0) { b->failNow() }
        }
    }
}

def run_materialize(b : B?) {
    let xml = fixture_xml_string(N)
    let dealers <- fixture_dealers_array()
    parse_xml(xml) $(doc, ok) {
        if (!ok) { b->failNow(); return }
        let de = doc.document_element
        b |> run("A_fused_materialize_strings/{N}", N) {
            var seq <- join_select_materialize(de, dealers)
            let t = consume(seq)
            if (t == 0) { b->failNow() }
        }
    }
}

def run_stream(b : B?) {
    let xml = fixture_xml_string(N)
    let dealers <- fixture_dealers_array()
    parse_xml(xml) $(doc, ok) {
        if (!ok) { b->failNow(); return }
        let de = doc.document_element
        b |> run("B_streaming_generator/{N}", N) {
            var seq <- join_select_stream(de, dealers)
            let t = consume(seq)
            if (t == 0) { b->failNow() }
        }
    }
}

[benchmark]
def probe_C_tier2(b : B?) {
    run_tier2(b)
}

[benchmark]
def probe_A_materialize(b : B?) {
    run_materialize(b)
}

[benchmark]
def probe_B_stream(b : B?) {
    run_stream(b)
}
