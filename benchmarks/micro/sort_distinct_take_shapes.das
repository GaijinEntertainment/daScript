options gen2
options persistent_heap

require pugixml/PUGIXML_boost
require daslib/linq        // spliced_push_heap / spliced_pop_heap (bounded-heap splice primitives)
require dastest/testing_boost
require strings
require math               // min

// Micro-bench: the emit shapes for a buffered/bucketed reducer XML chain whose whole row escapes but
// only a FEW elements survive — `order_by(price) |> take(K)`, `distinct_by(dealer_id) |> order_by(price)
// |> take(K)`, `where(p) |> last()` (N matches → 1 survivor), and `group_by(brand) |> select(first per
// group)` (survivors = #brands). Validates the "unified gate — buffer surrogates/handles, materialize
// survivors" design before touching the emits (profile-hand-coded-before-macro methodology).
//
//   C (current)   — the buffer/heap holds the full materialized row. from_xml_node builds the whole Car
//                   (all 6 attrs incl. the `name` clone) for EVERY source element, then the heap/dedup
//                   keeps only K (or one-per-key). N clones for K survivors — the m5f floor.
//   A (deferred)  — the buffer/heap holds a SURROGATE (precomputed cmp/dedup key + xml_node handle). The
//                   walk reads only the gate scalars cheaply (read_xml_field, no name clone); the full
//                   row is built (build_xml_row) only for the K survivors that remain after the reducer.
//
// The fork A explores is exactly Boris's "store materialized values, or pointers to nodes and then
// materialize" — A stores node handles + the key needed to order/dedup, deferring build_xml_row.
//
// Self-contained (own Car/fixture, no _common dependency) so it stays out of the results.md sweep.

let N = 100000
let BRAND_COUNT = 5
let DEALER_COUNT = 100
let TAKE_K = 10

struct Car {
    id        : int
    name      : string
    price     : int
    brand     : int
    year      : int
    dealer_id : int
}

