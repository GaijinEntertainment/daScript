
.. _stdlib_pugixml:

====================
XML parser (pugixml)
====================

.. das:module:: pugixml

The PUGIXML module provides XML parsing, navigation, manipulation, and XPath query
support built on top of the `pugixml <https://pugixml.org/>`_ C++ library. It exposes
document loading/saving, DOM-style node and attribute access, text content helpers,
and a full XPath 1.0 evaluation engine.

Use ``PUGIXML_boost`` for high-level helpers such as RAII document handling,
iterator-based traversal, builder EDSL, and struct ↔ XML serialization.
The low-level C++ bindings live in this module.

All functions and symbols are in "pugixml" module, use require to get access to it.

.. code-block:: das

    require pugixml

See also:

  * :ref:`PUGIXML_boost <stdlib_PUGIXML_boost>` — high-level helpers and serialization
  * `Tutorial 01 — Parsing and navigation <https://github.com/GaijinEntertainment/daScript/blob/master/tutorials/dasPUGIXML/01_parsing_and_navigation.das>`_
  * `Tutorial 02 — Building XML <https://github.com/GaijinEntertainment/daScript/blob/master/tutorials/dasPUGIXML/02_building_xml.das>`_
  * `Tutorial 03 — XPath <https://github.com/GaijinEntertainment/daScript/blob/master/tutorials/dasPUGIXML/03_xpath.das>`_
  * `Tutorial 04 — Serialization <https://github.com/GaijinEntertainment/daScript/blob/master/tutorials/dasPUGIXML/04_serialization.das>`_



+++++++++
Constants
+++++++++

.. _global-pugixml-parse_minimal:

.. das:attribute:: parse_minimal = 0x0

Minimal parsing mode: only elements and PCDATA are parsed.


.. _global-pugixml-parse_pi:

.. das:attribute:: parse_pi = 0x1

Parse processing instructions (`<?...?>`).


.. _global-pugixml-parse_comments:

.. das:attribute:: parse_comments = 0x2

Parse comments (`<!--...-->`).


.. _global-pugixml-parse_cdata:

.. das:attribute:: parse_cdata = 0x4

Parse CDATA sections (`<![CDATA[...]]>`).


.. _global-pugixml-parse_ws_pcdata:

.. das:attribute:: parse_ws_pcdata = 0x8

Parse whitespace-only PCDATA nodes.


.. _global-pugixml-parse_escapes:

.. das:attribute:: parse_escapes = 0x10

Parse character and entity references (`&amp;`, `&#123;`, etc.).


.. _global-pugixml-parse_eol:

.. das:attribute:: parse_eol = 0x20

Normalize line endings to `\n`.


.. _global-pugixml-parse_wconv_attribute:

.. das:attribute:: parse_wconv_attribute = 0x40

Normalize whitespace in attribute values (convert tabs/newlines to spaces).


.. _global-pugixml-parse_wnorm_attribute:

.. das:attribute:: parse_wnorm_attribute = 0x80

Normalize and collapse whitespace in attribute values.


.. _global-pugixml-parse_declaration:

.. das:attribute:: parse_declaration = 0x100

Parse XML declarations (`<?xml ...?>`).


.. _global-pugixml-parse_doctype:

.. das:attribute:: parse_doctype = 0x200

Parse DOCTYPE declarations.


.. _global-pugixml-parse_ws_pcdata_single:

.. das:attribute:: parse_ws_pcdata_single = 0x400

Parse whitespace-only PCDATA as a single node.


.. _global-pugixml-parse_trim_pcdata:

.. das:attribute:: parse_trim_pcdata = 0x800

Trim leading and trailing whitespace from PCDATA.


.. _global-pugixml-parse_fragment:

.. das:attribute:: parse_fragment = 0x1000

Parse as document fragment (allows multiple root elements).


.. _global-pugixml-parse_embed_pcdata:

.. das:attribute:: parse_embed_pcdata = 0x2000

Embed PCDATA value in the element node instead of creating a child.


.. _global-pugixml-parse_merge_pcdata:

.. das:attribute:: parse_merge_pcdata = 0x4000

Merge adjacent PCDATA nodes into one.


.. _global-pugixml-parse_default:

.. das:attribute:: parse_default = 0x74

Default parsing flags: parse_cdata | parse_escapes | parse_wconv_attribute | parse_eol.


.. _global-pugixml-parse_full:

.. das:attribute:: parse_full = 0x377

Full parsing: all possible constructs are parsed.


.. _global-pugixml-format_indent:

.. das:attribute:: format_indent = 0x1

Indent the output nodes according to tree depth.


.. _global-pugixml-format_write_bom:

.. das:attribute:: format_write_bom = 0x2

Write an encoding byte-order mark (BOM) at the start.


.. _global-pugixml-format_raw:

.. das:attribute:: format_raw = 0x4

Raw output: no indentation or newlines.


.. _global-pugixml-format_no_declaration:

.. das:attribute:: format_no_declaration = 0x8

Omit the XML declaration (`<?xml ...?>`) from output.


.. _global-pugixml-format_no_escapes:

.. das:attribute:: format_no_escapes = 0x10

Do not escape special characters in output.


.. _global-pugixml-format_save_file_text:

.. das:attribute:: format_save_file_text = 0x20

Use platform-native line endings when saving to file.


.. _global-pugixml-format_indent_attributes:

.. das:attribute:: format_indent_attributes = 0x40

Indent attributes on separate lines.


.. _global-pugixml-format_no_empty_element_tags:

.. das:attribute:: format_no_empty_element_tags = 0x80

Always use `<tag></tag>` instead of `<tag/>` for empty elements.


.. _global-pugixml-format_skip_control_chars:

.. das:attribute:: format_skip_control_chars = 0x100

Skip control characters during serialization.


.. _global-pugixml-format_attribute_single_quote:

.. das:attribute:: format_attribute_single_quote = 0x200

Use single quotes for attribute values.


.. _global-pugixml-format_default:

.. das:attribute:: format_default = 0x1

Default formatting flags: format_indent.



++++++++++++
Enumerations
++++++++++++

.. _enum-pugixml-xml_encoding:

.. das:attribute:: xml_encoding

Character encoding used for XML input/output operations.

:Values: * **encoding_auto** = 0 - Auto-detect encoding from BOM or content.

         * **encoding_utf8** = 1 - UTF-8 encoding.

         * **encoding_utf16_le** = 2 - UTF-16 little-endian encoding.

         * **encoding_utf16_be** = 3 - UTF-16 big-endian encoding.

         * **encoding_utf16** = 4 - UTF-16 with native endianness.

         * **encoding_utf32_le** = 5 - UTF-32 little-endian encoding.

         * **encoding_utf32_be** = 6 - UTF-32 big-endian encoding.

         * **encoding_utf32** = 7 - UTF-32 with native endianness.

         * **encoding_wchar** = 8 - System wchar_t encoding.

         * **encoding_latin1** = 9 - Latin-1 (ISO 8859-1) encoding.



.. _enum-pugixml-xml_node_type:

.. das:attribute:: xml_node_type

DOM node type identifying the kind of XML node.

:Values: * **node_null** = 0 - Empty (null) node handle.

         * **node_document** = 1 - Document tree root (not an XML element).

         * **node_element** = 2 - Element tag (e.g. `<node>`).

         * **node_pcdata** = 3 - Plain character data (text content).

         * **node_cdata** = 4 - Character data section (`<![CDATA[...]]>`).

         * **node_comment** = 5 - Comment node (`<!-- ... -->`).

         * **node_pi** = 6 - Processing instruction (`<?name ...?>`).

         * **node_declaration** = 7 - XML declaration (`<?xml ...?>`).

         * **node_doctype** = 8 - Document type declaration (`<!DOCTYPE ...>`).



.. _enum-pugixml-xml_parse_status:

.. das:attribute:: xml_parse_status

Parsing result status codes indicating success or the kind of error encountered.

