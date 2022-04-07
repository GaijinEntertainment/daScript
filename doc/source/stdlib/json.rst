
.. _stdlib_json:

=========================
JSON manipulation library
=========================

.. include:: detail/json.rst

The JSON module implements JSON parser and serialization routines.

All functions and symbols are in "json" module, use require to get access to it. ::

    require daslib/json


++++++++++++
Type aliases
++++++++++++

.. _alias-JsValue:

.. das:attribute:: JsValue is a variant type

+-------+------------------------+
+_object+table<string;JsonValue?>+
+-------+------------------------+
+_array +array<JsonValue?>       +
+-------+------------------------+
+_string+string                  +
+-------+------------------------+
+_number+double                  +
+-------+------------------------+
+_bool  +bool                    +
+-------+------------------------+
+_null  +void?                   +
+-------+------------------------+


|typedef-json-JsValue|

.. _alias-Token:

.. das:attribute:: Token is a variant type

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


|typedef-json-Token|

+++++++++
Constants
+++++++++

.. _global-json-Token_string:

.. das:attribute:: Token_string = 0

|variable-json-Token_string|

.. _global-json-Token_symbol:

.. das:attribute:: Token_symbol = 4

|variable-json-Token_symbol|

.. _struct-json-JsonValue:

.. das:attribute:: JsonValue



JsonValue fields are

+-----+--------------------------------+
+value+ :ref:`JsValue <alias-JsValue>` +
+-----+--------------------------------+


|structure-json-JsonValue|

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_json_c__c_JV_Cs:

.. das:function:: JV(v: string const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+-------------+
+argument+argument type+
+========+=============+
+v       +string const +
+--------+-------------+


|function-json-JV|

.. _function-_at_json_c__c_JV_Cd:

.. das:function:: JV(v: double const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+-------------+
+argument+argument type+
+========+=============+
+v       +double const +
+--------+-------------+


|function-json-JV|

.. _function-_at_json_c__c_JV_Cb:

.. das:function:: JV(v: bool const)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+-------------+
+argument+argument type+
+========+=============+
+v       +bool const   +
+--------+-------------+


|function-json-JV|

.. _function-_at_json_c__c_JVNull:

.. das:function:: JVNull()

JVNull returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

|function-json-JVNull|

.. _function-_at_json_c__c_JV_1_ls_s_gr_2_ls_1_ls_S_ls_JsonValue_gr__gr_?_gr_T:

.. das:function:: JV(v: table<string;json::JsonValue?>)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+v       +table<string; :ref:`json::JsonValue <struct-json-JsonValue>` ?>+
+--------+---------------------------------------------------------------+


|function-json-JV|

.. _function-_at_json_c__c_JV_1_ls_1_ls_S_ls_JsonValue_gr__gr_?_gr_A:

.. das:function:: JV(v: array<json::JsonValue?>)

JV returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+--------------------------------------------------------+
+argument+argument type                                           +
+========+========================================================+
+v       +array< :ref:`json::JsonValue <struct-json-JsonValue>` ?>+
+--------+--------------------------------------------------------+


|function-json-JV|

.. _function-_at_json_c__c_read_json_CIs_&s:

.. das:function:: read_json(text: string const implicit; error: string&)

read_json returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+text    +string const implicit+
+--------+---------------------+
+error   +string&              +
+--------+---------------------+


|function-json-read_json|

.. _function-_at_json_c__c_read_json_C1_ls_u8_gr_A_&s:

.. das:function:: read_json(text: array<uint8> const; error: string&)

read_json returns  :ref:`json::JsonValue <struct-json-JsonValue>` ?

+--------+------------------+
+argument+argument type     +
+========+==================+
+text    +array<uint8> const+
+--------+------------------+
+error   +string&           +
+--------+------------------+


|function-json-read_json|

.. _function-_at_json_c__c_write_json_C1_ls_S_ls_JsonValue_gr__gr_?:

.. das:function:: write_json(val: json::JsonValue? const)

write_json returns string

+--------+-------------------------------------------------------+
+argument+argument type                                          +
+========+=======================================================+
+val     + :ref:`json::JsonValue <struct-json-JsonValue>` ? const+
+--------+-------------------------------------------------------+


|function-json-write_json|


