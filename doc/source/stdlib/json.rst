
.. _stdlib_json:

=========================
JSON manipulation library
=========================

+++++++++++
TypeAliases
+++++++++++

.. _alias-JsValue:

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


typedef|json|JsValue to be documented

.. _alias-Token:

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


typedef|json|Token to be documented

.. _struct-json-JsonValue:

.. das:attribute:: JsonValue



JsonValue fields are

+-----+--------------------------------+
+value+ :ref:`JsValue <alias-JsValue>` +
+-----+--------------------------------+


structure|json|JsonValue to be documented

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_json_c__c_JV__hh_array_hh__hh_ptr_hh_JsonValue:

.. das:function:: JV(v: array<json::JsonValue?>)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

arguments are

+-+--------------------------------------------------------+
+v+array< :ref:`json::JsonValue <struct-json-JsonValue>` ?>+
+-+--------------------------------------------------------+


function|json|JV to be documented

.. _function-_at_json_c__c_JV__hh_ptr_hh_void_hh_const:

.. das:function:: JV(v: void? const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

arguments are

+-+-----------+
+v+void? const+
+-+-----------+


function|json|JV to be documented

.. _function-_at_json_c__c_JV__hh_table_hh_string_hh__hh_ptr_hh_JsonValue:

.. das:function:: JV(v: table<string;json::JsonValue?>)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

arguments are

+-+---------------------------------------------------------------+
+v+table<string; :ref:`json::JsonValue <struct-json-JsonValue>` ?>+
+-+---------------------------------------------------------------+


function|json|JV to be documented

.. _function-_at_json_c__c_JV_bool_hh_const:

.. das:function:: JV(v: bool const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

arguments are

+-+----------+
+v+bool const+
+-+----------+


function|json|JV to be documented

.. _function-_at_json_c__c_JV_double_hh_const:

.. das:function:: JV(v: double const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

arguments are

+-+------------+
+v+double const+
+-+------------+


function|json|JV to be documented

.. _function-_at_json_c__c_JV_string_hh_const:

.. das:function:: JV(v: string const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

arguments are

+-+------------+
+v+string const+
+-+------------+


function|json|JV to be documented

.. _function-_at_json_c__c_read_json__hh_array_hh_uint8_hh_const_string_hh_ref:

.. das:function:: read_json(text: array<uint8> const; error: string&)

read_json returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

arguments are

+-----+------------------+
+text +array<uint8> const+
+-----+------------------+
+error+string&           +
+-----+------------------+


function|json|read_json to be documented

.. _function-_at_json_c__c_read_json_string_hh_const_string_hh_ref:

.. das:function:: read_json(text: string const; error: string&)

read_json returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

arguments are

+-----+------------+
+text +string const+
+-----+------------+
+error+string&     +
+-----+------------+


function|json|read_json to be documented

.. _function-_at_json_c__c_write_json__hh_ptr_hh_JsonValue_hh_const:

.. das:function:: write_json(val: json::JsonValue? const)

write_json returns string

arguments are

+---+-------------------------------------------------------+
+val+ :ref:`json::JsonValue <struct-json-JsonValue>` ? const+
+---+-------------------------------------------------------+


function|json|write_json to be documented


