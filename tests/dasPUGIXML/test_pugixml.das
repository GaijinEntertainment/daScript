options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options strict_smart_pointers = true

require pugixml/PUGIXML_boost
require math
require strings
require dastest/testing_boost public

// ============ Node navigation ============

[test(description = "last_child returns the final child element of a node")]
def test_node_last_child(t : T?) {
    t |> run("last_child returns final child element") <| @(t : T?) {
        parse_xml("<root><a/><b/><c/></root>") $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            t |> equal(root.last_child.name, "c")
        }
    }
}

[test(description = "previous_sibling walks backward through a node's siblings")]
def test_node_previous_sibling(t : T?) {
    t |> run("previous_sibling navigates backward") <| @(t : T?) {
        parse_xml("<root><a/><b/><c/></root>") $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            let c_node = root.last_child
            t |> equal(c_node.previous_sibling.name, "b")
            t |> equal(c_node.previous_sibling.previous_sibling.name, "a")
        }
    }
}

[test(description = "parent returns the element that contains a node")]
def test_node_parent(t : T?) {
    t |> run("parent returns containing element") <| @(t : T?) {
        parse_xml("<root><child><nested/></child></root>") $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            let nested = child(child(root, "child"), "nested")
            t |> equal(nested.parent.name, "child")
            t |> equal(nested.parent.parent.name, "root")
        }
    }
}

[test(description = "root returns the document node from any depth")]
def test_node_root(t : T?) {
    t |> run("root returns document node from any depth") <| @(t : T?) {
        parse_xml("<root><a><b><c/></b></a></root>") $(doc, ok) {
            t |> success(ok)
            let deep = child(child(child(doc.document_element, "a"), "b"), "c")
            let r = deep.root
            t |> success(r.ok)
            // root is the document node (not the document element)
            t |> equal(int(r._type), int(xml_node_type.node_document))
        }
    }
}

[test(description = "last_attribute returns the final attribute of a node")]
def test_node_last_attribute(t : T?) {
    t |> run("last_attribute returns final attribute") <| @(t : T?) {
        parse_xml("<root x=\"1\" y=\"2\" z=\"3\"/>") $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            t |> equal(root.last_attribute.name, "z")
        }
    }
}

[test(description = "child_value returns the PCDATA text of a node")]
def test_node_child_value(t : T?) {
    t |> run("child_value returns PCDATA text content") <| @(t : T?) {
        parse_xml("<root>hello world</root>") $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            t |> equal(root.child_value, "hello world")
        }
    }
}

[test(description = "child_value with a name returns the text of the named child")]
def test_node_child_value_by_name(t : T?) {
    t |> run("child_value(name) returns named child text") <| @(t : T?) {
        parse_xml("<root><title>My Title</title><body>Content</body></root>") $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            t |> equal(child_value(root, "title"), "My Title")
            t |> equal(child_value(root, "body"), "Content")
            t |> equal(child_value(root, "missing"), "")
        }
    }
}

[test(description = "find_child_by_attribute locates a child by element name and attribute value")]
def test_find_child_by_attribute(t : T?) {
    t |> run("find_child_by_attribute locates element") <| @(t : T?) {
        parse_xml("<root><item id=\"a\"/><item id=\"b\"/><item id=\"c\"/></root>") $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            let found = find_child_by_attribute(root, "item", "id", "b")
            t |> success(found.ok)
            t |> equal(node_attr(found, "id"), "b")
        }
    }
}

[test(description = "find_child_by_attribute without an element name finds a child by attribute alone")]
def test_find_child_by_attribute_any_name(t : T?) {
    t |> run("find_child_by_attribute without element name") <| @(t : T?) {
        parse_xml("<root><foo key=\"x\"/><bar key=\"y\"/></root>") $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            let found = find_child_by_attribute(root, "key", "y")
            t |> success(found.ok)
            t |> equal(found.name, "bar")
        }
    }
}

