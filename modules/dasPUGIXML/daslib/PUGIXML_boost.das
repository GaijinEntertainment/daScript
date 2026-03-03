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
require daslib/apply
require daslib/contracts
require daslib/templates
require daslib/enum_trait
require daslib/json_boost
require daslib/static_let
require strings

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

[unsafe_outside_of_for]
def each(node_set : xpath_node_set?) : iterator<xpath_node> {
    //! Returns a lazy iterator over all ``xpath_node`` entries in an
    //! ``xpath_node_set``. Intended for use in ``for`` loops.
    //! Because the iterator is named ``each``, the call can be omitted:
    //!
    //!   ``for (xn in node_set) { ... }``
    return <- generator<xpath_node>() <| $ {
        let n = node_set.size
        for (i in range(n)) {
            yield at(node_set, i)
        }
        return false
    }
}

[unsafe_outside_of_for]
def each_child(node : xml_node) : iterator<xml_node> {
    //! Returns a lazy iterator over all child elements of *node*.
    //! Intended for use in ``for`` loops:
    //!
    //!   ``for (ch in each_child(node)) { ... }``
    return <- generator<xml_node>() <| $ {
        var ch = node.first_child
        while (ch.ok) {
            var next = ch.next_sibling   // capture before yield (user may mutate)
            yield ch
            ch = next
        }
        return false
    }
}

[unsafe_outside_of_for]
def each_child(node : xml_node; name : string) : iterator<xml_node> {
    //! Returns a lazy iterator over child elements of *node* whose tag
    //! matches *name*. Intended for use in ``for`` loops:
    //!
    //!   ``for (ch in each_child(node, "item")) { ... }``
    return <- generator<xml_node>() <| $ {
        var ch = child(node, name)
        while (ch.ok) {
            var next = next_sibling(ch, name)   // capture before yield
            yield ch
            ch = next
        }
        return false
    }
}

