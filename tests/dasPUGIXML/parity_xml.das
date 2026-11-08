options gen2

require pugixml/PUGIXML_boost
require daslib/linq_fold
require daslib/linq_boost public
require daslib/strings_boost
require dastest/testing_boost public
require daslib/linq public
require daslib/sort_boost

// XML linq parity — `_where` predicate surface + `_select` projection.
// Each shape runs three ways and must agree:
//   m5f  = _fold over from_xml_node(...)   (fused/cascaded XML source — under test)
//   m3f  = _fold over each(arr)            (array fold)
//   m3   = plain array linq                (ground truth)
// from_xml_node is [unsafe_outside_of_for] and _fold skips the const&->value copy, so the XML
// source is wrapped in unsafe(...) and the node is passed inline by value (doc.document_element);
// a `let`-bound node would be const& and rejected.

struct Car {
    id    : int
    price : float
    qty   : int
    brand : string
}

let XML_NOISY = ("<cars>" +
    "<car id=\"1\" price=\"100.0\" qty=\"2\" brand=\"Ford\"/>noise" +
    "<car id=\"2\" price=\"9999.0\" qty=\"4\" brand=\"Ferrari\"/>" +
    "<car id=\"3\" price=\"200.0\" qty=\"6\" brand=\"Honda\"/>more" +
    "<car id=\"4\" price=\"50.0\" qty=\"8\" brand=\"Ford\"/>" +
    "<car id=\"5\" price=\"88888.0\" qty=\"1\" brand=\"Ferrari\"/>" +
    "<car id=\"6\" price=\"300.0\" qty=\"3\" brand=\"Honda\"/>" +
    "<car id=\"7\" price=\"150.0\" qty=\"5\" brand=\"Ford\"/>" +
    "<car id=\"8\" price=\"2000.0\" qty=\"7\" brand=\"Toyota\"/>" +
    "</cars>")

def fixture_array() : array<Car> {
    return <- [
        Car(id = 1, price = 100.0,   qty = 2, brand = "Ford"),
        Car(id = 2, price = 9999.0,  qty = 4, brand = "Ferrari"),
        Car(id = 3, price = 200.0,   qty = 6, brand = "Honda"),
        Car(id = 4, price = 50.0,    qty = 8, brand = "Ford"),
        Car(id = 5, price = 88888.0, qty = 1, brand = "Ferrari"),
        Car(id = 6, price = 300.0,   qty = 3, brand = "Honda"),
        Car(id = 7, price = 150.0,   qty = 5, brand = "Ford"),
        Car(id = 8, price = 2000.0,  qty = 7, brand = "Toyota")]
}

def cars_equal(a : array<Car>; b : array<Car>) : bool {
    if (length(a) != length(b)) { return false }
    for (ca, cb in a, b) {
        if (ca.id != cb.id || ca.price != cb.price || ca.qty != cb.qty || ca.brand != cb.brand) {
            return false
        }
    }
    return true
}

// run the same to_array-producing chain three ways and assert all three agree.
def check3(t : T?; var m5f, m3f, m3 : array<Car>) {
    t |> success(cars_equal(m5f, m3f), "m5f == m3f")
    t |> success(cars_equal(m3f, m3), "m3f == m3")
    delete m5f; delete m3f; delete m3
}

[test(description = "A reverse-then-take chain over an XML source with text nodes between elements skips the text and gives the same result on all three paths")]
def parity_reverse_backward_text_skip(t : T?) {
    // This fixture interleaves "noise"/"more" text nodes between elements. The BACKWARD DOM walk must
    // skip non-element nodes exactly like the forward walk, so the fused result equals the array fold.
    parse_xml(XML_NOISY) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        // reverse |> take(3) — backward walk over the text-interleaved doc
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>)).reverse().take(3).to_array())
        var m3f <- _fold(each(arr).reverse().take(3).to_array())
        t |> success(cars_equal(m5f, m3f), "reverse|take backward text-skip")
        delete m5f; delete m3f
        // bare last() backward-1-step lands on the last ELEMENT (id 8), not the trailing text
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>)).last()).id,
                   _fold(each(arr).last()).id)
        // reverse |> first = last element
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>)).reverse().first()).id,
                   _fold(each(arr).reverse().first()).id)
    }
}

[test(description = "Numeric where predicates over an XML source give the same result in the fused XML fold, the array fold and plain linq")]
def parity_where_numeric(t : T?) {
    parse_xml(XML_NOISY) $(doc, ok) {
        t |> success(ok)
        var arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.price > 199.0).to_array())
        var m3f <- _fold(each(arr)._where(_.price > 199.0).to_array())
        var m3  <- (arr |> _where(_.price > 199.0))
        check3(t, m5f, m3f, m3)
    }
}

[test(description = "A where mixing an or with a string predicate over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_where_or_with_string(t : T?) {
    parse_xml(XML_NOISY) $(doc, ok) {
        t |> success(ok)
        var arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.price < 100.0 || (_.brand |> starts_with("Fe"))).to_array())
        var m3f <- _fold(each(arr)._where(_.price < 100.0 || (_.brand |> starts_with("Fe"))).to_array())
        var m3  <- (arr |> _where(_.price < 100.0 || (_.brand |> starts_with("Fe"))))
        check3(t, m5f, m3f, m3)
    }
}