:Values: * **status_ok** = 0 - No error, parsing succeeded.

         * **status_file_not_found** = 1 - File was not found during load.

         * **status_io_error** = 2 - I/O error during read.

         * **status_out_of_memory** = 3 - Out of memory.

         * **status_internal_error** = 4 - Internal parser error.

         * **status_unrecognized_tag** = 5 - Unrecognized tag encountered.

         * **status_bad_pi** = 6 - Malformed processing instruction.

         * **status_bad_comment** = 7 - Malformed comment.

         * **status_bad_cdata** = 8 - Malformed CDATA section.

         * **status_bad_doctype** = 9 - Malformed document type declaration.

         * **status_bad_pcdata** = 10 - Malformed PCDATA section.

         * **status_bad_start_element** = 11 - Malformed start element tag.

         * **status_bad_attribute** = 12 - Malformed attribute.

         * **status_bad_end_element** = 13 - Malformed end element tag.

         * **status_end_element_mismatch** = 14 - Start/end element tag mismatch.

         * **status_append_invalid_root** = 15 - Cannot append nodes to the specified root.

         * **status_no_document_element** = 16 - No document element found.



.. _enum-pugixml-xpath_value_type:

.. das:attribute:: xpath_value_type

XPath expression return type.

:Values: * **xpath_type_none** = 0 - Unknown or no type.

         * **xpath_type_node_set** = 1 - Node set (collection of XML nodes).

         * **xpath_type_number** = 2 - Floating-point number.

         * **xpath_type_string** = 3 - Character string.

         * **xpath_type_boolean** = 4 - Boolean value.




++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-pugixml-xml_attribute:

.. das:attribute:: xml_attribute

.. _function-pugixml__dot__rq_name_xml_attribute:

.. das:function:: xml_attribute.name() : string

Returns the attribute name.


.. _function-pugixml__dot__rq_value_xml_attribute:

.. das:function:: xml_attribute.value() : string

Returns the attribute value as a string.


.. _function-pugixml__dot__rq_empty_xml_attribute:

.. das:function:: xml_attribute.empty() : bool

Returns true if the attribute handle is empty (null).


:Properties: * **name** : string

             * **value** : string

             * **empty** : bool

Handle to a single XML attribute (name-value pair) on an element node.
The attribute name.
The attribute value as a string.
True if this handle is empty (not bound to any attribute).



.. _handle-pugixml-xml_text:

.. das:attribute:: xml_text

.. _function-pugixml__dot__rq_empty_xml_text:

.. das:function:: xml_text.empty() : bool

Returns true if the attribute handle is empty (null).


.. _function-pugixml__dot__rq_get_xml_text:

.. das:function:: xml_text.get() : string

Returns the text content as a raw string.


:Properties: * **empty** : bool

             * **get** : string

Accessor for the text content of an element node. Provides typed read and write operations.
True if this text object is empty (the element has no text content).
The text content as a string.



.. _handle-pugixml-xml_node:

.. das:attribute:: xml_node

.. _function-pugixml__dot__rq_name_xml_node:

.. das:function:: xml_node.name() : string

Returns the attribute name.


.. _function-pugixml__dot__rq_value_xml_node:

.. das:function:: xml_node.value() : string

Returns the attribute value as a string.


.. _function-pugixml__dot__rq__type_xml_node:

.. das:function:: xml_node._type() : xml_node_type

Returns the node type (element, pcdata, cdata, comment, etc.).


.. _function-pugixml__dot__rq_empty_xml_node:

.. das:function:: xml_node.empty() : bool

Returns true if the attribute handle is empty (null).


:Properties: * **name** : string

             * **value** : string

             * **_type** :  :ref:`xml_node_type <enum-pugixml-xml_node_type>`

             * **empty** : bool

Lightweight handle to a DOM node (element, text, comment, etc.). Does not own memory; the owning xml_document must outlive it.
The tag name of this element node.
The raw text value of this node.
The node type (see xml_node_type enumeration).
True if this handle is empty (not bound to any node).



.. _handle-pugixml-xpath_node:

.. das:attribute:: xpath_node

A single result from an XPath query. Wraps either an xml_node or an xml_attribute together with its parent.



.. _handle-pugixml-xpath_node_set:

.. das:attribute:: xpath_node_set

An ordered collection of xpath_node results returned by XPath evaluation. Supports indexed access and sorting.



.. _handle-pugixml-xpath_query:

.. das:attribute:: xpath_query

A compiled XPath 1.0 expression. Compile once and evaluate many times for efficiency.



.. _handle-pugixml-xpath_variable_set:

.. das:attribute:: xpath_variable_set

A set of named XPath variables (bool, number, string) that can be bound to a compiled xpath_query.



.. _handle-pugixml-xml_document:

.. das:attribute:: xml_document

Owns the entire DOM tree. Must stay alive while any xml_node or xml_attribute obtained from it is in use.



.. _handle-pugixml-xml_parse_result:

.. das:attribute:: xml_parse_result

.. _function-pugixml__dot__rq_description_xml_parse_result:

.. das:function:: xml_parse_result.description() : string

Returns a human-readable description of the parse result status.


:Properties: * **description** : string

Result of a parsing operation. Contains the status code, encoding, and character offset of the first error.
The parsing status code (see xml_parse_status enumeration).

:Fields: * **status** :  :ref:`xml_parse_status <enum-pugixml-xml_parse_status>` - Character offset in the source where the error occurred (0 on success).

         * **offset** : int64 - The detected or specified document encoding.

         * **encoding** :  :ref:`xml_encoding <enum-pugixml-xml_encoding>` - Human-readable description of the parsing result.




+++++++++++++++++++
Document operations
+++++++++++++++++++

  *  :ref:`document_as_node (document: xml_document?) : xml_node <function-pugixml_document_as_node_xml_document_q_>`
  *  :ref:`load_document (doc: xml_document?; filename: string; result: xml_parse_result) : bool <function-pugixml_load_document_xml_document_q__string_xml_parse_result>`
  *  :ref:`load_string (doc: xml_document?; content: string; result: xml_parse_result; options: uint) : bool <function-pugixml_load_string_xml_document_q__string_xml_parse_result_uint>`
  *  :ref:`reset (document: xml_document?) <function-pugixml_reset_xml_document_q_>`
  *  :ref:`save_file (document: xml_document const?; filename: string; indent: string; flags: uint; encoding: xml_encoding) : bool <function-pugixml_save_file_xml_document_const_q__string_string_uint_xml_encoding>`
  *  :ref:`save_string (document: xml_document const?; indent: string; flags: uint; encoding: xml_encoding) : string <function-pugixml_save_string_xml_document_const_q__string_uint_xml_encoding>`
  *  :ref:`xml_document const?.document_element (document: xml_document const?) : xml_node <function-pugixml__dot__rq_document_element_xml_document_const_q_>`

.. _function-pugixml_document_as_node_xml_document_q_:

.. das:function:: document_as_node(document: xml_document?) : xml_node

Returns the document as an xml_node, allowing direct node operations on the document root.


:Arguments: * **document** :  :ref:`xml_document <handle-pugixml-xml_document>`? implicit

.. _function-pugixml_load_document_xml_document_q__string_xml_parse_result:

.. das:function:: load_document(doc: xml_document?; filename: string; result: xml_parse_result) : bool

Loads an XML document from a file. Populates *result* with parse status and error offset.


:Arguments: * **doc** :  :ref:`xml_document <handle-pugixml-xml_document>`? implicit

            * **filename** : string implicit

            * **result** :  :ref:`xml_parse_result <handle-pugixml-xml_parse_result>` implicit

.. _function-pugixml_load_string_xml_document_q__string_xml_parse_result_uint:

.. das:function:: load_string(doc: xml_document?; content: string; result: xml_parse_result; options: uint) : bool

Parses an XML string into the document. Uses the given parse options flags.


:Arguments: * **doc** :  :ref:`xml_document <handle-pugixml-xml_document>`? implicit

            * **content** : string implicit

            * **result** :  :ref:`xml_parse_result <handle-pugixml-xml_parse_result>` implicit

            * **options** : uint

.. _function-pugixml_reset_xml_document_q_:

.. das:function:: reset(document: xml_document?)

Resets the document, removing all nodes and freeing memory.


:Arguments: * **document** :  :ref:`xml_document <handle-pugixml-xml_document>`? implicit

.. _function-pugixml_save_file_xml_document_const_q__string_string_uint_xml_encoding:

.. das:function:: save_file(document: xml_document const?; filename: string; indent: string; flags: uint; encoding: xml_encoding) : bool

