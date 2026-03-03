
.. _stdlib_PUGIXML_boost:

======================================
High-level XML helpers (pugixml boost)
======================================

.. das:module:: PUGIXML_boost

The PUGIXML_BOOST module provides high-level daScript helpers on top of the
low-level ``pugixml`` C++ bindings.

Includes RAII document management (``open_xml``, ``parse_xml``, ``with_doc``),
iterator-based traversal (``each``, ``each_child``, ``each_attribute``),
a builder EDSL (``tag``, ``attr``), quick attribute/text accessors, XPath
convenience functions, ``is``/``as`` type-conversion operators, and generic
struct ↔ XML serialization (``to_XML``, ``from_XML``, ``XML``).

All functions and symbols are in "PUGIXML_boost" module, use require to get access to it.

.. code-block:: das

    require pugixml/PUGIXML_boost

See also:

  * :ref:`pugixml <stdlib_pugixml>` — low-level C++ bindings
  * `Tutorial 01 — Parsing and navigation <https://github.com/GaijinEntertainment/daScript/blob/master/tutorials/dasPUGIXML/01_parsing_and_navigation.das>`_
  * `Tutorial 02 — Building XML <https://github.com/GaijinEntertainment/daScript/blob/master/tutorials/dasPUGIXML/02_building_xml.das>`_
  * `Tutorial 03 — XPath <https://github.com/GaijinEntertainment/daScript/blob/master/tutorials/dasPUGIXML/03_xpath.das>`_
  * `Tutorial 04 — Serialization <https://github.com/GaijinEntertainment/daScript/blob/master/tutorials/dasPUGIXML/04_serialization.das>`_



+++++++++++++++++++++
Block-based iteration
+++++++++++++++++++++

  *  :ref:`for_each (node_set: xpath_node_set?; blk: block\<(xn:xpath_node):void\>) <function-PUGIXML_boost_for_each_xpath_node_set_q__block_ls_xn_c_xpath_node_c_void_gr_>`
  *  :ref:`for_each_attribute (node: xml_node; blk: block\<(attr:xml_attribute):void\>) <function-PUGIXML_boost_for_each_attribute_xml_node_block_ls_attr_c_xml_attribute_c_void_gr_>`
  *  :ref:`for_each_child (node: xml_node; name: string; blk: block\<(child:xml_node):void\>) <function-PUGIXML_boost_for_each_child_xml_node_string_block_ls_child_c_xml_node_c_void_gr_>`
  *  :ref:`for_each_child (node: xml_node; blk: block\<(child:xml_node):void\>) <function-PUGIXML_boost_for_each_child_xml_node_block_ls_child_c_xml_node_c_void_gr_>`

.. _function-PUGIXML_boost_for_each_xpath_node_set_q__block_ls_xn_c_xpath_node_c_void_gr_:

.. das:function:: for_each(node_set: xpath_node_set?; blk: block<(xn:xpath_node):void>)

Iterates over all ``xpath_node`` entries in an ``xpath_node_set``.


:Arguments: * **node_set** :  :ref:`xpath_node_set <handle-pugixml-xpath_node_set>`?

            * **blk** : block<(xn: :ref:`xpath_node <handle-pugixml-xpath_node>`):void>

.. _function-PUGIXML_boost_for_each_attribute_xml_node_block_ls_attr_c_xml_attribute_c_void_gr_:

.. das:function:: for_each_attribute(node: xml_node; blk: block<(attr:xml_attribute):void>)

Iterates over all attributes of *node*.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>`

            * **blk** : block<(attr: :ref:`xml_attribute <handle-pugixml-xml_attribute>`):void>


for_each_child
^^^^^^^^^^^^^^

.. _function-PUGIXML_boost_for_each_child_xml_node_string_block_ls_child_c_xml_node_c_void_gr_:

.. das:function:: for_each_child(node: xml_node; name: string; blk: block<(child:xml_node):void>)

Iterates over child elements of *node* whose tag matches *name*.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>`

            * **name** : string

            * **blk** : block<(child: :ref:`xml_node <handle-pugixml-xml_node>`):void>

.. _function-PUGIXML_boost_for_each_child_xml_node_block_ls_child_c_xml_node_c_void_gr_:

.. das:function:: for_each_child(node: xml_node; blk: block<(child:xml_node):void>)


++++++++++++++++++++++++
Iterator-based iteration
++++++++++++++++++++++++

  *  :ref:`each (node_set: xpath_node_set?) : iterator\<xpath_node\> <function-PUGIXML_boost_each_xpath_node_set_q_>`
  *  :ref:`each_attribute (node: xml_node) : iterator\<xml_attribute\> <function-PUGIXML_boost_each_attribute_xml_node>`
  *  :ref:`each_child (node: xml_node; name: string) : iterator\<xml_node\> <function-PUGIXML_boost_each_child_xml_node_string>`
  *  :ref:`each_child (node: xml_node) : iterator\<xml_node\> <function-PUGIXML_boost_each_child_xml_node>`

.. _function-PUGIXML_boost_each_xpath_node_set_q_:

.. das:function:: each(node_set: xpath_node_set?) : iterator<xpath_node>