[test(description = "String predicates in where over an XML source give the same result in the fused XML fold, the array fold and plain linq")]
def parity_where_string_predicates(t : T?) {
    parse_xml(XML_NOISY) $(doc, ok) {
        t |> success(ok)
        var arr <- fixture_array()
        // ends_with
        var e5 <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.brand |> ends_with("da")).to_array())
        var ef <- _fold(each(arr)._where(_.brand |> ends_with("da")).to_array())
        var em <- (arr |> _where(_.brand |> ends_with("da")))
        check3(t, e5, ef, em)
        // contains
        var c5 <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.brand |> contains("o")).to_array())
        var cf <- _fold(each(arr)._where(_.brand |> contains("o")).to_array())
        var cm <- (arr |> _where(_.brand |> contains("o")))
        check3(t, c5, cf, cm)
        // to_lower ==
        var l5 <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.brand |> to_lower() == "ford").to_array())
        var lf <- _fold(each(arr)._where(_.brand |> to_lower() == "ford").to_array())
        var lm <- (arr |> _where(_.brand |> to_lower() == "ford"))
        check3(t, l5, lf, lm)
    }
}

[test(description = "Length and arithmetic inside a where over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_where_length_and_arith(t : T?) {
    parse_xml(XML_NOISY) $(doc, ok) {
        t |> success(ok)
        var arr <- fixture_array()
        // length(string)
        var n5 <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(length(_.brand) > 5).to_array())
        var nf <- _fold(each(arr)._where(length(_.brand) > 5).to_array())
        var nm <- (arr |> _where(length(_.brand) > 5))
        check3(t, n5, nf, nm)
        // arithmetic on a column
        var a5 <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.price * 2.0 > 400.0).to_array())
        var af <- _fold(each(arr)._where(_.price * 2.0 > 400.0).to_array())
        var am <- (arr |> _where(_.price * 2.0 > 400.0))
        check3(t, a5, af, am)
    }
}

[test(description = "A select projection summed over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_select_projection_sum(t : T?) {
    parse_xml(XML_NOISY) $(doc, ok) {
        t |> success(ok)
        var arr <- fixture_array()
        let s5 = _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._select(_.qty)._where(_ > 3).sum())
        let sf = _fold(each(arr)._select(_.qty)._where(_ > 3).sum())
        let sm = (arr._select(_.qty)._where(_ > 3).sum())
        t |> equal(s5, sf)
        t |> equal(sf, sm)
    }
}

[test(description = "Select with where collected to an array over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_select_where_to_array(t : T?) {
    parse_xml(XML_NOISY) $(doc, ok) {
        t |> success(ok)
        var arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.qty > 4)._where(_.price < 1000.0).to_array())
        var m3f <- _fold(each(arr)._where(_.qty > 4)._where(_.price < 1000.0).to_array())
        var m3  <- (arr |> _where(_.qty > 4) |> _where(_.price < 1000.0))
        check3(t, m5f, m3f, m3)
    }
}

// XML linq-join parity — `join` (and `join |> group_by`) over a from_xml_node source. The join fuses via
// XmlAdapter.emit_join_hook (srcB collected into a hash, srcA probed from the field-pruned DOM walk);
// `join |> group_by` fuses via XmlJoinAdapter (returned from build_group_by_adapter on the upstream-join arm).
// This file proves the fused XML result equals the array-fold result — the correctness gate for the fusion.
//   m5f = _fold over from_xml_node(...)   m3f = _fold over each(arr)

struct BrandHQ {
    brand   : string
    country : string
}

let XML = ("<cars>" +
    "<car id=\"1\" price=\"100.0\" qty=\"2\" brand=\"Ford\"/>" +
    "<car id=\"2\" price=\"9999.0\" qty=\"4\" brand=\"Ferrari\"/>" +
    "<car id=\"3\" price=\"200.0\" qty=\"6\" brand=\"Honda\"/>" +
    "<car id=\"4\" price=\"50.0\" qty=\"8\" brand=\"Ford\"/>" +
    "<car id=\"5\" price=\"88888.0\" qty=\"1\" brand=\"Ferrari\"/>" +
    "<car id=\"6\" price=\"300.0\" qty=\"3\" brand=\"Honda\"/>" +
    "<car id=\"7\" price=\"150.0\" qty=\"5\" brand=\"Ford\"/>" +
    "<car id=\"8\" price=\"2000.0\" qty=\"7\" brand=\"Toyota\"/>" +
    "</cars>")

def brand_array() : array<BrandHQ> {
    return <- [
        BrandHQ(brand = "Ford",    country = "USA"),
        BrandHQ(brand = "Ferrari", country = "Italy"),
        BrandHQ(brand = "Honda",   country = "Japan"),
        BrandHQ(brand = "Toyota",  country = "Japan")]
}