Saves the document to a file with the specified indentation, flags, and encoding.


:Arguments: * **document** :  :ref:`xml_document <handle-pugixml-xml_document>`? implicit

            * **filename** : string implicit

            * **indent** : string implicit

            * **flags** : uint

            * **encoding** :  :ref:`xml_encoding <enum-pugixml-xml_encoding>`

.. _function-pugixml_save_string_xml_document_const_q__string_uint_xml_encoding:

.. das:function:: save_string(document: xml_document const?; indent: string; flags: uint; encoding: xml_encoding) : string

Serializes the entire document to a string with the specified formatting.


:Arguments: * **document** :  :ref:`xml_document <handle-pugixml-xml_document>`? implicit

            * **indent** : string implicit

            * **flags** : uint

            * **encoding** :  :ref:`xml_encoding <enum-pugixml-xml_encoding>`

.. _function-pugixml__dot__rq_document_element_xml_document_const_q_:

.. das:function:: xml_document const?.document_element(document: xml_document const?) : xml_node

Returns the root element of the document (i.e. the outermost element).


:Arguments: * **document** :  :ref:`xml_document <handle-pugixml-xml_document>`? implicit


+++++++++++
Node lookup
+++++++++++

  *  :ref:`append_child (node: xml_node; type: xml_node_type) : xml_node <function-pugixml_append_child_xml_node_xml_node_type>`
  *  :ref:`append_child (node: xml_node; name: string) : xml_node <function-pugixml_append_child_xml_node_string>`
  *  :ref:`child (node: xml_node; name: string) : xml_node <function-pugixml_child_xml_node_string>`
  *  :ref:`find_child_by_attribute (node: xml_node; element_name: string; attr_name: string; attr_value: string) : xml_node <function-pugixml_find_child_by_attribute_xml_node_string_string_string>`
  *  :ref:`find_child_by_attribute (node: xml_node; attr_name: string; attr_value: string) : xml_node <function-pugixml_find_child_by_attribute_xml_node_string_string>`
  *  :ref:`first_element_by_path (node: xml_node; path: string) : xml_node <function-pugixml_first_element_by_path_xml_node_string>`
  *  :ref:`prepend_child (node: xml_node; name: string) : xml_node <function-pugixml_prepend_child_xml_node_string>`
  *  :ref:`prepend_child (node: xml_node; type: xml_node_type) : xml_node <function-pugixml_prepend_child_xml_node_xml_node_type>`
  *  :ref:`remove_child (node: xml_node; child: xml_node) : bool <function-pugixml_remove_child_xml_node_xml_node>`
  *  :ref:`remove_child (node: xml_node; name: string) : bool <function-pugixml_remove_child_xml_node_string>`
  *  :ref:`xml_node.first_child (node: xml_node) : xml_node <function-pugixml__dot__rq_first_child_xml_node>`
  *  :ref:`xml_node.last_child (node: xml_node) : xml_node <function-pugixml__dot__rq_last_child_xml_node>`


append_child
^^^^^^^^^^^^

.. _function-pugixml_append_child_xml_node_xml_node_type:

.. das:function:: append_child(node: xml_node; type: xml_node_type) : xml_node

// stub
def append_child (node: xml_node; type: xml_node_type) : xml_node


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

            * **type** :  :ref:`xml_node_type <enum-pugixml-xml_node_type>`

.. _function-pugixml_append_child_xml_node_string:

.. das:function:: append_child(node: xml_node; name: string) : xml_node

----

.. _function-pugixml_child_xml_node_string:

.. das:function:: child(node: xml_node; name: string) : xml_node

Returns the first child element with the given name, or an empty node handle if not found.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

            * **name** : string implicit


find_child_by_attribute
^^^^^^^^^^^^^^^^^^^^^^^

.. _function-pugixml_find_child_by_attribute_xml_node_string_string_string:

.. das:function:: find_child_by_attribute(node: xml_node; element_name: string; attr_name: string; attr_value: string) : xml_node

Finds the first child element that has an attribute matching the given name and value.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

            * **element_name** : string implicit

            * **attr_name** : string implicit

            * **attr_value** : string implicit

.. _function-pugixml_find_child_by_attribute_xml_node_string_string:

.. das:function:: find_child_by_attribute(node: xml_node; attr_name: string; attr_value: string) : xml_node

----

.. _function-pugixml_first_element_by_path_xml_node_string:

.. das:function:: first_element_by_path(node: xml_node; path: string) : xml_node

Navigates a slash-separated element path (e.g. `a/b/c`) and returns the target node.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

            * **path** : string implicit


prepend_child
^^^^^^^^^^^^^

.. _function-pugixml_prepend_child_xml_node_string:

.. das:function:: prepend_child(node: xml_node; name: string) : xml_node

Prepends a new child element with the given name to the node.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

            * **name** : string implicit

.. _function-pugixml_prepend_child_xml_node_xml_node_type:

.. das:function:: prepend_child(node: xml_node; type: xml_node_type) : xml_node

----


remove_child
^^^^^^^^^^^^

.. _function-pugixml_remove_child_xml_node_xml_node:

.. das:function:: remove_child(node: xml_node; child: xml_node) : bool

Removes the given child node from this node.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

            * **child** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

.. _function-pugixml_remove_child_xml_node_string:

.. das:function:: remove_child(node: xml_node; name: string) : bool

----

.. _function-pugixml__dot__rq_first_child_xml_node:

.. das:function:: xml_node.first_child(node: xml_node) : xml_node

Returns the first child node of this element.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

.. _function-pugixml__dot__rq_last_child_xml_node:

.. das:function:: xml_node.last_child(node: xml_node) : xml_node

Returns the last child node of this element.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit


+++++++++++++++
Node navigation
+++++++++++++++

  *  :ref:`next_sibling (node: xml_node; name: string) : xml_node <function-pugixml_next_sibling_xml_node_string>`
  *  :ref:`path (node: xml_node; delimiter: string) : string <function-pugixml_path_xml_node_string>`
  *  :ref:`previous_sibling (node: xml_node; name: string) : xml_node <function-pugixml_previous_sibling_xml_node_string>`
  *  :ref:`print_to_string (node: xml_node; indent: string; flags: uint; encoding: xml_encoding) : string <function-pugixml_print_to_string_xml_node_string_uint_xml_encoding>`
  *  :ref:`xml_node.next_sibling (node: xml_node) : xml_node <function-pugixml__dot__rq_next_sibling_xml_node>`
  *  :ref:`xml_node.parent (node: xml_node) : xml_node <function-pugixml__dot__rq_parent_xml_node>`
  *  :ref:`xml_node.previous_sibling (node: xml_node) : xml_node <function-pugixml__dot__rq_previous_sibling_xml_node>`
  *  :ref:`xml_node.root (node: xml_node) : xml_node <function-pugixml__dot__rq_root_xml_node>`
  *  :ref:`xpath_node.parent (xpath_node: xpath_node) : xml_node <function-pugixml__dot__rq_parent_xpath_node>`

.. _function-pugixml_next_sibling_xml_node_string:

.. das:function:: next_sibling(node: xml_node; name: string) : xml_node

Returns the next sibling element with the given name, or an empty node if not found.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

            * **name** : string implicit

.. _function-pugixml_path_xml_node_string:

.. das:function:: path(node: xml_node; delimiter: string) : string

Returns the absolute path of the node from the document root, using the given delimiter.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

            * **delimiter** : string implicit

.. _function-pugixml_previous_sibling_xml_node_string:

.. das:function:: previous_sibling(node: xml_node; name: string) : xml_node

Returns the previous sibling element with the given name, or an empty node if not found.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

            * **name** : string implicit

.. _function-pugixml_print_to_string_xml_node_string_uint_xml_encoding:

.. das:function:: print_to_string(node: xml_node; indent: string; flags: uint; encoding: xml_encoding) : string

Serializes the node (and its subtree) to an XML string with the specified formatting.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

            * **indent** : string implicit

            * **flags** : uint

            * **encoding** :  :ref:`xml_encoding <enum-pugixml-xml_encoding>`

.. _function-pugixml__dot__rq_next_sibling_xml_node:

.. das:function:: xml_node.next_sibling(node: xml_node) : xml_node

