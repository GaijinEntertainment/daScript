options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4
options strict_smart_pointers = true

module PUGIXML_boost shared public

//! High-level helpers for the pugixml XML library.
//!
//! Re-exports the core ``pugixml`` module and adds iterator helpers,
//! RAII document management, quick node/attribute accessors, builder
//! shortcuts, and serialization utilities.

require pugixml public

// ============ Iteration helpers ============

def for_each_child(node : xml_node; blk : block<(child : xml_node) : void>) {
    //! Iterates over all child elements of *node*.
    var ch = node.first_child
    while (ch.ok) {
        var next = ch.next_sibling   // capture before block may mutate
        invoke(blk, ch)
        ch = next
    }
}

def for_each_child(node : xml_node; name : string; blk : block<(child : xml_node) : void>) {
    //! Iterates over child elements of *node* whose tag matches *name*.
    var ch = child(node, name)
    while (ch.ok) {
        var next = next_sibling(ch, name)   // capture before block may mutate
        invoke(blk, ch)
        ch = next
    }
}

def for_each_attribute(node : xml_node; blk : block<(attr : xml_attribute) : void>) {
    //! Iterates over all attributes of *node*.
    var a = node.first_attribute
    while (a.ok) {
        var next = a.next_attribute   // capture before block may mutate
        invoke(blk, a)
        a = next
    }
}

def for_each(node_set : xpath_node_set?; blk : block<(xn : xpath_node) : void>) {
    //! Iterates over all ``xpath_node`` entries in an ``xpath_node_set``.
    let n = node_set.size
    for (i in range(n)) {
        invoke(blk, at(node_set, i))
    }
}

// ============ RAII document management ============

def open_xml(filename : string; blk : block<(doc : xml_document?; success : bool) : void>) {
    //! Opens an XML file, invokes *blk* with the parsed document and a
    //! success flag, then automatically frees the document. The caller
    //! never needs ``unsafe { delete ... }``.
    var doc = new xml_document()
    var result : xml_parse_result
    let ok = load_document(doc, filename, result)
    invoke(blk, doc, ok)
    unsafe { delete doc; }
}

def parse_xml(content : string; blk : block<(doc : xml_document?; success : bool) : void>) {
    //! Parses an XML string, invokes *blk* with the document and a
    //! success flag, then automatically frees the document.
    var doc = new xml_document()
    var result : xml_parse_result
    let ok = load_string(doc, content, result, parse_default)
    invoke(blk, doc, ok)
    unsafe { delete doc; }
}

def with_doc(blk : block<(doc : xml_document?) : void>) {
    //! Creates an empty ``xml_document``, invokes *blk*, then frees it.
    //! Use this when building XML from scratch.
    var doc = new xml_document()
    invoke(blk, doc)
    unsafe { delete doc; }
}

// ============ Quick accessors ============

def node_text(node : xml_node; child_name : string; default_value : string = "") : string {
    //! Returns the text content of the child element named *child_name*,
    //! or *default_value* if the child does not exist or has no text.
    let ch = child(node, child_name)
    if (!ch.ok) {
        return default_value
    }
    return as_string(ch.text, default_value)
}

def node_attr(node : xml_node; attr_name : string; default_value : string = "") : string {
    //! Returns the string value of attribute *attr_name*,
    //! or *default_value* if no such attribute exists.
    let a = attribute(node, attr_name)
    if (!a.ok) {
        return default_value
    }
    return as_string(a, default_value)
}

def node_attr_int(node : xml_node; attr_name : string; default_value : int = 0) : int {
    //! Returns the integer value of attribute *attr_name*.
    let a = attribute(node, attr_name)
    return as_int(a, default_value)
}

def node_attr_float(node : xml_node; attr_name : string; default_value : float = 0.0) : float {
    //! Returns the float value of attribute *attr_name*.
    let a = attribute(node, attr_name)
    return as_float(a, default_value)
}

def node_attr_bool(node : xml_node; attr_name : string; default_value : bool = false) : bool {
    //! Returns the boolean value of attribute *attr_name*.
    let a = attribute(node, attr_name)
    return as_bool(a, default_value)
}

// ============ Builder helpers ============

def add_child(var node : xml_node; name : string; text_content : string) : xml_node {
    //! Appends a child element with *name* and sets its text content.
    //! Returns the new child node.
    var ch = append_child(node, name)
    set(ch.text, text_content)
    return ch
}

def add_child_ex(var node : xml_node; name, attr_name, attr_value : string) : xml_node {
    //! Appends a child element with *name* and a single attribute.
    //! Returns the new child node.
    var ch = append_child(node, name)
    var a = append_attribute(ch, attr_name)
    set_value(a, attr_value)
    return ch
}

def add_attr(var node : xml_node; name, value : string) : xml_attribute {
    //! Appends an attribute with *name* and *value* (string).
    var a = append_attribute(node, name)
    set_value(a, value)
    return a
}

def add_attr(var node : xml_node; name : string; value : int) : xml_attribute {
    //! Appends an attribute with *name* and *value* (int).
    var a = append_attribute(node, name)
    set_value(a, value)
    return a
}

def add_attr(var node : xml_node; name : string; value : float) : xml_attribute {
    //! Appends an attribute with *name* and *value* (float).
    var a = append_attribute(node, name)
    set_value(a, value)
    return a
}

def add_attr(var node : xml_node; name : string; value : bool) : xml_attribute {
    //! Appends an attribute with *name* and *value* (bool).
    var a = append_attribute(node, name)
    set_value(a, value)
    return a
}

// ============ Serialization helpers ============

def to_string(doc : xml_document?) : string {
    //! Serializes the entire document to a pretty-printed XML string
    //! with two-space indentation and default formatting.
    return save_string(doc, "  ", format_default, xml_encoding.encoding_auto)
}

