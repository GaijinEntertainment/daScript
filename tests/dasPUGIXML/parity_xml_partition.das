options gen2

require pugixml/PUGIXML_boost
require daslib/linq_fold
require daslib/linq_boost public
require dastest/testing_boost public

// XML linq parity — partitioning (take/skip/take_while/skip_while) and their combinations with
// where/aggregate. Each runs three ways (m5f XML fold, m3f array fold, m3 plain array linq).

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

[test]
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

[test]
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

[test]
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

[test]
def parity_where_then_take_count(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        var arr <- fixture_array()
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.price > 100.0).take(2).count()),
                   _fold(each(arr)._where(_.price > 100.0).take(2).count()))
    }
}

[test]
def parity_take_then_sum(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        var arr <- fixture_array()
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>))._select(_.qty).take(4).sum()),
                   _fold(each(arr)._select(_.qty).take(4).sum()))
    }
}
