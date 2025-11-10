
.. _stdlib_json:

=========================
JSON manipulation library
=========================

The JSON module implements JSON parser and serialization routines.
See `JSON <www.json.org>` for details.

All functions and symbols are in "json" module, use require to get access to it. ::

    require daslib/json

++++++++++++
Type aliases
++++++++++++

.. _alias-JsValue:

.. das:attribute:: variant JsValue

Single JSON element.

:Variants: * **_object** : table<string; :ref:`JsonValue <struct-json-JsonValue>` ?> - JSON object

           * **_array** : array< :ref:`JsonValue <struct-json-JsonValue>` ?> - JSON array

           * **_string** : string - JSON string

           * **_number** : double - JSON number

           * **_longint** : int64 - extension, not part of JSON standard (represents long integer numbers)

           * **_bool** : bool - JSON boolean

           * **_null** : void? - JSON null


.. _alias-Token:

.. das:attribute:: variant Token

JSON input stream token.

:Variants: * **_string** : string - string token

           * **_number** : double - number token

           * **_longint** : int64 - extension, not part of JSON standard (represents long integer numbers)

           * **_bool** : bool - boolean token

           * **_null** : void? - null token

           * **_symbol** : int - symbol token (one of []{}:,)

           * **_error** : string - error token


++++++++++
Structures
++++++++++

.. _struct-json-JsonValue:

.. das:attribute:: JsonValue

JSON value, wraps any JSON element.

:Fields: * **value** :  :ref:`JsValue <alias-JsValue>`  - value of the JSON element


.. _struct-json-TokenAt:

.. das:attribute:: TokenAt

JSON parsing token. Contains token and its position.

:Fields: * **value** :  :ref:`Token <alias-Token>`  - token value

         * **line** : int - token position in the input stream

         * **row** : int - token position in the input stream


++++++++++++++++
Value conversion
++++++++++++++++

  *  :ref:`JVNull () : JsonValue? <function-json_JVNull>` 
  *  :ref:`JV (v: string) : JsonValue? <function-json_JV_string>` 
  *  :ref:`JV (v: double) : JsonValue? <function-json_JV_double>` 
  *  :ref:`JV (v: bool) : JsonValue? <function-json_JV_bool>` 
  *  :ref:`JV (var v: table\<string, JsonValue?\>) : JsonValue? <function-json_JV_table_ls_string,_JsonValue_q__gr_>` 
  *  :ref:`JV (var v: array\<JsonValue?\>) : JsonValue? <function-json_JV_array_ls_JsonValue_q__gr_>` 
  *  :ref:`JV (v: float) : JsonValue? <function-json_JV_float>` 
  *  :ref:`JV (v: int) : JsonValue? <function-json_JV_int>` 
  *  :ref:`JV (v: bitfield) : JsonValue? <function-json_JV_bitfield>` 
  *  :ref:`JV (v: bitfield8:uint8\<\>) : JsonValue? <function-json_JV_bitfield8_c_uint8_ls__gr_>` 
  *  :ref:`JV (v: bitfield16:uint16\<\>) : JsonValue? <function-json_JV_bitfield16_c_uint16_ls__gr_>` 
  *  :ref:`JV (v: bitfield64:uint64\<\>) : JsonValue? <function-json_JV_bitfield64_c_uint64_ls__gr_>` 
  *  :ref:`JV (val: int8) : JsonValue? <function-json_JV_int8>` 
  *  :ref:`JV (val: uint8) : JsonValue? <function-json_JV_uint8>` 
  *  :ref:`JV (val: int16) : JsonValue? <function-json_JV_int16>` 
  *  :ref:`JV (val: uint16) : JsonValue? <function-json_JV_uint16>` 
  *  :ref:`JV (val: uint) : JsonValue? <function-json_JV_uint>` 
  *  :ref:`JV (val: int64) : JsonValue? <function-json_JV_int64>` 
  *  :ref:`JV (val: uint64) : JsonValue? <function-json_JV_uint64>` 

.. _function-json_JVNull:

.. das:function:: JVNull() : JsonValue?

Creates `JsonValue` representing `null`.

.. _function-json_JV_string:

.. das:function:: JV(v: string) : JsonValue?

Creates `JsonValue` out of string value.

:Arguments: * **v** : string

.. _function-json_JV_double:

.. das:function:: JV(v: double) : JsonValue?

Creates `JsonValue` out of double value.

:Arguments: * **v** : double

.. _function-json_JV_bool:

.. das:function:: JV(v: bool) : JsonValue?

Creates `JsonValue` out of boolean value.

:Arguments: * **v** : bool

.. _function-json_JV_table_ls_string,_JsonValue_q__gr_:

.. das:function:: JV(v: table<string, JsonValue?>) : JsonValue?

Creates `JsonValue` out of object (table string->JsonValue?) value.

:Arguments: * **v** : table<string; :ref:`JsonValue <struct-json-JsonValue>` ?>

.. _function-json_JV_array_ls_JsonValue_q__gr_:

.. das:function:: JV(v: array<JsonValue?>) : JsonValue?

Creates `JsonValue` out of array of `JsonValue?` value.

:Arguments: * **v** : array< :ref:`JsonValue <struct-json-JsonValue>` ?>

.. _function-json_JV_float:

.. das:function:: JV(v: float) : JsonValue?

Creates `JsonValue` out of float value.

:Arguments: * **v** : float

.. _function-json_JV_int:

.. das:function:: JV(v: int) : JsonValue?

Creates `JsonValue` out of int value.