Returns a lazy iterator over all ``xpath_node`` entries in an
``xpath_node_set``. Intended for use in ``for`` loops.
Because the iterator is named ``each``, the call can be omitted:

  ``for (xn in node_set) { ... }``


:Arguments: * **node_set** :  :ref:`xpath_node_set <handle-pugixml-xpath_node_set>`?

.. _function-PUGIXML_boost_each_attribute_xml_node:

.. das:function:: each_attribute(node: xml_node) : iterator<xml_attribute>

Returns a lazy iterator over all attributes of *node*.
Intended for use in ``for`` loops:

  ``for (a in each_attribute(node)) { ... }``


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>`


each_child
^^^^^^^^^^

.. _function-PUGIXML_boost_each_child_xml_node_string:

.. das:function:: each_child(node: xml_node; name: string) : iterator<xml_node>

Returns a lazy iterator over child elements of *node* whose tag
matches *name*. Intended for use in ``for`` loops:

  ``for (ch in each_child(node, "item")) { ... }``


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>`

            * **name** : string

.. _function-PUGIXML_boost_each_child_xml_node:

.. das:function:: each_child(node: xml_node) : iterator<xml_node>


++++++++++++++++++++++
RAII document handling
++++++++++++++++++++++

  *  :ref:`open_xml (filename: string; blk: block\<(doc:xml_document?;success:bool):void\>) <function-PUGIXML_boost_open_xml_string_block_ls_doc_c_xml_document_q_;success_c_bool_c_void_gr_>`
  *  :ref:`parse_xml (content: string; blk: block\<(doc:xml_document?;success:bool):void\>) <function-PUGIXML_boost_parse_xml_string_block_ls_doc_c_xml_document_q_;success_c_bool_c_void_gr_>`
  *  :ref:`with_doc (blk: block\<(doc:xml_document?):void\>) <function-PUGIXML_boost_with_doc_block_ls_doc_c_xml_document_q__c_void_gr_>`

.. _function-PUGIXML_boost_open_xml_string_block_ls_doc_c_xml_document_q_;success_c_bool_c_void_gr_:

.. das:function:: open_xml(filename: string; blk: block<(doc:xml_document?;success:bool):void>)

Opens an XML file, invokes *blk* with the parsed document and a
success flag, then automatically frees the document. The caller
never needs ``unsafe { delete ... }``.


:Arguments: * **filename** : string

            * **blk** : block<(doc: :ref:`xml_document <handle-pugixml-xml_document>`?;success:bool):void>

.. _function-PUGIXML_boost_parse_xml_string_block_ls_doc_c_xml_document_q_;success_c_bool_c_void_gr_:

.. das:function:: parse_xml(content: string; blk: block<(doc:xml_document?;success:bool):void>)

Parses an XML string, invokes *blk* with the document and a
success flag, then automatically frees the document.


:Arguments: * **content** : string

            * **blk** : block<(doc: :ref:`xml_document <handle-pugixml-xml_document>`?;success:bool):void>

.. _function-PUGIXML_boost_with_doc_block_ls_doc_c_xml_document_q__c_void_gr_:

.. das:function:: with_doc(blk: block<(doc:xml_document?):void>)

Creates an empty ``xml_document``, invokes *blk*, then frees it.
Use this when building XML from scratch.


:Arguments: * **blk** : block<(doc: :ref:`xml_document <handle-pugixml-xml_document>`?):void>


+++++++++++++++
Quick accessors
+++++++++++++++

  *  :ref:`node_attr (node: xml_node; attr_name: string; default_value: string = "") : string <function-PUGIXML_boost_node_attr_xml_node_string_string>`
  *  :ref:`node_attr_bool (node: xml_node; attr_name: string; default_value: bool = false) : bool <function-PUGIXML_boost_node_attr_bool_xml_node_string_bool>`
  *  :ref:`node_attr_float (node: xml_node; attr_name: string; default_value: float = 0f) : float <function-PUGIXML_boost_node_attr_float_xml_node_string_float>`
  *  :ref:`node_attr_int (node: xml_node; attr_name: string; default_value: int = 0) : int <function-PUGIXML_boost_node_attr_int_xml_node_string_int>`
  *  :ref:`node_text (node: xml_node; child_name: string; default_value: string = "") : string <function-PUGIXML_boost_node_text_xml_node_string_string>`

.. _function-PUGIXML_boost_node_attr_xml_node_string_string:

.. das:function:: node_attr(node: xml_node; attr_name: string; default_value: string = "") : string

Returns the string value of attribute *attr_name*,
or *default_value* if no such attribute exists.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>`

            * **attr_name** : string

            * **default_value** : string

.. _function-PUGIXML_boost_node_attr_bool_xml_node_string_bool:

.. das:function:: node_attr_bool(node: xml_node; attr_name: string; default_value: bool = false) : bool

Returns the boolean value of attribute *attr_name*.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>`

            * **attr_name** : string

            * **default_value** : bool

.. _function-PUGIXML_boost_node_attr_float_xml_node_string_float:

.. das:function:: node_attr_float(node: xml_node; attr_name: string; default_value: float = 0f) : float

Returns the float value of attribute *attr_name*.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>`

            * **attr_name** : string

            * **default_value** : float

.. _function-PUGIXML_boost_node_attr_int_xml_node_string_int:

.. das:function:: node_attr_int(node: xml_node; attr_name: string; default_value: int = 0) : int

Returns the integer value of attribute *attr_name*.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>`

            * **attr_name** : string

            * **default_value** : int

