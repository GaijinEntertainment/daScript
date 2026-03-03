options gen2
require pugixml/PUGIXML_boost
require math
require strings
require dastest/testing_boost public

[test]
def test_for_each_child(t : T?) {
    parse_xml("<root><a>1</a><b>2</b><c>3</c></root>") <| $(doc, ok) {
        t |> success(ok)
        let root = doc.document_element
        var names : array<string>
        for_each_child(root) <| $(ch) {
            names |> push(string(ch.name))
        }
        t |> equal(length(names), 3)
        t |> equal(names[0], "a")
        t |> equal(names[1], "b")
        t |> equal(names[2], "c")
    }
}

[test]
def test_for_each_child_named(t : T?) {
    parse_xml("<root><item>1</item><other/><item>2</item><item>3</item></root>") <| $(doc, ok) {
        t |> success(ok)
        let root = doc.document_element
        var count = 0
        for_each_child(root, "item") <| $(ch) {
            count++
            t |> success(ch.ok)
        }
        t |> equal(count, 3)
    }
}

[test]
def test_for_each_attribute(t : T?) {
    parse_xml("<root x=\"1\" y=\"2\" z=\"3\"/>") <| $(doc, ok) {
        t |> success(ok)
        let root = doc.document_element
        var attrs : array<string>
        for_each_attribute(root) <| $(a) {
            attrs |> push(string(a.name))
        }
        t |> equal(length(attrs), 3)
        t |> equal(attrs[0], "x")
        t |> equal(attrs[1], "y")
        t |> equal(attrs[2], "z")
    }
}

[test]
def test_for_each_xpath(t : T?) {
    parse_xml("<root><item key=\"a\"/><item key=\"b\"/></root>") <| $(doc, ok) {
        t |> success(ok)
        var ns = select_nodes(doc.document_element, "item")
        var count = 0
        for_each(ns) <| $(xn) {
            count++
            t |> success(xn.ok)
        }
        t |> equal(count, 2)
        unsafe { delete ns; }
    }
}

[test]
def test_parse_xml(t : T?) {
    parse_xml("<hello>world</hello>") <| $(doc, ok) {
        t |> success(ok)
        let root = doc.document_element
        t |> equal(string(root.name), "hello")
        t |> equal(as_string(root.text, ""), "world")
    }
}

[test]
def test_with_doc(t : T?) {
    with_doc() <| $(doc) {
        var root = append_child(document_as_node(doc), "test")
        add_child(root, "msg", "hello")
        let txt = node_text(root, "msg")
        t |> equal(txt, "hello")
    }
}

[test]
def test_node_text(t : T?) {
    parse_xml("<root><title>Hello World</title><empty/></root>") <| $(doc, ok) {
        t |> success(ok)
        let root = doc.document_element
        t |> equal(node_text(root, "title"), "Hello World")
        t |> equal(node_text(root, "empty"), "")
        t |> equal(node_text(root, "missing", "fallback"), "fallback")
    }
}

[test]
def test_node_attr(t : T?) {
    parse_xml("<root name=\"test\" count=\"42\" ratio=\"3.14\" flag=\"true\"/>") <| $(doc, ok) {
        t |> success(ok)
        let root = doc.document_element
        t |> equal(node_attr(root, "name"), "test")
        t |> equal(node_attr(root, "missing", "default"), "default")
        t |> equal(node_attr_int(root, "count"), 42)
        t |> equal(node_attr_int(root, "missing", -1), -1)
        t |> success(abs(node_attr_float(root, "ratio") - 3.14) < 0.01)
        t |> equal(node_attr_bool(root, "flag"), true)
    }
}

[test]
def test_add_child(t : T?) {
    with_doc() <| $(doc) {
        var root = append_child(document_as_node(doc), "catalog")
        add_child(root, "title", "My Book")
        add_child_ex(root, "item", "id", "42")
        t |> equal(node_text(root, "title"), "My Book")
        let item = child(root, "item")
        t |> equal(node_attr(item, "id"), "42")
    }
}

[test]
def test_add_attr(t : T?) {
    with_doc() <| $(doc) {
        var root = append_child(document_as_node(doc), "node")
        add_attr(root, "name", "test")
        add_attr(root, "count", 42)
        add_attr(root, "ratio", 3.14)
        add_attr(root, "flag", true)
        t |> equal(node_attr(root, "name"), "test")
        t |> equal(node_attr_int(root, "count"), 42)
        t |> success(abs(node_attr_float(root, "ratio") - 3.14) < 0.01)
        t |> equal(node_attr_bool(root, "flag"), true)
    }
}

[test]
def test_to_string_doc(t : T?) {
    with_doc() <| $(doc) {
        var root = append_child(document_as_node(doc), "root")
        add_child(root, "msg", "hello")
        let s = to_string(doc)
        t |> success(length(s) > 0)
        // Should contain the tag name
        t |> success(find(s, "root") >= 0)
        t |> success(find(s, "hello") >= 0)
    }
}

[test]
def test_to_string_node(t : T?) {
    parse_xml("<root><item>text</item></root>") <| $(doc, ok) {
        t |> success(ok)
        let item = child(doc.document_element, "item")
        let s = to_string(item)
        t |> success(find(s, "item") >= 0)
        t |> success(find(s, "text") >= 0)
    }
}

[test]
def test_with_xpath(t : T?) {
    parse_xml("<root><item>hello</item></root>") <| $(doc, ok) {
        t |> success(ok)
        let root = doc.document_element
        with_xpath("//item") <| $(q) {
            t |> success(q.ok)
            let result = evaluate_node(q, root)
            t |> equal(string(result.node.name), "item")
        }
    }
}

[test]
def test_select_text(t : T?) {
    parse_xml("<root><a><b>deep</b></a></root>") <| $(doc, ok) {
        t |> success(ok)
        let root = doc.document_element
        t |> equal(select_text(root, "a/b"), "deep")
        t |> equal(select_text(root, "missing", "none"), "none")
    }
}

[test]
def test_select_value_element(t : T?) {
    parse_xml("<root><title>hello</title></root>") <| $(doc, ok) {
        t |> success(ok)
        t |> equal(select_value(doc.document_element, "title"), "hello")
    }
}

[test]
def test_select_value_attribute(t : T?) {
    parse_xml("<root><item key=\"val\"/></root>") <| $(doc, ok) {
        t |> success(ok)
        t |> equal(select_value(doc.document_element, "item/@key"), "val")
    }
}

[test]
def test_for_each_select(t : T?) {
    parse_xml("<root><a>1</a><a>2</a><a>3</a></root>") <| $(doc, ok) {
        t |> success(ok)
        var count = 0
        for_each_select(doc.document_element, "a") <| $(xn) {
            count++
            t |> success(xn.ok)
        }
        t |> equal(count, 3)
    }
}
