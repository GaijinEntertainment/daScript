
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

.. das:function:: JV (v:array<json::JsonValue?> )  : json::JsonValue?

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

arguments are

+-+--------------------------------------------------------+
+v+array< :ref:`json::JsonValue <struct-json-JsonValue>` ?>+
+-+--------------------------------------------------------+


function|json|JV to be documented

.. das:function:: JV (v:void? const )  : json::JsonValue?

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

arguments are

+-+-----------+
+v+void? const+
+-+-----------+


function|json|JV to be documented

.. das:function:: JV (v:table<string;json::JsonValue?> )  : json::JsonValue?

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

arguments are

+-+---------------------------------------------------------------+
+v+table<string; :ref:`json::JsonValue <struct-json-JsonValue>` ?>+
+-+---------------------------------------------------------------+


function|json|JV to be documented

.. das:function:: JV (v:bool const )  : json::JsonValue?

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

arguments are

+-+----------+
+v+bool const+
+-+----------+


function|json|JV to be documented

.. das:function:: JV (v:double const )  : json::JsonValue?

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

arguments are

+-+------------+
+v+double const+
+-+------------+


function|json|JV to be documented

.. das:function:: JV (v:string const )  : json::JsonValue?

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

arguments are

+-+------------+
+v+string const+
+-+------------+


function|json|JV to be documented

.. das:function:: read_json (text:array<uint8> const; error:string& )  : json::JsonValue?

read_json returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

arguments are

+-----+------------------+
+text +array<uint8> const+
+-----+------------------+
+error+string&           +
+-----+------------------+


function|json|read_json to be documented

.. das:function:: read_json (text:string const; error:string& )  : json::JsonValue?

read_json returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

arguments are

+-----+------------+
+text +string const+
+-----+------------+
+error+string&     +
+-----+------------+


function|json|read_json to be documented

.. das:function:: write_json (val:json::JsonValue? const )  : string

write_json returns string

arguments are

+---+-------------------------------------------------------+
+val+ :ref:`json::JsonValue <struct-json-JsonValue>` ? const+
+---+-------------------------------------------------------+


function|json|write_json to be documented