Returns the next sibling node in document order.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

.. _function-pugixml__dot__rq_parent_xml_node:

.. das:function:: xml_node.parent(node: xml_node) : xml_node

Returns the parent node of this element.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

.. _function-pugixml__dot__rq_previous_sibling_xml_node:

.. das:function:: xml_node.previous_sibling(node: xml_node) : xml_node

Returns the previous sibling node in document order.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

.. _function-pugixml__dot__rq_root_xml_node:

.. das:function:: xml_node.root(node: xml_node) : xml_node

Returns the root node of the document this node belongs to.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

.. _function-pugixml__dot__rq_parent_xpath_node:

.. das:function:: xpath_node.parent(xpath_node: xpath_node) : xml_node

Returns the parent element of this XPath result node.


:Arguments: * **xpath_node** :  :ref:`xpath_node <handle-pugixml-xpath_node>` implicit


+++++++++++++
Node mutation
+++++++++++++

  *  :ref:`insert_child_after (node: xml_node; name: string; after: xml_node) : xml_node <function-pugixml_insert_child_after_xml_node_string_xml_node>`
  *  :ref:`insert_child_before (node: xml_node; name: string; before: xml_node) : xml_node <function-pugixml_insert_child_before_xml_node_string_xml_node>`
  *  :ref:`remove_children (node: xml_node) : bool <function-pugixml_remove_children_xml_node>`
  *  :ref:`set_name (node: xml_node; name: string) : bool <function-pugixml_set_name_xml_node_string>`
  *  :ref:`set_name (attribute: xml_attribute; name: string) : bool <function-pugixml_set_name_xml_attribute_string>`
  *  :ref:`set_value (attribute: xml_attribute; value: string) : bool <function-pugixml_set_value_xml_attribute_string>`
  *  :ref:`set_value (attribute: xml_attribute; value: int) : bool <function-pugixml_set_value_xml_attribute_int>`
  *  :ref:`set_value (node: xml_node; value: string) : bool <function-pugixml_set_value_xml_node_string>`
  *  :ref:`set_value (attribute: xml_attribute; value: float) : bool <function-pugixml_set_value_xml_attribute_float>`
  *  :ref:`set_value (attribute: xml_attribute; value: double) : bool <function-pugixml_set_value_xml_attribute_double>`
  *  :ref:`set_value (attribute: xml_attribute; value: uint) : bool <function-pugixml_set_value_xml_attribute_uint>`
  *  :ref:`set_value (attribute: xml_attribute; value: bool) : bool <function-pugixml_set_value_xml_attribute_bool>`

.. _function-pugixml_insert_child_after_xml_node_string_xml_node:

.. das:function:: insert_child_after(node: xml_node; name: string; after: xml_node) : xml_node

Inserts a new child element with the given name after the specified sibling.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

            * **name** : string implicit

            * **after** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

.. _function-pugixml_insert_child_before_xml_node_string_xml_node:

.. das:function:: insert_child_before(node: xml_node; name: string; before: xml_node) : xml_node

Inserts a new child element with the given name before the specified sibling.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

            * **name** : string implicit

            * **before** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

.. _function-pugixml_remove_children_xml_node:

.. das:function:: remove_children(node: xml_node) : bool

Removes all child nodes from this node.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit


set_name
^^^^^^^^

.. _function-pugixml_set_name_xml_node_string:

.. das:function:: set_name(node: xml_node; name: string) : bool

Changes the name (tag) of the node or attribute.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

            * **name** : string implicit

.. _function-pugixml_set_name_xml_attribute_string:

.. das:function:: set_name(attribute: xml_attribute; name: string) : bool

----


set_value
^^^^^^^^^

.. _function-pugixml_set_value_xml_attribute_string:

.. das:function:: set_value(attribute: xml_attribute; value: string) : bool

Sets the value of the node or attribute. Accepts string, int, uint, float, double, or bool.


:Arguments: * **attribute** :  :ref:`xml_attribute <handle-pugixml-xml_attribute>` implicit

            * **value** : string implicit

.. _function-pugixml_set_value_xml_attribute_int:

.. das:function:: set_value(attribute: xml_attribute; value: int) : bool

.. _function-pugixml_set_value_xml_node_string:

.. das:function:: set_value(node: xml_node; value: string) : bool

.. _function-pugixml_set_value_xml_attribute_float:

.. das:function:: set_value(attribute: xml_attribute; value: float) : bool

.. _function-pugixml_set_value_xml_attribute_double:

.. das:function:: set_value(attribute: xml_attribute; value: double) : bool

.. _function-pugixml_set_value_xml_attribute_uint:

.. das:function:: set_value(attribute: xml_attribute; value: uint) : bool

.. _function-pugixml_set_value_xml_attribute_bool:

.. das:function:: set_value(attribute: xml_attribute; value: bool) : bool


++++++++++++++++
Attribute access
++++++++++++++++

  *  :ref:`append_attribute (node: xml_node; name: string) : xml_attribute <function-pugixml_append_attribute_xml_node_string>`
  *  :ref:`attribute (node: xml_node; name: string) : xml_attribute <function-pugixml_attribute_xml_node_string>`
  *  :ref:`insert_attribute_after (node: xml_node; name: string; after: xml_attribute) : xml_attribute <function-pugixml_insert_attribute_after_xml_node_string_xml_attribute>`
  *  :ref:`insert_attribute_before (node: xml_node; name: string; before: xml_attribute) : xml_attribute <function-pugixml_insert_attribute_before_xml_node_string_xml_attribute>`
  *  :ref:`prepend_attribute (node: xml_node; name: string) : xml_attribute <function-pugixml_prepend_attribute_xml_node_string>`
  *  :ref:`remove_attribute (node: xml_node; attribute: xml_attribute) : bool <function-pugixml_remove_attribute_xml_node_xml_attribute>`
  *  :ref:`remove_attribute (node: xml_node; name: string) : bool <function-pugixml_remove_attribute_xml_node_string>`
  *  :ref:`remove_attributes (node: xml_node) : bool <function-pugixml_remove_attributes_xml_node>`
  *  :ref:`xml_attribute.next_attribute (attribute: xml_attribute) : xml_attribute <function-pugixml__dot__rq_next_attribute_xml_attribute>`
  *  :ref:`xml_attribute.previous_attribute (attribute: xml_attribute) : xml_attribute <function-pugixml__dot__rq_previous_attribute_xml_attribute>`
  *  :ref:`xml_node.first_attribute (node: xml_node) : xml_attribute <function-pugixml__dot__rq_first_attribute_xml_node>`
  *  :ref:`xml_node.last_attribute (node: xml_node) : xml_attribute <function-pugixml__dot__rq_last_attribute_xml_node>`
  *  :ref:`xpath_node.attribute (xpath_node: xpath_node) : xml_attribute <function-pugixml__dot__rq_attribute_xpath_node>`

.. _function-pugixml_append_attribute_xml_node_string:

.. das:function:: append_attribute(node: xml_node; name: string) : xml_attribute

Adds a new attribute with the given name at the end of the node's attribute list.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

            * **name** : string implicit

.. _function-pugixml_attribute_xml_node_string:

.. das:function:: attribute(node: xml_node; name: string) : xml_attribute

Returns the attribute with the given name, or an empty attribute handle if not found.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

            * **name** : string implicit

.. _function-pugixml_insert_attribute_after_xml_node_string_xml_attribute:

.. das:function:: insert_attribute_after(node: xml_node; name: string; after: xml_attribute) : xml_attribute

Inserts a new attribute with the given name after the specified attribute.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

            * **name** : string implicit

            * **after** :  :ref:`xml_attribute <handle-pugixml-xml_attribute>` implicit

.. _function-pugixml_insert_attribute_before_xml_node_string_xml_attribute:

.. das:function:: insert_attribute_before(node: xml_node; name: string; before: xml_attribute) : xml_attribute

Inserts a new attribute with the given name before the specified attribute.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

            * **name** : string implicit

            * **before** :  :ref:`xml_attribute <handle-pugixml-xml_attribute>` implicit

.. _function-pugixml_prepend_attribute_xml_node_string:

.. das:function:: prepend_attribute(node: xml_node; name: string) : xml_attribute