.. _function-PUGIXML_boost_node_text_xml_node_string_string:

.. das:function:: node_text(node: xml_node; child_name: string; default_value: string = "") : string

Returns the text content of the child element named *child_name*,
or *default_value* if the child does not exist or has no text.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>`

            * **child_name** : string

            * **default_value** : string


+++++++++++++++
Builder helpers
+++++++++++++++

  *  :ref:`add_attr (var node: xml_node; name: string; value: string) : xml_attribute <function-PUGIXML_boost_add_attr_xml_node_string_string>`
  *  :ref:`add_attr (var node: xml_node; name: string; value: float) : xml_attribute <function-PUGIXML_boost_add_attr_xml_node_string_float>`
  *  :ref:`add_attr (var node: xml_node; name: string; value: int) : xml_attribute <function-PUGIXML_boost_add_attr_xml_node_string_int>`
  *  :ref:`add_attr (var node: xml_node; name: string; value: bool) : xml_attribute <function-PUGIXML_boost_add_attr_xml_node_string_bool>`
  *  :ref:`add_child (var node: xml_node; name: string; text_content: string) : xml_node <function-PUGIXML_boost_add_child_xml_node_string_string>`
  *  :ref:`add_child_ex (var node: xml_node; name: string; attr_name: string; attr_value: string) : xml_node <function-PUGIXML_boost_add_child_ex_xml_node_string_string_string>`


add_attr
^^^^^^^^

.. _function-PUGIXML_boost_add_attr_xml_node_string_string:

.. das:function:: add_attr(node: xml_node; name: string; value: string) : xml_attribute

Appends an attribute with *name* and *value* (string).


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>`

            * **name** : string

            * **value** : string

.. _function-PUGIXML_boost_add_attr_xml_node_string_float:

.. das:function:: add_attr(node: xml_node; name: string; value: float) : xml_attribute

.. _function-PUGIXML_boost_add_attr_xml_node_string_int:

.. das:function:: add_attr(node: xml_node; name: string; value: int) : xml_attribute

.. _function-PUGIXML_boost_add_attr_xml_node_string_bool:

.. das:function:: add_attr(node: xml_node; name: string; value: bool) : xml_attribute

----

.. _function-PUGIXML_boost_add_child_xml_node_string_string:

.. das:function:: add_child(node: xml_node; name: string; text_content: string) : xml_node

Appends a child element with *name* and sets its text content.
Returns the new child node.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>`

            * **name** : string

            * **text_content** : string

.. _function-PUGIXML_boost_add_child_ex_xml_node_string_string_string:

.. das:function:: add_child_ex(node: xml_node; name: string; attr_name: string; attr_value: string) : xml_node

Appends a child element with *name* and a single attribute.
Returns the new child node.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>`

            * **name** : string

            * **attr_name** : string

            * **attr_value** : string


++++++++++++
Builder EDSL
++++++++++++

  *  :ref:`attr (var node: xml_node; name: string; value: int) : xml_node <function-PUGIXML_boost_attr_xml_node_string_int>`
  *  :ref:`attr (var node: xml_node; name: string; value: bool) : xml_node <function-PUGIXML_boost_attr_xml_node_string_bool>`
  *  :ref:`attr (var node: xml_node; name: string; value: string) : xml_node <function-PUGIXML_boost_attr_xml_node_string_string>`
  *  :ref:`attr (var node: xml_node; name: string; value: float) : xml_node <function-PUGIXML_boost_attr_xml_node_string_float>`
  *  :ref:`tag (var node: xml_node; name: string; text_content: string) : xml_node <function-PUGIXML_boost_tag_xml_node_string_string>`
  *  :ref:`tag (var node: xml_node; name: string; blk: block\<(var child:xml_node):void\>) : xml_node <function-PUGIXML_boost_tag_xml_node_string_block_ls_var_child_c_xml_node_c_void_gr_>`
  *  :ref:`tag (doc: xml_document?; name: string) : xml_node <function-PUGIXML_boost_tag_xml_document_q__string>`
  *  :ref:`tag (doc: xml_document?; name: string; text_content: string) : xml_node <function-PUGIXML_boost_tag_xml_document_q__string_string>`
  *  :ref:`tag (var node: xml_node; name: string) : xml_node <function-PUGIXML_boost_tag_xml_node_string>`
  *  :ref:`tag (doc: xml_document?; name: string; blk: block\<(var child:xml_node):void\>) : xml_node <function-PUGIXML_boost_tag_xml_document_q__string_block_ls_var_child_c_xml_node_c_void_gr_>`


attr
^^^^

.. _function-PUGIXML_boost_attr_xml_node_string_int:

.. das:function:: attr(node: xml_node; name: string; value: int) : xml_node