[test(description = "first_element_by_path resolves a slash-separated path to a descendant")]
def test_first_element_by_path(t : T?) {
    t |> run("first_element_by_path traverses path") <| @(t : T?) {
        parse_xml("<root><a><b><c>deep</c></b></a></root>") $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            let found = first_element_by_path(root, "a/b/c")
            t |> success(found.ok)
            t |> equal(found.name, "c")
            t |> equal(found.text as string, "deep")
        }
    }
}

[test(description = "path returns the slash-separated path of a node")]
def test_node_path(t : T?) {
    t |> run("path returns slash-separated path") <| @(t : T?) {
        parse_xml("<root><a><b/></a></root>") $(doc, ok) {
            t |> success(ok)
            let b_node = child(child(doc.document_element, "a"), "b")
            let p = path(b_node, "/")
            t |> success(find(p, "root") >= 0)
            t |> success(find(p, "a") >= 0)
            t |> success(find(p, "b") >= 0)
        }
    }
}

[test(description = "hash_value of the same node is stable")]
def test_node_hash_value(t : T?) {
    t |> run("hash_value is consistent for same node") <| @(t : T?) {
        parse_xml("<root><a/><b/></root>") $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            let a_node = root.first_child
            let b_node = a_node.next_sibling
            // Same node should have same hash
            t |> equal(a_node.hash_value, a_node.hash_value)
            // Different nodes should (usually) have different hashes
            t |> success(a_node.hash_value != b_node.hash_value)
        }
    }
}

[test(description = "node == and != compare node identity")]
def test_node_comparison(t : T?) {
    t |> run("node == and != operators") <| @(t : T?) {
        parse_xml("<root><a/><b/></root>") $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            let a1 = child(root, "a")
            let a2 = child(root, "a")
            let b_node = child(root, "b")
            t |> success(a1 == a2)
            t |> success(a1 != b_node)
        }
    }
}

[test(description = "an empty node reports empty and not ok while a real node reports the opposite")]
def test_node_empty(t : T?) {
    t |> run("node empty and ok properties") <| @(t : T?) {
        parse_xml("<root/>") $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            t |> success(root.ok)
            t |> success(!root.empty)
            let missing = child(root, "nope")
            t |> success(!missing.ok)
            t |> success(missing.empty)
        }
    }
}

[test(description = "type reports the correct xml_node_type")]
def test_node_type(t : T?) {
    t |> run("node type returns correct xml_node_type") <| @(t : T?) {
        parse_xml("<root>text<!-- comment --></root>") $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            t |> equal(int(root._type), int(xml_node_type.node_element))
        }
    }
}

// ============ Attribute navigation & properties ============

[test(description = "previous_attribute walks backward through a node's attributes")]
def test_attribute_previous(t : T?) {
    t |> run("previous_attribute navigates backward") <| @(t : T?) {
        parse_xml("<root a=\"1\" b=\"2\" c=\"3\"/>") $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            let last = root.last_attribute
            t |> equal(last.name, "c")
            t |> equal(last.previous_attribute.name, "b")
            t |> equal(last.previous_attribute.previous_attribute.name, "a")
        }
    }
}

[test(description = "hash_value of the same attribute is stable")]
def test_attribute_hash_value(t : T?) {
    t |> run("attribute hash_value is consistent") <| @(t : T?) {
        parse_xml("<root x=\"1\" y=\"2\"/>") $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            let x_attr = attribute(root, "x")
            let y_attr = attribute(root, "y")
            t |> equal(x_attr.hash_value, x_attr.hash_value)
            t |> success(x_attr.hash_value != y_attr.hash_value)
        }
    }
}

[test(description = "attribute == and != compare attribute identity")]
def test_attribute_comparison(t : T?) {
    t |> run("attribute == and != operators") <| @(t : T?) {
        parse_xml("<root x=\"1\" y=\"2\"/>") $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            let x1 = attribute(root, "x")
            let x2 = attribute(root, "x")
            let y_attr = attribute(root, "y")
            t |> success(x1 == x2)
            t |> success(x1 != y_attr)
        }
    }
}