:Arguments: * **v** : int

.. _function-json_JV_bitfield:

.. das:function:: JV(v: bitfield) : JsonValue?

Creates `JsonValue` out of bitfield value.

:Arguments: * **v** : bitfield<>

.. _function-json_JV_bitfield8_c_uint8_ls__gr_:

.. das:function:: JV(v: bitfield8:uint8<>) : JsonValue?

Creates `JsonValue` out of bitfield8 value.

:Arguments: * **v** : bitfield : uint8<>

.. _function-json_JV_bitfield16_c_uint16_ls__gr_:

.. das:function:: JV(v: bitfield16:uint16<>) : JsonValue?

Creates `JsonValue` out of bitfield16 value.

:Arguments: * **v** : bitfield : uint16<>

.. _function-json_JV_bitfield64_c_uint64_ls__gr_:

.. das:function:: JV(v: bitfield64:uint64<>) : JsonValue?

Creates `JsonValue` out of bitfield64 value.

:Arguments: * **v** : bitfield : uint64<>

.. _function-json_JV_int8:

.. das:function:: JV(val: int8) : JsonValue?

Creates `JsonValue` out of int8 value.

:Arguments: * **val** : int8

.. _function-json_JV_uint8:

.. das:function:: JV(val: uint8) : JsonValue?

Creates `JsonValue` out of uint8 value.

:Arguments: * **val** : uint8

.. _function-json_JV_int16:

.. das:function:: JV(val: int16) : JsonValue?

Creates `JsonValue` out of int16 value.

:Arguments: * **val** : int16

.. _function-json_JV_uint16:

.. das:function:: JV(val: uint16) : JsonValue?

Creates `JsonValue` out of uint16 value.

:Arguments: * **val** : uint16

.. _function-json_JV_uint:

.. das:function:: JV(val: uint) : JsonValue?

Creates `JsonValue` out of uint value.

:Arguments: * **val** : uint

.. _function-json_JV_int64:

.. das:function:: JV(val: int64) : JsonValue?

Creates `JsonValue` out of int64 value.

:Arguments: * **val** : int64

.. _function-json_JV_uint64:

.. das:function:: JV(val: uint64) : JsonValue?

Creates `JsonValue` out of uint64 value.

:Arguments: * **val** : uint64

++++++++++++++
Read and write
++++++++++++++

  *  :ref:`read_json (text: string implicit; var error: string&) : JsonValue? <function-json_read_json_string_implicit_string>` 
  *  :ref:`read_json (text: array\<uint8\>; var error: string&) : JsonValue? <function-json_read_json_array_ls_uint8_gr__string>` 
  *  :ref:`write_json (val: JsonValue?) : string <function-json_write_json_JsonValue_q_>` 
  *  :ref:`write_json (val: JsonValue?#) : string <function-json_write_json_JsonValue_q__hh_>` 

.. _function-json_read_json_string_implicit_string:

.. das:function:: read_json(text: string implicit; error: string&) : JsonValue?

reads JSON from the `text` string.
if `error` is not empty, it contains the parsing error message.

:Arguments: * **text** : string implicit

            * **error** : string&

.. _function-json_read_json_array_ls_uint8_gr__string:

.. das:function:: read_json(text: array<uint8>; error: string&) : JsonValue?

reads JSON from the `text` array of uint8.
if `error` is not empty, it contains the parsing error message.

:Arguments: * **text** : array<uint8>

            * **error** : string&

.. _function-json_write_json_JsonValue_q_:

.. das:function:: write_json(val: JsonValue?) : string

returns JSON (textual) representation of JsonValue as a string.

:Arguments: * **val** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-json_write_json_JsonValue_q__hh_:

.. das:function:: write_json(val: JsonValue?#) : string

Overload accepting temporary type

:Arguments: * **val** :  :ref:`JsonValue <struct-json-JsonValue>` ?#

+++++++++++++++
JSON properties
+++++++++++++++

  *  :ref:`set_no_trailing_zeros (value: bool) : bool <function-json_set_no_trailing_zeros_bool>` 
  *  :ref:`set_no_empty_arrays (value: bool) : bool <function-json_set_no_empty_arrays_bool>` 
  *  :ref:`set_allow_duplicate_keys (value: bool) : bool <function-json_set_allow_duplicate_keys_bool>` 

.. _function-json_set_no_trailing_zeros_bool:

.. das:function:: set_no_trailing_zeros(value: bool) : bool

if `value` is true, then numbers are written without trailing zeros.

:Arguments: * **value** : bool

.. _function-json_set_no_empty_arrays_bool:

.. das:function:: set_no_empty_arrays(value: bool) : bool

if `value` is true, then empty arrays are not written at all

:Arguments: * **value** : bool

.. _function-json_set_allow_duplicate_keys_bool:

.. das:function:: set_allow_duplicate_keys(value: bool) : bool

if `value` is true, then duplicate keys are allowed in objects. the later key overwrites the earlier one.

:Arguments: * **value** : bool

+++++++++++
Broken JSON
+++++++++++

  *  :ref:`try_fixing_broken_json (var bad: string) : string <function-json_try_fixing_broken_json_string>` 

.. _function-json_try_fixing_broken_json_string:

.. das:function:: try_fixing_broken_json(bad: string) : string

fixes broken json. so far supported
1. "string" + "string" string concatenation
2. "text "nested text" text" nested quotes
3. extra , at the end of object or array
4. /uXXXXXX sequences in the middle of white space

:Arguments: * **bad** : string