Appends an int attribute, returns parent for chaining.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>`

            * **name** : string

            * **value** : int

.. _function-PUGIXML_boost_attr_xml_node_string_bool:

.. das:function:: attr(node: xml_node; name: string; value: bool) : xml_node

.. _function-PUGIXML_boost_attr_xml_node_string_string:

.. das:function:: attr(node: xml_node; name: string; value: string) : xml_node

.. _function-PUGIXML_boost_attr_xml_node_string_float:

.. das:function:: attr(node: xml_node; name: string; value: float) : xml_node

----


tag
^^^

.. _function-PUGIXML_boost_tag_xml_node_string_string:

.. das:function:: tag(node: xml_node; name: string; text_content: string) : xml_node

Appends a child element with *name* and text content. Returns
the new child. Use for leaf elements:

  ``node |> tag("title", "The C Programming Language")``


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>`

            * **name** : string

            * **text_content** : string

.. _function-PUGIXML_boost_tag_xml_node_string_block_ls_var_child_c_xml_node_c_void_gr_:

.. das:function:: tag(node: xml_node; name: string; blk: block<(var child:xml_node):void>) : xml_node

.. _function-PUGIXML_boost_tag_xml_document_q__string:

.. das:function:: tag(doc: xml_document?; name: string) : xml_node

.. _function-PUGIXML_boost_tag_xml_document_q__string_string:

.. das:function:: tag(doc: xml_document?; name: string; text_content: string) : xml_node

.. _function-PUGIXML_boost_tag_xml_node_string:

.. das:function:: tag(node: xml_node; name: string) : xml_node

.. _function-PUGIXML_boost_tag_xml_document_q__string_block_ls_var_child_c_xml_node_c_void_gr_:

.. das:function:: tag(doc: xml_document?; name: string; blk: block<(var child:xml_node):void>) : xml_node


+++++++++++++++++
String conversion
+++++++++++++++++

  *  :ref:`to_string (node: xml_node) : string <function-PUGIXML_boost_to_string_xml_node>`
  *  :ref:`to_string (doc: xml_document?) : string <function-PUGIXML_boost_to_string_xml_document_q_>`


to_string
^^^^^^^^^

.. _function-PUGIXML_boost_to_string_xml_node:

.. das:function:: to_string(node: xml_node) : string

Serializes a single node (and its subtree) to an XML string
with two-space indentation.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>`

.. _function-PUGIXML_boost_to_string_xml_document_q_:

.. das:function:: to_string(doc: xml_document?) : string


+++++++++++++
XPath helpers
+++++++++++++

  *  :ref:`for_each_select (node: xml_node; xpath_query: string; blk: block\<(xn:xpath_node):void\>) <function-PUGIXML_boost_for_each_select_xml_node_string_block_ls_xn_c_xpath_node_c_void_gr_>`
  *  :ref:`select_text (node: xml_node; xpath_query: string; default_value: string = "") : string <function-PUGIXML_boost_select_text_xml_node_string_string>`
  *  :ref:`select_value (node: xml_node; xpath_query: string; default_value: string = "") : string <function-PUGIXML_boost_select_value_xml_node_string_string>`
  *  :ref:`with_xpath (query_str: string; blk: block\<(query:xpath_query?):void\>) <function-PUGIXML_boost_with_xpath_string_block_ls_query_c_xpath_query_q__c_void_gr_>`

.. _function-PUGIXML_boost_for_each_select_xml_node_string_block_ls_xn_c_xpath_node_c_void_gr_:

.. das:function:: for_each_select(node: xml_node; xpath_query: string; blk: block<(xn:xpath_node):void>)

Selects all nodes matching *xpath_query* and invokes *blk* for
each result. The temporary ``xpath_node_set`` is freed automatically.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>`

            * **xpath_query** : string

            * **blk** : block<(xn: :ref:`xpath_node <handle-pugixml-xpath_node>`):void>

.. _function-PUGIXML_boost_select_text_xml_node_string_string:

.. das:function:: select_text(node: xml_node; xpath_query: string; default_value: string = "") : string

Runs an XPath query and returns the text content of the first
matching node, or *default_value* if nothing matches.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>`

            * **xpath_query** : string

            * **default_value** : string

.. _function-PUGIXML_boost_select_value_xml_node_string_string:

.. das:function:: select_value(node: xml_node; xpath_query: string; default_value: string = "") : string

Runs an XPath query and returns the value of the first matching
node. If the result is an attribute, returns its string value;
if an element, returns its text content.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>`

            * **xpath_query** : string

            * **default_value** : string

.. _function-PUGIXML_boost_with_xpath_string_block_ls_query_c_xpath_query_q__c_void_gr_:

.. das:function:: with_xpath(query_str: string; blk: block<(query:xpath_query?):void>)

Compiles an XPath expression, invokes *blk* with the compiled
query handle, then frees it. Avoids manual ``delete``.


:Arguments: * **query_str** : string

            * **blk** : block<(query: :ref:`xpath_query <handle-pugixml-xpath_query>`?):void>