[test(description = "a missing attribute reports empty and not ok")]
def test_attribute_empty(t : T?) {
    t |> run("missing attribute is empty/not ok") <| @(t : T?) {
        parse_xml("<root x=\"1\"/>") $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            let x_attr = attribute(root, "x")
            t |> success(x_attr.ok)
            t |> success(!x_attr.empty)
            let missing = attribute(root, "nope")
            t |> success(!missing.ok)
            t |> success(missing.empty)
        }
    }
}

[test(description = "an attribute exposes its name and value")]
def test_attribute_name_value(t : T?) {
    t |> run("attribute name and value properties") <| @(t : T?) {
        parse_xml("<root color=\"red\"/>") $(doc, ok) {
            t |> success(ok)
            let a = attribute(doc.document_element, "color")
            t |> equal(a.name, "color")
            t |> equal(a.value, "red")
        }
    }
}

// ============ Text properties ============

[test(description = "the text get property returns the raw text of a node")]
def test_text_get(t : T?) {
    t |> run("text get property returns raw text") <| @(t : T?) {
        parse_xml("<root>hello</root>") $(doc, ok) {
            t |> success(ok)
            let tx = doc.document_element.text
            t |> success(tx.ok)
            t |> success(!tx.empty)
            t |> equal(tx.get, "hello")
        }
    }
}

[test(description = "text data returns the PCDATA node behind the text")]
def test_text_data(t : T?) {
    t |> run("text data returns PCDATA node") <| @(t : T?) {
        parse_xml("<root>hello</root>") $(doc, ok) {
            t |> success(ok)
            let tx = doc.document_element.text
            let data_node = tx.data
            t |> success(data_node.ok)
            // data returns the PCDATA child node; its parent is the element
            t |> equal(int(data_node._type), int(xml_node_type.node_pcdata))
            t |> equal(data_node.parent.name, "root")
        }
    }
}

// ============ Parse result ============

[test(description = "parse_result reports success for valid XML")]
def test_parse_result_success(t : T?) {
    t |> run("parse_result reports success for valid XML") <| @(t : T?) {
        var doc = new xml_document()
        var result : xml_parse_result
        let ok = load_string(doc, "<root/>", result, parse_default)
        t |> success(ok)
        t |> equal(int(result.status), int(xml_parse_status.status_ok))
        t |> success(!empty(result.description))
        unsafe { delete doc; }
    }
}

[test(description = "parse_result reports failure for invalid XML")]
def test_parse_result_failure(t : T?) {
    t |> run("parse_result reports failure for invalid XML") <| @(t : T?) {
        var doc = new xml_document()
        var result : xml_parse_result
        let ok = load_string(doc, "<root><unclosed>", result, parse_default)
        t |> success(!ok)
        t |> success(int(result.status) != int(xml_parse_status.status_ok))
        unsafe { delete doc; }
    }
}

// ============ Document operations ============

[test(description = "document_element returns the root element")]
def test_document_element(t : T?) {
    t |> run("document_element returns root element") <| @(t : T?) {
        parse_xml("<myroot/>") $(doc, ok) {
            t |> success(ok)
            t |> equal(doc.document_element.name, "myroot")
        }
    }
}

[test(description = "reset clears the document")]
def test_document_reset(t : T?) {
    t |> run("reset clears the document") <| @(t : T?) {
        var doc = new xml_document()
        var result : xml_parse_result
        load_string(doc, "<root><a/></root>", result, parse_default)
        t |> success(doc.document_element.ok)
        reset(doc)
        t |> success(!doc.document_element.ok)
        unsafe { delete doc; }
    }
}