// Sum of (id, country)-by-country maps must agree between XML fold and array fold.
def country_id_sums(rows : array<tuple<id : int; country : string>>) : table<string; int> {
    var m : table<string; int>   // nolint:STYLE027 accumulation (running sum per key), not a simple insert — no comprehension form
    for (r in rows) {
        m[r.country] = (m?[r.country] ?? 0) + r.id
    }
    return <- m
}

def tables_equal(a : table<string; int>; b : table<string; int>) : bool {
    if (length(a) != length(b)) { return false }
    for (k, v in keys(a), values(a)) {
        if ((b?[k] ?? -1) != v) { return false }
    }
    return true
}

[test(description = "A join count between an XML source and an array gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_join_count(t : T?) {
    // join + count (no where) — bucket-length fast path. Every car's brand matches exactly one HQ → 8.
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        let hqs <- brand_array()
        let m5f = _fold(unsafe(from_xml_node(doc.document_element, type<Car>)) |> _join(hqs,
                            $(c : Car, h : BrandHQ) => c.brand == h.brand,
                            $(c : Car, h : BrandHQ) => (id = c.id, country = h.country))
                       |> count())
        let m3f = _fold(each(arr) |> _join(hqs,
                            $(c : Car, h : BrandHQ) => c.brand == h.brand,
                            $(c : Car, h : BrandHQ) => (id = c.id, country = h.country))
                       |> count())
        t |> equal(m5f, 8)
        t |> equal(m5f, m3f)
    }
}

[test(description = "A join collected to an array with a country-to-id-sum table over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_join_to_array(t : T?) {
    // join + to_array (named-tuple projection) — compare (country → sum id) maps.
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        let hqs <- brand_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>)) |> _join(hqs,
                            $(c : Car, h : BrandHQ) => c.brand == h.brand,
                            $(c : Car, h : BrandHQ) => (id = c.id, country = h.country))
                       |> to_array())
        var m3f <- _fold(each(arr) |> _join(hqs,
                            $(c : Car, h : BrandHQ) => c.brand == h.brand,
                            $(c : Car, h : BrandHQ) => (id = c.id, country = h.country))
                       |> to_array())
        t |> equal(length(m5f), length(m3f))
        var s5 <- country_id_sums(m5f)
        var sf <- country_id_sums(m3f)
        t |> success(tables_equal(s5, sf), "country→sum(id) m5f == m3f")
        delete m5f; delete m3f; delete s5; delete sf
    }
}

[test(description = "A join with a where counted over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_join_where_count(t : T?) {
    // join + where + count — predicate fires per pair; only USA (Ford ×3) survives.
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        let hqs <- brand_array()
        let m5f = _fold(unsafe(from_xml_node(doc.document_element, type<Car>)) |> _join(hqs,
                            $(c : Car, h : BrandHQ) => c.brand == h.brand,
                            $(c : Car, h : BrandHQ) => (id = c.id, country = h.country))
                       |> _where(_.country == "USA")
                       |> count())
        let m3f = _fold(each(arr) |> _join(hqs,
                            $(c : Car, h : BrandHQ) => c.brand == h.brand,
                            $(c : Car, h : BrandHQ) => (id = c.id, country = h.country))
                       |> _where(_.country == "USA")
                       |> count())
        t |> equal(m5f, 3)
        t |> equal(m5f, m3f)
    }
}

[test(description = "A join with a select over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_join_select(t : T?) {
    // join + bare _select — sum the projected ids.
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        let hqs <- brand_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>)) |> _join(hqs,
                            $(c : Car, h : BrandHQ) => c.brand == h.brand,
                            $(c : Car, h : BrandHQ) => (id = c.id, country = h.country))
                       |> _select(_.id)
                       |> to_array())
        var m3f <- _fold(each(arr) |> _join(hqs,
                            $(c : Car, h : BrandHQ) => c.brand == h.brand,
                            $(c : Car, h : BrandHQ) => (id = c.id, country = h.country))
                       |> _select(_.id)
                       |> to_array())
        t |> equal(length(m5f), 8)
        var sum5 = 0
        var sumf = 0
        for (x in m5f) { sum5 += x }
        for (x in m3f) { sumf += x }
        t |> equal(sum5, 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8)
        t |> equal(sum5, sumf)
        delete m5f; delete m3f
    }
}

[test(description = "A join with a select and no terminal call over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_join_select_no_terminator(t : T?) {
    // join + bare _select with NO to_array → iterator-typed result. Previously bailed to tier-2;
    // now fuses via the fused-materialize path (build result array, wrap as sequence). Correctness
    // gate: the fused sequence equals the eager array lane.
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        let hqs <- brand_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>)) |> _join(hqs,
                            $(c : Car, h : BrandHQ) => c.brand == h.brand,
                            $(c : Car, h : BrandHQ) => (id = c.id, country = h.country))
                       |> _select(_.id))
        var m3f <- _fold(arr |> _join(hqs,
                            $(c : Car, h : BrandHQ) => c.brand == h.brand,
                            $(c : Car, h : BrandHQ) => (id = c.id, country = h.country))
                       |> _select(_.id))
        var sum5 = 0
        var cnt5 = 0
        for (x in m5f) {
            sum5 += x
            cnt5 ++
        }
        var sumf = 0
        for (x in m3f) {
            sumf += x
        }
        t |> equal(cnt5, 8)
        t |> equal(sum5, 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8)
        t |> equal(sum5, sumf)
        delete m5f
        delete m3f
    }
}