// Surrogate: the order key (price) + the node handle. The full Car is built from `node` only if this
// surrogate survives the reducer.
struct Surr {
    price : int
    node  : xml_node
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

// ===== sort_take: order_by(price) |> take(K) — K smallest by price, no distinct =====

// C — bounded max-heap of full Cars. from_xml_node materializes every element before the heap decides.
def sort_take_materialized(de : xml_node; k : int) : array<Car> {
    var heap : array<Car>
    for (c in from_xml_node(de, type<Car>)) {
        if (length(heap) < k) {
            heap |> push_clone(c)
            heap |> spliced_push_heap() <| $(a, b : Car) => a.price < b.price
        } elif (c.price < heap[0].price) {
            heap |> spliced_pop_heap() <| $(a, b : Car) => a.price < b.price
            heap[length(heap) - 1] := c
            heap |> spliced_push_heap() <| $(a, b : Car) => a.price < b.price
        }
    }
    sort(heap, $(a, b : Car) => a.price < b.price)
    return <- heap
}

// A — bounded max-heap of (price, node) surrogates. Reads only `price` per element; build_xml_row runs
// only for the K survivors.
def sort_take_deferred(de : xml_node; k : int) : array<Car> {
    let xdef = default<Car>
    var heap : array<Surr>
    var ch = de.first_child
    while (ch.ok) {
        let cur = ch
        ch = ch.next_sibling
        if (cur._type == xml_node_type.node_element) {
            let p = read_xml_field(cur, "price", xdef.price)
            if (length(heap) < k) {
                heap |> push(Surr(price = p, node = cur))
                heap |> spliced_push_heap() <| $(a, b : Surr) => a.price < b.price
            } elif (p < heap[0].price) {
                heap |> spliced_pop_heap() <| $(a, b : Surr) => a.price < b.price
                heap[length(heap) - 1] = Surr(price = p, node = cur)
                heap |> spliced_push_heap() <| $(a, b : Surr) => a.price < b.price
            }
        }
    }
    sort(heap, $(a, b : Surr) => a.price < b.price)
    var out : array<Car>
    out |> reserve(length(heap))
    for (s in heap) {
        var it = default<Car>
        build_xml_row(s.node, it)
        out |> push_clone(it)
    }
    return <- out
}

// ===== distinct_by_order_take: distinct_by(dealer_id) |> order_by(price) |> take(K) =====
// distinct survivors = DEALER_COUNT (100), then ordered by price, take K (10).

// C — dedup on dealer_id, buffer full Cars for first-per-dealer, order, take. Materializes all N.
def distinct_order_take_materialized(de : xml_node; k : int) : array<Car> {
    var dset : table<int>
    var buf : array<Car>
    for (c in from_xml_node(de, type<Car>)) {
        if (!key_exists(dset, c.dealer_id)) {
            dset |> insert(c.dealer_id)
            buf |> push_clone(c)
        }
    }
    sort(buf, $(a, b : Car) => a.price < b.price)
    var out : array<Car>
    let lim = min(k, length(buf))
    out |> reserve(lim)
    for (i in range(lim)) {
        out |> push_clone(buf[i])
    }
    return <- out
}

// A — dedup on dealer_id (cheap read), buffer (price, node) surrogates for first-per-dealer, order, take,
// then build_xml_row only for the K kept.
def distinct_order_take_deferred(de : xml_node; k : int) : array<Car> {
    let xdef = default<Car>
    var dset : table<int>
    var buf : array<Surr>
    var ch = de.first_child
    while (ch.ok) {
        let cur = ch
        ch = ch.next_sibling
        if (cur._type == xml_node_type.node_element) {
            let did = read_xml_field(cur, "dealer_id", xdef.dealer_id)
            if (!key_exists(dset, did)) {
                dset |> insert(did)
                let p = read_xml_field(cur, "price", xdef.price)
                buf |> push(Surr(price = p, node = cur))
            }
        }
    }
    sort(buf, $(a, b : Surr) => a.price < b.price)
    var out : array<Car>
    let lim = min(k, length(buf))
    out |> reserve(lim)
    for (i in range(lim)) {
        var it = default<Car>
        build_xml_row(buf[i].node, it)
        out |> push_clone(it)
    }
    return <- out
}

// touch each survivor's id + name length so the name clone can't be elided. Generic so it accepts the
// value (`array<Car>`), ref (`array<Car const -&>`), and pointer-materialized result lanes.
def check(rows) : int {
    var acc = 0
    for (c in rows) {
        acc += c.id + length(c.name)
    }
    return acc
}

def run_sort_take_C(b : B?) {
    let xml = fixture_xml_string(N)
    parse_xml(xml) $(doc, ok) {
        if (!ok) { b->failNow(); return }
        let de = doc.document_element
        b |> run("C_sort_take_materialized/{N}", N) {
            let r <- sort_take_materialized(de, TAKE_K)
            if (check(r) == 0) { b->failNow() }
        }
    }
}

def run_sort_take_A(b : B?) {
    let xml = fixture_xml_string(N)
    parse_xml(xml) $(doc, ok) {
        if (!ok) { b->failNow(); return }
        let de = doc.document_element
        b |> run("A_sort_take_deferred/{N}", N) {
            let r <- sort_take_deferred(de, TAKE_K)
            if (check(r) == 0) { b->failNow() }
        }
    }
}

def run_distinct_order_take_C(b : B?) {
    let xml = fixture_xml_string(N)
    parse_xml(xml) $(doc, ok) {
        if (!ok) { b->failNow(); return }
        let de = doc.document_element
        b |> run("C_distinct_order_take_materialized/{N}", N) {
            let r <- distinct_order_take_materialized(de, TAKE_K)
            if (check(r) == 0) { b->failNow() }
        }
    }
}

def run_distinct_order_take_A(b : B?) {
    let xml = fixture_xml_string(N)
    parse_xml(xml) $(doc, ok) {
        if (!ok) { b->failNow(); return }
        let de = doc.document_element
        b |> run("A_distinct_order_take_deferred/{N}", N) {
            let r <- distinct_order_take_deferred(de, TAKE_K)
            if (check(r) == 0) { b->failNow() }
        }
    }
}

[benchmark]
def probe_sort_take_C_materialized(b : B?) {
    run_sort_take_C(b)
}

[benchmark]
def probe_sort_take_A_deferred(b : B?) {
    run_sort_take_A(b)
}

[benchmark]
def probe_distinct_order_take_C_materialized(b : B?) {
    run_distinct_order_take_C(b)
}

[benchmark]
def probe_distinct_order_take_A_deferred(b : B?) {
    run_distinct_order_take_A(b)
}

// ===== Array lanes: current clone-heap baseline vs `array<Car?>` pointer-heap =====
// Settles whether arrays benefit from the same "buffer a handle, materialize survivors" deferral as
// XML. The current array lane (top_n_by_with_cmp / emit_bounded_heap) buffers via push_clone — it
// CLONES each buffered element. The pointer lane stores `Car const?` into the source array and
// materializes (clones) only the K survivors. Backing store (the source array) outlives the fold.

def fixture_array(n : int) : array<Car> {
    var arr : array<Car>
    arr |> reserve(n)
    for (i in range(n)) {
        arr |> emplace <| Car(id = i + 1, name = "Car{i}", price = (i * 37) % 1000,
            brand = i % BRAND_COUNT, year = 2010 + (i * 7) % 16, dealer_id = (i % DEALER_COUNT) + 1)
    }
    return <- arr
}

// C — bounded max-heap of full Cars (push_clone). Mirrors the array lane today.
def sort_take_array_clone(arr : array<Car>; k : int) : array<Car> {
    var heap : array<Car>
    for (c in arr) {
        if (length(heap) < k) {
            heap |> push_clone(c)
            heap |> spliced_push_heap() <| $(a, b : Car) => a.price < b.price
        } elif (c.price < heap[0].price) {
            heap |> spliced_pop_heap() <| $(a, b : Car) => a.price < b.price
            heap[length(heap) - 1] := c
            heap |> spliced_push_heap() <| $(a, b : Car) => a.price < b.price
        }
    }
    sort(heap, $(a, b : Car) => a.price < b.price)
    return <- heap
}

// A — bounded max-heap of pointers into the source array; clone only the K survivors at the tail.
def sort_take_array_ptr(arr : array<Car>; k : int) : array<Car> {
    var heap : array<Car const?>
    for (c in arr) {
        let p = unsafe(addr(c))
        if (length(heap) < k) {
            heap |> push(p)
            heap |> spliced_push_heap() <| $(a, b : Car const?) => a.price < b.price
        } elif (c.price < heap[0].price) {
            heap |> spliced_pop_heap() <| $(a, b : Car const?) => a.price < b.price
            heap[length(heap) - 1] = p
            heap |> spliced_push_heap() <| $(a, b : Car const?) => a.price < b.price
        }
    }
    sort(heap, $(a, b : Car const?) => a.price < b.price)
    var out : array<Car>
    out |> reserve(length(heap))
    for (p in heap) {
        out |> push_clone(*p)
    }
    return <- out
}

// C — dedup on dealer_id, buffer full Cars for first-per-dealer, order, take. Clones all survivors.
def distinct_order_take_array_clone(arr : array<Car>; k : int) : array<Car> {
    var dset : table<int>
    var buf : array<Car>
    for (c in arr) {
        if (!key_exists(dset, c.dealer_id)) {
            dset |> insert(c.dealer_id)
            buf |> push_clone(c)
        }
    }
    sort(buf, $(a, b : Car) => a.price < b.price)
    var out : array<Car>
    let lim = min(k, length(buf))
    out |> reserve(lim)
    for (i in range(lim)) {
        out |> push_clone(buf[i])
    }
    return <- out
}

// A — dedup on dealer_id, buffer pointers for first-per-dealer, order, take, clone only the K kept.
def distinct_order_take_array_ptr(arr : array<Car>; k : int) : array<Car> {
    var dset : table<int>
    var buf : array<Car const?>
    for (c in arr) {
        if (!key_exists(dset, c.dealer_id)) {
            dset |> insert(c.dealer_id)
            buf |> push(unsafe(addr(c)))
        }
    }
    sort(buf, $(a, b : Car const?) => a.price < b.price)
    var out : array<Car>
    let lim = min(k, length(buf))
    out |> reserve(lim)
    for (i in range(lim)) {
        out |> push_clone(*buf[i])
    }
    return <- out
}

def run_sort_take_array_C(b : B?) {
    let arr <- fixture_array(N)
    b |> run("C_sort_take_array_clone/{N}", N) {
        let r <- sort_take_array_clone(arr, TAKE_K)
        if (check(r) == 0) { b->failNow() }
    }
}

def run_sort_take_array_A(b : B?) {
    let arr <- fixture_array(N)
    b |> run("A_sort_take_array_ptr/{N}", N) {
        let r <- sort_take_array_ptr(arr, TAKE_K)
        if (check(r) == 0) { b->failNow() }
    }
}

def run_distinct_order_take_array_C(b : B?) {
    let arr <- fixture_array(N)
    b |> run("C_distinct_order_take_array_clone/{N}", N) {
        let r <- distinct_order_take_array_clone(arr, TAKE_K)
        if (check(r) == 0) { b->failNow() }
    }
}

def run_distinct_order_take_array_A(b : B?) {
    let arr <- fixture_array(N)
    b |> run("A_distinct_order_take_array_ptr/{N}", N) {
        let r <- distinct_order_take_array_ptr(arr, TAKE_K)
        if (check(r) == 0) { b->failNow() }
    }
}

[benchmark]
def probe_sort_take_array_C_clone(b : B?) {
    run_sort_take_array_C(b)
}

[benchmark]
def probe_sort_take_array_A_ptr(b : B?) {
    run_sort_take_array_A(b)
}

[benchmark]
def probe_distinct_order_take_array_C_clone(b : B?) {
    run_distinct_order_take_array_C(b)
}

[benchmark]
def probe_distinct_order_take_array_A_ptr(b : B?) {
    run_distinct_order_take_array_A(b)
}

// ===== Ref-form lanes: `array<Car const -&>` (exactly what top_n_by_with_cmp / the array emit use) =====
// The current array baseline buffers `array<TT -const -&>` via push_clone (linq.das:597,600). This lane
// reproduces that form to compare apples-to-apples against the clone-value and pointer lanes.

def sort_take_array_ref(arr : array<Car>; k : int) : array<Car -const -&> {
    var heap : array<Car -const -&>
    for (c in arr) {
        if (length(heap) < k) {
            heap |> push_clone(c)
            heap |> spliced_push_heap() <| $(a, b) => a.price < b.price
        } elif (c.price < heap[0].price) {
            heap |> spliced_pop_heap() <| $(a, b) => a.price < b.price
            heap[length(heap) - 1] := c
            heap |> spliced_push_heap() <| $(a, b) => a.price < b.price
        }
    }
    sort(heap, $(a, b) => a.price < b.price)
    return <- heap
}

def distinct_order_take_array_ref(arr : array<Car>; k : int) : array<Car -const -&> {
    var dset : table<int>
    var buf : array<Car -const -&>
    for (c in arr) {
        if (!key_exists(dset, c.dealer_id)) {
            dset |> insert(c.dealer_id)
            buf |> push_clone(c)
        }
    }
    sort(buf, $(a, b) => a.price < b.price)
    var out : array<Car -const -&>
    let lim = min(k, length(buf))
    out |> reserve(lim)
    for (i in range(lim)) {
        out |> push_clone(buf[i])
    }
    return <- out
}

def run_sort_take_array_ref(b : B?) {
    let arr <- fixture_array(N)
    b |> run("R_sort_take_array_ref/{N}", N) {
        let r <- sort_take_array_ref(arr, TAKE_K)
        if (check(r) == 0) { b->failNow() }
    }
}

def run_distinct_order_take_array_ref(b : B?) {
    let arr <- fixture_array(N)
    b |> run("R_distinct_order_take_array_ref/{N}", N) {
        let r <- distinct_order_take_array_ref(arr, TAKE_K)
        if (check(r) == 0) { b->failNow() }
    }
}

[benchmark]
def probe_sort_take_array_R_ref(b : B?) {
    run_sort_take_array_ref(b)
}

[benchmark]
def probe_distinct_order_take_array_R_ref(b : B?) {
    run_distinct_order_take_array_ref(b)
}

// ===== last_match: where(price > THRESHOLD) |> last() — overwrite-last, N matches → ONE survivor =====
// Validates N→1 handle-defer for the last/last_or_default emit (emit_reverse_walk_overwrite_scalar).

let THRESHOLD = 500

// C — overwrite-last clone. from_xml_node materializes every element; `last := c` clones the full Car
// (incl. the name string) on EVERY match. N clones for one survivor — the m5f floor.
def last_match_materialized(de : xml_node) : Car {
    var last = default<Car>
    var found = false
    for (c in from_xml_node(de, type<Car>)) {
        if (c.price > THRESHOLD) {
            last := c
            found = true
        }
    }
    if (!found) { panic("no match") }
    return last
}

// A — overwrite-last handle. Reads only `price` per element; remembers the node; build_xml_row runs once,
// for the single survivor.
def last_match_deferred(de : xml_node) : Car {
    let xdef = default<Car>
    var lastNode : xml_node
    var found = false
    var ch = de.first_child
    while (ch.ok) {
        let cur = ch
        ch = ch.next_sibling
        if (cur._type == xml_node_type.node_element) {
            let p = read_xml_field(cur, "price", xdef.price)
            if (p > THRESHOLD) {
                lastNode = cur
                found = true
            }
        }
    }
    if (!found) { panic("no match") }
    var it = default<Car>
    build_xml_row(lastNode, it)
    return it
}

def run_last_match_C(b : B?) {
    let xml = fixture_xml_string(N)
    parse_xml(xml) $(doc, ok) {
        if (!ok) { b->failNow(); return }
        let de = doc.document_element
        b |> run("C_last_match_materialized/{N}", N) {
            let r = last_match_materialized(de)
            if (empty(r.name)) { b->failNow() }
        }
    }
}

def run_last_match_A(b : B?) {
    let xml = fixture_xml_string(N)
    parse_xml(xml) $(doc, ok) {
        if (!ok) { b->failNow(); return }
        let de = doc.document_element
        b |> run("A_last_match_deferred/{N}", N) {
            let r = last_match_deferred(de)
            if (empty(r.name)) { b->failNow() }
        }
    }
}

[benchmark]
def probe_last_match_C_materialized(b : B?) {
    run_last_match_C(b)
}

[benchmark]
def probe_last_match_A_deferred(b : B?) {
    run_last_match_A(b)
}

// ===== groupby_first: group_by(brand) |> select((brand, first car per group)) — survivors = #brands =====
// The whole row escapes (FirstCar), but only for the #distinct-brands first-seen rows, not all N.

struct GBResult {
    brand : int
    car   : Car
}

// C — full materialize per element (mimics current m5f: whole-row escape → build_xml_row for every
// element), then first-per-group miss-insert. N clones for #brands survivors.
def groupby_first_materialized(de : xml_node) : array<GBResult> {
    var seen : table<int>
    var out : array<GBResult>
    for (c in from_xml_node(de, type<Car>)) {
        if (!key_exists(seen, c.brand)) {
            seen |> insert(c.brand)
            out |> push_clone(GBResult(brand = c.brand, car = c))
        }
    }
    return <- out
}

// A — read only `brand` per element; remember the first node per group; build_xml_row for the #brands
// survivors at the tail.
def groupby_first_deferred(de : xml_node) : array<GBResult> {
    let xdef = default<Car>
    var seen : table<int>
    var firstBrand : array<int>
    var firstNode : array<xml_node>
    var ch = de.first_child
    while (ch.ok) {
        let cur = ch
        ch = ch.next_sibling
        if (cur._type == xml_node_type.node_element) {
            let br = read_xml_field(cur, "brand", xdef.brand)
            if (!key_exists(seen, br)) {
                seen |> insert(br)
                firstBrand |> push(br)
                firstNode |> push(cur)
            }
        }
    }
    var out : array<GBResult>
    out |> reserve(length(firstBrand))
    for (i in range(length(firstBrand))) {
        var it = default<Car>
        build_xml_row(firstNode[i], it)
        out |> push_clone(GBResult(brand = firstBrand[i], car = it))
    }
    return <- out
}

def check_gb(rows : array<GBResult>) : int {
    var acc = 0
    for (r in rows) {
        acc += r.car.id + length(r.car.name)
    }
    return acc
}

def run_groupby_first_C(b : B?) {
    let xml = fixture_xml_string(N)
    parse_xml(xml) $(doc, ok) {
        if (!ok) { b->failNow(); return }
        let de = doc.document_element
        b |> run("C_groupby_first_materialized/{N}", N) {
            let r <- groupby_first_materialized(de)
            if (check_gb(r) == 0) { b->failNow() }
        }
    }
}

def run_groupby_first_A(b : B?) {
    let xml = fixture_xml_string(N)
    parse_xml(xml) $(doc, ok) {
        if (!ok) { b->failNow(); return }
        let de = doc.document_element
        b |> run("A_groupby_first_deferred/{N}", N) {
            let r <- groupby_first_deferred(de)
            if (check_gb(r) == 0) { b->failNow() }
        }
    }
}

[benchmark]
def probe_groupby_first_C_materialized(b : B?) {
    run_groupby_first_C(b)
}

[benchmark]
def probe_groupby_first_A_deferred(b : B?) {
    run_groupby_first_A(b)
}