+++++++++++++
Serialization
+++++++++++++

  *  :ref:`XML (var node: xml_node; value: auto(TT)) : auto <function-PUGIXML_boost_XML_xml_node_autoTT_0x311>`
  *  :ref:`XML_table (var node: xml_node; value: table\<auto(KT), void\>) : auto <function-PUGIXML_boost_XML_table_xml_node_table_ls_autoKT,_void_gr_>`
  *  :ref:`XML_table (var node: xml_node; value: table\<auto(KT), auto(VT)\>) : auto <function-PUGIXML_boost_XML_table_xml_node_table_ls_autoKT,_autoVT_gr_>`
  *  :ref:`from_XML (node: xml_node; ent: double; defV: double = 0lf) : double <function-PUGIXML_boost_from_XML_xml_node_double_double>`
  *  :ref:`from_XML (node: xml_node; ent: int8; defV: int8 = int8(0)) : int8 <function-PUGIXML_boost_from_XML_xml_node_int8_int8>`
  *  :ref:`from_XML (node: xml_node; ent: uint; defV: uint = 0x0) : uint <function-PUGIXML_boost_from_XML_xml_node_uint_uint>`
  *  :ref:`from_XML (node: xml_node; ent: uint8; defV: uint8 = uint8(0)) : uint8 <function-PUGIXML_boost_from_XML_xml_node_uint8_uint8>`
  *  :ref:`from_XML (node: xml_node; ent: int; defV: int = 0) : int <function-PUGIXML_boost_from_XML_xml_node_int_int>`
  *  :ref:`from_XML (node: xml_node; ent: int64; defV: int64 = 0) : int64 <function-PUGIXML_boost_from_XML_xml_node_int64_int64>`
  *  :ref:`from_XML (node: xml_node; ent: uint16; defV: uint16 = uint16(0)) : uint16 <function-PUGIXML_boost_from_XML_xml_node_uint16_uint16>`
  *  :ref:`from_XML (node: xml_node; ent: uint64; defV: uint64 = 0x0) : uint64 <function-PUGIXML_boost_from_XML_xml_node_uint64_uint64>`
  *  :ref:`from_XML (node: xml_node; anything: auto(TT)) : auto <function-PUGIXML_boost_from_XML_xml_node_autoTT_0x270>`
  *  :ref:`from_XML (node: xml_node; ent: auto(VecT); defV: VecT = VecT()) : VecT <function-PUGIXML_boost_from_XML_xml_node_autoVecT_VecT_0x2ce>`
  *  :ref:`from_XML (node: xml_node; ent: string; defV: string = "") : string <function-PUGIXML_boost_from_XML_xml_node_string_string>`
  *  :ref:`from_XML (node: xml_node; ent: float; defV: float = 0f) : float <function-PUGIXML_boost_from_XML_xml_node_float_float>`
  *  :ref:`from_XML (node: xml_node; ent: bool; defV: bool = false) : bool <function-PUGIXML_boost_from_XML_xml_node_bool_bool>`
  *  :ref:`from_XML (node: xml_node; ent: int16; defV: int16 = int16(0)) : int16 <function-PUGIXML_boost_from_XML_xml_node_int16_int16>`
  *  :ref:`from_XML (node: xml_node; anything: table\<auto(KT), void\>) : auto <function-PUGIXML_boost_from_XML_xml_node_table_ls_autoKT,_void_gr_>`
  *  :ref:`from_XML (node: xml_node; anything: table\<auto(KT), auto(VT)\>) : auto <function-PUGIXML_boost_from_XML_xml_node_table_ls_autoKT,_autoVT_gr_>`
  *  :ref:`from_XML (xml_str: string; anything: auto(TT)) : auto <function-PUGIXML_boost_from_XML_string_autoTT_0x39a>`
  *  :ref:`to_XML (value: auto(TT); root_name: string = "root") : string <function-PUGIXML_boost_to_XML_autoTT_string_0x38e>`

.. _function-PUGIXML_boost_XML_xml_node_autoTT_0x311:

.. das:function:: XML(node: xml_node; value: auto(TT)) : auto

Serializes a native value into XML child elements of *node*.
Structs/tuples: each field becomes a child element (via ``apply``).
Variants: stores ``_variant`` attribute with the active index.
Tables (key-value): each entry becomes an ``entry`` child with ``_key`` / ``_val`` sub-elements.
Tables (key-only): each key becomes an ``item`` child.
Arrays/dim: each element becomes an ``item`` child.
Enumerations: stored as string name. Bitfields: stored as integer value.
Primitives: sets the text content of *node*.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>`

            * **value** : auto(TT)


XML_table
^^^^^^^^^

.. _function-PUGIXML_boost_XML_table_xml_node_table_ls_autoKT,_void_gr_:

.. das:function:: XML_table(node: xml_node; value: table<auto(KT), void>) : auto

def XML_table (var node: xml_node; value: table<auto(KT), void>) : auto

:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>`

            * **value** : table<auto(KT);void>

.. _function-PUGIXML_boost_XML_table_xml_node_table_ls_autoKT,_autoVT_gr_:

.. das:function:: XML_table(node: xml_node; value: table<auto(KT), auto(VT)>) : auto

----


from_XML
^^^^^^^^

.. _function-PUGIXML_boost_from_XML_xml_node_double_double:

.. das:function:: from_XML(node: xml_node; ent: double; defV: double = 0lf) : double