[test(description = "A join followed by group by and sum over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_join_group_by_sum(t : T?) {
    // join |> group_by(country) |> select(sum qty) — group-by-after-join (XmlJoinAdapter). Compare country→sum(qty).
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        let hqs <- brand_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>)) |> _join(hqs,
                            $(c : Car, h : BrandHQ) => c.brand == h.brand,
                            $(c : Car, h : BrandHQ) => (country = h.country, qty = c.qty))
                       |> _group_by(_.country)
                       |> _select((c = _._0, total = _._1 |> _select(_.qty) |> sum()))
                       |> to_array())
        var m3f <- _fold(each(arr) |> _join(hqs,
                            $(c : Car, h : BrandHQ) => c.brand == h.brand,
                            $(c : Car, h : BrandHQ) => (country = h.country, qty = c.qty))
                       |> _group_by(_.country)
                       |> _select((c = _._0, total = _._1 |> _select(_.qty) |> sum()))
                       |> to_array())
        t |> equal(length(m5f), length(m3f))
        let s5 <- { for (g in m5f); g.c => g.total }
        let sf <- { for (g in m3f); g.c => g.total }
        t |> success(tables_equal(s5, sf), "country→sum(qty) m5f == m3f")
        delete m5f; delete m3f
    }
}

[test(description = "Join and group-by buckets on edge-case data over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_join_edge_buckets(t : T?) {
    // Inner-join semantics the 1:1 fixture above never hits: an unmatched srcA row (Tesla has no HQ → 0 rows)
    // and a multi-row srcB bucket (Ford has two HQ rows → each Ford car fans out ×2). Both exercise the
    // XmlAdapter hash-probe branches directly. Hand-computed inner join: Ford 2cars ×2HQ = 4, Honda 1×1 = 1,
    // Tesla 0 → 5 rows; country→sum(id) = {USA:3, Canada:3, Japan:4}.
    let xml = ("<cars>" +
        "<car id=\"1\" price=\"100.0\" qty=\"2\" brand=\"Ford\"/>" +
        "<car id=\"2\" price=\"200.0\" qty=\"3\" brand=\"Ford\"/>" +
        "<car id=\"3\" price=\"300.0\" qty=\"4\" brand=\"Tesla\"/>" +
        "<car id=\"4\" price=\"400.0\" qty=\"5\" brand=\"Honda\"/>" +
        "</cars>")
    parse_xml(xml) $(doc, ok) {
        t |> success(ok)
        let arr <- [
            Car(id = 1, price = 100.0, qty = 2, brand = "Ford"),
            Car(id = 2, price = 200.0, qty = 3, brand = "Ford"),
            Car(id = 3, price = 300.0, qty = 4, brand = "Tesla"),
            Car(id = 4, price = 400.0, qty = 5, brand = "Honda")]
        let hqs <- [
            BrandHQ(brand = "Ford",  country = "USA"),
            BrandHQ(brand = "Ford",  country = "Canada"),   // duplicate key → multi-row bucket → fan-out
            BrandHQ(brand = "Honda", country = "Japan")]    // no Tesla → unmatched srcA row
        // count fast-path over a length-2 bucket + an unmatched row.
        let c5 = _fold(unsafe(from_xml_node(doc.document_element, type<Car>)) |> _join(hqs,
                            $(c : Car, h : BrandHQ) => c.brand == h.brand,
                            $(c : Car, h : BrandHQ) => (id = c.id, country = h.country))
                       |> count())
        let cf = _fold(each(arr) |> _join(hqs,
                            $(c : Car, h : BrandHQ) => c.brand == h.brand,
                            $(c : Car, h : BrandHQ) => (id = c.id, country = h.country))
                       |> count())
        t |> equal(c5, 5)
        t |> equal(c5, cf)
        // materializing path — country→sum(id) must agree across the fan-out + unmatched row.
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>)) |> _join(hqs,
                            $(c : Car, h : BrandHQ) => c.brand == h.brand,
                            $(c : Car, h : BrandHQ) => (id = c.id, country = h.country))
                       |> to_array())
        var m3f <- _fold(each(arr) |> _join(hqs,
                            $(c : Car, h : BrandHQ) => c.brand == h.brand,
                            $(c : Car, h : BrandHQ) => (id = c.id, country = h.country))
                       |> to_array())
        t |> equal(length(m5f), 5)
        var s5 <- country_id_sums(m5f)
        var sf <- country_id_sums(m3f)
        t |> success(tables_equal(s5, sf), "edge country→sum(id) m5f == m3f")
        delete m5f; delete m3f; delete s5; delete sf
    }
}

