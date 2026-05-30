options gen2
options persistent_heap

require pugixml/PUGIXML_boost
require daslib/linq_fold
require daslib/linq_boost public
require dastest/testing_boost public

// XML linq parity — aggregate terminators. Each runs three ways (m5f XML fold, m3f array fold,
// m3 plain array linq) and must agree. `options persistent_heap` because the element-returning
// _by aggregates declare an uninitialized var of the element type (which has a string field).

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
def parity_count_and_sum(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>)).count()),
                   _fold(each(arr).count()))
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>))._where(_.qty > 3).count()),
                   _fold(each(arr)._where(_.qty > 3).count()))
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>)).long_count()),
                   _fold(each(arr).long_count()))
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>))._select(_.qty).sum()),
                   _fold(each(arr)._select(_.qty).sum()))
    }
}

[test]
def parity_min_max_average(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>))._select(_.qty).min()),
                   _fold(each(arr)._select(_.qty).min()))
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>))._select(_.qty).max()),
                   _fold(each(arr)._select(_.qty).max()))
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>))._select(_.price).average()),
                   _fold(each(arr)._select(_.price).average()))
    }
}

[test]
def parity_aggregate_fold(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        t |> equal(_fold(unsafe(from_xml_node(doc.document_element, type<Car>)).aggregate(0, $(acc, c) => acc + c.qty)),
                   _fold(each(arr).aggregate(0, $(acc, c) => acc + c.qty)))
    }
}

[test]
def parity_min_max_tuple(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        let x = _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._select(_.qty).min_max())
        let r = _fold(each(arr)._select(_.qty).min_max())
        t |> equal(x._0, r._0)
        t |> equal(x._1, r._1)
        let xa = _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._select(_.price).min_max_average())
        let ra = _fold(each(arr)._select(_.price).min_max_average())
        t |> equal(xa._0, ra._0)
        t |> equal(xa._1, ra._1)
        t |> equal(xa._2, ra._2)
    }
}

[test]
def parity_min_by_max_by(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let arr <- fixture_array()
        let mn5 = _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._min_by(_.price))
        let mnr = _fold(each(arr)._min_by(_.price))
        t |> equal(mn5.id, mnr.id)
        let mx5 = _fold(unsafe(from_xml_node(doc.document_element, type<Car>))._max_by(_.price))
        let mxr = _fold(each(arr)._max_by(_.price))
        t |> equal(mx5.id, mxr.id)
    }
}
