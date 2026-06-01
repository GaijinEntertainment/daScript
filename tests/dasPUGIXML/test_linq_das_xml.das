options gen2

require dastest/testing_boost public
require daslib/linq_das
require pugixml/PUGIXML_boost

// linq_das over an XML source: `from c : Car in <node>` → `from_in(node, type<Car>)` →
// `unsafe(from_xml_node(node, type<Car>))`, which `_fold` fuses via the XmlAdapter. The node is passed
// inline (a `let`-bound node would be `const&` and rejected — see PUGIXML linq_fold tests).

struct Car {
    id    : int
    price : float
    brand : string
}

let XML = ("<cars>" +
    "<car id=\"1\" price=\"100.0\" brand=\"Ford\"/>noise" +
    "<car id=\"2\" price=\"300.0\" brand=\"Honda\"/>" +
    "<car id=\"3\" price=\"50.0\" brand=\"Ford\"/>" +
    "</cars>")

[test]
def test_xml_where_select(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let brands <- %linq! from c : Car in doc.document_element where c.price > 99.0 select c.brand %%
        t |> equal(length(brands), 2, "xml where/select")
        t |> equal(brands[0], "Ford")
        t |> equal(brands[1], "Honda")
    }
}

[test]
def test_xml_identity_select(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let rows <- %linq! from c : Car in doc.document_element where c.price >= 100.0 select c %%
        t |> equal(length(rows), 2, "xml identity select returns rows")
        t |> equal(rows[0].id, 1)
        t |> equal(rows[1].id, 2)
    }
}

[test]
def test_xml_iterator_output(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let brands <- [for (b in %linq! from c : Car in doc.document_element where c.price > 99.0 select c.brand iterator %%); b]
        t |> equal(length(brands), 2, "xml iterator output")
    }
}