// XML linq parity — binary two-source ops (join family + set-ops) where the XML source is one
// operand and an in-memory array is the other. This is the proof of the mixed-source overloads
// added to daslib/linq.das: from_xml_node yields an iterator, so `cars |> _join(dealersArray, ...)`
// and `brands_iter |> union(brandArray)` exercise the new (iterator, array) overloads directly.
//   m5f = XML iterator + array   m3f = each(arr) iterator + array   m3 = all-array

struct Dealer {
    brand : string
    city  : string
}

def fixture_dealers() : array<Dealer> {
    return <- [
        Dealer(brand = "Ford",    city = "Detroit"),
        Dealer(brand = "Ferrari", city = "Maranello"),
        Dealer(brand = "Honda",   city = "Tokyo")]
}

// compare two string multisets ignoring order (set-op output order is not contractual)
def multiset_equal(var a, b : array<string>) : bool {
    if (length(a) != length(b)) { return false }
    sort(a)
    sort(b)
    for (sa, sb in a, b) {
        if (sa != sb) { return false }
    }
    return true
}

[test(description = "A join of an XML source with an array gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_join_with_array(t : T?) {
    // from_xml_node(cars) |> _join(dealersArray, ...) uses the mixed (iterator, array) join overload.
    // Inner join on brand: Toyota has no dealer, so id 8 drops. Matched ids: 1,2,3,4,5,6,7 (sum 28).
    // Plain linq (no _fold): join is a cascade op, not a loop_or_count fusion — the mixed overload is
    // exercised directly. (_fold's array-rewrite of a binary op needs *_to_array mixed forms; that is
    // a source-agnostic _fold limitation — each(arr)|>_join(array) hits it identically — out of scope.)
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        let dealers <- fixture_dealers()
        let c5 = (unsafe(from_xml_node(doc.document_element, type<Car>)) |> _join(dealers, $(c : Car, d : Dealer) => c.brand == d.brand, $(c : Car, _d : Dealer) => c.id) |> count())
        let cf = (unsafe(each(arr)) |> _join(dealers, $(c : Car, d : Dealer) => c.brand == d.brand, $(c : Car, _d : Dealer) => c.id) |> count())
        let cm = (arr |> _join(dealers, $(c : Car, d : Dealer) => c.brand == d.brand, $(c : Car, _d : Dealer) => c.id) |> count())
        t |> equal(c5, cf)
        t |> equal(cf, cm)
        t |> equal(c5, 7)
        let s5 = (unsafe(from_xml_node(doc.document_element, type<Car>)) |> _join(dealers, $(c : Car, d : Dealer) => c.brand == d.brand, $(c : Car, _d : Dealer) => c.id) |> sum())
        let sf = (unsafe(each(arr)) |> _join(dealers, $(c : Car, d : Dealer) => c.brand == d.brand, $(c : Car, _d : Dealer) => c.id) |> sum())
        t |> equal(s5, sf)
        t |> equal(s5, 28)
    }
}

[test(description = "A union of XML brands with an array gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_union_with_array(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        let extra <- ["Ford", "Lada"]
        var m5f <- (unsafe(from_xml_node(doc.document_element, type<Car>))._select(_.brand) |> union(extra) |> to_array())
        var m3f <- (unsafe(each(arr))._select(_.brand) |> union(extra) |> to_array())
        t |> success(multiset_equal(m5f, m3f), "union brands m5f == m3f")
        delete m5f; delete m3f
    }
}

[test(description = "An intersect of XML brands with an array gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_intersect_with_array(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        let only <- ["Ford", "Honda"]
        var m5f <- (unsafe(from_xml_node(doc.document_element, type<Car>))._select(_.brand) |> intersect(only) |> to_array())
        var m3f <- (unsafe(each(arr))._select(_.brand) |> intersect(only) |> to_array())
        t |> success(multiset_equal(m5f, m3f), "intersect brands m5f == m3f")
        delete m5f; delete m3f
    }
}

[test(description = "An except of XML brands with an array gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_except_with_array(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        let drop <- ["Ford"]
        var m5f <- (unsafe(from_xml_node(doc.document_element, type<Car>))._select(_.brand) |> except(drop) |> to_array())
        var m3f <- (unsafe(each(arr))._select(_.brand) |> except(drop) |> to_array())
        t |> success(multiset_equal(m5f, m3f), "except brands m5f == m3f")
        delete m5f; delete m3f
    }
}

[test(description = "A concat of XML brands with an array gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_concat_with_array(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        let more <- ["Lada", "Saab"]
        var m5f <- (unsafe(from_xml_node(doc.document_element, type<Car>))._select(_.brand) |> concat(more) |> to_array())
        var m3f <- (unsafe(each(arr))._select(_.brand) |> concat(more) |> to_array())
        t |> success(multiset_equal(m5f, m3f), "concat brands m5f == m3f")
        delete m5f; delete m3f
    }
}