Adds a new attribute with the given name at the beginning of the node's attribute list.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

            * **name** : string implicit


remove_attribute
^^^^^^^^^^^^^^^^

.. _function-pugixml_remove_attribute_xml_node_xml_attribute:

.. das:function:: remove_attribute(node: xml_node; attribute: xml_attribute) : bool

// stub
def remove_attribute (node: xml_node; attribute: xml_attribute) : bool


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

            * **attribute** :  :ref:`xml_attribute <handle-pugixml-xml_attribute>` implicit

.. _function-pugixml_remove_attribute_xml_node_string:

.. das:function:: remove_attribute(node: xml_node; name: string) : bool

----

.. _function-pugixml_remove_attributes_xml_node:

.. das:function:: remove_attributes(node: xml_node) : bool

Removes all attributes from the node.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

.. _function-pugixml__dot__rq_next_attribute_xml_attribute:

.. das:function:: xml_attribute.next_attribute(attribute: xml_attribute) : xml_attribute

Returns the next attribute in the element's attribute list.


:Arguments: * **attribute** :  :ref:`xml_attribute <handle-pugixml-xml_attribute>` implicit

.. _function-pugixml__dot__rq_previous_attribute_xml_attribute:

.. das:function:: xml_attribute.previous_attribute(attribute: xml_attribute) : xml_attribute

Returns the previous attribute in the element's attribute list.


:Arguments: * **attribute** :  :ref:`xml_attribute <handle-pugixml-xml_attribute>` implicit

.. _function-pugixml__dot__rq_first_attribute_xml_node:

.. das:function:: xml_node.first_attribute(node: xml_node) : xml_attribute

Returns the first attribute of this element node.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

.. _function-pugixml__dot__rq_last_attribute_xml_node:

.. das:function:: xml_node.last_attribute(node: xml_node) : xml_attribute

Returns the last attribute of this element node.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

.. _function-pugixml__dot__rq_attribute_xpath_node:

.. das:function:: xpath_node.attribute(xpath_node: xpath_node) : xml_attribute

Returns the attribute associated with this XPath result node, if any.


:Arguments: * **xpath_node** :  :ref:`xpath_node <handle-pugixml-xpath_node>` implicit


+++++++++++++
Copy and move
+++++++++++++

  *  :ref:`append_copy (node: xml_node; proto: xml_attribute) : xml_attribute <function-pugixml_append_copy_xml_node_xml_attribute>`
  *  :ref:`append_copy (node: xml_node; proto: xml_node) : xml_node <function-pugixml_append_copy_xml_node_xml_node>`
  *  :ref:`append_move (node: xml_node; moved: xml_node) : xml_node <function-pugixml_append_move_xml_node_xml_node>`
  *  :ref:`prepend_copy (node: xml_node; proto: xml_attribute) : xml_attribute <function-pugixml_prepend_copy_xml_node_xml_attribute>`
  *  :ref:`prepend_copy (node: xml_node; proto: xml_node) : xml_node <function-pugixml_prepend_copy_xml_node_xml_node>`
  *  :ref:`prepend_move (node: xml_node; moved: xml_node) : xml_node <function-pugixml_prepend_move_xml_node_xml_node>`


append_copy
^^^^^^^^^^^

.. _function-pugixml_append_copy_xml_node_xml_attribute:

.. das:function:: append_copy(node: xml_node; proto: xml_attribute) : xml_attribute

Appends a deep copy of the given attribute or node as the last child.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

            * **proto** :  :ref:`xml_attribute <handle-pugixml-xml_attribute>` implicit

.. _function-pugixml_append_copy_xml_node_xml_node:

.. das:function:: append_copy(node: xml_node; proto: xml_node) : xml_node

----

.. _function-pugixml_append_move_xml_node_xml_node:

.. das:function:: append_move(node: xml_node; moved: xml_node) : xml_node

Moves the given node to become the last child of this node.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

            * **moved** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit


prepend_copy
^^^^^^^^^^^^

.. _function-pugixml_prepend_copy_xml_node_xml_attribute:

.. das:function:: prepend_copy(node: xml_node; proto: xml_attribute) : xml_attribute

Prepends a deep copy of the given attribute or node as the first child.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

            * **proto** :  :ref:`xml_attribute <handle-pugixml-xml_attribute>` implicit

.. _function-pugixml_prepend_copy_xml_node_xml_node:

.. das:function:: prepend_copy(node: xml_node; proto: xml_node) : xml_node

----

.. _function-pugixml_prepend_move_xml_node_xml_node:

.. das:function:: prepend_move(node: xml_node; moved: xml_node) : xml_node

Moves the given node to become the first child of this node.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

            * **moved** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit


+++++++++++++
Value reading
+++++++++++++

  *  :ref:`as_bool (text: xml_text; default_value: bool) : bool <function-pugixml_as_bool_xml_text_bool>`
  *  :ref:`as_bool (attribute: xml_attribute; default_value: bool) : bool <function-pugixml_as_bool_xml_attribute_bool>`
  *  :ref:`as_double (text: xml_text; default_value: double) : double <function-pugixml_as_double_xml_text_double>`
  *  :ref:`as_double (attribute: xml_attribute; default_value: double) : double <function-pugixml_as_double_xml_attribute_double>`
  *  :ref:`as_float (text: xml_text; default_value: float) : float <function-pugixml_as_float_xml_text_float>`
  *  :ref:`as_float (attribute: xml_attribute; default_value: float) : float <function-pugixml_as_float_xml_attribute_float>`
  *  :ref:`as_int (attribute: xml_attribute; default_value: int) : int <function-pugixml_as_int_xml_attribute_int>`
  *  :ref:`as_int (text: xml_text; default_value: int) : int <function-pugixml_as_int_xml_text_int>`
  *  :ref:`as_int64 (text: xml_text; default_value: int64) : int64 <function-pugixml_as_int64_xml_text_int64>`
  *  :ref:`as_string (text: xml_text; default_value: string) : string <function-pugixml_as_string_xml_text_string>`
  *  :ref:`as_string (attribute: xml_attribute; default_value: string) : string <function-pugixml_as_string_xml_attribute_string>`
  *  :ref:`as_uint (attribute: xml_attribute; default_value: uint) : uint <function-pugixml_as_uint_xml_attribute_uint>`
  *  :ref:`as_uint (text: xml_text; default_value: uint) : uint <function-pugixml_as_uint_xml_text_uint>`
  *  :ref:`as_uint64 (text: xml_text; default_value: uint64) : uint64 <function-pugixml_as_uint64_xml_text_uint64>`
  *  :ref:`child_value (node: xml_node; name: string) : string <function-pugixml_child_value_xml_node_string>`
  *  :ref:`xml_node.child_value (node: xml_node) : string <function-pugixml__dot__rq_child_value_xml_node>`
  *  :ref:`xml_node.text (node: xml_node) : xml_text <function-pugixml__dot__rq_text_xml_node>`
  *  :ref:`xml_text.data (text: xml_text) : xml_node <function-pugixml__dot__rq_data_xml_text>`


as_bool
^^^^^^^

.. _function-pugixml_as_bool_xml_text_bool:

.. das:function:: as_bool(text: xml_text; default_value: bool) : bool

// stub
def as_bool (text: xml_text; default_value: bool) : bool


:Arguments: * **text** :  :ref:`xml_text <handle-pugixml-xml_text>` implicit

            * **default_value** : bool

.. _function-pugixml_as_bool_xml_attribute_bool:

.. das:function:: as_bool(attribute: xml_attribute; default_value: bool) : bool

----


as_double
^^^^^^^^^

.. _function-pugixml_as_double_xml_text_double:

.. das:function:: as_double(text: xml_text; default_value: double) : double

// stub
def as_double (text: xml_text; default_value: double) : double


:Arguments: * **text** :  :ref:`xml_text <handle-pugixml-xml_text>` implicit

            * **default_value** : double

.. _function-pugixml_as_double_xml_attribute_double:

.. das:function:: as_double(attribute: xml_attribute; default_value: double) : double

----


as_float
^^^^^^^^

.. _function-pugixml_as_float_xml_text_float:

.. das:function:: as_float(text: xml_text; default_value: float) : float

Returns the attribute or text value as a float, or *default_value* if conversion fails.


