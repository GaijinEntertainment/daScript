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

[test]
def test_xml_orderby(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        // ORDER BY price ASC → 50, 100, 300 → ids 3, 1, 2
        let ids <- %linq! from c : Car in doc.document_element orderby c.price select c.id %%
        t |> equal(length(ids), 3)
        t |> equal(ids[0], 3)
        t |> equal(ids[1], 1)
        t |> equal(ids[2], 2)
    }
}

[test]
def test_xml_orderby_descending(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        let ids <- %linq! from c : Car in doc.document_element orderby c.price descending select c.id %%
        t |> equal(ids[0], 2)   // 300
        t |> equal(ids[2], 3)   // 50
    }
}

[test]
def test_xml_group_by(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        // Ford: car1+car3, Honda: car2 → 2 buckets
        let groups <- %linq! from c : Car in doc.document_element group c by c.brand %%
        t |> equal(length(groups), 2, "xml group buckets Ford/Honda")
        for (g in groups) {
            if (g._0 == "Ford")  { t |> equal(length(g._1), 2) }
            if (g._0 == "Honda") { t |> equal(length(g._1), 1) }
        }
    }
}
