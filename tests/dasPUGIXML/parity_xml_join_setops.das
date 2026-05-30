options gen2

require pugixml/PUGIXML_boost
require daslib/linq_fold
require daslib/linq_boost public
require daslib/linq public
require dastest/testing_boost public
require daslib/sort_boost

// XML linq parity — binary two-source ops (join family + set-ops) where the XML source is one
// operand and an in-memory array is the other. This is the proof of the mixed-source overloads
// added to daslib/linq.das: from_xml_node yields an iterator, so `cars |> _join(dealersArray, ...)`
// and `brands_iter |> union(brandArray)` exercise the new (iterator, array) overloads directly.
//   m5f = XML iterator + array   m3f = each(arr) iterator + array   m3 = all-array

struct Car {
    id    : int
    price : float
    qty   : int
    brand : string
}
struct Dealer {
    brand : string
    city  : string
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
    for (i in range(length(a))) {
        if (a[i] != b[i]) { return false }
    }
    return true
}

[test]
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

[test]
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

[test]
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

[test]
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

[test]
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

[test]
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