Reads the text content of *node* as a ``double``.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>`

            * **ent** : double

            * **defV** : double

.. _function-PUGIXML_boost_from_XML_xml_node_int8_int8:

.. das:function:: from_XML(node: xml_node; ent: int8; defV: int8 = int8(0)) : int8

.. _function-PUGIXML_boost_from_XML_xml_node_uint_uint:

.. das:function:: from_XML(node: xml_node; ent: uint; defV: uint = 0x0) : uint

.. _function-PUGIXML_boost_from_XML_xml_node_uint8_uint8:

.. das:function:: from_XML(node: xml_node; ent: uint8; defV: uint8 = uint8(0)) : uint8

.. _function-PUGIXML_boost_from_XML_xml_node_int_int:

.. das:function:: from_XML(node: xml_node; ent: int; defV: int = 0) : int

.. _function-PUGIXML_boost_from_XML_xml_node_int64_int64:

.. das:function:: from_XML(node: xml_node; ent: int64; defV: int64 = 0) : int64

.. _function-PUGIXML_boost_from_XML_xml_node_uint16_uint16:

.. das:function:: from_XML(node: xml_node; ent: uint16; defV: uint16 = uint16(0)) : uint16

.. _function-PUGIXML_boost_from_XML_xml_node_uint64_uint64:

.. das:function:: from_XML(node: xml_node; ent: uint64; defV: uint64 = 0x0) : uint64

.. _function-PUGIXML_boost_from_XML_xml_node_autoTT_0x270:

.. das:function:: from_XML(node: xml_node; anything: auto(TT)) : auto

.. _function-PUGIXML_boost_from_XML_xml_node_autoVecT_VecT_0x2ce:

.. das:function:: from_XML(node: xml_node; ent: auto(VecT); defV: VecT = VecT()) : VecT

.. _function-PUGIXML_boost_from_XML_xml_node_string_string:

.. das:function:: from_XML(node: xml_node; ent: string; defV: string = "") : string

.. _function-PUGIXML_boost_from_XML_xml_node_float_float:

.. das:function:: from_XML(node: xml_node; ent: float; defV: float = 0f) : float

.. _function-PUGIXML_boost_from_XML_xml_node_bool_bool:

.. das:function:: from_XML(node: xml_node; ent: bool; defV: bool = false) : bool

.. _function-PUGIXML_boost_from_XML_xml_node_int16_int16:

.. das:function:: from_XML(node: xml_node; ent: int16; defV: int16 = int16(0)) : int16

.. _function-PUGIXML_boost_from_XML_xml_node_table_ls_autoKT,_void_gr_:

.. das:function:: from_XML(node: xml_node; anything: table<auto(KT), void>) : auto

.. _function-PUGIXML_boost_from_XML_xml_node_table_ls_autoKT,_autoVT_gr_:

.. das:function:: from_XML(node: xml_node; anything: table<auto(KT), auto(VT)>) : auto

.. _function-PUGIXML_boost_from_XML_string_autoTT_0x39a:

.. das:function:: from_XML(xml_str: string; anything: auto(TT)) : auto

----

.. _function-PUGIXML_boost_to_XML_autoTT_string_0x38e:

.. das:function:: to_XML(value: auto(TT); root_name: string = "root") : string

Serializes a struct to a complete XML string, wrapping it in an
element named *root_name* (default ``"root"``).


:Arguments: * **value** : auto(TT)

            * **root_name** : string


+++++++++++++++++++++++++
Type conversion operators
+++++++++++++++++++++++++

  *  :ref:`xml_attribute`as`bool (a: xml_attribute) : bool <function-PUGIXML_boost__rq_as_rq_bool_xml_attribute>`
  *  :ref:`xml_attribute`as`double (a: xml_attribute) : double <function-PUGIXML_boost__rq_as_rq_double_xml_attribute>`
  *  :ref:`xml_attribute`as`float (a: xml_attribute) : float <function-PUGIXML_boost__rq_as_rq_float_xml_attribute>`
  *  :ref:`xml_attribute`as`int (a: xml_attribute) : int <function-PUGIXML_boost__rq_as_rq_int_xml_attribute>`
  *  :ref:`xml_attribute`as`string (a: xml_attribute) : string <function-PUGIXML_boost__rq_as_rq_string_xml_attribute>`
  *  :ref:`xml_attribute`as`uint (a: xml_attribute) : uint <function-PUGIXML_boost__rq_as_rq_uint_xml_attribute>`
  *  :ref:`xml_attribute`is`bool (a: xml_attribute) : bool <function-PUGIXML_boost__rq_is_rq_bool_xml_attribute>`
  *  :ref:`xml_attribute`is`double (a: xml_attribute) : bool <function-PUGIXML_boost__rq_is_rq_double_xml_attribute>`
  *  :ref:`xml_attribute`is`float (a: xml_attribute) : bool <function-PUGIXML_boost__rq_is_rq_float_xml_attribute>`
  *  :ref:`xml_attribute`is`int (a: xml_attribute) : bool <function-PUGIXML_boost__rq_is_rq_int_xml_attribute>`
  *  :ref:`xml_attribute`is`string (a: xml_attribute) : bool <function-PUGIXML_boost__rq_is_rq_string_xml_attribute>`
  *  :ref:`xml_attribute`is`uint (a: xml_attribute) : bool <function-PUGIXML_boost__rq_is_rq_uint_xml_attribute>`
  *  :ref:`xml_document?`as`xml_node (doc: xml_document?) : xml_node <function-PUGIXML_boost__rq_as_rq_xml_node_xml_document_q_>`
  *  :ref:`xml_node[] (node: xml_node; attr_name: string) : xml_attribute <function-PUGIXML_boost__lb__rb__xml_node_string>`
  *  :ref:`xml_node`as`string (node: xml_node) : string <function-PUGIXML_boost__rq_as_rq_string_xml_node>`
  *  :ref:`xml_text`as`bool (t: xml_text) : bool <function-PUGIXML_boost__rq_as_rq_bool_xml_text>`
  *  :ref:`xml_text`as`double (t: xml_text) : double <function-PUGIXML_boost__rq_as_rq_double_xml_text>`
  *  :ref:`xml_text`as`float (t: xml_text) : float <function-PUGIXML_boost__rq_as_rq_float_xml_text>`
  *  :ref:`xml_text`as`int (t: xml_text) : int <function-PUGIXML_boost__rq_as_rq_int_xml_text>`
  *  :ref:`xml_text`as`string (t: xml_text) : string <function-PUGIXML_boost__rq_as_rq_string_xml_text>`
  *  :ref:`xml_text`as`uint (t: xml_text) : uint <function-PUGIXML_boost__rq_as_rq_uint_xml_text>`
  *  :ref:`xml_text`is`bool (t: xml_text) : bool <function-PUGIXML_boost__rq_is_rq_bool_xml_text>`
  *  :ref:`xml_text`is`double (t: xml_text) : bool <function-PUGIXML_boost__rq_is_rq_double_xml_text>`
  *  :ref:`xml_text`is`float (t: xml_text) : bool <function-PUGIXML_boost__rq_is_rq_float_xml_text>`
  *  :ref:`xml_text`is`int (t: xml_text) : bool <function-PUGIXML_boost__rq_is_rq_int_xml_text>`
  *  :ref:`xml_text`is`string (t: xml_text) : bool <function-PUGIXML_boost__rq_is_rq_string_xml_text>`
  *  :ref:`xml_text`is`uint (t: xml_text) : bool <function-PUGIXML_boost__rq_is_rq_uint_xml_text>`

.. _function-PUGIXML_boost__rq_as_rq_bool_xml_attribute:

.. das:function:: xml_attribute`as`bool(a: xml_attribute) : bool

Converts an ``xml_attribute`` value to ``bool`` (default ``false``).


:Arguments: * **a** :  :ref:`xml_attribute <handle-pugixml-xml_attribute>`

.. _function-PUGIXML_boost__rq_as_rq_double_xml_attribute:

.. das:function:: xml_attribute`as`double(a: xml_attribute) : double

Converts an ``xml_attribute`` value to ``double`` (default ``0.0lf``).


:Arguments: * **a** :  :ref:`xml_attribute <handle-pugixml-xml_attribute>`

.. _function-PUGIXML_boost__rq_as_rq_float_xml_attribute:

.. das:function:: xml_attribute`as`float(a: xml_attribute) : float

Converts an ``xml_attribute`` value to ``float`` (default ``0.0``).


:Arguments: * **a** :  :ref:`xml_attribute <handle-pugixml-xml_attribute>`

.. _function-PUGIXML_boost__rq_as_rq_int_xml_attribute:

.. das:function:: xml_attribute`as`int(a: xml_attribute) : int

Converts an ``xml_attribute`` value to ``int`` (default ``0``).


:Arguments: * **a** :  :ref:`xml_attribute <handle-pugixml-xml_attribute>`

.. _function-PUGIXML_boost__rq_as_rq_string_xml_attribute:

.. das:function:: xml_attribute`as`string(a: xml_attribute) : string

Converts an ``xml_attribute`` value to ``string`` (default ``""``).


:Arguments: * **a** :  :ref:`xml_attribute <handle-pugixml-xml_attribute>`

.. _function-PUGIXML_boost__rq_as_rq_uint_xml_attribute:

.. das:function:: xml_attribute`as`uint(a: xml_attribute) : uint

Converts an ``xml_attribute`` value to ``uint`` (default ``0u``).


:Arguments: * **a** :  :ref:`xml_attribute <handle-pugixml-xml_attribute>`

.. _function-PUGIXML_boost__rq_is_rq_bool_xml_attribute:

.. das:function:: xml_attribute`is`bool(a: xml_attribute) : bool

Returns ``true`` if the attribute exists.


:Arguments: * **a** :  :ref:`xml_attribute <handle-pugixml-xml_attribute>`

.. _function-PUGIXML_boost__rq_is_rq_double_xml_attribute:

.. das:function:: xml_attribute`is`double(a: xml_attribute) : bool

Returns ``true`` if the attribute exists.


:Arguments: * **a** :  :ref:`xml_attribute <handle-pugixml-xml_attribute>`

.. _function-PUGIXML_boost__rq_is_rq_float_xml_attribute:

.. das:function:: xml_attribute`is`float(a: xml_attribute) : bool

Returns ``true`` if the attribute exists.


:Arguments: * **a** :  :ref:`xml_attribute <handle-pugixml-xml_attribute>`

.. _function-PUGIXML_boost__rq_is_rq_int_xml_attribute:

.. das:function:: xml_attribute`is`int(a: xml_attribute) : bool

Returns ``true`` if the attribute exists (has a value convertible to int).


:Arguments: * **a** :  :ref:`xml_attribute <handle-pugixml-xml_attribute>`

.. _function-PUGIXML_boost__rq_is_rq_string_xml_attribute:

.. das:function:: xml_attribute`is`string(a: xml_attribute) : bool

Returns ``true`` if the attribute exists.


:Arguments: * **a** :  :ref:`xml_attribute <handle-pugixml-xml_attribute>`

.. _function-PUGIXML_boost__rq_is_rq_uint_xml_attribute:

.. das:function:: xml_attribute`is`uint(a: xml_attribute) : bool

Returns ``true`` if the attribute exists.


:Arguments: * **a** :  :ref:`xml_attribute <handle-pugixml-xml_attribute>`

.. _function-PUGIXML_boost__rq_as_rq_xml_node_xml_document_q_:

.. das:function:: xml_document?`as`xml_node(doc: xml_document?) : xml_node

Returns the document as an ``xml_node`` handle for mutation APIs
that require a node (e.g. ``append_child``).


:Arguments: * **doc** :  :ref:`xml_document <handle-pugixml-xml_document>`?

.. _function-PUGIXML_boost__lb__rb__xml_node_string:

.. das:function:: xml_node[](node: xml_node; attr_name: string) : xml_attribute

Returns the attribute named *attr_name*. Combine with ``is``/``as``
operators for typed access:

  ``node["price"] as float``

  ``if (node["discount"] is float) { ... }``


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>`

            * **attr_name** : string

