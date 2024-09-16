
.. _stdlib_json:

=========================
JSON manipulation library
=========================

The JSON module implements JSON parser and serialization routines.
See `JHSON <www.json.org>` for details.

All functions and symbols are in "json" module, use require to get access to it. ::

    require daslib/json


++++++++++++
Type aliases
++++++++++++

.. _alias-JsValue:

.. das:attribute:: JsValue is a variant type

Single JSON element.

:Variants: * **_object** : table<string;JsonValue?>

           * **_array** : array<JsonValue?>

           * **_string** : string

           * **_number** : double

           * **_bool** : bool

           * **_null** : void?

Single JSON element.

.. _alias-Token:

.. das:attribute:: Token is a variant type

JSON input stream token.

:Variants: * **_string** : string

           * **_number** : double

           * **_bool** : bool

           * **_null** : void?

           * **_symbol** : int

           * **_error** : string

JSON input stream token.

++++++++++
Structures
++++++++++

.. _struct-json-JsonValue:

.. das:attribute:: JsonValue

:Fields: * **value** :  :ref:`JsValue <alias-JsValue>`  - JSON value, wraps any JSON element.


.. _struct-json-TokenAt:

.. das:attribute:: TokenAt

JSON parsing token. Contains token and its position.

:Fields: * **value** :  :ref:`Token <alias-Token>` 

         * **line** : int

         * **row** : int

JSON parsing token. Contains token and its position.

++++++++++++++++
Value conversion
++++++++++++++++

  *  :ref:`JV (v: string) : JsonValue? <function-_at_json_c__c_JV_Cs>` 
  *  :ref:`JV (v: double) : JsonValue? <function-_at_json_c__c_JV_Cd>` 
  *  :ref:`JV (v: bool) : JsonValue? <function-_at_json_c__c_JV_Cb>` 
  *  :ref:`JVNull () : JsonValue? <function-_at_json_c__c_JVNull>` 
  *  :ref:`JV (var v: table\<string;JsonValue?\>) : JsonValue? <function-_at_json_c__c_JV_1_ls_s_gr_2_ls_1_ls_S_ls_json_c__c_JsonValue_gr__gr__qm__gr_T>` 
  *  :ref:`JV (var v: array\<JsonValue?\>) : JsonValue? <function-_at_json_c__c_JV_1_ls_1_ls_S_ls_json_c__c_JsonValue_gr__gr__qm__gr_A>` 

.. _function-_at_json_c__c_JV_Cs:

.. das:function:: JV(v: string) : JsonValue?

Creates `JsonValue` out of value.


:Arguments: * **v** : string

.. _function-_at_json_c__c_JV_Cd:

.. das:function:: JV(v: double) : JsonValue?

|detail/function-json-JV-0x2009c03d3b2bebb2|

:Arguments: * **v** : double

.. _function-_at_json_c__c_JV_Cb:

.. das:function:: JV(v: bool) : JsonValue?

|detail/function-json-JV-0xd3c782ad29543cc6|

:Arguments: * **v** : bool

.. _function-_at_json_c__c_JVNull:

.. das:function:: JVNull() : JsonValue?

Creates `JsonValue` representing `null`.

.. _function-_at_json_c__c_JV_1_ls_s_gr_2_ls_1_ls_S_ls_json_c__c_JsonValue_gr__gr__qm__gr_T:

.. das:function:: JV(v: table<string;JsonValue?>) : JsonValue?

|detail/function-json-JV-0xd8eea79eef7f5368|

:Arguments: * **v** : table<string; :ref:`JsonValue <struct-json-JsonValue>` ?>

.. _function-_at_json_c__c_JV_1_ls_1_ls_S_ls_json_c__c_JsonValue_gr__gr__qm__gr_A:

.. das:function:: JV(v: array<JsonValue?>) : JsonValue?

|detail/function-json-JV-0x9bc30875c756208b|

