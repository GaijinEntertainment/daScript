options gen2

require pugixml/PUGIXML_boost
require daslib/linq_fold
require daslib/linq_boost public
require dastest/testing_boost public

// XML linq parity — the cascade families (order_by / distinct / distinct_by / group_by / reverse).
// These are NOT fused by the XmlAdapter (loop_or_count only); they fall to tier-2 over the XML
// iterator source. This file proves the correct-but-unfused result still equals the array lanes.
//   m5f = _fold over from_xml_node(...)   m3f = _fold over each(arr)   m3 = plain array linq

struct Car {
    id    : int
    price : float
    qty   : int
    brand : string
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

def cars_equal(a : array<Car>; b : array<Car>) : bool {
    if (length(a) != length(b)) { return false }
    for (i in range(length(a))) {
        if (a[i].id != b[i].id || a[i].price != b[i].price || a[i].qty != b[i].qty || a[i].brand != b[i].brand) {
            return false
        }
    }
    return true
}
def check3(t : T?; var m5f, m3f, m3 : array<Car>) {
    t |> success(cars_equal(m5f, m3f), "m5f == m3f")
    t |> success(cars_equal(m3f, m3), "m3f == m3")
    delete m5f; delete m3f; delete m3
}

def strings_equal(a : array<string>; b : array<string>) : bool {
    if (length(a) != length(b)) { return false }
    for (i in range(length(a))) {
        if (a[i] != b[i]) { return false }
    }
    return true
}

[test]
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

[test]
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

[test]
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

[test]
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

[test]
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

[test]
def parity_group_by(t : T?) {
    // group_by emits a per-key bucket; assert XML fold matches array fold by (brand,count) pairs.
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._group_by(_.brand)._select((b = _._0, n = _._1 |> count())).to_array())
        var m3f <- _fold(each(arr)._group_by(_.brand)._select((b = _._0, n = _._1 |> count())).to_array())
        t |> equal(length(m5f), length(m3f))
        // build brand->count maps and compare (group order is not guaranteed)
        var c5 : table<string; int>
        var cf : table<string; int>
        for (g in m5f) { c5[g.b] = g.n }
        for (g in m3f) { cf[g.b] = g.n }
        t |> equal(length(c5), length(cf))
        for (k, v in keys(c5), values(c5)) {
            t |> equal(v, cf?[k] ?? -1)
        }
        delete m5f; delete m3f
    }
}