def to_string(node : xml_node) : string {
    //! Serializes a single node (and its subtree) to an XML string
    //! with two-space indentation.
    return print_to_string(node, "  ", format_default, xml_encoding.encoding_auto)
}

// ============ Type conversion operators (is/as) ============

// -- xml_attribute conversions --

def operator is int(a : xml_attribute) : bool {
    //! Returns ``true`` if the attribute exists (has a value convertible to int).
    return a.ok
}

def operator as int(a : xml_attribute) : int {
    //! Converts an ``xml_attribute`` value to ``int`` (default ``0``).
    return as_int(a, 0)
}

def operator is float(a : xml_attribute) : bool {
    //! Returns ``true`` if the attribute exists.
    return a.ok
}

def operator as float(a : xml_attribute) : float {
    //! Converts an ``xml_attribute`` value to ``float`` (default ``0.0``).
    return as_float(a, 0.0)
}

def operator is double(a : xml_attribute) : bool {
    //! Returns ``true`` if the attribute exists.
    return a.ok
}

def operator as double(a : xml_attribute) : double {
    //! Converts an ``xml_attribute`` value to ``double`` (default ``0.0lf``).
    return as_double(a, 0.0lf)
}

def operator is bool(a : xml_attribute) : bool {
    //! Returns ``true`` if the attribute exists.
    return a.ok
}

def operator as bool(a : xml_attribute) : bool {
    //! Converts an ``xml_attribute`` value to ``bool`` (default ``false``).
    return as_bool(a, false)
}

def operator is string(a : xml_attribute) : bool {
    //! Returns ``true`` if the attribute exists.
    return a.ok
}

def operator as string(a : xml_attribute) : string {
    //! Converts an ``xml_attribute`` value to ``string`` (default ``""``).
    return as_string(a, "")
}

def operator is uint(a : xml_attribute) : bool {
    //! Returns ``true`` if the attribute exists.
    return a.ok
}

def operator as uint(a : xml_attribute) : uint {
    //! Converts an ``xml_attribute`` value to ``uint`` (default ``0u``).
    return as_uint(a, 0u)
}

// -- xml_text conversions --

def operator is int(t : xml_text) : bool {
    //! Returns ``true`` if the text node is non-empty.
    return !t.empty
}

def operator as int(t : xml_text) : int {
    //! Converts ``xml_text`` content to ``int`` (default ``0``).
    return as_int(t, 0)
}

def operator is float(t : xml_text) : bool {
    //! Returns ``true`` if the text node is non-empty.
    return !t.empty
}

def operator as float(t : xml_text) : float {
    //! Converts ``xml_text`` content to ``float`` (default ``0.0``).
    return as_float(t, 0.0)
}

def operator is double(t : xml_text) : bool {
    //! Returns ``true`` if the text node is non-empty.
    return !t.empty
}

def operator as double(t : xml_text) : double {
    //! Converts ``xml_text`` content to ``double`` (default ``0.0lf``).
    return as_double(t, 0.0lf)
}

def operator is bool(t : xml_text) : bool {
    //! Returns ``true`` if the text node is non-empty.
    return !t.empty
}

def operator as bool(t : xml_text) : bool {
    //! Converts ``xml_text`` content to ``bool`` (default ``false``).
    return as_bool(t, false)
}

def operator is string(t : xml_text) : bool {
    //! Returns ``true`` if the text node is non-empty.
    return !t.empty
}

def operator as string(t : xml_text) : string {
    //! Converts ``xml_text`` content to ``string`` (default ``""``).
    return as_string(t, "")
}

def operator is uint(t : xml_text) : bool {
    //! Returns ``true`` if the text node is non-empty.
    return !t.empty
}

def operator as uint(t : xml_text) : uint {
    //! Converts ``xml_text`` content to ``uint`` (default ``0u``).
    return as_uint(t, 0u)
}

// -- xml_node / xml_document serialization --

def operator as string(node : xml_node) : string {
    //! Serializes an ``xml_node`` (and its subtree) to an XML string.
    return to_string(node)
}

// ============ XPath helpers ============

def with_xpath(query_str : string; blk : block<(query : xpath_query?) : void>) {
    //! Compiles an XPath expression, invokes *blk* with the compiled
    //! query handle, then frees it. Avoids manual ``delete``.
    var q = xpath_compile(query_str)
    invoke(blk, q)
    unsafe { delete q; }
}

def select_text(node : xml_node; xpath_query : string; default_value : string = "") : string {
    //! Runs an XPath query and returns the text content of the first
    //! matching node, or *default_value* if nothing matches.
    let result = select_node(node, xpath_query)
    if (!result.ok) {
        return default_value
    }
    let rn = result.node
    if (rn.ok) {
        return as_string(rn.text, default_value)
    }
    return default_value
}

def select_value(node : xml_node; xpath_query : string; default_value : string = "") : string {
    //! Runs an XPath query and returns the value of the first matching
    //! node. If the result is an attribute, returns its string value;
    //! if an element, returns its text content.
    let result = select_node(node, xpath_query)
    if (!result.ok) {
        return default_value
    }
    let ra = result.attribute
    if (ra.ok) {
        return as_string(ra, default_value)
    }
    let rn = result.node
    if (rn.ok) {
        return as_string(rn.text, default_value)
    }
    return default_value
}

def for_each_select(node : xml_node; xpath_query : string; blk : block<(xn : xpath_node) : void>) {
    //! Selects all nodes matching *xpath_query* and invokes *blk* for
    //! each result. The temporary ``xpath_node_set`` is freed automatically.
    var ns = select_nodes(node, xpath_query)
    let n = ns.size
    for (i in range(n)) {
        invoke(blk, at(ns, i))
    }
    unsafe { delete ns; }
}
