options gen2

require pugixml/PUGIXML_boost
require dastest/testing_boost public

// Mixed content: text nodes ("noise"/"more") interleaved between elements, and a non-"item" <sep/>
// element, so the reverse walks must skip non-element / non-matching nodes exactly like the forward ones.
let XML = ("<root>" +
    "<item id=\"1\"/>noise" +
    "<item id=\"2\"/>" +
    "<sep/>" +
    "<item id=\"3\"/>more" +
    "<item id=\"4\"/>" +
    "</root>")

def ids_each_reverse(root : xml_node) : array<int> {
    return <- [for (ch in each_child_reverse(root)); ch["id"] as int]
}

def ids_block_reverse(root : xml_node) : array<int> {
    var ids : array<int>
    root |> for_each_child_reverse() $(ch) {
        ids |> push(ch["id"] as int)
    }
    return <- ids
}

def ids_each_reverse_named(root : xml_node; name : string) : array<int> {
    return <- [for (ch in each_child_reverse(root, name)); ch["id"] as int]
}

def ids_block_reverse_named(root : xml_node; name : string) : array<int> {
    var ids : array<int>
    root |> for_each_child_reverse(name) $(ch) {
        ids |> push(ch["id"] as int)
    }
    return <- ids
}

def ids_each_forward(root : xml_node) : array<int> {
    return <- [for (ch in each_child(root)); ch["id"] as int]
}

def eq_ints(t : T?; a : array<int>; b : array<int>; msg : string) {
    t |> equal(length(a), length(b))
    if (length(a) == length(b)) {
        for (i in range(length(a))) {
            t |> success(a[i] == b[i], "{msg}[{i}]: {a[i]} == {b[i]}")
        }
    }
}

[test]
def test_each_child_reverse_all(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        // each_child[_reverse] (unnamed) mirrors forward: it yields ALL child NODES, text included
        // (text/<sep> have no id → 0). Forward is [1,0,2,0,3,0,4]; reverse is its exact mirror.
        var got <- ids_each_reverse(doc.document_element)
        var exp <- [4, 0, 3, 0, 2, 0, 1]
        eq_ints(t, got, exp, "each_reverse_all")
        delete got; delete exp
    }
}

[test]
def test_for_each_child_reverse_all(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        var got <- ids_block_reverse(doc.document_element)
        var exp <- [4, 0, 3, 0, 2, 0, 1]
        eq_ints(t, got, exp, "block_reverse_all")
        delete got; delete exp
    }
}

[test]
def test_each_child_reverse_named(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        // only <item> elements, reverse: 4, 3, 2, 1 (<sep> excluded by tag)
        var got <- ids_each_reverse_named(doc.document_element, "item")
        var exp <- [4, 3, 2, 1]
        eq_ints(t, got, exp, "each_reverse_named")
        delete got; delete exp
    }
}

[test]
def test_for_each_child_reverse_named(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        var got <- ids_block_reverse_named(doc.document_element, "item")
        var exp <- [4, 3, 2, 1]
        eq_ints(t, got, exp, "block_reverse_named")
        delete got; delete exp
    }
}

[test]
def test_reverse_round_trip(t : T?) {
    parse_xml(XML) $(doc, ok) {
        t |> success(ok)
        // reverse[i] must equal forward[n-1-i]
        var fwd <- ids_each_forward(doc.document_element)
        var rev <- ids_each_reverse(doc.document_element)
        t |> equal(length(fwd), length(rev))
        if (length(fwd) == length(rev)) {
            let n = length(fwd)
            for (i in range(n)) {
                t |> success(fwd[i] == rev[n - 1 - i], "round_trip[{i}]")
            }
        }
        delete fwd; delete rev
    }
}

[test]
def test_reverse_empty(t : T?) {
    parse_xml("<root></root>") $(doc, ok) {
        t |> success(ok)
        var got <- ids_each_reverse(doc.document_element)
        t |> equal(length(got), 0)
        delete got
    }
}
