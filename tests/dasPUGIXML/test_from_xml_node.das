options gen2
require pugixml/PUGIXML_boost
require dastest/testing_boost public

struct XmlCar {
    id : int
    name : string
    price : float
    ratio : double
    qty : uint
    active : bool
    year : int = 1990          // declared default — survives a missing attribute
}

[test]
def test_from_xml_node(t : T?) {
    // (d) collect rows inside the RAII block, then assert AFTER it closes —
    // materialized values (incl. strings) must stay valid once the document is freed.
    var collected : array<XmlCar>
    parse_xml(("<cars>" +
        "<car id=\"1\" name=\"alpha\" price=\"9.5\" ratio=\"0.25\" qty=\"7\" active=\"true\" year=\"2001\"/>" +
        "<car id=\"2\" name=\"beta\" price=\"3.0\" ratio=\"0.5\" qty=\"2\" active=\"false\"/>" +
        "</cars>")) $(doc, ok) {
        t |> success(ok)
        let root = doc.document_element
        for (c in from_xml_node(root, type<XmlCar>)) {
            collected |> push(c)
        }
    }
    t |> equal(length(collected), 2)
    // (a) every supported scalar type reads from its same-named attribute
    t |> equal(collected[0].id, 1)
    t |> equal(collected[0].name, "alpha")
    t |> equal(collected[0].price, 9.5)
    t |> equal(collected[0].ratio, 0.25lf)
    t |> equal(collected[0].qty, 7u)
    t |> success(collected[0].active)
    t |> equal(collected[0].year, 2001)
    // (b) the second row omits year → the struct's declared default (1990) is preserved
    t |> equal(collected[1].name, "beta")
    t |> success(!collected[1].active)
    t |> equal(collected[1].year, 1990)
}

[test]
def test_from_xml_node_filtered(t : T?) {
    parse_xml("<root><car id=\"1\"/><other id=\"99\"/><car id=\"2\"/><car id=\"3\"/></root>") $(doc, ok) {
        t |> success(ok)
        let root = doc.document_element
        // (c) tag-filtered overload skips <other>; (e) a comprehension consumes the iterator
        let ids <- [for (c in from_xml_node(root, "car", type<XmlCar>)); c.id]
        t |> equal(length(ids), 3)
        t |> equal(ids[0], 1)
        t |> equal(ids[1], 2)
        t |> equal(ids[2], 3)
    }
}

[test]
def test_from_xml_node_mixed_content(t : T?) {
    // bare from_xml_node skips non-element children (PCDATA between the cars),
    // so mixed content yields only the 2 elements — not extra default rows
    parse_xml("<cars>noise<car id=\"1\"/>more<car id=\"2\"/></cars>") $(doc, ok) {
        t |> success(ok)
        let root = doc.document_element
        let ids <- [for (c in from_xml_node(root, type<XmlCar>)); c.id]
        t |> equal(length(ids), 2)
        t |> equal(ids[0], 1)
        t |> equal(ids[1], 2)
    }
}
