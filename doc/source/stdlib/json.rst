
.. _stdlib_json:

=========================
JSON manipulation library
=========================

++++++++++++
Type aliases
++++++++++++

.. das:attribute:: JsValue is a variant type

it defines as follows ::

 _object : table<string;json::JsonValue?>
 _array  : array<json::JsonValue?>
 _string : string
 _number : double
 _bool   : bool
 _null   : void?

to be documented

.. das:attribute:: Token is a variant type

it defines as follows ::

 _string : string
 _number : double
 _bool   : bool
 _null   : void?
 _symbol : int
 _error  : string

to be documented

++++++++++
Structures
++++++++++

.. das:attribute:: JsonValue

it defines as follows ::

  value : variant<_object:table<string;json::JsonValue?>;_array:array<json::JsonValue?>;_string:string;_number:double;_bool:bool;_null:void?>

to be documented


+++++++++
Functions
+++++++++

.. das:function:: JV(v:array<json::JsonValue?>)

JV returns json::JsonValue?



function arguments are

+-+-----------------------+
+v+array<json::JsonValue?>+
+-+-----------------------+



to be documented


.. das:function:: JV(v:void? const)

JV returns json::JsonValue?



function arguments are

+-+-----------+
+v+void? const+
+-+-----------+



to be documented


.. das:function:: JV(v:table<string;json::JsonValue?>)

JV returns json::JsonValue?



function arguments are

+-+------------------------------+
+v+table<string;json::JsonValue?>+
+-+------------------------------+



to be documented


.. das:function:: JV(v:bool const)

JV returns json::JsonValue?



function arguments are

+-+----------+
+v+bool const+
+-+----------+



to be documented


.. das:function:: JV(v:double const)

JV returns json::JsonValue?



function arguments are

+-+------------+
+v+double const+
+-+------------+



to be documented


.. das:function:: JV(v:string const)

JV returns json::JsonValue?



function arguments are

+-+------------+
+v+string const+
+-+------------+



to be documented


.. das:function:: read_json(text:array<uint8> const; error:string&)

read_json returns json::JsonValue?



function arguments are

+-----+------------------+
+text +array<uint8> const+
+-----+------------------+
+error+string&           +
+-----+------------------+



to be documented


.. das:function:: read_json(text:string const; error:string&)

read_json returns json::JsonValue?



function arguments are

+-----+------------+
+text +string const+
+-----+------------+
+error+string&     +
+-----+------------+



to be documented


.. das:function:: write_json(val:json::JsonValue? const)

write_json returns string



function arguments are

+---+----------------------+
+val+json::JsonValue? const+
+---+----------------------+



to be documented