:Arguments: * **v** : array< :ref:`JsonValue <struct-json-JsonValue>` ?>

++++++++++++++
Read and write
++++++++++++++

  *  :ref:`read_json (text: string implicit; var error: string&) : JsonValue? <function-_at_json_c__c_read_json_CIs_&s>` 
  *  :ref:`read_json (text: array\<uint8\>; var error: string&) : JsonValue? <function-_at_json_c__c_read_json_C1_ls_u8_gr_A_&s>` 
  *  :ref:`write_json (val: JsonValue?) : string <function-_at_json_c__c_write_json_C1_ls_S_ls_json_c__c_JsonValue_gr__gr__qm_>` 
  *  :ref:`write_json (val: JsonValue?#) : string <function-_at_json_c__c_write_json_C_hh_1_ls_S_ls_json_c__c_JsonValue_gr__gr__qm_>` 

.. _function-_at_json_c__c_read_json_CIs_&s:

.. das:function:: read_json(text: string implicit; error: string&) : JsonValue?

reads JSON from the `text` string.
if `error` is not empty, it contains the parsing error message.


:Arguments: * **text** : string implicit

            * **error** : string&

.. _function-_at_json_c__c_read_json_C1_ls_u8_gr_A_&s:

.. das:function:: read_json(text: array<uint8>; error: string&) : JsonValue?

|detail/function-json-read_json-0xac5b7e2a4f536460|

:Arguments: * **text** : array<uint8>

            * **error** : string&

.. _function-_at_json_c__c_write_json_C1_ls_S_ls_json_c__c_JsonValue_gr__gr__qm_:

.. das:function:: write_json(val: JsonValue?) : string

returns JSON (textual) representation of JsonValue as a string.


:Arguments: * **val** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-_at_json_c__c_write_json_C_hh_1_ls_S_ls_json_c__c_JsonValue_gr__gr__qm_:

.. das:function:: write_json(val: JsonValue?#) : string

Overload accepting temporary type


:Arguments: * **val** :  :ref:`JsonValue <struct-json-JsonValue>` ?#

+++++++++++++++
JSON properties
+++++++++++++++

  *  :ref:`set_no_trailing_zeros (value: bool) : bool <function-_at_json_c__c_set_no_trailing_zeros_Cb>` 
  *  :ref:`set_no_empty_arrays (value: bool) : bool <function-_at_json_c__c_set_no_empty_arrays_Cb>` 
  *  :ref:`set_allow_duplicate_keys (value: bool) : bool <function-_at_json_c__c_set_allow_duplicate_keys_Cb>` 

.. _function-_at_json_c__c_set_no_trailing_zeros_Cb:

.. das:function:: set_no_trailing_zeros(value: bool) : bool

if `value` is true, then numbers are written without trailing zeros.


:Arguments: * **value** : bool

.. _function-_at_json_c__c_set_no_empty_arrays_Cb:

.. das:function:: set_no_empty_arrays(value: bool) : bool

if `value` is true, then empty arrays are not written at all


:Arguments: * **value** : bool

.. _function-_at_json_c__c_set_allow_duplicate_keys_Cb:

.. das:function:: set_allow_duplicate_keys(value: bool) : bool

if `value` is true, then duplicate keys are allowed in objects. the later key overwrites the earlier one.


:Arguments: * **value** : bool

+++++++++++
Broken JSON
+++++++++++

  *  :ref:`try_fixing_broken_json (var bad: string) : string <function-_at_json_c__c_try_fixing_broken_json_s>` 

.. _function-_at_json_c__c_try_fixing_broken_json_s:

.. das:function:: try_fixing_broken_json(bad: string) : string

fixes broken json. so far supported
1. "string" + "string" string concatination
2. "text "nested text" text" nested quotes
3. extra , at the end of object or array
4. /uXXXXXX sequences in the middle of white space


:Arguments: * **bad** : string