[test(description = "Sequence_equal between an XML source and an array gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_sequence_equal_with_array(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        let ids <- [for (c in arr); c.id]
        // XML id iterator vs the array of ids — equal in order
        t |> success(sequence_equal(unsafe(from_xml_node(doc.document_element, type<Car>))._select(_.id), ids),
                     "XML id sequence equals array of ids")
    }
}

// XML linq parity — the cascade families (order_by / distinct / distinct_by / group_by / reverse,
// plus distinct+take, group_by+reduction, order+take). These now FUSE over the XmlAdapter (the cascade
// arms ride wrap_source_loop / build_group_by_adapter / emit_distinct_take_loop). This file proves the
// fused XML result equals the array lanes — the correctness gate for the fusion.
//   m5f = _fold over from_xml_node(...)   m3f = _fold over each(arr)   m3 = plain array linq

def strings_equal(a : array<string>; b : array<string>) : bool {
    if (length(a) != length(b)) { return false }
    for (sa, sb in a, b) {
        if (sa != sb) { return false }
    }
    return true
}

[test(description = "Order_by over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_order_by(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._order_by(_.price).to_array())
        var m3f <- _fold(each(arr)._order_by(_.price).to_array())
        var m3  <- (arr |> _order_by(_.price))
        check3(t, m5f, m3f, m3)
    }
}

[test(description = "Order_by_descending over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_order_by_descending(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._order_by_descending(_.price).to_array())
        var m3f <- _fold(each(arr)._order_by_descending(_.price).to_array())
        var m3  <- (arr |> _order_by_descending(_.price))
        check3(t, m5f, m3f, m3)
    }
}

[test(description = "Distinct over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_distinct(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._select(_.brand).distinct().to_array())
        var m3f <- _fold(each(arr)._select(_.brand).distinct().to_array())
        t |> success(strings_equal(m5f, m3f), "distinct brands m5f == m3f")
        delete m5f; delete m3f
    }
}

[test(description = "Distinct_by over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_distinct_by(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._distinct_by(_.brand).to_array())
        var m3f <- _fold(each(arr)._distinct_by(_.brand).to_array())
        var m3  <- (arr |> _distinct_by(_.brand))
        check3(t, m5f, m3f, m3)
    }
}

[test(description = "Reverse over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_reverse(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.price > 0.0).reverse().to_array())
        var m3f <- _fold(each(arr)._where(_.price > 0.0).reverse().to_array())
        var m3  <- (arr |> _where(_.price > 0.0) |> reverse())
        check3(t, m5f, m3f, m3)
    }
}

[test(description = "Reverse followed by distinct_by over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_reverse_distinct_by(t : T?) {
    // reverse + distinct_by: keep LAST per brand, output in reverse-discovery order. m5f fuses via the
    // FORWARD keep-last table-overwrite (R-2b, deferred handles); m3f rides the array backward-index walk.
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>)).reverse()._distinct_by(_.brand).to_array())
        var m3f <- _fold(each(arr).reverse()._distinct_by(_.brand).to_array())
        var m3  <- (arr |> reverse() |> _distinct_by(_.brand))
        check3(t, m5f, m3f, m3)
    }
}

[test(description = "Reverse followed by take over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_reverse_take(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>)).reverse().take(3).to_array())
        var m3f <- _fold(each(arr).reverse().take(3).to_array())
        var m3  <- (arr |> reverse() |> take(3))
        check3(t, m5f, m3f, m3)
    }
}

[test(description = "Reverse, take and select over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_reverse_take_select(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>)).reverse().take(3)._select(_.brand).to_array())
        var m3f <- _fold(each(arr).reverse().take(3)._select(_.brand).to_array())
        t |> success(strings_equal(m5f, m3f), "reverse|take|select m5f == m3f")
        delete m5f; delete m3f
    }
}

[test(description = "Reverse followed by take(0) and take beyond the length over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_reverse_take_edges(t : T?) {
    // backward-walk edge cases: take(0) -> empty; take(N > element count) -> all, reversed.
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var a5 <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>)).reverse().take(0).to_array())
        var a3 <- _fold(each(arr).reverse().take(0).to_array())
        t |> equal(length(a5), 0)
        t |> success(cars_equal(a5, a3), "reverse|take(0)")
        delete a5; delete a3
        var b5 <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>)).reverse().take(99).to_array())
        var b3 <- _fold(each(arr).reverse().take(99).to_array())
        t |> equal(length(b5), 8)
        t |> success(cars_equal(b5, b3), "reverse|take(99)")
        delete b5; delete b3
    }
}

[test(description = "Reverse followed by take through the named from_xml_node overload gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_reverse_take_named_fallback(t : T?) {
    // The named 3-arg from_xml_node has no last-named-child primitive, so reverse|>take falls back to the
    // buffer-all path (not the backward walk) — it must still produce the correct (last-3-reversed) result.
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, "car", type<Car>)).reverse().take(3).to_array())
        var m3f <- _fold(each(arr).reverse().take(3).to_array())
        t |> success(cars_equal(m5f, m3f), "named reverse|take fallback")
        delete m5f; delete m3f
    }
}