[test(description = "save_string serializes the document back to text")]
def test_save_string(t : T?) {
    t |> run("save_string serializes document") <| @(t : T?) {
        with_doc() $(doc) {
            var root = append_child(doc as xml_node, "test")
            add_child(root, "item", "value")
            let s = save_string(doc, "\t", format_default, xml_encoding.encoding_auto)
            t |> success(!empty(s))
            t |> success(find(s, "test") >= 0)
            t |> success(find(s, "value") >= 0)
        }
    }
}

// ============ XPath advanced ============

[test(description = "evaluate_boolean returns the boolean result of an XPath expression")]
def test_xpath_evaluate_boolean(t : T?) {
    t |> run("evaluate_boolean returns bool result") <| @(t : T?) {
        parse_xml("<root><item/></root>") $(doc, ok) {
            t |> success(ok)
            with_xpath("boolean(//item)") $(q) {
                t |> success(evaluate_boolean(q, doc.document_element))
            }
            with_xpath("boolean(//missing)") $(q) {
                t |> success(!evaluate_boolean(q, doc.document_element))
            }
        }
    }
}

[test(description = "evaluate_number returns the numeric result of an XPath expression")]
def test_xpath_evaluate_number(t : T?) {
    t |> run("evaluate_number returns numeric result") <| @(t : T?) {
        parse_xml("<root><a/><a/><a/></root>") $(doc, ok) {
            t |> success(ok)
            with_xpath("count(//a)") $(q) {
                t |> success(abs(evaluate_number(q, doc.document_element) - 3.0lf) < 0.001lf)
            }
        }
    }
}

[test(description = "evaluate_string returns the string result of an XPath expression")]
def test_xpath_evaluate_string(t : T?) {
    t |> run("evaluate_string returns string result") <| @(t : T?) {
        parse_xml("<root><title>Hello</title></root>") $(doc, ok) {
            t |> success(ok)
            with_xpath("string(//title)") $(q) {
                t |> equal(evaluate_string(q, doc.document_element), "Hello")
            }
        }
    }
}

[test(description = "evaluate_node_set returns the node set an XPath expression selects")]
def test_xpath_evaluate_node_set(t : T?) {
    t |> run("evaluate_node_set returns node set") <| @(t : T?) {
        parse_xml("<root><a/><b/><c/></root>") $(doc, ok) {
            t |> success(ok)
            with_xpath("/root/*") $(q) {
                var ns = evaluate_node_set(q, doc.document_element)
                t |> equal(ns.size, 3)
                t |> success(!ns.empty)
                t |> equal(ns.first.node.name, "a")
                t |> equal(at(ns, 2).node.name, "c")
                unsafe { delete ns; }
            }
        }
    }
}

[test(description = "an xpath_query exposes its return_type and result")]
def test_xpath_query_properties(t : T?) {
    t |> run("xpath_query exposes return_type and result") <| @(t : T?) {
        with_xpath("count(//a)") $(q) {
            t |> success(q.ok)
            t |> equal(int(q.return_type), int(xpath_value_type.xpath_type_number))
        }
        with_xpath("boolean(//a)") $(q) {
            t |> equal(int(q.return_type), int(xpath_value_type.xpath_type_boolean))
        }
    }
}

[test(description = "sorting an xpath_node_set reverses its order")]
def test_xpath_node_set_sort(t : T?) {
    t |> run("xpath_node_set sort reverses order") <| @(t : T?) {
        parse_xml("<root><a/><b/><c/></root>") $(doc, ok) {
            t |> success(ok)
            var ns = select_nodes(doc.document_element, "*")
            t |> equal(at(ns, 0).node.name, "a")
            sort(ns, true)
            t |> equal(at(ns, 0).node.name, "c")
            unsafe { delete ns; }
        }
    }
}

