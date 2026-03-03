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

// ============ operator is/as type tests ============

[test]
def test_attr_as_int(t : T?) {
    parse_xml("<root count=\"42\"/>") <| $(doc, ok) {
        t |> success(ok)
        let a = attribute(doc.document_element, "count")
        t |> success(a is int)
        t |> equal(a as int, 42)
    }
}

[test]
def test_attr_as_float(t : T?) {
    parse_xml("<root ratio=\"3.14\"/>") <| $(doc, ok) {
        t |> success(ok)
        let a = attribute(doc.document_element, "ratio")
        t |> success(a is float)
        t |> success(abs((a as float) - 3.14) < 0.01)
    }
}

[test]
def test_attr_as_bool(t : T?) {
    parse_xml("<root flag=\"true\"/>") <| $(doc, ok) {
        t |> success(ok)
        let a = attribute(doc.document_element, "flag")
        t |> success(a is bool)
        t |> equal(a as bool, true)
    }
}

[test]
def test_attr_as_string(t : T?) {
    parse_xml("<root name=\"hello\"/>") <| $(doc, ok) {
        t |> success(ok)
        let a = attribute(doc.document_element, "name")
        t |> success(a is string)
        t |> equal(a as string, "hello")
    }
}

[test]
def test_attr_as_uint(t : T?) {
    parse_xml("<root val=\"100\"/>") <| $(doc, ok) {
        t |> success(ok)
        let a = attribute(doc.document_element, "val")
        t |> success(a is uint)
        t |> equal(a as uint, 100u)
    }
}

[test]
def test_attr_as_double(t : T?) {
    parse_xml("<root val=\"2.718281828\"/>") <| $(doc, ok) {
        t |> success(ok)
        let a = attribute(doc.document_element, "val")
        t |> success(a is double)
        t |> success(abs((a as double) - 2.718281828lf) < 0.0001lf)
    }
}

[test]
def test_attr_missing_is_not(t : T?) {
    parse_xml("<root/>") <| $(doc, ok) {
        t |> success(ok)
        let a = attribute(doc.document_element, "missing")
        t |> success(!(a is int))
        t |> success(!(a is float))
        t |> success(!(a is bool))
        t |> success(!(a is string))
    }
}

[test]
def test_text_as_int(t : T?) {
    parse_xml("<root><val>99</val></root>") <| $(doc, ok) {
        t |> success(ok)
        let tx = child(doc.document_element, "val").text
        t |> success(tx is int)
        t |> equal(tx as int, 99)
    }
}

[test]
def test_text_as_float(t : T?) {
    parse_xml("<root><val>1.5</val></root>") <| $(doc, ok) {
        t |> success(ok)
        let tx = child(doc.document_element, "val").text
        t |> success(tx is float)
        t |> success(abs((tx as float) - 1.5) < 0.01)
    }
}

[test]
def test_text_as_bool(t : T?) {
    parse_xml("<root><val>true</val></root>") <| $(doc, ok) {
        t |> success(ok)
        let tx = child(doc.document_element, "val").text
        t |> success(tx is bool)
        t |> equal(tx as bool, true)
    }
}

[test]
def test_text_as_string(t : T?) {
    parse_xml("<root><val>hello world</val></root>") <| $(doc, ok) {
        t |> success(ok)
        let tx = child(doc.document_element, "val").text
        t |> success(tx is string)
        t |> equal(tx as string, "hello world")
    }
}

[test]
def test_text_empty_is_not(t : T?) {
    parse_xml("<root><val/></root>") <| $(doc, ok) {
        t |> success(ok)
        let tx = child(doc.document_element, "val").text
        t |> success(!(tx is int))
        t |> success(!(tx is float))
        t |> success(!(tx is string))
    }
}

[test]
def test_node_as_string(t : T?) {
    parse_xml("<root><item>text</item></root>") <| $(doc, ok) {
        t |> success(ok)
        let item = child(doc.document_element, "item")
        let s = item as string
        t |> success(find(s, "item") >= 0)
        t |> success(find(s, "text") >= 0)
    }
}