:Arguments: * **text** :  :ref:`xml_text <handle-pugixml-xml_text>` implicit

            * **default_value** : float

.. _function-pugixml_as_float_xml_attribute_float:

.. das:function:: as_float(attribute: xml_attribute; default_value: float) : float

----


as_int
^^^^^^

.. _function-pugixml_as_int_xml_attribute_int:

.. das:function:: as_int(attribute: xml_attribute; default_value: int) : int

// stub
def as_int (attribute: xml_attribute; default_value: int) : int


:Arguments: * **attribute** :  :ref:`xml_attribute <handle-pugixml-xml_attribute>` implicit

            * **default_value** : int

.. _function-pugixml_as_int_xml_text_int:

.. das:function:: as_int(text: xml_text; default_value: int) : int

----

.. _function-pugixml_as_int64_xml_text_int64:

.. das:function:: as_int64(text: xml_text; default_value: int64) : int64

Returns the text value as a 64-bit signed integer, or *default_value* if conversion fails.


:Arguments: * **text** :  :ref:`xml_text <handle-pugixml-xml_text>` implicit

            * **default_value** : int64


as_string
^^^^^^^^^

.. _function-pugixml_as_string_xml_text_string:

.. das:function:: as_string(text: xml_text; default_value: string) : string

Returns the attribute or text value as a string, or *default_value* if empty.


:Arguments: * **text** :  :ref:`xml_text <handle-pugixml-xml_text>` implicit

            * **default_value** : string implicit

.. _function-pugixml_as_string_xml_attribute_string:

.. das:function:: as_string(attribute: xml_attribute; default_value: string) : string

----


as_uint
^^^^^^^

.. _function-pugixml_as_uint_xml_attribute_uint:

.. das:function:: as_uint(attribute: xml_attribute; default_value: uint) : uint

Returns the attribute or text value as a uint, or *default_value* if conversion fails.


:Arguments: * **attribute** :  :ref:`xml_attribute <handle-pugixml-xml_attribute>` implicit

            * **default_value** : uint

.. _function-pugixml_as_uint_xml_text_uint:

.. das:function:: as_uint(text: xml_text; default_value: uint) : uint

----

.. _function-pugixml_as_uint64_xml_text_uint64:

.. das:function:: as_uint64(text: xml_text; default_value: uint64) : uint64

Returns the text value as a 64-bit unsigned integer, or *default_value* if conversion fails.


:Arguments: * **text** :  :ref:`xml_text <handle-pugixml-xml_text>` implicit

            * **default_value** : uint64

.. _function-pugixml_child_value_xml_node_string:

.. das:function:: child_value(node: xml_node; name: string) : string

Returns the text content of the first child element with the given name.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

            * **name** : string implicit

.. _function-pugixml__dot__rq_child_value_xml_node:

.. das:function:: xml_node.child_value(node: xml_node) : string

Returns the text content of the first PCDATA/CDATA child of this node.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

.. _function-pugixml__dot__rq_text_xml_node:

.. das:function:: xml_node.text(node: xml_node) : xml_text

Returns an xml_text accessor for the text content of this element.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

.. _function-pugixml__dot__rq_data_xml_text:

.. das:function:: xml_text.data(text: xml_text) : xml_node

Returns the data node that holds the actual character data for this xml_text.


:Arguments: * **text** :  :ref:`xml_text <handle-pugixml-xml_text>` implicit


+++++++++++++
Value writing
+++++++++++++

  *  :ref:`set (text: xml_text; value: int) : bool <function-pugixml_set_xml_text_int>`
  *  :ref:`set (text: xml_text; value: string) : bool <function-pugixml_set_xml_text_string>`
  *  :ref:`set (text: xml_text; value: double) : bool <function-pugixml_set_xml_text_double>`
  *  :ref:`set (text: xml_text; value: uint) : bool <function-pugixml_set_xml_text_uint>`
  *  :ref:`set (text: xml_text; value: int8) : bool <function-pugixml_set_xml_text_int8>`
  *  :ref:`set (text: xml_text; value: bool) : bool <function-pugixml_set_xml_text_bool>`
  *  :ref:`set (text: xml_text; value: uint8) : bool <function-pugixml_set_xml_text_uint8>`
  *  :ref:`set (text: xml_text; value: float) : bool <function-pugixml_set_xml_text_float>`
  *  :ref:`set (text: xml_text; value: int64) : bool <function-pugixml_set_xml_text_int64>`
  *  :ref:`set (text: xml_text; value: uint16) : bool <function-pugixml_set_xml_text_uint16>`
  *  :ref:`set (text: xml_text; value: uint64) : bool <function-pugixml_set_xml_text_uint64>`
  *  :ref:`set (variables: xpath_variable_set?; name: string; value: bool) : bool <function-pugixml_set_xpath_variable_set_q__string_bool>`
  *  :ref:`set (variables: xpath_variable_set?; name: string; value: double) : bool <function-pugixml_set_xpath_variable_set_q__string_double>`
  *  :ref:`set (text: xml_text; value: int16) : bool <function-pugixml_set_xml_text_int16>`
  *  :ref:`set (variables: xpath_variable_set?; name: string; value: string) : bool <function-pugixml_set_xpath_variable_set_q__string_string>`


set
^^^

.. _function-pugixml_set_xml_text_int:

.. das:function:: set(text: xml_text; value: int) : bool

Sets the text content or XPath variable value. Multiple overloads accept string, int, uint, float, double, bool, int64, uint64.


:Arguments: * **text** :  :ref:`xml_text <handle-pugixml-xml_text>` implicit

            * **value** : int

.. _function-pugixml_set_xml_text_string:

.. das:function:: set(text: xml_text; value: string) : bool

.. _function-pugixml_set_xml_text_double:

.. das:function:: set(text: xml_text; value: double) : bool

.. _function-pugixml_set_xml_text_uint:

.. das:function:: set(text: xml_text; value: uint) : bool

.. _function-pugixml_set_xml_text_int8:

.. das:function:: set(text: xml_text; value: int8) : bool

.. _function-pugixml_set_xml_text_bool:

.. das:function:: set(text: xml_text; value: bool) : bool

.. _function-pugixml_set_xml_text_uint8:

.. das:function:: set(text: xml_text; value: uint8) : bool

.. _function-pugixml_set_xml_text_float:

.. das:function:: set(text: xml_text; value: float) : bool

.. _function-pugixml_set_xml_text_int64:

.. das:function:: set(text: xml_text; value: int64) : bool

.. _function-pugixml_set_xml_text_uint16:

.. das:function:: set(text: xml_text; value: uint16) : bool

.. _function-pugixml_set_xml_text_uint64:

.. das:function:: set(text: xml_text; value: uint64) : bool

.. _function-pugixml_set_xpath_variable_set_q__string_bool:

.. das:function:: set(variables: xpath_variable_set?; name: string; value: bool) : bool

.. _function-pugixml_set_xpath_variable_set_q__string_double:

.. das:function:: set(variables: xpath_variable_set?; name: string; value: double) : bool

.. _function-pugixml_set_xml_text_int16:

.. das:function:: set(text: xml_text; value: int16) : bool

.. _function-pugixml_set_xpath_variable_set_q__string_string:

.. das:function:: set(variables: xpath_variable_set?; name: string; value: string) : bool


++++++++++++++++++++++++++++++++
XPath compilation and evaluation
++++++++++++++++++++++++++++++++

  *  :ref:`evaluate_boolean (query: xpath_query const?; node: xml_node) : bool <function-pugixml_evaluate_boolean_xpath_query_const_q__xml_node>`
  *  :ref:`evaluate_node (query: xpath_query const?; node: xml_node) : xpath_node <function-pugixml_evaluate_node_xpath_query_const_q__xml_node>`
  *  :ref:`evaluate_node_set (query: xpath_query const?; node: xml_node) : xpath_node_set? <function-pugixml_evaluate_node_set_xpath_query_const_q__xml_node>`
  *  :ref:`evaluate_number (query: xpath_query const?; node: xml_node) : double <function-pugixml_evaluate_number_xpath_query_const_q__xml_node>`
  *  :ref:`evaluate_string (query: xpath_query const?; node: xml_node) : string <function-pugixml_evaluate_string_xpath_query_const_q__xml_node>`
  *  :ref:`xpath_compile (query: string; variables: xpath_variable_set?) : xpath_query? <function-pugixml_xpath_compile_string_xpath_variable_set_q_>`
  *  :ref:`xpath_compile (query: string) : xpath_query? <function-pugixml_xpath_compile_string>`
  *  :ref:`xpath_query const?.result_description (query: xpath_query const?) : string <function-pugixml__dot__rq_result_description_xpath_query_const_q_>`
  *  :ref:`xpath_query const?.result_offset (query: xpath_query const?) : int <function-pugixml__dot__rq_result_offset_xpath_query_const_q_>`
  *  :ref:`xpath_query const?.return_type (query: xpath_query const?) : xpath_value_type <function-pugixml__dot__rq_return_type_xpath_query_const_q_>`