[test(description = "Distinct_by followed by take over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_distinct_by_take(t : T?) {
    // distinct+take fuses via XmlAdapter.emit_distinct_take_loop (early-exit after N distinct).
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._distinct_by(_.brand).take(2).to_array())
        var m3f <- _fold(each(arr)._distinct_by(_.brand).take(2).to_array())
        var m3  <- (arr |> _distinct_by(_.brand) |> take(2))
        check3(t, m5f, m3f, m3)
    }
}

[test(description = "Order_by followed by take over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_order_by_take(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._order_by(_.price).take(3).to_array())
        var m3f <- _fold(each(arr)._order_by(_.price).take(3).to_array())
        var m3  <- (arr |> _order_by(_.price) |> take(3))
        check3(t, m5f, m3f, m3)
    }
}

// ===== Deferred-materialization (handle-buffering) bounded-heap variants — see daslib/ARCHITECTURE_LINQ_FOLD.md, the deferred materialization section =====

[test(description = "Order_by_descending followed by take over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_order_by_descending_take(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._order_by_descending(_.price).take(3).to_array())
        var m3f <- _fold(each(arr)._order_by_descending(_.price).take(3).to_array())
        var m3  <- (arr |> _order_by_descending(_.price) |> take(3))
        check3(t, m5f, m3f, m3)
    }
}

[test(description = "Distinct_by, order_by and take over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_distinct_by_order_take(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._distinct_by(_.brand)._order_by(_.price).take(3).to_array())
        var m3f <- _fold(each(arr)._distinct_by(_.brand)._order_by(_.price).take(3).to_array())
        var m3  <- (arr |> _distinct_by(_.brand) |> _order_by(_.price) |> take(3))
        check3(t, m5f, m3f, m3)
    }
}

[test(description = "Where, order_by and take over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_where_order_take(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.price > 100.0)._order_by(_.price).take(3).to_array())
        var m3f <- _fold(each(arr)._where(_.price > 100.0)._order_by(_.price).take(3).to_array())
        var m3  <- (arr |> _where(_.price > 100.0) |> _order_by(_.price) |> take(3))
        check3(t, m5f, m3f, m3)
    }
}

[test(description = "Distinct_by and order_by collected to an array over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_distinct_by_order_to_array(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._distinct_by(_.brand)._order_by(_.price).to_array())
        var m3f <- _fold(each(arr)._distinct_by(_.brand)._order_by(_.price).to_array())
        var m3  <- (arr |> _distinct_by(_.brand) |> _order_by(_.price))
        check3(t, m5f, m3f, m3)
    }
}

[test(description = "Where followed by order_by over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_where_order(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.price > 100.0)._order_by(_.price).to_array())
        var m3f <- _fold(each(arr)._where(_.price > 100.0)._order_by(_.price).to_array())
        var m3  <- (arr |> _where(_.price > 100.0) |> _order_by(_.price))
        check3(t, m5f, m3f, m3)
    }
}

[test(description = "Order_by followed by first over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_order_by_first(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        let m5f = _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._order_by(_.price).first())
        let m3f = _fold(each(arr)._order_by(_.price).first())
        t |> success(m5f.id == m3f.id && m5f.price == m3f.price && m5f.brand == m3f.brand, "order|first m5f == m3f")
    }
}

[test(description = "Order_by_descending followed by first over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_order_by_descending_first(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        let m5f = _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._order_by_descending(_.price).first())
        let m3f = _fold(each(arr)._order_by_descending(_.price).first())
        t |> success(m5f.id == m3f.id && m5f.price == m3f.price && m5f.brand == m3f.brand, "order_desc|first m5f == m3f")
    }
}

[test(description = "Select, order, distinct and take over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_select_order_distinct_take(t : T?) {
    // Leading `_select(_.brand)` projects the source, then order |> distinct |> take. Fuses via
    // ProjectedSourceAdapter (the srcsel slot) — single-pass dedup-walk + bounded top-N, field-pruned to brand.
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._select(_.brand)._order_by(_).distinct().take(2).to_array())
        var m3f <- _fold(each(arr)._select(_.brand)._order_by(_).distinct().take(2).to_array())
        t |> success(strings_equal(m5f, m3f), "select|order|distinct|take m5f == m3f")
        t |> equal(length(m5f), 2)
        t |> equal(m5f[0], "Ferrari")   // distinct sorted brands: Ferrari, Ford, Honda, Toyota → take 2
        t |> equal(m5f[1], "Ford")
        delete m5f; delete m3f
    }
}

[test(description = "Select, order and take over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_select_order_take(t : T?) {
    // Leading `_select` + order + take (no distinct) — bounded heap over projected brands.
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._select(_.brand)._order_by(_).take(3).to_array())
        var m3f <- _fold(each(arr)._select(_.brand)._order_by(_).take(3).to_array())
        t |> success(strings_equal(m5f, m3f), "select|order|take m5f == m3f")
        t |> equal(length(m5f), 3)
        delete m5f; delete m3f
    }
}