.. _function-PUGIXML_boost__rq_as_rq_string_xml_node:

.. das:function:: xml_node`as`string(node: xml_node) : string

Serializes an ``xml_node`` (and its subtree) to an XML string.


:Arguments: * **node** :  :ref:`xml_node <handle-pugixml-xml_node>`

.. _function-PUGIXML_boost__rq_as_rq_bool_xml_text:

.. das:function:: xml_text`as`bool(t: xml_text) : bool

Converts ``xml_text`` content to ``bool`` (default ``false``).


:Arguments: * **t** :  :ref:`xml_text <handle-pugixml-xml_text>`

.. _function-PUGIXML_boost__rq_as_rq_double_xml_text:

.. das:function:: xml_text`as`double(t: xml_text) : double

Converts ``xml_text`` content to ``double`` (default ``0.0lf``).


:Arguments: * **t** :  :ref:`xml_text <handle-pugixml-xml_text>`

.. _function-PUGIXML_boost__rq_as_rq_float_xml_text:

.. das:function:: xml_text`as`float(t: xml_text) : float

Converts ``xml_text`` content to ``float`` (default ``0.0``).


:Arguments: * **t** :  :ref:`xml_text <handle-pugixml-xml_text>`

.. _function-PUGIXML_boost__rq_as_rq_int_xml_text:

.. das:function:: xml_text`as`int(t: xml_text) : int

Converts ``xml_text`` content to ``int`` (default ``0``).


:Arguments: * **t** :  :ref:`xml_text <handle-pugixml-xml_text>`

.. _function-PUGIXML_boost__rq_as_rq_string_xml_text:

.. das:function:: xml_text`as`string(t: xml_text) : string

Converts ``xml_text`` content to ``string`` (default ``""``).


:Arguments: * **t** :  :ref:`xml_text <handle-pugixml-xml_text>`

.. _function-PUGIXML_boost__rq_as_rq_uint_xml_text:

.. das:function:: xml_text`as`uint(t: xml_text) : uint

Converts ``xml_text`` content to ``uint`` (default ``0u``).


:Arguments: * **t** :  :ref:`xml_text <handle-pugixml-xml_text>`

.. _function-PUGIXML_boost__rq_is_rq_bool_xml_text:

.. das:function:: xml_text`is`bool(t: xml_text) : bool

Returns ``true`` if the text node is non-empty.


:Arguments: * **t** :  :ref:`xml_text <handle-pugixml-xml_text>`

.. _function-PUGIXML_boost__rq_is_rq_double_xml_text:

.. das:function:: xml_text`is`double(t: xml_text) : bool

Returns ``true`` if the text node is non-empty.


:Arguments: * **t** :  :ref:`xml_text <handle-pugixml-xml_text>`

.. _function-PUGIXML_boost__rq_is_rq_float_xml_text:

.. das:function:: xml_text`is`float(t: xml_text) : bool

Returns ``true`` if the text node is non-empty.


:Arguments: * **t** :  :ref:`xml_text <handle-pugixml-xml_text>`

.. _function-PUGIXML_boost__rq_is_rq_int_xml_text:

.. das:function:: xml_text`is`int(t: xml_text) : bool

Returns ``true`` if the text node is non-empty.


:Arguments: * **t** :  :ref:`xml_text <handle-pugixml-xml_text>`

.. _function-PUGIXML_boost__rq_is_rq_string_xml_text:

.. das:function:: xml_text`is`string(t: xml_text) : bool

Returns ``true`` if the text node is non-empty.


:Arguments: * **t** :  :ref:`xml_text <handle-pugixml-xml_text>`

.. _function-PUGIXML_boost__rq_is_rq_uint_xml_text:

.. das:function:: xml_text`is`uint(t: xml_text) : bool

Returns ``true`` if the text node is non-empty.


:Arguments: * **t** :  :ref:`xml_text <handle-pugixml-xml_text>`