[test(description = "an xpath_node exposes its node, attribute and parent")]
def test_xpath_node_accessors(t : T?) {
    t |> run("xpath_node node/attribute/parent accessors") <| @(t : T?) {
        parse_xml("<root><item key=\"val\"/></root>") $(doc, ok) {
            t |> success(ok)
            // Node result
            let nr = select_node(doc.document_element, "item")
            t |> success(nr.ok)
            t |> equal(nr.node.name, "item")
            t |> success(nr.parent.ok)
            t |> equal(nr.parent.name, "root")
            // Attribute result
            let ar = select_node(doc.document_element, "item/@key")
            t |> success(ar.ok)
            t |> equal(ar.attribute.name, "key")
            t |> equal(ar.attribute.value, "val")
        }
    }
}

[test(description = "an XPath query substitutes variables bound in a variable set")]
def test_xpath_variables(t : T?) {
    t |> run("xpath with variable substitution") <| @(t : T?) {
        parse_xml("<root><item n=\"1\"/><item n=\"2\"/><item n=\"3\"/></root>") $(doc, ok) {
            t |> success(ok)
            var vars = new xpath_variable_set()
            set(vars, "target", 2.0lf)
            var q = xpath_compile("item[@n=$target]", vars)
            t |> success(q.ok)
            let result = evaluate_node(q, doc.document_element)
            t |> success(result.ok)
            t |> equal(node_attr(result.node, "n"), "2")
            unsafe { delete q; }
            unsafe { delete vars; }
        }
    }
}

// ============ Node mutation ============

[test(description = "set_name renames a node")]
def test_set_name(t : T?) {
    t |> run("set_name renames a node") <| @(t : T?) {
        with_doc() $(doc) {
            var root = append_child(doc as xml_node, "old")
            t |> equal(root.name, "old")
            set_name(root, "new_name")
            t |> equal(root.name, "new_name")
        }
    }
}

[test(description = "set_value changes a node's value")]
def test_set_value(t : T?) {
    t |> run("set_value changes node value") <| @(t : T?) {
        with_doc() $(doc) {
            var dnode = doc as xml_node
            var root = append_child(dnode, "root")
            var pi = append_child(root, xml_node_type.node_pi)
            set_name(pi, "target")
            set_value(pi, "data")
            t |> equal(pi.value, "data")
        }
    }
}

[test(description = "prepend_child inserts a new child before the existing ones")]
def test_prepend_child(t : T?) {
    t |> run("prepend_child inserts at beginning") <| @(t : T?) {
        with_doc() $(doc) {
            var root = append_child(doc as xml_node, "root")
            append_child(root, "second")
            prepend_child(root, "first")
            t |> equal(root.first_child.name, "first")
            t |> equal(root.last_child.name, "second")
        }
    }
}

[test(description = "insert_child_after places a new child directly after the given node")]
def test_insert_child_after(t : T?) {
    t |> run("insert_child_after places node correctly") <| @(t : T?) {
        with_doc() $(doc) {
            var root = append_child(doc as xml_node, "root")
            let a_node = append_child(root, "a")
            append_child(root, "c")
            insert_child_after(root, "b", a_node)
            // Order should be: a, b, c
            t |> equal(root.first_child.name, "a")
            t |> equal(root.first_child.next_sibling.name, "b")
            t |> equal(root.last_child.name, "c")
        }
    }
}

[test(description = "insert_child_before places a new child directly before the given node")]
def test_insert_child_before(t : T?) {
    t |> run("insert_child_before places node correctly") <| @(t : T?) {
        with_doc() $(doc) {
            var root = append_child(doc as xml_node, "root")
            append_child(root, "a")
            let c_node = append_child(root, "c")
            insert_child_before(root, "b", c_node)
            // Order should be: a, b, c
            t |> equal(root.first_child.next_sibling.name, "b")
        }
    }
}

[test(description = "remove_child removes the given child node")]
def test_remove_child(t : T?) {
    t |> run("remove_child removes a node object") <| @(t : T?) {
        with_doc() $(doc) {
            var root = append_child(doc as xml_node, "root")
            append_child(root, "keep")
            let rm = append_child(root, "remove_me")
            append_child(root, "also_keep")
            remove_child(root, rm)
            var count = 0
            for_each_child(root) $(ch) {
                count++
                t |> success(ch.name != "remove_me")
            }
            t |> equal(count, 2)
        }
    }
}

