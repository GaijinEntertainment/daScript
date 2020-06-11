
.. _stdlib_json:

=========================
JSON manipulation library
=========================

++++++++++++
Type aliases
++++++++++++

.. _alias_JsValue:

.. das:attribute:: JsValueis a variant type

+-------+---------------------------------------------------------------+
+_object+table<string; :ref:`json::JsonValue <struct-json-JsonValue>` ?>+
+-------+---------------------------------------------------------------+
+_array +array< :ref:`json::JsonValue <struct-json-JsonValue>` ?>       +
+-------+---------------------------------------------------------------+
+_string+string                                                         +
+-------+---------------------------------------------------------------+
+_number+double                                                         +
+-------+---------------------------------------------------------------+
+_bool  +bool                                                           +
+-------+---------------------------------------------------------------+
+_null  +void?                                                          +
+-------+---------------------------------------------------------------+


to be documented

.. _alias_Token:

.. das:attribute:: Tokenis a variant type

+-------+------+
+_string+string+
+-------+------+
+_number+double+
+-------+------+
+_bool  +bool  +
+-------+------+
+_null  +void? +
+-------+------+
+_symbol+int   +
+-------+------+
+_error +string+
+-------+------+


to be documented

++++++++++
Structures
++++++++++

.. _struct-json-JsonValue:

.. das:attribute:: JsonValue

JsonValue fields are

+-----+--------------------------------+
+value+ :ref:`JsValue <alias_JsValue>` +
+-----+--------------------------------+


to be documented


+++++++++
Functions
+++++++++

.. das:function:: JV ( v:array<json::JsonValue?> )  : json::JsonValue?

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?



function arguments are

+-+--------------------------------------------------------+
+v+array< :ref:`json::JsonValue <struct-json-JsonValue>` ?>+
+-+--------------------------------------------------------+



to be documented


.. das:function:: JV ( v:void? const )  : json::JsonValue?

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?



function arguments are

+-+-----------+
+v+void? const+
+-+-----------+



to be documented


.. das:function:: JV ( v:table<string;json::JsonValue?> )  : json::JsonValue?

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?



function arguments are

+-+---------------------------------------------------------------+
+v+table<string; :ref:`json::JsonValue <struct-json-JsonValue>` ?>+
+-+---------------------------------------------------------------+



to be documented


.. das:function:: JV ( v:bool const )  : json::JsonValue?

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?



function arguments are

+-+----------+
+v+bool const+
+-+----------+



to be documented


.. das:function:: JV ( v:double const )  : json::JsonValue?

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?



function arguments are

+-+------------+
+v+double const+
+-+------------+



to be documented


.. das:function:: JV ( v:string const )  : json::JsonValue?

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?



function arguments are

+-+------------+
+v+string const+
+-+------------+



to be documented


.. das:function:: read_json ( text:array<uint8> const; error:string& )  : json::JsonValue?

read_json returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?



function arguments are

+-----+------------------+
+text +array<uint8> const+
+-----+------------------+
+error+string&           +
+-----+------------------+



to be documented


.. das:function:: read_json ( text:string const; error:string& )  : json::JsonValue?

read_json returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?



function arguments are

+-----+------------+
+text +string const+
+-----+------------+
+error+string&     +
+-----+------------+



to be documented


.. das:function:: write_json ( val:json::JsonValue? const )  : string

write_json returns string



function arguments are

+---+-------------------------------------------------------+
+val+ :ref:`json::JsonValue <struct-json-JsonValue>` ? const+
+---+-------------------------------------------------------+



to be documented



