options gen2

require pugixml/PUGIXML_boost
require daslib/linq_fold
require daslib/linq_boost public
require daslib/strings_boost
require dastest/testing_boost public

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

let XML = ("<cars>" +
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
    for (i in range(length(a))) {
        if (a[i].id != b[i].id || a[i].price != b[i].price || a[i].qty != b[i].qty || a[i].brand != b[i].brand) {
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

[test]
def parity_reverse_backward_text_skip(t : T?) {
    // This fixture interleaves "noise"/"more" text nodes between elements. The BACKWARD DOM walk must
    // skip non-element nodes exactly like the forward walk, so the fused result equals the array fold.
    parse_xml(XML) $(doc, ok) {
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

[test]
def parity_where_numeric(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        var arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.price > 199.0).to_array())
        var m3f <- _fold(each(arr)._where(_.price > 199.0).to_array())
        var m3  <- (arr |> _where(_.price > 199.0))
        check3(t, m5f, m3f, m3)
    }
}

[test]
def parity_where_or_with_string(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        var arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.price < 100.0 || (_.brand |> starts_with("Fe"))).to_array())
        var m3f <- _fold(each(arr)._where(_.price < 100.0 || (_.brand |> starts_with("Fe"))).to_array())
        var m3  <- (arr |> _where(_.price < 100.0 || (_.brand |> starts_with("Fe"))))
        check3(t, m5f, m3f, m3)
    }
}

[test]
def parity_where_string_predicates(t : T?) {
    parse_xml(XML) $(doc, ok) {
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

[test]
def parity_where_length_and_arith(t : T?) {
    parse_xml(XML) $(doc, ok) {
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

[test]
def parity_select_projection_sum(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        var arr <- fixture_array()
        let s5 = _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._select(_.qty)._where(_ > 3).sum())
        let sf = _fold(each(arr)._select(_.qty)._where(_ > 3).sum())
        let sm = (arr._select(_.qty)._where(_ > 3).sum())
        t |> equal(s5, sf)
        t |> equal(sf, sm)
    }
}

[test]
def parity_select_where_to_array(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        var arr <- fixture_array()
        var m5f <- _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.qty > 4)._where(_.price < 1000.0).to_array())
        var m3f <- _fold(each(arr)._where(_.qty > 4)._where(_.price < 1000.0).to_array())
        var m3  <- (arr |> _where(_.qty > 4) |> _where(_.price < 1000.0))
        check3(t, m5f, m3f, m3)
    }
}