[test(description = "remove_child by name removes the first child with that name")]
def test_remove_child_by_name(t : T?) {
    t |> run("remove_child by name removes first match") <| @(t : T?) {
        with_doc() $(doc) {
            var root = append_child(doc as xml_node, "root")
            append_child(root, "a")
            append_child(root, "b")
            append_child(root, "c")
            remove_child(root, "b")
            var names : array<string>
            for_each_child(root) $(ch) {
                names |> push(ch.name)
            }
            t |> equal(length(names), 2)
            t |> equal(names[0], "a")
            t |> equal(names[1], "c")
        }
    }
}

[test(description = "remove_children removes every child")]
def test_remove_children(t : T?) {
    t |> run("remove_children clears all children") <| @(t : T?) {
        with_doc() $(doc) {
            var root = append_child(doc as xml_node, "root")
            append_child(root, "a")
            append_child(root, "b")
            append_child(root, "c")
            remove_children(root)
            t |> success(!root.first_child.ok)
        }
    }
}

// ============ Attribute mutation ============

[test(description = "prepend_attribute inserts an attribute before the existing ones")]
def test_prepend_attribute(t : T?) {
    t |> run("prepend_attribute inserts at beginning") <| @(t : T?) {
        with_doc() $(doc) {
            var root = append_child(doc as xml_node, "root")
            var a2 = append_attribute(root, "second")
            set_value(a2, "2")
            var a1 = prepend_attribute(root, "first")
            set_value(a1, "1")
            t |> equal(root.first_attribute.name, "first")
            t |> equal(root.last_attribute.name, "second")
        }
    }
}

[test(description = "insert_attribute_after places a new attribute directly after the given one")]
def test_insert_attribute_after(t : T?) {
    t |> run("insert_attribute_after places attribute correctly") <| @(t : T?) {
        with_doc() $(doc) {
            var root = append_child(doc as xml_node, "root")
            var a = append_attribute(root, "a")
            set_value(a, "1")
            var c = append_attribute(root, "c")
            set_value(c, "3")
            var b = insert_attribute_after(root, "b", a)
            set_value(b, "2")
            t |> equal(root.first_attribute.next_attribute.name, "b")
        }
    }
}

[test(description = "insert_attribute_before places a new attribute directly before the given one")]
def test_insert_attribute_before(t : T?) {
    t |> run("insert_attribute_before places attribute correctly") <| @(t : T?) {
        with_doc() $(doc) {
            var root = append_child(doc as xml_node, "root")
            var a = append_attribute(root, "a")
            set_value(a, "1")
            var c = append_attribute(root, "c")
            set_value(c, "3")
            var b = insert_attribute_before(root, "b", c)
            set_value(b, "2")
            t |> equal(root.first_attribute.next_attribute.name, "b")
        }
    }
}

[test(description = "remove_attribute removes the given attribute")]
def test_remove_attribute(t : T?) {
    t |> run("remove_attribute removes attribute object") <| @(t : T?) {
        with_doc() $(doc) {
            var root = append_child(doc as xml_node, "root")
            add_attr(root, "keep", "yes")
            var rm = append_attribute(root, "remove_me")
            set_value(rm, "no")
            remove_attribute(root, rm)
            t |> success(!attribute(root, "remove_me").ok)
            t |> success(attribute(root, "keep").ok)
        }
    }
}

[test(description = "remove_attribute by name removes the named attribute")]
def test_remove_attribute_by_name(t : T?) {
    t |> run("remove_attribute by name") <| @(t : T?) {
        with_doc() $(doc) {
            var root = append_child(doc as xml_node, "root")
            add_attr(root, "a", "1")
            add_attr(root, "b", "2")
            add_attr(root, "c", "3")
            remove_attribute(root, "b")
            t |> success(!attribute(root, "b").ok)
            t |> success(attribute(root, "a").ok)
            t |> success(attribute(root, "c").ok)
        }
    }
}

