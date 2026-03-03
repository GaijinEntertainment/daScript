options gen2
require pugixml

[export]
def main() {
    // Test 1: Create document and build XML
    var doc = new xml_document()
    var doc_node = document_as_node(doc)
    var root = append_child(doc_node, "catalog")
    var book = append_child(root, "book")
    set_name(book, "book")
    var attr = append_attribute(book, "id")
    set_value(attr, "bk101")
    var title = append_child(book, "title")
    set(title.text, "XML Programming")
    var price = append_child(book, "price")
    set(price.text, 29.99)
    print("=== Created XML ===\n")
    let xml_str = save_string(doc, "  ", format_default, xml_encoding.encoding_auto)
    print("{xml_str}")

    // Test 2: Navigation
    print("\n=== Navigation ===\n")
    let root2 = doc.document_element
    print("root name: {root2.name}\n")
    print("root ok: {root2.ok}\n")
    print("first child: {root2.first_child.name}\n")
    print("last child: {root2.last_child.name}\n")
    let book2 = child(root2, "book")
    let title_text = child_value(book2, "title")
    print("book child_value(title): {title_text}\n")
    let title_node = child(book2, "title")
    let title_str = as_string(title_node.text, "")
    print("title text: {title_str}\n")
    let price_node = child(book2, "price")
    print("price text: {as_float(price_node.text, 0.0)}\n")

    // Test 3: Attribute access
    print("\n=== Attributes ===\n")
    let id_attr = attribute(book2, "id")
    print("id attr ok: {id_attr.ok}\n")
    let id_val = as_string(id_attr, "")
    print("id value: {id_val}\n")

    // Test 4: XPath (string query)
    print("\n=== XPath ===\n")
    let xpath_result = select_node(root2, "book[@id='bk101']/title")
    print("xpath found: {xpath_result.ok}\n")
    print("xpath node name: {xpath_result.node.name}\n")
    let xpath_text = as_string(xpath_result.node.text, "")
    print("xpath node text: {xpath_text}\n")

    // Test 5: Compiled XPath
    var q = xpath_compile("//book")
    print("query ok: {q.ok}\n")
    let result = evaluate_node(q, root2)
    print("evaluate_node: {result.node.name}\n")
    unsafe { delete q; }

    // Test 6: Load from string
    var doc2 = new xml_document()
    var parse_result : xml_parse_result
    load_string(doc2, "<root><item key=\"a\">1</item><item key=\"b\">2</item></root>", parse_result, parse_default)
    print("\n=== Load from string ===\n")
    print("parse status: {parse_result.status}\n")
    var items = select_nodes(doc2.document_element, "item")
    let n_items = items.size
    print("items found: {n_items}\n")
    for (i in range(n_items)) {
        let n = at(items, i)
        let item_key = as_string(attribute(n.node, "key"), "")
        let item_text = as_string(n.node.text, "")
        print("  item key={item_key} text={item_text}\n")
    }
    unsafe { delete items; }

    // Test 7: Constants
    print("\n=== Constants ===\n")
    print("parse_default = {parse_default}\n")
    print("format_default = {format_default}\n")

    unsafe { delete doc2; }
    unsafe { delete doc; }
    print("\nAll smoke tests passed!\n")
}