.. _function-pugixml_evaluate_boolean_xpath_query_const_q__xml_node:

.. das:function:: evaluate_boolean(query: xpath_query const?; node: xml_node) : bool

Evaluates the compiled XPath query against the given node and returns a bool result.


:Arguments: * **query** :  :ref:`xpath_query <handle-pugixml-xpath_query>`? implicit

            * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

.. _function-pugixml_evaluate_node_xpath_query_const_q__xml_node:

.. das:function:: evaluate_node(query: xpath_query const?; node: xml_node) : xpath_node

Evaluates the compiled XPath query and returns the first matching xpath_node.


:Arguments: * **query** :  :ref:`xpath_query <handle-pugixml-xpath_query>`? implicit

            * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

.. _function-pugixml_evaluate_node_set_xpath_query_const_q__xml_node:

.. das:function:: evaluate_node_set(query: xpath_query const?; node: xml_node) : xpath_node_set?

Evaluates the compiled XPath query and returns all matching nodes as an xpath_node_set.


:Arguments: * **query** :  :ref:`xpath_query <handle-pugixml-xpath_query>`? implicit

            * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

.. _function-pugixml_evaluate_number_xpath_query_const_q__xml_node:

.. das:function:: evaluate_number(query: xpath_query const?; node: xml_node) : double

Evaluates the compiled XPath query against the given node and returns a numeric result.


:Arguments: * **query** :  :ref:`xpath_query <handle-pugixml-xpath_query>`? implicit

            * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

.. _function-pugixml_evaluate_string_xpath_query_const_q__xml_node:

.. das:function:: evaluate_string(query: xpath_query const?; node: xml_node) : string

Evaluates the compiled XPath query against the given node and returns a string result.


:Arguments: * **query** :  :ref:`xpath_query <handle-pugixml-xpath_query>`? implicit

            * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit


xpath_compile
^^^^^^^^^^^^^

.. _function-pugixml_xpath_compile_string_xpath_variable_set_q_:

.. das:function:: xpath_compile(query: string; variables: xpath_variable_set?) : xpath_query?

Compiles an XPath expression string into an xpath_query. Optionally accepts an xpath_variable_set for parameterized queries.


:Arguments: * **query** : string implicit

            * **variables** :  :ref:`xpath_variable_set <handle-pugixml-xpath_variable_set>`? implicit

.. _function-pugixml_xpath_compile_string:

.. das:function:: xpath_compile(query: string) : xpath_query?

----

.. _function-pugixml__dot__rq_result_description_xpath_query_const_q_:

.. das:function:: xpath_query const?.result_description(query: xpath_query const?) : string

Returns a human-readable error description if the XPath query failed to compile.


:Arguments: * **query** :  :ref:`xpath_query <handle-pugixml-xpath_query>`? implicit

.. _function-pugixml__dot__rq_result_offset_xpath_query_const_q_:

.. das:function:: xpath_query const?.result_offset(query: xpath_query const?) : int

Returns the character offset in the query string where the compilation error occurred.


:Arguments: * **query** :  :ref:`xpath_query <handle-pugixml-xpath_query>`? implicit

.. _function-pugixml__dot__rq_return_type_xpath_query_const_q_:

.. das:function:: xpath_query const?.return_type(query: xpath_query const?) : xpath_value_type

Returns the XPath result type (node_set, number, string, or boolean).


:Arguments: * **query** :  :ref:`xpath_query <handle-pugixml-xpath_query>`? implicit


+++++++++++++++
XPath selection
+++++++++++++++

  *  :ref:`select_node (node: xml_node; query: xpath_query const?) : xpath_node <function-pugixml_select_node_xml_node_xpath_query_const_q_>`
  *  :ref:`select_node (node: xml_node; query: string) : xpath_node <function-pugixml_select_node_xml_node_string>`
  *  :ref:`select_nodes (node: xml_node; query: xpath_query const?) : xpath_node_set? <function-pugixml_select_nodes_xml_node_xpath_query_const_q_>`
  *  :ref:`select_nodes (node: xml_node; query: string) : xpath_node_set? <function-pugixml_select_nodes_xml_node_string>`
  *  :ref:`xpath_node.node (xpath_node: xpath_node) : xml_node <function-pugixml__dot__rq_node_xpath_node>`


select_node
^^^^^^^^^^^

.. _function-pugixml_select_node_xml_node_xpath_query_const_q_:

.. das:function:: select_node(node: xml_node; query: xpath_query const?) : xpath_node

Selects the first node matching the XPath query string or compiled query.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

            * **query** :  :ref:`xpath_query <handle-pugixml-xpath_query>`? implicit

.. _function-pugixml_select_node_xml_node_string:

.. das:function:: select_node(node: xml_node; query: string) : xpath_node

----


select_nodes
^^^^^^^^^^^^

.. _function-pugixml_select_nodes_xml_node_xpath_query_const_q_:

.. das:function:: select_nodes(node: xml_node; query: xpath_query const?) : xpath_node_set?

Selects all nodes matching the XPath query and returns them as an xpath_node_set.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

            * **query** :  :ref:`xpath_query <handle-pugixml-xpath_query>`? implicit

.. _function-pugixml_select_nodes_xml_node_string:

.. das:function:: select_nodes(node: xml_node; query: string) : xpath_node_set?

----

.. _function-pugixml__dot__rq_node_xpath_node:

.. das:function:: xpath_node.node(xpath_node: xpath_node) : xml_node

Returns the xml_node associated with this XPath result, if any.


:Arguments: * **xpath_node** :  :ref:`xpath_node <handle-pugixml-xpath_node>` implicit


+++++++++++++++++++++++++
XPath node set operations
+++++++++++++++++++++++++

  *  :ref:`at (set: xpath_node_set const?; index: int) : xpath_node <function-pugixml_at_xpath_node_set_const_q__int>`
  *  :ref:`sort (set: xpath_node_set?; reverse: bool) <function-pugixml_sort_xpath_node_set_q__bool>`
  *  :ref:`xpath_node_set const?.empty (set: xpath_node_set const?) : bool <function-pugixml__dot__rq_empty_xpath_node_set_const_q_>`
  *  :ref:`xpath_node_set const?.first (set: xpath_node_set const?) : xpath_node <function-pugixml__dot__rq_first_xpath_node_set_const_q_>`
  *  :ref:`xpath_node_set const?.size (set: xpath_node_set const?) : int <function-pugixml__dot__rq_size_xpath_node_set_const_q_>`

.. _function-pugixml_at_xpath_node_set_const_q__int:

.. das:function:: at(set: xpath_node_set const?; index: int) : xpath_node

Returns the xpath_node at the given zero-based index in the node set.


:Arguments: * **set** :  :ref:`xpath_node_set <handle-pugixml-xpath_node_set>`? implicit

            * **index** : int

.. _function-pugixml_sort_xpath_node_set_q__bool:

.. das:function:: sort(set: xpath_node_set?; reverse: bool)

Sorts the xpath_node_set in document order (or reverse document order if *reverse* is true).


:Arguments: * **set** :  :ref:`xpath_node_set <handle-pugixml-xpath_node_set>`? implicit

            * **reverse** : bool

.. _function-pugixml__dot__rq_empty_xpath_node_set_const_q_:

.. das:function:: xpath_node_set const?.empty(set: xpath_node_set const?) : bool

Returns true if the xpath_node_set contains no results.