[test(description = "Group_by with a sum over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_group_by_sum(t : T?) {
    // group_by + per-group reduction (sum) — the field-pruning win shape (reads only brand + qty).
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._group_by(_.brand)._select((b = _._0, total = _._1 |> _select(_.qty) |> sum())).to_array())
        var m3f <- _fold(each(arr)._group_by(_.brand)._select((b = _._0, total = _._1 |> _select(_.qty) |> sum())).to_array())
        t |> equal(length(m5f), length(m3f))
        var s5 <- { for (g in m5f); g.b => g.total }
        let sf <- { for (g in m3f); g.b => g.total }
        t |> equal(length(s5), length(sf))
        for (k, v in keys(s5), values(s5)) {
            t |> equal(v, sf?[k] ?? -1)
        }
        delete m5f; delete m3f
    }
}

[test(description = "Group_by over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_group_by(t : T?) {
    // group_by emits a per-key bucket; assert XML fold matches array fold by (brand,count) pairs.
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._group_by(_.brand)._select((b = _._0, n = _._1 |> count())).to_array())
        var m3f <- _fold(each(arr)._group_by(_.brand)._select((b = _._0, n = _._1 |> count())).to_array())
        t |> equal(length(m5f), length(m3f))
        // build brand->count maps and compare (group order is not guaranteed)
        var c5 <- { for (g in m5f); g.b => g.n }
        let cf <- { for (g in m3f); g.b => g.n }
        t |> equal(length(c5), length(cf))
        for (k, v in keys(c5), values(c5)) {
            t |> equal(v, cf?[k] ?? -1)
        }
        delete m5f; delete m3f
    }
}

[test(description = "Group_by followed by first over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_group_by_first(t : T?) {
    // group_by + first-per-group — the deferred-materialization shape: the whole row escapes (FirstCar) but
    // only #brands first-seen rows survive. m5f builds each survivor from the node handle in the miss-branch.
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._group_by(_.brand)._select((b = _._0, first = _._1 |> first())).to_array())
        var m3f <- _fold(each(arr)._group_by(_.brand)._select((b = _._0, first = _._1 |> first())).to_array())
        t |> equal(length(m5f), length(m3f))
        var f5, ff : table<string; int>
        for (g in m5f) {
            f5[g.b] = g.first.id
            t |> equal(g.first.brand, g.b)   // materialized survivor's own brand must match its group key
        }
        for (g in m3f) { ff[g.b] = g.first.id }
        t |> equal(length(f5), length(ff))
        for (k, v in keys(f5), values(f5)) {
            t |> equal(v, ff?[k] ?? -1)
        }
        delete m5f; delete m3f
    }
}

// XML linq parity — partitioning (take/skip/take_while/skip_while) and their combinations with
// where/aggregate. Each runs three ways (m5f XML fold, m3f array fold, m3 plain array linq).

[test(description = "Take and skip over an XML source give the same result in the fused XML fold, the array fold and plain linq")]
def parity_take_skip(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        var arr <- fixture_array()
        var tk5 <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>)).take(3).to_array())
        var tkf <- _fold(each(arr).take(3).to_array())
        var tkm <- (arr |> take(3))
        check3(t, tk5, tkf, tkm)
        var sk5 <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>)).skip(5).to_array())
        var skf <- _fold(each(arr).skip(5).to_array())
        var skm <- (arr |> skip(5))
        check3(t, sk5, skf, skm)
    }
}

[test(description = "Take_while and skip_while over an XML source give the same result in the fused XML fold, the array fold and plain linq")]
def parity_take_while_skip_while(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        var arr <- fixture_array()
        // take while price < 1000 (stops at id 2)
        var tw5 <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._take_while(_.price < 1000.0).to_array())
        var twf <- _fold(each(arr)._take_while(_.price < 1000.0).to_array())
        var twm <- (arr |> _take_while(_.price < 1000.0))
        check3(t, tw5, twf, twm)
        var sw5 <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._skip_while(_.price < 1000.0).to_array())
        var swf <- _fold(each(arr)._skip_while(_.price < 1000.0).to_array())
        var swm <- (arr |> _skip_while(_.price < 1000.0))
        check3(t, sw5, swf, swm)
    }
}

[test(description = "Take followed by where over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_take_then_where(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        var arr <- fixture_array()
        // take the first 5, then keep qty > 3
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>)).take(5)._where(_.qty > 3).to_array())
        var m3f <- _fold(each(arr).take(5)._where(_.qty > 3).to_array())
        var m3  <- (arr |> take(5) |> _where(_.qty > 3))
        check3(t, m5f, m3f, m3)
    }
}

[test(description = "Where, take and count over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_where_then_take_count(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        var arr <- fixture_array()
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.price > 100.0).take(2).count()),
                   _fold(each(arr)._where(_.price > 100.0).take(2).count()))
    }
}

[test(description = "Take followed by sum over an XML source gives the same result in the fused XML fold, the array fold and plain linq")]
def parity_take_then_sum(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        var arr <- fixture_array()
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>))._select(_.qty).take(4).sum()),
                   _fold(each(arr)._select(_.qty).take(4).sum()))
    }
}