[test(description = "remove_attributes removes every attribute")]
def test_remove_attributes(t : T?) {
    t |> run("remove_attributes clears all attributes") <| @(t : T?) {
        with_doc() $(doc) {
            var root = append_child(doc as xml_node, "root")
            add_attr(root, "a", "1")
            add_attr(root, "b", "2")
            remove_attributes(root)
            t |> success(!root.first_attribute.ok)
        }
    }
}

[test(description = "attribute set_name renames an attribute")]
def test_attribute_set_name(t : T?) {
    t |> run("set_name renames an attribute") <| @(t : T?) {
        with_doc() $(doc) {
            var root = append_child(doc as xml_node, "root")
            var a = append_attribute(root, "old_name")
            set_value(a, "val")
            set_name(a, "new_name")
            t |> equal(a.name, "new_name")
            t |> success(attribute(root, "new_name").ok)
        }
    }
}

[test(description = "attribute set_value stores each supported value type as text")]
def test_attribute_set_value_types(t : T?) {
    t |> run("set_value works for all types") <| @(t : T?) {
        with_doc() $(doc) {
            var root = append_child(doc as xml_node, "root")
            var ai = append_attribute(root, "i")
            set_value(ai, 42)
            t |> equal(ai as int, 42)
            var au = append_attribute(root, "u")
            set_value(au, 100u)
            t |> equal(au as uint, 100u)
            var af = append_attribute(root, "f")
            set_value(af, 3.14)
            t |> success(abs(af as float -3.14) < 0.01)
            var ad = append_attribute(root, "d")
            set_value(ad, 2.718lf)
            t |> success(abs(ad as double - 2.718lf) < 0.01lf)
            var ab = append_attribute(root, "b")
            set_value(ab, true)
            t |> equal(ab as bool, true)
        }
    }
}

// ============ Text mutation ============

[test(description = "text set with a string changes the text content")]
def test_text_set_string(t : T?) {
    t |> run("text set(string) changes text content") <| @(t : T?) {
        with_doc() $(doc) {
            var root = append_child(doc as xml_node, "root")
            set(root.text, "hello")
            t |> equal(root.text as string, "hello")
            set(root.text, "world")
            t |> equal(root.text as string, "world")
        }
    }
}

[test(description = "text set with an int stores its decimal text")]
def test_text_set_int(t : T?) {
    t |> run("text set(int) stores numeric text") <| @(t : T?) {
        with_doc() $(doc) {
            var root = append_child(doc as xml_node, "root")
            set(root.text, 42)
            t |> equal(root.text as int, 42)
        }
    }
}

[test(description = "text set with a float stores its text")]
def test_text_set_float(t : T?) {
    t |> run("text set(float) stores float text") <| @(t : T?) {
        with_doc() $(doc) {
            var root = append_child(doc as xml_node, "root")
            set(root.text, 3.14)
            t |> success(abs(root.text as float -3.14) < 0.01)
        }
    }
}

[test(description = "text set with a double stores its text")]
def test_text_set_double(t : T?) {
    t |> run("text set(double) stores double text") <| @(t : T?) {
        with_doc() $(doc) {
            var root = append_child(doc as xml_node, "root")
            set(root.text, 2.718281828lf)
            t |> success(abs(root.text as double - 2.718281828lf) < 0.0001lf)
        }
    }
}

[test(description = "text set with a uint stores its decimal text")]
def test_text_set_uint(t : T?) {
    t |> run("text set(uint) stores unsigned text") <| @(t : T?) {
        with_doc() $(doc) {
            var root = append_child(doc as xml_node, "root")
            set(root.text, 999u)
            t |> equal(root.text as uint, 999u)
        }
    }
}