[unsafe_outside_of_for]
def each_attribute(node : xml_node) : iterator<xml_attribute> {
    //! Returns a lazy iterator over all attributes of *node*.
    //! Intended for use in ``for`` loops:
    //!
    //!   ``for (a in each_attribute(node)) { ... }``
    return <- generator<xml_attribute>() <| $ {
        var a = node.first_attribute
        while (a.ok) {
            var next = a.next_attribute   // capture before yield
            yield a
            a = next
        }
        return false
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

def operator [] (node : xml_node; attr_name : string) : xml_attribute {
    //! Returns the attribute named *attr_name*. Combine with ``is``/``as``
    //! operators for typed access:
    //!
    //!   ``node["price"] as float``
    //!
    //!   ``if (node["discount"] is float) { ... }``
    return attribute(node, attr_name)
}

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

// ============ Builder EDSL ============

def tag(var node : xml_node; name : string; blk : block<(var child : xml_node) : void>) : xml_node {
    //! Appends a child element with *name*, invokes *blk* with it, and
    //! returns the new child. Use with pipe+block for nested XML:
    //!
    //!   ``node |> tag("book") <| $(var book) { book |> tag("title", "...") }``
    var ch = append_child(node, name)
    invoke(blk, ch)
    return ch
}

def tag(var node : xml_node; name : string; text_content : string) : xml_node {
    //! Appends a child element with *name* and text content. Returns
    //! the new child. Use for leaf elements:
    //!
    //!   ``node |> tag("title", "The C Programming Language")``
    var ch = append_child(node, name)
    set(ch.text, text_content)
    return ch
}

def tag(var node : xml_node; name : string) : xml_node {
    //! Appends a bare child element with *name*. Returns the new child.
    return append_child(node, name)
}

def tag(doc : xml_document?; name : string; blk : block<(var child : xml_node) : void>) : xml_node {
    //! Appends a child element to a document, invokes *blk*, returns it.
    //! Allows ``doc |> tag("root") <| $(var root) { ... }`` without manual
    //! ``doc as xml_node``.
    var dnode = doc as xml_node
    var ch = append_child(dnode, name)
    invoke(blk, ch)
    return ch
}

def tag(doc : xml_document?; name : string; text_content : string) : xml_node {
    //! Appends a child element with text to a document.
    var dnode = doc as xml_node
    var ch = append_child(dnode, name)
    set(ch.text, text_content)
    return ch
}

def tag(doc : xml_document?; name : string) : xml_node {
    //! Appends a bare child element to a document.
    var dnode = doc as xml_node
    return append_child(dnode, name)
}

def attr(var node : xml_node; name : string; value : string) : xml_node {
    //! Appends an attribute and returns the *parent node* for chaining:
    //!
    //!   ``book |> attr("isbn", "978-...") |> attr("lang", "en")``
    var a = append_attribute(node, name)
    set_value(a, value)
    return node
}

def attr(var node : xml_node; name : string; value : int) : xml_node {
    //! Appends an int attribute, returns parent for chaining.
    var a = append_attribute(node, name)
    set_value(a, value)
    return node
}

def attr(var node : xml_node; name : string; value : float) : xml_node {
    //! Appends a float attribute, returns parent for chaining.
    var a = append_attribute(node, name)
    set_value(a, value)
    return node
}

def attr(var node : xml_node; name : string; value : bool) : xml_node {
    //! Appends a bool attribute, returns parent for chaining.
    var a = append_attribute(node, name)
    set_value(a, value)
    return node
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

// -- xml_node / xml_document conversions --

def operator as string(node : xml_node) : string {
    //! Serializes an ``xml_node`` (and its subtree) to an XML string.
    return to_string(node)
}

def operator as xml_node(doc : xml_document?) : xml_node {
    //! Returns the document as an ``xml_node`` handle for mutation APIs
    //! that require a node (e.g. ``append_child``).
    return document_as_node(doc)
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

// ============ Struct ↔ XML serialization ============

// --- from_XML: deserialize xml_node → native value ---

[generic, template(ent), unused_argument(ent)]
def from_XML(node : xml_node; ent : string; defV : string = "") : string {
    //! Reads the text content of *node* as a ``string``.
    return as_string(node.text, defV)
}

[generic, template(ent), unused_argument(ent)]
def from_XML(node : xml_node; ent : int; defV : int = 0) : int {
    //! Reads the text content of *node* as an ``int``.
    return as_int(node.text, defV)
}

[generic, template(ent), unused_argument(ent)]
def from_XML(node : xml_node; ent : float; defV : float = 0.0) : float {
    //! Reads the text content of *node* as a ``float``.
    return as_float(node.text, defV)
}

[generic, template(ent), unused_argument(ent)]
def from_XML(node : xml_node; ent : double; defV : double = 0.0lf) : double {
    //! Reads the text content of *node* as a ``double``.
    return as_double(node.text, defV)
}

[generic, template(ent), unused_argument(ent)]
def from_XML(node : xml_node; ent : bool; defV : bool = false) : bool {
    //! Reads the text content of *node* as a ``bool``.
    return as_bool(node.text, defV)
}

[generic, template(ent), unused_argument(ent)]
def from_XML(node : xml_node; ent : uint; defV : uint = 0u) : uint {
    //! Reads the text content of *node* as a ``uint``.
    return as_uint(node.text, defV)
}

[generic, template(ent), unused_argument(ent)]
def from_XML(node : xml_node; ent : int8; defV : int8 = int8(0)) : int8 {
    //! Reads the text content of *node* as an ``int8``.
    return int8(as_int(node.text, int(defV)))
}

[generic, template(ent), unused_argument(ent)]
def from_XML(node : xml_node; ent : uint8; defV : uint8 = uint8(0)) : uint8 {
    //! Reads the text content of *node* as a ``uint8``.
    return uint8(as_uint(node.text, uint(defV)))
}

[generic, template(ent), unused_argument(ent)]
def from_XML(node : xml_node; ent : int16; defV : int16 = int16(0)) : int16 {
    //! Reads the text content of *node* as an ``int16``.
    return int16(as_int(node.text, int(defV)))
}

[generic, template(ent), unused_argument(ent)]
def from_XML(node : xml_node; ent : uint16; defV : uint16 = uint16(0)) : uint16 {
    //! Reads the text content of *node* as a ``uint16``.
    return uint16(as_uint(node.text, uint(defV)))
}

[generic, template(ent), unused_argument(ent)]
def from_XML(node : xml_node; ent : int64; defV : int64 = 0l) : int64 {
    //! Reads the text content of *node* as an ``int64``.
    return as_int64(node.text, defV)
}

[generic, template(ent), unused_argument(ent)]
def from_XML(node : xml_node; ent : uint64; defV : uint64 = 0ul) : uint64 {
    //! Reads the text content of *node* as a ``uint64``.
    return as_uint64(node.text, defV)
}

[unused_argument(anything)]
def from_XML(node : xml_node; anything : auto(TT)) {
    //! Deserializes an XML node tree into a native value.
    //! Struct fields become child elements; arrays become sequences of
    //! child elements. Missing elements yield default values.
    static_if (typeinfo is_dim(anything)) {
        var ret : TT[typeinfo dim(anything)]
        var ch = node.first_child
        for (i in 0..typeinfo dim(anything)) {
            if (ch.ok) {
                var next_ch = ch.next_sibling
                move_to_ref(ret[i], _::from_XML(ch, decltype_noref(anything[0])))
                ch = next_ch
            }
        }
        return <- ret
    } static_elif (typeinfo is_struct(anything)) {
        var ret <- default<TT -const>
        apply(ret) $(name : string; var field; annotations) {
            static_let("{typeinfo typename(type<TT-const-&>)}`{name}") {
                var fieldState : JsonFieldState
                var fieldStateParsed : bool = false
            }
            if (!fieldStateParsed) {
                fieldState <- parse_json_annotation(name, annotations)
                fieldStateParsed = true
            }
            let ch = child(node, fieldState.argName)
            if (ch.ok) {
                move_to_ref(field, _::from_XML(ch, decltype_noref(field)))
            }
        }
        return <- ret
    } static_elif (typeinfo is_tuple(anything)) {
        var ret <- default<TT -const>
        apply(ret) $(name : string; var field) {
            let ch = child(node, name)
            if (ch.ok) {
                move_to_ref(field, _::from_XML(ch, decltype_noref(field)))
            }
        }
        return <- ret
    } static_elif (typeinfo is_variant(anything)) {
        var ret : TT
        let index = node_attr_int(node, "_variant", 0)
        unsafe {
            set_variant_index(ret, index)
        }
        apply(ret) $(name : string; var field) {
            let ch = child(node, name)
            if (ch.ok) {
                move_to_ref(field, _::from_XML(ch, decltype_noref(field)))
            }
        }
        return <- ret
    } static_elif (typeinfo is_array(anything)) {
        var ret : TT
        var ch = node.first_child
        while (ch.ok) {
            var next_ch = ch.next_sibling
            static_if (typeinfo can_copy(anything[0])) {
                ret |> push_clone <| _::from_XML(ch, decltype_noref(anything[0]))
            } else {
                ret |> emplace <| _::from_XML(ch, decltype_noref(anything[0]))
            }
            ch = next_ch
        }
        return <- ret
    } static_elif (typeinfo is_enum(anything)) {
        let txt = as_string(node.text, "")
        // Try name-based first, then int-based for @enum_as_int compatibility
        let nameResult = to_enum(type<TT -const>, txt, default<TT -const>)
        if ("{nameResult}" == txt) {
            return nameResult
        }
        // Fall back to int-based matching
        let intVal = as_int(node.text, 0)
        for (e in type<TT -const>) {
            if (int(e) == intVal) {
                return e
            }
        }
        return default<TT -const>
    } static_elif (typeinfo is_bitfield(anything)) {
        static_if (typeinfo sizeof(type<TT>) == 8) {
            return TT(as_uint64(node.text, 0ul))
        } else {
            return TT(as_uint(node.text, 0u))
        }
    } else {
        concept_assert(false, "unsupported from_XML type")
    }
}

[expect_any_vector_type(ent), template(ent), unused_argument(ent)]
def from_XML(node : xml_node; ent : auto(VecT); defV : VecT = VecT()) : VecT {
    //! Deserializes an XML node into a vector or range value.
    //! Each component (``x``, ``y``, ``z``, ``w``) is a child element.
    let VecV : auto(VecTT) = VecT()
    static_if (typeinfo vector_dim(VecV) == 2) {
        return VecTT(_::from_XML(child(node, "x"), decltype_noref(defV.x), defV.x), _::from_XML(child(node, "y"), decltype_noref(defV.y), defV.y))
    } static_elif (typeinfo vector_dim(VecV) == 3) {
        return VecTT(_::from_XML(child(node, "x"), decltype_noref(defV.x), defV.x), _::from_XML(child(node, "y"), decltype_noref(defV.y), defV.y), _::from_XML(child(node, "z"), decltype_noref(defV.z), defV.z))
    } static_elif (typeinfo vector_dim(VecV) == 4) {
        return VecTT(_::from_XML(child(node, "x"), decltype_noref(defV.x), defV.x), _::from_XML(child(node, "y"), decltype_noref(defV.y), defV.y), _::from_XML(child(node, "z"), decltype_noref(defV.z), defV.z), _::from_XML(child(node, "w"), decltype_noref(defV.w), defV.w))
    }
}

[unused_argument(anything)]
def from_XML(node : xml_node; anything : table<auto(KT); auto(VT)>) {
    //! Deserializes an XML node tree into a ``table<K;V>``.
    //! Each ``entry`` child has ``_key`` and ``_val`` sub-elements.
    var ret : table<KT; VT>
    var ch = node.first_child
    while (ch.ok) {
        var next_ch = ch.next_sibling
        let key_ch = child(ch, "_key")
        let val_ch = child(ch, "_val")
        unsafe {
            move_to_ref(ret[_::from_XML(key_ch, type<KT -&>)], _::from_XML(val_ch, type<VT -&>))
        }
        ch = next_ch
    }
    return <- ret
}

[unused_argument(anything)]
def from_XML(node : xml_node; anything : table<auto(KT)>) {
    //! Deserializes an XML node tree into a key-only ``table<K>`` (set).
    //! Each ``item`` child’s text content becomes a key.
    var ret : table<KT>
    var ch = node.first_child
    while (ch.ok) {
        var next_ch = ch.next_sibling
        ret |> insert(_::from_XML(ch, type<KT -&>))
        ch = next_ch
    }
    return <- ret
}

// --- XML helpers for table serialization ---

def XML_table(var node : xml_node; value : table<auto(KT); auto(VT)>) {
    for (k, v in keys(value), values(value)) {
        var ch = append_child(node, "entry")
        var key_ch = append_child(ch, "_key")
        _::XML(key_ch, k)
        var val_ch = append_child(ch, "_val")
        _::XML(val_ch, v)
    }
}

def XML_table(var node : xml_node; value : table<auto(KT)>) {
    for (k in keys(value)) {
        var ch = append_child(node, "item")
        _::XML(ch, k)
    }
}

// --- XML: serialize native value → xml_node children ---

[unused_argument(value)]
def XML(var node : xml_node; value : auto(TT)) {
    //! Serializes a native value into XML child elements of *node*.
    //! Structs/tuples: each field becomes a child element (via ``apply``).
    //! Variants: stores ``_variant`` attribute with the active index.
    //! Tables (key-value): each entry becomes an ``entry`` child with ``_key`` / ``_val`` sub-elements.
    //! Tables (key-only): each key becomes an ``item`` child.
    //! Arrays/dim: each element becomes an ``item`` child.
    //! Enumerations: stored as string name. Bitfields: stored as integer value.
    //! Primitives: sets the text content of *node*.
    static_if (typeinfo is_dim(value)) {
        for (item in value) {
            var ch = append_child(node, "item")
            _::XML(ch, item)
        }
    } static_elif (typeinfo is_struct(value)) {
        apply(value) $(name : string; field; annotations) {
            static_if (typeinfo is_function(field)) {
                pass
            } static_elif (typeinfo is_void_pointer(field)) {
                pass
            } static_elif (typeinfo is_pointer(field)) {
                if (field != null) {
                    static_let("{typeinfo typename(type<TT-const-&>)}`{name}") {
                        var fieldState : JsonFieldState
                        var fieldStateParsed : bool = false
                    }
                    if (!fieldStateParsed) {
                        fieldState <- parse_json_annotation(name, annotations)
                        fieldStateParsed = true
                    }
                    var ch = append_child(node, fieldState.argName)
                    _::XML(ch, field)
                }
            } else {
                static_let("{typeinfo typename(type<TT-const-&>)}`{name}") {
                    var fieldState : JsonFieldState
                    var fieldStateParsed : bool = false
                }
                if (!fieldStateParsed) {
                    fieldState <- parse_json_annotation(name, annotations)
                    fieldStateParsed = true
                }
                if (fieldState.enumAsInt) {
                    static_if (typeinfo is_enum(field)) {
                        var ch = append_child(node, fieldState.argName)
                        set(ch.text, int(field))
                        return
                    }
                } elif (fieldState.unescape) {
                    static_if (typeinfo is_string(field)) {
                        var ch = append_child(node, fieldState.argName)
                        set(ch.text, unescape(field))
                        return
                    }
                }
                var ch = append_child(node, fieldState.argName)
                _::XML(ch, field)
            }
        }
    } static_elif (typeinfo is_tuple(value)) {
        apply(value) $(name : string; field) {
            static_if (typeinfo is_function(field)) {
                pass
            } static_elif (typeinfo is_void_pointer(field)) {
                pass
            } else {
                var ch = append_child(node, name)
                _::XML(ch, field)
            }
        }
    } static_elif (typeinfo is_variant(value)) {
        node |> attr("_variant", variant_index(value))
        apply(value) $(name : string; field) {
            static_if (typeinfo is_function(field)) {
                pass
            } static_elif (typeinfo is_void_pointer(field)) {
                pass
            } else {
                var ch = append_child(node, name)
                _::XML(ch, field)
            }
        }
    } static_elif (typeinfo is_table(value)) {
        _::XML_table(node, value)
    } static_elif (typeinfo is_array(value)) {
        for (item in value) {
            var ch = append_child(node, "item")
            _::XML(ch, item)
        }
    } static_elif (typeinfo is_enum(value)) {
        set(node.text, "{value}")
    } static_elif (typeinfo is_bitfield(value)) {
        static_if (typeinfo sizeof(value) == 8) {
            set(node.text, uint64(value))
        } else {
            set(node.text, uint(value))
        }
    } static_elif (typeinfo is_vector(value)) {
        static_if (typeinfo vector_dim(value) == 2) {
            var ch_x = append_child(node, "x")
            set(ch_x.text, value.x)
            var ch_y = append_child(node, "y")
            set(ch_y.text, value.y)
        } static_elif (typeinfo vector_dim(value) == 3) {
            var ch_x = append_child(node, "x")
            set(ch_x.text, value.x)
            var ch_y = append_child(node, "y")
            set(ch_y.text, value.y)
            var ch_z = append_child(node, "z")
            set(ch_z.text, value.z)
        } static_elif (typeinfo vector_dim(value) == 4) {
            var ch_x = append_child(node, "x")
            set(ch_x.text, value.x)
            var ch_y = append_child(node, "y")
            set(ch_y.text, value.y)
            var ch_z = append_child(node, "z")
            set(ch_z.text, value.z)
            var ch_w = append_child(node, "w")
            set(ch_w.text, value.w)
        }
    } else {
        set(node.text, value)
    }
}

def to_XML(value : auto(TT); root_name : string = "root") : string {
    //! Serializes a struct to a complete XML string, wrapping it in an
    //! element named *root_name* (default ``"root"``).
    var doc = new xml_document()
    var dnode = doc as xml_node
    var root_node = append_child(dnode, root_name)
    _::XML(root_node, value)
    let result = to_string(doc)
    unsafe { delete doc; }
    return result
}

def from_XML(xml_str : string; anything : auto(TT)) {
    //! Parses an XML string and deserializes the document element into
    //! a native value in one step. Symmetric with ``to_XML``.
    var doc = new xml_document()
    var result : xml_parse_result
    load_string(doc, xml_str, result, parse_default)
    static_if (typeinfo can_copy(type<TT>)) {
        let ret = _::from_XML(doc.document_element, type<TT>)
        unsafe { delete doc; }
        return ret
    } else {
        var ret <- _::from_XML(doc.document_element, type<TT>)
        unsafe { delete doc; }
        return <- ret
    }
}