:Arguments: * **set** :  :ref:`xpath_node_set <handle-pugixml-xpath_node_set>`? implicit

.. _function-pugixml__dot__rq_first_xpath_node_set_const_q_:

.. das:function:: xpath_node_set const?.first(set: xpath_node_set const?) : xpath_node

Returns the first xpath_node in the set (in document order).


:Arguments: * **set** :  :ref:`xpath_node_set <handle-pugixml-xpath_node_set>`? implicit

.. _function-pugixml__dot__rq_size_xpath_node_set_const_q_:

.. das:function:: xpath_node_set const?.size(set: xpath_node_set const?) : int

Returns the number of xpath_node entries in the node set.


:Arguments: * **set** :  :ref:`xpath_node_set <handle-pugixml-xpath_node_set>`? implicit


+++++++++++++++++++++
Construction and RAII
+++++++++++++++++++++

  *  :ref:`using (arg0: block\<(xml_document):void\>) <function-pugixml_using_block_ls_xml_document_c_void_gr_>`
  *  :ref:`using (arg0: block\<(xpath_variable_set):void\>) <function-pugixml_using_block_ls_xpath_variable_set_c_void_gr_>`
  *  :ref:`using (arg0: block\<(xpath_node_set):void\>) <function-pugixml_using_block_ls_xpath_node_set_c_void_gr_>`
  *  :ref:`xml_document () : xml_document <function-pugixml_xml_document>`
  *  :ref:`xpath_node_set () : xpath_node_set <function-pugixml_xpath_node_set>`
  *  :ref:`xpath_variable_set () : xpath_variable_set <function-pugixml_xpath_variable_set>`


using
^^^^^

.. _function-pugixml_using_block_ls_xml_document_c_void_gr_:

.. das:function:: using(arg0: block<(xml_document):void>)

Constructs an xml_document, passes it to the block, then destroys it automatically.


:Arguments: * **arg0** : block<( :ref:`xml_document <handle-pugixml-xml_document>`):void> implicit

.. _function-pugixml_using_block_ls_xpath_variable_set_c_void_gr_:

.. das:function:: using(arg0: block<(xpath_variable_set):void>)

.. _function-pugixml_using_block_ls_xpath_node_set_c_void_gr_:

.. das:function:: using(arg0: block<(xpath_node_set):void>)

----

.. _function-pugixml_xml_document:

.. das:function:: xml_document() : xml_document

Constructs a new empty xml_document.


.. _function-pugixml_xpath_node_set:

.. das:function:: xpath_node_set() : xpath_node_set

Constructs a new empty xpath_node_set.


.. _function-pugixml_xpath_variable_set:

.. das:function:: xpath_variable_set() : xpath_variable_set

Constructs a new empty xpath_variable_set.



++++++++++++++++++++++++++++++
Handle validity and comparison
++++++++++++++++++++++++++++++

  *  :ref:`xml_attribute\!= (attr_a: xml_attribute; attr_b: xml_attribute) : bool <function-pugixml__ex__eq__xml_attribute_xml_attribute>`
  *  :ref:`xml_attribute.hash_value (attribute: xml_attribute) : uint64 <function-pugixml__dot__rq_hash_value_xml_attribute>`
  *  :ref:`xml_attribute.ok (attribute: xml_attribute) : bool <function-pugixml__dot__rq_ok_xml_attribute>`
  *  :ref:`xml_attribute== (attr_a: xml_attribute; attr_b: xml_attribute) : bool <function-pugixml__eq__eq__xml_attribute_xml_attribute>`
  *  :ref:`xml_node\!= (node_a: xml_node; node_b: xml_node) : bool <function-pugixml__ex__eq__xml_node_xml_node>`
  *  :ref:`xml_node.hash_value (node: xml_node) : uint64 <function-pugixml__dot__rq_hash_value_xml_node>`
  *  :ref:`xml_node.offset_debug (node: xml_node) : int <function-pugixml__dot__rq_offset_debug_xml_node>`
  *  :ref:`xml_node.ok (node: xml_node) : bool <function-pugixml__dot__rq_ok_xml_node>`
  *  :ref:`xml_node== (node_a: xml_node; node_b: xml_node) : bool <function-pugixml__eq__eq__xml_node_xml_node>`
  *  :ref:`xml_text.ok (text: xml_text) : bool <function-pugixml__dot__rq_ok_xml_text>`
  *  :ref:`xpath_node.ok (xpath_node: xpath_node) : bool <function-pugixml__dot__rq_ok_xpath_node>`
  *  :ref:`xpath_query const?.ok (query: xpath_query const?) : bool <function-pugixml__dot__rq_ok_xpath_query_const_q_>`

.. _function-pugixml__ex__eq__xml_attribute_xml_attribute:

.. das:function:: xml_attribute!=(attr_a: xml_attribute; attr_b: xml_attribute) : bool

Returns true if two xml_attribute handles refer to different attributes.


:Arguments: * **attr_a** :  :ref:`xml_attribute <handle-pugixml-xml_attribute>` implicit

            * **attr_b** :  :ref:`xml_attribute <handle-pugixml-xml_attribute>` implicit

.. _function-pugixml__dot__rq_hash_value_xml_attribute:

.. das:function:: xml_attribute.hash_value(attribute: xml_attribute) : uint64

Returns a hash value for the attribute handle, usable as a table key.


:Arguments: * **attribute** :  :ref:`xml_attribute <handle-pugixml-xml_attribute>` implicit

.. _function-pugixml__dot__rq_ok_xml_attribute:

.. das:function:: xml_attribute.ok(attribute: xml_attribute) : bool

Returns true if the attribute handle is valid (non-null).


:Arguments: * **attribute** :  :ref:`xml_attribute <handle-pugixml-xml_attribute>` implicit

.. _function-pugixml__eq__eq__xml_attribute_xml_attribute:

.. das:function:: xml_attribute==(attr_a: xml_attribute; attr_b: xml_attribute) : bool

Returns true if two xml_attribute handles refer to the same attribute.


:Arguments: * **attr_a** :  :ref:`xml_attribute <handle-pugixml-xml_attribute>` implicit

            * **attr_b** :  :ref:`xml_attribute <handle-pugixml-xml_attribute>` implicit

.. _function-pugixml__ex__eq__xml_node_xml_node:

.. das:function:: xml_node!=(node_a: xml_node; node_b: xml_node) : bool

Returns true if two xml_node handles refer to different DOM nodes.


:Arguments: * **node_a** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

            * **node_b** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

.. _function-pugixml__dot__rq_hash_value_xml_node:

.. das:function:: xml_node.hash_value(node: xml_node) : uint64

Returns a hash value for the node handle, usable as a table key.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

.. _function-pugixml__dot__rq_offset_debug_xml_node:

.. das:function:: xml_node.offset_debug(node: xml_node) : int

Returns the byte offset of this node in the original parsed XML source.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

.. _function-pugixml__dot__rq_ok_xml_node:

.. das:function:: xml_node.ok(node: xml_node) : bool

Returns true if the node handle is valid (non-null).


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

.. _function-pugixml__eq__eq__xml_node_xml_node:

.. das:function:: xml_node==(node_a: xml_node; node_b: xml_node) : bool

Returns true if two xml_node handles refer to the same DOM node.


:Arguments: * **node_a** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

            * **node_b** :  :ref:`xml_node <handle-pugixml-xml_node>` implicit

.. _function-pugixml__dot__rq_ok_xml_text:

.. das:function:: xml_text.ok(text: xml_text) : bool

Returns true if the text handle is valid (non-null).


:Arguments: * **text** :  :ref:`xml_text <handle-pugixml-xml_text>` implicit

.. _function-pugixml__dot__rq_ok_xpath_node:

.. das:function:: xpath_node.ok(xpath_node: xpath_node) : bool

Returns true if the xpath_node result is valid (non-null).


:Arguments: * **xpath_node** :  :ref:`xpath_node <handle-pugixml-xpath_node>` implicit

.. _function-pugixml__dot__rq_ok_xpath_query_const_q_:

.. das:function:: xpath_query const?.ok(query: xpath_query const?) : bool

Returns true if the XPath query compiled successfully.


:Arguments: * **query** :  :ref:`xpath_query <handle-pugixml-xpath_query>`? implicit


