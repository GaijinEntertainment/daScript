options gen2

require pugixml/PUGIXML_boost
require daslib/linq_fold
require daslib/linq_boost public
require dastest/testing_boost public

// XML linq-join parity — `join` (and `join |> group_by`) over a from_xml_node source. The join fuses via
// XmlAdapter.emit_join_hook (srcB collected into a hash, srcA probed from the field-pruned DOM walk);
// `join |> group_by` fuses via XmlJoinAdapter (returned from build_group_by_adapter on the upstream-join arm).
// This file proves the fused XML result equals the array-fold result — the correctness gate for the fusion.
//   m5f = _fold over from_xml_node(...)   m3f = _fold over each(arr)

struct Car {
    id    : int
    price : float
    qty   : int
    brand : string
}

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

[test]
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

[test]
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

[test]
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

[test]
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

[test]
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
        var s5 : table<string; int>
        var sf : table<string; int>
        for (g in m5f) { s5[g.c] = g.total }
        for (g in m3f) { sf[g.c] = g.total }
        t |> success(tables_equal(s5, sf), "country→sum(qty) m5f == m3f")
        delete m5f; delete m3f
    }
}

[test]
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
