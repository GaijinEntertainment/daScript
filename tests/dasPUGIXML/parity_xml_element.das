options gen2
options persistent_heap

require pugixml/PUGIXML_boost
require daslib/linq_fold
require daslib/linq_boost public
require dastest/testing_boost public

// XML linq parity — element + quantifier terminators (first/last/single/element_at + _or_default,
// any/all/contains). Each runs three ways and must agree. `options persistent_heap` because the
// single-element terminators declare an uninitialized var of the (string-bearing) element type.

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

[test]
def parity_first_last(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        // first matching price > 199
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.price > 199.0).first()).id,
                   _fold(each(arr)._where(_.price > 199.0).first()).id)
        // last matching price > 199
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.price > 199.0).last()).id,
                   _fold(each(arr)._where(_.price > 199.0).last()).id)
    }
}

[test]
def parity_first_or_default(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        let SENT = Car(id = -1)
        // hit
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.brand == "Toyota").first_or_default(SENT)).id,
                   _fold(each(arr)._where(_.brand == "Toyota").first_or_default(SENT)).id)
        // miss -> sentinel
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.brand == "Nope").first_or_default(SENT)).id, -1)
        // last_or_default miss
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.brand == "Nope").last_or_default(SENT)).id, -1)
    }
}

[test]
def parity_single(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        // exactly one Toyota
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.brand == "Toyota").single()).id,
                   _fold(each(arr)._where(_.brand == "Toyota").single()).id)
        let SENT = Car(id = -1)
        // single_or_default: one match -> that; zero -> sentinel; many -> sentinel
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.brand == "Toyota").single_or_default(SENT)).id, 8)
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.brand == "Nope").single_or_default(SENT)).id, -1)
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.brand == "Ford").single_or_default(SENT)).id, -1)
    }
}

[test]
def parity_under_guard_string_field(t : T?) {
    // Materialize-under-guard defers the full row build (incl. the string clone) behind the where-gate;
    // read a STRING field off the survivor to confirm it is cloned correctly, and exercise a
    // string-typed where-field (read cheaply, then re-read in the deferred full build).
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        // last matching price > 199 -> Toyota (id 8); read the survivor's string field
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.price > 199.0).last()).brand,
                   _fold(each(arr)._where(_.price > 199.0).last()).brand)
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.brand == "Toyota").single()).brand, "Toyota")
        // string-typed where-field with last() -> last Ford is id 7
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.brand == "Ford").last()).id,
                   _fold(each(arr)._where(_.brand == "Ford").last()).id)
    }
}

[test]
def parity_element_at(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>)).element_at(2)).id,
                   _fold(each(arr).element_at(2)).id)
        // element_at_or_default takes only an index; out-of-range yields the type's default (id 0).
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>)).element_at_or_default(99)).id,
                   _fold(each(arr).element_at_or_default(99)).id)
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>)).element_at_or_default(0)).id,
                   _fold(each(arr).element_at_or_default(0)).id)
    }
}

[test]
def parity_any_all_contains(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.price > 50000.0).any()),
                   _fold(each(arr)._where(_.price > 50000.0).any()))
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>))._all(_.price > 0.0)),
                   _fold(each(arr)._all(_.price > 0.0)))
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>))._select(_.id).contains(5)),
                   _fold(each(arr)._select(_.id).contains(5)))
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>))._select(_.id).contains(99)),
                   _fold(each(arr)._select(_.id).contains(99)))
    }
}