[test(description = "text set with a bool stores its text")]
def test_text_set_bool(t : T?) {
    t |> run("text set(bool) stores boolean text") <| @(t : T?) {
        with_doc() $(doc) {
            var root = append_child(doc as xml_node, "root")
            set(root.text, true)
            t |> equal(root.text as bool, true)
            set(root.text, false)
            t |> equal(root.text as bool, false)
        }
    }
}

// ============ Copy / Move operations ============

[test(description = "append_copy duplicates a node under the target parent")]
def test_append_copy_node(t : T?) {
    t |> run("append_copy duplicates a node") <| @(t : T?) {
        with_doc() $(doc) {
            var dnode = doc as xml_node
            var root = append_child(dnode, "root")
            var src = append_child(root, "source")
            add_attr(src, "id", "1")
            add_child(src, "inner", "data")
            var root2 = append_child(dnode, "root2")
            let copy = append_copy(root2, src)
            t |> equal(copy.name, "source")
            t |> equal(node_attr(copy, "id"), "1")
            t |> equal(node_text(copy, "inner"), "data")
            // Original still exists
            t |> success(child(root, "source").ok)
        }
    }
}

[test(description = "prepend_copy duplicates a node before the target parent's children")]
def test_prepend_copy_node(t : T?) {
    t |> run("prepend_copy duplicates at beginning") <| @(t : T?) {
        with_doc() $(doc) {
            var root = append_child(doc as xml_node, "root")
            append_child(root, "existing")
            let src = append_child(root, "source")
            var target = append_child(doc as xml_node, "target")
            append_child(target, "before")
            prepend_copy(target, src)
            t |> equal(target.first_child.name, "source")
        }
    }
}

[test(description = "append_copy duplicates an attribute onto a node")]
def test_append_copy_attr(t : T?) {
    t |> run("append_copy duplicates an attribute") <| @(t : T?) {
        with_doc() $(doc) {
            var root = append_child(doc as xml_node, "root")
            add_attr(root, "src_attr", "value")
            var target = append_child(root, "target")
            let src_a = attribute(root, "src_attr")
            append_copy(target, src_a)
            t |> equal(node_attr(target, "src_attr"), "value")
        }
    }
}

[test(description = "append_move moves a node to a new parent")]
def test_append_move_node(t : T?) {
    t |> run("append_move moves node to new parent") <| @(t : T?) {
        with_doc() $(doc) {
            var root = append_child(doc as xml_node, "root")
            var src = append_child(root, "moveable")
            add_child(src, "data", "test")
            var target = append_child(root, "target")
            append_move(target, src)
            // Should be under target now
            t |> success(child(target, "moveable").ok)
            // Should no longer be a direct child of root (but root still has "target")
            var direct_children : array<string>
            for_each_child(root) $(ch) {
                direct_children |> push(ch.name)
            }
            t |> success(!direct_children |> has("moveable"))
        }
    }
}

// ============ Append child by type ============

[test(description = "append_child with a node type creates a node of that type")]
def test_append_child_by_type(t : T?) {
    t |> run("append_child with node type creates typed node") <| @(t : T?) {
        with_doc() $(doc) {
            var root = append_child(doc as xml_node, "root")
            var comment = append_child(root, xml_node_type.node_comment)
            set_value(comment, "this is a comment")
            t |> equal(int(comment._type), int(xml_node_type.node_comment))
            t |> equal(comment.value, "this is a comment")
        }
    }
}

[test(description = "prepend_child with a node type creates a node of that type before the existing children")]
def test_prepend_child_by_type(t : T?) {
    t |> run("prepend_child with node type") <| @(t : T?) {
        with_doc() $(doc) {
            var root = append_child(doc as xml_node, "root")
            append_child(root, "element")
            var cdata = prepend_child(root, xml_node_type.node_cdata)
            set_value(cdata, "raw data")
            t |> equal(int(root.first_child._type), int(xml_node_type.node_cdata))
        }
    }
}

// ============ has() helper for array<string> ============

def has(arr : array<string>; val : string) : bool {
    for (s in arr) {
        if (s == val) {
            return true
        }
    }
    return false
}
